#include <msp430.h>
#include "oled.h"
#include "type.h"
#include "stdint.h"
#include <stdio.h>

volatile unsigned int counter;

void Motor(){
    /* P1.4(TA0.3): Left side PWM Output
     * P1.5(TA0.4): Right side PWM Output
     * P3.5: IN1
     * P3.6: IN2
     * P3.7: IN3
     * P8.8: IN4 */

    //init PWM port
    P2DIR |= BIT0+BIT5;                       // P1.2 and P1.3 output
    P2SEL |= BIT0+BIT5;                       // P1.2 and P1.3 options select

    TA1CCR0 = 512 - 1;
    TA1CCTL1 = OUTMOD_7;
    TA1CCR1 = 384;
    TA1CTL = TASSEL_2 + MC_1 + TACLR ;

    TA2CCR0 = 512 - 1;
    TA2CCTL2 = OUTMOD_7;
    TA2CCR2 = 128;
    TA2CTL = TASSEL_2 + MC_1 + TACLR ;

    //init Lx port
    P3DIR |= BIT5+BIT6+BIT7;
    P8DIR |= BIT2;

    //Set Lx=[1,0,1,0]
    P3OUT |= BIT6 + BIT7;
}

//void PORT_Interrupt_Init(){
//    /* P1.2-6(!): Front IR Sensor Input
//     * P2.2-3,4-6(!): Back IR Sensor Input*/
//
//    // Enable internal resistance
//    //P1REN |= BIT2+BIT3+BIT4+BIT5+BIT6;
//    P2REN |= BIT2+BIT3+BIT4+BIT5+BIT6;
//
//    // Set as pull-Up resistance
//    //P1OUT |= BIT2+BIT3+BIT4+BIT5+BIT6;
//    P2OUT |= BIT2+BIT3+BIT4+BIT5+BIT6;
//
//    // Set trigger as High -> Low
//    //P1IES |= BIT2+BIT3+BIT4+BIT5+BIT6;
//    P2IES |= BIT2+BIT3+BIT4+BIT5+BIT6;
//
//    // Clear port interrupt flag(IFG)
//    //P1IFG &= ~BIT2;P4OUT &= ~BIT3;P4OUT &= ~BIT4;P4OUT &= ~BIT5;P4OUT &= ~BIT6;
//    P2IFG = 0x00;
//
//    // Enable port interrupt
//    //P1IE |= BIT2+BIT3+BIT4+BIT5+BIT6;
//    P2IE |= BIT2+BIT3+BIT4+BIT5+BIT6;
//
//    // Enable Global Interrupt
//    __enable_interrupt();
//}
//
//
//#pragma vector=PORT2_VECTOR
//__interrupt void P2_ISR(void){
//    switch(P2IFG & 0x7C){
//        case 0x30: TA0CCR3 = 125-25;TA0CCR4 = 175-25;P2IFG = 0x00;break;
//        case 0x20: TA0CCR3 = 100-25;TA0CCR4 = 200-25;P2IFG = 0x00;break;
//        case 0x60: TA0CCR3 = 75-25;TA0CCR4 = 225-25;P2IFG = 0x00;break;
//        case 0x40: TA0CCR3 = 50-25;TA0CCR4 = 250-25;P2IFG = 0x00;break;
//
//        case 0x18: TA0CCR3 = 175-25;TA0CCR4 = 125-25;P2IFG = 0x00;break;
//        case 0x8: TA0CCR3 = 200-25;TA0CCR4 = 100-25;P2IFG = 0x00;break;
//        case 0xC: TA0CCR3 = 225-25;TA0CCR4 = 75-25;P2IFG = 0x00;break;
//        case 0x4: TA0CCR3 = 250-25;TA0CCR4 = 50-25;P2IFG = 0x00;break;
//        default: TA0CCR3 = 150;TA0CCR4 = 150;break;
//   }
//    if ((P2IN==0x0)||(P2IN==0x44)){
//                    TA0CCR3 = 0;TA0CCR4 = 0;P4OUT = 0x00;
//                }
//}
//
//
//void main(void){
//    WDTCTL = WDTPW + WDTHOLD;
//    P8DIR |= BIT1;
//    P8OUT &= ~BIT1;
//
//    Motor(Speed(0));
//
//    PORT_Interrupt_Init();
//}


int main(void){
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer
    OLED_Init();
    OLED_Clear();
    OLED_ShowString(7,3,"freq",8);

    Motor();

//    TA1CCTL0 = CCIE;
//    TA1CCR0 = 50000;
//    TA1CTL = TASSEL_2 + MC_2 + TACLR;
    CBCTL0 |= CBIMEN + CBIPEN;
    CBCTL1 |= CBON;
    CBCTL2 |= CBRSEL + CBREF0_0 + CBREF1_0;
    P6DIR = BIT0+BIT1+BIT2+BIT3;
    P6SEL = BIT0+BIT1+BIT2+BIT3;
    P7DIR = BIT0;
    P7SEL = BIT0;

    P1DIR |= BIT0;

    // P1.2/TA0.1
    P1DIR |= BIT2;
    P1SEL |= BIT2;

//    // TA0CCR1 Capture mode; CCI1A; Both Rising and Falling Edge; interrupt enable
//    TA0CCTL1 |= CM_1 + CCIS_0 + SCS + CAP + CCIE;
//
//    // SMCLK, Cont Mode; start timer
//    TA0CTL |= TASSEL_2 + MC_2 + TACLR;
     __bis_SR_register(GIE);
    __no_operation();

}


#pragma vector = TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR (void){
  switch(__even_in_range(TA0IV,0x0E)){
      case TA0IV_NONE: break;
      case TA0IV_TACCR1:
          counter = TA0CCR1;
          OLED_ShowVI(50-1,3,counter,8);
          if (TA0CCTL1 & COV) TA0CCTL1 &= ~COV;
          break;
      case TA0IV_TACCR2: break;
      case TA0IV_TACCR3: break;
      case TA0IV_TACCR4: break;
      case TA0IV_5: break;
      case TA0IV_6: break;
      case TA0IV_TAIFG: break;
      default: break;
  }
}

//#pragma vector=TIMER1_A0_VECTOR
//__interrupt void TIMER1_A0_ISR(void){
//    CBCTL0 |= CBIMSEL_0 + CBIPSEL_1;
//    CBCTL3 |= BIT0 + BIT1;
////    CBCTL0 |= CBIMSEL_2 + CBIPSEL_3;
////    CBCTL3 |= BIT2 + BIT3;
////    CBCTL0 |= CBIMSEL_4 + CBIPSEL_8;
////    CBCTL3 |= BIT4 + BIT8;
////    CBCTL0 |= CBIMSEL_5 + CBIPSEL_6;
////    CBCTL3 |= BIT5 + BIT6;
//    if (CBOUT) P1OUT ^= BIT0;
//    else P1OUT |= BIT0;
//}

