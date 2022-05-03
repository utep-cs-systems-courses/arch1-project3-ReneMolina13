#include "msquares.h"


void msquaresInit()
{
  /* Fill positions array */
  
  // upper left
  positions[0].col = 10;
  positions[0].row = 10;
  // lower left
  positions[1].col = 10;
  positions[1].row = screenHeight-10;
  // lower right
  positions[2].col = screenWidth-10;
  positions[2].row = screenHeight-10;
  // upper right
  positions[3].col = screenWidth-1;
  positions[3].row = 10;
  // middle
  positions[4].col = screenWidth/2;
  positions[4].row = screenHeight/2;

  /* Fill sqColors array */

  sqColors[0] = COLOR_BLUE;
  sqColors[1] = COLOR_GREEN;
  sqColors[2] = COLOR_ORANGE;
  sqColors[3] = COLOR_BLUE;

  /* Initialize variables */
  
  current_position = 0;
  current_color = 0;
  redrawScreen = 1;
  switches = 0;
}


void switch_init()
{  
  P2REN |= SWITCHES;		/* enables resistors for switches */
  P2IE |= SWITCHES;		/* enable interrupts from switches */
  P2OUT |= SWITCHES;		/* pull-ups for switches */
  P2DIR &= ~SWITCHES;		/* set switches' bits for input */
  switch_update_interrupt_sense();
}


// WARNING: LCD DISPLAY USES P1.0.  Do not touch!!! 

static char switch_update_interrupt_sense()
{
  char p2val = P2IN;
  /* update switch interrupt to detect changes from current buttons */
  P2IES |= (p2val & SWITCHES);	/* if switch up, sense down */
  P2IES &= (p2val | ~SWITCHES);	/* if switch down, sense up */
  return p2val;
}
    
    
void update_shape()
{
  static char last_position = 0, last_color = 0;
  redrawScreen = 0;
  int pos = current_position, color = current_color;

  if (pos == last_position && color == last_color) /* nothing to redraw */
    return;

  /* erase old shape */
  short col = positions[last_position].col;
  short row = positions[last_position].row;
  if (pos != last_position)    /* erase if position changed */
    fillRectangle(col-5, row-5, 10, 10, BG_COLOR); 
  /* draw new shape */
  col = positions[pos].col;
  row = positions[pos].row;
  fillRectangle(col-5, row-5, 10, 10, sqColors[color]); /* draw new shape */
  /* remember color & pos for next redraw */
  last_position = pos;
  last_color = color;
}
