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

#include "taboosearchreferenceset.h"

mt::TabooSearchReferenceSet::TabooSearchReferenceSet( const mt::Problem * const argProblem,
                                                      const unsigned short &argTSInstanceAmount ) :
    solutions{ argTSInstanceAmount, nullptr },
    tabooTenures{ argTSInstanceAmount, nullptr }
{
    std::cout << "    Constructing TabooSearchReferenceSet" << std::endl;
    for ( unsigned short i = 0; i < argTSInstanceAmount; ++i ) {
        solutions[ i ] = new mt::RandomKeySolution{ argProblem->size };
        tabooTenures[ i ] = new mt::Matrix< unsigned int >{ static_cast< int >( argProblem->size ), 0 };
    }
}

mt::TabooSearchReferenceSet::~TabooSearchReferenceSet() {
    for ( auto it = solutions.begin(); it != solutions.end(); ++it) {
        delete *it;
    }
    for ( auto it = tabooTenures.begin(); it != tabooTenures.end(); ++it ) {
        delete *it;
    }
}

void mt::TabooSearchReferenceSet::SetSolution( const unsigned short &argIndex,
                                               mt::RandomKeySolution *argSolution ) {
    if ( argSolution == solutions[ argIndex ] ) {
        return;
    }
    delete solutions[ argIndex ];
    solutions[ argIndex ] = argSolution;
}
