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

mt::RandomKeySolution::RandomKeySolution( const std::size_t &argSize ) :
    Solution{},
    solutionVec{ new std::vector< double > }
{
    solutionVec->resize( argSize );

    std::random_device randomDevice;
#ifdef Q_PROCESSOR_X86_64
    std::mt19937_64 engine{ randomDevice() };
#else
    std::mt19937 engine{ randomDevice() };
#endif
    std::uniform_real_distribution< double > distribution{ 0.0, 1.0 };

    for ( std::size_t i = 0; i < argSize; ++i ) {
        ( *solutionVec )[ i ] =  distribution( engine );
    }
}

mt::RandomKeySolution::RandomKeySolution( std::vector<double> * const argSolution ) :
    Solution{},
    solutionVec{ argSolution }
{
}

mt::RandomKeySolution::RandomKeySolution( const mt::RandomKeySolution &argRandomKeySolution ) :
    Solution{ argRandomKeySolution },
    solutionVec{ new std::vector< double >{ *argRandomKeySolution.solutionVec } }
{
}

mt::RandomKeySolution::RandomKeySolution( mt::RandomKeySolution &&argRandomKeySolution ) :
    Solution{},
    solutionVec{ argRandomKeySolution.solutionVec }
{
    argRandomKeySolution.solutionVec = nullptr;
}

mt::RandomKeySolution::~RandomKeySolution() {
    delete solutionVec;
}

mt::SolutionBase *mt::RandomKeySolution::Copy() const {
    return new mt::RandomKeySolution{ new std::vector< double >{ *solutionVec } };
}

mt::SolutionBase *mt::RandomKeySolution::GetQAPSolution() const {
    return QAPSolution::ComputeFromRandomKeys( this );
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
    const unsigned long newSolutionVecSize = solutionVec->size();
    std::vector< double > * const newSolutionVec = new std::vector< double >;
    newSolutionVec->resize( newSolutionVecSize, 0.0 );

    std::vector< double > *otherParentsAssignment = argOtherParent->GetSolutionVectorCopy();

    for ( unsigned long i = 0; i < newSolutionVecSize; ++i ) {
        if ( i < argCrossoverPoint ) {
            newSolutionVec->at( i ) = ( *solutionVec )[ i ];
        } else {
            newSolutionVec->at( i ) = ( *otherParentsAssignment )[ i ];
        }
    }

    delete otherParentsAssignment;

    return new mt::RandomKeySolution{ newSolutionVec };
}
