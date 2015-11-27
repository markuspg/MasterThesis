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
    Solution{ GenerateRandomSolution( argSeed, argSize ) }
{
}

mt::RandomKeySolution::RandomKeySolution( std::vector<double> * const argSolution ) :
    Solution{ argSolution }
{
}

mt::RandomKeySolution::RandomKeySolution( const mt::RandomKeySolution &argRandomKeySolution ) :
    Solution{ argRandomKeySolution },
    permutationVec{ new std::vector< unsigned long >{ *argRandomKeySolution.permutationVec } }
{
}

mt::RandomKeySolution::RandomKeySolution( mt::RandomKeySolution &&argRandomKeySolution ) :
    Solution{ std::move( argRandomKeySolution ) },
    permutationVec{ argRandomKeySolution.permutationVec }
{
    argRandomKeySolution.permutationVec = nullptr;
}

mt::RandomKeySolution::~RandomKeySolution() {
    delete permutationVec;
}

mt::SolutionBase *mt::RandomKeySolution::Copy() const {
    return new RandomKeySolution{ *this };
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
        tempVec->at( i ) = distribution( engine );
    }

    return tempVec;
}

mt::PermSolution *mt::RandomKeySolution::GetPermSolution() {
    if ( solVecChanged ) {
        UpdatePermutationVector();
    }
    return new PermSolution{ new std::vector< unsigned long >{ *permutationVec } };
}

mt::SolutionBase *mt::RandomKeySolution::GetSwappedVariant( const unsigned long &argSwapIndexI,
                                                            const unsigned long &argSwapIndexJ ) const {
    std::vector< double > *temp = new std::vector< double >{ *solutionVec };
    double tempD = ( *temp )[ argSwapIndexI ];
    ( *temp )[ argSwapIndexI ] = ( *temp )[ argSwapIndexJ ];
    ( *temp )[ argSwapIndexJ ] = tempD;
    return new RandomKeySolution{ temp };
}

#ifdef Q_PROCESSOR_X86_64
void mt::RandomKeySolution::Mutate( std::mt19937_64 &argEngine ) {
#else
void mt::RandomKeySolution::Mutate( std::mt19937 &argEngine ) {
#endif
    unsigned long mutationQuantity = std::round( *settings->mutationImpact * size );
    // Ensure at least one mutation
    mutationQuantity = mutationQuantity >= 1 ? mutationQuantity : 1;

    std::uniform_int_distribution<> alleleDistribution{ 0, static_cast< int >( size - 1 ) };
    std::uniform_real_distribution<> geneDistribution{ 0.0, 1.0 };

    for ( unsigned long i = 0; i < mutationQuantity; ++i ) {
        ( *solutionVec )[ alleleDistribution( argEngine ) ] = geneDistribution( argEngine );
    }

    solVecChanged = true;
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

void mt::RandomKeySolution::UpdatePermutationVector() {
    delete permutationVec;
    // The solution to be converted and calculated
    std::vector< double > solution{ *solutionVec };
    // Stores the converted solution
    std::vector< unsigned long > *tempAssignment = new std::vector< unsigned long >;
    tempAssignment->resize( size );

    // Query for 'solution-size' iterations the minimum element and set the index in the assigment vector
    for ( unsigned long i = 0; i < size; ++i ) {
        std::vector< double >::iterator minElem = std::min_element( solution.begin(), solution.end() );
        auto offset = std::distance( solution.begin(), minElem) ;
        ( *tempAssignment )[ i ] = offset;
        *minElem = std::numeric_limits< double >::max();
    }

    permutationVec = tempAssignment;

    solVecChanged = false;
}
