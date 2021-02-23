// 18f45k22

void tris_clear(void){
	TRISA=0b11111111;
	TRISB=0b11111111;
	TRISC=0b11111111;
	TRISD=0b11111111;
	TRISE=0b00000111;
}

void tris_init(void){
	TRISA=0b11111010;
	TRISB=0b11101111;
	TRISC=0b10010010;
	TRISD=0b11101111;
	TRISE=0b00000100;
}

void port_clear(void){
	PORTA=0;
	PORTB=0;
	PORTC=0;
	PORTD=0;
	PORTE=0;
}

void adc_dis(void){
	ADCON0=0;
	ADCON1=0b00000000;
	ADCON2=0;
        ANSELA=0;
        ANSELB=0;
        ANSELC=0;
        ANSELD=0;
        ANSELE=0;

}

void osc_int(void){
	OSCCON=0b01110000;//16Mhz int osc
        OSCCON2=0b10000000;
	OSCTUNE=0b01000000;//PLL EN x4 MHZ
}

void timers_dis(void){
	T0CON=0;// Timer0 dis
	T1CON=0;// Timer1 dis
	T2CON=0;// Timer2 dis
	T3CON=0;// Timer3 dis
}

void interrupt_dis(void){

	INTCON =0;
	INTCON2=0b10000000;//pull up dis
	INTCON3=0;
	PIR1=0;
	PIR2=0;
	PIE1=0;
	PIE2=0;
	IPR1=0;
	IPR2=0;
}

void cpp_dis(void){
	CCP1CON=0b00000000;
	CCP2CON=0b00000000;
	CCP3CON=0b00000000;
        CCP4CON=0b00000000;
        CCP5CON=0b00000000;
}

void mssp_dis(void){
    //PMD0bits.SPI1MD=0;//enrjisini keser
	SSP1STAT=0;
	SSP1CON1=0;
        SSP1CON2=0;
        SSP1CON3=0;
        SSP2STAT=0;
	SSP2CON1=0;
        SSP2CON2=0;
        SSP2CON3=0;

}

void pic_init(void){
	osc_int();
	tris_clear();
	port_clear();
	adc_dis();
	timers_dis();
	interrupt_dis();
	cpp_dis();
	mssp_dis();
}

void timer0_init(){
	T0CON=0b10101000;
	TMR0IE=0;
}

void timer1_init(){
	
	T1CON=0b00000001;
	TMR1IE=1;
}
void timer2_init(void){
	T2CON=0b00011111;//4x16
	TMR2=0;
	PR2=255;
	TMR2IE=1;
}
void timer3_init(){
	T3CON=0b00000001;
	TMR3IE=1;
}

void timer4_init(void){
	T4CON=0b01111111;//4x16
	TMR4=0;
	PR4=100;
}


void cpp_init(void){
	CCPR1H=0;
	CCPR2H=0;
	CCPR1L=0;
	CCPR2L=0;
        CCPTMRS0=0b00001001;
        ECCP1AS=0b00000000;
        ECCP2AS=0b00000000;
        PWM1CON=0b00000000;
        PWM2CON=0b00000000;
	CCP1CON=0b00001100;
	CCP2CON=0b00001100;
}
