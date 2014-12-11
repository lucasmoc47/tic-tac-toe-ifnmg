#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <cstdio>

const int SCREEN_WIDTH = 311;
const int SCREEN_HEIGHT = 308;

const int BUTTON_WIDTH = 94;
const int BUTTON_HEIGHT = 94;
const int BAR_SIZE = 4;
const int COMP = 4;

#define GRID_SIZE 3

int pressedPosition;
int posX, posY; //Posições da matriz que representa o grid
int gridMat[GRID_SIZE][GRID_SIZE];
int player;
int winner;
int ocupados;

//Imagens
SDL_Surface *grid = NULL;
SDL_Surface *xImage = NULL;
SDL_Surface *oImage = NULL;
SDL_Surface *screen = NULL;
SDL_Surface *newGame = NULL;

//SFX
Mix_Chunk *sfxClickX = NULL;
Mix_Chunk *sfxClickO = NULL;
Mix_Chunk *sfxGameEnded = NULL;

//Janela
SDL_Window *window = NULL;

//Posições do grid
SDL_Rect gridRect[9];

//Eventos
SDL_Event event;


bool InitWindow(){

	if (SDL_Init(SDL_INIT_VIDEO < 0 | SDL_INIT_AUDIO) < 0){
		printf("Erro ao inicializar SDL! Erro SDL: %s\n", SDL_GetError());
		return 0;
	}
	else{

		if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0){
			printf("SDL_mixer não pode ser inicializado! Erro SDL: %s\n", Mix_GetError());
			return 0;
		}

		window = SDL_CreateWindow("TIC-TAC-TOE", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

		if (window == NULL){
			printf("Erro ao inicializar a janela\n");
			return 0;
		}
		else{
			screen = SDL_GetWindowSurface(window);
		}

	}

	int imgFlags = IMG_INIT_PNG;

	if(!(IMG_Init(imgFlags) & imgFlags)){
		printf("Erro. Não foi possível inicializar SDL_image!\n%s\n", IMG_GetError());
		return 0;
	}

	if ((grid = IMG_Load("../img/grid.png")) == NULL){
		printf("Erro ao carregar o background\n");
		return 0;
	}

	if ((xImage = IMG_Load("../img/x.png")) == NULL){
		printf("Erro %s\n", SDL_GetError());
		return 0;
	}

	if ((oImage = IMG_Load("../img/o.png")) == NULL){
		printf("Erro %s\n", SDL_GetError());
		return 0;
	}

	if ((newGame = IMG_Load("../img/new_game.png")) == NULL){
		printf("Erro %s\n", SDL_GetError());
		return 0;
	}

	if((sfxClickX = Mix_LoadWAV("../sfx/x_sound.wav")) == NULL){
		printf("Não foi possível carregar \"x_sound.wav\"! Erro SDL: %s\n", Mix_GetError());
		return 0;
	}

	if((sfxClickO = Mix_LoadWAV("../sfx/o_sound.wav")) == NULL){
		printf("Não foi possível carregar \"o_sound.wav\"! Erro SDL: %s\n", Mix_GetError());
		return 0;
	}

	if((sfxGameEnded = Mix_LoadWAV("../sfx/game_ended.wav")) == NULL){
		printf("Não foi possível carregar \"game_ended.wav\"! Erro SDL: %s\n", Mix_GetError());
		return 0;
	}
	return 1;

}

bool newGameScreen(){
	//TODO
	//O "new game" deve funcionar como um botão, e não fazer parte do background

	while(1){
		SDL_BlitSurface(newGame, NULL, screen, NULL);
		SDL_UpdateWindowSurface(window);

		SDL_PollEvent(&event);

		if(event.type == SDL_MOUSEBUTTONDOWN){
			int x, y;
			SDL_GetMouseState(&x, &y);

			if(((x >= 80) && (x <= 220)) && ((y >= 210) && (y <= 225))){
				return 0;
			}else if(((x >= 115) && (x <= 170)) && ((y >= 250) && (y <= 265))){
				return 1;
			}
		}

	}

}

