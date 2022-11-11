/*
    stats.h
    
    Copyright 2007 Sebastien Delestaing
    
    This file is part of "the_cube".

    "the_cube" is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    "the_cube" is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _STATS_H_
#define _STATS_H_

#include "main.h"

typedef enum
{
    STAT_MENU_ON,
    STAT_NEW_GAME,
    STAT_CONTINUE,
    STAT_STATS,
    STAT_CREDITS,
    STAT_CUBE_COMPLETED
}STAT_EVENT_T;

void statsNew (screen_t *);
void statsEvent (STAT_EVENT_T);

#endif //#ifndef _STATS_H_
