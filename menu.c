/*
    menu.c

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

#include <pspdisplay.h>
#include <pspctrl.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#include <pspgu.h>
#include <pspgum.h>

#include "common.h"
#include "camera.h"
#include "cube.h"
#include "game.h"
#include "main.h"
#include "stats.h"
#include "music.h"
#include "sfx.h"

/************************************ TYPEDEFS ***************************************/

// menu item state
typedef enum
{
    ENABLED,
    DISABLED
}STATE_T;

// menu item
typedef struct s_item
{
    char *caption;
    void (*callback) (int *);
    int *user_data; // pointer passed to the callback
    STATE_T state;
}item_t;

/********************************* EXTERNAL VARS *************************************/

extern float gf_cube_spin_y;
extern cam_t *g_camera;
extern sfx_t *sfx_camera1, *sfx1, *sfx2;
extern float gf_camera_phi, gf_camera_theta;
extern int gi_game_in_progress;

/************************************* STATICS ***************************************/

static item_t **g_menu; // The menu is an array of items
static int gi_nb_items;
static int gi_cursor;   // Currently selected item
static int gi_menu_enabled;

static sfx_t *sfx_move, *sfx_alpha;
static int global_menu_posx, global_menu_posy, gi_logo_alpha;
float gf_menu_cube_spin_y;

/********************************* LOCAL FUNCTIONS ***********************************/

void printWavingText (int dx, int dy, float angle, char *text, u32 color)
{
    int i;
    int srcx, srcy, dstx, dsty;

    int xoffset = 24.0 * sinf ((angle + dy) * GU_PI / 180.0);
    for (i = 0; i < strlen (text); i++)
    {
        int font_xpos, font_ypos, letter_width;

        switch (text[i])
        {
            case ' ':
                xoffset += 8;
                continue;
                //break;
            case '.':
                letter_width = 9;
                font_xpos = 7;
                font_ypos = 2;
                break;
            case ':':
                letter_width = 9;
                font_xpos = 10;
                font_ypos = 2;
                break;
            default:
                if (isalpha (text[i]))
                {
                    int c = toupper (text[i]);
                    if (c < 'M')
                    {
                        font_xpos = c - 'A';
                        font_ypos = 0;
                    }
                    else if (c < 'Y')
                    {
                        font_xpos = c - 'M';
                        font_ypos = 1;
                    }
                    else
                    {
                        font_xpos = c - 'Y';
                        font_ypos = 2;
                    }
                    if (c == 'I')
                        letter_width = 9;
                    else
                        letter_width = 20;
                }
                else if (isdigit (text[i]))
                {
                    font_xpos = text[i] - '0';
                    font_ypos = 3;
                    if (text[i] == '1')
                        letter_width = 9;
                    else
                        letter_width = 20;
                }
                else
                {
                    xoffset += 8;
                    continue;
                }
        }

        srcx = font_xpos * 21;
        srcy = font_ypos * 24;
        dstx = dx + 32 + xoffset; 
        dsty = dy + 16 + 10.0 * sinf ((angle + i * 10.0) * GU_PI / 180.0);
        xoffset += letter_width;
        
        blit (srcx, srcy, letter_width, 24, dstx, dsty, color);
    }
}

item_t *item_new (void)
{
    item_t *new;

    new = (item_t *) malloc (sizeof (item_t));
    memset (new, 0, sizeof (item_t));

    return new;
}

void item_dispose (item_t *item)
{
    if (item->caption)
        free (item->caption);
    free (item);
}

void new_game_callback_complete (sfx_t *sfx)
{
    statsEvent (STAT_NEW_GAME);
    gameStart ();
    mainSetMode (GAME);
}

