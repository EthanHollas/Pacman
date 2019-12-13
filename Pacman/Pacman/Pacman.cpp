#include "Pacman.h"
#include <time.h>
#include <sstream>
//debug
#include <iostream>
#include <cstdlib>
#include <chrono>

#include <ctime>


Pacman::Pacman(int argc, char* argv[], int munchieCount, int ghostCount) : Game(argc, argv), _cPacmanSpeed(0.1f), _cPacmanFrameTime(250), _cMunchieFrameTime(500)
{
	munchieCounts = munchieCount;
	ghostCounts = ghostCount;

	munchieGotten = 0;
	timepassed = 0.0f;
	// Initialise member variables 
	_pacman = new Player();
	_pacman->dead = false;
	
	srand(time(NULL));
	_pop = new SoundEffect();
	_warp = new SoundEffect();
	_death = new SoundEffect();
	_Odeath = new SoundEffect();
	_twinkle = new SoundEffect();
	_winning = new SoundEffect();


	// Local Variables 
	int i;
	// Initialise Munchies 
	_munchies = new Enemy[munchieCount];
	for (i = 0; i < munchieCount; i++)
	{
		_munchies[i].dead = false;
		_munchies[i]._munchieCurrentFrameTime = 0;
		_munchies[i]._munchieFrame = rand() % 1;
		_munchies[i]._munchieFrameTime = rand() % 500 + 5;

	}
	_obstacles = new Enemy[OBSTACLECOUNT];

	_cherries = new Enemy[CHERRYCOUNT];
	//initialise ghost character
	for (i = 0; i < ghostCount; i++)
	{
		float randomNum = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 0.12f));
		_ghosts[i] = new MovingEnemy();
		_ghosts[i]->direction = 0;
		_ghosts[i]->speed = randomNum;	
	}

	for (i = 0; i < CHERRYCOUNT; i++)
	{
		_cherries[i].dead = false;
		_cherries[i]._munchieCurrentFrameTime = 0;
		_cherries[i]._munchieFrame = rand() % 1;
		_cherries[i]._munchieFrameTime = rand() % 500 + 5;

	}

	_menu = new Menu();
	_menu->_paused = false;
	_menu->_start = true;
	_menu->_death = false;
	_menu->_instructions = false;
	_menu->_win = false;

	_pacman->_pacmanCurrentFrameTime = 0;
	_pacman->_pacmanFrame = 0;
	_pacman->speedMultiplier = 1.0f;

	//Initialise important Game aspects
	Audio::Initialise();
	Graphics::Initialise(argc, argv, this, 1024, 768, false, 25, 25, "Pacman", 60);
	Input::Initialise();

	// Start the Game Loop - This calls Update and Draw in game loop
	Graphics::StartGameLoop();
	
}

Pacman::~Pacman()
{

	int i;
	// Clean up the texture 
	delete _munchies[0]._munchieTexture;
	// Initialise Munchies 
	for (i = 0; i < munchieCounts; i++)
	{
		delete _munchies[i]._munchieRect;
	}
	// Order of operations is important, array deleted last delete[] _munchies; 
	// Remaining clean up to be implemented by the developer…
	
	// Cleanup pointers within the pacman structure
	delete _pacman->_pacmanTexture;
	delete _pacman->_pacmanSourceRect;
	delete _pacman->_pacmanPosition;
	// Clean up the Pacman structure pointer
	delete _pacman;
	delete _munchies[0]._munchieTexture;
	int nCount = 0;
	for (nCount = 0; nCount < munchieCounts; nCount++)
	{
		delete _munchies[nCount]._munchiePosition;
		delete _munchies[nCount]._munchieRect;
	}
	for (nCount = 0; nCount < CHERRYCOUNT; nCount++)
	{
		delete _cherries[nCount]._munchiePosition;
		delete _cherries[nCount]._munchieRect;
	}

	//Array deleted last
	delete[] _munchies;
	delete[] _cherries;
	delete[] _obstacles;
	//Clean up Ghosts

	int gCount = 0;
	for (gCount = 0; gCount < ghostCounts; gCount++)
	{
		delete _ghosts[gCount]->texture;
		delete _ghosts[gCount]->position;
		delete _ghosts[gCount]->sourceRect;
	}
	delete[] _ghosts;

	delete _pop;
	delete _warp;
	delete _death;
	delete _Odeath;

	delete _twinkle;
	delete _winning;



	

}

