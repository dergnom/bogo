#ifdef WIN32
#include "Python/Python.h"
#include <windows.h>
#include "GL/gl.h"
#include "GL/glext.h"
#else
#include "Python.h"
#include "GL/gl.h"
#endif

#include <iostream>
#include <stdlib.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "gui.h"
#include "gui_items.h"
#include "gl_font.h"

using namespace std;

//methods for the cGuiItems reference-class***************************************************************************************
int cGuiItems::testVirtual(int test){
	for(int i=0;i<test;i++){
		cout << i << ": ich bin ein cGuiItem" << endl;
	}
	
	return(0);
};

int cGuiItems::Print(void){
	cout << "ich bin ein virtuelles dings" << endl;
return(0);
};

int cGuiItems::Edit(void){
	cout << "ich bin ein virtuelles dings" << endl;
return(0);
};

bool cGuiItems::InEditMode(){
	if((myGui->editSlot==this) && (myGui->editMode==1)){
		return(true);
	}
	else{
		return(false);
	}	
}

//methods for the cButton class***************************************************************************************************
int cButton::testVirtual(int test){
	for(int i=0;i<test;i++){
		cout << i << ": ich bin ein cButton" << endl;
	}
	return(0);
};

//draw the button
int cButton::Print(void){
	int x=0;
	int y=12;
	SDL_GetMouseState(&x,&y);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	//draw shadow
	float shadowFact = 0.05f;
	float shadowX = (rightBorder-leftBorder)*shadowFact;
	float shadowY = (buttom-top)*shadowFact;
	glColor3f(0.2f,0.2f,0.2f);
	glBegin(GL_QUADS);
		glVertex3d(rightBorder+shadowX,top+shadowY,-1.0f);
		glVertex3d(leftBorder+shadowX,top+shadowY,-1.0f);
		glVertex3d(leftBorder+shadowX,buttom+shadowY,-1.0f);
		glVertex3d(rightBorder+shadowX,buttom+shadowY,-1.0f);
	glEnd();
	
	//draw button
	glColor3f(1.0,0.0,0.0);
	glBegin(GL_QUADS);
		glVertex3d(leftBorder,top,-0.8);
		glVertex3d(leftBorder,buttom,-0.8);
		glVertex3d(rightBorder,buttom,-0.8);
		glVertex3d(rightBorder,top,-0.8);
	glEnd();
		
	//if the mouse pointer is over the button the text is drawn in highlighted mode else in normal mode
	if( ((x>leftBorder) && (x<rightBorder)) && ((y<buttom*-1) && (y>top*-1)) ){
		writeDisplayListTri(myGui->fontHighlight,label, leftBorder+20.0f, buttom + (top-buttom) * 0.5f, -1.0f);
	}
	else{
		writeDisplayListTri(myGui->font,label, leftBorder+20.0f, buttom + (top-buttom) * 0.5f, -1.0f);
	}
	return(0);
};


int cButton::Edit(void){
	cout << this->label << " im Editmode" << endl;
	myGui->nextFunction = pressFunction;
	myGui->editMode=0;
	myGui->quit=1;
	return(0);
};


/*//methods for the cQuitButtonClass************************************************************************************************
int cQuitButton::testVirtual(int test){
	for(int i=0;i<test;i++){
		cout << i << ": ich bin ein cQuitButton" << endl;
	}
	return(0);
};

int cQuitButton::Print(void){
	cout << "QuitButton print" << endl;
	int x,y;
	SDL_GetMouseState(&x,&y);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glColor3f(0.0,0.0,0.0);
	glBegin(GL_QUADS);
		glVertex3d(leftBorder,top,-1.0);
		glVertex3d(leftBorder,buttom,-1.0);
		glVertex3d(rightBorder,buttom,-1.0);
		glVertex3d(rightBorder,top,-1.0);
	glEnd();
	
	if( ((x>leftBorder) && (x<rightBorder)) && ((y>buttom) && (y<top)) ){
		writeDisplayList(myGui->fontHighlight,label, rightBorder+20.0f, buttom + (top-buttom) * 0.3f, -1.0f);
	}
	else{
		writeDisplayList(myGui->font,label, rightBorder+20.0f, buttom + (top-buttom) * 0.3f, -1.0f);
	}
	return(0);
}

int cQuitButton::Edit(void){
	myGui->editMode=0;
	myGui->quit=1;
	return(0);
}

*/

