 #    Copyright (c) 2010-2017, Delft University of Technology
 #    All rigths reserved
 #
 #    This file is part of the Tudat. Redistribution and use in source and
 #    binary forms, with or without modification, are permitted exclusively
 #    under the terms of the Modified BSD license. You should have received
 #    a copy of the license with this file. If not, please or visit:
 #    http://tudat.tudelft.nl/LICENSE.

# Set the source files.
set(ACCELERATION_PARTIALS_SOURCES
  "${SRCROOT}${ACCELERATIONPARTIALSDIR}/accelerationPartial.cpp"
  "${SRCROOT}${ACCELERATIONPARTIALSDIR}/centralGravityAccelerationPartial.cpp"
  "${SRCROOT}${ACCELERATIONPARTIALSDIR}/numericalAccelerationPartial.cpp"
  "${SRCROOT}${ACCELERATIONPARTIALSDIR}/radiationPressureAccelerationPartial.cpp"
  "${SRCROOT}${ACCELERATIONPARTIALSDIR}/sphericalHarmonicPartialFunctions.cpp"
  "${SRCROOT}${ACCELERATIONPARTIALSDIR}/sphericalHarmonicAccelerationPartial.cpp"
)

# Set the header files.
set(ACCELERATION_PARTIALS_HEADERS
  "${SRCROOT}${ACCELERATIONPARTIALSDIR}/accelerationPartial.h"
  "${SRCROOT}${ACCELERATIONPARTIALSDIR}/thirdBodyGravityPartial.h"
  "${SRCROOT}${ACCELERATIONPARTIALSDIR}/centralGravityAccelerationPartial.h"
  "${SRCROOT}${ACCELERATIONPARTIALSDIR}/numericalAccelerationPartial.h"
  "${SRCROOT}${ACCELERATIONPARTIALSDIR}/radiationPressureAccelerationPartial.h"
  "${SRCROOT}${ACCELERATIONPARTIALSDIR}/sphericalHarmonicPartialFunctions.h"
  "${SRCROOT}${ACCELERATIONPARTIALSDIR}/sphericalHarmonicAccelerationPartial.h"
)

# Add static libraries.
add_library(tudat_acceleration_partials STATIC ${ACCELERATION_PARTIALS_SOURCES} ${ACCELERATION_PARTIALS_HEADERS})
setup_tudat_library_target(tudat_acceleration_partials "${SRCROOT}{ACCELERATIONPARTIALSDIR}")

if(USE_CSPICE)

# Add unit tests
add_executable(test_AccelerationPartials "${SRCROOT}${ACCELERATIONPARTIALSDIR}/UnitTests/unitTestAccelerationPartials.cpp")
setup_custom_test_program(test_AccelerationPartials "${SRCROOT}${ACCELERATIONPARTIALSDIR}")
target_link_libraries(test_AccelerationPartials ${TUDAT_ESTIMATION_LIBRARIES} ${Boost_LIBRARIES})

# Add unit tests
add_executable(test_SphericalHarmonicPartials "${SRCROOT}${ACCELERATIONPARTIALSDIR}/UnitTests/unitTestSphericalHarmonicPartials.cpp")
setup_custom_test_program(test_SphericalHarmonicPartials "${SRCROOT}${ACCELERATIONPARTIALSDIR}")
target_link_libraries(test_SphericalHarmonicPartials ${TUDAT_ESTIMATION_LIBRARIES} ${Boost_LIBRARIES})

endif( )


