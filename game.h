/*
    game.h

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

#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"

void gameNew (screen_t *);
void gameStart (void);
int gameLoadFromFile (char *);
void gameSaveToFile (char *);

#endif //#ifndef _GAME_H_