//methods for cTextLabel class****************************************************************************************************
int cTextLabel::testVirtual(int test){
	for(int i=0;i<test;i++){
		cout << i << ": ich bin ein cTextLabel" << endl;
	}
	return(0);
};

int cTextLabel::Print(void){
	writeDisplayList(myGui->fontHighlight,text,leftBorder,(float)top,-1.0);
	return(0);
};

int cTextLabel::Edit(void){
	myGui->editMode=0;
	return(0);
};

//methods for cCharField class****************************************************************************************************

int cCharField::testVirtual(int test){
	for(int i=0;i<test;i++){
		cout << i << ": ich bin ein cCharField" << endl;
	}
	return(0);
};

int cCharField::Print(void){
	double shadowFact = 0.05;
	double shadowX = (rightBorder-leftBorder)*shadowFact;
	double shadowY = (buttom-top)*shadowFact;
	
	glColor3f(0.2f,0.2f,0.2f);
	glBegin(GL_QUADS);
		glVertex3d(rightBorder+shadowX,top+shadowY,-1.0f);
		glVertex3d(leftBorder+shadowX,top+shadowY,-1.0f);
		glVertex3d(leftBorder+shadowX,buttom+shadowY,-1.0f);
		glVertex3d(rightBorder+shadowX,buttom+shadowY,-1.0f);
	glEnd();
	
	glColor3f(0.5,0.5,0.0);
	glBegin(GL_QUADS);
		glVertex3d(rightBorder,top,-1.0);
		glVertex3d(leftBorder,top,-1.0);
		glVertex3d(leftBorder,buttom,-1.0);
		glVertex3d(rightBorder,buttom,-1.0);
	glEnd();
	
	int x=0;
	int y=0;
	SDL_GetMouseState(&x,&y);
	
	char text[256];
	strcpy(text,SDL_GetKeyName(*keysym));
	
	if((x>leftBorder) && (x<rightBorder) && (y<buttom*-1) && (y>top*-1)){
	
		//writeDisplayList(myGui->font,text,leftBorder,buttom,-1.0f);
		writeDisplayList(myGui->font,text,leftBorder+(rightBorder-leftBorder)/3.0f,buttom+(top-buttom)/5.0f,-1.0f);
	}
	else{
		//writeDisplayList(myGui->fontHighlight,text,leftBorder,buttom,-1.0);
		writeDisplayList(myGui->fontHighlight,text,leftBorder+(rightBorder-leftBorder)/3.0f,buttom+(top-buttom)/5.0f,-1.0f);
	}
	return(0);
};

int cCharField::Edit(void){
	SDL_Event event;
	*keysym=SDLK_SPACE;
	
	while(SDL_PollEvent(&event)!=0){
		if(event.type & SDL_KEYDOWN){
			*keysym=event.key.keysym.sym;
			myGui->editMode=0;
		}
	}
	return(0);
};

//methods for cTextLineField******************************************************************************************************


cTextLineField::cTextLineField(void){
	blinkCounter=SDL_GetTicks();
	currentBlinkFont = 0x0;
}

int cTextLineField::testVirtual(int test){
	for(int i=0;i<test;i++){
		cout << i << ": ich bin ein cTextLineField" << endl;
	}
	return(0);
};

int cTextLineField::Print(void){
	if(currentBlinkFont == 0x0){
		currentBlinkFont = myGui->fontHighlight;
	}
	double shadowFact = 0.05;
	double shadowX = (rightBorder-leftBorder)*shadowFact;
	double shadowY = (buttom-top)*shadowFact;
	
	if(this->InEditMode()){
		unsigned int ticks = SDL_GetTicks();
		//swap fonts every 0.5 seconds
		if(ticks-blinkCounter>500){
			blinkCounter=ticks;
			if(currentBlinkFont==myGui->font){
				currentBlinkFont=myGui->fontHighlight;
			}
			else{
				currentBlinkFont=myGui->font;
			}
		}

		
		writeDisplayList(currentBlinkFont,text,leftBorder,buttom,-1.0);
	}
	else{
		writeDisplayList(myGui->fontHighlight,text,leftBorder,buttom,-1.0);
	}

	return(0);
};

