#include "msquares.h"


void msquares_init()
{
  /* Fill positions array */
  
  // upper left (index 0)
  common_positions[UPPER_LEFT].col = 10;
  common_positions[UPPER_LEFT].row = 10;
  // lower left (index 1)
  common_positions[LOWER_LEFT].col = 10;
  common_positions[LOWER_LEFT].row = screenHeight-10;
  // lower right (index 2)
  common_positions[LOWER_RIGHT].col = screenWidth-10;
  common_positions[LOWER_RIGHT].row = screenHeight-10;
  // upper right (index 3)
  common_positions[UPPER_RIGHT].col = screenWidth-10;
  common_positions[UPPER_RIGHT].row = 10;
  // middle (index 4)
  common_positions[MIDDLE].col = screenWidth/2;
  common_positions[MIDDLE].row = screenHeight/2;

  /* Fill colors array (macros are BGR) */
  
  msColors[0] = COLOR_BLUE;
  msColors[1] = COLOR_RED;
  msColors[2] = COLOR_GREEN;
  msColors[3] = COLOR_CYAN;
  msColors[4] = COLOR_MAGENTA;
  msColors[5] = COLOR_YELLOW;
  msColors[6] = COLOR_ORANGE;
  msColors[7] = COLOR_ORANGE_RED;
  msColors[8] = COLOR_DARK_ORANGE;
  msColors[9] = COLOR_GRAY;
  msColors[10] = COLOR_NAVY;
  msColors[11] = COLOR_ROYAL_BLUE;
  msColors[12] = COLOR_SKY_BLUE;
  msColors[13] = COLOR_TURQUOISE;
  msColors[14] = COLOR_STEEL_BLUE;
  msColors[15] = COLOR_LIGHT_BLUE;
  msColors[16] = COLOR_AQUAMARINE;
  msColors[17] = COLOR_DARK_GREEN;
  msColors[18] = COLOR_DARK_OLIVE_GREEN;
  msColors[19] = COLOR_SEA_GREEN;
  msColors[20] = COLOR_SPRING_GREEN;
  msColors[21] = COLOR_PALE_GREEN;
  msColors[22] = COLOR_GREEN_YELLOW;
  msColors[23] = COLOR_LIME_GREEN;
  msColors[24] = COLOR_FOREST_GREEN;
  msColors[25] = COLOR_KHAKI;
  msColors[26] = COLOR_GOLD;
  msColors[27] = COLOR_GOLDENROD;
  msColors[28] = COLOR_SIENNA;
  msColors[29] = COLOR_BEIGE;
  msColors[30] = COLOR_TAN;
  msColors[31] = COLOR_BROWN;
  msColors[32] = COLOR_CHOCOLATE;
  msColors[33] = COLOR_FIREBRICK;
  msColors[34] = COLOR_HOT_PINK;
  msColors[35] = COLOR_PINK;
  msColors[36] = COLOR_DEEP;
  msColors[37] = COLOR_VIOLET;
  msColors[38] = COLOR_DARK_VIOLE;
  msColors[39] = COLOR_PURPLE;
  msColors[40] = COLOR_MEDIUM_PURPLE;
  msColors[41] = COLOR_BLACK;
  msColors[42] = COLOR_WHITE;

  /* Initialize variables */

  // set null-terminator on string
  str[STR_LENGTH-1] = 0;

  // set current/next posions, colors, & fonts
  current_position.col = common_positions[MIDDLE].col;
  current_position.row = common_positions[MIDDLE].row;
  next_position = NO_CHANGE;
  current_font_color = 42; // white
  current_background = 41; // black
  minCol = common_positions[MIDDLE].col;
  minRow = common_positions[MIDDLE].row;
  maxCol = common_positions[MIDDLE].col;
  maxRow = common_positions[MIDDLE].row;
  // set control flags
  resetScreen = 0;
  redrawScreen = 0;
  switchPort = 0;
  switches = 0;
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
  /* set interrupt sense on press */
  P1IES |= SW_0;
  P2IES |= SWITCHES;
  /* set switches' bits for input */
  P1DIR &= ~SW_0;
  P2DIR &= ~SWITCHES;
}


/* update switch interrupt to detect changes from current buttons */

u_char switch_update_interrupt_sense()
{
  u_char p2val = P2IN;
  
  /* if switch up, sense down */
  P2IES |= (p2val & SWITCHES);
  /* if switch down, sense up */
  P2IES &= (p2val | ~SWITCHES);	

  return p2val;
}


