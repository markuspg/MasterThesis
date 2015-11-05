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
    tsReferenceSet{ problem, *settings->tsInstances }
{
    std::cout << "    Constructing Analyzer" << std::endl;
}

void mt::Analyzer::Analyze() {
    std::cout << "     Analyzing ..." << std::endl;

    while ( true ) {
        ++tsReferenceSet;

        // Create the threads
        std::vector< std::thread > gaThreads;
        std::vector< std::thread > tsThreads;
        for ( unsigned short i = 0; i < *settings->gaInstances; ++i ) {
            gaThreads.emplace_back( mt::GeneticAlgorithmCycle, i );
        }
        for ( unsigned short i = 0; i < *settings->tsInstances; ++i ) {
            tsThreads.emplace_back( mt::TabooSearchCycle, i, std::ref( tsReferenceSet ) );
        }
        for ( unsigned short i = 0; i < *settings->gaInstances; ++i ) {
            gaThreads[ i ].join();
        }
        for ( unsigned short i = 0; i < *settings->tsInstances; ++i ) {
            tsThreads[ i ].join();
        }
    }

    std::cout << "     Analyzer finished after " << tsReferenceSet.GetIterationCount()
              << " iterations" << std::endl;
}
