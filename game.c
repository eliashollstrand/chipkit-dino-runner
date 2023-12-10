/**
 * @file game.c
 * @brief This file contains the implementation of the game logic and display functions.
 *
 * This file defines the game logic and display functions for a simple side-scrolling dino runner game.
 * It includes functions for updating the game state, drawing the ground, character, and obstacles,
 * checking for collisions, and generating random numbers. The game state includes variables for
 * the character's position, height, and width, the obstacle's position, height, and width,
 * the character's action, the score, and the highscore. The display functions are responsible
 * for updating the display by clearing all pixels, drawing the ground, character, obstacles,
 * score, and highscore, and displaying the objects. The game logic functions handle moving
 * the character, moving the obstacle, checking for collisions, and spawning new obstacles.
 * There is also a function for resetting the game state to its initial values.
 *
 * @author Axel Isaksson
 * @author F Lundevall
 * @author Elias Hollstrand
 * @author Mattias Kvist
 *
 * @date 2023-12-10
 *
 * For copyright and licensing, see file COPYING
 */

#include <stdint.h>	 /* Declarations of uint_32 and the like */
#include <pic32mx.h> /* Declarations of system-specific addresses etc */
#include "declare.h" /* Declatations for these labs */

#define OBSTACLE_SPAWN_X 127
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
int obstacle_x;
int obstacle_y;
int obstacle_height;
int obstacle_width;
float y_velocity;
float speed;

int score = 0;
int highscore;
int dino_frames_passed = 0;
int bird_frames_passed = 0;

characterAction action;
const uint8_t *obstacle;

int leaderboard_index;


/**
 * @brief Draws the ground on the screen.
 * 
 * This function fills a rectangle on the screen to represent the ground.
 * The rectangle starts at coordinates (0, 31) and has a width of 128 pixels
 * and a height of 1 pixel.
 */
void draw_ground(void) {
	fill_rectangle(0, 31, 128, 1);
}


/**
 * @brief Draws the obstacles on the screen.
 
 * The width of the obstacle is adjusted if it exceeds the screen width.
 */
void draw_obstacles(void) {
	int width = obstacle_width;

	if (obstacle_x + obstacle_width > 128) {
		width = 128 - obstacle_x;
	}

	draw_image(obstacle_x, obstacle_y, width, obstacle_height, obstacle);
}

/**
 * @brief Draws the character on the screen based on the current action and animation frames.

 * The character image is selected based on the action and the number of frames passed.
 * If the action is RUNNING, the character image alternates between dino1 and dino2 every 5 frames (running animation).
 * If the action is DUCKING, the character image alternates between dino_ducking1 and dino_ducking2 every 5 frames (ducking and running).
 * The character's position and dimensions are passed to the draw_image function to draw the image on the screen.
 */
void draw_character(void) {
	const uint8_t *image;

	switch(action) {
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

/**
 * @brief Moves the character based on button presses and updates its position.
 */
void move_character() {
	// check for button presses
	if(getbtns() == BTN4 && character_y > 10 && y_velocity <= 0) { // BTN4
		y_velocity = JUMP_VELOCITY;
		action = RUNNING;
	} else if (getbtns() == BTN3 && character_y == GROUND_Y - character_height) { // BTN3
		character_height = DINO_DUCKING_HEIGHT;
		character_width = DINO_DUCKING_WIDTH;
		character_y = 31 - DINO_DUCKING_HEIGHT;
		action = DUCKING;
	} else if (!(getbtns() == BTN3) && character_height == DINO_DUCKING_HEIGHT) {
		character_height = DINO_STANDING_HEIGHT;
		character_width = DINO_STANDING_WIDTH;
		action = RUNNING;
	}

	// Update the character's y position
	y_velocity += GRAVITY;
	if(character_y + y_velocity > GROUND_Y - character_height) {
		character_y = GROUND_Y - character_height;
		y_velocity = 0;
	} else {
		character_y += y_velocity;
	}
}

/**
 * @brief Checks if the character is colliding with the obstacle.
 * 
 * If there is a collision, it calls insert_score().
 */
void check_collision() {
	// Check if the character is colliding with the obstacle
	if(character_x + character_width - 3 >= obstacle_x + 3 && character_x + 3 <= obstacle_x + obstacle_width - 3) { // Check if the character is in the x range of the obstacle
		if(obstacle_y + obstacle_height >= character_y && obstacle_y <= character_y + character_height) { // Check if the character is in the y range of the obstacle
			insert_score(score);
		}
	}
}

/**
 * @brief Generates a seemingly random integer based on the value of TMR3 and the score.
 * 
 * @return The generated random integer.
 */
int random_int() {
	int random = (TMR3 + score) % 4;
	TMR3 = 0;
	return random;
}

/**
 * @brief spawns an obstacle in the game.
 * 
 * This function spawns an obstacle in the game. The type of obstacle spawned 
 * is determined randomly. The obstacle's position, height, and width are set 
 * based on the type of obstacle.
 */
void spawn_obstacle() {
	obstacle_x = OBSTACLE_SPAWN_X;

	switch(random_int()) {
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

/**
 * @brief Moves the obstacle in the game.
 
 * The speed of the obstacle is determined by the score.
 * If the obstacle is off the screen, a new obstacle is spawned and the score is incremented.
 * update_LEDs() is called to visualize the score.
 */
void move_obstacle() {	
	speed = 0.05f * score + 2.0;
	
	if(obstacle_x + obstacle_width > 0) {
		obstacle_x-=speed;

		// Animate the bird
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
	} else {
		if(obstacle_x + obstacle_width <= 1) {
			spawn_obstacle();
			score++;	
			update_LEDs();
		}
	}
}

/**
 * @brief Resets the game state to its initial values.
 * 
 * This function resets the character's position, height, width, velocity, speed, score, and action.
 * It also calls the spawn_obstacle() function to generate a new obstacle and updates the LEDs.
 */
void reset_game(void) {
	character_x = 10;
	character_y = GROUND_Y - DINO_STANDING_HEIGHT;
	character_height = DINO_STANDING_HEIGHT;
	character_width = DINO_STANDING_WIDTH;
	y_velocity = 0;
	speed = 1;
	score = 0;
	action = RUNNING;
	spawn_obstacle();
	update_LEDs();
}

/**
 * @brief draws everything on the screen.
 * 
 * Updates the display by clearing all pixels, drawing the ground, character, obstacles,
 * score, and highscore, and displaying the objects.
 */
void update_display(void) {
	clear_all_pixels();

	draw_ground();

	draw_character();

	draw_obstacles();

	draw_string(0, 0, "score: ");

	draw_number(35, 0, score);

	draw_string(50, 0, "highscore: ");
	
	draw_number(110, 0, highscore);

	display_objects();
}

/**
 * @brief Updates the game state by moving the character, moving the obstacle, and checking for collisions.
 * 
 * This function is responsible for updating the game state by performing the following actions:
 * - Moving the character.
 * - Moving the obstacle.
 * - Checking for collisions between the character and the obstacle.
 * 
 * @return void
 */
void update_game(void) {
	move_character();

	move_obstacle();

	check_collision();
}