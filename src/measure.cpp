/*
 * Copyright 2015-2018 Markus Prasser
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

void mt::Measure::AddTSThreadIterations(std::string &argIterationsString) {
    tsThreadIterations.emplace_back(argIterationsString);
}

void mt::Measure::SetFinalStats(const double argFinMedian,
                                const double argFinMedium) {
    finalMedian = argFinMedian;
    finalMedium = argFinMedium;
}

void mt::Measure::SetGlobalBestDevelopment(const std::string &argGlobalBestDevelopment) {
    globalBestDevelopment = argGlobalBestDevelopment;
    globalBestDevelopment.pop_back();
}

void mt::Measure::SetInitializationStats(const double argIniMedian,
                                         const double argIniMedium) {
    initializationMedian = argIniMedian;
    initializationMedium = argIniMedium;
}

void mt::Measure::SetOptimizationStats(const double argOptMedian,
                                       const double argOptMedium) {
    optimizationMedian = argOptMedian;
    optimizationMedium = argOptMedium;
}

void mt::Measure::SetOverallIterationCount(const unsigned long argIterationCount) {
    analyzerIterations = argIterationCount;
}

void mt::Measure::SetProblemParameters(const std::string &argName,
                                       const unsigned long argSize,
                                       const std::string &argType) {
    problemName = argName;
    problemSize = argSize;
    problemType = argType;
}

void mt::Measure::SetSettingsParameters(const unsigned short argGAInstances,
                                        const bool argGlobalBestAspiration,
                                        const double argImmigrationRate,
                                        const unsigned int argMaxFailures,
                                        const double argMutationImpact,
                                        const double argMutationRate,
                                        const std::string &argOutputFile,
                                        const bool argPromoteGlobalBestSol,
                                        const bool argRandomizedTabooTenures,
                                        const bool argRandomKeys,
                                        const double argReproductionRate,
                                        const double argTabooTenureDeviation,
                                        const unsigned short argTabooTenuresFac,
                                        const bool argTabooTenureShuffling,
                                        const unsigned short argTSInstances ) {
    gaInstances = argGAInstances;
    globalBestAspiration = argGlobalBestAspiration;
    immigrationRate = argImmigrationRate;
    maxFailures = argMaxFailures;
    mutationImpact = argMutationImpact;
    mutationRate = argMutationRate;
    outputFile = argOutputFile;
    promoteGlobalBestSol = argPromoteGlobalBestSol;
    randomizedTabooTenures = argRandomizedTabooTenures;
    randomKeys = argRandomKeys;
    reproductionRate = argReproductionRate;
    tabooTenureDeviation = argTabooTenureDeviation;
    tabooTenuresFac = argTabooTenuresFac;
    tabooTenureShuffling = argTabooTenureShuffling;
    tsInstances = argTSInstances;

    tsThreadIterations.reserve(tsInstances);
}

void mt::Measure::StartTimer() {
    start = std::chrono::system_clock::now();
}

void mt::Measure::WriteToDisk() {
    // Optimization execution time measuring
    std::chrono::system_clock::time_point finish = std::chrono::system_clock::now();
    auto optimizationDuration = finish - start;

    std::stringstream threadIterationsStream;
    threadIterationsStream << tsThreadIterations.front();
    for (const auto &tsThreadIteration : tsThreadIterations) {
        threadIterationsStream << ';' << tsThreadIteration;
    }
    std::string threadIterations = threadIterationsStream.str();
    std::ofstream outputStream;
    outputStream.open(outputFile.c_str(), std::ios::app | std::ios::out);
    if (!outputStream.tellp()) {
        outputStream << "problemName|PType|PSize|GAI|GBA|IR|MF|MI|MR|PGB|RTT|RK"
                        "|RR|TTD|TTF|TTS|TSI|AnIt|ThIt|iniMian|iniMium|optMian"
                        "|optMium|finMian|finMium|GBD|dur\n";
    }
    outputStream << problemName << '|' << problemType << '|' << problemSize
                 << '|' << gaInstances << '|' << globalBestAspiration
                 << '|' << immigrationRate << '|' << maxFailures
                 << '|' << mutationImpact << '|' << mutationRate
                 << '|' << promoteGlobalBestSol << '|' << randomizedTabooTenures
                 << '|' << randomKeys << '|' << reproductionRate
                 << '|' << tabooTenureDeviation << '|' << tabooTenuresFac
                 << '|' << tabooTenureShuffling << '|' << tsInstances
                 << '|' << analyzerIterations << '|' << threadIterations
                 << '|' << initializationMedian << '|' << initializationMedium
                 << '|' << optimizationMedian << '|' << optimizationMedium
                 << '|' << finalMedian << '|' << finalMedium
                 << '|' << globalBestDevelopment << '|'
                 << std::chrono::duration_cast<std::chrono::milliseconds>(
                        optimizationDuration).count()
                 << "\n";
    outputStream.flush();
    outputStream.close();

    // Clean up any data which will not be automatically overwritten in the next run
    tsThreadIterations.clear();
}
