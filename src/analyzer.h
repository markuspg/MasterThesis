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

#ifndef ANALYZER_H
#define ANALYZER_H

#include "gathread.h"
#include "problem.h"
#include "settings.h"
#include "tsreferenceset.h"
#include "tsthread.h"

#include <functional>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

extern mt::Settings *settings;

namespace mt {

class Analyzer {
public:
    Analyzer( const mt::Problem *argProblem );
    Analyzer( const Analyzer &argAnalyzer ) = delete;
    Analyzer( Analyzer &&argAnalyzer ) = delete;
    
    void Analyze();
    void Run();

private:
    std::vector< mt::GAThread > gaThreadObjects;
    const mt::Problem * const problem;
    mt::TSReferenceSet tsReferenceSet;
    std::mutex tsReferenceSetMutex;
    std::vector< mt::TSThread > tsThreadObjects;
};

}

#endif // ANALYZER_H
