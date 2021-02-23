unsigned

#include "NRF2401.c"
 char spi_data[35];
//unsigned long spi_adress=0;

#define SPI_CS  PORTCbits.RC2	//Ausgang f�r Chip Select
#define SPI_CE  PORTAbits.RA0

void delay(void)
{
    char i;
    for(i=0;i<200;i++)
    {
        NOP();
    }
}

char SPI(char d)		// �ber SPI-Schnittstelle senden
{				// 
    SSP1BUF=d;
    while (!SSP1STATbits.BF);			// 	Warten bis gesendet ist
    PORTCbits.RC0=!PORTCbits.RC0;
    return SSP1BUF;			// gleichzeitig empfangen
}

void send()
{
    SPI_CE=1;
    delay();
    delay();
//    delay();
//    delay();
//    delay();
//    delay();
//    delay();
//    delay();
    SPI_CE=0;
}

void spi_start_command (void)
{
    SPI_CS=1;
    delay();
    SPI_CS=0;
}

void spi_end_command (void)
{
    SPI_CS=1;
}


void SPI_Init()
{
// Init SPI
SSP1STATbits.SMP=0;		//Read at the end of the clock
SSP1STATbits.CKE=1;		// transmit occurs on transition from active to idle clock state
SSP1CON1bits.CKP=0;		// idle state of the clock is low level

//SSP1CON1bits.SSPM3=0;
//SSP1CON1bits.SSPM2=0;
//SSP1CON1bits.SSPM1=1;	// Fosc/64(1MHz)
//SSP1CON1bits.SSPM0=0;	//
//SSP1CON1bits.SSPEN=1;	// SPI1 enabled

SSP1CON1=0b00100010;
//SSP1ADD=63;
}

void spi_read(unsigned char * data, unsigned char adr,unsigned char len){
    unsigned char i=0;
    spi_start_command();
    stat=SPI(adr);
    for(i=0;i<len;i++){
    data[i]=SPI(0xFF);
    }
    spi_end_command();
}

void spi_write(unsigned char * data, unsigned char adr,unsigned char len){
    unsigned char i=0;
    spi_start_command();
        SPI(adr | 0b00100000);//yazma i�in adres de?i?ikligi
    for(i=0;i<len;i++){
        SPI(data[i]);
    }
    spi_end_command();
}

void write_payload(unsigned char * data, unsigned char adr,unsigned char len){
    unsigned char i=0;
    spi_start_command();
        SPI(adr);//yazma i�in adres de?i?ikligi
    for(i=0;i<len;i++){
        SPI(data[i]);
    }
    spi_end_command();
}

void receive_init()
{
    //spi_data[0]=0b00110011;
    spi_data[0]=0b01111111;
    spi_write(spi_data,SETUP_RETR,1);
    spi_data[0]=0b00001111; // 2mbps
    //spi_data[0]=0b00101111; // 250kbps
    spi_write(spi_data,RF_SETUP,1);

    spi_data[0]=0b00001011;
    spi_write(spi_data,CONFIG,1);
    spi_data[0]=0b00100000;
    spi_write(spi_data,RX_PW_P0,1);
    SPI_CE=1;
    delay();
    delay();
    
}

//void spi_transmit(){
//    //paket_sayisi++;
//    spi_start_command();
//    SPI(FLUSH_TX);
//    spi_end_command();
//    spi_data[0]=0b01111110;
//    spi_write(spi_data,STATUS,1);
//    SPI_CE=0;
//    spi_data[0]=0b00001010;
//    spi_write(spi_data,CONFIG,1);     //PRIM_RX low
//    spi_data[0]=0xE7;
//    spi_data[1]=0xE7;
//    spi_data[2]=0xE7;
//    spi_data[3]=0xE7;
//    spi_data[4]=0xE7;
//    spi_write(spi_data,TX_ADDR,5);        //the address is written to TX_ADDR
//
//    spi_write(&paket,W_TX_PAYLOAD,32);   //data written to TX_FIFO
//    spi_data[0]=0b00111111;
//    spi_write(spi_data,EN_AA,1);      //autoacknowledgment enabled
//    send();
//    receive_init();
//}

void spi_transmit(unsigned int start_reg_id)
{
    unsigned char i=0,* tmp_buf;
    //paket_sayisi++;
    spi_start_command();
    SPI(FLUSH_TX);
    spi_end_command();
    spi_data[0]=0b01111110;
    spi_write(spi_data,STATUS,1);
    SPI_CE=0;
    spi_data[0]=0b00001010; //crc 2 byte
    spi_write(spi_data,CONFIG,1);     //PRIM_RX low
//    spi_data[0]=0xE7;
//    spi_data[1]=0xE7;
//    spi_data[2]=0xE7;
//    spi_data[3]=0xE7;
//    spi_data[4]=0xE7;
//    spi_write(spi_data,TX_ADDR,5);        //the address is written to TX_ADDR

    tmp_buf=&Registers[start_reg_id];

    spi_data[0]=start_reg_id%256;
    spi_data[1]=start_reg_id/256;

    for(i=0;i<30;i++){
        spi_data[i+2]=*tmp_buf;
        tmp_buf++;
    }

    spi_write(spi_data,W_TX_PAYLOAD,32);   //data written to TX_FIFO
    spi_data[0]=0b00111111;
    spi_write(spi_data,EN_AA,1);      //autoacknowledgment enabled
    send();
    receive_init();
}



char receive_check()
{
    char return_val=0;
    spi_data[0]=0b00110000;
    spi_write(spi_data,STATUS,1);
    spi_read(&stat,STATUS,1);

    if(stat & 0b01000000)
    {
        spi_read(&alinan,0x61,32);
        spi_read(&reg_value,0x61,5);
        SPI(FLUSH_RX);

        spi_data[0]=0b01111110;
        spi_write(spi_data,STATUS,1);
        return_val=1;

    }
    return return_val;
}

//void receive_check()
//{
//    //GIE=0;
//    spi_data[0]=0b00110000;
//    spi_write(spi_data,STATUS,1);
//    spi_read(&stat,STATUS,1);
//    //sil=stat & 0b01000000;
//
//    if(stat & 0b01000000)
//    {
//        spi_read(&alinan,0x61,32);
//        spi_read(&reg_value,0x61,5);
//        SPI(FLUSH_RX);
//        spi_data[0]=0b01111110;
//        spi_write(spi_data,STATUS,1);
//    }
//    //GIE=1;
//}

void change_channel(unsigned char ch)
{
    spi_data[0]=0b00000010+ch;
    spi_write(spi_data,RF_CH,1);

    spi_data[0]=ch;
    spi_data[1]=0xE7;
    spi_data[2]=ch;
    spi_data[3]=ch;
    spi_data[4]=0xE7;
    spi_write(spi_data,TX_ADDR,5);        //the address is written to TX_ADDR

    spi_data[0]=ch;
    spi_data[1]=0xE7;
    spi_data[2]=ch;
    spi_data[3]=ch;
    spi_data[4]=0xE7;
    spi_write(spi_data,RX_ADDR_P0,5);        //the address is written to RX_ADDR


}