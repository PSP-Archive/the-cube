/*
    sfx.h

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

#ifndef _SFX_H_
#define _SFX_H_

typedef enum
{
    SFX_LINEAR
}SFX_FUNCTION_T;

typedef struct s_sfx
{
    float start;
    float stop;
    int nb_of_steps;
    int current_step;
    int completed;
    void *user_data;
    SFX_FUNCTION_T function;
    void (*process_callback)(struct s_sfx *, float);
    void (*complete_callback)(struct s_sfx *);
}sfx_t;


sfx_t *sfxNew (void);
void sfxSet (sfx_t *, float, float, float, SFX_FUNCTION_T, void (*)(sfx_t *, float), void (*)(sfx_t *), void *);
void sfxStop (sfx_t *);
void sfxProcess (sfx_t *);
sfx_t * sfxFree (sfx_t *);

void sfxCompleteSetIntTrue (sfx_t *);
void sfxProcessSetInt (sfx_t *, float);
void sfxProcessSetFloat (sfx_t *, float);
void sfxProcessFade (sfx_t *, float);

#endif //#ifndef _SFX_H_
