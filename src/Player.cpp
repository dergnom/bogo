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
#include <iostream>
#include "MovingObject.h"
#include "Player.h"
#include "constants.h"



/*class cPlayer : public cMovingObject {

public:
	char name[20];
	int points;
	int plays;
	int area;
	SDLKey moveLeft;
	SDLKey moveRight;
	SDLKey jump;
	//cpu...not used yet
	int cpuControlled;
	int moveDirection;
	int moveFrame;
	SDL_Surface * skin[2][2];
	int winner;
	int looser;
	int framecounter;
	char pAIModule[20];

	void LoadAI();
	void ReactToWorld(Uint8 * KeySnapshot,cPlayer * OtherPlayer,cBall * theBall);
	void Move(float timescale);
	void InitArea0();
	void InitArea1();
	cPlayer(void);
	
private:
	void ReactToKeystate(Uint8 * KeySnapshot);
	void ReactToAI(cPlayer * OtherPlayer,cBall * theBall);
	PyObject * pAIFunc;
	PyObject * pAIContainer;

};*/

using namespace std;

/*constructor*/
cPlayer::cPlayer() {

points=0;
area=0;
cpuControlled=0;
plays=0;
moveFrame=1;
framecounter=0;
strcpy(pAIModule,"default");
pAIContainer=Py_BuildValue("(ii)",1,2);
Py_INCREF(pAIContainer);
}

void cPlayer::LoadAI(){
if(cpuControlled==1){
	PyObject * pModuleName = PyString_FromString(pAIModule);
	PyObject * pModule;
	if(! (pModule = PyImport_Import(pModuleName)) ){
				cerr << "Couldn't load " << pAIModule << endl;
	}
	pAIFunc=PyObject_GetAttrString(pModule,"aifunc");
	Py_INCREF(pAIFunc);
	/*if(PyCallable_Check(pAIFunc)){
		cerr << "Everything fine, pAIFunc callable" << endl;
	}
	else{
		cerr << "Grave Error: pAIFunc not callable" << endl;
		exit(1);
	}*/
	cout << name <<" cpuControlled. AI \"" << pAIModule << "\" loaded" << endl;
}
else{
	cout << name <<" not cpuControlled. Not loading AI" << endl; 
}
}

void cPlayer::InitArea1() {
cout << "Init 1" << endl;
rLimit=(int) (SCREEN_WIDTH-23);
lLimit= (int) (SCREEN_WIDTH/2+R+23);
posX= (float) (SCREEN_WIDTH*0.75);
posY= (float) (300.0);
velX= 0.0;
velY= 0.0;
area=1;
}

void cPlayer::InitArea0() {
cout << "Init 0" << endl;
rLimit=(int) (SCREEN_WIDTH/2-R-23);
lLimit= (int) (0+23);
posX= (float) (SCREEN_WIDTH*0.25);
posY= (float) (300);
velY= 0;
velX= 0;
area= 0;
}


void cPlayer::ReactToWorld(Uint8 * KeySnapshot,cPlayer * otherPlayer,cBall * theBall){
if(cpuControlled==1){
	ReactToAI(otherPlayer,theBall,KeySnapshot);
}
else{
	ReactToKeystate(KeySnapshot);
}
}