u_char msRand(u_char type)
{
  u_char val;
  
  switch (type) {
  case RAND_POSITION:
    val = rand() % NUM_POSITIONS;
    break;
  case RAND_COLOR:
    val = rand() % NUM_COLORS;
    break;
  case RAND_CHARACTER:
    val = rand() % NUM_CHARACTERS;
    val += 65; // brings val=0 to 'A'
    if (val > 90)
      val += 6; // if not uppercase, make lowercase
  }
  
  return val;
}

void update_position(u_char col, u_char row)
{
  /* Update current position*/
  
  switch (next_position) {
  case LEFT:
    if (col >= 10)
      col--;
    break;
    
  case UP:
    if (row >= 10)
      row--;
    break;
    
  case DOWN:
    if (row <= screenHeight-10)
      row++;
    break;
    
  case RIGHT:
    if (col <= screenWidth-10)
      col++;
    break;
    
  case LEFT_UP:
    if (col >= 10)
      col--;
    if (row >= 10)
      row--;    
    break;
    
  case LEFT_DOWN:
    if (col >= 10)
      col--;
    if (row <= screenHeight-10)
      row++;
    break;
    
  case RIGHT_UP:
    if (col <= screenWidth-10)
      col++;
    if (row >= 10)
      row--;
    break;
    
  case RIGHT_DOWN:
    if (col <= screenWidth-10)
      col++;
    if (row <= screenHeight-10)
      row++;
    break;
    
  case NO_CHANGE:
    return;
  }

  // compare to min & max values
  if (row > maxRow)
    maxRow = row;
  else if (row < minRow)
    minRow = row;
  if (col > maxCol)
    maxCol = col;
  else if (col < minCol)
    minCol = col;
  
  // draw pixel at current position
  drawPixel(col, row, msColors[current_font_color]);

  // update current position global variables
  current_position.col = col;
  current_position.row = row;
}


void update_shape()
{
  /* check if screen must be cleared */
  if (resetScreen == 1) {
    // lower resetScreen flag
    resetScreen = 0;
    // clear drawing and set random background
    current_background = msRand(RAND_COLOR);
    clearScreen(msColors[current_background]);
    // reset cursor to random position
    u_char randIndex = msRand(RAND_POSITION);
    current_position.col = common_positions[randIndex].col;
    current_position.row = common_positions[randIndex].row;
  }
  
  /* if no position change, determine if interrupt caused by SW_0 */
  else if (next_position == NO_CHANGE) {
    // P1.3 buton pressed
    if (switchPort == 1) {
      // randomly change background and font colors
      current_background = msRand(RAND_COLOR);
      current_font_color = msRand(RAND_COLOR);
      u_int string_background = msColors[current_background];
      u_int string_font_color = msColors[current_font_color];

      // set a random (common) position for the string
      u_char randIndex = msRand(RAND_POSITION);
      u_char stringCol = common_positions[randIndex].col;
      u_char stringRow = common_positions[randIndex].row;

      // type random string
      int i;
      u_char c;

      // generate random string
      for (i = 0; i < STR_LENGTH-1; i++)
	str[i] = msRand(RAND_CHARACTER);
      
      // print random string
      for (i = 0; i < STR_LENGTH; i++) {
	// ensure there is room for the character
	if (stringCol + WIDTH_5x7 > screenWidth-10) {
	  stringCol = 10;
	  stringRow += (HEIGHT_5x7)+1;
	}
	if (stringRow + HEIGHT_5x7 > screenHeight-10) {
	  stringCol = 10;
	  stringRow = 10;
	}
	// print character and move to next character location
	drawChar5x7(stringCol, stringRow, str[i], string_font_color, string_background);
	stringCol += (WIDTH_5x7)+1;
	stringRow += (HEIGHT_5x7)+1;
      }

      // raise resetScreen flag (holds drawing for 5 seconds before resetting)
      resetScreen = 1;
    }

    // all buttons released
    else if (switchPort == 2) {
      // make a box around work done
      maxRow += 10;
      maxCol += 10;
      minRow -= 10;
      minCol -= 10;
      u_char width = maxRow - minRow;
      u_char height = maxCol - minCol;
      drawRectOutline(minCol, minRow, width, height, msColors[current_font_color]);
      // raise resetScreen flag (holds drawing for 5 seconds before resetting)
      resetScreen = 1;
    }
  }

  /* otherwise, update current position */
  else
    update_position(current_position.col, current_position.row);

  // lower redrawScreen flag
  redrawScreen = 0;
}