void Pacman::LoadContent()
{
	// Load Munchie
	int i;
	
	Texture2D* munchieTex = new Texture2D();
	munchieTex->Load("Textures/munchies.png", true);
	
	//Sound
	_pop->Load("Sounds/pop.wav");
	_warp->Load("Sounds/warp.wav");
	_death->Load("Sounds/death.wav");
	_Odeath->Load("Sounds/Odeath.wav");
	_twinkle->Load("Sounds/twinkle.wav");
	_winning->Load("Sounds/win.wav");


	// Initialise Munchies 
	for (i = 0; i < munchieCounts; i++)
	{

		_munchies[i]._munchieTexture = munchieTex;
		_munchies[i]._munchieRect = new Rect(0.0f, 0.0f, 12, 12);
		_munchies[i]._munchiePosition = new Vector2(rand() % Graphics::GetViewportWidth(), rand() % Graphics::GetViewportHeight());
	}

	// Load Pacman
	_pacman->_pacmanDirection = 0;
	_pacman->_pacmanTexture = new Texture2D();
	_pacman->_pacmanTexture->Load("Textures/Pacman.tga", false);
	_pacman->_pacmanPosition = new Vector2(350.0f, 350.0f);
	_pacman->_pacmanSourceRect = new Rect(0.0f, 0.0f, 32, 32);

	// Load Obstacle
	for (i = 0; i < OBSTACLECOUNT; i++)
	{
		_obstacles[i]._munchieTexture = new Texture2D();
		_obstacles[i]._munchieTexture->Load("Textures/obstacle.png", false);
		_obstacles[i]._munchieRect = new Rect(0.0f, 0.0f, 166.0f, 83.0f);
	}
	_obstacles[0]._munchiePosition = new Vector2(150.0f, 150.0f);
	_obstacles[1]._munchiePosition = new Vector2(600.0f, 600.0f);
	_obstacles[2]._munchiePosition = new Vector2(600.0f, 150.0f);
	_obstacles[3]._munchiePosition = new Vector2(150.0f, 600.0f);

	// Load Cherries

	for (i = 0; i < CHERRYCOUNT; i++)
	{
		_cherries[i]._munchieTexture= new Texture2D();
		_cherries[i]._munchieTexture->Load("Textures/cherries.png", false);
		_cherries[i]._munchieRect = new Rect(0.0f, 0.0f, 12.0f, 12.0f);
	}
	_cherries[0]._munchiePosition = new Vector2(100.0f, 150.0f);
	_cherries[1]._munchiePosition = new Vector2(900.0f, 600.0f);
	_cherries[2]._munchiePosition = new Vector2(900.0f, 150.0f);
	_cherries[3]._munchiePosition = new Vector2(100.0f, 600.0f);

	// Set Menu Paramters
	_menu->_menuBackground = new Texture2D();
	_menu->_menuBackground->Load("Textures/Transparency.png", false);
	_menu->_menuRectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());
	_menu->_menuStringPosition = new Vector2(Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 2.0f);

	// Set string position
	_stringPosition = new Vector2(10.0f, 25.0f);

	//Initialise ghost Character
	
	for (i = 0; i < ghostCounts; i++)
	{
		_ghosts[i]->texture = new Texture2D();
		_ghosts[i]->texture->Load("Textures/GhostsBlue.png", false);
		_ghosts[i]->position = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
		_ghosts[i]->sourceRect = new Rect(0.0f, 0.0f, 25, 22);
	}
}

