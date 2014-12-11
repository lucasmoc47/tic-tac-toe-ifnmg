#include "ttt.hpp"

//Get move
//Put it on table
//Check if won
//Next player

char Grid[GRID_SIZE][GRID_SIZE];

void InitGrid(){
	for(int i = 0; i < GRID_SIZE; i++){
		for(int j = 0; j < GRID_SIZE; j++){
			Grid[i][j] = '\0';
		}
	}
}

void MarkGrid(char playerChar, int y, int x){
	Grid[y][x] = playerChar;
}

bool playerWon(char playerChar){
	bool won;

	//Linhas
	for(int i = 0; i < GRID_SIZE; i++){
		for(int j = 0, won = true; j < GRID_SIZE; j++){
			if(Grid[i][j] != playerChar){
				won = false;
			}
		}

		if(won) return true;
	}

	//Colunas
	for(int i = 0; i < GRID_SIZE; i++){
		for(int j = 0, won = true; j < GRID_SIZE; j++){
			if(Grid[j][i] != playerChar){
				won = false;
			}
		}
		if(won) return true;
	}

	//Diagonal principal
	for(int i = 0, won = true; i < GRID_SIZE; i++){
		if(Grid[i][i] != playerChar){
			won = false;
		}
	}

	if(won) return true;

	//Diagonal secundária
	for(int i = GRID_SIZE - 1, won = true; i >= 0; i--){
		if(Grid[i][i] != playerChar){
			won = false;
		}
	}

	if(won) return true;
	if(!won) return false;
}
