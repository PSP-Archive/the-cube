/*
    stats.c
    
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

#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>
#include <pspctrl.h>

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "main.h"
#include "stats.h"
#include "common.h"
#include "sfx.h"

/************************************* DEFINES ***************************************/

#define STAT_FILENAME     "stats.sav"

/************************************ TYPEDEFS ***************************************/

typedef struct s_stats
{
    time_t first_run_date;
    double total_run_time;
    int nb_of_runs;
    int nb_of_cube_completed;
    int nb_of_cube_started;

}stats_t;

/************************************* STATICS ***************************************/

static stats_t stats;
static time_t this_run_date;

/********************************* LOCAL FUNCTIONS ***********************************/

int stats_load_from_file (void)
{
    SceUID fileHandle;
    int size;

    fileHandle = sceIoOpen (STAT_FILENAME, PSP_O_RDONLY, 0777);
    
    if (fileHandle < 0)
        return -1;

    size = sceIoRead (fileHandle, &stats, sizeof(stats_t));

    sceIoClose (fileHandle);

    if (size != sizeof (stats_t))
         return -2;

    return 0;    
}

int stats_write_to_file (void)
{
    SceUID fileHandle;
    int size;

    fileHandle = sceIoOpen (STAT_FILENAME, PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
    
    if (fileHandle < 0)
        return -1;

    size = sceIoWrite (fileHandle, &stats, sizeof (stats_t));
    
    sceIoClose (fileHandle);

    if (size != sizeof (stats_t))
         return -2;

    return 0;    
}

void stats_done (sfx_t *sfx)
{
    sfxSet (g_sfx_fade, 0, 255, 30, SFX_LINEAR, sfxProcessFade, NULL, NULL);            
    mainSetMode (MENU);
}

static void controls (void)
{
    if (pad.Buttons != oldPad.Buttons)
    {
        if (pad.Buttons & PSP_CTRL_CIRCLE)
        {
            if (g_sfx_fade->completed)
                sfxSet (g_sfx_fade, 255, 0, 30, SFX_LINEAR, sfxProcessFade, stats_done, NULL);        
        }
    }
}

static void render (void)
{
    char buffer [100];

    renderPlasma ();
  
    sceGuDisable (GU_DEPTH_TEST);
    sceGuBlendFunc (GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);
    sceGuEnable (GU_BLEND);

// text
    sceGuTexMode (GU_PSM_4444, 0, 0, 0);
    sceGuTexImage (0, 256, 16, 256, font88_start);
    sceGuTexFunc (GU_TFX_MODULATE, GU_TCC_RGBA);
    sceGuTexFilter (GU_NEAREST, GU_NEAREST);
    
    sprintf (buffer, "time since first run: %.0fs\n", difftime (time (NULL), stats.first_run_date));
    printTextAt (0, 0, buffer, 0xffffffff, 1);
    sprintf (buffer, "total run time: %.0fs\n", stats.total_run_time + difftime (time (NULL), this_run_date));
    printTextAt (0, 8, buffer, 0xffffffff, 1);
    sprintf (buffer, "number of runs: %d\n", stats.nb_of_runs);
    printTextAt (0, 16, buffer, 0xffffffff, 1);
    sprintf (buffer, "cubes completed: %d\n", stats.nb_of_cube_completed);
    printTextAt (0, 24, buffer, 0xffffffff, 1);
    sprintf (buffer, "cubes started: %d\n", stats.nb_of_cube_started);
    printTextAt (0, 32, buffer, 0xffffffff, 1);

    sceGuEnable (GU_DEPTH_TEST);
    sceGuDisable(GU_BLEND);
}

static void init (void)
{
    if (stats_load_from_file () != 0)
    {
        stats.first_run_date = this_run_date;
        stats.total_run_time = 0.0;
        stats.nb_of_runs = 1;
        stats.nb_of_cube_completed = 0;
        stats.nb_of_cube_started = 0;
    }
    
    this_run_date = time (NULL);
    stats.nb_of_runs++;    
}

static void dispose (void)
{
    stats.total_run_time += difftime (time (NULL), this_run_date);
    stats_write_to_file ();
}

static void on_enter (void)
{
    sfxSet (g_sfx_fade, 0, 255, 30, SFX_LINEAR, sfxProcessFade, NULL, NULL);
}

/******************************* EXPORTED FUNCTIONS **********************************/

void statsEvent (STAT_EVENT_T event)
{
    switch (event)
    {
        case STAT_MENU_ON:
            break;

        case STAT_NEW_GAME:
            stats.nb_of_cube_started++;
            break;

        case STAT_CONTINUE:
            break;

        case STAT_STATS:
            break;
 
        case STAT_CREDITS:
            break;

        case STAT_CUBE_COMPLETED:
            stats.nb_of_cube_completed++;
            break;
    }
}

void statsNew (screen_t *new)
{
    new->Init = init;
    new->Dispose = dispose;
    new->Render = render;
    new->Controls = controls;
    new->OnEnter = on_enter;
}



