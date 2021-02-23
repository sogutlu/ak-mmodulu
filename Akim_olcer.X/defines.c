

signed long watt1_integ;
signed long watt2_integ;

#define led LATCbits.LATC0
#define role LATBbits.LATB4
#define l_out LATEbits.LATE1
#define r_out LATEbits.LATE0

#define l_in PORTBbits.RB1
#define r_in PORTBbits.RB0
#define reset_capasite PORTCbits.RC1

#define rf_reset_pin PORTEbits.RE3


#define rf_adress_report Registers[0]
#define durum Registers[1]

#define watt1 Registers[2]
#define watt2 Registers[3]

#define hall1 Registers[4]
#define hall2 Registers[5]
#define vbatt1 Registers[6]
#define hiz Registers[7]

#define sicaklik_kabin Registers[8]

#define hall1_adc Registers[9]
#define hall2_adc Registers[10]
#define vbatt1_adc Registers[11]
#define hiz_clk Registers[12]

#define watt_h1_report Registers[13]
#define watt_h2_report Registers[14]



#define vbatt2 Registers[15]
#define vbatt2_adc Registers[16]

#define rf_send_type  Registers[20]
#define stat        Registers[21]
#define reg_value   Registers[22]

#define alinan   Registers[25]
#define rf_command_reg_id   Registers[25]
#define rf_command_reg_val   Registers[26]



#define versiyon Registers[48]

#define save_reg Registers[49]

// eeprom kay?t ba?lang?c?
#define modbus_adress Registers[50]
#define batt_max_capasite Registers[51]
#define rf_adress Registers[52]

#define watt_h1 Registers[55]
#define watt_h2 Registers[56]


#define hall1_sum Registers[60]
#define hall1_mul Registers[61]
#define hall2_sum Registers[62]
#define hall2_mul Registers[63]
#define vbatt1_sum Registers[64]
#define vbatt1_mul Registers[65]
#define vbatt2_sum Registers[66]
#define vbatt2_mul Registers[67]
#define hiz_sum Registers[68]
#define hiz_mul Registers[69]