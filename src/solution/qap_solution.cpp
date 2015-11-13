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
    assignment{ new std::vector< unsigned long > }
{
    assignment->resize( argSize, 0 );
    std::iota( assignment->begin(), assignment->end(), 0 );

    std::random_device randomDevice;
#ifdef Q_PROCESSOR_X86_64
    std::mt19937_64 engine{ randomDevice() };
#else
    std::mt19937 engine{ randomDevice() };
#endif
    std::shuffle( assignment->begin(), assignment->end(), engine );
}

mt::QAPSolution::QAPSolution( std::vector< unsigned long > * const argSolution ) :
    Solution{},
    assignment{ argSolution }
{
}

mt::QAPSolution::QAPSolution( const mt::QAPSolution &argQAPSolution ) :
    Solution{ argQAPSolution },
    assignment{ new std::vector< unsigned long >{ *argQAPSolution.assignment } }
{
}

mt::QAPSolution::QAPSolution( mt::QAPSolution &&argQAPSolution ) :
    Solution{},
    assignment{ argQAPSolution.assignment }
{
    argQAPSolution.assignment = nullptr;
}

mt::QAPSolution::~QAPSolution() {
    delete assignment;
}

mt::QAPSolution *mt::QAPSolution::ComputeFromRandomKeys( const mt::RandomKeySolution *argSol ) {
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

    return new mt::QAPSolution{ tempAssignment };
}

mt::SolutionBase *mt::QAPSolution::Copy() const {
    return new mt::QAPSolution{ new std::vector< unsigned long >{ *assignment } };
}

std::vector< unsigned long > *mt::QAPSolution::GetAssignmentVectorCopy() const {
    return new std::vector< unsigned long >{ *assignment };
}

mt::SolutionBase *mt::QAPSolution::GetQAPSolution() const {
    return Copy();
}

mt::SolutionBase *mt::QAPSolution::GetSwappedVariant( const unsigned long &argSwapIndexI,
                                                      const unsigned long &argSwapIndexJ ) const {
    std::vector< unsigned long > *temp = new std::vector< unsigned long >{ *assignment };
    double tempD = ( *temp )[ argSwapIndexI ];
    ( *temp )[ argSwapIndexI ] = ( *temp )[ argSwapIndexJ ];
    ( *temp )[ argSwapIndexJ ] = tempD;
    return new QAPSolution{ temp };
}

mt::SolutionBase *mt::QAPSolution::ReproduceWithOtherParent( const unsigned long &argCrossoverPoint,
            const mt::SolutionBase * const argOtherParent ) const {
    const unsigned long newAssignmentSize = assignment->size();
    std::vector< unsigned long > * const newAssignment = new std::vector< unsigned long >;
    newAssignment->resize( newAssignmentSize, 0 );

    std::vector< unsigned long > *otherParentsAssignment = argOtherParent->GetAssignmentVectorCopy();

    for ( unsigned long i = 0; i < newAssignmentSize; ++i ) {
        if ( i < argCrossoverPoint ) {
            newAssignment->at( i ) = ( *assignment )[ i ];
        } else {
            newAssignment->at( i ) = ( *otherParentsAssignment )[ i ];
        }
    }

    delete otherParentsAssignment;

    return new mt::QAPSolution{ newAssignment };
}
