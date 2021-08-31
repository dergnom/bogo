WINDOWS_CC=i386-mingw32msvc-g++
WINDOWS_CFLAGS=-O3 -I/usr/local/cross-tools/i386-mingw32msvc/include/SDL -I/msdos/Python23/include -I./include -Dmain=SDL_main
WINDOWS_LIBS= -L/usr/local/cross-tools/i386-mingw32msvc/lib -L/usr/wine/c/windows/system -lpython23 -lmingw32 -lSDLmain -lSDL -mwindows -lopengl32 -lSDL_image

LINUX_CC=g++
LINUX_LIBS= -lSDL -lSDL_image -lpython3.8 -lGL
LINUX_CFLAGS=-O3 -I./gui/include -I../include -I/usr/include/python3.8 -I/usr/local/include



linux: MovingObject.cpp Player.cpp Ball.cpp Game.cpp main.cpp
	make -C ./gui -f Makefile gl_font.a 
	make -C ./gui -f Makefile gui.a
	cp ./gui/gui.a ./
	cp ./gui/gl_font.a ./
	$(LINUX_CC) $(LINUX_CFLAGS) -c -g MovingObject.cpp Player.cpp Ball.cpp Game.cpp main.cpp
	$(LINUX_CC) main.o MovingObject.o Player.o Ball.o Game.o gui.a gl_font.a $(LINUX_LIBS) -o linux

clean:
	/bin/rm *.o *.a
