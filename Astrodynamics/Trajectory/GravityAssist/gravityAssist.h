/*! \file gravityAssist.h
 *    This header file contains a base class for the gravity assist method.
 *
 *    Path              : /Astrodynamics/Trajectory/GravityAssist/
 *    Version           : 7
 *    Check status      : Checked
 *
 *    Author            : E. Iorfida
 *    Affiliation       : Delft University of Technology
 *    E-mail address    : elisabetta_iorfida@yahoo.it
 *
 *    Checker           : J. Melman
 *    Affiliation       : Delft University of Technology
 *    E-mail address    : J.C.P.Melman@tudelft.nl
 *
 *    Date created      : 17 January, 2011
 *    Last modified     : 12 February, 2011
 *
 *    References
 *
 *    Notes
 *      Gravity assist and swing-by are different words for the same thing.
 *      The delta-V that is computed for a powered swing-by has not been
 *      proven to be the optimum (lowest) to achieve the desired geometry
 *      of incoming and outgoing hyperbolic legs.
 *      For the moment in this code the radius of the central body, and the
 *      smallestPeriapsisDistanceFactor are given as external input by the
 *      user, but they should be part of the CelestialBody object.
 *      Also, the velocity of the central body will need to be computed by
 *      the ephemeris code.
 *      At the moment, this code uses a Newton-Raphson root finder by default.
 *      In the future it should be possible to apply for example the Halley
 *      method by using polymorphism.
 *
 *    Copyright (c) 2010 Delft University of Technology.
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
 *      YYMMDD    author        comment
 *      110117    E. Iorfida    First creation of code.
 *      110128    E. Iorfida    Added member variables necessary for the
 *                              unit tests.
 *      110202    J. Melman     Changed several names and suggested
 *                              minor restructuring of class.
 *      110203    E. Iorfida    Changed some variable names and modified
 *                              punctuation.
 *      110205    J. Melman     Removed the trailing underscores in some
 *                              public variables. Changed and added some notes.
 *      110208    E. Iorfida    Added CartesianVelocityElements objects as input.
 *                              Deleted inheritance from TrajectoryDesingMethods.
 *      110212    J. Melman     Made delta-V private. getDeltaV changed into
 *                              computeDeltaV.
 */

#ifndef GRAVITYASSIST_H
#define GRAVITYASSIST_H

// Include statements.
# include "newtonRaphson.h"
# include "newtonRaphsonAdaptor.h"
# include "unitConversions.h"
# include "basicMathematicsFunctions.h"
# include "celestialBody.h"
# include "cartesianVelocityElements.h"

//! Gravity assist method class.
/*!
 * Gravity assist method class.
 */
class GravityAssist
{
public:

    //! Default constructor.
    /*!
     * Default constructor.
     */
    GravityAssist( );

    //! Default destructor.
    /*!
     * Default destructor.
     */
    virtual ~GravityAssist( );

    // TEMPORARY!! Needs to be part of CelestialBody object.
    //! Set radius of the swing-by central body.
    /*!
     * This function sets the radius of the central body involved in the swing-by.
     * \param BodyRadius.
     */
    void setCentralBodyRadius( const double& centralBodyRadius );

    //! Set central body of the swing-by.
    /*!
     * Sets pointer to central body of the swing-by.
     * \param pointerToCentralBody Central body of the swing-by.
     */
    void setCentralBody( CelestialBody* pointerToCentralBody );

    //! Set velocity of the swing-by central body.
    /*!
     * Sets the velocity of the central body involved in the swing-by,
     * which will come from the Ephemeris class.
     * \param centralBodyVelocity Velocity of swing-by central body.
     */
    void setCentralBodyVelocity( Vector3d centralBodyVelocity );

    // TEMPORARY!! Needs to be part of CelestialBody object.
    //! Set smallest periapsis distance factor.
    /*!
     * Sets the smallest allowable periapsis distance factor that has to be
     * multiplied with the central body radius to find the smallest allowable
     * periapsis distance.
     * \param smallestPeriapsisDistanceFactor Smallest periapsis distance factor.
     */
    void setSmallestPeriapsisDistanceFactor( const double&
                                             smallestPeriapsisDistanceFactor );

    //! Set pointer to incoming velocity of the satellite.
    /*!
     * Sets pointer to the incoming velocity vector of the satellite.
     * \param pointerToIncomingVelocity Pointer to incoming velocity of the satellite.
     */
    void setPointerToIncomingVelocity( CartesianVelocityElements* pointerToIncomingVelocity );

    //! Set pointer to outgoing velocity of the satellite.
    /*!
     * Sets pointer to the outgoing velocity of the satellite.
     * \param pointerToOutgoingVelocity Pointer to outgoing velocity of the satellite.
     */
    void setPointerToOutgoingVelocity( CartesianVelocityElements* pointerToOutgoingVelocity );

