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

#include "helper_functions.h"

mt::RandomKeySolution *mt::GetBestNeighbour( const unsigned short &argIndex,
                                             const unsigned int &argIterationCounter,
                                             const mt::Problem * const argProblem,
                                             mt::TSProcessorSettings * const argProcessorSettings,
                                             const mt::RandomKeySolution * const argSolution ) {
    mt::Matrix< double > costs{ static_cast< int >( argProblem->size ),
                                std::numeric_limits< double >::max() };
    // Iterate over all lines
    for ( unsigned long i = 0; i < argProblem->size; ++i ) {
        // Iterate over all columns above the main diagonal (because swaps are symmetrical)
        for ( unsigned long j = i + 1; j < argProblem->size; ++j ) {
            mt::RandomKeySolution *tempSolution =  argSolution->GetSwappedVariant( i, j );
            costs( i, j ) = argProblem->GetOFV( tempSolution );
            delete tempSolution;
        }
    }
    // Analyze all swaps
    double costV = 0;
    long swapI = 0, swapJ = 0;
    while ( true ) {
        costV = costs.GetMinimumValueIndizes( swapI, swapJ );
        // If no improvement could be achieved, leave with false, indicating improvement failure
        if ( costV == std::numeric_limits< double >::max() ) {
            return nullptr;
        }
        // If a swap is taboo, exclude it from consideration by settings its cost to the maximum double value
        if ( ( *argProcessorSettings->tabooTenures )( swapI, swapJ ) >= argIterationCounter
             // New global optimum as aspiration criterion
             && costV > argProcessorSettings->bestSolV ) {
            costs( swapI, swapJ ) = std::numeric_limits< double >::max();
        } else {
            // Taillard's taboo evaluation => constant time
            ( *argProcessorSettings->tabooTenures )( swapI, swapJ ) =
                    argIterationCounter + argProcessorSettings->tabooTenure;
            return argSolution->GetSwappedVariant( swapI, swapJ );
        }
    }
}

unsigned int mt::GetRandomizedTT( const unsigned int &argPS ) {
    std::random_device randomDevice;
#ifdef Q_PROCESSOR_X86_64
    std::mt19937_64 engine{ randomDevice() };
#else
    std::mt19937 engine{ randomDevice() };
#endif
    std::uniform_int_distribution<> distribution{
        static_cast< int > ( argPS * ( 1.0 - *settings->tabooTenureDeviation ) ),
        static_cast< int > ( argPS * ( 1.0 + *settings->tabooTenureDeviation ) ) };

    return distribution( engine );
}

mt::Problem *mt::LoadProblem( const std::string &argLine ) {
    std::vector< std::string > problemTokens = mt::Split( argLine, '|' );
    if ( problemTokens[ 1 ] == "QAP" ) {
        return new mt::QAP{ problemTokens };
    }
    if ( problemTokens[ 1 ] == "SALBP" ) {
    }
    return nullptr;
}

int mt::ParseCommandLine( int argC, char *argV[] ) {
    // A temporary storage for the extracted strings
    std::vector< std::string > commandLineArguments{};
    commandLineArguments.reserve( argC );
    
    // The variables for the data
    unsigned short tempGAInstances = 0;
    unsigned int tempMaxFailures = 0;
    std::string tempOutputFile;
    std::vector< std::string > tempProblemFiles;
    bool tempRandomizedTabooTenures = false;
    double tempTabooTenureDeviation = 0;
    unsigned short tempTabooTenureFac = 0;
    unsigned short tempTSInstances = 0;
    
    // Convert all arguments to strings and organize them in a vector
    for ( unsigned short i = 0; i < argC; ++i ) {
        commandLineArguments.emplace_back( argV[ i ] );
    }
    
    unsigned int lastActiveIndex = 0;
    // Store relevant options to the settings
    for ( unsigned short i = 0; i < argC; ++i ) {
        if ( argC < 2 || commandLineArguments[ i ] == "--help" || commandLineArguments[ i ] == "-h" ) {
            std::cout << "MasterThesis is a parallelized adaptive memory heuristic implementation\n"
                         "for my master thesis. The command line arguments are the following:\n\n"
                         "\t--ga <gaInstances>    The desired number of genetic algorithm threads\n"
                         "\t--ts <tsInstances>    The desired number of taboo search threads\n"
                         "\t--mf <maxFailures>    The number of allowed improvement failures,\n"
                         "\t                      before the search terminates\n"
                         "\t--rtt <randTTenures>  If the taboo tenures shall be randomized {0;1}\n"
                         "\t--ttd <tTenureDev>    The spread factor for randomized taboo tenures\n"
                         "\t--ttf <tTenureFac>    How many times the problem size\n"
                         "\t                      the taboo tenure shall last" << std::endl;
            return 1;
        }
        if ( commandLineArguments[ i ] == "--ga" ) {
            tempGAInstances = std::stoul( commandLineArguments[ i + 1 ] );
            lastActiveIndex = i + 1;
            continue;
        }
        if ( commandLineArguments[ i ] == "--mf" ) {
            tempMaxFailures = std::stoul( commandLineArguments[ i + 1 ] );
            lastActiveIndex = i + 1;
            continue;
        }
        if ( commandLineArguments[ i ] == "--rtt" ) {
            tempRandomizedTabooTenures = static_cast< bool >( std::stoul( commandLineArguments[ i + 1 ] ) );
            lastActiveIndex = i + 1;
            continue;
        }
        if ( commandLineArguments[ i ] == "--ttd" ) {
            tempTabooTenureDeviation = std::stod( commandLineArguments[ i + 1 ] );
            lastActiveIndex = i + 1;
            continue;
        }
        if ( commandLineArguments[ i ] == "--ttf" ) {
            tempTabooTenureFac = std::stoul( commandLineArguments[ i + 1 ] );
            lastActiveIndex = i + 1;
            continue;
        }
        if ( commandLineArguments[ i ] == "--ts" ) {
            tempTSInstances = std::stoul( commandLineArguments[ i + 1 ] );
            lastActiveIndex = i + 1;
            continue;
        }
    }

    for ( unsigned int i = lastActiveIndex + 1; i < static_cast< unsigned int >( argC ) - 1; ++i ) {
        tempProblemFiles.emplace_back( argV[ i ] );
    }

    tempOutputFile = commandLineArguments.back();
    
    settings = new mt::Settings{ tempGAInstances, tempMaxFailures, tempOutputFile,
                                 std::move( tempProblemFiles ), tempRandomizedTabooTenures,
                                 tempTabooTenureDeviation, tempTabooTenureFac, tempTSInstances };
    
    return 0;
}

std::vector<std::string> mt::Split( const std::string &argString, char argDelim ) {
    std::vector<std::string> elements;
    mt::Split( argString, argDelim, elements );
    return elements;
}

void mt::Split( const std::string &argString, char argDelim, std::vector<std::string> &argElements ) {
    std::stringstream stringStream{ argString };
    std::string item;
    while (std::getline( stringStream, item, argDelim ) ) {
        argElements.push_back( item );
    }
}