void Pacman::Update(int elapsedTime)
{
	// Gets the current state of the keyboard
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();
	Input::MouseState* mouseState = Input::Mouse::GetState();
	CheckStart(keyboardState, Input::Keys::SPACE);
	if (!_menu->_start)
	{
		CheckPaused(keyboardState, Input::Keys::P);
		CheckInstructions(keyboardState, Input::Keys::I);
		CheckInstructions(keyboardState, Input::Keys::I);


		if (!_menu->_paused && !_menu->_instructions && !_menu->_win && !_menu->_death) 

			{
			timepassed += (elapsedTime);
				Input(elapsedTime, keyboardState, mouseState);
				CheckViewportCollision();
				UpdatePacman(elapsedTime);
				CheckGhostCollisions();
				CheckObstacleCollisions();
				for (int i = 0; i < munchieCounts; i++)
				{
					UpdateMunchie(_munchies[i], elapsedTime);
					CheckMunchieCollisions(_munchies[i]);

				}
				for (int i = 0; i < CHERRYCOUNT; i++)
				{
					UpdateMunchie(_cherries[i], elapsedTime);
					CheckCherryCollisions(_cherries[i]);

				}
				for (int i = 0; i < ghostCounts; i++)
				{
					UpdateGhost(_ghosts[i], elapsedTime);
				}

				if (munchieCounts == 20)
				{
					if (munchieGotten >= 30)
					{
						if (_menu->_win == false)
						{
							Audio::Play(_winning);
							_menu->_win = true;
						}
					}
				}

				else if (munchieCounts == 35)
				{
					if (munchieGotten >= 43)
					{
						if (_menu->_win == false)
						{
							Audio::Play(_winning);
							_menu->_win = true;
						}
					}
				}

				else if (munchieCounts == 50)
				{
					if (munchieGotten >= 60)
					{
						if (_menu->_win == false)
						{
							Audio::Play(_winning);
							_menu->_win = true;
						}
					}
				}
			}
		}
	}
	