void cPlayer::ReactToAI(cPlayer * otherPlayer,cBall * theBall,Uint8 * KeySnapshot){

int rPressed=0;int lPressed=0;int JumpPressed=0;
PyObject * pPlTuple = Py_BuildValue("(ffii)",posX,posY,points,plays);
PyObject * pPlOTuple = Py_BuildValue("(ffii)",otherPlayer->posX,otherPlayer->posY,otherPlayer->points,otherPlayer->plays);
PyObject * pBallTuple = Py_BuildValue("(ffff)",theBall->posX,theBall->posY,theBall->velX,theBall->velY);
PyObject * pArgs = Py_BuildValue("(OOOO)",pPlTuple,pPlOTuple,pBallTuple,pAIContainer);
/*if(pArgs){
	cerr << "pArgs generated" << endl;
}
else{
	cerr << "pArgs not generated" << endl;
}*/
//pass the relevant information to the ai-routine
Py_DECREF(pPlTuple);
Py_DECREF(pPlOTuple);
Py_DECREF(pBallTuple);
PyObject * pReturn=PyObject_CallObject(pAIFunc,pArgs);
Py_DECREF(pArgs);
/*if(pReturn!=NULL){
	cerr << "Call to pAIFunc seems to be OK." << endl;
}
else{
	cerr << "pAIFunc returned error!" << endl;
}*/
//release the unused python variables
if(PyErr_Occurred()){
	PyErr_Print();
}
//convert the tuple returned by the ai-routine to c-integers. pAIContainer will be passed again to the ai-routine during its next run. it depends on this routine if it makes use of this container.
PyArg_ParseTuple(pReturn,"iiiO",&rPressed,&lPressed,&JumpPressed,&pAIContainer);
Py_DECREF(pReturn);
//Based on the action the AI-Routine calls for a KeySnapshot is generated and passed to the ReactToKeystateMethod
KeySnapshot[moveLeft]=lPressed;
KeySnapshot[moveRight]=rPressed;
KeySnapshot[jump]=JumpPressed;
//ReactToKeystate will be kind enough to calculate the movement of the cpuControlled player.
ReactToKeystate(KeySnapshot);
}

void cPlayer::ReactToKeystate(Uint8 * KeySnapshot){
 /*check if player stands on the floor*/

if (posY<71){
	posY=71;
	velY=0;
}
if (posY>=SCREEN_HEIGHT-71) { 
	velX=0;
	if (KeySnapshot[moveLeft]==1) {
		velX=(float)(-MOVE);
		moveDirection=1;
		framecounter=framecounter+1;
	}
	if (KeySnapshot[moveRight]==1) {
		velX=(float)(MOVE);
		moveDirection=0;
		framecounter=framecounter+1;
	}
	if( (KeySnapshot[moveRight]==0) && (KeySnapshot[moveLeft]==0) ) {
		velX=0;
	}
	if (KeySnapshot[jump]==1) {
		velY=(float)(-JUMP);
	}
}
else{  velX=0;
	if (KeySnapshot[moveLeft]==1){
		velX=(float)(velX-MOVE);
		moveDirection=1;
		if (velX>VEL_MAX) {velX=VEL_MAX;}
	}
	if (KeySnapshot[moveRight]==1){
		velX=(float)(velX+MOVE);
		moveDirection=0;
		if (velX>VEL_MAX) {velX=VEL_MAX;}
	}
}

}



void cPlayer::Move(float timescale) {
//unused?: int zeiger;
SDL_Rect src2,dest2;
src2.x=0;
src2.y=0;
dest2.x=0;
dest2.y=0;
/*move  the player in y direction. first apply the GRAV constant to the current speed to make sure the the player is
falling. make sure that the player stops after hitting the ground*/
velY = velY+(float)(3*timescale*GRAV);
//cerr << "Timescale: " << timescale << endl;

if (velY+posY >= SCREEN_HEIGHT-71) {
	posY=(float)(SCREEN_HEIGHT-71);
	velY=0;
	}
else {
	posY=posY+(float)(velY*timescale);

}

/*Move the player in x direction. check if the player leaves his area if he moves with his current speed*/
if (posX+velX < lLimit) {
	posX=(float)(lLimit);
}

else {
	if (posX +velX > rLimit){
		posX=float(rLimit);
	}
	else{

		posX=posX+(float)(velX*timescale);
		//velX=0;
  	}
}


/*let the painted frame change every UPDATE times this function is run to achieve the animation*/
//framecounter is incremented by the react to keystate function
if (framecounter>3*timescale) {
	framecounter=0;
	moveFrame=moveFrame*(-1);
}

}

int cPlayer::Draw(){
	float destx=posX-23.0f;
	float desty=posY-71.0f+skin[0][0]->h;
	int AnimFrame = (int)(  ((moveFrame+1)/2));
	glLoadIdentity();
	glRasterPos3f(destx,desty*(-1.0f),0.0f);
	SDL_Surface * dummy=skin[moveDirection][AnimFrame];
	glDrawPixels(dummy->w,dummy->h,GL_RGBA,GL_UNSIGNED_BYTE,dummy->pixels);
	return(0);	
}

