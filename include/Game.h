#ifndef GAME
#define GAME
#include "Player.h"

#include "Ball.h"
#include "SDL/SDL.h"
#include "constants.h"
#include "gui.h"

#ifndef BOGOFUNCTION
typedef int (*bogoFunction)();
#endif

class cGame {

public :
	int exit;
	bogoFunction nextFunction;
	int swt2;
	int finished;
	int timeScaleSwitch;
	float timeScale;
	int finDelay;
	Uint32 ticks1;
	Uint32 ticks2;
	cPlayer * player1;
	cPlayer * player2;
	cBall * theBall;
	GLuint TexName;
	//gui Elements used to display the gamescores
	cGui * tGui;
	cTextLabel * LabelPoints;
	cTextLabel * LabelPlays;
	cTextLabel * LabelPointsRight;
	cTextLabel * LabelPlaysRight;
	cTextLabel * LabelPointsLeft;
	cTextLabel * LabelPlaysLeft;
	cTextLabel * Player1Name;
	cTextLabel * Player2Name;
	//the picture of the net
	SDL_Surface * net;
	
	int InitScoreGui();
	int PrintScoreGui();
	int DrawGameBackground();
	int DrawNet();
	int DeleteScoreGui();
	void CheckScore();
	void RestartGame(int change = 0);
	void CalcTimescale();
	void ProcessDelay();
	void ProcessStart(Uint8 * keystate);
	void ProcessQuitPressed(Uint8 * keystate);
	void ProcessResSwitch(Uint8 * keystate);
	
	//void WriteStats(SDL_Surface * screen, TTF_Font * font,SDL_Color fg, SDL_Color bg);
	cGame(void);
	~cGame(void);

};

#endif 


