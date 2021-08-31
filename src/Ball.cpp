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
#include <MovingObject.h>
#include <Player.h>
#include <constants.h>
#include <math.h>
#include <Ball.h>
#include <iostream>

using namespace std;



cBall::cBall(void) {
	out=0;
	start=0;
	outDelay=1;
	outTime=0;

}


void cBall::InitBall(int area) {
velX=0;
velY=0;
out=0;
start=1;
if (area==0) {
	posX=(float) (SCREEN_WIDTH*0.25);
	posY=(float) (SCREEN_HEIGHT*0.25);
}
else {
	posX=(float) (SCREEN_WIDTH*0.75);
	posY=(float) (SCREEN_HEIGHT*0.25);
}

}



void cBall::MoveAndCollideWall(float timescale) {
 if (start!=1) {
	posX=posX+velX*timescale;
	posY=posY+velY*timescale;
	velY=velY+BALLGRAV*timescale;

	if (posX<lLimit){
		velX=velX*(-1);
		posX=lLimit;
	}

	if (posX>rLimit){
		velX=velX*(-1);
		posX=rLimit;
	}


	if (posY>= SCREEN_HEIGHT-34) {
		velY=velY*(-1);
		posY=lowerLimit;

	}

	if(posY<34) {
		velY=velY*(-1);
		posY=upperLimit;

	}


	if (posY >= SCREEN_HEIGHT-34) {

		if (out==0) {
			out=1;
			checkScore=1;
			outTime=SDL_GetTicks();
		}
	}
}
}


void cBall::CollideNet() {
float normDelta, deltaX, deltaY, x, y, phi, t1, t2, r1, r2;
if (posY > YM-R-34) {
	if ((posY < YM) && ( (posX>LEFT_BORDER_OF_NET-34)&&(posX<RIGHT_BORDER_OF_NET)) ) {
		/*deltaX= (float) (posX-XM);
		deltaY=(float) (posY-YM);
		posX=(float) (XM+deltaX/sqrt(deltaX*deltaX+deltaY*deltaY)*(R+34));
		posY=(float) (YM+deltaY/sqrt(deltaY*deltaX+deltaY*deltaY)*(R+34));
		phi=(float) (acos(deltaX/sqrt(deltaX*deltaX+deltaY*deltaY)));

		x=velX; y=velY;
		r1=cos(phi)*x-sin(phi)*y;
		t1=sin(phi)*x+cos(phi)*y;
		r2=(-1)*e*r1; t2=t1;
		x=r2*cos(phi)+t2*sin(phi);
		y=t2*cos(phi)-r2*sin(phi);

		velX=(float)(x); velY=(float)(y);*/
		deltaX = float(posX - XM);
		deltaY = float(posY - YM);
		normDelta=sqrt((deltaX*deltaX+deltaY*deltaY));
		posX=posX+deltaX/normDelta*10;
		posY=posY+deltaY/normDelta*10;

		deltaX = float(posX-XM);
		deltaY = float(posY-YM);
		phi=acos(deltaX/sqrt(deltaX*deltaX+deltaY*deltaY));
		x=velX; y=velY;

		r1=cos(phi)*x-sin(phi)*y;
		t1=sin(phi)*x+cos(phi)*y;
		r2=e*(-1)*r1; t2=e*t1;
		x=r2*cos(phi)+t2*sin(phi);
		y=t2*cos(phi)-r2*sin(phi);

		velX=(float)(x); velY=(float)(y);

	}

	else {

		if ((posX>LEFT_BORDER_OF_NET-34) && (posX < SCREEN_WIDTH / 2)){
			posX=LEFT_BORDER_OF_NET-34;
			velX=(float) (velX*e*(-1));
		}
		if ((posX<RIGHT_BORDER_OF_NET+34) && (posX > SCREEN_WIDTH / 2)){
			posX=RIGHT_BORDER_OF_NET+34;
			velX=(float) (velX*e*(-1));
		}
	}
}

}



