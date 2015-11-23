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

#include "../helper_functions.h"
#include "../matrix.h"
#include "../measure.h"
#include "../problem/problem.h"
#include "../solution/solutionbase.h"

#include <algorithm>
#include <iostream>
#include <memory>
#include <mutex>
#include <tuple>
#include <vector>

typedef std::pair< double, mt::SolutionBase* > dSol;
typedef std::tuple< mt::SolutionBase*, double, unsigned long, bool > solTup;

extern mt::Measure measure;
extern std::mutex measureMutex;

namespace mt {

// For diversification and intensification (james2009cooperative, p. 814)
class TSReferenceSet final {
public:
    TSReferenceSet( const Problem * const argProblem );
    ~TSReferenceSet();

    double GetGlobalMinimumSolV() const { return *std::min_element( bestSolutionValues.begin(),
                                                                    bestSolutionValues.end() ); }
    unsigned int GetIterationCount() const { return iterationCounter; }
    SolutionBase *GetStartSolution( const unsigned short &argIndex ) const;
    double GetStartSolutionValue( const unsigned short &argIndex ) const;
    void PrepareOptimizationRun();
    void RotateSolutions();
    void SetSolution( const unsigned short &argIndex, SolutionBase *argSolution, const double &argV );

    TSReferenceSet &operator++() { ++iterationCounter; return *this; }

    const mt::Problem * const problem;

private:
    void DiversifyUnchangedSolutions();
    void PromoteBestSolution( const unsigned short &argIndex );
    void SpreadGlobalBestSolution();

    std::vector< SolutionBase* > bestSolutions;
    std::vector< double > bestSolutionValues;
    mt::Matrix< unsigned long > frequenciesMatrix;
    bool initializationRun = true;
    std::unique_ptr< SolutionBase > globalBestSolution = nullptr;
    double globalBestSolutionV = std::numeric_limits< double >::max();
    unsigned int iterationCounter = 0;
    // Store solutions, their OFV and their update status (james2009cooperative, p.816)
    std::vector< solTup > solutions;
    bool updatedGlobalBestSolution = false;
};

}

#endif // TABOOSEARCHREFERENCESET_H
