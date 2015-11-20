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

#ifndef QAP_H
#define QAP_H

#include "problem.h"
#include "../helper_functions.h"
#include "../matrix.h"
#include "../solution/perm_solution.h"
#include "../solution/solution.h"

#include <cassert>
#include <iostream>
#include <vector>

namespace mt {

class QAP final : public Problem {
public:
    QAP( const std::vector<std::string> &argTokens );
    QAP( const Problem &argProblem ) = delete;
    QAP( Problem &&argProblem ) = delete;
    virtual ~QAP();

    virtual bool CheckIfTaboo( const unsigned int &argIterationCount,
                               SolutionBase * const argSolution,
                               const unsigned long &argSwapIndexI,
                               const unsigned long &argSwapIndexJ,
                               const Matrix< unsigned long > &argTTMatrix ) const override;
    virtual SolutionBase *GenerateRandomSolution( const unsigned int &argSeed ) const override;
    virtual double GetOFV( SolutionBase * const argSolution ) const override;
    virtual void UpdateTabooTenures( SolutionBase * const argNewSolution,
                                     const long &argSwapI, const long &argSwapJ,
                                     const unsigned long &argTabooTenure,
                                     mt::Matrix< unsigned long > &argTTMatrix ) const override;

    const mt::Matrix< int > distances;  //! From location to location
    const mt::Matrix< int > flows;      //! From unit to unit
};

}

#endif // QAP_H