void new_game_callback (int *user_data)
{
    gi_menu_enabled = 0;
    
    gf_menu_cube_spin_y = ((int)gf_menu_cube_spin_y) % 360;
    
    sfxSet (sfx1, gf_menu_cube_spin_y, gf_cube_spin_y, 30, SFX_LINEAR, sfxProcessSetFloat, NULL, &gf_menu_cube_spin_y);
    sfxSet (sfx2, gf_camera_phi, 45.0f, 30, SFX_LINEAR, sfxProcessSetFloat, NULL, &gf_camera_phi);     
    sfxSet (sfx_move, 20, -200, 30, SFX_LINEAR, sfxProcessSetInt, new_game_callback_complete, &global_menu_posx);
    sfxSet (sfx_alpha, 0xff, 0, 30, SFX_LINEAR, sfxProcessSetInt, NULL, &gi_logo_alpha);
}

void continue_callback_complete (sfx_t *sfx)
{
    statsEvent (STAT_CONTINUE);
    mainSetMode (GAME);
}

void continue_callback (int *user_data)
{
    gi_menu_enabled = 0;

    gf_menu_cube_spin_y = ((int)gf_menu_cube_spin_y) % 360;

    sfxSet (sfx1, gf_menu_cube_spin_y, gf_cube_spin_y, 30, SFX_LINEAR, sfxProcessSetFloat, NULL, &gf_menu_cube_spin_y);
    sfxSet (sfx2, gf_camera_phi, 45.0f, 30, SFX_LINEAR, sfxProcessSetFloat, NULL, &gf_camera_phi);     
    sfxSet (sfx_move, 20, -200, 30, SFX_LINEAR, sfxProcessSetInt, continue_callback_complete, &global_menu_posx);
    sfxSet (sfx_alpha, 0xff, 0, 30, SFX_LINEAR, sfxProcessSetInt, NULL, &gi_logo_alpha);
}

void stats_callback_complete (sfx_t *sfx)
{
    statsEvent (STAT_STATS);
    mainSetMode (STATS);
}

void stats_callback (int *user_data)
{
    gi_menu_enabled = 0;
    sfxSet (g_sfx_fade, 255, 0, 30, SFX_LINEAR, sfxProcessFade, stats_callback_complete, NULL);
    sfxSet (sfx_move, 20, -200, 30, SFX_LINEAR, sfxProcessSetInt, NULL, &global_menu_posx);
    sfxSet (sfx_alpha, 0xff, 0, 30, SFX_LINEAR, sfxProcessSetInt, NULL, &gi_logo_alpha);
}

void credits_callback_complete (sfx_t *sfx)
{
    statsEvent (STAT_CREDITS);
    mainSetMode (CREDITS);
}

void credits_callback (int *user_data)
{
    gi_menu_enabled = 0;
    sfxSet (g_sfx_fade, 255, 0, 30, SFX_LINEAR, sfxProcessFade, credits_callback_complete, NULL);
    sfxSet (sfx_move, 20, -200, 30, SFX_LINEAR, sfxProcessSetInt, NULL, &global_menu_posx);
    sfxSet (sfx_alpha, 0xff, 0, 30, SFX_LINEAR, sfxProcessSetInt, NULL, &gi_logo_alpha);
}

void quit_callback_complete (sfx_t *sfx)
{
    mainQuit ();
}

void quit_callback (int *user_data)
{
    gi_menu_enabled = 0;
    sfxSet (g_sfx_fade, 255, 0, 30, SFX_LINEAR, sfxProcessFade, quit_callback_complete, NULL);
    sfxSet (sfx_move, 20, -200, 30, SFX_LINEAR, sfxProcessSetInt, NULL, &global_menu_posx);
    sfxSet (sfx_alpha, 0xff, 0, 30, SFX_LINEAR, sfxProcessSetInt, NULL, &gi_logo_alpha);
}

void menu_add_item (char *caption, void (*callback) (int *), int *user_data, STATE_T state)
{
    g_menu = (item_t **) realloc (g_menu, sizeof (item_t *) * (gi_nb_items + 1));
    g_menu[gi_nb_items] = item_new ();
    g_menu[gi_nb_items]->caption = strdup (caption);
    g_menu[gi_nb_items]->state = state;
    g_menu[gi_nb_items]->callback = callback;
    g_menu[gi_nb_items]->user_data = user_data;

    gi_nb_items++;
}

