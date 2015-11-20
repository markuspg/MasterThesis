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

#include "perm_solution.h"

mt::PermSolution::PermSolution( const unsigned int &argSeed, const std::size_t &argSize ) :
    Solution{ solutionTypes_t::PERMSOLUTION, GenerateRandomSolution( argSeed, argSize ) }
{
}

mt::PermSolution::PermSolution( std::vector< unsigned long > * const argSolution ) :
    Solution{ solutionTypes_t::PERMSOLUTION, argSolution }
{
}

mt::PermSolution::PermSolution( const mt::PermSolution &argPermSolution ) :
    Solution{ argPermSolution }
{
}

mt::PermSolution::PermSolution( mt::PermSolution &&argPermSolution ) :
    Solution{ argPermSolution }
{
}

mt::PermSolution::~PermSolution() {
}

mt::SolutionBase *mt::PermSolution::Copy() const {
    return new PermSolution{ new std::vector< unsigned long >{ *solutionVec } };
}

std::vector< unsigned long > *mt::PermSolution::GenerateRandomSolution( const unsigned int &argSeed,
                                                                        const std::size_t &argSize ) const {
    std::vector< unsigned long > *tempVec = new std::vector< unsigned long >;
    tempVec->resize( argSize, 0 );
    std::iota( tempVec->begin(), tempVec->end(), 0 );

    std::random_device randomDevice;
#ifdef Q_PROCESSOR_X86_64
    std::mt19937_64 engine{ randomDevice() + argSeed };
#else
    std::mt19937 engine{ randomDevice() + argSeed };
#endif
    std::shuffle( tempVec->begin(), tempVec->end(), engine );

    return tempVec;
}

mt::PermSolution *mt::PermSolution::GetPermSolution() const {
    return dynamic_cast< PermSolution* >( Copy() );
}

mt::SolutionBase *mt::PermSolution::GetSwappedVariant( const unsigned long &argSwapIndexI,
                                                       const unsigned long &argSwapIndexJ ) const {
    std::vector< unsigned long > *temp = new std::vector< unsigned long >{ *solutionVec };
    double tempD = ( *temp )[ argSwapIndexI ];
    ( *temp )[ argSwapIndexI ] = ( *temp )[ argSwapIndexJ ];
    ( *temp )[ argSwapIndexJ ] = tempD;
    return new PermSolution{ temp };
}

mt::SolutionBase *mt::PermSolution::ReproduceWithOtherParent( const unsigned long &argCrossoverPoint,
            const mt::SolutionBase * const argOtherParent ) const {
    const PermSolution * const tempSol = dynamic_cast< const PermSolution* >( argOtherParent );
    assert( tempSol );

    std::vector< unsigned long > * const newAssignment = new std::vector< unsigned long >;
    newAssignment->resize( size, 0 );

    for ( unsigned long i = 0; i < size; ++i ) {
        if ( i < argCrossoverPoint ) {
            newAssignment->at( i ) = ( *solutionVec )[ i ];
        } else {
            newAssignment->at( i ) = ( *tempSol )( i );
        }
    }

    return new PermSolution{ newAssignment };
}