    //! Set pointer to Newton-Raphson method for gravity assist algorithm.
    /*!
     * Sets a pointer to the Newton-Raphson method for gravity assist algorithm.
     * \param pointerToNewtonRaphsonMethod Pointer to NewtonRaphson object.
     */
    void setNewtonRaphsonMethod( NewtonRaphson* pointerToNewtonRaphson );

    //! Compute the delta-V of a powered swing-by.
    /*!
     * Computes the necessary delta-V to perform a powered swing-by.
     * \return deltaV Necessary delta-V of swing-by.
     */
    const double& computeDeltaV( );

    //! Define root-finder function.
    /*!
     * Defines root-finder function for the velocity-effect delta-V.
     * \param incomingEccentricity Incoming eccentricity.
     * \return Value of root-finder function at defined eccentricity.
     */
    double velocityEffectFunction( double& incomingEccentricity );

    //! Define first derivative of root-finder function.
    /*!
     * Defines first derivative of root-finder function for the velocity-effect
     * delta-V.
     * \param incomingEccentricity Incoming eccentricity.
     * \return Value of first derivative of root-finder function at defined
     * eccentricity.
     */
    double firstDerivativeVelocityEffectFunction( double&
                                                  incomingEccentricity );

    //! Eccentricity of the incoming hyperbolic leg.
    /*!
     * Eccentricity of the incoming hyperbolic leg.
     */
    double incomingEccentricity;

    //! Eccentricity of the outgoing hyperbolic leg.
    /*!
     * Eccentricity of the outgoing hyperbolic leg.
     */
    double outgoingEccentricity;

    //! Boolean to set whether the root finder is required for checking.
    /*!
     * Boolean to set whether the root finder is required for checking.
     */
    bool isRootFinderRequiredForChecking;

protected:

    //! Radius of the swing-by central body.
    /*!
     * Radius of the central body involved in the swing-by.
     */
    double centralBodyRadius_;

    //! Pointer to CelestialBody class for swing-by.
    /*!
     * Pointer to CelestialBody class for swing-by.
     */
    CelestialBody* pointerToCentralBody_;

    //! Velocity of the swing-by central body.
    /*!
     * Velocity vector of the central body involved in the swing-by.
     */
    Vector3d centralBodyVelocity_;

    //! Smallest periapsisDistanceFactor.
    /*!
     * The smallest allowable periapsis distance factor
     * that is to be multiplied with the central body radius to find the
     * smallest allowable periapsis distance.
     */
    double smallestPeriapsisDistanceFactor_;

    //! Pointer to CartesianVelocityElements object.
    /*!
     * Pointer to CartesianVelocityElements object.
     */
    CartesianVelocityElements* pointerToIncomingVelocity_;

    //! Pointer to CartesianVelocityElements object.
    /*!
     * Pointer to CartesianVelocityElements object.
     */
    CartesianVelocityElements* pointerToOutgoingVelocity_;

    //! Hyperbolic excess velocity of the incoming leg.
    /*!
     * Hyperbolic excess velocity of the incoming leg.
     */
    Vector3d incomingHyperbolicExcessVelocity_;

    //! Hyperbolic excess velocity of the outgoing leg.
    /*!
     * Hyperbolic excess velocity of the outgoing leg.
     */
    Vector3d outgoingHyperbolicExcessVelocity_;

    //! Delta-V of powered gravity assist.
    /*!
     * Necessary delta-V to perform a powered swing-by.
     */
    double deltaV_;

    //! Bending angle between the excess velocities.
    /*!
     * Bending angle between the excess velocities.
     */
    double bendingAngle_;

    //! Semi-major axis of the incoming hyperbolic leg.
    /*!
     *  Semi-major axis of the incoming hyperbolic leg.
     */
    double incomingSemiMajorAxis_;

    //! Semi-major axis of the outgoing hyperbolic leg.
    /*!
     * Semi-major axis of the outgoing hyperbolic leg.
     */
    double outgoingSemiMajorAxis_;

    //! Delta-V due to the effect of large bending angle.
    /*!
     * Delta-V due to the effect of large bending angle between excess
     * velocities.
     */
    double bendingEffectDeltaV_;

    //! Delta-V due to the effect of different excess speeds.
    /*!
     * Delta-V due to the effect of different excess speeds.
     */
    double velocityEffectDeltaV_;

    //! Pointer to object of NewtonRaphson class.
    /*!
     * Pointer to object of NewtonRaphson class.
     */
    NewtonRaphson* pointerToNewtonRaphson_;

    //! Pointer to adaptor object of NewtonRaphsonAdaptor class.
    /*!
     * Pointer to adaptor object of NewtonRaphsonAdaptor class. The template
     * parameter passed is this class.
     */
    NewtonRaphsonAdaptor< GravityAssist >
            newtonRaphsonAdaptorForGravityAssist_;

private:
};

#endif // GRAVITYASSIST_H

// End of file.