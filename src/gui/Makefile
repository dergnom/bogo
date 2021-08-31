LINUX_CC=g++
LINUX_LIBS=-lSDL -lSDL_image -lGL -lpython3.8
LINUX_CFLAGS=-O3 -I./include -I/usr/include/python3.8

gui.a: ./src/gui.cpp ./src/gui_items.cpp
	$(LINUX_CC) $(LINUX_CFLAGS) -c -g ./src/gui.cpp ./src/gui_items.cpp
	ar rcs gui.a gui.o gui_items.o
	ranlib gui.a

gl_font.a: ./src/gl_font.cpp
	$(LINUX_CC) $(LINUX_CFLAGS) -c -g ./src/gl_font.cpp
	ar rcs gl_font.a gl_font.o
	ranlib gl_font.a


clean:
	rm *.o gl_font.a gui.a 
