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

#ifndef TABOOSEARCHPROCESSORSETTINGS_H
#define TABOOSEARCHPROCESSORSETTINGS_H

#include "matrix.h"
#include "randomkeysolution.h"

namespace mt {

struct TSProcessorSettings {
public:
    TSProcessorSettings( const unsigned int &argTabooTenure,
                         mt::Matrix< unsigned int > * const argTabooTenures );

    unsigned int GetFailures() const { return failures; }
    void IncFailures() { ++failures; }
    void IncInvalidSolutions() { ++invalidSolutions; }
    bool IsFinished() const { return finished; }
    void SetFinished() { finished = true; }

    mt::RandomKeySolution *bestSol = nullptr;
    double bestSolV = std::numeric_limits< double >::max();
    const unsigned int tabooTenure = 0;
    mt::Matrix< unsigned int > * const tabooTenures = nullptr;

private:
    unsigned int failures = 0;
    bool finished = false;
    unsigned int invalidSolutions = 0;
};

}

#endif // TABOOSEARCHPROCESSORSETTINGS_H
