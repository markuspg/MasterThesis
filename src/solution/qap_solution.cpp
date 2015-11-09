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

#include "qap_solution.h"

mt::QAPSolution::QAPSolution( const std::size_t &argSize ) :
    Solution{},
    assignment{ GenerateRandomSolution( argSize ) }
{
}

mt::QAPSolution::QAPSolution( const std::vector< unsigned long > * const argSolution ) :
    Solution{},
    assignment{ argSolution }
{
}

mt::QAPSolution::QAPSolution( QAPSolution &&argQAPSolution ) :
    Solution{},
    assignment{ argQAPSolution.assignment }
{
    argQAPSolution.assignment = nullptr;
}

mt::QAPSolution::~QAPSolution() {
    delete assignment;
}

std::vector< unsigned long > *mt::QAPSolution::ComputeFromRandomKeys( const mt::RandomKeySolution *argSol ) {
    // The solution to be converted and calculated
    std::vector< double > * solution = new std::vector< double >{ *argSol->solutionVec };
    // Stores the converted solution
    std::vector< unsigned long > *tempAssignment = new std::vector< unsigned long >;
    tempAssignment->resize( solution->size() );

    // Query for 'solution-size' iterations the minimum element and set the index in the assigment vector
    for ( unsigned long i = 0; i < solution->size(); ++i ) {
        std::vector<double>::iterator minElem = std::min_element( solution->begin(), solution->end() );
        auto offset = std::distance( solution->begin(), minElem) ;
        ( *tempAssignment )[ i ] = offset;
        *minElem = std::numeric_limits< double >::max();
    }
    delete solution;
    solution = nullptr;

    return tempAssignment;
}

std::vector< unsigned long > *mt::QAPSolution::GenerateRandomSolution( const std::size_t &argSize ) {
    std::vector< unsigned long > * tempSolution = new std::vector< unsigned long >;
    tempSolution->resize( argSize );
    std::iota( tempSolution->begin(), tempSolution->end(), 0 );

    std::random_device randomDevice;
#ifdef Q_PROCESSOR_X86_64
    std::mt19937_64 engine{ randomDevice() };
#else
    std::mt19937 engine{ randomDevice() };
#endif
    std::shuffle( tempSolution->begin(), tempSolution->end(), engine );

    return tempSolution;
}

mt::QAPSolution *mt::QAPSolution::GetSwappedVariant( const unsigned long &argSwapIndexI,
                                                     const unsigned long &argSwapIndexJ ) const {
    std::vector< unsigned long > *temp = new std::vector< unsigned long >{ *assignment };
    double tempD = ( *temp )[ argSwapIndexI ];
    ( *temp )[ argSwapIndexI ] = ( *temp )[ argSwapIndexJ ];
    ( *temp )[ argSwapIndexJ ] = tempD;
    return new QAPSolution{ temp };
}
