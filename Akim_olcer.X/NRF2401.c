//COMMAND LIST
#define R_REGISTER      0x00            //000AAAAA Read command and status registers. AAAAA = 5 bit Register Map Address
#define W_REGISTER      0x20            //001AAAAA Write command and status registers. AAAAA = 5bit Register Map Address
                                        //Executable in power down or standby modes only.

#define R_RX_PAYLOAD    0b01100001      //Read RX-payload: 1 ? 32 bytes. A read operation always starts at byte 0.
                                        //Payload is deleted from FIFO after it is read. Used in RX mode.

#define W_TX_PAYLOAD    0b10100000      //Write TX-payload: 1 ? 32 bytes. A write operation always starts at byte 0 used in TX payload.

#define FLUSH_TX        0b11100001      //Flush TX FIFO, used in TX mode //gönderim kay?tç?s?n? temizler
#define FLUSH_RX        0b11100010      //Flush RX FIFO, used in RX mode //al?m kay?tç?s?n? temizler
                                        //Should not be executed during transmission of //al?m yap?l?rken kullan?lmamal?
                                        //acknowledge, that is, acknowledge package will
                                        //not be completed.

#define REUSE_TX_PL     0b11100011      //Reuse last transmitted payload //son gönderilen paketi kullanma W_TX_PAYLOAD ve FLUSH_TX kullan?lmad??? sürece aktif kal?r

//FEATURE register should be set before using the following  three commands
#define R_RX_PL_WID     0b01100000      //Read RX payload width for the top R_RX_PAYLOAD in the RX FIFO.
#define W_ACK_PAYLOAD   0b10101000      //Used in RX mode. Write Payload to be transmitted together with ACK packet on PIPE PPP. PPP 000 to 101 (0-5)
#define W_TX_PAYLOAD_NOACK  0b1011000   //Used in TX mode. Disables AUTOACK on this specific packet.

#define NO_OP           0xFF            //no operation can be used to read STATUS register

////////////////////////////////////////////////////////////////////////


//REGISTER LIST
#define CONFIG          0x00            //0,MASK_RX_DR,MASK_TX_DS,MASK_MAX_RT,EN_CRC,CRCO,PWR_UP,PRX/PTX(1/0)
                                        //MASK interrupts for receive, tranmit, maximum retransmit
                                        //CRC encoding enable
                                        //CRC 1byte='0' or 2 byte='1'
                                        //Power up:1 power down:0
                                        //RX:1 TX:0

#define EN_AA           0x01            //enable autoacknowledge for pipes
                                        //00PPPPPP   for example to disable autoacknowledge for pipe 3
                                        //register should be set to : 00110111
#define EN_RXADDR       0x02            //enabled rx addresses reset value 00000011
                                        //00PPPPPP to enable pipe 3 address
                                        //register is set to : 00001011

#define SETUP_AW        0x03            //address width for pipes 000000 following '00' - Illegal, '01' - 3 bytes, '10' - 4 bytes, '11' ? 5 bytes
#define SETUP_RETR      0x04            //setup for retransmission  MS4 bits ?0000? ? Wait 250?S ?0001? ? Wait 500?S ?0010? ? Wait 750?S ?1111? ? Wait 4000?S
                                        //LS4 bits ?0000? ?Re-Transmit disabled, ?1111? ? Up to 15 Re-Transmit on fail of AA
#define RF_CH           0x05            //channel LS7bits determines the communication frequency
#define RF_SETUP        0x06            //CONT_WAVE, 0, RF_DR_LOW, PLL_LOCK, RF_DR_HIGH, RF_PWR(2bits),0
                                        //[RF_DR_LOW,RF_DR_HIGH] = ?00? ? 1Mbps, ?01? ? 2Mbps, ?10? ? 250kbps ?11? ? Reserved
                                        //RF_PWR = '00' ? -18dBm '01' ? -12dBm '10' ? -6dBm '11' ? 0dBm

#define STATUS          0x07            //0, RX_DR, TX_DS, MAX_RT, RX_P_NO, TX_FULL
                                        //000-101: Data Pipe Number, 110: Not Used, 111: RX FIFO Empty
#define OBSERVE_TX      0x08            //Transmit observe register MS4bits count lost packets LS4bits count retransmitted packets
#define RPD             0x09            //Received power detector LSB is 1 if power detected MS7bits are 0000000
#define RX_ADDR_P0      0x0A            //Receive address data pipe 0. 5 Bytes maximum length. (LSByte is written first. Write the number of bytes defined by SETUP_AW)
#define RX_ADDR_P1      0x0B            //Receive address data pipe 1. 5 Bytes maximum length. (LSByte is written first. Write the number of bytes defined by SETUP_AW)
#define RX_ADDR_P2      0x0C            //Receive address data pipe 2. Only LSB. MSBytes equal to RX_ADDR_P1[39:8]
#define RX_ADDR_P3      0x0D            //Receive address data pipe 3. Only LSB. MSBytes equal to RX_ADDR_P1[39:8]
#define RX_ADDR_P4      0x0E            //Receive address data pipe 4. Only LSB. MSBytes equal to RX_ADDR_P1[39:8]
#define RX_ADDR_P5      0x0F            //Receive address data pipe 5. Only LSB. MSBytes equal to RX_ADDR_P1[39:8]
#define TX_ADDR         0x10            //Transmit address. Used for a PTX device only. (LSByte is written first)
                                        //Set RX_ADDR_P0 equal to this address to handleautomatic acknowledge if this is a PTX device
                                        //with Enhanced ShockBurst? enabled.
#define RX_PW_P0        0x11            //RX payload width in data pipe 0 MS2bits 00, LS6bits 0-32 bytes
#define RX_PW_P1        0x12            //RX payload width in data pipe 1 MS2bits 00, LS6bits 0-32 bytes
#define RX_PW_P2        0x13            //RX payload width in data pipe 2 MS2bits 00, LS6bits 0-32 bytes
#define RX_PW_P3        0x14            //RX payload width in data pipe 3 MS2bits 00, LS6bits 0-32 bytes
#define RX_PW_P4        0x15            //RX payload width in data pipe 4 MS2bits 00, LS6bits 0-32 bytes
#define RX_PW_P5        0x16            //RX payload width in data pipe 5 MS2bits 00, LS6bits 0-32 bytes
#define FIFO_STATUS     0x17            //0,TX_REUSE, TX_FULL, TX_EMPTY,00,RX_FULL,RX_EMPTY
#define DYNPD           0x1C            //Enable dynamic payload length 00,DPL_P5,DPL_P4,DPL_P3,DPL_P2,DPL_P1,DPL_P0
#define FEATURE         0x1D            //00000, EN_DPL, EN_ACK_PAY, EN_DYN_ACK