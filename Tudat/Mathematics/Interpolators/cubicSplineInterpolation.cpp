/*    Copyright (c) 2010-2012 Delft University of Technology.
 *
 *    This software is protected by national and international copyright.
 *    Any unauthorized use, reproduction or modification is unlawful and
 *    will be prosecuted. Commercial and non-private application of the
 *    software in any form is strictly prohibited unless otherwise granted
 *    by the authors.
 *
 *    The code is provided without any warranty; without even the implied
 *    warranty of merchantibility or fitness for a particular purpose.
 *
 *    Changelog
 *      YYMMDD    Author            Comment
 *                S. van Doorn      Original code from EPM simulator.
 *      110620    F.M. Engelen      File created and converted to Tudat standards.
 *      110707    E.A.G. Heeren     Minor spelling/lay-out corrections.
 *      110714    E.A.G. Heeren     Minor spelling/lay-out corrections.
 *      110905    S. Billemont      Reorganized includes.
 *                                  Moved (con/de)structors and getter/setters to header.
 *
 *    References
 *    Numerical Recipes Third Edition - W.H. Press - page 118
 *
 */

#include <iostream>
#include "Tudat/Mathematics/Interpolators/cubicSplineInterpolation.h"
#include "Tudat/Mathematics/BasicMathematics/nearestNeighbourSearch.h"

namespace tudat
{
namespace mathematics
{
namespace interpolators
{

//! Initialize cubic spline interpolation.
void CubicSplineInterpolation::initializeCubicSplineInterpolation(
        Eigen::VectorXd& independentVariables, Eigen::VectorXd& dependentVariables )
{
    // Set the vectors.
    independentVariables_ = independentVariables;
    dependentVariables_ = dependentVariables;

    // Call functions to initialize tridiagonal matrix and y'' (second derivative of curvature).
    constructTridiagonalMatrix_( );
    computeSecondDerivativeOfCurvature_( );
}

//! Interpolate a point.
double CubicSplineInterpolation::interpolate( double targetIndependentVariableValue )
{
    if ( independentVariables_.size( ) == 0 || dependentVariables_.size( ) == 0 )
    {
        std::cerr << "The vectors used in the cubic spline interpolation are empty." << std::endl;
    }

    // Set independent variable value.
    targetIndependentVariableValue_ = targetIndependentVariableValue;

    // Determine the lower entry in the table corresponding to the target independent variable
    // value.
    lowerEntry_ = tudat::basic_mathematics::computeNearestLeftNeighborUsingBinarySearch(
            independentVariables_, targetIndependentVariableValue_ );

    // Calculate coefficients A,B,C,D.
    coefficientA_ = ( independentVariables_( lowerEntry_ + 1 ) - targetIndependentVariableValue_ )
            / ( independentVariables_( lowerEntry_ + 1 ) - independentVariables_( lowerEntry_ ) );
    coefficientB_ = 1 - coefficientA_;
    coefficientC_ = ( pow( coefficientA_, 3 ) - coefficientA_ ) /
            6.0 * pow( ( independentVariables_( lowerEntry_ + 1 )
                         - independentVariables_( lowerEntry_ ) ), 2 );
    coefficientD_ = ( pow( coefficientB_, 3 ) - coefficientB_ ) /
            6.0 * pow( ( independentVariables_( lowerEntry_ + 1 )
                         - independentVariables_( lowerEntry_ ) ) , 2 );

    // The interpolated dependent variable value.
    return coefficientA_ * dependentVariables_( lowerEntry_ ) +
            coefficientB_ * dependentVariables_( lowerEntry_ + 1 ) +
            coefficientC_ * secondDerivativeOfCurvature_( lowerEntry_ ) +
            coefficientD_ * secondDerivativeOfCurvature_( lowerEntry_ + 1 );
}

//! Initialize tridiagonal matrix.
void CubicSplineInterpolation::constructTridiagonalMatrix_( )
{
    // Get length of vector.
    numberOfDataPoints_ = independentVariables_.size( );

    // Initialize vectors for calculating y''.
    hCoefficients_.setZero( numberOfDataPoints_ - 1 );
    aCoefficients_.setZero( numberOfDataPoints_ - 2 );
    bCoefficients_.setZero( numberOfDataPoints_ - 2 );
    cCoefficients_.setZero( numberOfDataPoints_ - 2 );
    rCoefficients_.setZero( numberOfDataPoints_ - 2 );

    // Set to zero as they fall outside the TriDiagonal matrix.
    aCoefficients_( 0 ) = 0.0;
    cCoefficients_( numberOfDataPoints_- 3 ) = 0.0;

    // Compute the vectors h,a,c,b,r.
    for ( unsigned int i = 0; i < ( numberOfDataPoints_ - 1 ); i++ )
    {
        hCoefficients_( i ) = independentVariables_( i + 1 ) - independentVariables_( i );
    }

    for ( unsigned int i = 0; i < ( numberOfDataPoints_ - 3 ); i++ )
    {
        aCoefficients_( i + 1 ) = hCoefficients_( i + 1 );
        cCoefficients_( i )   = hCoefficients_( i + 1 );
    }

    for ( unsigned int i = 0; i < ( numberOfDataPoints_ - 2 ); i++ )
    {
        bCoefficients_( i ) = 2.0 * ( hCoefficients_( i + 1 ) + hCoefficients_( i ) );
        rCoefficients_( i ) = 6.0 * ( ( dependentVariables_( i + 2 )
                                        - dependentVariables_( i + 1 ) )
                                      / hCoefficients_( i + 1 )
                                      - ( dependentVariables_( i + 1 ) -
                                          dependentVariables_( i ) ) / hCoefficients_( i ) );
    }
}

//! Compute second derivative of curvature.
void CubicSplineInterpolation::computeSecondDerivativeOfCurvature_( )
{
    // Initialise g, y'' intermediate and y'' vector.
    gCoefficients_.setZero( numberOfDataPoints_ - 2 );
    intermediateSecondDerivativeOfCurvature_.setZero( numberOfDataPoints_ - 2 );
    secondDerivativeOfCurvature_.setZero( numberOfDataPoints_ );

    // Algorithm to calculate y'' intermediate.
    coefficientb2_ = bCoefficients_( 0 );
    intermediateSecondDerivativeOfCurvature_( 0 ) = rCoefficients_( 0 ) / ( coefficientb2_ );

    for ( unsigned int i = 1; i < ( numberOfDataPoints_ - 2 ); i++ )
    {
        gCoefficients_( i ) = cCoefficients_( i - 1 ) / coefficientb2_;
        coefficientb2_ = bCoefficients_( i )- aCoefficients_( i ) * gCoefficients_( i );
        intermediateSecondDerivativeOfCurvature_( i ) = (
                rCoefficients_( i ) -
                aCoefficients_( i ) * intermediateSecondDerivativeOfCurvature_( i - 1 ) )
                                                       / coefficientb2_;
    }

    for ( int i = ( numberOfDataPoints_ - 4 ); i >= 0; i-- )
    {
        intermediateSecondDerivativeOfCurvature_( i ) -=
                gCoefficients_( i + 1 ) * intermediateSecondDerivativeOfCurvature_( i + 1 );
    }

    // Set y''.
    secondDerivativeOfCurvature_ << 0.0, intermediateSecondDerivativeOfCurvature_, 0.0;
}

} // namespace interpolators
} // namespace mathematics
} // namespace tudat