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

#ifndef SOLUTION_H
#define SOLUTION_H

#include "solutionbase.h"

#include <list>
#include <vector>

namespace mt {

class QAPSolution;

template < typename T >
class Solution : public SolutionBase
{
public:
    Solution();
    Solution( const Solution &argSolution );
    Solution( Solution &&argSolution );
    Solution( std::vector< T > * const argSolution );
    virtual ~Solution();

    T& operator() ( const unsigned long &argIndex ) { return ( *solutionVec )[ argIndex ]; }

    // Solution diversification operator (james2009cooperative, p. 816)
    void Diversify( const unsigned long &argStepWidthBase ) override;
    virtual std::vector< T > *GenerateRandomSolution( const unsigned int &argSeed,
                                                      const std::size_t &argSize ) const = 0;
    typename std::vector< T >::size_type GetSize() const { return size; }

protected:
    typename std::vector< T >::size_type size = 0;
    std::vector< T > * solutionVec = nullptr;
};

}

template < typename T >
mt::Solution< T >::Solution() :
    SolutionBase{}
{
}

template < typename T >
mt::Solution< T >::Solution( const Solution &argSolution ) :
    SolutionBase{ argSolution },
    solutionVec{ new std::vector< T >{ *argSolution.solutionVec } }
{
}

template < typename T >
mt::Solution< T >::Solution( Solution &&argSolution ) :
    SolutionBase{ argSolution },
    solutionVec{ argSolution.solutionVec }
{
    argSolution.solutionVec = nullptr;
}

template < typename T >
mt::Solution< T >::Solution( std::vector< T > * const argSolution ) :
    SolutionBase{},
    size{ argSolution->size() },
    solutionVec{ argSolution }
{
}

template < typename T >
mt::Solution< T >::~Solution() {
    delete solutionVec;
}

template< typename T >
void mt::Solution< T >::Diversify( const unsigned long &argStepWidthBase ) {
    // This ensures that the diversification step size always starts with 2 (james2009cooperative, p. 816)
    unsigned long stepWidth = argStepWidthBase % ( solutionVec->size() - 1 );
    ++stepWidth;
    std::list< T > vecCopy{ solutionVec->begin(), solutionVec->end() };
    solutionVec->clear();
    for ( unsigned long step = stepWidth + 1; step > 0; --step ) {
        std::list< typename std::list< T >::iterator > iteratorList;
        // Collect iterators to all items to be moved
        typename std::list< T >::size_type size = vecCopy.size();
        for ( unsigned long index = step - 1; index < size; index += step ) {
            auto it = vecCopy.begin();
            std::advance( it, index );
            iteratorList.emplace_back( it );
        }
        // Move the referenced items to the vector and delete them from the vector copy list
        for ( auto it = iteratorList.begin(); it != iteratorList.end(); ++it ) {
            solutionVec->emplace_back( *( *it ) );
            vecCopy.erase( *it );
        }
    }
}

#endif // SOLUTION_H
