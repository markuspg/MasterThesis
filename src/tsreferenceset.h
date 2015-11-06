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

#ifndef TABOOSEARCHREFERENCESET_H
#define TABOOSEARCHREFERENCESET_H

#include "helper_functions.h"
#include "matrix.h"
#include "problem.h"
#include "randomkeysolution.h"
#include "tsprocessorsettings.h"

#include <iostream>
#include <vector>

namespace mt {

class TSReferenceSet {
public:
    TSReferenceSet( const mt::Problem * const argProblem,
                    const unsigned short &argTSInstanceAmount );
    ~TSReferenceSet();

    unsigned int GetIterationCount() const { return iterationCounter; }
    mt::TSProcessorSettings *GetProcessorSettings( std::size_t argIndex ) const
        { return processorSettings[ argIndex ]; }
    mt::RandomKeySolution *GetStartSolution( const unsigned short &argIndex ) const
        { return solutions[ argIndex ]; }
    double GetStartSolutionValue( const unsigned short &argIndex ) const
        { return solutionValues[ argIndex ]; }
    bool IsTSFinished( const unsigned short &argIndex ) const { return processorSettings[ argIndex ]->IsFinished(); }
    void PromoteBestSolution( const unsigned short &argIndex );
    void RotateSolutions() {}
    void SetSolution( const unsigned short &argIndex, mt::RandomKeySolution *argSolution );
    void SetSolutionValue( const unsigned short &argIndex, const double &argSolutionValue )
        { solutionValues[ argIndex ] = argSolutionValue; }

    TSReferenceSet &operator++() { ++iterationCounter; return *this; }

    const mt::Problem * const problem;

private:
    std::vector< mt::RandomKeySolution* > bestSolutions;
    std::vector< double > bestSolutionValues;
    unsigned int iterationCounter = 0;
    std::vector< mt::TSProcessorSettings* > processorSettings;
    std::vector< mt::RandomKeySolution* > solutions;
    std::vector< double > solutionValues;
    std::vector< mt::Matrix< unsigned int >* > tabooTenures;
};

}

#endif // TABOOSEARCHREFERENCESET_H
