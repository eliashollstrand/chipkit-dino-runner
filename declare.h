/**
 * @file declare.h
 * @brief Header file containing declarations for various functions, arrays, and variables used in the game.
 *
 * This file declares display-related functions, arrays for letters, dino characters, cacti, birds, numbers, and arrow_up.
 * It also defines game states and character actions, as well as global variables for the game.
 * Additionally, it declares functions for changing game state and defines constants for leaderboard entries and initials length.
 *
 * @author Axel Isaksson,
 * @author F Lundevall,
 * @author Elias Hollstrand,
 * @author Mattias Kvist
 *
 * @date 2023-12-07
 *
 * For copyright and licensing, see file COPYING.
 */

/* Declare display-related functions from mipslabfunc.c */
void display_init(void);
uint8_t spi_send_recv(uint8_t data);

/*------------------------------------------------------------------*/
/* Code by Elias Hollstrand and Matƒtias Kvist */

// Declare an array of letters
extern const uint8_t const letters[28][25];

// Declare an array containing dino character
extern const uint8_t const dino1[195];
extern const uint8_t const dino2[195];
extern const uint8_t const dino_ducking1[200];
extern const uint8_t const dino_ducking2[200];

// Declare arrays containing cactae
extern const uint8_t const cactus_small[66];
extern const uint8_t const cactus_big[112];

// Declare array containing bird
extern const uint8_t const bird1[72];
extern const uint8_t const bird2[72];

// Declare an array of numbers
extern const uint8_t const numbers[10][20];

// Declare an array containing arrow_up
extern const uint8_t const arrow_up [5][5];

// Declare gamestates for the game
typedef enum {
    MENU_STATE,
    GAME_STATE,
    GAME_OVER_STATE,
    ENTER_NAME_STATE
} GameState;

// Declare character actions for the game
typedef enum {
    RUNNING, 
    DUCKING
} characterAction;

// Declare global variables for the game
extern int highscore;
extern int score;

extern int menu_page;

// Declare function to change gamestate
void change_state(GameState newState);

// Declare constants for leaderboard entries and initials length
#define NUM_LEADERBOARD_ENTRIES 6
#define INITIALS_LENGTH 3

// Declare leaderboard arrays
extern uint8_t leaderboard_scores[NUM_LEADERBOARD_ENTRIES];
extern char leaderboard_initials[NUM_LEADERBOARD_ENTRIES][INITIALS_LENGTH];

// Declare global variables for leaderboard stuff
extern int letter_index;
extern char initials[3];
extern int leaderboard_index;