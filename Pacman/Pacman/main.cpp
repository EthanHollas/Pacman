#include "Pacman.h"
#include <iostream>

//Entry Point for Application
int main(int argc, char* argv[]) {
	int ghostCount;
	int munchieCount;
	string difficulty;
	std::cout << "hard, normal or easy?" << std::endl;
	std::cin >> difficulty;
	if (difficulty == "easy")
	{
		munchieCount = 20;
		ghostCount = 5;
	}

	else if (difficulty == "normal")
	{
		munchieCount = 35;
		ghostCount = 7;

	}

	else if (difficulty == "hard")
	{
		munchieCount = 50;
		ghostCount = 10;

	}




	
	
	Pacman* game = new Pacman(argc, argv, munchieCount, ghostCount);

}