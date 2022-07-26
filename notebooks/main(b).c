#include <msp430.h> 
#define uint unsigned int
#define delay_us(x) __delay_cycles((unsigned long)(4*x))
#define delay_ms(x) __delay_cycles((unsigned long)(4000*x))
#define delay_s(x) __delay_cycles((unsigned long)(4000000*x))


void Motor(int p){
    /* P1.4(TA0.3): Left side PWM Output
     * P1.5(TA0.4): Right side PWM Output
     * P4.0: L1
     * P4.1: L2
     * P4.2: L3
     * P4.3: L4 */

    //init PWM port
    P1DIR |= BIT4+BIT5;
    P1SEL |= BIT4+BIT5;
    P1OUT |= BIT4+BIT5;
    TA0CCR0 = 512-1;
    TA0CCTL3 = OUTMOD_7;
    TA0CCTL4 = OUTMOD_7;

    //init Lx port
    P4DIR |= BIT0+BIT1+BIT2+BIT3;

    //Set Lx=[1,0,1,0]
    P4OUT |= BIT0;
    P4OUT &= ~BIT1;
    P4OUT |= BIT2;
    P4OUT &= ~BIT3;

    //set duty cycle and start PWM output
    TA0CCR3 = 400;
    TA0CCR4 = 400;
    TA0CTL = TASSEL_2 + MC_1 + TACLR;
    delay_s(0.2);
    TA0CCR3 = p;
    TA0CCR4 = p;
}

int Speed(int time){
    if (time==0) return 150;
    return (2 - time/10)*500;
}

void PORT_Interrupt_Init(){
    /* P1.2-6(!): Front IR Sensor Input
     * P2.2-3,4-6(!): Back IR Sensor Input*/

    // Enable internal resistance
    //P1REN |= BIT2+BIT3+BIT4+BIT5+BIT6;
    P2REN |= BIT0+BIT2+BIT3+BIT4+BIT5+BIT6;

    // Set as pull-Up resistance
    //P1OUT |= BIT2+BIT3+BIT4+BIT5+BIT6;
    P2OUT |= BIT0+BIT2+BIT3+BIT4+BIT5+BIT6;

    // Set trigger as High -> Low
    //P1IES |= BIT2+BIT3+BIT4+BIT5+BIT6;
    P2IES |= BIT0+BIT2+BIT3+BIT4+BIT5+BIT6;

    // Clear port interrupt flag(IFG)
    //P1IFG &= ~BIT2;P4OUT &= ~BIT3;P4OUT &= ~BIT4;P4OUT &= ~BIT5;P4OUT &= ~BIT6;
    P2IFG = 0x00;

    // Enable port interrupt
    //P1IE |= BIT2+BIT3+BIT4+BIT5+BIT6;
    P2IE |= BIT0+BIT2+BIT3+BIT4+BIT5+BIT6;

    // Enable Global Interrupt
    __enable_interrupt();
}

#pragma vector=PORT2_VECTOR
__interrupt void Back_Sensor(void){
    switch(P2IFG & 0x7D){
        case 0x8: TA0CCR3 = 100;TA0CCR4 = 275;P2IFG=0x00;break; //001000
        case 0xC: TA0CCR3 = 100;TA0CCR4 = 275;P2IFG=0x00;break; //011000
        case 0x4: TA0CCR3 = 50;TA0CCR4 = 300;P2IFG=0x00;break; //010000
        case 0x5: TA0CCR3 = 50;TA0CCR4 = 300;P2IFG=0x00;break; //110000
        case 0x1: TA0CCR3 = 0;TA0CCR4 = 350;P2IFG=0x00;break; //100000

        case 0x10: TA0CCR3 = 275;TA0CCR4 = 100;P2IFG=0x00;break; //000100
        case 0x30: TA0CCR3 = 275;TA0CCR4 = 100;P2IFG=0x00;break; //000110
        case 0x20: TA0CCR3 = 300;TA0CCR4 = 50;P2IFG=0x00;break; //000010
        case 0x60: TA0CCR3 = 300;TA0CCR4 = 50;P2IFG=0x00;break; //000011
        case 0x40: TA0CCR3 = 350;TA0CCR4 = 0;P2IFG=0x00;break; //000001
        case 0x00: TA0CCR3 = 150;TA0CCR4 = 150;P2IFG=0x00;break;

        //case 0x18:case 0x3C:case 0x7D:case 0x1C:case 0x38:
            //P2IE = 0x00;
            //P1OUT = 0x00;
            //P2IFG = 0x00;
            //__disable_interrupt();
            //P8OUT |= BIT1;delay_s(1);P8OUT &= ~BIT1;
            //break;
        default: P4OUT = 0x00; //001100, 011110, 111111, 011100, 001110
            TA0CCR3 = 0;TA0CCR4 = 0;
            __disable_interrupt();break;//TA0CCR3 = 200;TA0CCR4 = 200;P2IFG=0x00;break;
    }
}

void main(void){
    WDTCTL = WDTPW + WDTHOLD;
    P8DIR |= BIT1;
    P8OUT &= ~BIT1;
    P2DIR &= ~(BIT0+BIT2+BIT3+BIT4+BIT5+BIT6);
    //P2IN = 0x00;
    Motor(Speed(0));
    PORT_Interrupt_Init();
}
