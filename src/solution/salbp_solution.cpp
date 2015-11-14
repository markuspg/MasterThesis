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

#include "salbp_solution.h"

mt::SALBPSolution::SALBPSolution( const std::size_t &argSize ) :
    Solution{ GenerateRandomSolution( argSize ) }
{
}

mt::SALBPSolution::SALBPSolution( std::vector< unsigned long > * const argSolution ) :
    Solution{ argSolution }
{
}

mt::SALBPSolution::SALBPSolution( const mt::SALBPSolution &argSALBPSolution ) :
    Solution{ argSALBPSolution }
{
}

mt::SALBPSolution::SALBPSolution( mt::SALBPSolution &&argSALBPSolution ) :
    Solution{ argSALBPSolution }
{
}

mt::SALBPSolution::~SALBPSolution() {
}

mt::SALBPSolution *mt::SALBPSolution::ComputeFromRandomKeys( const mt::RandomKeySolution *argSol ) {
    // The solution to be converted and calculated
    std::vector< double > * solution = argSol->GetSolutionVectorCopy();
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

    return new mt::SALBPSolution{ tempAssignment };
}

mt::SolutionBase *mt::SALBPSolution::Copy() const {
    return new mt::SALBPSolution{ new std::vector< unsigned long >{ *solutionVec } };
}

std::vector< unsigned long > *mt::SALBPSolution::GenerateRandomSolution( const std::size_t &argSize ) const {
    std::vector< unsigned long > *tempVec = new std::vector< unsigned long >;
    tempVec->resize( argSize, 0 );
    std::iota( tempVec->begin(), tempVec->end(), 0 );

    std::random_device randomDevice;
#ifdef Q_PROCESSOR_X86_64
    std::mt19937_64 engine{ randomDevice() };
#else
    std::mt19937 engine{ randomDevice() };
#endif
    std::shuffle( tempVec->begin(), tempVec->end(), engine );

    return tempVec;
}

std::vector< unsigned long > *mt::SALBPSolution::GetAssignmentVectorCopy() const {
    return new std::vector< unsigned long >{ *solutionVec };
}

mt::QAPSolution *mt::SALBPSolution::GetQAPSolution() const {
    return nullptr;
}

mt::SALBPSolution *mt::SALBPSolution::GetSALBPSolution() const {
    return dynamic_cast< mt::SALBPSolution* >( Copy() );
}

std::vector< double > *mt::SALBPSolution::GetSolutionVectorCopy() const {
    return nullptr;
}

mt::SolutionBase *mt::SALBPSolution::GetSwappedVariant( const unsigned long &argSwapIndexI,
                                                        const unsigned long &argSwapIndexJ ) const {
    std::vector< unsigned long > *temp = new std::vector< unsigned long >{ *solutionVec };
    double tempD = ( *temp )[ argSwapIndexI ];
    ( *temp )[ argSwapIndexI ] = ( *temp )[ argSwapIndexJ ];
    ( *temp )[ argSwapIndexJ ] = tempD;
    return new SALBPSolution{ temp };
}

mt::SolutionBase *mt::SALBPSolution::ReproduceWithOtherParent( const unsigned long &argCrossoverPoint,
            const mt::SolutionBase * const argOtherParent ) const {
    const unsigned long newAssignmentSize = solutionVec->size();
    std::vector< unsigned long > * const newAssignment = new std::vector< unsigned long >;
    newAssignment->resize( newAssignmentSize, 0 );

    std::vector< unsigned long > *otherParentsAssignment = argOtherParent->GetAssignmentVectorCopy();

    for ( unsigned long i = 0; i < newAssignmentSize; ++i ) {
        if ( i < argCrossoverPoint ) {
            newAssignment->at( i ) = ( *solutionVec )[ i ];
        } else {
            newAssignment->at( i ) = ( *otherParentsAssignment )[ i ];
        }
    }

    delete otherParentsAssignment;

    return new mt::SALBPSolution{ newAssignment };
}