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

#include "analyzer.h"

mt::Analyzer::Analyzer( const mt::Problem *argProblem ) :
    problem{ argProblem },
    tsReferenceSet{ problem }
{
    std::cout << "    Constructing Analyzer" << std::endl;
}

void mt::Analyzer::Analyze() {
    std::cout << "     Analyzing ..." << std::endl;

    gaThreadObjects.reserve( *settings->gaInstances );
    for ( unsigned short i = 0; i < *settings->gaInstances; ++i ) {
        gaThreadObjects.emplace_back( i, tsReferenceSetMutex, problem, tsReferenceSet );
    }
    tsThreadObjects.reserve( *settings->tsInstances );
    for ( unsigned short i = 0; i < *settings->tsInstances; ++i ) {
        tsThreadObjects.emplace_back( i, tsReferenceSetMutex, problem, tsReferenceSet );
    }

    // Do the initialization run
    Run();

    tsReferenceSet.PrepareOptimizationRun();
    for ( auto it = tsThreadObjects.begin(); it != tsThreadObjects.end(); ++it ) {
        ( *it ).PrepareOptimizationRun();
    }

    // Do the optimization run
    Run();
}

void mt::Analyzer::Run() {
    while ( true ) {
        ++tsReferenceSet;

        unsigned short finishedThreads = 0;
        // Create the threads
        std::vector< std::thread > gaThreads;
        std::vector< std::thread > tsThreads;
        for ( unsigned short i = 0; i < *settings->gaInstances; ++i ) {
            if ( !gaThreadObjects[ i ].IsFinished() ) {
                gaThreads.emplace_back( &mt::GAThread::Iteration, &gaThreadObjects[ i ] );
            } else {
                ++finishedThreads;
                gaThreads.emplace_back();
            }
        }
        for ( unsigned short i = 0; i < *settings->tsInstances; ++i ) {
            if ( !tsThreadObjects[ i ].IsFinished() ) {
                tsThreads.emplace_back( &mt::TSThread::Iteration, &tsThreadObjects[ i ] );
            } else {
                ++finishedThreads;
                tsThreads.emplace_back();
            }
        }
        for ( unsigned short i = 0; i < *settings->gaInstances; ++i ) {
            gaThreads[ i ].join();
        }
        for ( unsigned short i = 0; i < *settings->tsInstances; ++i ) {
            if ( tsThreads[ i ].joinable() ) {
                tsThreads[ i ].join();
            }
        }

        tsReferenceSet.RotateSolutions();

        // Since the genetic algorithm never finishes in this implementation, this behaviour is fine
        if ( finishedThreads == *settings->tsInstances ) {
            break;
        }
    }

    // No matter that this will be called twice, since the second run will overwrite the first one's data
    tsReferenceSet.SetFinalStatistics();

    std::cout << "     Analyzer finished after " << tsReferenceSet.GetIterationCount()
              << " iterations" << std::endl;
}
