#include <msp430.h>

int main(void)
{
  WDTCTL = WDTPW | WDTHOLD;                 // Stop WDT

  // GPIO Setup
  P1OUT = 0;
  P1DIR = BIT0;                             // For LED

  // Set up XT1
  PJSEL0 = BIT4 | BIT5;                     // For XT1

  // Clock System Setup
  CSCTL0_H = CSKEY >> 8;                    // Unlock CS registers
  CSCTL1 = DCOFSEL_0;                       // Set DCO to 1MHz
  CSCTL2 = SELA__LFXTCLK | SELS__DCOCLK | SELM__DCOCLK;
  CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;     // Set all dividers
  CSCTL4 &= ~LFXTOFF;

  RTCCTL0_H = RTCKEY_H;                     // Unlock RTC key protected registers
  RTCCTL0_L = RTCRDYIE;					    // Real-time clock ready interrupt enable
  RTCCTL1 = RTCMODE;                        // to trigger interrupt every second
  RTCCTL1 &= ~RTCHOLD;                      // Start RTC calendar mode
  RTCCTL0_H = 0;                            // Lock RTC key protected registers

  // Disable the GPIO power-on default high-impedance mode to activate
  // previously configured port settings
  PM5CTL0 &= ~LOCKLPM5;

  for (;;) {
    __bis_SR_register(LPM3_bits + GIE);       // Enter LPM3, enable interrupts
  }
}

// RTC Interrupt Service Routine
#pragma vector=RTC_VECTOR
__interrupt void RTC_ISR(void)
{
  switch (__even_in_range(RTCIV, RTCIV_RT1PSIFG))
  {
    case RTCIV_NONE:      break;            // Vector  0:  No pending interrupt
    case RTCIV_RTCOFIFG:  break;			// Vector  2:  Oscillator fault
    case RTCIV_RTCRDYIFG:                   // Vector  4:  RTC Ready
      P1OUT ^= BIT0;                        // Toggle P1.0
      break;
    case RTCIV_RTCTEVIFG: break;            // Vector  6:  RTC interval timer
    case RTCIV_RTCAIFG:   break;            // Vector  8:  RTC user alarm
    case RTCIV_RT0PSIFG:  break;            // Vector  10:  RTC prescaler 0
    case RTCIV_RT1PSIFG:  break;            // Vector  12:  RTC prescaler 1
    default: break;
  }
}