void cBall::CollidePlayer(cPlayer * player, float timescale, SDL_Surface * screen) {
if (start!=1) {
	int playerHit=0;
	int hitPositionY;
	float deltaX;
	float deltaY;
	float phi;
	float x,y,r1,t1,r2,t2,normDelta;
	//unused?: SDL_Rect src,dest;


	/*check where the Player is hit by the ball*/
	if(  ( sqrt( pow(posX-(player->posX)-0,2) + pow(posY-player->posY-48,2) ) ) < 53  ) {
		hitPositionY=-48;
		playerHit=1;
	}
	else {
	if( (sqrt( pow(posX-player->posX-0,2) + pow(posY-player->posY+48,2) )  ) < 53 ) {
		hitPositionY=48;
		playerHit=1;
	}

	}



	if (playerHit==1) {
		deltaX= float(posX - player->posX);
		deltaY= float(posY - player->posY);
		normDelta=sqrt((deltaX*deltaX+deltaY*deltaY));
		posX=posX+deltaX/normDelta*37;
		posY=posY-hitPositionY+deltaY/normDelta*37;

		deltaX= float(posX-player->posX);
		deltaY= float(posY-player->posY+hitPositionY);
		phi=acos(deltaX/sqrt(deltaX*deltaX+deltaY*deltaY));
		x=velX+player->velX; y=velY-player->velY;
		r1=cos(phi)*x-sin(phi)*y;
		t1=sin(phi)*x+cos(phi)*y;
		r2=e_BOGO*(-1)*r1; t2=e_BOGO*t1;
		x=r2*cos(phi)+t2*sin(phi);
		y=t2*cos(phi)-r2*sin(phi);

		velX=(float)(x); velY=(float)(y);
		playerHit=0;




	}




	}

	if (sqrt(float(velX*velX)+float(velY*velY))>VELBALLMAX)
		{
		  velX= float (velX/sqrt((float(velX*velX)+float(velY*velY)))*VELBALLMAX);
		  velY= float (velY/sqrt((float(velX*velX)+float(velY*velY)))*VELBALLMAX);
	}



}


void cBall::CollidePlayer(cPlayer * player, float timescale) {
if (start!=1) {
	int playerHit=0;
	int hitPositionY;
	float deltaX;
	float deltaY;
	float phi;
	float x,y,r1,t1,r2,t2,normDelta;
	//unused?: SDL_Rect src,dest;


	/*check where the Player is hit by the ball*/
	if(  ( sqrt( pow(posX-(player->posX)-0,2) + pow(posY-player->posY-48,2) ) ) < 53  ) {
		hitPositionY=-48;
		playerHit=1;
	}
	else {
	if( (sqrt( pow(posX-player->posX-0,2) + pow(posY-player->posY+48,2) )  ) < 53 ) {
		hitPositionY=48;
		playerHit=1;
	}

	}



	if (playerHit==1) {
		deltaX= float(posX - player->posX);
		deltaY= float(posY - player->posY);
		normDelta=sqrt((deltaX*deltaX+deltaY*deltaY));
		posX=posX+deltaX/normDelta*37;
		posY=posY-hitPositionY+deltaY/normDelta*37;

		deltaX= float(posX-player->posX);
		deltaY= float(posY-player->posY+hitPositionY);
		phi=acos(deltaX/sqrt(deltaX*deltaX+deltaY*deltaY));
		x=velX+player->velX; y=velY-player->velY;
		r1=cos(phi)*x-sin(phi)*y;
		t1=sin(phi)*x+cos(phi)*y;
		r2=e_BOGO*(-1)*r1; t2=e_BOGO*t1;
		x=r2*cos(phi)+t2*sin(phi);
		y=t2*cos(phi)-r2*sin(phi);

		velX=(float)(x); velY=(float)(y);
		playerHit=0;




	}




	}

	if (sqrt(float(velX*velX)+float(velY*velY))>VELBALLMAX)
		{
		  velX= float (velX/sqrt((float(velX*velX)+float(velY*velY)))*VELBALLMAX);
		  velY= float (velY/sqrt((float(velX*velX)+float(velY*velY)))*VELBALLMAX);
	}


}

int cBall::Draw(){
	//blit the ball
	float destx=posX-34.0f;
	float desty=posY-34.0f+ballSkin->h;
	glLoadIdentity();
	glRasterPos3f( destx,desty*(-1.0f),0.0f );
	SDL_Surface * dummy=ballSkin;
	glDrawPixels(dummy->w,dummy->h,GL_RGBA,GL_UNSIGNED_BYTE,dummy->pixels);
	//ball complete	
	return(0);
}

