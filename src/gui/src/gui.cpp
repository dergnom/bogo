#ifdef WIN32
#include "Python/Python.h"
#include <windows.h>
#include "GL/gl.h"
#include "GL/glext.h"
#else
#include "Python.h"
#include "GL/gl.h"
#endif

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "gl_font.h"
#include "gui_items.h"
#include "gui.h"
#include <iostream>

using namespace std;

int cGui::Loop(){
	while(quit==0){
		Iterate();
		SDL_Delay(50);
	}
	return(0);
}


//do one step in the gui loop i.e. process the event-queue till empty, call the edit function if a gui element is in edit mode, if not in edit mode switch to edit mode if neccessary
int cGui::Iterate(){
	int clicked = 0;
	int x = -1;
	int y=-1;
	//bring OpenGL into a finite state. Draw the Background image. The lower left quadtant of the x/y plane is used(defined in the constructor)
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0,800.0,-600.0,0.0,2.0,-2.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRasterPos3f(0.0,-600.0,-1.0);
	glCallList(background);
	for(int i=0;i<numOfGuiItems;i++){
		guiItems[i]->Print();
	}
	
	//if editMode == 0 we check if one of the gui elements gets activated by a mouse click
	if(editMode==0){
		SDL_Event event;
		SDL_PollEvent(0);
		
		//check if the left mouse button was pressed -> if the user activated a gui element
		while(SDL_PollEvent(&event)!=0){

			//check if the left mouse button is released, i.e. if the user "clicked" somewhere
			if(event.type == SDL_MOUSEBUTTONUP){
			if((event.button.button == SDL_BUTTON_LEFT) && (event.button.state == SDL_RELEASED)){
				clicked=1;
				x=event.button.x;
				y=event.button.y;
				break;
			}
			}
		}
		
		//check if cursor is over a gui element. if so and if a click was detected before, the gui changes to edit mode
		if(x=-1){
			SDL_GetMouseState(&x,&y);
		}
		
		for(int j=0;j<numOfGuiItems;j++){
			
			if ( ((x > guiItems[j]->leftBorder) && (x < guiItems[j]->rightBorder)) && ((y < guiItems[j]->buttom*-1) && (y > guiItems[j]->top*-1)) ){

				if(clicked==1){
					SDL_GetMouseState(&x,&y);
					editMode=1;
					editSlot=guiItems[j];
				}
				guiItems[j]->mouseOver=1;
			}
			else{
				guiItems[j]->mouseOver=0;
			}
		}
	}
	
	//activate the gui item the user is interacting with 
	if(editMode==1){
		editSlot->Edit();
	}
	
	//update screen
	SDL_GL_SwapBuffers();
	return(0);
}


//load a background image from a file
int cGui::InitBackground(const char * filename){
	SDL_Surface * image = IMG_Load(filename);
	
	background = glGenLists(1);
	glNewList(background,GL_COMPILE);
		glDrawPixels(image->w,image->h,GL_BGR,GL_UNSIGNED_BYTE,image->pixels);
	glEndList();
	
	SDL_FreeSurface(image);
	return(0);
}

//add a new item to the gui
int cGui::AddGuiItem(cGuiItems * newItem){
	if(numOfGuiItems>0){
		numOfGuiItems++;
		guiItems=(cGuiItems **) realloc(guiItems,sizeof(cGuiItems**)*numOfGuiItems);
		guiItems[numOfGuiItems-1]=newItem;
		cout << "Nutze realloc. NumOfGuiItems: " << numOfGuiItems << endl;

	}
	else{
		guiItems=(cGuiItems **) malloc(sizeof(cGuiItems*));
		guiItems[0]=newItem;
		numOfGuiItems++;
		cout << "Nutze malloc. NumOfGuiItems: " << numOfGuiItems << endl;
	}
	
	return(0);
}

//tell the gui to quit on the next occasion
int cGui::Quit(){
	quit=1;
	return(0);
}

//constructor initiate certain opengl and sdl parameters, load the used fonts (1 for normal text and one for highlighted text)
cGui::cGui(const char * stdFont, const char * highlightFont){
	SDL_EnableKeyRepeat(350,30);
	
	glClearColor(0.0,1.0,0.0,0.0);
 	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
 	glOrtho(0.0,800.0,-600.0,0.0,1.0,-1.0);
 	glEnable(GL_ALPHA_TEST);
 	glAlphaFunc(GL_GREATER,0.0);
 	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	SDL_Surface * fontArray[ASCII_SLICE];
	loadBMPFont(stdFont,fontArray);
	font = generateDisplayList(fontArray);
	loadBMPFont(highlightFont,fontArray);
	fontHighlight=generateDisplayList(fontArray);
	
	editMode=0;
	quit=0;
	numOfGuiItems=0;
}

cGui::cGui(){
	SDL_EnableKeyRepeat(350,30);
	
	glClearColor(0.0,1.0,0.0,0.0);
 	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
 	glOrtho(0.0,800.0,-600.0,0.0,1.0,-1.0);
 	glEnable(GL_ALPHA_TEST);
 	glAlphaFunc(GL_GREATER,0.0);
 	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	editMode=0;
	quit=0;
	numOfGuiItems=0;
}

int cGui::loadFont(char * fontName){
	//möglicherweise problematisch, wenn keine listen angelegt sind
	glDeleteLists(font,ASCII_START + ASCII_SLICE - 1);
	
	SDL_Surface * fontArray[ASCII_SLICE];
	loadBMPFont(fontName,fontArray);
	font = generateDisplayList(fontArray);
	return(0);
}

int cGui::DeleteAllGuiItems(){
	for(int i=0;i<numOfGuiItems;i++){
		delete(guiItems[i]);
		cerr <<"guiItems["<<i<<"] geloescht"<< endl;
	}
	if(numOfGuiItems>0){
		free(guiItems);
	}
	numOfGuiItems=0;
	return(0);
}

int cGui::GetGuiItemIndexFromIdTag(const char * searchTag){
	int foundAtIndex=-1;
	cout << "numOfGuiItems: " << numOfGuiItems << endl;
	for(int i=0;i<numOfGuiItems;i++){
		if(strcmp(searchTag,guiItems[i]->idTag)==0){
			foundAtIndex=i;
			break;
		}
	}
	return(foundAtIndex);
}


int cGui::loadHighlightFont(char * fontName){
	glDeleteLists(fontHighlight,ASCII_START + ASCII_SLICE - 1);
	SDL_Surface * fontArray[ASCII_SLICE];
	loadBMPFont(fontName,fontArray);
	fontHighlight = generateDisplayList(fontArray);
	return(0);
}

cGui::~cGui(void){
	if(numOfGuiItems>0){
		//possibly dangerous, because guiItems might be uninitialized->happy freeing of random memory
		free(guiItems);
	}
	SDL_EnableKeyRepeat(0,0);
	
	//deletes the display list with the index stored in font and the following (ASCII_START + ASCII_SLICE -1) lists
	glDeleteLists(font,ASCII_START + ASCII_SLICE - 1);
	glDeleteLists(fontHighlight,ASCII_START + ASCII_SLICE - 1);
	glDeleteLists(background,1);
}
