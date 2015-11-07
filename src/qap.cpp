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

#include "qap.h"

mt::QAP::QAP( const std::vector<std::string> &argTokens ) :
    Problem{ problemTypes_t::QAP, argTokens },
    distances{ mt::Matrix< int >::ConvertStringVecToIntVec( Split( argTokens[ 4 ], ';' ) ) },
    flows{ mt::Matrix< int >::ConvertStringVecToIntVec( Split( argTokens[ 3 ], ';' ) ) }
{
    std::cout << "      > Constructing QAP " << argTokens[ 0 ] << std::endl;
}

mt::QAP::~QAP() {
}

std::vector< unsigned long > mt::QAP::ConvertRandomKeysToAssignment(
             const mt::RandomKeySolution * const argSolution ) const {
    // The solution to be converted and calculated
    std::vector< double > * solution = new std::vector< double >{ *argSolution->solution };
    // Stores the converted solution
    std::vector< unsigned long > assignments;
    assignments.resize( solution->size() );

    // Query for 'solution-size' iterations the minimum element and set the index in the assigment vector
    for ( unsigned long i = 0; i < solution->size(); ++i ) {
        std::vector<double>::iterator minElem = std::min_element( solution->begin(), solution->end() );
        auto offset = std::distance( solution->begin(), minElem) ;
        assignments[ i ] = offset;
        *minElem = std::numeric_limits< double >::max();
    }
    delete solution;
    solution = nullptr;

    return assignments;
}

double mt::QAP::GetOFV( const mt::RandomKeySolution * const argSolution ) const {
    // Stores the converted solution
    std::vector< unsigned long > assignments = ConvertRandomKeysToAssignment( argSolution );

    unsigned long ofv = 0;
    for ( unsigned long i = 0; i < assignments.size(); ++i ) {
        for ( unsigned long j = 0; j < assignments.size(); ++j ) {
            const int flow = flows( assignments[ i ], assignments[ j ] );
            const int distance = distances( i, j );
            ofv += flow * distance;
        }
    }

    return ofv;
}
