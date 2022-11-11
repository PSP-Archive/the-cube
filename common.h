/*
    common.h

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

#ifndef _COMMON_H_
#define _COMMON_H_

#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>
#include <psprtc.h>
#include <pspctrl.h>
#include <pspgu.h>
#include <pspgum.h>

#include "callbacks.h"
#include "vram.h"

#include "sfx.h"

#define BUF_WIDTH (512)
#define SCR_WIDTH (480)
#define SCR_HEIGHT (272)

#define BG_COLOR        0xff603020

// Some useful macros
#define _LSB(n) ((n) & 0xff)
#define _MSB(n) (((n) >> 8) & 0xff)

// typedefs for GU vectors
typedef struct
{
    ScePspFVector2 texture;
    u32 color;
    ScePspFVector3 normal;
    ScePspFVector3 position;
} TCNPVertex; // Texture, Color, Normal, Position
#define TCNP_VERTEX_FORMAT (GU_TEXTURE_32BITF|GU_COLOR_8888|GU_NORMAL_32BITF|GU_VERTEX_32BITF)

typedef struct
{
    ScePspFVector2 texture;
    u32 color;
    ScePspFVector3 position;
} TCPVertex; // Texture, Color, Position
#define TCP_VERTEX_FORMAT (GU_TEXTURE_32BITF|GU_COLOR_8888|GU_VERTEX_32BITF)

typedef struct
{
    u32 color;
    ScePspFVector3 normal;
    ScePspFVector3 position;
} CNPVertex; // Color, Normal, Position
#define CNP_VERTEX_FORMAT (GU_COLOR_8888|GU_NORMAL_32BITF|GU_VERTEX_32BITF)

typedef struct
{
    ScePspFVector2 texture;
    ScePspFVector3 position;
} TPVertex; // Texture, Position
#define TP_VERTEX_FORMAT (GU_TEXTURE_32BITF|GU_VERTEX_32BITF)

typedef struct
{
    ScePspFVector3 normal;
    ScePspFVector3 position;
} NPVertex; // Normal, Position
#define NP_VERTEX_FORMAT (GU_NORMAL_32BITF|GU_VERTEX_32BITF)

typedef struct
{
    u32 color;
    ScePspFVector3 position;
} CPVertex; // Color, Position
#define CP_VERTEX_FORMAT (GU_COLOR_8888|GU_VERTEX_32BITF)

// global list, for every potential render function
unsigned int __attribute__((aligned(16))) list[262144];

// framebuffer pointers 
void* fbp0;
void* fbp1;
void* zbp;
void* rtp;

// external data
extern unsigned char devsgen_start[];
extern unsigned char sebonpsp1_start[];
extern unsigned char sebonpsp2_start[];
extern unsigned char font_start[];
extern unsigned char font88_start[];
extern unsigned char alpha_circle_start[];
extern unsigned char controls_start[];
extern unsigned char stars_start[];
extern unsigned char cube_logo_start[];
// music
extern unsigned char menu_mod_start[];
extern unsigned char menu_mod_end[];
extern unsigned char credits_mod_start[];
extern unsigned char credits_mod_end[];
extern unsigned char success_mod_start[];
extern unsigned char success_mod_end[];

// global fade effect
sfx_t *g_sfx_fade;

// various rendering functions
void blit (int sx, int sy, int sw, int sh, int dx, int dy, u32 color);
void blitScale (int sx, int sy, int sw, int sh, int dx, int dy, float scale, u32 color);
void drawRectangle (int x1, int y1, int x2, int y2, u32 color);
void printTextAt (int x, int y, char *text, u32 color, int drop_shadow);
void renderPlasma (void);
void screenshot (void);

#endif