void Pacman::Draw(int elapsedTime)
{
		// Local Variables 
		int i;
	
		// Allows us to easily create a string
		std::stringstream stream;
		stream << "Pacman X: " << _pacman->_pacmanPosition->X << " Y: " << _pacman->_pacmanPosition->Y << endl;
		stream << "Score = " << munchieGotten << "            Time Passed : " << timepassed/1000;

		SpriteBatch::BeginDraw(); // Starts Drawing

		//if (!_pacman->dead)
		//{
			SpriteBatch::Draw(_pacman->_pacmanTexture, _pacman->_pacmanPosition, _pacman->_pacmanSourceRect);
		//}
		// Draw ghosts
		for (int i = 0; i < ghostCounts; i++)
		{
			SpriteBatch::Draw(_ghosts[i]->texture, _ghosts[i]->position, _ghosts[i]->sourceRect);
		}
		// Draws Munchie
		
		for (i = 0; i < munchieCounts; i++)
		{
			if (!_munchies[i].dead)
			{
				SpriteBatch::Draw(_munchies[i]._munchieTexture, _munchies[i]._munchiePosition, _munchies[i]._munchieRect);
			}
	
		}

		// Draws Obstacles
		for (int i = 0; i < OBSTACLECOUNT; i++)
		{
			SpriteBatch::Draw(_obstacles[i]._munchieTexture, _obstacles[i]._munchiePosition, _obstacles[i]._munchieRect);
		}

		
		for (int i = 0; i < CHERRYCOUNT; i++)
		{
			if (!_cherries[i].dead)
			{
				SpriteBatch::Draw(_cherries[i]._munchieTexture, _cherries[i]._munchiePosition, _cherries[i]._munchieRect);
			}
		}

		// Draws String
		SpriteBatch::DrawString(stream.str().c_str(), _stringPosition, Color::Green);
		if (_menu->_start)
		{
			std::stringstream menuStream;
			menuStream << "PRESS SPACE TO START!" << endl << "AVOID THE GHOSTS AND OBSTACLES" << endl << "COLLECT AS MANY MUNCHIES AS YOU CAN" << endl << "PRESS LEFT SHIFT FOR A BOOST!";
			SpriteBatch::Draw(_menu->_menuBackground, _menu->_menuRectangle, nullptr);
			SpriteBatch::DrawString(menuStream.str().c_str(), _menu->_menuStringPosition, Color::Green);
		}
		if (_menu->_paused)
		{
			std::stringstream menuStream;
			menuStream << "PAUSED! - YOU HAVE : " << munchieGotten << "POINTS";
			SpriteBatch::Draw(_menu->_menuBackground, _menu->_menuRectangle, nullptr);
			SpriteBatch::DrawString(menuStream.str().c_str(), _menu->_menuStringPosition, Color::Magenta);
		}
	
		if (_menu->_death)
		{
			std::stringstream menuStream;
			menuStream << "GAME OVER!"<< endl << "YOU SCORED : " << munchieGotten << " POINTS" << endl << "YOU DIED IN " << timepassed/1000 << " SECONDS"; 
			SpriteBatch::Draw(_menu->_menuBackground, _menu->_menuRectangle, nullptr);
			SpriteBatch::DrawString(menuStream.str().c_str(), _menu->_menuStringPosition, Color::Red);
		}

		if (_menu->_instructions)
		{
			std::stringstream menuStream;
			menuStream << "USE W,A,S,D To move!" << endl << " PRESS LEFT SHIFT FOR A SPEED BOOST " << endl << "AVOID THE GHOSTS AND OBSTACLES";
			SpriteBatch::Draw(_menu->_menuBackground, _menu->_menuRectangle, nullptr);
			SpriteBatch::DrawString(menuStream.str().c_str(), _menu->_menuStringPosition, Color::Magenta);
		}

		if (_menu->_win)
		{
			std::stringstream menuStream;
			menuStream << "CONGRATULATIONS!" << endl << "YOU WON AND SCORED : " << munchieGotten << " POINTS" << endl << "IT ONLY TOOK YOU " << timepassed/1000 << " SECONDS";
			SpriteBatch::Draw(_menu->_menuBackground, _menu->_menuRectangle, nullptr);
			SpriteBatch::DrawString(menuStream.str().c_str(), _menu->_menuStringPosition, Color::Red);
		}
		

		SpriteBatch::EndDraw(); // Ends Drawing
	
}

void Pacman::Input(int elapsedTime, Input::KeyboardState* state, Input::MouseState* mouseState) 
{
	float pacmanSpeed = _cPacmanSpeed * elapsedTime * _pacman->speedMultiplier;
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();
	mouseState->LeftButton;

	// Handle Mouse Input - Reposition Cherry
	if (mouseState->LeftButton == Input::ButtonState::PRESSED)
	{
		_munchies[0]._munchiePosition->X = mouseState->X;
		_munchies[0]._munchiePosition->Y = mouseState->Y;

	}
	// Speed Multiplier
	if (keyboardState->IsKeyDown(Input::Keys::LEFTSHIFT))
	{
		// Apply multiplier
		_pacman->speedMultiplier = 2.0f;
	}
	else
	{
		//Reset multiplier
		_pacman->speedMultiplier = 1.0f;
	}
	if (keyboardState->IsKeyDown(Input::Keys::D)) {
		_pacman->_pacmanPosition->X += pacmanSpeed; //Moves Pacman across X axis
		_pacman->_pacmanDirection = 0;
	}
	else if (keyboardState->IsKeyDown(Input::Keys::A)) {
		_pacman->_pacmanPosition->X -= pacmanSpeed; //Moves Pacman across X axis
		_pacman->_pacmanDirection = 2;
	}
	else if (keyboardState->IsKeyDown(Input::Keys::W)) {
		_pacman->_pacmanPosition->Y -= pacmanSpeed; //Moves Pacman across Y axis
		_pacman->_pacmanDirection = 3;
	}

	else if (keyboardState->IsKeyDown(Input::Keys::S)) {
		_pacman->_pacmanPosition->Y += pacmanSpeed; //Moves Pacman across Y axis
		_pacman->_pacmanDirection = 1;
	}
}