void SetGridRect(){

	gridRect[0].x = gridRect[0].y = COMP;
	gridRect[0].w = BUTTON_WIDTH;
	gridRect[0].h = BUTTON_HEIGHT;

	gridRect[1].x = BUTTON_WIDTH + COMP*3 + BAR_SIZE;
	gridRect[1].y = COMP;
	gridRect[1].w = BUTTON_WIDTH;
	gridRect[1].h = BUTTON_HEIGHT;

	gridRect[2].x = gridRect[1].x * 2 - COMP;
	gridRect[2].y = COMP;
	gridRect[2].w = BUTTON_WIDTH;
	gridRect[2].h = BUTTON_HEIGHT;

	gridRect[3].x = COMP;
	gridRect[3].y = BUTTON_WIDTH + BAR_SIZE + COMP*3;
	gridRect[3].w = BUTTON_WIDTH;
	gridRect[3].h = BUTTON_HEIGHT;

	gridRect[4].x = gridRect[1].x;
	gridRect[4].y = BUTTON_WIDTH + BAR_SIZE + COMP * 3;
	gridRect[4].w = BUTTON_WIDTH;
	gridRect[4].h = BUTTON_HEIGHT;

	gridRect[5].x = gridRect[2].x;
	gridRect[5].y = BUTTON_WIDTH + BAR_SIZE + COMP * 3;
	gridRect[5].w = BUTTON_WIDTH;
	gridRect[5].h = BUTTON_HEIGHT;

	gridRect[6].x = COMP;
	gridRect[6].y = BUTTON_WIDTH * 2 + BAR_SIZE * 2 + COMP * 5;
	gridRect[6].w = BUTTON_WIDTH;
	gridRect[6].h = BUTTON_HEIGHT;

	gridRect[7].x = gridRect[1].x;
	gridRect[7].y = BUTTON_WIDTH * 2 + BAR_SIZE * 2 + COMP * 5;
	gridRect[7].w = BUTTON_WIDTH;
	gridRect[7].h = BUTTON_HEIGHT;

	gridRect[8].x = gridRect[2].x;
	gridRect[8].y = BUTTON_WIDTH * 2 + BAR_SIZE * 2 + COMP * 5;
	gridRect[8].w = BUTTON_WIDTH;
	gridRect[8].h = BUTTON_HEIGHT;
}

void GetPosition(int x, int y){

	if (x < gridRect[0].w && y < gridRect[0].h){
		pressedPosition = 1;
		posX = 0;
		posY = 0;
	}
	else if (x >= gridRect[1].x && y >= gridRect[1].y && x <= (gridRect[1].x + gridRect[1].w) && y <= (gridRect[1].y + gridRect[1].h)){
		pressedPosition = 2;
		posX = 0;
		posY = 1;
	}
	else if (x > gridRect[2].x && y > gridRect[2].y && x < (gridRect[2].x + gridRect[2].w) && y < (gridRect[2].y + gridRect[2].h)){
		pressedPosition = 3;
		posX = 0;
		posY = 2;
	}
	else if (x > gridRect[3].x && y > gridRect[3].y && x < (gridRect[3].x + gridRect[3].w) && y < (gridRect[3].y + gridRect[3].h)){
		pressedPosition = 4;
		posX = 1;
		posY = 0;
	}
	else if (x > gridRect[4].x && y > gridRect[4].y && x < (gridRect[4].x + gridRect[4].w) && y < (gridRect[4].y + gridRect[4].h)){
		pressedPosition = 5;
		posX = 1;
		posY = 1;
	}
	else if (x > gridRect[5].x && y > gridRect[5].y && x < (gridRect[5].x + gridRect[5].w) && y < (gridRect[5].y + gridRect[5].h)){
		pressedPosition = 6;
		posX = 1;
		posY = 2;
	}
	else if (x > gridRect[6].x && y > gridRect[6].y && x < (gridRect[6].x + gridRect[6].w) && y < (gridRect[6].y + gridRect[6].h)){
		pressedPosition = 7;
		posX = 2;
		posY = 0;
	}
	else if (x > gridRect[7].x && y > gridRect[7].y && x < (gridRect[7].x + gridRect[7].w) && y < (gridRect[7].y + gridRect[7].h)){
		pressedPosition = 8;
		posX = 2;
		posY = 1;
	}
	else if (x > gridRect[8].x && y > gridRect[8].y && x < (gridRect[8].x + gridRect[8].w) && y < (gridRect[8].y + gridRect[8].h)){
		pressedPosition = 9;
		posX = 2;
		posY = 2;
	}

	else printf("NADA\n");

}

