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

#include "tsreferenceset.h"

mt::TSReferenceSet::TSReferenceSet( const mt::Problem * const argProblem,
                                    const unsigned short &argTSInstanceAmount ) :
    problem{ argProblem }
{
    bestSolutions.resize( argTSInstanceAmount, nullptr );
    bestSolutionValues.resize( argTSInstanceAmount, std::numeric_limits< double >::max() );
    solutions.resize( argTSInstanceAmount, nullptr );
    solutionValues.resize( argTSInstanceAmount, std::numeric_limits< double >::max() );
    std::cout << "    Constructing TabooSearchReferenceSet" << std::endl;
    for ( unsigned short i = 0; i < argTSInstanceAmount; ++i ) {
        solutions[ i ] = new mt::RandomKeySolution{ argProblem->size };
    }
}

mt::TSReferenceSet::~TSReferenceSet() {
    for ( auto it = solutions.begin(); it != solutions.end(); ++it) {
        delete *it;
    }
}

void mt::TSReferenceSet::PromoteBestSolution( const unsigned short &argIndex ) {
    if ( solutionValues[ argIndex ] < bestSolutionValues[ argIndex ] ) {
        delete bestSolutions[ argIndex ];
        bestSolutions[ argIndex ]
                = new mt::RandomKeySolution{
                        new std::vector< double >{ *solutions[ argIndex ]->solution } };
        bestSolutionValues[ argIndex ] = solutionValues[ argIndex ];
    }
}

void mt::TSReferenceSet::SetSolution( const unsigned short &argIndex,
                                      mt::RandomKeySolution *argSolution ) {
    if ( argSolution == solutions[ argIndex ] ) {
        return;
    }
    delete solutions[ argIndex ];
    solutions[ argIndex ] = argSolution;
}