int cTextLineField::Edit(void){
	SDL_Event event;
	SDLKey keysym;
	SDLMod mod;
	char keyName[256];
	
	
	while(SDL_PollEvent(&event)!=0){
		if(event.type == SDL_KEYDOWN){
			keysym=event.key.keysym.sym;
			mod = event.key.keysym.mod;
			strcpy(keyName,SDL_GetKeyName(keysym));
			cout << keyName << endl;
			
			if(strcmp(keyName,"return")==0){
				myGui->editMode=0;
				//cout << "return" << endl;
				break;
			}
			
			if(strcmp(keyName,"backspace")==0){
				if(strlen(text)>0){
					text[strlen(text)-1]='\0';
				}
				//cout << "backspace" << endl;
			}
			
			if((strlen(text)<textLength) && (strlen(keyName)==1)){
				if((mod & KMOD_LSHIFT) || (mod & KMOD_LSHIFT)){
						if(((int)keyName[0]<=122) && ((int)keyName[0]>=97)){
							keyName[0] = (char)((int)keyName[0] - 32);
						}
				}
				text=strcat(text,keyName);

				//cout << "Füge " << keyName << " hinzu" << endl;
			}
			
			if((strlen(text)<textLength) && (strcmp(keyName,"space")==0)){
				text=strcat(text," ");
				//cout << "Füge " << "\" \""  << " hinzu" << endl;
			}
			
		}
	}
	return(0);
};

//methods for the cListDialogue class*********************************************************************************************

cListDialogue::cListDialogue(){
	buttom=0;
	top=0;
	leftBorder=0;
	rightBorder=0;
	fontHeight=32;
	currentListIndex=0;
	rows=0;
	displayListLength=0;
};

cListDialogue::~cListDialogue(){
};

int cListDialogue::testVirtual(int test){
	for(int i=0;i<test;i++){
		cout << i << ": ich bin ein cListDialogue" << endl;
	}
	return(0);
};

int cListDialogue::Print(void){
	double height = (double) (top - buttom);
	double width = (double) (rightBorder - leftBorder);
	double delimiterX = (double) (width * 0.2);
	double shadowFact = 0.05;
	double shadowX = (rightBorder-leftBorder)*shadowFact;
	double shadowY = (buttom-top)*shadowFact;
	int x=0;
	int y=0;
	SDL_GetMouseState(&x,&y);
	//the lower right quadrant of the OpenGL coordinate system is used -> y runs from zero (top of the screen) to -800 (buttom of the screen)
	y=y*-1;
 
	//background and shadow
	glColor3f(0.2f,0.2f,0.2f);
	glBegin(GL_QUADS);
		glVertex3d(rightBorder+shadowX,top+shadowY,-1.0f);
		glVertex3d(leftBorder+shadowX,top+shadowY,-1.0f);
		glVertex3d(leftBorder+shadowX,buttom+shadowY,-1.0f);
		glVertex3d(rightBorder+shadowX,buttom+shadowY,-1.0f);
	glEnd();
	
	glColor3f(0.0,1.0,0.0);
	glBegin(GL_QUADS);
		glVertex3d(rightBorder,top,-1.0);
		glVertex3d(leftBorder,top,-1.0);
		glVertex3d(leftBorder,buttom,-1.0);
		glVertex3d(rightBorder,buttom,-1.0);
	glEnd();
	
	//delimiter between arrows and list
	glColor3f(0.0,0.0,0.0);
	glBegin(GL_LINE_STRIP);
		glVertex3d(leftBorder + delimiterX,top,-1.0);
		glVertex3d(leftBorder + delimiterX,buttom,-1.0);
	glEnd();
	
	//upper arrow
	if(  ((x>leftBorder)&&(x<leftBorder+delimiterX)) && ((y<top)&&(y>top-height*0.5))  ) {
		glColor3f(1.0,1.0,0.0);
	}
	else{
		glColor3f(0.0,0.0,0.0);
	}
	glBegin(GL_TRIANGLES);
		glVertex3d(leftBorder+delimiterX/2.0,top,-1.0);
		glVertex3d(leftBorder,top-height*0.2,-1.0);
		glVertex3d(leftBorder+delimiterX,top-height*0.2,-1.0);
	glEnd();
	glBegin(GL_QUADS);
		glVertex3d(leftBorder+delimiterX*0.7,top-height*0.2,-1.0);
		glVertex3d(leftBorder+delimiterX*0.3,top-height*0.2,-1.0);
		glVertex3d(leftBorder+delimiterX*0.3,top-height*0.5,-1.0);
		glVertex3d(leftBorder+delimiterX*0.7,top-height*0.5,-1.0);
	glEnd();
	
	//lower arrow
	if( ((x>leftBorder)&&(x<leftBorder+delimiterX)) && ((y<top-0.5*height)&&(y>buttom)) ){
		glColor3f(1.0,1.0,0.0);
	}
	else{
		glColor3f(0.0,0.0,0.0);
	}
	glBegin(GL_TRIANGLES);
		glVertex3d(leftBorder+delimiterX/2.0,buttom,-1.0);
		glVertex3d(leftBorder+delimiterX,buttom+height*0.2,-1.0);
		glVertex3d(leftBorder,buttom+height*0.2,-1.0);
	glEnd();
	glBegin(GL_QUADS);
		glVertex3d(leftBorder+delimiterX*0.7,top-height*0.5,-1.0);
		glVertex3d(leftBorder+delimiterX*0.3,top-height*0.5,-1.0);
		glVertex3d(leftBorder+delimiterX*0.3,top-height*0.8,-1.0);
		glVertex3d(leftBorder+delimiterX*0.7,top-height*0.8,-1.0);
	glEnd();
	
	//text of the displayed list elemnts
	for(int i=0;i<displayListLength;i++){
		if(i==displayListIndex){
			writeDisplayList(myGui->fontHighlight,displayList[i],leftBorder+(float)delimiterX*1.1f,top-fontHeight*(i+1),-1.0f);
		}
		else{
			writeDisplayList(myGui->font,displayList[i],leftBorder+(float)delimiterX*1.1f,top-fontHeight*((float)i+1.0f),-1.0f);
		}
	}
	return(0);
};

