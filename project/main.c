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
  /* New State Machine */

  // Change position every 16 tics (15.625 position changes/second)
  // will take roghly 9 seconds to go all the way across long edge
  // will take roughly 7 seconds to go all the way across short edge

  // Change font color every second to a random color (not background color, black, or white)

  static int posCount = 0;
  static int fontCount = 0;


  if (posCount++ >= 16) {
    // update position
    posCount = 0;
    redrawScreen = 1;
  }

  if (fontCount++ >= 250) {
    // update font color
    fontCount = 0;
    redrawScreen = 1;
  }




  
  

   static int sec2Count = 0;
   static int sec1Count = 0;
   
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
  char p1val = ~P1IN & SW_0;
  char p2val = switch_update_interrupt_sense();
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

  redrawScreen = 1;
  
  switch (switches) {
    // left
  case SW_1:
  case (SW_1 | SW_2 | SW_3):
    break;
    
    // up
  case SW_2:
  case (SW_2 | SW_1 | SW_4):
    break;
    
    // down
  case SW_3:
  case (SW_3 | SW_1 | SW_4):
    break;
    
    // right
  case SW_4:
  case (SW_4 | SW_2 | SW_3):
    break;
    
    // left & up
  case (SW_1 | SW_2):
    break;
    
    // left & down
  case (SW_1 | SW_3):
    break;
    
    // right & up
  case (SW_4 | SW_2):
    break;
    
    // right & down
  case (SW_4 | SW_3):
    break;
    
    // drawing complete / draw string
  case 0:
    if (switchCase == 1) {
      // P1.3 buton pressed: change background to random color, type random string,
      // place a box around the string, update next font size
      
    }

    else if (switchCase == 2) {
      // all buttons released; make a box around work done
      
    }
    break;
    
    // conflicting buttons pressed - no movement
  default:
    redrawScreen = 0;
  }
  








  
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
