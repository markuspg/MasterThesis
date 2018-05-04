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

#ifndef TASKSTORAGE_H
#define TASKSTORAGE_H

#include <string>
#include <vector>

namespace mt {

class Task;

class TaskStorage final {
public:
    TaskStorage() = delete;
    TaskStorage(const TaskStorage &argTaskStorage);
    TaskStorage(TaskStorage &&argTaskStorage) = default;
    TaskStorage(const unsigned long argSize,
                const std::vector<std::string> &argTokens);
    ~TaskStorage();

    Task *operator() (const unsigned long argIndex);
    Task *operator() (const unsigned long argIndex) const;

private:
    std::vector<Task*> tasks;
};

} // namespace mt

inline mt::Task *mt::TaskStorage::operator() (const unsigned long argIndex) {
    return tasks[argIndex];
}

inline mt::Task *mt::TaskStorage::operator() (const unsigned long argIndex) const {
    return tasks[argIndex];
}

#endif // TASKSTORAGE_H
