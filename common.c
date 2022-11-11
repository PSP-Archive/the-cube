/*
    common.c

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

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <time.h>

#include "common.h"

/******************************* EXPORTED FUNCTIONS **********************************/

// This blits a 2D sprite with scaling (keeps aspect ratio)
void blitScale (int sx, int sy, int sw, int sh, int dx, int dy, float scale, u32 color)
{
    // Allocate vertices on the list
    TCPVertex *vertices = (TCPVertex *)sceGuGetMemory (2 * sizeof (TCPVertex));

    // Init vertex 1
    vertices[0].texture.x = sx; vertices[0].texture.y = sy;
    vertices[0].color = color;
    vertices[0].position.x = dx; vertices[0].position.y = dy; vertices[0].position.z = 0;

    // Init vertex 2
    vertices[1].texture.x = sx + sw; vertices[1].texture.y = sy + sh;
    vertices[1].color = color;
    vertices[1].position.x = dx + sw * scale; vertices[1].position.y = dy + sh * scale; vertices[1].position.z = 0;

    // Draw sprite
    sceGuDrawArray (GU_SPRITES, TCP_VERTEX_FORMAT | GU_TRANSFORM_2D, 2, 0, vertices);
}

// This blits a 2D sprite
void blit (int sx, int sy, int sw, int sh, int dx, int dy, u32 color)
{
    // Allocate vertices on the list
    TCPVertex *vertices = (TCPVertex *)sceGuGetMemory (2 * sizeof (TCPVertex));

    // Init vertex 1
    vertices[0].texture.x = sx; vertices[0].texture.y = sy;
    vertices[0].color = color;
    vertices[0].position.x = dx; vertices[0].position.y = dy; vertices[0].position.z = 0;

    // Init vertex 2
    vertices[1].texture.x = sx + sw; vertices[1].texture.y = sy + sh;
    vertices[1].color = color;
    vertices[1].position.x = dx + sw; vertices[1].position.y = dy + sh; vertices[1].position.z = 0;

    // Draw sprite
    sceGuDrawArray (GU_SPRITES, TCP_VERTEX_FORMAT | GU_TRANSFORM_2D, 2, 0, vertices);
}

// Draw a filled rectangle with given color (RGBA)
void drawRectangle (int x1, int y1, int x2, int y2, u32 color)
{
    // Allocate 4 vertices on the list
    CPVertex *vertices = (CPVertex*) sceGuGetMemory (4 * sizeof(CPVertex));

    // Init the 4 corners as a strip (1 triangle + 1 corner)
    vertices[0].color = color;
    vertices[0].position.x = x1;
    vertices[0].position.y = y1;
    vertices[0].position.z = 0;

    vertices[1].color = color;
    vertices[1].position.x = x2;
    vertices[1].position.y = y1;
    vertices[1].position.z = 0;

    vertices[2].color = color;
    vertices[2].position.x = x1;
    vertices[2].position.y = y2;
    vertices[2].position.z = 0;

    vertices[3].color = color;
    vertices[3].position.x = x2;
    vertices[3].position.y = y2;
    vertices[3].position.z = 0;

    // Draw the rectangle
    sceGuDrawArray (GU_TRIANGLE_STRIP, CP_VERTEX_FORMAT | GU_TRANSFORM_2D, 4, 0, vertices);
}

