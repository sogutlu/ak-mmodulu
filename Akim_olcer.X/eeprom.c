unsigned char read_eeprom(unsigned char addr){
    EEADR=addr;
    EECON1bits.EEPGD=0;
    EECON1bits.CFGS=0;
    EECON1bits.RD=1;
    return EEDATA;
}
void write_eeprom(unsigned char addr, unsigned char value){
    EEADR=addr;
    EEDATA=value;
    EECON1bits.EEPGD=0;
    EECON1bits.CFGS=0;
    GIE=0;
    EECON1bits.WREN=1;
    EECON2=0x55;
    EECON2=0x0AA;
    EECON1bits.WR=1;
    GIE=1;
    Nop();

    while (PIR2bits.EEIF==0)//Wait till write operation complete
    {
        Nop();
    }
    EECON1bits.WREN=0;
    PIR2bits.EEIF=0;
}



void load_EEPROM(signed int * point, unsigned char len){
   unsigned int addr, tmp;
	unsigned char tmp_8bit[2];
	unsigned int * tmp_16bit;
	tmp_16bit=&tmp_8bit[0];
   for(addr=0;addr<len;addr++){
      tmp=addr+addr;
		tmp_8bit[1]=read_eeprom (tmp);
		tmp_8bit[0]=read_eeprom (tmp+1);
      *point=*tmp_16bit;
		point++;
   }
}

void save_EEPROM(signed int * point, unsigned char len){
	unsigned char addr;
	unsigned char tmp_8bit[2];
	unsigned int * tmp_16bit;
	tmp_16bit=&tmp_8bit[0];
   for(addr=0;addr<len;addr++){
		*tmp_16bit=*point;
      write_eeprom (addr*2,tmp_8bit[1]);
      write_eeprom (addr*2+1,tmp_8bit[0]);
		point++;
   }
}