void Pacman::CheckPaused(Input::KeyboardState* state, Input::Keys pauseKey)
{
	// Gets the current state of the keyboard
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();

	if (keyboardState->IsKeyDown(Input::Keys::P) && !_menu->_pKeyDown)
	{
		_menu->_paused = !_menu->_paused;
		_menu->_pKeyDown = true;
	}

	if (keyboardState->IsKeyUp(Input::Keys::P))
		_menu->_pKeyDown = false;

}

void Pacman::CheckInstructions(Input::KeyboardState* state, Input::Keys pauseKey)
{
	// Gets the current state of the keyboard
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();

	if (keyboardState->IsKeyDown(Input::Keys::I) && !_menu->_iKeyDown)
	{
		_menu->_instructions = !_menu->_instructions;
		_menu->_iKeyDown = true;
	}

	if (keyboardState->IsKeyUp(Input::Keys::I))
		_menu->_iKeyDown = false;

}

void Pacman::CheckStart(Input::KeyboardState* state, Input::Keys startKey)
{
	// Gets the current state of the keyboard
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();

	if (keyboardState->IsKeyDown(Input::Keys::SPACE) && !_menu->_spaceDown)
	{
		_menu->_start = !_menu->_start;
		_menu->_spaceDown = true;
	}
}
void Pacman::CheckViewportCollision()
{
	if (_pacman->_pacmanPosition->X + _pacman->_pacmanSourceRect->Width > Graphics::GetViewportWidth()) {//1024 is game width
				// Pacman hit right wall - reset his position
		Audio::Play(_warp);

		_pacman->_pacmanPosition->X = 33 - _pacman->_pacmanSourceRect->Width;
	}

	if (_pacman->_pacmanPosition->X + _pacman->_pacmanSourceRect->Width < 33) { //1024 is game width
		//left wall
		_pacman->_pacmanPosition->X = 960 + _pacman->_pacmanSourceRect->Width;
		Audio::Play(_warp);
	}

	if (_pacman->_pacmanPosition->Y + _pacman->_pacmanSourceRect->Width > Graphics::GetViewportHeight()) { //1024 is game width
		//bottom
		_pacman->_pacmanPosition->Y = 33 - _pacman->_pacmanSourceRect->Width;
		Audio::Play(_warp);

	}

	if (_pacman->_pacmanPosition->Y + _pacman->_pacmanSourceRect->Width < 33) {//1024 is game width
		//top
		_pacman->_pacmanPosition->Y = 700 + _pacman->_pacmanSourceRect->Width;
		Audio::Play(_warp);

	}
}
void Pacman::UpdatePacman(int elapsedTime)
{
	_pacman->_pacmanCurrentFrameTime += elapsedTime;

	if (_pacman->_pacmanCurrentFrameTime > _cPacmanFrameTime)
	{
		_pacman->_pacmanFrame++;
		if (_pacman->_pacmanFrame >= 2)
			_pacman->_pacmanFrame = 0;

		_pacman->_pacmanCurrentFrameTime = 0;
	}

	_pacman->_pacmanSourceRect->X = _pacman->_pacmanSourceRect->Width * _pacman->_pacmanFrame;
	_pacman->_pacmanSourceRect->Y = _pacman->_pacmanSourceRect->Height * _pacman->_pacmanDirection;
}
void Pacman::UpdateMunchie(Enemy& munchie, int elapsedTime)
{
 
	munchie._munchieCurrentFrameTime += elapsedTime;

	if (munchie._munchieCurrentFrameTime > _cMunchieFrameTime)
	{
		munchie._munchieFrame++;
		if (munchie._munchieFrame >= 2)
			munchie._munchieFrame = 0;
		munchie._munchieCurrentFrameTime = 0;
	}
	munchie._munchieRect->X = munchie._munchieRect->Width * munchie._munchieFrame;

}
void Pacman::UpdateGhost(MovingEnemy* ghost, int elapsedTime)
{
	//if (ghost->direction == 0) //Moves Right
	//{
	//	ghost->position->X += ghost->speed * elapsedTime;
	//}
	//else if (ghost->direction == 1) //Moves Left
	//{
	//	ghost->position->X -= ghost->speed * elapsedTime;
	//}
	//if (ghost->position->X + ghost->sourceRect->Width >= Graphics::GetViewportWidth()) //Moves Left
	//{
	//	ghost->direction = 1; //Change direction
	//}
	//else if (ghost->position->X <= 0) //Hits left edge
	//{
	//	ghost->direction = 0; //Change direction
	//}
	//ghost->sourceRect->Y = ghost->sourceRect->Height * ghost->direction;
	if (_pacman->_pacmanPosition->X > ghost->position->X)
	{
		ghost->position->X += ghost->speed * elapsedTime;
		ghost->direction = 0;
	}
	if (_pacman->_pacmanPosition->X < ghost->position->X)
	{
		ghost->position->X -= ghost->speed * elapsedTime;
		ghost->direction = 1;

	}
	if (_pacman->_pacmanPosition->Y < ghost->position->Y)
	{
		ghost->position->Y -= ghost->speed * elapsedTime;
		ghost->direction = 2;

	}
	if (_pacman->_pacmanPosition->Y > ghost->position->Y)
	{
		ghost->position->Y += ghost->speed * elapsedTime;
		ghost->direction = 3;
	}
	
	ghost->sourceRect->Y = ghost->sourceRect->Height * ghost->direction;
	ghost->sourceRect->X = ghost->sourceRect->Width * ghost->direction;

}
void Pacman::CheckGhostCollisions()
{
	// Local Variables
	int i = 0;
	int bottom1 = _pacman->_pacmanPosition->Y + _pacman->_pacmanSourceRect->Height;
	int bottom2 = 0;
	int left1 = _pacman->_pacmanPosition->X;
	int left2 = 0;
	int right1 = _pacman->_pacmanPosition->X + _pacman->_pacmanSourceRect->Width;
	int right2 = 0;
	int top1 = _pacman->_pacmanPosition->Y;
	int top2 = 0;
	for (i = 0; i < ghostCounts; i++)
	{
		//Populate variables with Ghost Data
		bottom2 = _ghosts[i]->position->Y + _ghosts[i]->sourceRect->Height;
		left2 = _ghosts[i]->position->X;
		right2 = _ghosts[i]->position->X + _ghosts[i]->sourceRect->Width;
		top2 = _ghosts[i]->position->Y;

		if ((bottom1 > top2) && (top1 < bottom2) && (right1 > left2) && (left1 < right2))
		{
			if (_pacman->dead == false)
			{
				_pacman->dead = true;
				Audio::Play(_death);
				i = ghostCounts;
				_menu->_death = true;
				if (_pacman->_pacmanDirection == 0)
				_pacman->_pacmanTexture->Load("Textures/deadright.png", false);
				
				else if (_pacman->_pacmanDirection == 1)
					_pacman->_pacmanTexture->Load("Textures/deaddown.png", false);

				else if (_pacman->_pacmanDirection == 2)
					_pacman->_pacmanTexture->Load("Textures/deadleft.png", false);

				else if (_pacman->_pacmanDirection == 3)
					_pacman->_pacmanTexture->Load("Textures/deadup.png", false);
			}

		}
	}
}


