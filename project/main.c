#include "msquares.h"

void main()
{
  msquaresInit();
  P1DIR |= LED;		/**< Green led on when CPU on */
  P1OUT &= ~LED;
  configureClocks();
  lcd_init();
  switch_init();
  enableWDTInterrupts();      /**< enable periodic interrupt */
  clearScreen(BG_COLOR);  

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
  static int sec2Count = 0;
  static int sec1Count = 0;


  /* New State Machine */

  // Change position every 16 tics (15.625 position changes/second)
  // will take roghly 9 seconds to go all the way across long edge
  // will take roughly 7 seconds to go all the way across short edge

  // Change font color every second to a random color (not background color, black, or white)

  

  
  if (sec2Count++ >= 125) {		/* 2/sec */
    sec2Count = 0;
    current_color = (current_color+1) % NUM_SQCOLORS;
    redrawScreen = 1;
  }
  if (sec1Count++ >= 250) {		/* 1/sec */
    sec1Count = 0;
    current_position = (current_position+1) % NUM_POSITIONS;
    redrawScreen = 1;
  }
}


void switch_c_handler()
{
  char p2val = switch_update_interrupt_sense();
  switches = ~p2val & SWITCHES;

  /* New State Machine */

  // If switch 0 pressed: change background to black and font to white
  // also type a random string, place a box around characters, update next font

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


  

  if (switches & SWITCHES) { 	/* a switch is depresssed */
    redrawScreen = 1;
    for (char swNum = 0; swNum < 4; swNum++) { /* respond to lowest button pressed */
      int swFlag = 1 << swNum;
      if (switches & swFlag) {
	current_position = swNum;
	break;
      }
    }
  }
}
