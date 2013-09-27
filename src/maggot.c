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

// The board/map
int map[MAP_HEIGHT + 2][MAP_HEIGHT + 2];

// The coordinates of the snake
int snake[400][2];

enum snake_direction_enum
{
  SNAKE_NONE = 0,
  SNAKE_UP = 1,
  SNAKE_RIGHT = 2,
  SNAKE_DOWN = 3,
  SNAKE_LEFT = 4
};

enum snake_direction_enum snake_direction = SNAKE_UP;

float velocity = 2.73;		// my_sqrt(5) + 0.5

int points = 0;
int foodx = 1;
int foody = 1;

// Initialize the snake
void
init_snake ()
{
  snake[0][0] = rand_lim (10) + 5;
  snake[0][1] = rand_lim (10) + 5;

  if (snake[0][0] >= 10 && snake[0][1] >= 10)
    {
      snake[1][0] = snake[0][0] - 1;
      snake[1][1] = snake[0][1];
      snake[2][0] = snake[0][0] - 2;
      snake[2][1] = snake[0][1];
      snake[3][0] = snake[0][0] - 3;
      snake[3][1] = snake[0][1];
      snake[4][0] = snake[0][0] - 4;
      snake[4][1] = snake[0][1];
      snake_direction = SNAKE_RIGHT;
    }
  else if (snake[0][0] >= 10 && snake[0][1] <= 10)
    {
      snake[1][0] = snake[0][0];
      snake[1][1] = snake[0][1] + 1;
      snake[2][0] = snake[0][0];
      snake[2][1] = snake[0][1] + 2;
      snake[3][0] = snake[0][0];
      snake[3][1] = snake[0][1] + 3;
      snake[4][0] = snake[0][0];
      snake[4][1] = snake[0][1] + 4;
      snake_direction = SNAKE_UP;
    }
  else if (snake[0][0] <= 10 && snake[0][1] >= 10)
    {
      snake[1][0] = snake[0][0];
      snake[1][1] = snake[0][1] - 1;
      snake[2][0] = snake[0][0];
      snake[2][1] = snake[0][1] - 2;
      snake[3][0] = snake[0][0];
      snake[3][1] = snake[0][1] - 3;
      snake[4][0] = snake[0][0];
      snake[4][1] = snake[0][1] - 4;
      snake_direction = SNAKE_DOWN;
    }
  else if (snake[0][0] <= 10 && snake[0][1] <= 10)
    {
      snake[1][0] = snake[0][0] + 1;
      snake[1][1] = snake[0][1];
      snake[2][0] = snake[0][0] + 2;
      snake[2][1] = snake[0][1];
      snake[3][0] = snake[0][0] + 3;
      snake[3][1] = snake[0][1];
      snake[4][0] = snake[0][0] + 4;
      snake[4][1] = snake[0][1];
      snake_direction = SNAKE_LEFT;
    }
}

// Initialize the map/board
void
init_map (unsigned int map_height)
{
  int h = 0, l = 0;

  for (; h <= map_height; h++)
    {
      l = 0;
      map[h][l] = WALL_SYMBOL;
      l = map_height;
      map[h][l] = WALL_SYMBOL;
      l++;
      map[h][l] = '\n';
    }
  for (l = 0; l <= map_height; l++)
    {
      h = 0;
      map[h][l] = WALL_SYMBOL;
      h = map_height;
      map[h][l] = WALL_SYMBOL;
    }
  map[map_height][map_height + 1] = 'E';
  for (h = 0; h <= map_height; h++)
    {
      for (l = 0; l <= map_height; l++)
	{
	  if (map[h][l] != WALL_SYMBOL && map[h][l] != '\n'
	      && map[h][l] != 'E')
	    {
	      map[h][l] = ' ';
	    }
	}
    }

  newfood (map_height);
}

// Output the map/board
int
printmap ()
{
  int y = 0, x;

  printf ("\n ");
  for (;; y++)
    {
      for (x = 0;; x++)
	{
	  if (map[y][x] == 'E')
	    {
	      points = (lenofsnake () * lenofsnake ()) / 4 - 6;
	      printf ("\n\t\t\033[1;37mPoints: %d\x1b[0m\t\n", points);
	      return 0;
	    }
	  else if (istheresnake (y, x))
	    {
	      printf ("\033[1;32mo \x1b[0m");
	    }
	  else
	    {
	      if (map[y][x] == WALL_SYMBOL)
		{
		  printf ("\033[47;37m%c \x1b[0m", map[y][x]);
		}
	      else if (map[y][x] == FOOD_SYMBOL)
		{
		  printf ("\033[1;31m%c \x1b[0m", map[y][x]);
		}
	      else
		{
		  printf ("%c ", map[y][x]);
		  if (map[y][x] == '\n')
		    {
		      break;
		    }
		}
	    }
	}
    }
}

