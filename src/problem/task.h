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

#ifndef TASK_H
#define TASK_H

#include <vector>

namespace mt {

class Task final {
public:
    Task( const unsigned long &argDuration, const unsigned long &argIndex,
          const std::vector< Task* > *argPredecessors );
    Task( const Task &argTask ) = delete;
    Task( Task &&argTask ) = delete;
    ~Task();

    bool AllPredecessorsScheduled();
    bool IsScheduled() const { return isScheduled; }
    void TaskGotScheduled() { isScheduled = true; }

    const unsigned long duration = 0;
    const unsigned long index = 0;
    const std::vector< Task* > * const predecessors = nullptr;

private:
    bool allPredecessorsScheduled = false;
    bool isScheduled = false;
};

}

#endif // TASK_H
