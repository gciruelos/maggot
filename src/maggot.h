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

#ifndef MAGGOT_H
#define MAGGOT_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

#define MAP_HEIGHT  21
#define KEY_ESC     27
#define FOOD_SYMBOL '@'
#define WALL_SYMBOL '$'
#define VERSION "0.0.3"

void init_snake ();
void init_map ();
int printmap ();
int istheresnake (int y, int x);
int rand_lim (int limit);
int getch ();
int kbhit (void);
void move (unsigned int map_height);
void turn (char input, int map_height);
int lenofsnake ();
void changemode (int);
void newfood (unsigned int map_height);
void grow (void);
void info (void);
void end (bool dead);
void paused (void);
double my_sqrt (float n);
void wait_for_key (char k, bool any);

#endif // MAGGOT_H