// Check if the snake occupies the given coordinates
int
istheresnake (int y, int x)
{
  int iterator = 0;
  int spotx, spoty;

  if (!y && !x)
    {
      return 0;
    }

  for (; (spotx = snake[iterator][0]) && (spoty = snake[iterator][1]);
       iterator++)
    {
      if (spotx == x && spoty == y)
	{
	  return 1;
	}
    }
  return 0;
}

// Get the pressed key
int
getch ()
{
  /*http://forums.debian.net/viewtopic.php?f=8&t=59524 */
  int ch;
  struct termios origterm, tmpterm;

  tcgetattr (STDIN_FILENO, &origterm);
  tmpterm = origterm;
  tmpterm.c_lflag &= ~(ICANON | ECHO);
  tcsetattr (STDIN_FILENO, TCSANOW, &tmpterm);
  ch = getchar ();
  tcsetattr (STDIN_FILENO, TCSANOW, &origterm);

  return ch;
}

// Check if key is pressed
int
kbhit ()
{
  /*http://cboard.cprogramming.com/c-programming/63166-kbhit-linux.html */

  struct termios oldt, newt;
  int ch;
  int oldf;

  tcgetattr (STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr (STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl (STDIN_FILENO, F_GETFL, 0);
  fcntl (STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

  ch = getchar ();

  tcsetattr (STDIN_FILENO, TCSANOW, &oldt);
  fcntl (STDIN_FILENO, F_SETFL, oldf);

  if (ch != EOF)
    {
      ungetc (ch, stdin);
      return 1;
    }
  return 0;
}

// Extend the snake
void
grow ()
{
  int index = lenofsnake ();
  switch (snake_direction)
    {
    case SNAKE_UP:
      snake[index][0] = snake[index - 1][0];
      snake[index][0] = snake[index - 1][0] + 1;
      break;
    case SNAKE_RIGHT:
      snake[index][0] = snake[index - 1][0] - 1;
      snake[index][0] = snake[index - 1][0];
      break;
    case SNAKE_DOWN:
      snake[index][0] = snake[index - 1][0];
      snake[index][0] = snake[index - 1][0] - 1;
      break;
    case SNAKE_LEFT:
      snake[index][0] = snake[index - 1][0] + 1;
      snake[index][0] = snake[index - 1][0];
      break;
    default:
      return;
    }
}

// Place new food on the board
void
newfood (unsigned int map_height)
{
  int i, tocontinue = 0;

  map[foody][foodx] = ' ';

  while (1)
    {
      tocontinue = 0;
      foodx = rand_lim (map_height - 2) + 1;
      foody = rand_lim (map_height - 2) + 1;

      if (map[foody][foodx] == WALL_SYMBOL)
	{
	  continue;
	}

      for (i = 0; snake[i][0]; i++)
	{
	  if (foodx == snake[i][0] && foody == snake[i][1])
	    {
	      tocontinue = 1;
	    }
	}

      if (!tocontinue)
	{
	  break;
	}
    }

  map[foody][foodx] = FOOD_SYMBOL;
}

// Handle keypresses
void
turn (char input, int map_height)
{
  switch (input)
    {
    case 'q':
      end (0);
      break;
    case 'p':
      paused ();
      break;
    case 'i':
      info ();
      break;
    case '[':
    case KEY_ESC:
      return;
    case 'w':
    case 'A':
      if (snake_direction == SNAKE_RIGHT || snake_direction == SNAKE_LEFT)
	{
	  snake_direction = SNAKE_UP;
	}
      break;
    case 'd':
    case 'C':
      if (snake_direction == SNAKE_UP || snake_direction == SNAKE_DOWN)
	{
	  snake_direction = SNAKE_RIGHT;
	}
      break;
    case 's':
    case 'B':
      if (snake_direction == SNAKE_RIGHT || snake_direction == SNAKE_LEFT)
	{
	  snake_direction = SNAKE_DOWN;
	}
      break;
    case 'a':
    case 'D':
      if (snake_direction == SNAKE_UP || snake_direction == SNAKE_DOWN)
	{
	  snake_direction = SNAKE_LEFT;
	}
      break;
    default:
      return;
    }
  move (map_height);
}

// Retrieve the length of the snake
int
lenofsnake ()
{
  int i = 0;
  for (; snake[i][0]; i++);
  return i;
}

// Move the snake one step
void
move (unsigned int map_height)
{
  int i, direction[2] = { 0, 0 };

  switch (snake_direction)
    {
    case SNAKE_UP:
      direction[0] = 0;
      direction[1] = -1;
      break;
    case SNAKE_LEFT:
      direction[0] = -1;
      direction[1] = 0;
      break;
    case SNAKE_DOWN:
      direction[0] = 0;
      direction[1] = 1;
      break;
    case SNAKE_RIGHT:
      direction[0] = 1;
      direction[1] = 0;
      break;
    default:
      break;
    }

  for (i = lenofsnake () - 1; i > 0; i--)
    {
      snake[i][0] = snake[i - 1][0];
      snake[i][1] = snake[i - 1][1];
    }

  snake[lenofsnake ()][0] = 0;
  snake[lenofsnake ()][1] = 0;

  snake[0][0] = snake[0][0] + direction[0];
  snake[0][1] = snake[0][1] + direction[1];

  if (map[snake[0][1]][snake[0][0]] == FOOD_SYMBOL)
    {
      grow ();
      newfood (map_height);
    }
  else if (map[snake[0][1]][snake[0][0]] != ' ')
    {
      end (1);
    }

  for (i = 1; snake[i][0]; i++)
    {
      if (snake[0][0] == snake[i][0] && snake[0][1] == snake[i][1])
	{
	  end (1);
	}
    }
}

// Output when the game is paused
void
paused ()
{
  printf ("\r\n");
  printf ("GAME PAUSED (Press 'p' to continue)\n");
  wait_for_key ('p', false);
}

// Wait for a given key, or any if 'any' is true
void
wait_for_key (char k, bool any)
{
  while (1)
    {
      if (any && getch ())
	{
	  break;
	}
      else if (getch () == k)
	{
	  break;
	}
    }
}

// Output various info about the application
void
info (void)
{
  system ("clear");

  printf ("\n%s - %s\n", "maggot", "Simple snake game written in C");
  printf ("%s: %s\n", "Version", VERSION);
  printf ("%s: %s <%s>\n", "Author", "Gonzalo Ciruelos",
	  "comp.gonzalo@gmail.com");
  printf ("%s: %s\n", "License", "GPL v2");

  wait_for_key (0, true);
}

// Output the end screen
void
end (bool dead)
{
  system ("clear");

  if (dead)
    {
      printf ("\nYOU LOST\n");
    }
  else
    {
      printf ("\nTHANKS FOR PLAYING\n");
    }
  printf ("\nPOINTS: %d\n\n\n", points);

  exit (0);
}

// Alternative square root function
double
my_sqrt (float n)
{
  // Babylonian method
  float iter = 0, x = 1;
  while (iter < 100)
    {
      x = x / 2 + n / (2 * x);
      iter++;
    }
  return x;
}

// Function for retrieving random numbers
int
rand_lim (int limit)
{
  // http://stackoverflow.com/questions/2999075/generate-a-random-number-within-range/2999130#2999130
  // Somewhat modified.
  int r;
  int divisor = RAND_MAX / (limit + 1), retval;

  do
    {
      r = rand ();
      retval = r / divisor;
    }
  while (retval > limit);

  return retval;
}

int
main ()
{
  srand ((unsigned int) time (0));

  init_snake ();
  init_map (MAP_HEIGHT);

  struct timespec delay;
  delay.tv_sec = 0;
  delay.tv_nsec = 500000000L / velocity;

  bool isupdated = false;

  while (1)
    {
      while (!kbhit ())
	{
	  if (!isupdated)
	    {
	      move (MAP_HEIGHT);
	    }
	  system ("clear");
	  printmap ();
	  nanosleep (&delay, NULL);
	  isupdated = false;
	  velocity = my_sqrt (lenofsnake ()) + 0.5;
	}
      turn (getch (), MAP_HEIGHT);
      fflush (stdin);
      isupdated = true;
      continue;
    }

  return 0;
}
