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
#define CHARACTER_WIDTH 5
#define CHARACTER_STANDING_HEIGHT 10
#define CHARACTER_DUCKING_HEIGHT 4
#define OBSTACLE_SPAWN_WIDTH 5
#define OBSTACLE_HEIGHT 10
#define JUMP_VELOCITY -5
#define GRAVITY 1

static int FPS = 10;

int character_x = 10;
int character_y = 31 - CHARACTER_STANDING_HEIGHT;
int character_height = CHARACTER_STANDING_HEIGHT;
int i = 0;
int grass_x = GRASSX;
int grass_x2 = GRASSX + 20;
int obstacle_x = 127;
int obstacle_y = 31 - OBSTACLE_HEIGHT;
int obstacle_width = OBSTACLE_SPAWN_WIDTH;
int y_velocity = 0;

int score = 0;

void user_isr(void)
{
	if (IFS(0) & 0x100) // Timer 2 interrupt
	{
		// Clear the interrupt flag
		IFSCLR(0) = 0x100;

		// Update game logic
		update_game();

		// Update the display
		update_display();
	}
}

void update_display(void)
{
	// Clear the display
	clear_all_pixels();

	// Draw the ground
	draw_ground();

	// Draw the grass
	draw_grass();

	// Draw the character
	draw_character();

	draw_obstacles();

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

void draw_grass(void)
{
	set_pixel(grass_x, 30);
	set_pixel(grass_x, 29);
	set_pixel(grass_x - 1, 28);
	set_pixel(grass_x + 1, 28);

	set_pixel(grass_x2, 30);
	set_pixel(grass_x2, 29);
	set_pixel(grass_x2 - 1, 28);
	set_pixel(grass_x2 + 1, 28);
}

void draw_obstacles(void)
{
	fill_rectangle(obstacle_x, obstacle_y, obstacle_width, OBSTACLE_HEIGHT);
}

void draw_character(void)
{
	fill_rectangle(character_x, character_y, CHARACTER_WIDTH, character_height);
}

void move_character()
{
	// check for button presses
	if (getbtns() == 4 && character_y > 10 && y_velocity <= 0) // BTN4
	{
		y_velocity = JUMP_VELOCITY;
	}
	else if (getbtns() == 2 && character_y == 31 - character_height) // BTN3
	{
		character_height = CHARACTER_DUCKING_HEIGHT;
		character_y = 31 - CHARACTER_DUCKING_HEIGHT;
	}
	else if (!(getbtns() == 2) && character_height == CHARACTER_DUCKING_HEIGHT)
	{
		character_height = CHARACTER_STANDING_HEIGHT;
	}

	// Update the character's y position
	y_velocity += GRAVITY;
	if (character_y + y_velocity > 31 - character_height) // 31 is the ground
	{
		character_y = 31 - character_height;
		y_velocity = 0;
	}
	else
	{
		character_y += y_velocity;
	}
}

void move_obstacle()
{
	if (obstacle_x > 0){
		obstacle_x--;
	}
	else if (obstacle_x <= 0)
	{
		// Shrink the obstacle
		obstacle_width--;
		if (obstacle_width < 0)
		{
			obstacle_width = OBSTACLE_SPAWN_WIDTH;
			obstacle_x = 127; // Reset the obstacle
			score++;
		}
	}
}

void check_collision()
{
	// Check if the character is colliding with the obstacle
	if (character_x + CHARACTER_WIDTH >= obstacle_x && character_x <= obstacle_x + obstacle_width)
	{
		if (character_y + character_height >= obstacle_y)
		{
			// respawn the obstacle
			obstacle_x = 127;
		}
	}
}
int main(void)
{
	/* Set up timers, interrupts, input and outputs, display, I2C etc. */
	chip_init();

	while (1)
	{
	}
	return 0;
}
