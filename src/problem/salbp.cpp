/*
 * Copyright 2015 Markus Prasser
 *
 * This file is part of MasterThesis.
 *
 *  MasterThesis is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  MasterThesis is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with MasterThesis.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "salbp.h"

mt::SALBP::SALBP( const std::vector<std::string> &argTokens ) :
    Problem{ problemTypes_t::SALBP, argTokens },
    cycleTime{ std::stoul( argTokens[ 5 ] ) },
    tasks{ size, nullptr }
{
    // Split the tokens for their usage whilst the Tasks' construction
    std::vector< std::string > durationStrings = Split( argTokens[ 3 ], ';' );
    std::vector< std::string > precedenceStrings = Split( argTokens[ 4 ], ';' );

    // Construct all tasks, one after another
    for ( unsigned long i = 0; i < size; ++i ) {
        // First declare and initialize the needed variables
        std::vector< Task* > *predecessors = new std::vector< Task* >;
        unsigned long taskDuration = 0;
        unsigned long taskID = 0;

        // Then convert the string values to the needed data items
        taskDuration = std::stoul( durationStrings[ i ] );
        std::vector< std::string > precedenceString = Split( precedenceStrings[ i ], ':' );
        taskID = std::stoul( precedenceString[ 0 ] );
        if ( precedenceString.size() > 1 ) {
            std::vector< std::string > predecessorIndicesString = Split( precedenceString[ 1 ], ',' );
            for ( std::size_t j = 0; j < predecessorIndicesString.size(); ++j ) {
                predecessors->emplace_back( tasks[ std::stoul( predecessorIndicesString[ j ] ) - 1 ] );
            }
        }
        tasks[ i ] = new Task{ taskDuration, taskID, predecessors };
    }
}

mt::SALBP::~SALBP() {
    for ( auto &s: tasks ) {
        delete s;
    }
}

bool mt::SALBP::CheckIfTaboo( const unsigned int &argIterationCount,
                              const SolutionBase * const argSolution,
                              const unsigned long &argSwapIndexI,
                              const unsigned long &argSwapIndexJ,
                              const Matrix< unsigned long > &argTTMatrix ) const {
    SALBPSolution * const tempSol = dynamic_cast< SALBPSolution* >( argSolution->GetSALBPSolution() );
    if ( argTTMatrix( ( *tempSol )( argSwapIndexI ), argSwapIndexI ) > argIterationCount
         && argTTMatrix( ( *tempSol )( argSwapIndexJ ), argSwapIndexJ ) > argIterationCount ) {
        delete tempSol;
        return true;
    }
    delete tempSol;
    return false;
}

mt::SolutionBase *mt::SALBP::GenerateRandomSolution( const unsigned int &argSeed ) const {
    if ( *settings->randomKeys ) {
        return new mt::RandomKeySolution{ argSeed, size };
    } else {
        return new mt::SALBPSolution{ argSeed, size };
    }
}

double mt::SALBP::GetOFV( const SolutionBase * const argSolution ) const {
    // Storage for the converted solution and its objective function value
    mt::SALBPSolution * tempSol = dynamic_cast< mt::SALBPSolution* >( argSolution->GetSALBPSolution() );

    std::list< Task* > tasksToBeScheduled;
    std::vector< unsigned long > stationTimes;
    stationTimes.resize( size, 0 );
    // Sort the tasks according to their priorities in the list of the to be assigned tasks
    for ( unsigned long i = 0; i < size; ++i ) {
        tasksToBeScheduled.emplace_back( tasks[ ( *tempSol )( i ) ] );
    }

    std::list< Task* >::iterator *deleteIt = nullptr;
    for ( unsigned long i = 0; i < size; ++i ) {
        auto it = tasksToBeScheduled.begin();
        while ( it != tasksToBeScheduled.end() ) {
            if ( deleteIt ) {
                tasksToBeScheduled.erase( *deleteIt );
                delete deleteIt;
                deleteIt = nullptr;
                it = tasksToBeScheduled.begin();
                continue;
            }
            // First check for task, if it is available for scheduling (all predecessors scheduled)
            if ( ( *it )->AllPredecessorsScheduled() ) {
                // Check, if the task fits into the current station
                if ( stationTimes[ i ] + ( *it )->duration <= cycleTime ) {
                    // If yes, schedule it there, ...
                    stationTimes[ i ] += ( *it )->duration;
                    ( *it )->TaskGotScheduled();
                    deleteIt = new std::list< Task* >::iterator{ it };
                    continue;
                }
            }
            ++it;
        }
    }

    delete tempSol;

    return std::distance( stationTimes.begin(), std::find( stationTimes.begin(), stationTimes.end(), 0 ) );
}

void mt::SALBP::UpdateTabooTenures( const SolutionBase * const argNewSolution,
                                    const long &argSwapI, const long &argSwapJ,
                                    const unsigned long &argTabooTenure,
                                    mt::Matrix<unsigned long> &argTTMatrix ) const {
    SALBPSolution * const tempSol = dynamic_cast< SALBPSolution* >( argNewSolution->GetSALBPSolution() );
    // Forbid the  re-assignment of the swapped tasks to the same priority list positions
    argTTMatrix( ( *tempSol )( argSwapI ), argSwapI ) = argTabooTenure;
    argTTMatrix( ( *tempSol )( argSwapJ ), argSwapJ ) = argTabooTenure;
    delete tempSol;
}
