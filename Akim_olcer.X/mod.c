#define inp_get_count 100 
#define Register_Count 200
#define enable_pin LATDbits.LATD4
#define modbus_holding_reg Registers





unsigned char bank1 inp_get_tmp[inp_get_count],out_send_tmp[inp_get_count];
//unsigned char modbus_adress=0;
unsigned char modbus_time_over=0;//if time over
unsigned char modbus_get_start=0;//if get start
unsigned char send_count=0;
unsigned char modbus_check_silent=0;

unsigned char get_i=0;
signed int modbus_holding_reg[Register_Count];
unsigned char tmp2[2];
unsigned int * tmp2_16bit;
unsigned char modbus_get_routine_tmr=0;

unsigned char get_tmp;
unsigned char read_write=0;//0 read,1 write


void modbus_RTU(void);
void modbus_RTU_err(unsigned char err_cd);


//unsigned char read_eeprom(unsigned char addr){
//    EEADR=addr;
//    EECON1bits.EEPGD=0;
//    EECON1bits.CFGS=0;
//    EECON1bits.RD=1;
//    return EEDATA;
//}
//void write_eeprom(unsigned char addr, unsigned char value){
//    EEADR=addr;
//    EEDATA=value;
//    EECON1bits.EEPGD=0;
//    EECON1bits.CFGS=0;
//    GIE=0;
//    EECON1bits.WREN=1;
//    EECON2=0x55;
//    EECON2=0x0AA;
//    EECON1bits.WR=1;
//    GIE=1;
//    Nop();
//
//    while (PIR2bits.EEIF==0)//Wait till write operation complete
//    {
//        Nop();
//    }
//    EECON1bits.WREN=0;
//    PIR2bits.EEIF=0;
//}



//void load_EEPROM(signed int * point, unsigned char len){
//   unsigned int addr, tmp;
//	unsigned char tmp_8bit[2];
//	unsigned int * tmp_16bit;
//	tmp_16bit=&tmp_8bit[0];
//   for(addr=0;addr<len;addr++){
//      tmp=addr+addr;
//		tmp_8bit[1]=read_eeprom (tmp);
//		tmp_8bit[0]=read_eeprom (tmp+1);
//      *point=*tmp_16bit;
//		point++;
//   }
//}
//
//void save_EEPROM(signed int * point, unsigned char len){
//	unsigned char addr;
//	unsigned char tmp_8bit[2];
//	unsigned int * tmp_16bit;
//	tmp_16bit=&tmp_8bit[0];
//   for(addr=0;addr<len;addr++){
//		*tmp_16bit=*point;
//      write_eeprom (addr*2,tmp_8bit[1]);
//      write_eeprom (addr*2+1,tmp_8bit[0]);
//		point++;
//   }
//}


void baglanti_hizi(unsigned char hiz_secim){
	int i,tmp;
	tmp=25; //32mhz için
	for (i=0;i<hiz_secim;i++)
	tmp=tmp/2;
	SPBRG=tmp;

}

void modbus_slave_rtu_init(unsigned char adress){
   modbus_adress=adress;
   modbus_time_over=0;
   modbus_get_start=0;
   send_count=0;
	tmp2_16bit=&tmp2[0];   
	enable_pin=0;

        TXSTA1=0b00000100;
	RCSTA1=0b10010000;
	BAUDCON1=0b00000000;
        TXSTA1bits.SYNC=0;
        TXSTA1bits.BRGH=1;
        BAUDCON1bits.BRG16=1;

                SPBRGH1=0x06;
                SPBRG1=0x82;//9600 64Mhz
               
            /*
            case 1:
                SPBRGH1=0x03;
                SPBRG1=0x40;//19.2k 64Mhz
                break;
            case 2:
                SPBRGH1=0x01;
                SPBRG1=0x15;//57.6 64Mhz
                break;
            case 3:
                SPBRGH1=0x00;
                SPBRG1=0x8A;//115.11k 64Mhz
                break;
            */
        
        
	PIE1bits.RC1IE=1;
   
}


unsigned char CRC16(unsigned char * buffer,unsigned char dataLength,char check) //CRC 16 for modbus checksum
{
   unsigned int CheckSum;
   unsigned int j;
   unsigned char lowCRC;
   unsigned char highCRC;
   unsigned char i;
      CheckSum = 0xffff;
   
   for (j=0; j<dataLength; j++)
   {
      CheckSum = CheckSum^(unsigned int)buffer[j];
      for(i=8;i>0;i--)
      if((CheckSum)&0x0001)
      CheckSum = (CheckSum>>1)^0xa001;
      else
      CheckSum>>=1;
   }   
   highCRC = CheckSum>>8;
   CheckSum<<=8;
   lowCRC = CheckSum>>8;
   if (check==1){   
      if ( (buffer[dataLength+1] == highCRC) & (buffer[dataLength] == lowCRC ))   
        return 1;
      else
        return 0;

   }
   else
   {  
      buffer[dataLength] = lowCRC;
      buffer[dataLength+1] = highCRC;
      return 1;    
   }
}


