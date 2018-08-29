/*    Copyright (c) 2010-2018, Delft University of Technology
 *    All rigths reserved
 *
 *    This file is part of the Tudat. Redistribution and use in source and
 *    binary forms, with or without modification, are permitted exclusively
 *    under the terms of the Modified BSD license. You should have received
 *    a copy of the license with this file. If not, please or visit:
 *    http://tudat.tudelft.nl/LICENSE.
 */

#ifndef TUDAT_JSONESTIMATIONINTERFACE_H
#define TUDAT_JSONESTIMATIONINTERFACE_H

#include "Tudat/JsonInterface/jsonInterface.h"
#include "Tudat/SimulationSetup/tudatEstimationHeader.h"

namespace tudat
{

namespace json_interface
{



//! Class for managing JSON-based simulations.
template< typename TimeType = double, typename StateScalarType = double >
class JsonEstimationManager: public JsonSimulationManager< TimeType, StateScalarType >
{

public:
    //! Constructor from JSON file.
    /*!
     * Constructor.
     * \param inputFilePath Path to the root JSON input file. Can be absolute or relative (to the working directory).
     * \param initialClockTime Initial clock time from which the cummulative CPU time during the propagation will be
     * computed. Default is the moment at which the constructor was called.
     */
    JsonEstimationManager(
            const std::string& inputFilePath,
            const std::chrono::steady_clock::time_point initialClockTime = std::chrono::steady_clock::now( ) ):
        JsonSimulationManager( inputFilePath, initialClockTime )
    { }

    //! Constructor from JSON object.
    /*!
     * Constructor.
     * \param jsonObject The root JSON object.
     * \param initialClockTime Initial clock time from which the cummulative CPU time during the propagation will be
     * computed. Default is the moment at which the constructor was called.
     */
    JsonEstimationManager(
            const nlohmann::json& jsonObject,
            const std::chrono::steady_clock::time_point initialClockTime = std::chrono::steady_clock::now( ) ):
        JsonSimulationManager( jsonObject, initialClockTime ){ }

    ~JsonEstimationManager( ){ }

    virtual void updateSettingsDerived( )
    {
        resetObservationSettings( );
        resetEstimationSettings( );
    }

    virtual void runEstimation( )
    {
        // Check if any keys in jsonObject_ haven't been used
        checkUnusedKeys( jsonObject_, applicationOptions_->unusedKey_ );

        if ( profiling )
        {
            std::cout << "checkUnusedKeys: " << std::chrono::duration_cast< std::chrono::milliseconds >(
                             std::chrono::steady_clock::now( ) - initialClockTime_ ).count( ) * 1.0e-3 << " s" << std::endl;
            initialClockTime_ = std::chrono::steady_clock::now( );
        }

        // Export full settings JSON file if requested
        if ( ! applicationOptions_->fullSettingsFile_.empty( ) )
        {
            exportAsJson( applicationOptions_->fullSettingsFile_ );
        }

        // Print message on propagation start if requested
        if ( applicationOptions_->notifyOnPropagationStart_ )
        {
            std::cout << "Estimation of file " << inputFilePath_ << " started." << std::endl;
        }

        orbitDeterminationManager_->estimateParameters(
                    podInput_, convergenceChecker_ );

//        // Print message on propagation termination if requested
//        if ( applicationOptions_->notifyOnPropagationTermination_ )
//        {
//            if ( dynamicsSimulator_->integrationCompletedSuccessfully( ) )
//            {
//                std::cout << "SUCCESS: propagation of file " << inputFilePath_ << " terminated with no errors."
//                          << std::endl;
//            }
//            else
//            {
//                std::cout << "FAILURE: propagation of file " << inputFilePath_ << " terminated with errors."
//                          << std::endl;
//            }
//        }

        if ( profiling )
        {
            std::cout << "run: " << std::chrono::duration_cast< std::chrono::milliseconds >(
                             std::chrono::steady_clock::now( ) - initialClockTime_ ).count( ) * 1.0e-3 << " s" << std::endl;
            initialClockTime_ = std::chrono::steady_clock::now( );
        }
    }

    //! Export the results of the dynamics simulation according to the export settings.
    /*!
     * @copybrief exportResults
     */
    void exportEstimationResults( )
    {

    }

protected:

    virtual void resetObservationSettings( )
    {
        updateFromJSON( observationSettingsMap_, jsonObject_, Keys::observations );

        if ( profiling )
        {

            std::cout << "resetObservationSettings: " << std::chrono::duration_cast< std::chrono::milliseconds >(
                             std::chrono::steady_clock::now( ) - initialClockTime_ ).count( ) * 1.0e-3 << " s" << std::endl;
            initialClockTime_ = std::chrono::steady_clock::now( );
        }
    }

    void resetEstimationSettings( )
    {
        updateFromJSON( podSettings_, jsonObject_, Keys::estimationSettings );

        if ( profiling )
        {
            std::cout << "resetEstimationSettings: " << std::chrono::duration_cast< std::chrono::milliseconds >(
                             std::chrono::steady_clock::now( ) - initialClockTime_ ).count( ) * 1.0e-3 << " s" << std::endl;
            initialClockTime_ = std::chrono::steady_clock::now( );
        }
    }


    //! Reset dynamicsSimulator_ for the current bodyMap_, integratorSettings_ and propagatorSettings_.
    /*!
     * @copybrief resetDynamicsSimulator
     */
    virtual void resetDynamicsSimulator( )
    {
        parseSettingsObjects( );
    }

    virtual void resetVariationalEquationsSolver( )
    {
        parseSettingsObjects( );
    }

    virtual void parseSettingsObjects( )
    {
        orbitDeterminationManager_ =
                boost::make_shared< simulation_setup::OrbitDeterminationManager< StateScalarType, TimeType > >(
                    bodyMap_, parametersToEstimate_, observationSettingsMap_, integratorSettings_, propagatorSettings_,
                    false );
        variationalEquationsSolver_ = orbitDeterminationManager_->getVariationalEquationsSolver( );
        dynamicsSimulator_ = variationalEquationsSolver_->getDynamicsSimulator( );
    }

private:

    observation_models::ObservationSettingsMap observationSettingsMap_;

    boost::shared_ptr< simulation_setup::OrbitDeterminationManager< StateScalarType, TimeType > > orbitDeterminationManager_;

    std::shared_ptr< simulation_setup::PodSettings > podSettings_;

};


} // namespace json_interface

} // namespace tudat

#endif // TUDAT_JSONESTIMATIONINTERFACE_H