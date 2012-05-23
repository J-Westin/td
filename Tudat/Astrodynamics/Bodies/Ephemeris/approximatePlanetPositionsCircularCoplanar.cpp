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
 *      110629    L. van der Ham    First creation of code.
 *      110803    L. van der Ham    Seperated this code from approximatePlanetPositions.
 *      120322    D. Dirkx          Modified to new Ephemeris interfaces.
 *
 *    References
 *      Standish, E.M. Keplerian Elements for Approximate Positions of the Major Planets,
 *          http://ssd.jpl.nasa.gov/txt/aprx_pos_planets.pdf, last accessed: 24 February, 2011.
 *
 */

#include <cmath>

#include <TudatCore/Astrodynamics/BasicAstrodynamics/unitConversions.h>
#include <TudatCore/Mathematics/BasicMathematics/coordinateConversions.h>
#include <TudatCore/Mathematics/BasicMathematics/mathematicalConstants.h>

#include "Tudat/Astrodynamics/Bodies/Ephemeris/approximatePlanetPositionsCircularCoplanar.h"
#include "Tudat/Astrodynamics/Bodies/planet.h"

namespace tudat
{
namespace ephemerides
{

//! Get state from ephemeris; circular, coplanar case
Eigen::VectorXd ApproximatePlanetPositionsCircularCoplanar::getCartesianStateFromEphemeris(
        const double julianDate )
{
    // Set Julian date.
    julianDate_ = julianDate;

    // Compute number of centuries past J2000.
    numberOfCenturiesPastJ2000_ = ( julianDate_ - 2451545.0 ) / 36525.0;

    // Compute mean longitude of planet at given Julian date.
    meanLongitudeAtGivenJulianDate_ = approximatePlanetPositionsDataContainer_.meanLongitude_
            + ( approximatePlanetPositionsDataContainer_.rateOfChangeOfMeanLongitude_
                * numberOfCenturiesPastJ2000_ );

    // Convert mean longitude at given Julian date from degrees to radians.
    meanLongitudeAtGivenJulianDate_ = unit_conversions::convertDegreesToRadians(
                meanLongitudeAtGivenJulianDate_);

    // Get semi-major axis at J2000 and assume constant radius of circular orbit.
    constantOrbitalRadius_ = unit_conversions::convertAstronomicalUnitsToMeters(
                approximatePlanetPositionsDataContainer_.semiMajorAxis_ );

    // Convert to Cartesian position.
    Eigen::VectorXd planetCartesianStateAtGivenJulianDate( 6 );
    planetCartesianStateAtGivenJulianDate.segment( 0, 3 ) = mathematics::coordinate_conversions::
            convertSphericalToCartesian( Eigen::Vector3d( constantOrbitalRadius_,
                                                          0.5 * mathematics::PI,
                                                          meanLongitudeAtGivenJulianDate_ ) );

    // Create predefined Sun.
    bodies::Planet predefinedSun_;
    predefinedSun_.setPredefinedPlanetSettings( bodies::Planet::sun );

    // Compute orbital velocity.
    double circularOrbitalVelocity = std::sqrt( predefinedSun_.getGravityFieldModel( )->
                     getGravitationalParameter( ) / constantOrbitalRadius_ );

    // Convert to Cartesian velocity.
    planetCartesianStateAtGivenJulianDate( 3 ) = -sin( meanLongitudeAtGivenJulianDate_ ) *
            circularOrbitalVelocity;
    planetCartesianStateAtGivenJulianDate( 4 ) = cos( meanLongitudeAtGivenJulianDate_ ) *
            circularOrbitalVelocity;
    planetCartesianStateAtGivenJulianDate( 5 ) = 0.0;

    // Return Cartesian state of planet at given Julian date.
    return planetCartesianStateAtGivenJulianDate;
}

} // namespace ephemerides
} // namespace tudat