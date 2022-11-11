TARGET = the_cube
OBJS = main.o common.o menu.o game.o stats.o credits.o camera.o sfx.o success_mod.o\
music.o menu_mod.o credits_mod.o psp_mreader.o cube.o devsgen.o font.o font88.o \
sebonpsp1.o sebonpsp2.o red_face.o green_face.o blue_face.o yellow_face.o \
orange_face.o white_face.o alpha_circle.o controls.o stars.o cursor.o cube_logo.o \
callbacks.o vram.o

INCDIR = 
CFLAGS = -O2 -G0 -Wall -D_PSP
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

LIBDIR = 
LDFLAGS = 
LIBS = -lmikmod -lpspaudio -lpspgum -lpspgu -lpng -lz -lm -lpsprtc

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = the_cube
PSP_EBOOT_ICON = images/eboot_icon.png
PSP_EBOOT_PIC1 = images/eboot_bg.png

PSPDIR=
PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak

menu_mod.o: music/menu.mod
	bin2o -i music/menu.mod menu_mod.o menu_mod

success_mod.o: music/success.mod
	bin2o -i music/success.mod success_mod.o success_mod

credits_mod.o: music/credits.mod
	bin2o -i music/credits.mod credits_mod.o credits_mod

devsgen.o: images/devsgen2.psptex
	bin2o -i images/devsgen2.psptex devsgen.o devsgen

sebonpsp1.o: images/sebonpsp1.psptex
	bin2o -i images/sebonpsp1.psptex sebonpsp1.o sebonpsp1

sebonpsp2.o: images/sebonpsp2.psptex
	bin2o -i images/sebonpsp2.psptex sebonpsp2.o sebonpsp2

font.o: images/fat_font_medium.psptex
	bin2o -i images/fat_font_medium.psptex font.o font

font88.o: images/font_8x8.psptex
	bin2o -i images/font_8x8.psptex font88.o font88

red_face.o: images/red_face2.psptex
	bin2o -i images/red_face2.psptex red_face.o red_face

green_face.o: images/green_face2.psptex
	bin2o -i images/green_face2.psptex green_face.o green_face

blue_face.o: images/blue_face2.psptex
	bin2o -i images/blue_face2.psptex blue_face.o blue_face

yellow_face.o: images/yellow_face2.psptex
	bin2o -i images/yellow_face2.psptex yellow_face.o yellow_face

orange_face.o: images/orange_face2.psptex
	bin2o -i images/orange_face2.psptex orange_face.o orange_face

white_face.o: images/white_face2.psptex
	bin2o -i images/white_face2.psptex white_face.o white_face

alpha_circle.o: images/alpha_circle.psptex
	bin2o -i images/alpha_circle.psptex alpha_circle.o alpha_circle

controls.o: images/controls1.1.psptex
	bin2o -i images/controls1.1.psptex controls.o controls
	
stars.o: images/stars.psptex
	bin2o -i images/stars.psptex stars.o stars

cursor.o: images/cursor.psptex
	bin2o -i images/cursor.psptex cursor.o cursor
	
cube_logo.o: images/cube.psptex
	bin2o -i images/cube.psptex cube_logo.o cube_logo


