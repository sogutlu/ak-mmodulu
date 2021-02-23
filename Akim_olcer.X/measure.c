void measure(void){
    static unsigned long hall_adc_av1=0,hall_adc_av2=0;
    static unsigned int akim_i=0;
    long tmp_watt=0;

    if(akim_i){
        hall_adc_av1+=adc[23];
        hall_adc_av2+=adc[21];
    }else{
        akim_i=51;
        hall1_adc=(unsigned int) (hall_adc_av1/50);
        hall2_adc=(unsigned int)(hall_adc_av2/50);
        hall_adc_av2=0;
        hall_adc_av1=0;
    }

    akim_i--;

    vbatt1_adc=adc[22];
    vbatt2_adc=adc[20];

    hall1=(((signed int)hall1_adc-hall1_sum)*0.01*hall1_mul);
    hall2=(((signed int)hall2_adc-hall2_sum)*0.01*hall2_mul);
    vbatt1=(((signed int)vbatt1_adc-vbatt1_sum)*0.01*vbatt1_mul);
    vbatt2=(((signed int)vbatt2_adc-vbatt2_sum)*0.01*vbatt2_mul);
    hiz=(((signed int)hiz_clk-hiz_sum)*0.01*hiz_mul);
    

    tmp_watt=hall1;
    tmp_watt*=vbatt1;
    tmp_watt/=100;
    watt1=tmp_watt;

    tmp_watt=hall2;
    tmp_watt*=vbatt1;
    tmp_watt/=100;
    watt2=tmp_watt;
    
//    watt1=(hall1/10)*(vbatt1/10);
//    watt2=(hall2/10)*(vbatt1/10);

    if(watt1_integ/360000){// watt saat çevrimi
        watt_h1-=(signed int)(watt1_integ/360000);
        watt1_integ%=360000;
        save_EEPROM(&Registers[50],50);
    }

    if(watt2_integ/360000){// watt saat çevrimi
        watt_h2+=(signed int)(watt2_integ/360000);
        watt2_integ%=360000;
        save_EEPROM(&Registers[50],50);
    }




}