// Print text using the 8*8 font (with optional drop shadow)
void printTextAt (int x, int y, char *text, u32 color, int drop_shadow)
{
    int i;

    // Init texture for the 8*8 font
    sceGuTexMode (GU_PSM_4444, 0, 0, 0);
    sceGuTexImage (0, 256, 16, 256, font88_start);
    sceGuTexFunc (GU_TFX_MODULATE, GU_TCC_RGBA);
    sceGuTexFilter (GU_NEAREST, GU_NEAREST);
    
    // For each character to draw
    for (i = 0; i < strlen (text); i++)
    {
        int srcx, srcy, font_xpos, font_ypos;

        // Select the character in the font
        switch (text[i])
        {
            case ' ':
                continue;
                //break;
            case '-':
                font_xpos = 10;
                font_ypos = 1;
                break;
            case '/':
                font_xpos = 13;
                font_ypos = 1;
                break;
            case '.':
                font_xpos = 15;
                font_ypos = 1;
                break;
            case ':':
                font_xpos = 18;
                font_ypos = 1;
                break;
            case '=':
                font_xpos = 20;
                font_ypos = 1;
                break;
            default:
                if (isalpha (text[i]))
                {
                    font_xpos = toupper (text[i]) - 'A';
                    font_ypos = 0;
                }else if (isdigit (text[i]))
                {
                    font_xpos = text[i] - '0';
                    font_ypos = 1;
                }else
                {
                    continue;
                }
        }

        srcx = font_xpos * 8;
        srcy = font_ypos * 8;

        // If it has a shadow, draw it. (ie. the character in black with 0.5 alpha)
        if (drop_shadow)
            blit (srcx, srcy, 8, 8, x + i * 8 + drop_shadow, y + drop_shadow, 0x80000000);
        // Then draw the character with the required color
        blit (srcx, srcy, 8, 8, x + i * 8, y, color);
    }
}

// Render the background plasma
void renderPlasma (void)
{
    float sx, sy, sw, sh;
    float cos_theta, sin_theta, cos_theta10, sin_theta10;
    // Base colors for the plasma
    struct s_color
    {
        unsigned char r;
        unsigned char g;
        unsigned char b;
    }colors [2] = {{0x80, 0x10, 0xf0}, {0xf0, 0x80, 0x10}};
    u32 color, r, g;
    
    // Static to retain the current angle
    static float theta = 0.0;

    // Allocate 2 vertices used for blitting
    TCPVertex *vertices = (TCPVertex *)sceGuGetMemory (2 * sizeof (TCPVertex));

    // Setup GU for blending
    sceGuDisable (GU_DEPTH_TEST);
    sceGuBlendFunc (GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);
    sceGuEnable (GU_BLEND);
    
    // Computes sin and cos of current angle and current angle * 10
    cos_theta = cosf (theta * GU_PI / 180.0f);
    sin_theta = sinf (theta * GU_PI / 180.0f);
    cos_theta10 = cosf (10.0f * theta * GU_PI / 180.0f);
    sin_theta10 = sinf (10.0f * theta * GU_PI / 180.0f);

    // Draw layer 1
    r = colors[0].r + (int)(cos_theta * 100.0f);
    if (r > 255)
        r = 255;
    if (r < 0)  
        r = 0;    
    color = r |
            ((colors[0].g << 8) & 0xff00) |
            ((colors[0].b << 16) & 0xff0000) |
            0xff000000;
    sx = 50 + 40.0 * sin_theta * cos_theta;
    sy = 10 + 44.0 * cos_theta;
    sw = 40.0 + 15.0 * cos_theta10 * sin_theta;
    sh = 40.0 + 5.0 * cos_theta;
    
    sceGuTexMode (GU_PSM_8888, 0, 0, 1);
    sceGuTexImage (0, 128, 128, 128, alpha_circle_start);
    sceGuTexWrap (GU_REPEAT, GU_REPEAT);     
    sceGuTexFunc (GU_TFX_MODULATE, GU_TCC_RGBA);
    sceGuTexFilter (GU_NEAREST, GU_NEAREST);
    sceGuTexEnvColor (color);

    vertices[0].texture.x = sx;
    vertices[0].texture.y = sy;
    vertices[0].color = color;
    vertices[0].position.x = 0;
    vertices[0].position.y = 0;
    vertices[0].position.z = 0;

    vertices[1].texture.x = sx + sw;
    vertices[1].texture.y = sy + sh;
    vertices[1].color = color;
    vertices[1].position.x = SCR_WIDTH;
    vertices[1].position.y = SCR_HEIGHT;
    vertices[1].position.z = 0;

    sceGuDrawArray (GU_SPRITES, TCP_VERTEX_FORMAT | GU_TRANSFORM_2D, 2, 0, vertices);

    // Draw layer 2
    g = colors[1].g + (int)(sin_theta * 40.0f);
    if (g > 255)
        g = 255;
    if (g < 0)  
        g = 0;        
    color = colors[1].r |
            ((g << 8) & 0xff00) |
            ((colors[1].b << 16) & 0xff0000) |
            0xff000000;
    sx = 60 + 20.0 * sin_theta * cos_theta;
    sy = 20 + 64.0 * cos_theta;
    sw = 60.0 + 15.0 * cos_theta * sin_theta10;
    sh = 40.0 + 10.0 * cos_theta;    

    sceGuTexMode (GU_PSM_8888, 0, 0, 1);
    sceGuTexImage (0, 128, 128, 128, alpha_circle_start);
    sceGuTexWrap (GU_REPEAT, GU_REPEAT);     
    sceGuTexFunc (GU_TFX_MODULATE, GU_TCC_RGBA);
    sceGuTexFilter (GU_NEAREST, GU_NEAREST);
    sceGuTexEnvColor (color);

    vertices[0].texture.x = sx + sw;
    vertices[0].texture.y = sy + sh;
    vertices[0].color = color;
    vertices[0].position.x = 0;
    vertices[0].position.y = 0;
    vertices[0].position.z = 0;

    vertices[1].texture.x = sx;
    vertices[1].texture.y = sy;
    vertices[1].color = color;
    vertices[1].position.x = SCR_WIDTH;
    vertices[1].position.y = SCR_HEIGHT;
    vertices[1].position.z = 0;

    sceGuDrawArray (GU_SPRITES, TCP_VERTEX_FORMAT | GU_TRANSFORM_2D, 2, 0, vertices);

    // Increment theta
    theta += 0.1;
    
    // Restore GU context
    sceGuEnable (GU_DEPTH_TEST);
    sceGuDisable (GU_BLEND);        
}

