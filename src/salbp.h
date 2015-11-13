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

#ifndef SALBP_H
#define SALBP_H

#include "problem.h"
#include "solution/solution.h"
#include "task.h"

namespace mt {

class SALBP final : public Problem {
public:
    SALBP( const std::vector<std::string> &argTokens );
    SALBP( const SALBP &argSALBP ) = delete;
    SALBP( SALBP &&argSALBP ) = delete;
    virtual ~SALBP();

    virtual SolutionBase *GenerateRandomSolution( const std::size_t &argSize ) const override;
    virtual double GetOFV( const SolutionBase * const argSolution ) const override;
    virtual void UpdateTabooTenures( const SolutionBase * const argNewSolution,
                                     const long &argSwapI, const long &argSwapJ,
                                     const unsigned long &argTabooTenure,
                                     mt::Matrix< unsigned long > &argTTMatrix ) const override;

private:
    std::vector< Task* > tasks;
};

}

#endif // SALBP_H
