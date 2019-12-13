#pragma once

// If Windows and not in Debug, this will run without a console window
// You can use this to output information when debugging using cout or cerr
#ifdef WIN32 
	#ifndef _DEBUG
		#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
	#endif
#endif

#define OBSTACLECOUNT 4
#define CHERRYCOUNT 4
#define GHOSTCOUNT 10

// Just need to include main header file
#include "S2D/S2D.h"

// Reduces the amount of typing by including all classes in S2D namespace
using namespace S2D;

// Declares the Pacman class which inherits from the Game class.
// This allows us to overload the Game class methods to help us
// load content, draw and update our game.

// Structure Definition
struct MovingEnemy
{
	Vector2* position;
	Texture2D* texture;
	Rect* sourceRect;
	int direction;
	float speed;
};



struct Player
{
	bool dead;
	Vector2* _pacmanPosition;
	Rect* _pacmanSourceRect;
	Texture2D* _pacmanTexture;
	int _pacmanDirection;
	int _pacmanFrame;
	int _pacmanCurrentFrameTime;
	float speedMultiplier;
};

struct Enemy
{
	bool dead;
	Rect* _munchieRect;
	Texture2D* _munchieTexture;
	Vector2* _munchiePosition;
	int _munchieFrame;
	int _munchieCurrentFrameTime;
	int _munchieFrameTime;
};


struct Menu
{
	Texture2D* _menuBackground;
	Texture2D* _menuForeground;
	Rect* _menuRectangle;
	Vector2* _menuStringPosition;
	bool _paused;
	bool _start;
	bool _death;
	bool _instructions;
	bool _win;

	bool _pKeyDown;
	bool _iKeyDown;
	bool _spaceDown;
};

class Pacman : public Game
{


private:
	// Data to represent Pacman
	Player* _pacman;

	//Constant data for Game Variables 
	const float _cPacmanSpeed;
	const int _cPacmanFrameTime;
	const int _cMunchieFrameTime;

	// Data for Menu
	Menu* _menu;

	// Data to represent Munchie
	Enemy* _munchies;
	Enemy* _cherries;
	int munchieCounts;
	int ghostCounts;
	int munchieGotten;
	float timepassed;
	MovingEnemy* _ghosts[GHOSTCOUNT];

	// Data to represent Obstacles
	Enemy* _obstacles;

	// Position for String
	Vector2* _stringPosition;

	//Represent Sound effects
	SoundEffect* _pop;
	SoundEffect* _warp;
	SoundEffect* _death;
	SoundEffect* _Odeath;

	SoundEffect* _twinkle;
	SoundEffect* _winning;



private:

	void Input(int elapsedTime, Input::KeyboardState* state, Input::MouseState* mouseState);

	void CheckPaused(Input::KeyboardState* state, Input::Keys pauseKey);
	
	void CheckInstructions(Input::KeyboardState* state, Input::Keys pauseKey);

	void CheckDead();

	void CheckStart(Input::KeyboardState* state, Input::Keys startKey);

	void CheckViewportCollision();

	void UpdatePacman(int elapsedTime);

	void UpdateMunchie(Enemy& munchie, int elapsedTime);

	void CheckGhostCollisions();

	void Pacman::CheckMunchieCollisions(Enemy& munchies);

	void Pacman::CheckCherryCollisions(Enemy& munchies);

	void Pacman::CheckObstacleCollisions();


	void UpdateGhost(MovingEnemy*, int elapsedTime);

public:
	/// <summary> Constructs the Pacman class. </summary>
	Pacman(int argc, char* argv[], int munchieCount, int ghostCount);

	/// <summary> Destroys any data associated with Pacman class. </summary>
	virtual ~Pacman();

	/// <summary> All content should be loaded in this method. </summary>
	void virtual LoadContent();

	/// <summary> Called every frame - update game logic here. </summary>
	void virtual Update(int elapsedTime);

	/// <summary> Called every frame - draw game here. </summary>
	void virtual Draw(int elapsedTime);
};