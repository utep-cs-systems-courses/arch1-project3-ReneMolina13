#ifndef MSQUARES
#define MSQUARES


#include <msp430.h>
#include <stdlib.h>
#include <time.h>
#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"

#define NUM_POSITIONS 5
#define NUM_SQCOLORS 43
#define NUM_FONTS 3
#define BG_COLOR COLOR_BLACK
#define LED BIT6     // represents P1.6 since P1.0 used for LCD
#define SWITCHES 15  // all Port 2 switches
#define SW_0 8       // P1.3
#define SW_1 1       // P2.0
#define SW_2 2       // P2.1
#define SW_3 4       // P2.2
#define SW_4 8       // P2.3
#define RAND_COLOR 0 // used as a flag for random function
#define RAND_FONT 1  // used as a flag for random function

typedef struct {    // indicates a position on LCD screen
  short col, row;
} Pos;

Pos positions[NUM_POSITIONS];          // holds an array of common LCD position
unsigned short sqColors[NUM_SQCOLORS]; // holds an array of LCD colors
char current_position;                 // holds the current position to be drawn to
char current_color;                    // holds the current color that will be drawn
int redrawScreen;                      // flag that determines if a new draw operation is needed
int switches;                          // holds the switches that are currently pressed
int switchPort;                        // flag that determines if the pressed switch is P1 or P2

void msquaresInit(void);                     // initializes miscellaneous items
static char switch_update_interrupt_sense(); // ensures button interrupt on both press & release
void switch_init();                          // initializes switches
static char random(char type);               // provides either a random font or color
void update_shape();                         // updates the shape on LCD screen


#endif /* MSQUARES */