// Create a TGA file with screen content in RGBA 8 bits format
void screenshot (void)
{
    char tmp_filename[64];
    const int width = 480;
    const int height = 272;
    unsigned char line[480 * 3];
    unsigned long* vram = (void *)(0x04000000 + (u32)fbp0);
    int x, y;
#pragma pack (1)    
    struct
    {
        char  idlength;
        char  colourmaptype;
        char  datatypecode;
        short int colourmaporigin;
        short int colourmaplength;
        char  colourmapdepth;
        short int x_origin;
        short int y_origin;
        unsigned char width_lsb;
        unsigned char width_msb;
        unsigned char height_lsb;
        unsigned char height_msb;
        char  bitsperpixel;
        char  imagedescriptor;
    }tgaHeader;
#pragma pack (0)    

    memset (&tgaHeader, 0, sizeof (tgaHeader));
    tgaHeader.datatypecode = 2;     // uncompressed RGB
    tgaHeader.width_lsb = _LSB(480);
    tgaHeader.width_msb = _MSB(480);
    tgaHeader.height_lsb = _LSB(272);
    tgaHeader.height_msb = _MSB(272);
    tgaHeader.bitsperpixel = 24;
    
    sprintf (tmp_filename, "screenshot_%d.tga", (int)time (NULL));
    int fd = sceIoOpen (tmp_filename, PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
    if (!fd)
        return;
    sceIoWrite (fd, &tgaHeader, sizeof(tgaHeader));

    for (y = height - 1; y >= 0; y--)
    {
        for (x = 0; x < width; x++)
        {
            unsigned long color = vram[y * 512 + x];

            line[x * 3] = (color >> 16) & 0xff;
            line[x * 3 + 1] = (color >> 8) & 0xff;
            line[x * 3 + 2] = color & 0xff;
        }
        sceIoWrite (fd, line, width * 3);
    }
    sceIoClose(fd);
}