static void controls (void)
{
    int i;

    if (!gi_menu_enabled)
        return;
        
    if (pad.Buttons != oldPad.Buttons)
    {
        if (pad.Buttons & PSP_CTRL_UP)
        {
            if (gi_cursor > 0)
                for (i = gi_cursor - 1; i >= 0; i--)
                    if (g_menu[i]->state == ENABLED)
                    {
                        gi_cursor = i;
                        break;
                    }
        }
        if (pad.Buttons & PSP_CTRL_DOWN)
        {
            if (gi_cursor < gi_nb_items - 1)
                for (i = gi_cursor + 1; i < gi_nb_items; i++)
                    if (g_menu[i]->state == ENABLED)
                    {
                        gi_cursor = i;
                        break;
                    }
        }
        if (pad.Buttons & PSP_CTRL_CROSS)
        {
            if (g_menu[gi_cursor]->state == ENABLED)
            {
                g_menu[gi_cursor]->callback (g_menu[gi_cursor]->user_data);
            }
        }
    }
}

static void render (void)
{
    int i, x, y;
    static float angle = 0.0;
    ScePspFVector3 lpos = {50, 50, 50};

    renderPlasma ();

    sceGuLight (0, GU_DIRECTIONAL, GU_DIFFUSE_AND_SPECULAR, &lpos);
    sceGuLightColor (0, GU_DIFFUSE_AND_SPECULAR, 0xffffffff);
    sceGuSpecular (12.0f);
    sceGuAmbientColor (0xffffffff);

    // setup matrices
    sceGumMatrixMode (GU_PROJECTION);
    sceGumLoadIdentity ();
    sceGumPerspective (30.0f, 16.0f/9.0f, 0.5f, 1000.0f);

    sceGumMatrixMode (GU_VIEW);
    sceGumLoadIdentity ();
    
    sfxProcess (sfx1);
    sfxProcess (sfx2);
    cameraRotateAroundOrigin (g_camera, gf_camera_theta, gf_camera_phi);
    
    cameraLook (g_camera);

    sceGumMatrixMode (GU_MODEL);
    sceGumLoadIdentity ();
    sceGumRotateY (gf_menu_cube_spin_y * GU_PI / 180.0);
    
    // draw cube
    cubeRender ();

    // Setup GU for 2D alpha blended rendering
    sceGuAlphaFunc (GU_GREATER, 0, 0xff);
    //sceGuEnable (GU_ALPHA_TEST);
    sceGuDisable (GU_DEPTH_TEST);
    sceGuBlendFunc (GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);
    sceGuEnable (GU_BLEND);

    sceGuTexMode (GU_PSM_4444, 0, 0, 1);
    sceGuTexImage (0, 256, 128, 256, font_start);
    sceGuTexFunc (GU_TFX_MODULATE, GU_TCC_RGBA);
    sceGuTexFilter (GU_NEAREST, GU_NEAREST);

    sfxProcess (sfx_move);

    // Render menu
    for (i = 0; i < gi_nb_items; i++)
    {
        char caption_buffer[50];

        sprintf (caption_buffer, "%s", g_menu[i]->caption);
        
        if (g_menu[i]->state == ENABLED)
            printWavingText (global_menu_posx, global_menu_posy + i * 22.0, angle, caption_buffer, 0xffffffff);
        else
            printWavingText (global_menu_posx, global_menu_posy + i * 22.0, angle, caption_buffer, 0x50ffffff);
    }

    // Render cursor
    sceGuTexMode (GU_PSM_4444, 0, 0, 1);
    sceGuTexImage (0, 512, 64, 512, stars_start);
    sceGuTexFunc (GU_TFX_MODULATE, GU_TCC_RGBA);
    sceGuTexFilter (GU_NEAREST, GU_NEAREST);
    
    y = global_menu_posy + gi_cursor * 22.0;
    x = global_menu_posx + 8 + 24.0 * sinf ((angle + y) * GU_PI / 180.0);
    y += 16 + 10.0 * sinf (angle * GU_PI / 180.0);
    blitScale (40 * (((int)(angle) % 72) / 6), 0, 40, 40, x, y, 0.5f, 0xffffffff);

    sfxProcess (sfx_alpha);
            
    // Render CUBE logo
    sceGuTexMode (GU_PSM_8888, 0, 0, 1);
    sceGuTexImage (0, 128, 64, 128, cube_logo_start);
    sceGuTexFunc (GU_TFX_MODULATE, GU_TCC_RGBA);
    sceGuTexFilter (GU_NEAREST, GU_NEAREST);

    blit (0, 0, 128, 58, (480 - 128 - 4), (272 - 58 - 4), 0x00ffffff | (gi_logo_alpha << 24));

    // Restore GU
    sceGuEnable (GU_DEPTH_TEST);
    sceGuDisable (GU_ALPHA_TEST);
    sceGuDisable (GU_BLEND);

    // Update angle (used for waving menu) 
    angle += 2.0;
    if (angle > 360.0)
        angle -= 360.0; 
}

