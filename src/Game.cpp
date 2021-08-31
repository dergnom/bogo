#ifdef WIN32
#include "Python/Python.h"
#include <windows.h>
#include "GL/gl.h"
#include "GL/glext.h"
#else
#include "Python.h"
#include "GL/gl.h"
#endif

#include <Player.h>
#include <Ball.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <constants.h>
#include <Game.h>
#include <iostream>
#include <stdlib.h>

extern GLuint TexName;

using namespace std;


cGame::cGame(void) {
	swt2=0;
	finished=0;
	timeScaleSwitch=1;
	timeScale=1.0;
	finDelay=0;
	ticks1=0;
	ticks2=0;
	//generate the Background texture
	glEnable(GL_TEXTURE_2D);
	glDeleteTextures(1,&TexName);
	glGenTextures(1,&TexName);
	glBindTexture(GL_TEXTURE_2D,TexName);
	SDL_Surface * Tex = IMG_Load("dat/textured.png");
	if (Tex==0) {cerr << "error loading texture" << endl; }
	glTexImage2D(GL_TEXTURE_2D,0,3,Tex->w,Tex->h,0,GL_RGB,GL_UNSIGNED_BYTE,Tex->pixels);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);
	glDisable(GL_TEXTURE_2D);

	cerr << "Texture generated" << endl;
	SDL_FreeSurface(Tex);
	//Background Texture Generated
	net = IMG_Load("dat/net.png");
	if (!(net)){
		cout << "Konnte die Datei ./dat/net.png nicht laden" << endl;
		std::exit(1);
	}
}

cGame::~cGame(void){
	glDeleteTextures(1,&TexName);
	SDL_FreeSurface(net);
	cerr << "Freed: net" << endl;	
}

void cGame::CalcTimescale() {
//cout << "timeScaleSwitch " << timeScaleSwitch << endl;
if (timeScaleSwitch!=0) {
	if (timeScaleSwitch>0) {
		ticks1=ticks2;
		ticks2=SDL_GetTicks();
		//cout << "ticks1 " << ticks1 << endl;
		//cout << "ticks2 " << ticks2 << endl;
		timeScale=(float)((float)((ticks2-ticks1))/30);

	}
	else{
		timeScale=float(timeScaleSwitch)*(-1);
	}

}
else {
	timeScale=1;

}
}



void cGame::CheckScore() {

int ar;
if (theBall->checkScore==1){

	if(theBall->posY >= SCREEN_HEIGHT-34) {

		if(theBall->posX < SCREEN_WIDTH /2) {
			ar=0;
		}
		else {
			ar=1;
		}

		if(ar==player1->area) {
			player1->looser=1;
			player1->winner=0;
			player2->looser=0;
			player2->winner=1;
			player2->points=player2->points+1;
			theBall->outTime=SDL_GetTicks();

		}
		else{
			player1->looser=0;
			player1->winner=1;
			player2->looser=1;
			player2->winner=0;
			player1->points=player1->points+1;
			theBall->outTime=SDL_GetTicks();

		}

		/*check if a player reached more than 5 points and a play is finished*/

		if ( (player1->winner==1) && (player1->points>5) ){
			cerr << "switching" << endl;
			swt2=1;
			player1->plays=player1->plays+1;
			if (player1->plays > 2) {
				finDelay=1;
			}
		}
		else{
			if( (player2->points>5) && (player2->winner==1) ) {
			cerr << "switching" << endl;
			swt2=1;
			player2->plays=player2->plays+1;
			if (player2->plays > 2) {
				finDelay=1;
			}
			}
		}



	theBall->checkScore=0;
	}


}

}




void cGame::ProcessDelay(){

if (theBall->out==1) {
	if(SDL_GetTicks() - theBall->outTime > theBall->outDelay *1000) {
		RestartGame(swt2);
		if (finDelay==1) {
			finished=1;
		}
	theBall->out=0;
	}


}
}


void cGame::ProcessStart(Uint8 * keystate) {
if (keystate[SDLK_SPACE]==1) {
	theBall->start=0;
}

}


void cGame::ProcessQuitPressed(Uint8 * keystate) {
if (keystate[SDLK_q]==1) {
	finished=-1;
}

}



