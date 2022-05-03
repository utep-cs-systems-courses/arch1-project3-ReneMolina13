#ifndef MSQUARES
#define MSQUARES


#include <msp430.h>
#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"

#define NUM_POSITIONS 5
#define NUM_SQCOLORS 4
#define BG_COLOR COLOR_BLACK
#define LED BIT6		/* note that bit zero req'd for display */
#define SWITCHES 15

typedef struct {
  short col, row;
} Pos;

Pos positions[NUM_POSITIONS];
unsigned short sqColors[NUM_SQCOLORS];
char current_position;
char current_color;
int redrawScreen;
int switches;

void msquaresInit(void);
static char switch_update_interrupt_sense();
void switch_init(); /* setup switch */
void update_shape();


#endif