void Pacman::CheckMunchieCollisions(Enemy& munchies)
{
	// Local Variables
	int i = 0;
	int bottom1 = _pacman->_pacmanPosition->Y + _pacman->_pacmanSourceRect->Height;
	int bottom2 = 0;
	int left1 = _pacman->_pacmanPosition->X;
	int left2 = 0;
	int right1 = _pacman->_pacmanPosition->X + _pacman->_pacmanSourceRect->Width;
	int right2 = 0;
	int top1 = _pacman->_pacmanPosition->Y;
	int top2 = 0;
	
		//Populate variables with Ghost Data
		bottom2 = munchies._munchiePosition->Y + munchies._munchieRect->Height;
		left2 = munchies._munchiePosition->X;
		right2 = munchies._munchiePosition->X + munchies._munchieRect->Width;
		top2 = munchies._munchiePosition->Y;

		if ((bottom1 > top2) && (top1 < bottom2) && (right1 > left2) && (left1 < right2))
		{
			if (munchies.dead == false) {
				munchies.dead = true;
				Audio::Play(_pop);
				munchieGotten = munchieGotten + 1;
			}
	
		}
	}

void Pacman::CheckCherryCollisions(Enemy& munchies)
{
	// Local Variables
	int i = 0;
	int bottom1 = _pacman->_pacmanPosition->Y + _pacman->_pacmanSourceRect->Height;
	int bottom2 = 0;
	int left1 = _pacman->_pacmanPosition->X;
	int left2 = 0;
	int right1 = _pacman->_pacmanPosition->X + _pacman->_pacmanSourceRect->Width;
	int right2 = 0;
	int top1 = _pacman->_pacmanPosition->Y;
	int top2 = 0;

	//Populate variables with Ghost Data
	bottom2 = munchies._munchiePosition->Y + munchies._munchieRect->Height;
	left2 = munchies._munchiePosition->X;
	right2 = munchies._munchiePosition->X + munchies._munchieRect->Width;
	top2 = munchies._munchiePosition->Y;

	if ((bottom1 > top2) && (top1 < bottom2) && (right1 > left2) && (left1 < right2))
	{
		if (munchies.dead == false) {
			munchies.dead = true;
			Audio::Play(_twinkle);
			munchieGotten = munchieGotten + 5;
			for (int i = 0; i < ghostCounts; i++)
			{
				_ghosts[i] -> speed = _ghosts[i]->speed / 1.5;
			}
		}

	}
}


