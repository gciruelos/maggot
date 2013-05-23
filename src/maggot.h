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

#include <stdio.h>
#include <string.h>

#include <stdlib.h>

#include <time.h>

#include <termios.h>
#include <unistd.h>
#include <fcntl.h>


#define	MAPH	21

#define RIGHT	100
#define LEFT	97
#define UP		119
#define DOWN	115

#define FOOD '@'
#define WALL '$'

#define VERSION "0.0.3"

void init_snake();
void init_map();
int printmap();
int istheresnake(int y, int x);
int rand_lim(int limit);
int getch();
int kbhit(void);
void move();
void turn(char input);
int lenofsnake();
void changemode(int);
void newfood(void);
void grow(void);
void info(void);
void end(int what);
void paused(void);
double sqrt(float n);