int cListDialogue::Edit(void){
	myGui->editMode=0;
	double height = (double) (top - buttom);
	double width = (double) (rightBorder - leftBorder);
	double delimiterX = (double) (width * 0.2);
	int x,y;
	SDL_GetMouseState(&x,&y);
	y=y*-1;
		
	if( (y>top+(buttom-top)/2) && ((x>leftBorder) && (x<leftBorder+delimiterX)) ){
		currentListIndex--;
		if (currentListIndex<0){
			currentListIndex=0;
		}
	}
	else{
	if((x>leftBorder) && (x<leftBorder+delimiterX)){
		currentListIndex++;
		if (currentListIndex>currentListLength-1){
			currentListIndex=currentListLength-1;
		}
	}
	}
	
	//determine index to start from to copy the currentListElements into the displayList 
	int i=0;
	int startpoint=0;
	do{
		startpoint = currentListIndex - int(rows/2) +i;
		i++;
	}while(startpoint<0);
	
	displayListIndex=currentListIndex - startpoint;
	
	//put row elements of the currentList in the displayList
	displayListLength=0;
	for(int j=0;j<rows;j++){
		if (startpoint+j >=currentListLength){ break;}
		strcpy(displayList[j],currentList[startpoint+j]);
		displayListLength++;
	}
	
	strcpy(text,displayList[displayListIndex]);
	cout << "Ausgewaehlter Text: " << text << endl;
	
	return(0);
};

int cListDialogue::setPosition(float lBorder, float rBorder, float tp, float bttn){
	leftBorder = lBorder;
	rightBorder = rBorder;
	top = tp;
	buttom = bttn;
	calcSize();
	return(0);
};

int cListDialogue::setFontPixelHeight(int pixels){
	fontHeight=(float)pixels;
	calcSize();
	return(0);
};

//change the list index, so that the currently choosen option is highlighted
int cListDialogue::indexText(){
	for(int i=0;i<currentListLength;i++){
		cout << "i: " << i << endl;
		cout << currentList[i] << ":" << text << endl;
		if (strcmp(currentList[i],text)==0){
			currentListIndex=i;
		}
	}
	return(0);
}

int cListDialogue::calcSize(){
	if( ( (rightBorder-leftBorder < 0) || (top - buttom < 0) ) ){
		cerr << "rb: " << rightBorder << " lb: " << leftBorder << " t: " << top << " b: " << buttom << endl; 
		cerr << "cListDialogue object has illegal size" << endl;
		exit(1);
	}
	
	if (fontHeight<=0){
		cerr << "cListDialogue object has illegal fontHeight" << endl;
		exit(1);
	}
	
	//recalculate the buttomposition
	float height = (float)(top - buttom);
	float fRows = height / fontHeight;
	rows = (int)floor(fRows);
	height=(float)rows * fontHeight;
	buttom = (float)(top - height);
	
	//determine index to start the copy of the currentListElements into the displayList 
	int i=0;
	int startpoint=0;
	do{
		startpoint = currentListIndex - int(rows/2) +i;
		i++;
	}while(startpoint<0);
	displayListIndex=currentListIndex - startpoint;
	
	//put row elements of the currentList in the displayList
	displayListLength=0;
	for(int j=0;j<rows;j++){
		if (startpoint+j >=currentListLength){ break;}
		strcpy(displayList[j],currentList[startpoint+j]);
		displayListLength++;
	}
	
	strcpy(text,displayList[displayListIndex]);
	return(0);
};

