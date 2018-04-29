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

#include "gathread.h"

mt::GAThread::GAThread(const unsigned short argIndex, std::mutex &argMutex,
                       const mt::Problem * const argProblem,
                       mt::TSReferenceSet &argReferenceSet) :
    index{argIndex},
    popSize{argProblem->size * argProblem->size / 2 - argProblem->size},
    immigrationsQuantity{static_cast<unsigned long>(
                         std::round(popSize * *settings->immigrationRate))},
    population{popSize, {0.0, nullptr}},
    problem{argProblem},
    mutationsQuantity{static_cast<unsigned long>(std::llround(popSize
                                                              * *settings->mutationRate))},
    reproductionQuantity{*settings->reproductionRate * popSize / 2 < 1
                         ? 1
                         : static_cast<unsigned long>(*settings->reproductionRate * popSize / 2)},
    tsReferenceSetMutex{argMutex},
    referenceSet{argReferenceSet}
{
    // std::cout << "      Constructing GAThread with id " << index << "\n";

    CreateInitialPopulation();
}

mt::GAThread::~GAThread() {
    for (auto &s: population) {
        delete s.second;
    }
}

void mt::GAThread::CreateInitialPopulation() {
    for (unsigned long i = 0; i < popSize; ++i) {
        population[i].second = problem->GenerateRandomSolution(i);
        population[i].first = problem->GetOFV(population[i].second);
    }
    std::sort(population.begin(), population.end(),
            [](const dSol &a, const dSol &b){
        return a.first < b.first;
    });
}

void mt::GAThread::Immigrate() {
    std::sort(population.begin(), population.end(),
            [](const dSol &a, const dSol &b){
        return a.first < b.first;
    });

    for (unsigned long i = 1; i <= immigrationsQuantity; ++i) {
        delete population[popSize - i].second;
        population[popSize - i].second = problem->GenerateRandomSolution(i);
        population[popSize - i].first = problem->GetOFV(population[popSize - i].second);
    }
}

void mt::GAThread::Iteration() {
    ++iterationCount;

    {
        std::lock_guard<std::mutex> lockTSReferenceSet{tsReferenceSetMutex};
        // Replace the weakest individuum from the population with one from the
        // reference set
        delete population[popSize - 1].second;
        // Since the GAs' solutions are stored 'behind' the TSs' solutions, the
        // index has to be shifted
        population[popSize - 1].second =
                referenceSet.GetStartSolution(index + *settings->tsInstances);
        population[popSize - 1].first =
                referenceSet.GetStartSolutionValue(index + *settings->tsInstances);
    }

    Reproduce();

    if (mutationsQuantity) {
        Mutate();
    }

    if (immigrationsQuantity) {
        Immigrate();
    }

    // Re-sort the population for the next iteration
    std::sort(population.begin(), population.end(),
            [](const dSol &a, const dSol &b){
        return a.first < b.first;
    });

    std::lock_guard<std::mutex> lockTSReferenceSet{tsReferenceSetMutex};
    // Since the GAs' solutions are stored 'behind' the TSs' solutions, the
    // index has to be shifted
    referenceSet.SetSolution("GA", index + *settings->tsInstances,
                             population[0].second->Copy(),
                             population[0].first);
}

void mt::GAThread::Mutate() {
    std::random_device randomDevice;
    std::mt19937_64 engine{randomDevice() + index};
    std::uniform_int_distribution<unsigned long> distribution{0, popSize - 1};

    for (unsigned long i = 0; i < mutationsQuantity; ++i) {
        const unsigned long mutateIndex = distribution(engine);
        population[mutateIndex].second->Mutate(engine);
        population[mutateIndex].first = problem->GetOFV(population[mutateIndex].second);
    }
}

void mt::GAThread::Reproduce() {
    std::random_device randomDevice;
    std::mt19937_64 engine{randomDevice()};
    std::uniform_real_distribution<> parentsDistribution{0.0, 1.0};

    for (unsigned long i = 0; i < reproductionQuantity; ++i) {
        std::pair<unsigned long, unsigned long> parentIndices;
        SelectParentsMonteCarlo(parentsDistribution(engine),
                                parentsDistribution(engine),
                                parentIndices);

        std::uniform_int_distribution<unsigned long> distribution{0,
                                                                  problem->size};
        const unsigned long crossoverPoint = distribution(engine);
        mt::SolutionBase *childA
                = population[parentIndices.first].second
                    ->ReproduceWithOtherParent(crossoverPoint,
                                               population[parentIndices.second].second);
        mt::SolutionBase *childB
                = population[parentIndices.second].second
                    ->ReproduceWithOtherParent(crossoverPoint,
                                               population[parentIndices.first].second);
        delete population[parentIndices.first].second;
        population[parentIndices.first].second = childA;
        population[parentIndices.first].first
                = problem->GetOFV(population[parentIndices.first].second);
        delete population[parentIndices.second].second;
        population[parentIndices.second].second = childB;
        population[parentIndices.second].first
                = problem->GetOFV(population[parentIndices.second].second);
    }
}

void mt::GAThread::SelectParentsMonteCarlo(
        const double argParentAValue, const double argParentBValue,
       std::pair<unsigned long, unsigned long> &argParentIndices) {
    std::vector<double> probabilities;
    probabilities.resize(popSize, 0.0);
    double sum = 0.0;
    // Use the inverted values as probabilities, since smaller values should be
    // chosen more likely
    for (unsigned long i = 0; i < popSize; ++i) {
        probabilities[i] = 1 / population[i].first;
        sum += probabilities[i];
    }

    // Turn the roulette wheel
    double parentAValue = argParentAValue * sum;
    double parentBValue = argParentBValue * sum;
    // Find the corresponding solutions
    double accum = 0.0;
    for (unsigned long i = 0; i < popSize; ++i) {
        accum += probabilities[i];
        if (!argParentIndices.first && accum > parentAValue) {
            argParentIndices.first = i;
        }
        if (!argParentIndices.second && accum > parentBValue) {
            argParentIndices.second = i;
        }
        if (argParentIndices.first && argParentIndices.second) {
            break;
        }
    }
}
