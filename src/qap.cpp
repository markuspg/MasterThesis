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

double mt::QAP::GetOFV( const mt::RandomKeySolution * const argSolution ) const {
    // Stores the converted solution
    mt::QAPSolution *assignment = new mt::QAPSolution{ mt::QAPSolution::ComputeFromRandomKeys( argSolution ) };
    const std::vector< unsigned long > * const assignmentVec = assignment->assignment;

    unsigned long ofv = 0;
    for ( unsigned long i = 0; i < assignmentVec->size(); ++i ) {
        for ( unsigned long j = 0; j < assignmentVec->size(); ++j ) {
            const int flow = flows( ( *assignmentVec )[ i ], ( *assignmentVec )[ j ] );
            const int distance = distances( i, j );
            ofv += flow * distance;
        }
    }
    delete assignment;

    return ofv;
}
