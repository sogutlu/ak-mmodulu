#include <htc.h>

#pragma config FOSC = INTIO67
#pragma config PLLCFG = ON
#pragma config PRICLKEN = OFF
#pragma config FCMEN = OFF
#pragma config IESO = OFF
#pragma config PWRTEN = ON
#pragma config BOREN = OFF
#pragma config BORV = 250
#pragma config WDTEN = OFF
#pragma config WDTPS = 512
#pragma config CCP2MX = PORTC1
#pragma config PBADEN = OFF
#pragma config HFOFST = OFF
#pragma config T3CMX = PORTC0
#pragma config P2BMX = PORTD2
#pragma config MCLRE = INTMCLR
#pragma config STVREN = OFF
#pragma config LVP = OFF
#pragma config XINST = OFF
#pragma config DEBUG = OFF
#pragma config CP0 = ON //CODE PROTECT
#pragma config CP1 = ON
#pragma config CP2 = ON
#pragma config CP3 = ON
#pragma config CPB = ON
#pragma config CPD = ON
#pragma config WRT0 = OFF // WRITE PROTECT
#pragma config WRT1 = OFF
#pragma config WRT2 = OFF
#pragma config WRT3 = OFF
#pragma config WRTC = OFF
#pragma config WRTB = OFF
#pragma config WRTD = OFF
#pragma config EBTR0 = OFF //TABLE READ PROTECT
#pragma config EBTR1 = OFF
#pragma config EBTR2 = OFF
#pragma config EBTR3 = OFF
#pragma config EBTRB = OFF


__EEPROM_DATA(0x00,0x01,0x03,0xD0,0x00,0x06,0x00,0x00);//50-53
__EEPROM_DATA(0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00);//54-57
__EEPROM_DATA(0x00,0x00,0x00,0x00,0x01,0xFC,0x00,0xB4);//58-61
__EEPROM_DATA(0x01,0xFB,0x00,0xB4,0x00,0x00,0x00,0xCD);//62-65
__EEPROM_DATA(0x00,0x00,0x00,0xCD,0x00,0x00,0x06,0x13);//66-69
__EEPROM_DATA(0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00);//70-73
__EEPROM_DATA(0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00);//74-77
__EEPROM_DATA(0x00,0x00,0x00,0x0F,0x00,0x00,0x00,0x00);//78-81
__EEPROM_DATA(0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00);//82-85
__EEPROM_DATA(0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00);//86-89


unsigned int timer_tmp=0,rf_send_timer=0,rf_recive_timer=0,rf_recive_done_timer=0,rf_all_send_timer=0,rf_reset_timer=0,rf_init_timer=0;


#include "pic_init.c"
#include "adc_lib.c"
#include "defines.c"
#include "mod.c"
#include "delay.c"
#include "eeprom.c"
#include "measure.c"

#include "NRF2401.c"
#include "spi.c"

void tmr2(void){// zamanlama için kullanılıyor
	modbus_get_routine(50); // 10ms modbus kontrol
}


void tmr1(void){
    static unsigned int timer_s=0,tmr_hiz=0,blink_tmr=0;
    static unsigned char blink=0;

    TMR1L=0x7F;
    TMR1H=0xC1;

    if(!tmr_hiz){
        hiz_clk=TMR0L;
        TMR0L=0;
        tmr_hiz=250;
    }

    if(!blink_tmr){

        blink^=1;

        if(l_in){
            l_out=blink;
        }else{
            l_out=0;
        }

        if(r_in){
            r_out=blink;
        }else{
            r_out=0;
        }
        blink_tmr=500;
    }    
    

    if(!timer_s){
        timer_s=1000;
        
        watt1_integ+= (signed long)hall1 * (signed long)vbatt1;
        watt2_integ+= (signed long)hall2 * (signed long)vbatt1;
        led^=1;
        
    }

    if(timer_tmp)timer_tmp--;
    if(rf_send_timer)rf_send_timer--;
    if(rf_recive_timer)rf_recive_timer--;
    if(rf_recive_done_timer)rf_recive_done_timer--;
    if(rf_reset_timer)rf_reset_timer--;
    if(rf_init_timer)rf_init_timer--;
    timer_s--;
    tmr_hiz--;
    blink_tmr--;
    
}

