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
  
  colors[0] = COLOR_BLUE;
  colors[1] = COLOR_RED;
  colors[2] = COLOR_GREEN;
  colors[3] = COLOR_CYAN;
  colors[4] = COLOR_MAGENTA;
  colors[5] = COLOR_YELLOW;
  colors[6] = COLOR_ORANGE;
  colors[7] = COLOR_ORANGE_RED;
  colors[8] = COLOR_DARK_ORANGE;
  colors[9] = COLOR_GRAY;
  colors[10] = COLOR_NAVY;
  colors[11] = COLOR_ROYAL_BLUE;
  colors[12] = COLOR_SKY_BLUE;
  colors[13] = COLOR_TURQUOISE;
  colors[14] = COLOR_STEEL_BLUE;
  colors[15] = COLOR_LIGHT_BLUE;
  colors[16] = COLOR_AQUAMARINE;
  colors[17] = COLOR_DARK_GREEN;
  colors[18] = COLOR_DARK_OLIVE_GREEN;
  colors[19] = COLOR_SEA_GREEN;
  colors[20] = COLOR_SPRING_GREEN;
  colors[21] = COLOR_PALE_GREEN;
  colors[22] = COLOR_GREEN_YELLOW;
  colors[23] = COLOR_LIME_GREEN;
  colors[24] = COLOR_FOREST_GREEN;
  colors[25] = COLOR_KHAKI;
  colors[26] = COLOR_GOLD;
  colors[27] = COLOR_GOLDENROD;
  colors[28] = COLOR_SIENNA;
  colors[29] = COLOR_BEIGE;
  colors[30] = COLOR_TAN;
  colors[31] = COLOR_BROWN;
  colors[32] = COLOR_CHOCOLATE;
  colors[33] = COLOR_FIREBRICK;
  colors[34] = COLOR_HOT_PINK;
  colors[35] = COLOR_PINK;
  colors[36] = COLOR_DEEP;
  colors[37] = COLOR_VIOLET;
  colors[38] = COLOR_DARK_VIOLE;
  colors[39] = COLOR_PURPLE;
  colors[40] = COLOR_MEDIUM_PURPLE;
  colors[41] = COLOR_BLACK;
  colors[42] = COLOR_WHITE;

  /* Initialize variables */

  // set null-terminator on string
  str[STR_LENGTH-1] = 0;

  // set current/next posions, colors, & fonts
  current_position.col = common_positions[MIDDLE].col;
  current_position.row = common_positions[MIDDLE].row;
  next_position = NO_CHANGE;
  current_font_color = 42; // white
  current_background = 41; // black
  current_font = SMALL_FONT;
  minCol = common_positions[MIDDLE].col;
  minRow = common_positions[MIDDLE].row;
  maxCol = common_positions[MIDDLE].col;
  maxRow = common_positions[MIDDLE].row;
  // set control flags
  clearScreen = 0;
  redrawScreen = 0;
  switchPort = 0;
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

static u_char switch_update_interrupt_sense()
{
  u_char p2val = P2IN;
  
  /* if switch up, sense down */
  P2IES |= (p2val & SWITCHES);
  /* if switch down, sense up */
  P2IES &= (p2val | ~SWITCHES);	

  return p2val;
}


static u_char random(u_char type)
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
  
  switch (nextPosition) {
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
  drawPixel(col, row, colors[current_font_color]);

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
    current_background = random(RAND_COLOR);
    clearScreen(colors[current_background]);
    // reset cursor to random position
    u_char randIndex = random(RAND_POSITION);
    currentPostion.col = common_positions[randIndex].col;
    currentPosition.row = common_positions[randIndex].row;
  }
  
  /* if no position change, determine if interrupt caused by SW_0 */
  else if (nextPosition == NO_CHANGE) {
    // P1.3 buton pressed
    if (switchPort == 1) {
      // randomly change background and font colors
      current_background = random(RAND_COLOR);
      current_font_color = random(RAND_COLOR);
      u_int string_backgound = colors[current_background];
      u_int string_font_color = colors[current_font_color];

      // set a random (common) position for the string
      u_char randIndex = random(RAND_POSITION);
      u_char stringCol = common_positions[randIndex].col;
      u_char stringRow = common_positions[randIndex].row;

      // type random string
      int i;
      u_char c;

      // generate random string
      for (i = 0; i < STR_LENGTH-1; i++)
	str[i] = random(RAND_CHARACTER);
      
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
      drawRectOutline(minCol, minRow, width, height, colors[current_font_color]);
      // raise resetScreen flag (holds drawing for 5 seconds before resetting)
      resetScreen = 1;
    }
  }

  /* otherwise, update current position */
  else
    updatePosition(current_position.col, current_position.row);

  // lower redrawScreen flag
  redrawScreen = 0;
}
