//
// Created by Luke LaCasse on 8/29/21.
//

#include <fe310_charlieplex.h>


extern void charlieplex_register_write_byte(struct metal_i2c *i2c, unsigned char reg_addr, unsigned char reg_data){
    unsigned char buf[REG_WR_MIN_LEN] = {reg_addr, reg_data};
    metal_i2c_write(i2c, CHARLIEPLEX_I2C_ADDR, REG_WR_MIN_LEN, buf, METAL_I2C_STOP_ENABLE);
}

// Description: Set Command Register Data
extern void charlieplex_set_cmd_reg(struct metal_i2c *i2c, unsigned char page_num){
    charlieplex_register_write_byte(i2c, CMD_REG, page_num);
}


extern struct metal_i2c *charlieplex_init(unsigned int baud){
    struct metal_i2c *i2c = NULL;
    i2c = metal_i2c_get_device(0);
    if(i2c == NULL) return NULL;

    metal_i2c_init(i2c, baud, METAL_I2C_MASTER);

    // Set Shutdown Mode: Normal
    charlieplex_set_cmd_reg(i2c, 0x0B);
    charlieplex_register_write_byte(i2c, SHUTDOWN_REG, NORMAL_OPERATION);

    return i2c;
}


extern void charlieplex_register_write_multibyte_data(struct metal_i2c *i2c, unsigned char page_num, unsigned char response_reg, unsigned char *data, unsigned int data_len){
    unsigned char buf[data_len + 1];
    buf[0] = response_reg;  // Load Response Reg Address into data buffer first

    // Fill Data Buffer
    for(int i = 0; i < data_len; i++){
        buf[i + 1] = data[i];  // Write data to buffer without overwriting register addresses
    }

    charlieplex_set_cmd_reg(i2c, page_num);  // Select Proper Page
    metal_i2c_write(i2c, CHARLIEPLEX_I2C_ADDR, (data_len + 1), buf, METAL_I2C_STOP_ENABLE);
}

extern void charlieplex_write_pixel(unsigned int x, unsigned int y, unsigned char duty_cycle){
    //unsigned int reversed[6] = {0x00, }
}