modbus_get_routine(unsigned char scale){

	if (modbus_get_start)modbus_get_routine_tmr++;
	//CCPR1L=modbus_routine_tmr;
   	if(!read_write){
		enable_pin=0;
		TXSTA1bits.TXEN=0;
	}
if(modbus_get_routine_tmr!=scale)return;
   

	modbus_get_routine_tmr=0;
   get_i=0;
   modbus_get_start=0;


 
   
}




void send_ready(unsigned char cnt){
    send_count=cnt;
    //enable_pin=1;
	read_write=1;
	//TX1REG=out_send_tmp[0];
	//PIE1bits.TX1IE=1;
	TXSTA1bits.TXEN=1;
	PIR1bits.TXIF=0;
}

void modbus_RTU(void){
   unsigned char get_addres=0,get_func=0;
   unsigned int reg_adr,reg_count;
   unsigned char i;
   
   get_addres=inp_get_tmp[0];
   get_func=inp_get_tmp[1];
   reg_adr=inp_get_tmp[2]*256+inp_get_tmp[3];
   reg_count=inp_get_tmp[4]*256+inp_get_tmp[5];
   
   if (get_addres==modbus_adress || get_addres==0xFF){
      
      switch(get_func){
         case 3:
            out_send_tmp[0] = get_addres;
            out_send_tmp[1] = get_func;
            out_send_tmp[2] = reg_count * 2;
            
            for (i=0;i<reg_count;i++){
				*tmp2_16bit=modbus_holding_reg[reg_adr+i];
               out_send_tmp[3+i*2] = tmp2[1];
               out_send_tmp[4+i*2] = tmp2[0];
            }
            
            CRC16(out_send_tmp,3+out_send_tmp[2],0);
            send_ready(5+out_send_tmp[2]);
//			haberlesme_kontrol=1;

            
            //send_byte(out_send_tmp,5+out_send_tmp[2]);
            break;
         case 6:
			tmp2[1] =  inp_get_tmp[4];
			tmp2[0] = inp_get_tmp[5];
            modbus_holding_reg[reg_adr]=*tmp2_16bit; // bms_select giriþi açýk devre ise register yazýlabilir.
            send_count=8;
            for (i=0;i<8;i++){out_send_tmp[i]=inp_get_tmp[i];}
            send_ready(8);
			// özel flag
			//if (reg_adr==19 || reg_adr==20) time_edit=1;//saat tarih düzenlemesi yapýldý
			if (reg_adr>49 && reg_adr<100){save_reg=1;}
//			if (reg_adr==14)oda_sicaklik_olcum_timer=10;//oda sýcaklýk bilgisi geliyor mu?
			//
            break;
         
         default:break;
      }
   }
   
   modbus_get_start=0;
   get_addres=0;
   get_func=0;

}

void modbus_RTU_err(unsigned char err_cd){
        
            out_send_tmp[0] = modbus_adress;
            out_send_tmp[1] = 3;
            out_send_tmp[2] =err_cd;
            
            
            CRC16(out_send_tmp,3,0);
            send_ready(5);
}

unsigned char modbus_addres_check(unsigned char adress){
   if (adress==modbus_adress || adress==0xFF){
		return 1;
	}else{
		return 0;
	}
}


//#INT_RDA
void get_data(void){
    
   get_tmp=RC1REG;//tmp=getc();

   if(!modbus_get_start){
      if(!modbus_addres_check(get_tmp)){//check the adress
        get_tmp=0;
		return;
      }
	modbus_get_routine_tmr=0;
	get_i=0;	
   }

   modbus_get_start=1;//mark as get started
	modbus_get_routine_tmr=0;

   inp_get_tmp [get_i] =get_tmp;// write inputs
	inp_get_tmp [get_i+1] =0;// write inputs

   if(get_i<inp_get_count)get_i++;// increase string counter

}

void tx_routine(void){
    unsigned char * tmp_out=out_send_tmp;
    enable_pin=0;
    if(!send_count)return;


    // ba?lang?ç beklemesi 3.5 karakter zaman?
    timer_tmp=9;
    while(timer_tmp);

    // enable pini beklemesi
    enable_pin=1;
    TXSTA1bits.TXEN=1;
    timer_tmp=1;
    while(timer_tmp);

    // Gönderim
    while(send_count){
        TX1REG=*tmp_out;

        while(!TXSTA1bits.TRMT);
        tmp_out++;
        send_count--;
    }
    // kapan?? beklemesi
    timer_tmp=1;
    while(timer_tmp);

    PIR1bits.TXIF=0;
    enable_pin=0;
}

void rx_routine(void){

    if (get_i>7){
            get_i=0;
            modbus_get_start=0;
            if(CRC16(inp_get_tmp,6,1)){
                    modbus_RTU();

            }
    }

}

//#INT_TBE
//void send_data(void){
//static unsigned char sent_i=0;
//
//if(!read_write){
//	TXSTA1bits.TXEN=0;
//	enable_pin=0;
//	return;
//}
//sent_i++;
//
//if(send_count==(sent_i)){
//   	read_write=0;
//	sent_i=0;
//	modbus_get_start=1;
//	PIE1bits.TXIE=0;
//	PIR1bits.TXIF=0;
//   return;
//}
//
//TX1REG=out_send_tmp[sent_i];
//PIR1bits.TXIF=0;
//}
