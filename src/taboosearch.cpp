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

#include "taboosearch.h"

std::mutex tsMutex;

void mt::TabooSearchCycle( const unsigned short argIndex, const mt::Problem * const argProblem,
                           mt::TSReferenceSet &argTSReferenceSet ) {
    std::cout << "      Running TabooSearchCycle in thread " << argIndex << std::endl;

    mt::TSProcessorSettings *processorSettings = argTSReferenceSet.GetProcessorSettings( argIndex );

    mt::RandomKeySolution *tempSol = nullptr;
    {
        std::lock_guard< std::mutex > lockTSReferenceSet{ tsMutex };
        tempSol = argTSReferenceSet.GetStartSolution( argIndex );
    }
    double tempSolV = argProblem->GetOFV( tempSol );

    mt::RandomKeySolution *bestNeigh = GetBestNeighbour( argIndex, argTSReferenceSet.GetIterationCount(),
                                                         argProblem, processorSettings, tempSol );
    double bestNeighV = 0.0;

    if ( !bestNeigh ) {
        processorSettings->IncFailures();
        processorSettings->IncInvalidSolutions();
        return;
    } else {
        bestNeighV = argProblem->GetOFV( bestNeigh );
    }

    if ( bestNeighV > tempSolV ) {
        processorSettings->IncFailures();
    }

    std::lock_guard< std::mutex > lockTSReferenceSet{ tsMutex };
    argTSReferenceSet.SetSolution( argIndex, tempSol );
}
