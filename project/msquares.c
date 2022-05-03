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
  positions[3].col = screenWidth-10;
  positions[3].row = 10;
  // middle
  positions[4].col = screenWidth/2;
  positions[4].row = screenHeight/2;

  /* Fill sqColors array */
  
  sqColors[0] = COLOR_BLUE;
  sqColors[1] = COLOR_RED;
  sqColors[2] = COLOR_GREEN;
  sqColors[3] = COLOR_CYAN;
  sqColors[4] = COLOR_MAGENTA;
  sqColors[5] = COLOR_YELLOW;
  sqColors[6] = COLOR_ORANGE;
  sqColors[7] = COLOR_ORANGE_RED;
  sqColors[8] = COLOR_DARK_ORANGE;
  sqColors[9] = COLOR_GRAY;
  sqColors[10] = COLOR_NAVY;
  sqColors[11] = COLOR_ROYAL_BLUE;
  sqColors[12] = COLOR_SKY_BLUE;
  sqColors[13] = COLOR_TURQUOISE;
  sqColors[14] = COLOR_STEEL_BLUE;
  sqColors[15] = COLOR_LIGHT_BLUE;
  sqColors[16] = COLOR_AQUAMARINE;
  sqColors[17] = COLOR_DARK_GREEN;
  sqColors[18] = COLOR_DARK_OLIVE_GREEN;
  sqColors[19] = COLOR_SEA_GREEN;
  sqColors[20] = COLOR_SPRING_GREEN;
  sqColors[21] = COLOR_PALE_GREEN;
  sqColors[22] = COLOR_GREEN_YELLOW;
  sqColors[23] = COLOR_LIME_GREEN;
  sqColors[24] = COLOR_FOREST_GREEN;
  sqColors[25] = COLOR_KHAKI;
  sqColors[26] = COLOR_GOLD;
  sqColors[27] = COLOR_GOLDENROD;
  sqColors[28] = COLOR_SIENNA;
  sqColors[29] = COLOR_BEIGE;
  sqColors[30] = COLOR_TAN;
  sqColors[31] = COLOR_BROWN;
  sqColors[32] = COLOR_CHOCOLATE;
  sqColors[33] = COLOR_FIREBRICK;
  sqColors[34] = COLOR_HOT_PINK;
  sqColors[35] = COLOR_PINK;
  sqColors[36] = COLOR_DEEP;
  sqColors[37] = COLOR_VIOLET;
  sqColors[38] = COLOR_DARK_VIOLE;
  sqColors[39] = COLOR_PURPLE;
  sqColors[40] = COLOR_MEDIUM_PURPLE;
  sqColors[41] = COLOR_BLACK;
  sqColors[42] = COLOR_WHITE;

  /* Initialize variables */
  
  current_position = 0;
  current_color = 0;
  redrawScreen = 1;
  switches = 0;

  /* Seed random function with current time */
  srand(time(NULL));
}


void switch_init()
{
  /* enables resistors for switches */
  P1REN |= SW_0;
  P2REN |= SWITCHES;
  /* enable interrupts from switches */
  P1IE |= SW_0;
  P2IE |= SWITCHES;
  /* pull-ups for switches */
  P1OUT |= SW_0;
  P2OUT |= SWITCHES;
  /* set switches' bits for input */
  P1DIR &= ~SW_0;
  P2DIR &= ~SWITCHES;
  
  switch_update_interrupt_sense();
}


/* update switch interrupt to detect changes from current buttons */

static char switch_update_interrupt_sense()
{
  char p2val = P2IN;
  
  /* if switch up, sense down */
  P1IES |= (P1IN & SW_0);
  P2IES |= (p2val & SWITCHES);
  /* if switch down, sense up */
  P1IES &= (P1IN | ~SW_0);
  P2IES &= (p2val | ~SWITCHES);	

  return p2val;
}


static char random(char type)
{
  int val = rand();
  
  switch (type) {
  case RAND_POSITION:
    val %= NUM_POSITIONS;
    break;
  case RAND_COLOR:
    val %= NUM_SQCOLORS;
    break;
  case RAND_FONT:
    val %= NUM_FONTS;
    break;
  }
  
  return val;
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
    fillRectangle(col-5, row-5, 10, 10, COLOR_BLACK); 
  /* draw new shape */
  col = positions[pos].col;
  row = positions[pos].row;
  fillRectangle(col-5, row-5, 10, 10, sqColors[color]); /* draw new shape */
  /* remember color & pos for next redraw */
  last_position = pos;
  last_color = color;
}
