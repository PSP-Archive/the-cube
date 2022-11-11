/*
    sfx.c

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
/************************************ INCLUDES ***************************************/

#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "sfx.h"

/******************************* EXPORTED FUNCTIONS **********************************/

sfx_t *sfxFree (sfx_t *sfx)
{
    free (sfx);
    return NULL;
}

void sfxSet (sfx_t *sfx, float start, float stop, float nb_of_steps, SFX_FUNCTION_T function, void (*process_callback)(sfx_t *, float), void (*complete_callback)(sfx_t *), void *user_data)
{
    sfx->start = start;
    sfx->stop = stop;
    sfx->nb_of_steps = nb_of_steps;
    sfx->function = function;
    sfx->user_data = user_data;
    sfx->process_callback = process_callback;
    sfx->complete_callback = complete_callback;
    sfx->current_step = 0;
    sfx->completed = 0;
}

sfx_t *sfxNew (void)
{
    sfx_t *new;

    new = (sfx_t *)malloc (sizeof (sfx_t));
    new->completed = -1;

    return new;
}

void sfxStop (sfx_t *sfx)
{
    sfx->completed = -1;
}

void sfxProcess (sfx_t *sfx)
{
    if (sfx->completed)
        return;

    switch (sfx->function)
    {
        case SFX_LINEAR:
            if (sfx->process_callback)
                sfx->process_callback (sfx, sfx->start + (float)(sfx->current_step) *
                                            (float)((sfx->stop - sfx->start) /
                                            (float)sfx->nb_of_steps));
            break;
    }

    sfx->current_step++;

    if (sfx->current_step > sfx->nb_of_steps)
    {
        sfx->completed = -1;
        if (sfx->complete_callback)
            sfx->complete_callback (sfx);
    }
}

void sfxCompleteSetIntTrue (sfx_t *sfx)
{
    int *p = (int *)sfx->user_data;

    *p = -1;
}

void sfxProcessSetInt (sfx_t *sfx, float value)
{
    int *p = (int *)sfx->user_data;

    *p = (int)value;
}

void sfxProcessSetFloat (sfx_t *sfx, float value)
{
    float *p = (float *)sfx->user_data;

    *p = (float)value;
}

void sfxProcessFade (sfx_t *sfx, float value)
{
    u32 color = 0xff - (int)value;
    color <<= 24;

    sceGuDisable (GU_TEXTURE_2D);
    sceGuDisable (GU_DEPTH_TEST);
    sceGuBlendFunc (GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);
    sceGuEnable(GU_BLEND);

    drawRectangle (0, 0, SCR_WIDTH, SCR_HEIGHT, color);

    sceGuEnable (GU_DEPTH_TEST);
    sceGuDisable(GU_BLEND);
    sceGuEnable (GU_TEXTURE_2D);
}

