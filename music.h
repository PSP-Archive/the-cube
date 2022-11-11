/*
    music.h

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

#ifndef _MUSIC_H_
#define _MUSIC_H_

#include <mikmod.h>

#define true 1
#define false 0

int musicInit (void); 
int musicPlayMODFromMemory (unsigned char *, unsigned char *); 
int musicPlayMODFromFile (char *); 
void musicStop (void);
void musicDispose (void); 

#endif
