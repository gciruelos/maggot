/*
 * maggot - Simple snake game written in C.
 * 
 * Copyright (C) 2013  Gonzalo Ciruelos
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
 
#include "maggot.h"

int map[MAPH+2][MAPH+2];

int snake[400][2];
int snake_direction = 0; /*1 UP, 2 RIGHT, 3 DOWN, 4 LEFT*/

float velocity = 2.73; /*sqrt(5)+0.5*/
int points = 0;

int foodx = 1, foody = 1;



int main (){
	srand((unsigned int) time(0));	
	
	init_snake();
	init_map();
	
	struct timespec delay;
	delay.tv_sec = 0; delay.tv_nsec = 500000000L/velocity; 

	int isupdated = 0;
	
	while (1){
		while(!kbhit()){
			if (!isupdated){
				move();
			}
			system("clear");
			printmap();
			nanosleep(&delay, NULL);
			isupdated = 0;
			velocity = sqrt(lenofsnake())+0.5;
		}
		turn(getch());
		fflush(stdin);
		isupdated = 1;
		continue;
	}
	
	
	return 0;
}


void init_snake(){
	snake[0][0] = rand_lim(10)+5;snake[0][1] = rand_lim(10)+5;
	
	if (snake[0][0]>=10 && snake[0][1]>=10){
		snake[1][0] = snake[0][0]-1; snake[1][1] = snake[0][1];
		snake[2][0] = snake[0][0]-2; snake[2][1] = snake[0][1];
		snake[3][0] = snake[0][0]-3; snake[3][1] = snake[0][1];
		snake[4][0] = snake[0][0]-4; snake[4][1] = snake[0][1];
		snake_direction = 2;
	}
	else if (snake[0][0]>=10 && snake[0][1]<=10){
		snake[1][0] = snake[0][0]; snake[1][1] = snake[0][1]+1;
		snake[2][0] = snake[0][0]; snake[2][1] = snake[0][1]+2;
		snake[3][0] = snake[0][0]; snake[3][1] = snake[0][1]+3;
		snake[4][0] = snake[0][0]; snake[4][1] = snake[0][1]+4;
		snake_direction = 1;
	}
	else if (snake[0][0]<=10 && snake[0][1]>=10){
		snake[1][0] = snake[0][0]; snake[1][1] = snake[0][1]-1;
		snake[2][0] = snake[0][0]; snake[2][1] = snake[0][1]-2;
		snake[3][0] = snake[0][0]; snake[3][1] = snake[0][1]-3;
		snake[4][0] = snake[0][0]; snake[4][1] = snake[0][1]-4;
		snake_direction = 3;
	}
	else if (snake[0][0]<=10 && snake[0][1]<=10){
		snake[1][0] = snake[0][0]+1; snake[1][1] = snake[0][1];
		snake[2][0] = snake[0][0]+2; snake[2][1] = snake[0][1];
		snake[3][0] = snake[0][0]+3; snake[3][1] = snake[0][1];
		snake[4][0] = snake[0][0]+4; snake[4][1] = snake[0][1];
		snake_direction = 4;
	}
		
}


void init_map(){
	int h, l;	

	for (h = 0; h<=MAPH; h++){
		l = 0;
		map[h][l] = WALL;
		l = MAPH;
		map[h][l] = WALL;
		l = MAPH+1;
		map[h][l] = '\n';
	}
	for (l = 0; l<=MAPH; l++){
		h = 0;
		map[h][l] = WALL;
		h = MAPH;
		map[h][l] = WALL;
	}
	map[MAPH][MAPH+1] = 'E';
	for (h = 0; h<=MAPH; h++){
		for (l = 0; l<=MAPH; l++){
			if (map[h][l] != WALL && map[h][l] != '\n' && map[h][l] != 'E'){
				map[h][l] = ' ';
			}
		}
	}
	
	newfood();
	
}


int printmap(){
	
	int y = 0, x;
		
	printf("\n ");
	for (y = 0;;y++){
		for (x = 0;; x++){
			if (map[y][x] == 'E'){
				points = lenofsnake()*lenofsnake()/4-6;
				printf("\n\t\t\033[1;37mPoints: %d\x1b[0m\t\n", points);
				return 0;
			}
			else if (istheresnake(y, x)){
				printf("\033[1;32mo \x1b[0m");
			}
			else{
				if (map[y][x] == WALL){
					printf("\033[47;37m%c \x1b[0m", map[y][x]);
				}
				else if (map[y][x] == FOOD){
					printf("\033[1;31m%c \x1b[0m", map[y][x]);
				}
				else{
					printf("%c ", map[y][x]);
					if (map[y][x] == '\n'){
						break;
					}
				}
			}
		}
	}
}


int istheresnake(int y, int x){
	int iterator;
	int spotx, spoty;
	
	if (!y && !x){
		return 0;
	}
	
	for (iterator = 0; (spotx=snake[iterator][0]) && (spoty=snake[iterator][1]); iterator++){
		if (spotx==x && spoty==y){
			return 1;
		}
	}
	
	return 0;
}


int getch(){
	/*http://forums.debian.net/viewtopic.php?f=8&t=59524*/
	int ch;
	struct termios origterm, tmpterm;

	tcgetattr(STDIN_FILENO, &origterm);
	tmpterm = origterm;
	tmpterm.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &tmpterm);
	ch = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &origterm);

	return ch;
}


