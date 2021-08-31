WINDOWS_CC=i386-mingw32msvc-g++
WINDOWS_CFLAGS=-O3 -I/usr/local/cross-tools/i386-mingw32msvc/include/SDL -I/msdos/Python23/include -I./include -Dmain=SDL_main
WINDOWS_LIBS= -L/usr/local/cross-tools/i386-mingw32msvc/lib -L/usr/wine/c/windows/system -lpython23 -lmingw32 -lSDLmain -lSDL -mwindows -lopengl32 -lSDL_image

LINUX_CC=g++
LINUX_LIBS= -lSDL -lSDL_image -lpython3.8 -lGL
LINUX_CFLAGS=-O3 -I./src/gui/include -I./include -I/usr/include/python3.8

clean:
	/bin/rm ./src/*.o
	/bin/rm ./src/linux
	/bin/rm Bogo.Lin
	/bin/rm ./src/*.a
	/bin/rm ./src/gui/*.o

bogo:	./src/Makefile 
	make -C ./src -f Makefile linux
	cp ./src/linux ./Bogo.Lin