void cGame::RestartGame(int swt2) {
int helper;
if(  ( (player1->winner==1) && (player1->points>5) ) || ((player2->points>5) && (player2->winner==1) ) ) {
 cerr << "Switched" << endl;
 cerr << "swt2= " << swt2 << endl;
 helper = player1->area;
 player1->area=player2->area;
 player2->area=helper;
 player1->points=0;
 player2->points=0;

 if (player1->area==0) {
	player1->InitArea0();
	player2->InitArea1();
	swt2=0;
 }
 else {
	player1->InitArea1();
	player2->InitArea0();
	swt2=0;

 }

}

if (player1->looser==1) {
theBall->InitBall(player1->area);
}
else {
theBall->InitBall(player2->area);
}
swt2=0;
cerr << "RestartGame swt2: " << swt2 << endl;
}


void cGame::ProcessResSwitch(Uint8 * keystate) {

if(keystate[SDLK_f]==1) {
	glDeleteTextures(1,&TexName);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);
	SDL_SetVideoMode(800,600,0,SDL_OPENGL | SDL_FULLSCREEN);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER,0);
	glEnable(GL_TEXTURE_2D);
	glClearColor(0.0,0.0,0.0,0.0);
	glColor4f(1.0,1.0,1.0,1.0);
	glClearColor(0.0,0.0,0.0,1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0,800.0,-600.0,0.0,0.0,10.0);
	glEnable(GL_TEXTURE_2D);
	glMatrixMode(GL_MODELVIEW);
	
	//generate the background texture
	glGenTextures(1,&TexName);
	glBindTexture(GL_TEXTURE_2D,TexName);
	SDL_Surface * Tex = IMG_Load("dat/textured.png");
	if (Tex==0) {cerr << "error loading texture" << endl; std::exit(1);}
	glTexImage2D(GL_TEXTURE_2D,0,3,Tex->w,Tex->h,0,GL_RGB,GL_UNSIGNED_BYTE,Tex->pixels);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);

	cerr << "Texture generated" << endl;
	SDL_FreeSurface(Tex);
	//texture generation complete

		
}

else {
	if(keystate[SDLK_g]==1) {
	glDeleteTextures(1,&TexName);
	SDL_SetVideoMode(800,600,0,SDL_OPENGL);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER,0);
	glEnable(GL_TEXTURE_2D);
	glClearColor(0.0,0.0,0.0,0.0);
	glColor4f(1.0,1.0,1.0,1.0);
	glClearColor(0.0,0.0,0.0,1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0,800.0,-600.0,0.0,0.0,10.0);
	glEnable(GL_TEXTURE_2D);
	glMatrixMode(GL_MODELVIEW);
	
	//generate the background texture
	glGenTextures(1,&TexName);
	glBindTexture(GL_TEXTURE_2D,TexName);
	SDL_Surface * Tex = IMG_Load("dat/textured.png");
	if (Tex==0) {cerr << "error loading texture" << endl; std::exit(1);}
	glTexImage2D(GL_TEXTURE_2D,0,3,Tex->w,Tex->h,0,GL_RGB,GL_UNSIGNED_BYTE,Tex->pixels);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);

	cerr << "Texture generated" << endl;
	SDL_FreeSurface(Tex);
	//texture generation complete
	}
}

}


int cGame::DrawGameBackground(){
	int w = (int) SCREEN_WIDTH ;
	int h = (int) SCREEN_HEIGHT ;
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,TexName);
	glLoadIdentity();
	//glColor4f(0.0,0.0,1.0,1.0);
	glBegin(GL_QUADS);
		glTexCoord2i(0,0);
		glVertex3i(0,-h,-1);
		glTexCoord2i(6,0);
		glVertex3i(w,-h,-1);
		glTexCoord2i(6,4);
		glVertex3i(w,0,-1);
		glTexCoord2i(0,4);
		glVertex3i(0,0,-1);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	return(0);
}

int cGame::DrawNet(){
	float destx=390.0f;
	float desty=-600.0f;
	glLoadIdentity();
	glRasterPos3f( destx,desty,0.0 );
	glDrawPixels(net->w,net->h,GL_RGB,GL_UNSIGNED_BYTE,net->pixels);
	return(0);	
}