int kbhit(void){
	/*http://cboard.cprogramming.com/c-programming/63166-kbhit-linux.html*/

	struct termios oldt, newt;
	int ch;
	int oldf;

	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

	ch = getchar();
 
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, oldf);
 
	if(ch != EOF){
		ungetc(ch, stdin);
		return 1;
	}
 
	return 0;
}


void grow(void){
	int index;
	
	for (index = 1; snake[index][0]; index++){
		;
	}
	
	if (snake_direction==1){
		snake[index][0]=snake[index-1][0]; snake[index][0] = snake[index-1][0]+1;
	}
	else if (snake_direction==2){
		snake[index][0]=snake[index-1][0]-1; snake[index][0] = snake[index-1][0];
	}
	else if (snake_direction==3){
		snake[index][0]=snake[index-1][0]; snake[index][0] = snake[index-1][0]-1;
	}
	else if (snake_direction==4){
		snake[index][0]=snake[index-1][0]+1; snake[index][0] = snake[index-1][0];
	}
}


void newfood(void){
	int tocontinue;
	
	map[foody][foodx] = ' ';
	
	while (1){
		tocontinue = 0;
		foodx = rand_lim(MAPH-2)+1;
		foody = rand_lim(MAPH-2)+1;
		
		if (map[foody][foodx]==WALL){
			continue;
		}
		
		int i;
		for (i = 0; snake[i][0]; i++){
			if (foodx==snake[i][0] && foody==snake[i][1]){
				tocontinue = 1;
			}
		}
		
		if (tocontinue){
			continue;
		}
		else{
			break;
		}
	}
	
	map[foody][foodx] = FOOD;
}


void turn(char input){
	
	if (input == 'q'){
		end(0);
	}
	else if (input == 'p'){
		paused();
	}
	else if (input == 'i'){
		info();
	}
	
	
	else if (input == 27 || input == 91){
		return NULL;
	}
	
	if (input == UP || input==65){
		if (snake_direction==1 || snake_direction==3){
			;
		}
		else{
			snake_direction = 1;
		}
	}
	else if (input==RIGHT || input==67){
		if (snake_direction==2 || snake_direction==4){
			;
		}
		else{
			snake_direction = 2;
		}
	}
	else if (input==DOWN || input==66){
		if (snake_direction==1 || snake_direction==3){
			;
		}
		else{
			snake_direction = 3;
		}
	}
	else if (input==LEFT || input==68){
		if (snake_direction==2 || snake_direction==4){
			;
		}
		else{
			snake_direction = 4;
		}
	}
	else{
		return NULL;
	}
	
	move();
}


int lenofsnake(){
	int iter, len;
	
	for (iter = 0, len = 0; snake[iter][0]; iter++, len++){
		 ;
	}
	 
	 return len;
}


void move(){
	int i, direction[2];
	
	if (snake_direction==1){
		direction[0] = 0; direction[1] = -1;
	}
	else if (snake_direction==2){
		direction[0] = 1; direction[1] = 0;
	}
	else if (snake_direction==3){
		direction[0] = 0; direction[1] = 1;
	}
	else if (snake_direction==4){
		direction[0] = -1; direction[1] = 0;
	}
	
	
	for (i = lenofsnake()-1; i>0; i--){
		snake[i][0] = snake[i-1][0];snake[i][1] = snake[i-1][1];
	}
	
	snake[lenofsnake()][0] = 0; snake[lenofsnake()][1] = 0;
	
	snake[0][0] = snake[0][0]+direction[0];
	snake[0][1] = snake[0][1]+direction[1];
	
	if (map[snake[0][1]][snake[0][0]] == FOOD){
		grow();
		newfood();
	}
	else if (map[snake[0][1]][snake[0][0]] != ' '){
		end(1);
	}
	for (i = 1; snake[i][0]; i++){
		if (snake[0][0]==snake[i][0] && snake[0][1]==snake[i][1]){
			end(1);
		}
	}
}


void paused(void){
	//system("clear");
	
	printf("\r\nGAME PAUSED (Press 'p' to continue)\n");
	
	while (1){
		if (getch()=='p'){
			break;
		}
	}
}


void info(void){
	system("clear");
	
	printf("\n");
	printf("maggot - Simple snake game written in C.\n");
	printf("Version: %s\n", VERSION);
	printf("Author: Gonzalo Ciruelos <comp.gonzalo@gmail.com>\n");
	printf("License: GPL v2\n");
	
	
	while (1){
		if (getch()){
			break;
		}
	}
}	


void end(int what){
	/* what = 1 --> dead
	 * what = 0 --> quit
	 */
	 
	system("clear");
	
	if (what){
		printf("\nYOU LOST\n");
	}
	else{
		printf("\nTHANKS FOR PLAYING\n");
	}
	
	printf("\nPOINTS: %d\n\n\n", points);

	exit(0);
	 
}


double sqrt(float n){
	/* Babylonian method*/
	float iter, x = 1;
	while (iter<100){
		x = x/2 + n/(2*x);
		iter++;
	}
	return x;
}


int rand_lim(int limit) {
	/* http://stackoverflow.com/questions/2999075/generate-a-random-number-within-range/2999130#2999130
	 * Somewhat modified.*/
	int r;
	int divisor = RAND_MAX/(limit+1), retval;

	do{
		r = rand();
		retval = r / divisor;
	} while (retval > limit);

	return retval;
}

