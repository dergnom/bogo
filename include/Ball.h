#ifndef BALL
#define BALL

#ifdef WIN32
#include "Python/Python.h"
#include <windows.h>
#include "GL/gl.h"
#include "GL/glext.h"
#else
#include "Python.h"
#include "GL/gl.h"
#endif

#include <SDL/SDL.h>
#include <Player.h>
#include <constants.h>
#include <math.h>


class cPlayer;

class cBall : public cMovingObject {

public: int out;
	 int checkScore;
	 int start;
	 unsigned int outDelay;
	 unsigned int outTime;
	 SDL_Surface * ballSkin;
	 
	 
	 void CollidePlayer(cPlayer * player,float timescale,SDL_Surface * screen);
	 void CollidePlayer(cPlayer * player, float timescale);
	 void InitBall(int area);
	 void MoveAndCollideWall(float timescale);
	 void CollideNet();
	 int Draw();
	 cBall(void);
};

#endif


