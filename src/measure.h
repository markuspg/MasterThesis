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

#ifndef MEASURE_H
#define MEASURE_H

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

namespace mt {

class Measure
{
public:
    Measure();
    Measure( const Measure &argMeasure ) = delete;
    Measure( Measure &argMeasure ) = delete;

    void AddTSThreadIterations( std::string &argIterationsString );
    void SetOverallIterationCount( const unsigned long &argIterationCount );
    void SetProblemParameters( const std::string &argName,
                               const unsigned long &argSize,
                               const std::string &argType );
    void SetSettingsParameters( const unsigned short &argGAInstances,
                                const double &argImmigrationRate,
                                const unsigned int &argMaxFailures,
                                const double &argMutationImpact,
                                const double &argMutationRate,
                                const std::string &argOutputFile,
                                const bool &argPromoteGlobalBestSol,
                                const bool &argRandomizedTabooTenures,
                                const bool &argRandomKeys,
                                const double &argReproductionRate,
                                const double &argTabooTenureDeviation,
                                const unsigned short &argTabooTenuresFac,
                                const unsigned short &argTSInstances );
    void WriteToDisk();

private:
    unsigned long analyzerIterations;
    unsigned short gaInstances = 1;
    double immigrationRate = 0.0;
    unsigned int maxFailures = 1000000;
    double mutationImpact = 0.1;
    double mutationRate = 0.2;
    std::string outputFile;
    std::string problemName;
    unsigned long problemSize;
    std::string problemType;
    bool promoteGlobalBestSol = false;
    bool randomizedTabooTenures = false;
    bool randomKeys = false;
    double reproductionRate = 0.5;
    double tabooTenureDeviation = 0.1;
    unsigned short tabooTenuresFac = 1;
    std::vector< std::string > tsThreadIterations;
    unsigned short tsInstances = 1;
};

}

#endif // MEASURE_H
