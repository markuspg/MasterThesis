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

#include "settings.h"

mt::Settings::Settings( const unsigned short &argGAInstances, const bool &argGlobalBestAspiration,
                        const double &argImmigrationRate, const unsigned int &argMaxFailures,
                        const double &argMutationImpact, const double &argMutationRate,
                        const std::string &argOutputFile, std::vector< std::string > &&argProblemFiles,
                        const bool &argPromoteGlobalBestSol, const bool &argRandomizedTabooTenures,
                        const bool &argRandomKeys, const double &argReproductionRate,
                        const double &argTabooTenureDeviation, const unsigned short &argTabooTenureFac,
                        const bool &argTabooTenureShuffling, const unsigned short &argTSInstances ) :
    gaInstances{ new unsigned short{ argGAInstances } },
    globalBestAspiration{ new bool{ argGlobalBestAspiration } },
    immigrationRate{ new double{ argImmigrationRate } },
    maxFailures{ new unsigned int{ argMaxFailures } },
    mutationImpact{ new double{ argMutationImpact } },
    mutationRate{ new double{ argMutationRate } },
    outputFile{ new std::string{ argOutputFile } },
    problemFiles{ new std::vector< std::string >{ argProblemFiles } },
    promoteGlobalBestSol{ new bool{ argPromoteGlobalBestSol } },
    randomizedTabooTenures{ new bool{ argRandomizedTabooTenures } },
    randomKeys{ new bool{ argRandomKeys } },
    reproductionRate{ new double{ argReproductionRate } },
    tabooTenureDeviation{ new double { argTabooTenureDeviation } },
    tabooTenuresFac{ new unsigned short{ argTabooTenureFac } },
    tabooTenureShuffling{ new bool{ argTabooTenureShuffling } },
    tsInstances{ new unsigned short{ argTSInstances } }
{
    std::cout << "  Settings constructor" << std::endl;
    std::cout << "   gaInstances:\t\t\t" << *gaInstances
              << "\n   globalBestAspiration:\t" << *globalBestAspiration
              << "\n   immigrationRate:\t\t" << *immigrationRate
              << "\n   maxFailures:\t\t\t" << *maxFailures
              << "\n   mutationImpact:\t\t" << *mutationImpact
              << "\n   mutationRate:\t\t" << *mutationRate
              << "\n   outputFile:\t\t\t" << *outputFile
              << "\n   problemFiles:\t\t" << CreateStringOfProblemFiles()
              << "\n   promoteGlobalBestSol:\t" << *promoteGlobalBestSol
              << "\n   randomizedTabooTenures:\t" << *randomizedTabooTenures
              << "\n   randomKeys:\t\t\t" << *randomKeys
              << "\n   reproductionRate:\t\t" << *reproductionRate
              << "\n   tabooTenureDeviation:\t" << *tabooTenureDeviation
              << "\n   tabooTenuresFac:\t\t" << *tabooTenuresFac
              << "\n   tabooTenureShuffling:\t" << *tabooTenureShuffling
              << "\n   tsInstances:\t\t\t" << *tsInstances << std::endl;

    std::lock_guard< std::mutex > lockMeasure{ measureMutex };
    measure.SetSettingsParameters( *gaInstances, *globalBestAspiration, *immigrationRate, *maxFailures,
                                   *mutationImpact, *mutationRate, *outputFile, *promoteGlobalBestSol,
                                   *randomizedTabooTenures, *randomKeys, *reproductionRate,
                                   *tabooTenureDeviation, *tabooTenuresFac, *tabooTenureShuffling,
                                   *tsInstances );
}

mt::Settings::~Settings() {
    std::cout << " Settings destructor" << std::endl;
    delete gaInstances;
    delete globalBestAspiration;
    delete immigrationRate;
    delete maxFailures;
    delete mutationImpact;
    delete mutationRate;
    delete outputFile;
    delete problemFiles;
    delete promoteGlobalBestSol;
    delete randomizedTabooTenures;
    delete randomKeys;
    delete reproductionRate;
    delete tabooTenureDeviation;
    delete tabooTenuresFac;
    delete tabooTenureShuffling;
    delete tsInstances;
}

std::string mt::Settings::CreateStringOfProblemFiles() const {
    std::string output;
    for ( auto cit = problemFiles->cbegin(); cit != problemFiles->cend() - 1; ++cit ) {
        output.append( *cit );
        output.append( 1, ';' );
    }
    output.append( problemFiles->back() );

    return output;
}
