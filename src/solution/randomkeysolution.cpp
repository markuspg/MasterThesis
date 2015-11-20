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

#include "randomkeysolution.h"

mt::RandomKeySolution::RandomKeySolution( const unsigned int &argSeed, const std::size_t &argSize ) :
    Solution{ solutionTypes_t::RANDOMKEYSOLUTION, GenerateRandomSolution( argSeed, argSize ) }
{
}

mt::RandomKeySolution::RandomKeySolution( std::vector<double> * const argSolution ) :
    Solution{ solutionTypes_t::RANDOMKEYSOLUTION, argSolution }
{
}

mt::RandomKeySolution::RandomKeySolution( const mt::RandomKeySolution &argRandomKeySolution ) :
    Solution{ argRandomKeySolution }
{
}

mt::RandomKeySolution::RandomKeySolution( mt::RandomKeySolution &&argRandomKeySolution ) :
    Solution{ argRandomKeySolution }
{
}

mt::RandomKeySolution::~RandomKeySolution() {
}

mt::SolutionBase *mt::RandomKeySolution::Copy() const {
    return new mt::RandomKeySolution{ new std::vector< double >{ *solutionVec } };
}

std::vector< double > *mt::RandomKeySolution::GenerateRandomSolution( const unsigned int &argSeed,
                                                                      const std::size_t &argSize ) const {
    std::vector< double > *tempVec = new std::vector< double >;
    tempVec->resize( argSize, 0.0 );

    std::random_device randomDevice;
#ifdef Q_PROCESSOR_X86_64
    std::mt19937_64 engine{ randomDevice() + argSeed };
#else
    std::mt19937 engine{ randomDevice() + argSeed };
#endif
    std::uniform_real_distribution<> distribution{ 0.0, 1.0 };
    for ( unsigned long i = 0; i < argSize; ++i ) {
        ( *tempVec )[ i ] = distribution( engine );
    }

    return tempVec;
}

mt::PermSolution *mt::RandomKeySolution::GetPermSolution() const {
    return PermSolution::ComputeFromRandomKeys( this );
}

std::vector< double > *mt::RandomKeySolution::GetSolutionVectorCopy() const {
    return new std::vector< double >{ *solutionVec };
}

mt::SolutionBase *mt::RandomKeySolution::GetSwappedVariant( const unsigned long &argSwapIndexI,
                                                            const unsigned long &argSwapIndexJ ) const {
    std::vector< double > *temp = new std::vector< double >{ *solutionVec };
    double tempD = ( *temp )[ argSwapIndexI ];
    ( *temp )[ argSwapIndexI ] = ( *temp )[ argSwapIndexJ ];
    ( *temp )[ argSwapIndexJ ] = tempD;
    return new RandomKeySolution{ temp };
}

mt::SolutionBase *mt::RandomKeySolution::ReproduceWithOtherParent( const unsigned long &argCrossoverPoint,
            const mt::SolutionBase * const argOtherParent ) const {
    const RandomKeySolution* const tempSol = dynamic_cast< const RandomKeySolution* >( argOtherParent );
    assert( tempSol );

    std::vector< double > * const newSolutionVec = new std::vector< double >;
    newSolutionVec->resize( size, 0.0 );

    for ( unsigned long i = 0; i < size; ++i ) {
        if ( i < argCrossoverPoint ) {
            newSolutionVec->at( i ) = ( *solutionVec )[ i ];
        } else {
            newSolutionVec->at( i ) = ( *tempSol )( i );
        }
    }

    return new mt::RandomKeySolution{ newSolutionVec };
}
