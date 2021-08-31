#include "Python.h"
#ifdef WIN32
	#include "windows.h"
#endif
#include <iostream>
#include "SDL/SDL.h"
#include "GL/gl.h"
#include "glext.h"
#include "gl_font.h"
#include "gui.h"


using namespace std;

int yodelbaer(){
	cout << "yodelbaer" << endl;
	return(0);
}

int hurgamon(){
	cout << "hurgamon" << endl;
	return(0);
}

int main(int argc, char * argv[]){
 SDL_Init(SDL_INIT_VIDEO);
 SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);
 
 SDL_Surface * screen = SDL_SetVideoMode(800,600,24,SDL_OPENGL);
 
 cout << "theGui" << endl;
 cGui * theGui = new(cGui);
 cout << "Button1" << endl;
 cButton * Button1 = new(cButton);
 cout << "Button2" << endl;
 cButton * Button2 = new(cButton);
 cout << "TextLabel1" << endl;
 cTextLabel * TextLabel1 = new(cTextLabel);
 cout << "charField1" << endl;
 cCharField * CharField1 = new(cCharField);
 cout << "TextLineField1" << endl;
 cTextLineField * TextLineField1 = new(cTextLineField);
 cout << "ListDialogue1" << endl;
 cListDialogue * ListDialogue1 = new(cListDialogue);
 
 cout << "InitBackground" << endl;
 theGui->InitBackground("./pine.bmp");
 
 cout << "Initialisiere Button1" << endl;
 Button1->label="Yodelbaer";
 Button1->pressFunction=&yodelbaer;
 Button1->leftBorder=10;
 Button1->rightBorder=250;
 Button1->top=-320;
 Button1->buttom=-570;
 Button1->myGui=theGui;
 
 cout << "Initialisiere Button2" << endl;
 Button2->label="Hurg";
 Button2->pressFunction=&hurgamon;
 Button2->leftBorder=450;
 Button2->rightBorder=600;
 Button2->buttom=-450;
 Button2->top=-350;
 Button2->myGui=theGui;
 
 cout << "Initialisiere TextLabel1" << endl;
 TextLabel1->leftBorder=350;
 TextLabel1->top=-100;
 TextLabel1->myGui=theGui;
 strcpy(TextLabel1->text,argv[0]);
 
 cout << "Initialisiere CharField1" << endl;
 CharField1->leftBorder=550;
 CharField1->rightBorder=750;
 CharField1->top=-100;
 CharField1->buttom=-300;
 CharField1->myGui=theGui;
 SDLKey key = SDLK_a;
 CharField1->keysym=&key;
 
 cout << "Initialisiere TextLineField1" << endl;
 TextLineField1->leftBorder=50;
 TextLineField1->rightBorder=250;
 TextLineField1->top=-100;
 TextLineField1->buttom=-290;
 TextLineField1->myGui=theGui;
 char editText[256] = "Bitte aendern";
 TextLineField1->text=editText;
 TextLineField1->textLength=40;
 
 cout << "Initialisiere ListDialogue1" << endl;
 char * list[8] = {"hallo","hurga","yodelbaer","hoho","dingsbums","Cpt.Nemo","Das Gesp. von Canterville","Mr. Spock"};
 for (int i=0;i<8;i++){cout << i << ": " << list[i] << endl;}
 ListDialogue1->currentList=list;
 ListDialogue1->currentListLength=8;
 char testText[12] = "hurgamon";
 ListDialogue1->text=testText;
 ListDialogue1->setPosition(400,600,-300,-600);
 ListDialogue1->myGui=theGui;
 
  
 //register gui-objects with the gui
 cout << "AddGuiItem1" << endl;
 theGui->AddGuiItem(Button1);
 cout << "Add TextLabel1" << endl;
 theGui->AddGuiItem(TextLabel1);
 cout << "Add CharField1" << endl;
 theGui->AddGuiItem(CharField1);
 cout << "charField1 hinzugefügt" << endl;
 theGui->AddGuiItem(TextLineField1);
 cout << "TextLineField1 hinzugefügt" << endl;
 theGui->AddGuiItem(ListDialogue1);
 cout << "ListDialogue1 hinzugefügt" << endl;
 
 cout << "Loop" << endl;
 theGui->Loop();
 cout << "Loop beendet" << endl;
 
 cout << "testText: " << testText << endl;
 SDL_Quit();
}
