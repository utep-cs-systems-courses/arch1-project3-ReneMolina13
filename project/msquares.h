#ifndef MSQUARES
#define MSQUARES


#include <msp430.h>
#include <stdlib.h>
#include <time.h>
#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"
#include "lcdtypes.h"




/* MACROS */


/* Array sizes */
#define NUM_POSITIONS 5
#define NUM_SQCOLORS 43

/* common_positions array indices */
#define UPPER_LEFT  0
#define LOWER_LEFT  1
#define LOWER_RIGHT 2
#define UPPER_RIGHT 3
#define MIDDLE      4

/* LEDs & switches */
#define LED BIT6    // represents P1.6 since P1.0 used for LCD
#define SWITCHES 15 // all Port 2 switches
#define SW_0 8      // P1.3
#define SW_1 1      // P2.0
#define SW_2 2      // P2.1
#define SW_3 4      // P2.2
#define SW_4 8      // P2.3

/* Used as flags for random function */
#define RAND_POSITION 0
#define RAND_COLOR    1 
#define RAND_FONT     2  

/* Represents fonts in the current_font variable */
#define SMALL_FONT   0
#define MEDIUTM_FONT 1
#define LARGE_FONT   2

/* Represents directions in the next_position variable */
#define LEFT       0
#define UP         1
#define DOWN       2
#define RIGHT      3
#define LEFT_UP    4
#define LEFT_DOWN  5
#define RIGHT_UP   6
#define RIGHT_DOWN 7
#define NO_CHANGE  8




/* STRUCTUES & ARRAYS */


/* structure indicating a position on LCD screen */
typedef struct {
  u_int col, row;
} Pos;

/* position & color arrays */
Pos common_positions[NUM_POSITIONS]; // holds an array of common LCD position
u_int sqColors[NUM_SQCOLORS];        // holds an array of LCD colors




/* VARIABLES */


/* current/next positions, colors, & fonts */
Pos current_position; // holds the current pixel position
u_char next_position; // holds the direction of the next pixel position
u_char current_color; // holds the current color that will be drawn
u_char current_font;  // holds the value of the current font

/* control flags */
u_int redrawScreen;   // flag that determines if a new draw operation is needed
u_int switchPort;     // flag that determines if the pressed switch is P1 or P
u_int switches;       // holds the switches that are currently pressed




/* FUNCTIONS */


void msquaresInit();                           // initializes miscellaneous items
void switch_init();                            // initializes switches
static u_char switch_update_interrupt_sense(); // ensures button interrupt on both press & release
static u_char random(u_char type);             // provides either a random font or color
void update_shape();                           // updates the shape on LCD screen


#endif /* MSQUARES */