int cGame::InitScoreGui(){
	tGui = new cGui;
	tGui->loadFont("colophon24");
	tGui->loadHighlightFont("colophon30");
	LabelPoints = new cTextLabel;
	LabelPlays = new cTextLabel;
	Player1Name = new cTextLabel;
	Player2Name = new cTextLabel;

	LabelPointsLeft = new cTextLabel;
	LabelPointsRight = new cTextLabel;

	LabelPlaysLeft = new cTextLabel;
	LabelPlaysRight = new cTextLabel;

	LabelPoints->leftBorder=20.0;
	LabelPoints->top=-80.0;
	strcpy(LabelPoints->text,"Punkte");
	LabelPoints->myGui=tGui;
	strcpy(LabelPoints->idTag,"LabelPoints");
	tGui->AddGuiItem(LabelPoints);

	LabelPlays->leftBorder=20.0;
	LabelPlays->top=-150.0;
	strcpy(LabelPlays->text,"Saetze");
	LabelPlays->myGui=tGui;
	strcpy(LabelPlays->idTag,"LabelPlays");
	tGui->AddGuiItem(LabelPlays);

	Player1Name->myGui=tGui;
	strcpy(Player1Name->text,player1->name);
	strcpy(Player1Name->idTag,"Player1Name");
	tGui->AddGuiItem(Player1Name);

	Player2Name->myGui=tGui;
	strcpy(Player2Name->text,player2->name);
	strcpy(Player2Name->idTag,"Player2Name");
	tGui->AddGuiItem(Player2Name);

	LabelPointsLeft->myGui=tGui;
	tGui->AddGuiItem(LabelPointsLeft);
	strcpy(LabelPointsLeft->idTag,"LabelPointsLeft");
	LabelPointsLeft->leftBorder=170.0;
	LabelPointsLeft->top=-80.0;

	LabelPlaysLeft->myGui=tGui;
	tGui->AddGuiItem(LabelPlaysLeft);
	strcpy(LabelPlaysLeft->idTag,"LabelPlaysLeft");
	LabelPlaysLeft->leftBorder=170.0;
	LabelPlaysLeft->top=-150.0;

	LabelPointsRight->myGui=tGui;
	tGui->AddGuiItem(LabelPointsRight);
	strcpy(LabelPointsRight->idTag,"LabelPointsRight");
	LabelPointsRight->leftBorder=550.0;
	LabelPointsRight->top=-80.0;

	LabelPlaysRight->myGui=tGui;
	tGui->AddGuiItem(LabelPlaysRight);
	strcpy(LabelPlaysRight->idTag,"LabelPlaysRight");
	LabelPlaysRight->leftBorder=550.0;
	LabelPlaysRight->top=-150.0;	
	
	return(0);
}

int cGame::PrintScoreGui(){
	
	//paint the game stats above each player
	LabelPoints->Print();
	LabelPlays->Print();
	
	if (player1->area==0) {
		Player1Name->leftBorder=115.0;
		Player1Name->top=-40.0;
		sprintf(LabelPointsLeft->text,"%i",player1->points);
		sprintf(LabelPlaysLeft->text,"%i",player1->plays);

		Player2Name->leftBorder=515.0;
		Player2Name->top=-40.0;
		sprintf(LabelPointsRight->text,"%i",player2->points);
		sprintf(LabelPlaysRight->text,"%i",player2->plays);
	}
	else{
		Player2Name->leftBorder=115.0;
		Player2Name->top=-40.0;
		sprintf(LabelPointsLeft->text,"%i",player2->points);
		sprintf(LabelPlaysLeft->text,"%i",player2->plays);


		Player1Name->leftBorder=515.0;
		Player1Name->top=-40.0;
		sprintf(LabelPointsRight->text,"%i",player1->points);
		sprintf(LabelPlaysRight->text,"%i",player1->plays);
	}//game stats completed
	
	Player1Name->Print();
	Player2Name->Print();
	LabelPointsLeft->Print();
	LabelPlaysLeft->Print();
	LabelPointsRight->Print();
	LabelPlaysRight->Print();	
}

int cGame::DeleteScoreGui(){
	tGui->DeleteAllGuiItems();
	delete(tGui);
	return(0);
}