/*
//methods for the cTextFieldclass*********************************************************************************************

cTextField::cTextField(){
	buttom=0;
	top=0;
	leftBorder=0;
	rightBorder=0;
	fontHeight=32;
	currentListIndex=0;
	rows=0;
	displayListLength=0;
};

cTextField::~cTextField(){
};

int cTextField::testVirtual(int test){
	for(int i=0;i<test;i++){
		cout << i << ": ich bin ein cListDialogue" << endl;
	}
	return(0);
};

int cTextField::Print(void){
	double height = (double) (top - buttom);
	double width = (double) (rightBorder - leftBorder);
	double delimiterX = (double) (width * 0.2);
	double shadowFact = 0.05;
	double shadowX = (rightBorder-leftBorder)*shadowFact;
	double shadowY = (buttom-top)*shadowFact;
	int x=0;
	int y=0;
	SDL_GetMouseState(&x,&y);
	//the lower right quadrant of the OpenGL coordinate system is used -> y runs from zero (top of the screen) to -800 (buttom of the screen)
	y=y*-1;
 
	//shadow
	glColor3f(0.2f,0.2f,0.2f);
	glBegin(GL_QUADS);
		glVertex3d(rightBorder+shadowX,top+shadowY,-1.0f);
		glVertex3d(leftBorder+shadowX,top+shadowY,-1.0f);
		glVertex3d(leftBorder+shadowX,buttom+shadowY,-1.0f);
		glVertex3d(rightBorder+shadowX,buttom+shadowY,-1.0f);
	glEnd();
	
	//background
	glColor3f(0.0,1.0,0.0);
	glBegin(GL_QUADS);
		glVertex3d(rightBorder,top,-1.0);
		glVertex3d(leftBorder,top,-1.0);
		glVertex3d(leftBorder,buttom,-1.0);
		glVertex3d(rightBorder,buttom,-1.0);
	glEnd();
	
	//delimiter between arrows and list
	glColor3f(0.0,0.0,0.0);
	glBegin(GL_LINE_STRIP);
		glVertex3d(leftBorder + delimiterX,top,-1.0);
		glVertex3d(leftBorder + delimiterX,buttom,-1.0);
	glEnd();
	
	//upper arrow
	if(  ((x>leftBorder)&&(x<leftBorder+delimiterX)) && ((y<top)&&(y>top-height*0.5))  ) {
		glColor3f(1.0,1.0,0.0);
	}
	else{
		glColor3f(0.0,0.0,0.0);
	}
	glBegin(GL_TRIANGLES);
		glVertex3d(leftBorder+delimiterX/2.0,top,-1.0);
		glVertex3d(leftBorder,top-height*0.2,-1.0);
		glVertex3d(leftBorder+delimiterX,top-height*0.2,-1.0);
	glEnd();
	glBegin(GL_QUADS);
		glVertex3d(leftBorder+delimiterX*0.7,top-height*0.2,-1.0);
		glVertex3d(leftBorder+delimiterX*0.3,top-height*0.2,-1.0);
		glVertex3d(leftBorder+delimiterX*0.3,top-height*0.5,-1.0);
		glVertex3d(leftBorder+delimiterX*0.7,top-height*0.5,-1.0);
	glEnd();
	
	//lower arrow
	if( ((x>leftBorder)&&(x<leftBorder+delimiterX)) && ((y<top-0.5*height)&&(y>buttom)) ){
		glColor3f(1.0,1.0,0.0);
	}
	else{
		glColor3f(0.0,0.0,0.0);
	}
	glBegin(GL_TRIANGLES);
		glVertex3d(leftBorder+delimiterX/2.0,buttom,-1.0);
		glVertex3d(leftBorder+delimiterX,buttom+height*0.2,-1.0);
		glVertex3d(leftBorder,buttom+height*0.2,-1.0);
	glEnd();
	glBegin(GL_QUADS);
		glVertex3d(leftBorder+delimiterX*0.7,top-height*0.5,-1.0);
		glVertex3d(leftBorder+delimiterX*0.3,top-height*0.5,-1.0);
		glVertex3d(leftBorder+delimiterX*0.3,top-height*0.8,-1.0);
		glVertex3d(leftBorder+delimiterX*0.7,top-height*0.8,-1.0);
	glEnd();
	
	//text of the displayed list elemnts
	for(int i=0;i<displayListLength;i++){
		if(i==displayListIndex){
			writeDisplayList(myGui->fontHighlight,displayList[i],leftBorder+(float)delimiterX*1.1f,top-fontHeight*(i+1),-1.0f);
		}
		else{
			writeDisplayList(myGui->font,displayList[i],leftBorder+(float)delimiterX*1.1f,top-fontHeight*((float)i+1.0f),-1.0f);
		}
	}
	return(0);
};

int cTextField::Edit(void){
	myGui->editMode=0;
	double height = (double) (top - buttom);
	double width = (double) (rightBorder - leftBorder);
	double delimiterX = (double) (width * 0.2);
	int x,y;
	SDL_GetMouseState(&x,&y);
	y=y*-1;
		
	if( (y>top+(buttom-top)/2) && ((x>leftBorder) && (x<leftBorder+delimiterX)) ){
		currentListIndex--;
		if (currentListIndex<0){
			currentListIndex=0;
		}
	}
	else{
	if((x>leftBorder) && (x<leftBorder+delimiterX)){
		currentListIndex++;
		if (currentListIndex>currentListLength-1){
			currentListIndex=currentListLength-1;
		}
	}
	}
	
	//determine index to start from to copy the currentListElements into the displayList 
	int i=0;
	int startpoint=0;
	do{
		startpoint = currentListIndex - int(rows/2) +i;
		i++;
	}while(startpoint<0);
	
	displayListIndex=currentListIndex - startpoint;
	
	//put row elements of the currentList in the displayList
	displayListLength=0;
	for(int j=0;j<rows;j++){
		if (startpoint+j >=currentListLength){ break;}
		strcpy(displayList[j],currentList[startpoint+j]);
		displayListLength++;
	}
	
	strcpy(text,displayList[displayListIndex]);
	cout << "Ausgewaehlter Text: " << text << endl;
	
	return(0);
};

int cTextField::setPosition(float lBorder, float rBorder, float tp, float bttn){
	leftBorder = lBorder;
	rightBorder = rBorder;
	top = tp;
	buttom = bttn;
	calcSize();
	return(0);
};

int cTextField::setFontPixelHeight(int pixels){
	fontHeight=(float)pixels;
	calcSize();
	return(0);
};

//change the list index, so that the currently choosen option is highlighted
int cTextField::indexText(){
	for(int i=0;i<currentListLength;i++){
		cout << "i: " << i << endl;
		cout << currentList[i] << ":" << text << endl;
		if (strcmp(currentList[i],text)==0){
			currentListIndex=i;
		}
	}
	return(0);
}

int cTextField::calcSize(){
	if( ( (rightBorder-leftBorder < 0) || (top - buttom < 0) ) ){
		cerr << "rb: " << rightBorder << " lb: " << leftBorder << " t: " << top << " b: " << buttom << endl; 
		cerr << "cListDialogue object has illegal size" << endl;
		exit(1);
	}
	
	if (fontHeight<=0){
		cerr << "cListDialogue object has illegal fontHeight" << endl;
		exit(1);
	}
	
	//recalculate the buttomposition
	float height = (float)(top - buttom);
	float fRows = height / fontHeight;
	rows = (int)floor(fRows);
	height=(float)rows * fontHeight;
	buttom = (float)(top - height);
	

	//determine index to start from to copy the currentListElements into the displayList 
	int i=0;
	int startpoint=0;
	do{
		startpoint = currentListIndex - int(rows/2) +i;
		i++;
	}while(startpoint<0);
	displayListIndex=currentListIndex - startpoint;
	
	//put row elements of the currentList in the displayList
	displayListLength=0;
	for(int j=0;j<rows;j++){
		if (startpoint+j >=currentListLength){ break;}
		strcpy(displayList[j],currentList[startpoint+j]);
		displayListLength++;
	}
	
	strcpy(text,displayList[displayListIndex]);
	return(0);
};
*/
