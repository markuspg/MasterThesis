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

    virtual std::vector< T > *GenerateRandomSolution( const std::size_t &argSize ) const = 0;

protected:
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
    solutionVec{ argSolution }
{
}

template < typename T >
mt::Solution< T >::~Solution() {
    delete solutionVec;
}

#endif // SOLUTION_H
