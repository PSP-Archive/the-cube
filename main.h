/*
    main.h

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

#ifndef _MAIN_H_
#define _MAIN_H_

// enum describing the different screens
typedef enum
{
    NONE = -1,
    MENU,
    GAME,
    STATS,
    CREDITS,
    NB_SCREENS
}SCREEN_INDEX_T;

typedef struct s_screen
{
    void (* Init) (void);
    void (* Dispose) (void);
    void (* Controls) (void);
    void (* Render) (void);
    void (* OnEnter) (void);
    void (* OnLeave) (void);
}screen_t;

// This is global to avoid key rebounds between screens (since we switch between control functions)
SceCtrlData pad, oldPad;

void mainQuit (void);
void mainSetMode (SCREEN_INDEX_T);

#endif //#ifndef _MAIN_H_