bool GetEvents(int player){

	while(SDL_PollEvent(&event) != 0){

		if(event.type == SDL_QUIT){
			return 1;
		}
		if(event.type == SDL_MOUSEBUTTONDOWN){

			int x, y;

			SDL_GetMouseState(&x, &y);

			GetPosition(x, y);

			if(player == 1){
				Mix_PlayChannel(-1, sfxClickO, 0);
			}else{
				Mix_PlayChannel(-1, sfxClickX, 0);
			}

		}
		if(event.type == SDL_KEYDOWN){

			if(event.key.keysym.scancode == SDL_SCANCODE_ESCAPE){
				return 1;
			}

		}

	}

}

void NewRound(){

	for (int i = 0; i < GRID_SIZE; i++){
		for (int j = 0; j < GRID_SIZE; j++){
			gridMat[i][j] = -1;
		}
	}

	grid = IMG_Load("../img/grid.png");

	if(grid == NULL){
		printf("Erro ao carregar o grid. (New round)");
	}

	winner = 0;
	ocupados = 0;
	player = 1;

}

bool CheckPosition(){

	if (pressedPosition != 0){
		if (gridMat[posX][posY] == -1){
			gridMat[posX][posY] = player;
			player == 1 ? player = 2 : player = 1;
			return true;
		}
	}

	return false;
}

int ScanMatriz(){

	int horizontal = 0, vertical = 0;
	int diagonal_1 = 0, diagonal_2 = 0;

	bool changedH = false, changedV = false, changedD1 = false, changedD2 = false;

	//Verifica as linhas e diagonais
	for (int i = 0; i < GRID_SIZE; i++){
		for (int j = 0; j < GRID_SIZE; j++){
			if (horizontal != 0 && horizontal != gridMat[i][j]){
				changedH = true;
			}
			horizontal = gridMat[i][j];
			if (i == j){
				if (diagonal_1 != 0 && diagonal_1 != gridMat[i][j]){
					changedD1 = true;
				}
				diagonal_1 = gridMat[i][j];
			}
			if ((i + j) == 2){
				if (diagonal_2 != 0 && diagonal_2 != gridMat[i][j]){
					changedD2 = true;
				}
				diagonal_2 = gridMat[i][j];
			}
		}

		if (!changedH){
			return horizontal;
		}
		changedH = false;
		horizontal = 0;
	}

	if (!changedD1){
		return diagonal_1;
	}
	if (!changedD2){
		return diagonal_2;
	}

	//Verifica as colunas
	for (int i = 0; i < GRID_SIZE; i++){
		for (int j = 0; j < GRID_SIZE; j++){
			if (vertical != 0 && vertical != gridMat[j][i]){
				changedV = true;
			}
			vertical = gridMat[j][i];
		}

		if (!changedV){
			return vertical;
		}
		changedV = false;
		vertical = 0;
	}

	return 0;
}

void GameLoop(){
	bool quit = false;

	while (!quit){
		pressedPosition = 0;

		quit = GetEvents(player);
		if (CheckPosition()){
			SDL_BlitSurface(player == 1 ? xImage : oImage, NULL, grid, &gridRect[pressedPosition - 1]);
			ocupados++;
			if (ocupados > 4){
				winner = ScanMatriz();
				if (winner == 1 || winner == 2 || ocupados == 9){
					if (ocupados == 9){
						printf("Empate!\n");
					}else{
						printf("Player %d ganhou\n", winner);
						Mix_PlayChannel(-1, sfxGameEnded, 0);
					}
					SDL_BlitSurface(grid, NULL, screen, NULL);
					SDL_BlitSurface(player == 1 ? xImage : oImage, NULL, grid, &gridRect[pressedPosition - 1]);
					SDL_UpdateWindowSurface(window);
					SDL_Delay(2000);
					return;
				}
			}
		}

		SDL_BlitSurface(grid, NULL, screen, NULL);
		SDL_UpdateWindowSurface(window);
	}
}

void Close(){

	SDL_DestroyWindow(window);

	SDL_FreeSurface(grid);
	SDL_FreeSurface(xImage);
	SDL_FreeSurface(oImage);
	SDL_FreeSurface(newGame);

	Mix_FreeChunk(sfxClickX);
	Mix_FreeChunk(sfxClickO);
	Mix_FreeChunk(sfxGameEnded);

}

int main(int argc, char *argv[]){

	bool quit = false;

	if (!InitWindow()){
		printf("Erro total!\n");
		return 0;
	}

	while(!(quit = newGameScreen())){
		SetGridRect();
		NewRound();
		GameLoop();
	}

	return 0;
}