static void interrupt
inter(void){
	if (TMR2IF){
		tmr2();
		TMR2IF=0;
	}
	if (TMR1IF){
		tmr1();
		TMR1IF=0;
	}
        if(TMR3IF){
		//ADC_read_tmr(0xCE,0x5F);//
		TMR3IF=0;
	}

	if(ADIF){
		//adc_read_all();
		ADIF=0;
	}
	if(RCIF){
		get_data();
		RCIF=0;
	}

//	if(PIR1bits.TX1IF && TXSTA1bits.TXEN && TXSTA1bits.TRMT){
//		send_data();
//	}
}


void main(void){
    unsigned int i, rf_send_i;
pic_init();
GIE=1;
PEIE=1;
timer2_init();
timer1_init();
timer3_init();
timer0_init();
tris_init();
adc_init();
load_EEPROM(&Registers[50],50);
modbus_slave_rtu_init(modbus_adress);

    SPI_Init();
    receive_init();
    rf_adress=6;
    change_channel(rf_adress);
    rf_reset_timer=2000;
	while(1){
		versiyon=614;//Program versiyonu
        rf_adress=6;
                read_all_adc();
                measure();
                rx_routine();
                tx_routine();
                if(save_reg){
                
                    save_EEPROM(&Registers[50],50);
                    save_reg=0;
                
                }
                if(!reset_capasite){

                    watt_h1=batt_max_capasite;
                    watt_h2=0;
                    save_reg=1;

                }


                if(!rf_recive_timer){// önce rf recive kontrolü

                rf_recive_timer=50;
                rf_send_timer=20;
                if(receive_check() && !rf_recive_done_timer){
                    Registers[rf_command_reg_id]=rf_command_reg_val;
                    if(rf_command_reg_id>49 && rf_command_reg_id<100)save_reg=1;
                    if(rf_command_reg_id==52)change_channel(rf_adress);// rf kanal? de?i?ikli?i gelmi?se de?i?tirir.
                    rf_recive_done_timer=2000;
                    //rf_command_reg_id=0;
                }



            }

//                if(!rf_send_timer){// recive kontrolünden sonra gönderim
//                    rf_send_timer=200;// bi anlam? yok güvenlik icin
//                    spi_transmit(rf_send_i);
//                    rf_send_i+=15;
//
//                    if(rf_send_type==0){
//                        //if(rf_send_i>15)rf_send_i=0;// 15 in kat? olsun
//                        rf_send_i=0;
//                        rf_all_send_timer=30000;
//
//                    }else{
//                        if(rf_send_i>90)rf_send_i=0;// 15 in kat? olsun
//                        if(!rf_all_send_timer)rf_send_type=0;// belirli süre sonra h?zl? gönderime dön.
//                    }
//                    //rf_recive_timer=1;
//                }

                if(!rf_send_timer){// recive kontrolünden sonra gönderim
                    rf_send_timer=3000;// bi anlam? yok güvenlik icin
                    if(!rf_send_type){
                        spi_transmit(rf_send_i);// arada bir kalan register gönder
                        rf_send_i+=15;
                    }else{
                        spi_transmit(0);// asl? registerlar? s?k gönder
                    }

                    if(rf_send_i>90)rf_send_i=0;// 15 in kat? olsun
                    rf_send_type++;
                    rf_send_type%=5;//belirli gönderiden biri geri plan registerlar? olsun
                }

                rf_adress_report=rf_adress;
                watt_h1_report=watt_h1;
                watt_h2_report=watt_h2;

                if(!rf_reset_pin){// rf reset algortm
                    if(!rf_reset_timer){
                        rf_adress=6;
                        change_channel(rf_adress);
                        change_channel(rf_adress);
                        save_reg=1;
                    }
                }else{
                    rf_reset_timer=2000;
                }
                    
                if(!rf_init_timer){
                    receive_init();
                    change_channel(rf_adress);
                    rf_init_timer=3000;
                }
                
                
	}

}