void Pacman::CheckObstacleCollisions()
{
	// Local Variables
	int i = 0;
	int bottom1 = _pacman->_pacmanPosition->Y + _pacman->_pacmanSourceRect->Height;
	int bottom2 = 0;
	int left1 = _pacman->_pacmanPosition->X;
	int left2 = 0;
	int right1 = _pacman->_pacmanPosition->X + _pacman->_pacmanSourceRect->Width;
	int right2 = 0;
	int top1 = _pacman->_pacmanPosition->Y;
	int top2 = 0;
	for (i = 0; i < OBSTACLECOUNT; i++)
	{
		//Populate variables with Ghost Data
		bottom2 = _obstacles[i]._munchiePosition->Y + _obstacles[i]._munchieRect->Height;
		left2 = _obstacles[i]._munchiePosition->X;
		right2 = _obstacles[i]._munchiePosition->X + _obstacles[i]._munchieRect->Width;
		top2 = _obstacles[i]._munchiePosition->Y;

		if ((bottom1 > top2) && (top1 < bottom2) && (right1 > left2) && (left1 < right2))
		{
			if (_pacman->dead == false)
			{
				_pacman->dead = true;
				Audio::Play(_Odeath);
				i = ghostCounts;
				_menu->_death = true;
				if (_pacman->_pacmanDirection == 0)
					_pacman->_pacmanTexture->Load("Textures/deadright2.png", false);

				else if (_pacman->_pacmanDirection == 1)
					_pacman->_pacmanTexture->Load("Textures/deaddown2.png", false);

				else if (_pacman->_pacmanDirection == 2)
					_pacman->_pacmanTexture->Load("Textures/deadleft2.png", false);

				else if (_pacman->_pacmanDirection == 3)
					_pacman->_pacmanTexture->Load("Textures/deadup2.png", false);
			}

		}
	}
}

	

