#ifndef MSQUARES
#define MSQUARES


#include <msp430.h>
#include <stdlib.h>
#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"
#include "lcdtypes.h"




/* MACROS */


/* Array sizes */
#define NUM_POSITIONS 5
#define NUM_COLORS 43
#define STR_LENGTH 5
#define NUM_CHARACTERS 52

/* common_positions array indices */
#define UPPER_LEFT  0
#define LOWER_LEFT  1
#define LOWER_RIGHT 2
#define UPPER_RIGHT 3
#define MIDDLE      4

/* LEDs & switches */
#define LED BIT6    // represents P1.6 since P1.0 used for LCD
#define SWITCHES 15 // all Port 2 switches
#define SW_0 BIT3   // P1.3
#define SW_1 BIT0   // P2.0
#define SW_2 BIT1   // P2.1
#define SW_3 BIT2   // P2.2
#define SW_4 BIT3   // P2.3

/* Used as flags for random function */
#define RAND_POSITION  0
#define RAND_COLOR     1 
#define RAND_CHARACTER 2

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

/* Defines character widths & heights */
#define HEIGHT_5x7 7
#define WIDTH_5x7  5




/* STRUCTUES & ARRAYS */


/* structure indicating a position on LCD screen */
typedef struct {
  u_char col, row;
} Pos;

/* position & color arrays */
Pos common_positions[NUM_POSITIONS]; // holds an array of common LCD position
u_int msColors[NUM_COLORS];          // holds an array of LCD colors
u_char str[STR_LENGTH];              // holds a string to be displayed


/* VARIABLES */


/* current/next positions, colors, & fonts */
Pos current_position;      // holds the current pixel position
u_char next_position;      // holds the direction of the next pixel position
u_char current_font_color; // holds a current index of the colors array
u_char current_background; // holds a current index of the colors array

/* holds the corners of the box to be placed around the drawing */
u_char minCol;
u_char minRow;
u_char maxCol;
u_char maxRow;

/* control flags */
/* u_char resetScreen;        // flag that is set when the screen must be cleared */
u_char redrawScreen;       // flag that determines if a new draw operation is needed
u_char switchPort;         // flag that determines if the pressed switch is P1 or P2
u_char switches;           // holds the Port 2 switches currently pressed




/* FUNCTIONS */


void msquares_init();                         // initializes miscellaneous items
void switch_init();                           // initializes switches
u_char switch_update_interrupt_sense();       // ensures button interrupt on both press & release
u_char msRand(u_char type);                   // provides either a random font or color
void update_position(u_char col, u_char row); // updates the position of the drawing
void update_shape();                          // updates the shape on LCD screen


#endif /* MSQUARES */
