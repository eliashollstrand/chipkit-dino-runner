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
#define OBSTACLE_SPAWN_X 127
// #define OBSTACLE_SPAWN_WIDTH 5
#define BIG_OBSTACLE_HEIGHT 10
#define SMALL_OBSTACLE_HEIGHT 5
#define SMALL_CACTUS_WIDTH 6
#define SMALL_CACTUS_HEIGHT 11
#define BIG_CACTUS_WIDTH 8
#define BIG_CACTUS_HEIGHT 14
#define BIRD_WIDTH 12
#define BIRD_HEIGHT 6
#define JUMP_VELOCITY -4
#define GRAVITY 0.4f
#define GROUND_Y 31
#define MID_AIR_Y 25
#define HIGH_AIR_Y GROUND_Y - 11
#define DINO_STANDING_WIDTH 13
#define DINO_STANDING_HEIGHT 15
#define DINO_DUCKING_HEIGHT 10
#define DINO_DUCKING_WIDTH 20

#define BTN4 4
#define BTN3 2

int character_x;
float character_y;
int character_height;
int character_width;
int i;
int grass_x;
int grass_x2;
int obstacle_x;
int obstacle_y;
int obstacle_height;
int obstacle_width;
float y_velocity;
float speed;

int score = 0;
int highscore;
// int dino_dino_frames_passed = 0;
// int dino_ducking_dino_frames_passed = 0;
int dino_frames_passed = 0;
int bird_frames_passed = 0;

characterAction action;
uint8_t *obstacle;

int leaderboard_index;

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
	draw_image(obstacle_x, obstacle_y, width, obstacle_height, obstacle);
}

void draw_character(void)
{
	uint8_t *image;
	switch (action)
	{
	case RUNNING:
		if(dino_frames_passed <= 5) {
			image = dino1;
			dino_frames_passed++;
		} else {
			image = dino2;
			dino_frames_passed++;
			if(dino_frames_passed >= 10) {
				dino_frames_passed = 0;
			}
		}
		break;

	case DUCKING:
		if(dino_frames_passed <= 5) {
			image = dino_ducking1;
			dino_frames_passed++;
		} else {
			image = dino_ducking2;
			dino_frames_passed++;
			if(dino_frames_passed >= 10) {
				dino_frames_passed = 0;
			}
		}
		break;
	
	default:
		break;
	}
	draw_image(character_x, (int)character_y, character_width, character_height, image);
}

void move_character()
{
	// check for button presses
	if (getbtns() == BTN4 && character_y > 10 && y_velocity <= 0) // BTN4
	{
		y_velocity = JUMP_VELOCITY;
		action = RUNNING;
	}
	else if (getbtns() == BTN3 && character_y == GROUND_Y - character_height) // BTN3
	{
		character_height = DINO_DUCKING_HEIGHT;
		character_width = DINO_DUCKING_WIDTH;
		character_y = 31 - DINO_DUCKING_HEIGHT;
		action = DUCKING;
	}
	else if (!(getbtns() == BTN3) && character_height == DINO_DUCKING_HEIGHT)
	{
		character_height = DINO_STANDING_HEIGHT;
		character_width = DINO_STANDING_WIDTH;
		action = RUNNING;
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
	if (character_x + character_width - 3 >= obstacle_x + 3 && character_x + 3 <= obstacle_x + obstacle_width - 3) // Check if the character is in the x range of the obstacle
	{
		if (obstacle_y + obstacle_height >= character_y && obstacle_y <= character_y + character_height) // Check if the character is in the y range of the obstacle
		{
			insert_score(score);
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
	speed = 0.05f * score + 2.0;
	if (obstacle_x + obstacle_width > 0){
		obstacle_x-=speed;

		if(obstacle == bird1 && bird_frames_passed < 3) {
			bird_frames_passed++;
			if(bird_frames_passed == 3) {
				obstacle = bird2;
				bird_frames_passed = 0;
			}
		} else if(obstacle == bird2 && bird_frames_passed < 3){
			bird_frames_passed++;
			if(bird_frames_passed == 3) {
				obstacle = bird1;
				bird_frames_passed = 0;
			}
		}
	} else
	{
		if(obstacle_x + obstacle_width <= 1) {
			spawn_obstacle();
			score++;	
			update_LEDs();
		}
	}
}

// Spawn a new obstacle
void spawn_obstacle()
{
	obstacle_x = OBSTACLE_SPAWN_X;
	switch (random_int())
	{
	case 0: // Spawn a small obstacle on the ground
		obstacle = cactus_small;
		obstacle_y = GROUND_Y - SMALL_CACTUS_HEIGHT;
		obstacle_height = SMALL_CACTUS_HEIGHT;
		obstacle_width = SMALL_CACTUS_WIDTH;
		break;
	
	case 1: // Spawn a big obstacle
		obstacle = cactus_big;
		obstacle_y = GROUND_Y - BIG_CACTUS_HEIGHT;
		obstacle_height = BIG_CACTUS_HEIGHT;
		obstacle_width = BIG_CACTUS_WIDTH;
		break;
	
	case 2: // Spawn a bird at low height
		obstacle = bird1;
		obstacle_y = MID_AIR_Y - BIRD_HEIGHT;
		obstacle_height = BIRD_HEIGHT;
		obstacle_width = BIRD_WIDTH;
		break;	

	case 3: // Spawn a bird at high height
		obstacle = bird1;
		obstacle_y = HIGH_AIR_Y - BIRD_HEIGHT;
		obstacle_height = BIRD_HEIGHT;
		obstacle_width = BIRD_WIDTH;
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
	character_width = DINO_STANDING_WIDTH;
    i = 0;
    grass_x = GRASSX;
    grass_x2 = GRASSX + 20;
    y_velocity = 0;
    speed = 1;
    score = 0;
	action = RUNNING;
	spawn_obstacle();
	update_LEDs();
}