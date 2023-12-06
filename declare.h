/* 
Authors: 
Axel Isaksson,
F Lundevall,
Elias Hollstrand,
Mattias Kvist

Date: 2023-11-01

For copyright and licensing, see file COPYING 
*/

/* Declare display-related functions from mipslabfunc.c */
void display_image(int x, const uint8_t *data);
void display_init(void);
void display_string(int line, char *s);
void display_update(void);
uint8_t spi_send_recv(uint8_t data);


/* Declare display_debug - a function to help debugging.

   After calling display_debug,
   the two middle lines of the display show
   an address and its current contents.

   There's one parameter: the address to read and display.

   Note: When you use this function, you should comment out any
   repeated calls to display_image; display_image overwrites
   about half of the digits shown by display_debug.
*/
void display_debug( volatile int * const addr );

/* Declare bitmap array containing font */
extern const uint8_t const font[128*8];
/* Declare bitmap array containing icon */
extern const uint8_t const icon[128];
/* Declare text buffer for display output */
extern char textbuffer[4][16];

// Declare an array of letters
extern const uint8_t const letters[28][25];

// Declare an array containing dino character
extern const uint8_t const dino1[13][15];
extern const uint8_t const dino2[13][15];
extern const uint8_t const dino_ducking1[20][10];
extern const uint8_t const dino_ducking2[20][10];

// Declare arrays containing cactae
extern const uint8_t const cactus_small[6][11];
extern const uint8_t const cactus_big[8][14];

// Declare array containing bird
extern const uint8_t const bird1[12][6];
extern const uint8_t const bird2[12][6];

// Declare an array of numbers
extern const uint8_t const numbers[10][20];

// Declare an array containing arrow_up
extern const uint8_t const arrow_up [5][5];

// gamestates for the game
typedef enum {
    MENU_STATE,
    GAME_STATE,
    GAME_OVER_STATE,
    ENTER_NAME_STATE
} GameState;

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

// Declare leaderboard 
extern uint8_t leaderboard[6];

#define NUM_LEADERBOARD_ENTRIES 6
#define INITIALS_LENGTH 3

extern uint8_t leaderboard_scores[NUM_LEADERBOARD_ENTRIES];

extern char leaderboard_initials[NUM_LEADERBOARD_ENTRIES][INITIALS_LENGTH];
extern int letter_index;
extern char initials[3];
extern int leaderboard_index;