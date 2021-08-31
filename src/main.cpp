#include "Python.h"
#include "GL/gl.h"



#include <SDL/SDL.h>
#include <iostream>
#include "Player.h"
#include "Ball.h"
#include "Game.h"
#include "constants.h"
#include <stdlib.h>
#include <SDL/SDL_image.h>
#include "gui.h"
#include "gui_items.h"


#ifndef BOGOFUNCTION
typedef int (*bogoFunction)();
#endif

using namespace std;

int gameOptionsFunc();

//Initialize as Global Variables
cGame * aGame;
bogoFunction nextFunction;
SDL_Surface * screen;
int gameFunc();
int startMenu();

int gameInit(int argc, char * argv[]){
 cout << "gameInit started" << endl;
 strcpy(aGame->player1->name,"Karl Koch");
 strcpy(aGame->player2->name,"Adam Weisshaupt");
 aGame->player1->moveLeft=SDLK_a;
 aGame->player1->moveRight=SDLK_d;
 aGame->player1->jump=SDLK_w;
 aGame->player2->moveLeft=SDLK_LEFT;
 aGame->player2->moveRight=SDLK_RIGHT;
 aGame->player2->jump=SDLK_UP;

 aGame->player1->cpuControlled=0;
 strcpy(aGame->player1->pAIModule,"none");
 aGame->player2->cpuControlled=1;
 strcpy(aGame->player2->pAIModule,"theOtherOne");
 if(argc>1){ 
	strcpy(aGame->player2->pAIModule,argv[1]);
	aGame->player2->cpuControlled=1;

 }
 if (argc>2){
	strcpy(aGame->player1->pAIModule,argv[1]);
	strcpy(aGame->player2->pAIModule,argv[2]);
	aGame->player1->cpuControlled=1;
	aGame->player2->cpuControlled=1;
 }
 aGame->player1->LoadAI();
 aGame->player2->LoadAI();
 cout << "gameInit finished" << endl;
 return(0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int startScreen(){
 cout << "startScreen started" << endl; 
 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 glOrtho(0.0,800.0,-600.0,0.0,-10.0,10.0);
 glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();
 SDL_Surface * Start = IMG_Load("dat/StartBild.png");
 //display the startscreen
 glClearColor(0.0,0.0,0.0,1.0);
 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 glColor4f(1.0,1.0,1.0,1.0);
 glRasterPos3f(0.0,-600.0,0.0);
 glDrawPixels(Start->w,Start->h,GL_RGBA,GL_UNSIGNED_BYTE,Start->pixels);
 
 SDL_GL_SwapBuffers();
 SDL_Delay(2000);
 //unload the startscreen
 SDL_FreeSurface(Start);
 nextFunction = &startMenu;
 cout << "startScreen finished" << endl;
 return(0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
int startMenu(){
 cout << "startMenu started" << endl;
 SDL_Event dummyEvent;
 SDL_Event * dumpEvent = &dummyEvent;
 while(SDL_PollEvent(dumpEvent)){
 cout << "Event removed" << endl;
 if(dumpEvent->type==SDL_KEYDOWN){
	 cout << "Event was KEY_DOWN: " << dumpEvent->key.keysym.sym << endl;
  }
  }

 cGui * theGui = new cGui("arial","arial2");
 cout << "gui gestartet" << endl;
 
 cButton * startButton = new cButton;
 cout << "startButton" << endl;
 
 cButton * quitButton = new cButton;
 cout << "quitButton" << endl;
 
 cButton * optionsButton = new cButton;
 cout <<"optionsBUtton" << endl;
 
 theGui->InitBackground("./dat/pine.bmp");
 cout << "background init" <<endl;
 
 strcpy(startButton->label,"Start Game");
 startButton->pressFunction = &gameFunc;
 startButton->leftBorder = 250;
 startButton->rightBorder = 550;
 startButton->top = -100;
 startButton->buttom = -200;
 startButton->myGui=theGui;
 
 strcpy(quitButton->label,"Exit Game");
 quitButton->pressFunction = NULL;
 quitButton->leftBorder = 250;
 quitButton->rightBorder = 550;
 quitButton->top = -400;
 quitButton->buttom = -500;
 quitButton->myGui=theGui;
 
 strcpy(optionsButton->label,"Game Options");
 optionsButton->pressFunction = &gameOptionsFunc;
 optionsButton->leftBorder = 250;
 optionsButton->rightBorder = 550;
 optionsButton->top = -250;
 optionsButton->buttom = -350;
 optionsButton->myGui=theGui;
 
 theGui->AddGuiItem(quitButton);
 theGui->AddGuiItem(startButton);
 theGui->AddGuiItem(optionsButton);
 
 theGui->Loop();
 
 nextFunction = theGui->nextFunction;
 
 delete(theGui);
 delete(startButton);
 delete(quitButton);
 delete(optionsButton);
 cout << "startMenu finished" << endl;
 return(0);
}

int gameOptionsFunc(){
 SDL_Event dummyEvent;
 SDL_Event * dumpEvent = &dummyEvent;
 while(SDL_PollEvent(dumpEvent)){
 	cout << "Event removed" << endl;
 }

 cGui * theGui = new cGui("colophon24","colophon30");

 //SPEICHERLECK IRGENDWO IN LOADFONT!!!!!!!!!!!!!!!!!!
 //theGui->loadFont("colophon24");

 //theGui->loadHighlightFont("colophon30");
 
 theGui->InitBackground("./dat/pine.bmp");

 


 cListDialogue * aiListDialogue1 = new cListDialogue;
 cListDialogue * aiListDialogue2 = new cListDialogue;
 cButton * returnButton = new cButton;
 cTextLabel * TextLabel1 = new cTextLabel;
 cTextLabel * TextLabel2 = new cTextLabel;
 cTextLabel * TextLabel3 = new cTextLabel;
 cTextLabel * TextLabel4 = new cTextLabel;
 cTextLabel * TextLabel5 = new cTextLabel;
 cTextLabel * TextLabelN1 = new cTextLabel;
 cTextLineField * TextLineField1 = new cTextLineField; 
 cCharField * P1_Left = new cCharField;
 cCharField * P1_Right = new cCharField;
 cCharField * P1_Jump = new cCharField;
 cTextLabel * P2_TextLabel1 = new cTextLabel;
 cTextLabel * P2_TextLabel2 = new cTextLabel;
 cTextLabel * P2_TextLabel3 = new cTextLabel;
 cTextLabel * P2_TextLabel4 = new cTextLabel;
 cTextLabel * P2_TextLabel5 = new cTextLabel;
 cTextLabel * P2_TextLabelN1 = new cTextLabel;
 cTextLineField * P2_TextLineField1 = new cTextLineField;
 cCharField * P2_Left = new cCharField;
 cCharField * P2_Right = new cCharField;
 cCharField * P2_Jump = new cCharField;
 

 TextLabel1->leftBorder=125;
 TextLabel1->top=-55;
 TextLabel1->myGui=theGui;
 strcpy(TextLabel1->text,"Player1");
 
 TextLabelN1->leftBorder=50;
 TextLabelN1->top=-125;
 TextLabelN1->myGui=theGui;
 strcpy(TextLabelN1->text,"Name");
 
 cout << "Initialisiere TextLineField1" << endl;
 TextLineField1->leftBorder=150;
 TextLineField1->rightBorder=300;
 TextLineField1->top=-75;
 TextLineField1->buttom=-125;
 TextLineField1->myGui=theGui;
 char editText[256];
 strcpy(editText,aGame->player1->name);
 TextLineField1->text=editText;
 TextLineField1->textLength=40;
 
 TextLabel2->leftBorder=50;
 TextLabel2->top=-195;
 TextLabel2->myGui=theGui;
 strcpy(TextLabel2->text,"Left");
 
 P1_Left->leftBorder = 150;
 P1_Left->rightBorder = 275;
 P1_Left->top=-145;
 P1_Left->buttom=-195;
 P1_Left->keysym = &(aGame->player1->moveLeft);
 P1_Left->myGui=theGui;
 
 TextLabel3->leftBorder=50;
 TextLabel3->top=-265;
 TextLabel3->myGui=theGui;
 strcpy(TextLabel3->text,"Right");
 
 P1_Right->leftBorder = 150;
 P1_Right->rightBorder = 275;
 P1_Right->top = -215;
 P1_Right->buttom = -265; 
 P1_Right->keysym = &(aGame->player1->moveRight);
 P1_Right->myGui = theGui;
 
 TextLabel4->leftBorder=50;
 TextLabel4->top=-340;
 TextLabel4->myGui=theGui;
 strcpy(TextLabel4->text,"Jump");
 
 P1_Jump->leftBorder = 150;
 P1_Jump->rightBorder = 275;
 P1_Jump->top = -290;
 P1_Jump->buttom = -340;
 P1_Jump->keysym = &(aGame->player1->jump);
 P1_Jump->myGui = theGui;
 
 TextLabel5->leftBorder=80;
 TextLabel5->top=-400;
 TextLabel5->myGui=theGui;
 strcpy(TextLabel5->text,"Player1 AI");

 char * list1[3] = {"none", "default","theOtherOne"};
 aiListDialogue1->currentList=list1;
 aiListDialogue1->currentListLength=3;
 char testText1[256];
 strcpy(testText1,aGame->player1->pAIModule);
 aiListDialogue1->text=testText1;
 aiListDialogue1->indexText();
 aiListDialogue1->setPosition(30,255,-410,-600);
 aiListDialogue1->myGui=theGui;
 
//Gui Items for Player2 
 P2_TextLabel1->leftBorder=510;
 P2_TextLabel1->top=-55;
 P2_TextLabel1->myGui=theGui;
 strcpy(P2_TextLabel1->text,"Player2");
 
 P2_TextLabelN1->leftBorder=435;
 P2_TextLabelN1->top=-125;
 P2_TextLabelN1->myGui=theGui;
 strcpy(P2_TextLabelN1->text,"Name");
 
 P2_TextLineField1->leftBorder=535;
 P2_TextLineField1->rightBorder=685;
 P2_TextLineField1->top=-75;
 P2_TextLineField1->buttom=-125;
 P2_TextLineField1->myGui=theGui;
 char editText2[256];
 strcpy(editText2,aGame->player2->name);
 P2_TextLineField1->text=editText2;
 P2_TextLineField1->textLength=40;
 
 P2_TextLabel2->leftBorder=435;
 P2_TextLabel2->top=-195;
 P2_TextLabel2->myGui=theGui;
 strcpy(P2_TextLabel2->text,"Left");
 
 P2_Left->leftBorder = 535;
 P2_Left->rightBorder = 660;
 P2_Left->top=-145;
 P2_Left->buttom=-195;
 P2_Left->keysym = &(aGame->player2->moveLeft);
 P2_Left->myGui=theGui;
 
 P2_TextLabel3->leftBorder=435;
 P2_TextLabel3->top=-265;
 P2_TextLabel3->myGui=theGui;
 strcpy(P2_TextLabel3->text,"Right");
 
 P2_Right->leftBorder = 535;
 P2_Right->rightBorder = 660;
 P2_Right->top = -215;
 P2_Right->buttom = -265; 
 P2_Right->keysym = &(aGame->player2->moveRight);
 P2_Right->myGui = theGui;
 
 P2_TextLabel4->leftBorder=435;
 P2_TextLabel4->top=-340;
 P2_TextLabel4->myGui=theGui;
 strcpy(P2_TextLabel4->text,"Jump");
 
 P2_Jump->leftBorder = 535;
 P2_Jump->rightBorder = 660;
 P2_Jump->top = -290;
 P2_Jump->buttom = -340;
 P2_Jump->keysym = &(aGame->player2->jump);
 P2_Jump->myGui = theGui;
 
 P2_TextLabel5->leftBorder=595;
 P2_TextLabel5->top=-400;
 P2_TextLabel5->myGui=theGui;
 strcpy(P2_TextLabel5->text,"Player2 AI");

 char * list2[3] = {"none", "default","theOtherOne"};
 aiListDialogue2->currentList=list2;
 aiListDialogue2->currentListLength=3;
 char testText2[256];
 strcpy(testText2,aGame->player2->pAIModule);
 aiListDialogue2->text=testText2;
 aiListDialogue2->indexText();
 aiListDialogue2->setPosition(545,770,-410,-600);
 aiListDialogue2->myGui=theGui;
 
 strcpy(returnButton->label,"Main Menu");
 returnButton->pressFunction = &startMenu;
 returnButton->leftBorder = 300;
 returnButton->rightBorder = 500;
 returnButton->top = -525;
 returnButton->buttom = -575;
 returnButton->myGui=theGui;
 
 theGui->AddGuiItem(aiListDialogue1);
 theGui->AddGuiItem(aiListDialogue2);
 theGui->AddGuiItem(returnButton);
 theGui->AddGuiItem(TextLabel1);
 theGui->AddGuiItem(TextLabel2);
 theGui->AddGuiItem(TextLabel3);
 theGui->AddGuiItem(TextLabel4);
 theGui->AddGuiItem(TextLabel5);
 theGui->AddGuiItem(TextLabelN1);
 theGui->AddGuiItem(P1_Left);
 theGui->AddGuiItem(P1_Right);
 theGui->AddGuiItem(P1_Jump);
 theGui->AddGuiItem(TextLineField1);
 theGui->AddGuiItem(P2_TextLabel1);
 theGui->AddGuiItem(P2_TextLabel2);
 theGui->AddGuiItem(P2_TextLabel3);
 theGui->AddGuiItem(P2_TextLabel4);
 theGui->AddGuiItem(P2_TextLabel5);
 theGui->AddGuiItem(P2_TextLabelN1);
 theGui->AddGuiItem(P2_Left);
 theGui->AddGuiItem(P2_Right);
 theGui->AddGuiItem(P2_Jump);
 theGui->AddGuiItem(P2_TextLineField1);

 //initialization of the gui finished, it will loop until sbd. presses the quit button
 theGui->Loop();
 
 
 nextFunction=&startMenu;
 
 //make final changes to player1: name, ai-controlled, ai-module
 strcpy(aGame->player1->name,TextLineField1->text);
 if(strcmp(aiListDialogue1->text,"none")!=0){
 	aGame->player1->cpuControlled=1;
 	strcpy(aGame->player1->pAIModule,aiListDialogue1->text);
 	aGame->player1->LoadAI();
 }
 else{
 	strcpy(aGame->player1->pAIModule,aiListDialogue1->text);
	aGame->player1->cpuControlled=0;
 }
 
 //make final changes to player2: name, ai-controlled, ai-module
 strcpy(aGame->player2->name,P2_TextLineField1->text);
 if(strcmp(aiListDialogue2->text,"none")!=0){
 	aGame->player2->cpuControlled=1;
	strcpy(aGame->player2->pAIModule,aiListDialogue2->text);
 	aGame->player2->LoadAI();
 }
 else{
 	strcpy(aGame->player2->pAIModule,aiListDialogue2->text);
	aGame->player2->cpuControlled=0;
 }
 
 delete(theGui);
 delete(aiListDialogue1);
 delete(aiListDialogue2);
 delete(returnButton);
 delete(TextLabel1);
 delete(TextLabel2);
 delete(TextLabel3);
 delete(TextLabel4);
 delete(TextLabel5);
 delete(TextLabelN1);
 delete(P1_Left);
 delete(P1_Right);
 delete(P1_Jump);
 delete(TextLineField1);
 delete(P2_TextLabel1);
 delete(P2_TextLabel2);
 delete(P2_TextLabel3);
 delete(P2_TextLabel4);
 delete(P2_TextLabel5);
 delete(P2_TextLabelN1);
 delete(P2_Left);
 delete(P2_Right);
 delete(P2_Jump);
 delete(P2_TextLineField1);
 return(0);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

int gameFunc(){
cout << "gameFunc started" << endl;
SDL_Event dummyEvent;
SDL_Event * dumpEvent = &dummyEvent;
while(SDL_PollEvent(dumpEvent)){
}

int leave = 0;
int mouseLocation = 0;

SDL_Surface * Bogol1;
SDL_Surface * Bogol2;
SDL_Surface * BogoR1;
SDL_Surface * BogoR2;
SDL_Surface * BallPic;


SDL_ShowCursor(SDL_DISABLE);

//initialize OpenGL
cout << "Initializing OpenGL" << endl;
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
glOrtho(0.0,800.0,-600.0,0.0,-10.0,10.0);
glMatrixMode(GL_MODELVIEW);
glLoadIdentity();
glEnable(GL_ALPHA_TEST);
glAlphaFunc(GL_GREATER,0);
//glEnable(GL_DEPTH_TEST);
glClearColor(0.0,0.0,0.0,0.0);
glColor4f(1.0,1.0,1.0,1.0);
cout << "GL init complete" << endl;

cout << "Loading Bogo Images" << endl;
Bogol1 = IMG_Load("dat/Bogol1.png");
Bogol2 = IMG_Load("dat/Bogol2.png");
BogoR1 = IMG_Load("dat/BogoR1.png");
BogoR2 = IMG_Load("dat/BogoR2.png");
BallPic = IMG_Load("dat/Ball.png");
cout << "Images Loaded" << endl;

int start=0;


cout << "Setting up game and player structures" << endl;
aGame->player1->skin[0][0] = Bogol1;
aGame->player1->skin[0][1] = Bogol2;
aGame->player1->skin[1][0] = BogoR1;
aGame->player1->skin[1][1] = BogoR2;
aGame->player2->skin[0][0] = Bogol1;
aGame->player2->skin[0][1] = Bogol2;
aGame->player2->skin[1][0] = BogoR1;
aGame->player2->skin[1][1] = BogoR2;

aGame->theBall->ballSkin=BallPic;
aGame->theBall->lLimit=0+34;
aGame->theBall->rLimit=SCREEN_WIDTH-34;
aGame->theBall->upperLimit=34;
aGame->theBall->lowerLimit=SCREEN_HEIGHT-34;

aGame->finished=0;
aGame->finDelay=0;
aGame->player1->area=0;
aGame->player1->moveDirection=1;
aGame->player1->points=0;
aGame->player1->plays=0;
aGame->player1->looser=1;
aGame->player2->area=1;
aGame->player2->moveDirection=0;
aGame->player2->points=0;
aGame->player2->plays=0;
aGame->swt2=0;
aGame->theBall->start=1;
aGame->player1->InitArea0();
aGame->player2->InitArea1();
aGame->theBall->InitBall(0);
cout << "Game and player setup complete" << endl;


Uint8 * KeysPointer;
char DummyString[2];

DummyString[1]='\0';

//rewrite InitScoreGui and PrintScoreGui so that InitScoreGui can be called from the constructor of cGame one day....
cout << "InitScoreGui" << endl;
aGame->InitScoreGui();
cout << "Score UI init complete" << endl;
while ((aGame->finished!=1) && (aGame->finished!=-1)) {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClearColor(1.0,0.0,0.0,1.0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	SDL_PumpEvents();

	KeysPointer = SDL_GetKeyState(0);

//the game mechanics 
	cout << "Starting Game Mechanics" << endl;
	aGame->ProcessQuitPressed(KeysPointer);
	aGame->ProcessResSwitch(KeysPointer);
	aGame->ProcessStart(KeysPointer);
	
	//watch for user input or ai-instructions
	aGame->player1->ReactToWorld(KeysPointer,aGame->player2,aGame->theBall);
	aGame->player2->ReactToWorld(KeysPointer,aGame->player1,aGame->theBall);

	aGame->CalcTimescale();
	
	//move the players according to user input or ai-instrctions
	aGame->player1->Move(aGame->timeScale);
	aGame->player2->Move(aGame->timeScale);

	aGame->theBall->MoveAndCollideWall(aGame->timeScale);
	aGame->theBall->CollidePlayer(aGame->player1,aGame->timeScale);
	aGame->theBall->CollidePlayer(aGame->player2,aGame->timeScale);
	aGame->theBall->CollideNet();
	
//game mechanics finished
cout << "Game mechanics finished" << endl;

//game graphics
cout << "Starting Game graphics" << endl;
	
	aGame->DrawGameBackground();

cout << "Draw players" << endl;	
	aGame->player1->Draw();
	aGame->player2->Draw();
	
cout << "Draw ball" << endl;
	aGame->theBall->Draw();

cout << "Draw net" << endl;
	//blit the .net (hohoho)
	aGame->DrawNet();
cout << "Draw gui" << endl;
	aGame->PrintScoreGui();
cout << "Swap" << endl;
	SDL_GL_SwapBuffers();
//graphics finished
cout << "Game Graphics finished" << endl;

//game rules
	aGame -> CheckScore();
	aGame -> ProcessDelay();
//game rules finished
}


glClearColor(0.0f,1.0f,1.0f,1.0f);
glClear(GL_COLOR_BUFFER_BIT);

//print a messsage annoncing who has won the game
if (aGame->finished!=-1) {

	cout << "spiel zwischen " << aGame->player1->name << " und " << aGame->player2->name << " beendet." << endl;
	cout << "Hat " << aGame->player1->name << " gewonnen? " << aGame->player1->winner << " oder " << aGame->player2->name << " " << aGame->player2->winner << "." << endl; 
	
	cTextLabel * VictoryMessage = new cTextLabel;
	cout << "VictoryMessage angelegt." << endl;
	VictoryMessage->leftBorder=150.0;
	VictoryMessage->top=-270.0;
	cout << "Koordinaten gesetzt" << endl;
	VictoryMessage->myGui=aGame->tGui;

	if (aGame->player1->winner==1 ) {
		strcpy(VictoryMessage->text,aGame->player1->name);
		strcat(VictoryMessage->text," ist der Strahlende Sieger!");
	}
	else {
		strcpy(VictoryMessage->text,aGame->player2->name);
		strcat(VictoryMessage->text," ist der Strahlende Sieger!");
	}
	cout << "Sieger ermittelt" << endl;
	VictoryMessage->Print();
	cout << "Meldung angezeigt" << endl;
	SDL_GL_SwapBuffers();
	cout << "Buffer-Swap" << endl;
	SDL_Delay(3500);
	cout << "Fertig gewartet" << endl;
	delete(VictoryMessage);

}

//releasing the SDL_Surfaces
SDL_FreeSurface(Bogol1);
cerr << "Freed: Bogol1" << endl;
SDL_FreeSurface(Bogol2);
cerr << "Freed: Bogol2" << endl;
SDL_FreeSurface(BogoR1);
cerr << "Freed: BogoR1" << endl;
SDL_FreeSurface(BogoR2);
cerr << "Freed: BogoR2" << endl;
SDL_FreeSurface(BallPic);
cerr << "Freed: BallPic" << endl;

cerr << "quit"<< endl;

glDisable(GL_DEPTH_TEST);
glDisable(GL_ALPHA_TEST);
glDisable(GL_TEXTURE_2D);
SDL_ShowCursor(SDL_ENABLE);

aGame->DeleteScoreGui();

nextFunction=&startMenu;
return(0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char * argv[]){
putenv("PYTHONPATH=./python/");
Py_Initialize();

SDL_Init(SDL_INIT_VIDEO);
atexit(SDL_Quit);
SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);
screen=SDL_SetVideoMode(SCREEN_WIDTH,SCREEN_HEIGHT,24,SDL_OPENGL);
SDL_Surface * Ico = SDL_LoadBMP("dat/bogoimg.bmp");
SDL_WM_SetIcon(Ico,0);
SDL_WM_SetCaption("glBogo","./dat/bogoimg.bmp");

cBall * aBall = new cBall;
cPlayer * aPlayer1 = new cPlayer;
cPlayer * aPlayer2 = new cPlayer;
aGame = new cGame;

aGame->player1=aPlayer1;
aGame->player2=aPlayer2;
aGame->theBall=aBall;

gameInit(argc,argv);

nextFunction=&startScreen;

while (nextFunction != 0){
	nextFunction();
}


Py_Finalize();
SDL_FreeSurface(screen);
delete(aGame);
delete(aPlayer1);
delete(aPlayer2);
delete(aBall);
return(0);
}
