/*! \file gl_font.cpp
\brief Implementation of an OpenGL font API.
\author Thomas Kueppers
\version 1.0
\date 07.03.2007
\warning This version is NOT! thread safe. -> fileNameForChar(...) usage of static variables

!*/

#ifdef WIN32
#include "Python/Python.h"
#include <windows.h>
#include "GL/gl.h"
#include "GL/glext.h"
#else
#include "GL/gl.h"
#endif


#include "SDL/SDL.h"
#include <iostream>

//the first ASCII character that is contained in the bitmap font
#define ASCII_START 32
//Number of ASCII characters contained in the bitmap font. The ASCII_START character ist character number one. 
#define ASCII_SLICE 95

using namespace std;

struct RGBA{
Uint8 rgba[4];
};
/// hallo
char * fileNameForChar(char character){
	static char fileName[100];
	//char fileName[100];
	sprintf(fileName,"%i.bmp",(int) character);
	return(fileName);
}


char * fileNameForChar(int character){
	static char fileName[100];
	//char fileName[100];
	sprintf(fileName,"%i.bmp",character);
	//cout << fileName << endl;
	return(fileName);
}



int loadBMPFont(const char * name,SDL_Surface * array[ASCII_SLICE]){
	for(int i=0;i<ASCII_SLICE;i++){
		char fileName[100];
		int file = i+ASCII_START;
		sprintf(fileName,"./fonts/%s/%i.bmp",name,file);
		//cout << "Lade " << fileName << endl; 
		array[i]=SDL_LoadBMP(fileName);
	}
	return(0);
}

GLuint generateDisplayList(SDL_Surface * fontArray[ASCII_SLICE]){
	GLuint startIndex = glGenLists(ASCII_START+ASCII_SLICE-1);
	for(int i=0;i<=ASCII_SLICE-1;i++){
		
		SDL_Surface * newSurf = SDL_CreateRGBSurface(SDL_SWSURFACE,fontArray[i]->w,fontArray[i]->h,32,0x000000ff,0x0000ff00,0x00ff0000,0xff000000);
		SDL_BlitSurface(fontArray[i],0,newSurf,0);
		
		Uint32 * pixels = (Uint32 *)newSurf->pixels;
		Uint8 r,g,b,a,aNew;
		
		for(int j=0;j<newSurf->w*newSurf->h;j++){
			SDL_GetRGBA(pixels[j],newSurf->format,&r,&g,&b,&a);
			if( (r>150) && (g>150) && (b>150)){
				aNew=0;
			}
			else{
				aNew=255;
			}
			pixels[j]=SDL_MapRGBA(newSurf->format,r,g,b,aNew);
		}
		
		glNewList(startIndex+ASCII_START+i,GL_COMPILE);
			glDrawPixels(newSurf->w,newSurf->h,GL_RGBA,GL_UNSIGNED_BYTE,newSurf->pixels);
			glTranslatef((float)fontArray[i]->w,0,0);
			//cout << i << " Displaylist fuer " << (char) (ASCII_START+i) << " generiert" << endl;
		glEndList();
		SDL_FreeSurface(newSurf);
		SDL_FreeSurface(fontArray[i]);
	}
	
	cout << "Alle displaylists generiert" << endl;
	return(startIndex);
}



//write a line of text on the screen.
int writeDisplayList(GLuint fontIndex,const char * text, float xPos, float yPos, float zPos){
	glRasterPos3f(xPos,yPos,zPos);

	//draw the text. the modelview matrix is manipulated so we call Push- an PopMatrix to limit the changes to the writeDisplayList function
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	for(unsigned int i=0;i<strlen(text);i++){
		glRasterPos3f(xPos,yPos,zPos);
		glCallList(fontIndex+(GLuint) text[i]);
	}
	glPopMatrix();
	return(0);
}


///write a line on the screen,place a triangle before the beginning of the text
int writeDisplayListTri(GLuint fontIndex,const char * text, float xPos, float yPos, float zPos){
	glRasterPos3f(xPos,yPos,zPos);
		
	glColor3f(0.0,0.0,1.0);
	//Draw a triangel before the beginning of the text
	glBegin(GL_TRIANGLES);
		glVertex3f(xPos-10.0f,yPos+10.0f,zPos);
		glVertex3f(xPos,yPos,zPos);
		glVertex3f(xPos-10.0f,yPos-10.0f,zPos);
	glEnd();
	
	//draw the text. the modelview matrix is manipulated so we call Push- an PopMatrix to limit the changes to the writeDisplayList function
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	for(unsigned int i=0;i<strlen(text);i++){
		glRasterPos3f(xPos,yPos,zPos);
		glCallList(fontIndex+(GLuint) text[i]);
	}
	glPopMatrix();
	return(0);
}


