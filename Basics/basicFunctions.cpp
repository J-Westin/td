/*! \file basicFunctions.cpp
 *    Source file that defines the basicFunctions namespace, containing all
 *    basic functions contained in Tudat.
 *
 *    Path              : /Basics/
 *    Version           : 4
 *    Check status      : Checked
 *
 *    Author            : K. Kumar
 *    Affiliation       : Delft University of Technology
 *    E-mail address    : K.Kumar@tudelft.nl
 *
 *    Checker           : D. Dirkx
 *    Affiliation       : Delft University of Technology
 *    E-mail address    : D.Dirkx@student.tudelft.nl
 *
 *    Date created      : 1 september, 2010
 *    Last modified     : 29 september, 2010
 *
 *    References
 *      Press W.H., et al. Numerical Recipes in C++: The Art of
 *          Scientific Computing. Cambridge University Press, February 2002.
 *
 *    Notes
 *      Need to add bounds checking with respect to targetValue for
 *      computeNearestLeftNeighborUsingBinarySearch().
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
 *      YYMMDD    author              comment
 *      100902    K. Kumar            File header and footer added.
 *      100916    D. Dirkx            Added minor comments during checking.
 *      100928    K. Kumar            Small comment modifications.
 *      100929    K. Kumar            Small comment modifications.
 */

// Include statements.
#include "basicFunctions.h"

//! basicFunctions namespace.
namespace basic_functions
{

//! Nearest left neighbor binary search.
int computeNearestLeftNeighborUsingBinarySearch(
        Vector& vectorOfSortedData,
        double& targetValueInVectorOfSortedData )
{
    // Declare local variables.
    // Declare bounds of vector of sorted data and current position.
    int leftLimitOfVectorOfSortedData = 0;
    // PLACEHOLDER
    int rightLimitOfVectorOfSortedData = vectorOfSortedData.rows() - 1;
    int currentPositionInVectorOfSortedData;

    // Check if data is sorted in ascending order
    // ( true if ascending, else false ).
    bool isVectorOfSortedDataAscending
            = ( vectorOfSortedData[rightLimitOfVectorOfSortedData]
                >= vectorOfSortedData[leftLimitOfVectorOfSortedData]);

    // Loop through vector of sorted data until left and right limits
    // are neighbours.
    while ( rightLimitOfVectorOfSortedData
            - leftLimitOfVectorOfSortedData > 1 )
    {
        // Compute midpoint ( bitshift is same as division by 2.0 ).
        currentPositionInVectorOfSortedData
                = ( rightLimitOfVectorOfSortedData
                    + leftLimitOfVectorOfSortedData ) >> 1;

        // Check which limit to replace ( if ascending and target datum
        // is in right half, replace left limit ).
        if ( targetValueInVectorOfSortedData
             >= vectorOfSortedData[ currentPositionInVectorOfSortedData ]
             == isVectorOfSortedDataAscending )
        {
            // Set left limit to current position in vector of sorted data.
            leftLimitOfVectorOfSortedData = currentPositionInVectorOfSortedData;
        }
        else
        {
            // Set right limit to current position in vector of sorted data.
            rightLimitOfVectorOfSortedData = currentPositionInVectorOfSortedData;
        }
    }

    // Set current position to left limit.
    currentPositionInVectorOfSortedData = leftLimitOfVectorOfSortedData;

    // Return current position in vector.
    return currentPositionInVectorOfSortedData;
}

//! Nearest left neighbor binary search.
int computeNearestLeftNeighborUsingBinarySearch(
        std::map < double, Vector >& sortedIndepedentAndDependentVariables,
        double& targetValueInMapOfData )
{
    // Declare local variables.
    // Declare bounds of key of map of data and current position.
    int leftLimitOfKeyOfMapOfData = 0;
    int rightLimitOfKeyOfMapOfData = sortedIndepedentAndDependentVariables
                                     .size() - 1;
    int currentPositionInKeyOfMapOfData;

    // Declare map iterator
    std::map < double, Vector >::iterator mapIterator;

    // Loop through vector of sorted data until left and right limits
    // are neighbours
    while ( rightLimitOfKeyOfMapOfData - leftLimitOfKeyOfMapOfData > 1 )
    {
        // Compute midpoint ( bitshift is same as division by 2.0 ).
        currentPositionInKeyOfMapOfData
                = ( rightLimitOfKeyOfMapOfData
                   + leftLimitOfKeyOfMapOfData ) >> 1;

        // Set map iterator to begin begin of map of sorted independent and
        // dependent variables.
        mapIterator = sortedIndepedentAndDependentVariables.begin();

        // Advance iterator to location of current position in key of map of
        // data.
        advance( mapIterator, currentPositionInKeyOfMapOfData );

        // Check that target value lies to the right of lower bound.
        if ( targetValueInMapOfData
             >= mapIterator->first )
        {
            // Set left limit to current position in map of data.
            leftLimitOfKeyOfMapOfData = currentPositionInKeyOfMapOfData;
        }


        else
        {
            // Set right limit to current position in map of data.
            rightLimitOfKeyOfMapOfData = currentPositionInKeyOfMapOfData;
        }
    }

    // Set current position to left limit.
    currentPositionInKeyOfMapOfData = leftLimitOfKeyOfMapOfData;

    // Return current position in map of data.
    return currentPositionInKeyOfMapOfData;
}

}

// End of file.