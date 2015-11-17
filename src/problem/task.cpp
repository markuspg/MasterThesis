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

#include "task.h"

mt::Task::Task( const unsigned long &argDuration, const unsigned long &argIndex,
                const std::vector< Task* > * argPredecessors ) :
    duration{ argDuration },
    index{ argIndex },
    predecessors{ argPredecessors }
{
    if ( predecessors->empty() ) {
        allPredecessorsScheduled = true;
    }
}

mt::Task::~Task() {
    delete predecessors;
}

bool mt::Task::AllPredecessorsScheduled() {
    if ( allPredecessorsScheduled ) {
        return true;
    } else {
        for ( auto cit = predecessors->cbegin(); cit != predecessors->cend(); ++cit ) {
            if ( !( *cit )->IsScheduled() ) {
                return false;
            }
        }
    }
    allPredecessorsScheduled = true;
    return true;
}