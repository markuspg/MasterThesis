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

#include "measure.h"

mt::Measure::Measure() {
}

void mt::Measure::SetProblemParameters( const std::string &argName,
                                        const unsigned long &argSize,
                                        const std::string &argType ) {
    problemName = argName;
    problemSize = argSize;
    problemType = argType;
}

void mt::Measure::SetSettingsParameters( const unsigned short &argGAInstances,
                                         const unsigned int &argMaxFailures,
                                         const double &argMutationImpact,
                                         const double &argMutationRate,
                                         const std::string &argOutputFile,
                                         const bool &argRandomizedTabooTenures,
                                         const bool &argRandomKeys,
                                         const double &argReproductionRate,
                                         const double &argTabooTenureDeviation,
                                         const unsigned short &argTabooTenuresFac,
                                         const unsigned short &argTSInstances ) {
    gaInstances = argGAInstances;
    maxFailures = argMaxFailures;
    mutationImpact = argMutationImpact;
    mutationRate = argMutationRate;
    outputFile = argOutputFile;
    randomizedTabooTenures = argRandomizedTabooTenures;
    randomKeys = argRandomKeys;
    reproductionRate = argReproductionRate;
    tabooTenureDeviation = argTabooTenureDeviation;
    tabooTenuresFac = argTabooTenuresFac;
    tsInstances = argTSInstances;
}

void mt::Measure::WriteToDisk() {
    std::ofstream outputStream;
    outputStream.open( outputFile.c_str(), std::ios::app | std::ios::out );
    outputStream << problemName << ';' << problemType << ';' << problemSize << ';' << gaInstances << ';'
                 << maxFailures << ';' << mutationImpact << ';' << mutationRate << ';'
                 << randomizedTabooTenures << ';' << randomKeys << ';' << reproductionRate << ';'
                 << tabooTenureDeviation << ';' << tabooTenuresFac << ';'  << tsInstances << "\n";
    outputStream.close();
}
