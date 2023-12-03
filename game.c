/*
Authors:
Axel Isaksson,
F Lundevall,
Elias Hollstrand,
Mattias Kvist

Date: 2023-11-01

For copyright and licensing, see file COPYING
*/

#include <stdint.h>	 /* Declarations of uint_32 and the like */
#include <pic32mx.h> /* Declarations of system-specific addresses etc */
#include "declare.h" /* Declatations for these labs */

#define GRASSX 127
// #define CHARACTER_WIDTH 5 
// #define CHARACTER_STANDING_HEIGHT 10 
// #define DINO_DUCKING_HEIGHT 4 
#define OBSTACLE_SPAWN_WIDTH 5
#define OBSTACLE_SPAWN_X 127
#define BIG_OBSTACLE_HEIGHT 10
#define SMALL_OBSTACLE_HEIGHT 5
#define JUMP_VELOCITY -4
#define GRAVITY 0.5f
#define GROUND_Y 31
#define MID_AIR_Y 25
#define HIGH_AIR_Y 20
#define DINO_WIDTH 13
#define DINO_STANDING_HEIGHT 15
#define DINO_DUCKING_HEIGHT 6

#define BTN4 4
#define BTN3 2

int character_x;
float character_y;
int character_height;
int i;
int grass_x;
int grass_x2;
int obstacle_x;
int obstacle_y;
int obstacle_height;
int obstacle_width;
float y_velocity;
float speed;

int score;
int highscore;


void update_display(void)
{
	// Clear the display
	clear_all_pixels();

	// Draw the ground
	draw_ground();

	// Draw the grass
	// draw_grass();

	// Draw the character
	draw_character();

	draw_obstacles();

	draw_string(0, 0, "score: ");
	draw_number(35, 0, score);

	draw_string(50, 0, "highscore: ");
	draw_number(110, 0, highscore);

	// Update the display
	display_objects();
}

void update_game(void)
{
	// Move the character
	move_character();

	// Move the grass
	// grass_x--;
	// grass_x = grass_x % 128;

	// grass_x2--;
	// grass_x2 = grass_x2 % 128;

	// Move the obstacle
	move_obstacle();

	// Check for collisions
	check_collision();
}

void draw_ground(void)
{
	fill_rectangle(0, 31, 128, 1);
}

// void draw_grass(void)
// {
// 	set_pixel(grass_x, 30);
// 	set_pixel(grass_x, 29);
// 	set_pixel(grass_x - 1, 28);
// 	set_pixel(grass_x + 1, 28);

// 	set_pixel(grass_x2, 30);
// 	set_pixel(grass_x2, 29);
// 	set_pixel(grass_x2 - 1, 28);
// 	set_pixel(grass_x2 + 1, 28);
// }

void draw_obstacles(void)
{
	int width = obstacle_width;
	if (obstacle_x + obstacle_width > 128)
	{
		width = 128 - obstacle_x;
	}
	fill_rectangle(obstacle_x, obstacle_y, width, obstacle_height);
}

void draw_character(void)
{
	draw_image(character_x, (int)character_y, DINO_WIDTH, DINO_STANDING_HEIGHT, dino);
	// fill_rectangle(character_x, (int)character_y, CHARACTER_WIDTH, character_height);
}

void move_character()
{
	// check for button presses
	if (getbtns() == BTN4 && character_y > 10 && y_velocity <= 0) // BTN4
	{
		y_velocity = JUMP_VELOCITY;
	}
	else if (getbtns() == BTN3 && character_y == GROUND_Y - character_height) // BTN3
	{
		character_height = DINO_DUCKING_HEIGHT;
		character_y = 31 - DINO_DUCKING_HEIGHT;
	}
	else if (!(getbtns() == BTN3) && character_height == DINO_DUCKING_HEIGHT)
	{
		character_height = DINO_STANDING_HEIGHT;
	}

	// Update the character's y position
	y_velocity += GRAVITY;
	if (character_y + y_velocity > GROUND_Y - character_height) 
	{
		character_y = GROUND_Y - character_height;
		y_velocity = 0;
	}
	else
	{
		character_y += y_velocity;
	}
}

void check_collision()
{
	// Check if the character is colliding with the obstacle
	if (character_x + DINO_WIDTH >= obstacle_x && character_x <= obstacle_x + obstacle_width) // Check if the character is in the x range of the obstacle
	{
		if (obstacle_y + obstacle_height >= character_y && obstacle_y <= character_y + character_height) // Check if the character is in the y range of the obstacle
		{
			insert_score(score);
			change_state(GAME_OVER_STATE);
		}

	}
}

// Returns a seemingly random integer between 0 and 3
int random_int()
{
	int random = (TMR3 + score) % 4;
	TMR3 = 0;
	return (random);
}

void move_obstacle()
{	
	speed = 0.05f * score + 1.0;
	if (obstacle_x > 0 && (obstacle_x-=speed > 0)){
		obstacle_x-=speed;
	} else
	{
		// Shrink the obstacle
		obstacle_width-=speed;
		if(obstacle_width <= 1) {
			spawn_obstacle();
			score++;
		}
	}
}

// Spawn a new obstacle
void spawn_obstacle()
{
	obstacle_x = OBSTACLE_SPAWN_X;
	obstacle_width = OBSTACLE_SPAWN_WIDTH;
	switch (random_int())
	{
	case 0: // Spawn a small obstacle on the ground
		obstacle_y = GROUND_Y - SMALL_OBSTACLE_HEIGHT;
		obstacle_height = SMALL_OBSTACLE_HEIGHT;
		break;
	
	case 1: // Spawn a big obstacle
		obstacle_y = GROUND_Y - BIG_OBSTACLE_HEIGHT;
		obstacle_height = BIG_OBSTACLE_HEIGHT;
		break;
	
	case 2: // Spawn a small obstacle in the middle
		obstacle_y = MID_AIR_Y - SMALL_OBSTACLE_HEIGHT;
		obstacle_height = SMALL_OBSTACLE_HEIGHT;
		break;	

	case 3: // Spawn a small obstacle in the air
		obstacle_y = HIGH_AIR_Y - SMALL_OBSTACLE_HEIGHT;
		obstacle_height = SMALL_OBSTACLE_HEIGHT;
		break;

	default:
		break;
	}
}

void reset_game(void)
{
    character_x = 10;
    character_y = GROUND_Y - DINO_STANDING_HEIGHT;
    character_height = DINO_STANDING_HEIGHT;
    i = 0;
    grass_x = GRASSX;
    grass_x2 = GRASSX + 20;
    obstacle_x = OBSTACLE_SPAWN_X;
    obstacle_y = GROUND_Y - SMALL_OBSTACLE_HEIGHT;
    obstacle_height = SMALL_OBSTACLE_HEIGHT;
    obstacle_width = OBSTACLE_SPAWN_WIDTH;
    y_velocity = 0;
    speed = 1;
    score = 0;
}