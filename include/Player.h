#ifndef PLAYER
#define PLAYER


#ifdef WIN32
#include "Python/Python.h"
#include <windows.h>
#include "GL/gl.h"
#include "GL/glext.h"
#else
#include "Python.h"
#include "GL/gl.h"
#endif


#include <MovingObject.h>
#include <SDL/SDL.h>
#include <constants.h>
#include "Ball.h"

class cBall;

class cPlayer : public cMovingObject {

public:
	char name[256];
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
	int Draw();
	cPlayer(void);

	
private:
	void ReactToKeystate(Uint8 * KeySnapshot);
	void ReactToAI(cPlayer * OtherPlayer,cBall * theBall,Uint8 * KeySnapshot);
	PyObject * pAIFunc;
	PyObject * pAIContainer;

};

#endif
