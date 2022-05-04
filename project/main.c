#include "msquares.h"

void main()
{
  P1DIR |= LED;		/**< Green led on when CPU on */
  P1OUT &= ~LED;
  msquaresInit();
  configureClocks();
  lcd_init();
  switch_init();
  enableWDTInterrupts();      /**< enable periodic interrupt */
  clearScreen(COLOR_BLACK);  

  while (1) {			/* forever */
    if (redrawScreen)
      update_shape();
    P1OUT |= LED;	/* led on (sleep mode) */
    or_sr(0x18);	/**< CPU OFF, GIE ON */
    and_sr(0xf7)        /* GIE off */
    P1OUT &= ~LED;	/* led off */
  }
}


void wdt_c_handler()
{
  /* New State Machine */

  // Change position every 16 tics (15.625 position changes/second)
  // will take roghly 9 seconds to go all the way across long edge
  // will take roughly 7 seconds to go all the way across short edge

  // Change font color every second to a random color (not background color, black, or white)

  static u_int posCount = 0;
  static u_int fontCount = 0;


  if (posCount++ >= 16) {
    // update position
    posCount = 0;
    if (nextPosition != NO_CHANGE)
      redrawScreen = 1;
  }

  if (fontCount++ >= 250) {
    // update font color
    fontCount = 0;
    redrawScreen = 1;
    current_color = random(RAND_COLOR);
  }
}


void switch_c_handler()
{
  u_char p1val = ~P1IN & SW_0;
  u_char p2val = switch_update_interrupt_sense();
  switches = ~p2val & SWITCHES;

  /* New State Machine */

  // If switch 0 pressed: change background to random color, type a random string,
  // place a box around characters, update next font

  // If switch 0 unpressed: clear screen, move current position to one of the default locations

  // If switch 1 pressed: place a pixel, move 1 pixel left (if possible)

  // If switch 1 unpressed: stop moving pixel in that direction

  // If switch 2 pressed: place a pixel, move 1 pixel up (if possible)

  // If switch 2 unpressed: stop moving pixel in that direction

  // If switch 3 pressed: place a pixel, move 1 pixel down (if possible)

  // If switch 3 unpressed: stop moving pixel in that direction

  // If switch 4 pressed: place a pixel, move 1 pixel right (if possible)

  // If switch 4 unpressed: stop moving pixel in that direction

  // Combinations of port 2 switches pressed moves the pixel in the combined direction

  // If all port 2 switches depressed: place a box around design
  
  switch (switches) {
    // left
  case SW_1:
  case (SW_1 | SW_2 | SW_3):
    nextPosition = LEFT;
    break;
    
    // up
  case SW_2:
  case (SW_2 | SW_1 | SW_4):
    nextPosition = UP;
    break;
    
    // down
  case SW_3:
  case (SW_3 | SW_1 | SW_4):
    nextPosition = DOWN;
    break;
    
    // right
  case SW_4:
  case (SW_4 | SW_2 | SW_3):
    nextPosition = RIGHT;
    break;
    
    // left & up
  case (SW_1 | SW_2):
    nextPosition = LEFT_UP;
    break;
    
    // left & down
  case (SW_1 | SW_3):
    nextPosition = LEFT_DOWN;
    break;
    
    // right & up
  case (SW_4 | SW_2):
    nextPosition = RIGHT_UP;
    break;
    
    // right & down
  case (SW_4 | SW_3):
    nextPosition = RIGHT_DOWN;
    break;
    
    // drawing complete
  case 0:
    if (switchPort == 1) {
      // P1.3 buton pressed: change background to random color, type random string,
      // place a box around the string, update next font size
      redrawScreen = 1;
      nextPosition = NO_CHANGE;
    }

    else if (switchPort == 2) {
      // all buttons released; make a box around work done
      redrawScreen = 1;
      nextPosition = NO_CHANGE;
    }
    break;
    
    // conflicting buttons pressed -> no movement
  default:
    nextPosition = NO_CHANGE;
  }  
}