void menu_move_complete (sfx_t *sfx)
{
    gi_menu_enabled = -1;
    sfxSet (sfx1, gf_menu_cube_spin_y, gf_menu_cube_spin_y + 360.0f, 3000, SFX_LINEAR, sfxProcessSetFloat, menu_move_complete, &gf_menu_cube_spin_y); 
}

static void on_enter (void)
{
    // Menu is disabled at first
    gi_menu_enabled = 0;
    
    // Make the menu enter the screen from the left side
    sfxSet (sfx_move, -200, 20, 30, SFX_LINEAR, sfxProcessSetInt, menu_move_complete, &global_menu_posx);
    sfxSet (sfx_alpha, 0, 0xff, 30, SFX_LINEAR, sfxProcessSetInt, NULL, &gi_logo_alpha);
    
    // Start playing menu music
    if (musicPlayMODFromMemory (menu_mod_start, menu_mod_end) < 0)
    {   
        int i;
        // wait 2s to be able to read error message (if any)
        for (i = 0; i < 120; i++) sceDisplayWaitVblankStart();
    }
    // Hide selection on the cube
    cubeSelectFace (CF_NONE);

    // Disable/Enable "continue" menu if game is completed
    if (!gi_game_in_progress)
    {
        g_menu[1]->state = DISABLED;
        // If the cursor was on "continue", move it on "new"
        if (gi_cursor == 1)
            gi_cursor = 0;
    }
    else
    {
        // Enable "continue" and make it the default    
        g_menu[1]->state = ENABLED;
        gi_cursor = 1;
    }    
}

static void on_leave (void)
{
    musicStop ();
}

static void init (void)
{
    gi_cursor = 0;
    gi_nb_items = 0;
    g_menu = NULL;
    sfx_move = sfxNew ();
    sfx_alpha = sfxNew ();
    
    global_menu_posy = 20;
    
    menu_add_item ("new game", new_game_callback, NULL, ENABLED);
    menu_add_item ("continue", continue_callback, NULL, DISABLED);
    //menu_add_item ("stats", stats_callback, NULL, ENABLED);
    menu_add_item ("credits", credits_callback, NULL, ENABLED);
    menu_add_item ("quit", quit_callback, NULL, ENABLED);
}

static void dispose (void)
{
    int i;

    for (i = 0; i < gi_nb_items; i++)
        item_dispose (g_menu[i]);

    free (g_menu);
    sfxFree (sfx_move);
    sfxFree (sfx_alpha);
}

/******************************* EXPORTED FUNCTIONS **********************************/

void menuNew (screen_t *new)
{
    new->Init = init;
    new->Dispose = dispose;
    new->Render = render;
    new->Controls = controls;
    new->OnEnter = on_enter;
    new->OnLeave = on_leave;
}

