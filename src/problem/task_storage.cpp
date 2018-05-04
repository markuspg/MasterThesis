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

#include "../helper_functions.h"

/*!
 * \brief This copy-constructor got implemented manually to create deep copies
 * \param argTaskStorage The TaskStorage object to be copied
 */
mt::TaskStorage::TaskStorage(const TaskStorage &argTaskStorage) :
    tasks(argTaskStorage.tasks.size(), nullptr)
{
    const auto size = argTaskStorage.tasks.size();
    for (auto i = decltype(size){0}; i < size; ++i) {
        std::vector<Task*> predecessors;
        for (auto cit = argTaskStorage.tasks[i]->predecessors.cbegin();
             cit != argTaskStorage.tasks[i]->predecessors.cend(); ++cit) {
            predecessors.emplace_back(tasks[(*cit)->index - 1]);
        }
        tasks[i] = new Task{argTaskStorage.tasks[i]->duration,
                            argTaskStorage.tasks[i]->index,
                            std::move(predecessors)};
    }
}

mt::TaskStorage::TaskStorage(const unsigned long argSize,
                             const std::vector<std::string> &argTokens) :
    tasks(argSize, nullptr)
{
    // Split the tokens for their usage whilst the Tasks' construction
    const auto durationStrings = tools::Split(argTokens[3], ';');
    const auto precedenceStrings = tools::Split(argTokens[4], ';');

    // Construct all tasks, one after another
    for (unsigned long i = 0; i < argSize; ++i) {
        // First declare and initialize the needed variables
        std::vector<Task*> predecessors;
        unsigned long taskDuration = 0;
        unsigned long taskID = 0;

        // Then convert the string values to the needed data items
        taskDuration = std::stoul(durationStrings.at(i));
        const auto precedenceString = tools::Split(precedenceStrings.at(i), ':');
        taskID = std::stoul(precedenceString.at(0));
        if (precedenceString.size() > 1) {
            const auto predecessorIndicesString
                    = tools::Split(precedenceString.at(1), ',');
            for (std::size_t j = 0; j < predecessorIndicesString.size(); ++j) {
                const unsigned long predecessorIndexString
                        = std::stoul(predecessorIndicesString.at(j)) - 1;
                // Ensure lexicographical ordering of the tasks
                assert(predecessorIndexString < i);
                predecessors.emplace_back(tasks.at(predecessorIndexString));
            }
        }
        tasks[i] = new Task{taskDuration, taskID, std::move(predecessors)};
    }
}

mt::TaskStorage::~TaskStorage() {
    for (const auto taskPtr : tasks) {
        delete taskPtr;
    }
}
