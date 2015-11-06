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

#include "tsthread.h"

mt::TSThread::TSThread( const unsigned short &argIndex, std::mutex &argMutex,
                        mt::TSReferenceSet &argReferenceSet ) :
    index{ argIndex },
    mutex{ argMutex },
    referenceSet{ argReferenceSet }
{
    std::cout << "      Constructing TSThread with id " << index << std::endl;
}

mt::RandomKeySolution *mt::TSThread::GetBestNeigh( double &argBestNeighV,
                                                   mt::RandomKeySolution *argTempSol ) {
    return argTempSol;
}

void mt::TSThread::Iteration() {
    mt::RandomKeySolution *tempSol = referenceSet.GetStartSolution( index );
    double tempSolV = referenceSet.GetStartSolutionValue( index );

    double bestNeighV = std::numeric_limits< double >::max();
    mt::RandomKeySolution *bestNeigh = GetBestNeigh( bestNeighV, tempSol );

    if ( !bestNeigh ) {
        ++failures;
        ++invalidSolutions;
        return;
    }

    if ( bestNeighV > tempSolV ) {
        ++failures;
    }

    referenceSet.SetSolution( index, bestNeigh );
    referenceSet.SetSolutionValue( index, bestNeighV );

    referenceSet.PromoteBestSolution( index );
}
