#define adc_ort_say 5 //min 2 yap�n birincisi de�erlendirmeye al�nmaz
#define an_port_say 4
#define an_buyuk_port 23

#define ADC_TMRH TMR3H
#define ADC_TMRL TMR3L


unsigned int adc[an_buyuk_port+1];
const unsigned char adc_list[an_port_say]={20,21,22,23}; // Listen Ports
unsigned char adc_all_port_done=0;
unsigned char adcon0;

void adc_init(void){

    VREFCON0=0b10100000;//fix voltage ref
    VREFCON1=0b11101000;//DAC
    VREFCON2=0b00010000;//DAC
	ADCON0=0b00000001;
	ADCON1=0b00001000;
	ADCON2=0b10111110;
        ANSELA=0b00000000;
        ANSELB=0b00000000;
        ANSELC=0b00000000;
        ANSELD=0b00001111;

	ADIE=0;
}

//ADC Timer Interrupt
void ADC_read_tmr(unsigned char tmr_high,unsigned char tmr_low){
ADC_TMRH=tmr_high;TMR3L=tmr_low;//Timer3 Set 65000
ADCON0|=0b00000011;//Start Read from ADC0
adcon0=ADCON0;
adc_all_port_done=0;
}

//ADC Interrupt
void adc_read_all(void){
   static unsigned char adc_i,adc_j;
   static unsigned int adc_tmp=0;
	
   
   if(adc_j)adc_tmp+=ADRESH*256+ADRESL; //birinci �l��m de�erlendirmeye al�nmaz
   adc_j=++adc_j%adc_ort_say;
   
   
   if (!adc_j){
      adc_j=0;
      adc[adc_list[adc_i]]=adc_tmp/(adc_ort_say-1);
      adc_tmp=0;
      adc_i=++adc_i % an_port_say;
      if (!adc_i){adc_all_port_done=1;}
      	ADCON0 &= 0b10000011 ; // change ADC PORT and end adc read
		ADCON0 |= adc_list[adc_i] << 2;
		adcon0=ADCON0;
      return;
   }
}

void read_all_adc()
{
    unsigned int * point;
    unsigned char tmp[2];
    unsigned int ort_i=0;
    long ort=0;

    point=tmp;
    GIE=1;
    ort=0;
    for(ort_i=0;ort_i<adc_ort_say;ort_i++){
        ADCON0=0b01010011;//an20
        while(GODONE);
        tmp[1]=ADRESH;
        tmp[0]=ADRESL;
        ort+=*point;
    }
    adc[20]=ort/adc_ort_say;

    ort=0;
    for(ort_i=0;ort_i<adc_ort_say;ort_i++){
        ADCON0=0b01010111;//an21
        while(GODONE);
        tmp[1]=ADRESH;
        tmp[0]=ADRESL;
        ort+=*point;
    }
    adc[21]=*point;

    ort=0;
    for(ort_i=0;ort_i<adc_ort_say;ort_i++){
        ADCON0=0b01011011;//an22
        while(GODONE);
        tmp[1]=ADRESH;
        tmp[0]=ADRESL;
        ort+=*point;
    }
    adc[22]=*point;

    ort=0;
    for(ort_i=0;ort_i<adc_ort_say;ort_i++){
        ADCON0=0b01011111;//an23
        while(GODONE);
        tmp[1]=ADRESH;
        tmp[0]=ADRESL;
        ort+=*point;
    }
    adc[23]=*point;
    GIE=1;
}
