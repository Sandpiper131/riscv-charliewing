//
// Created by Luke LaCasse on 8/29/21.
//

#include <fe310_charlieplex.h>

// Found at: https://stackoverflow.com/questions/746171/efficient-algorithm-for-bit-reversal-from-msb-lsb-to-lsb-msb-in-c
static const unsigned char BitReverseTable256[] =
        {
                0x00, 0x80, 0x40, 0xC0, 0x20, 0xA0, 0x60, 0xE0, 0x10, 0x90, 0x50, 0xD0, 0x30, 0xB0, 0x70, 0xF0,
                0x08, 0x88, 0x48, 0xC8, 0x28, 0xA8, 0x68, 0xE8, 0x18, 0x98, 0x58, 0xD8, 0x38, 0xB8, 0x78, 0xF8,
                0x04, 0x84, 0x44, 0xC4, 0x24, 0xA4, 0x64, 0xE4, 0x14, 0x94, 0x54, 0xD4, 0x34, 0xB4, 0x74, 0xF4,
                0x0C, 0x8C, 0x4C, 0xCC, 0x2C, 0xAC, 0x6C, 0xEC, 0x1C, 0x9C, 0x5C, 0xDC, 0x3C, 0xBC, 0x7C, 0xFC,
                0x02, 0x82, 0x42, 0xC2, 0x22, 0xA2, 0x62, 0xE2, 0x12, 0x92, 0x52, 0xD2, 0x32, 0xB2, 0x72, 0xF2,
                0x0A, 0x8A, 0x4A, 0xCA, 0x2A, 0xAA, 0x6A, 0xEA, 0x1A, 0x9A, 0x5A, 0xDA, 0x3A, 0xBA, 0x7A, 0xFA,
                0x06, 0x86, 0x46, 0xC6, 0x26, 0xA6, 0x66, 0xE6, 0x16, 0x96, 0x56, 0xD6, 0x36, 0xB6, 0x76, 0xF6,
                0x0E, 0x8E, 0x4E, 0xCE, 0x2E, 0xAE, 0x6E, 0xEE, 0x1E, 0x9E, 0x5E, 0xDE, 0x3E, 0xBE, 0x7E, 0xFE,
                0x01, 0x81, 0x41, 0xC1, 0x21, 0xA1, 0x61, 0xE1, 0x11, 0x91, 0x51, 0xD1, 0x31, 0xB1, 0x71, 0xF1,
                0x09, 0x89, 0x49, 0xC9, 0x29, 0xA9, 0x69, 0xE9, 0x19, 0x99, 0x59, 0xD9, 0x39, 0xB9, 0x79, 0xF9,
                0x05, 0x85, 0x45, 0xC5, 0x25, 0xA5, 0x65, 0xE5, 0x15, 0x95, 0x55, 0xD5, 0x35, 0xB5, 0x75, 0xF5,
                0x0D, 0x8D, 0x4D, 0xCD, 0x2D, 0xAD, 0x6D, 0xED, 0x1D, 0x9D, 0x5D, 0xDD, 0x3D, 0xBD, 0x7D, 0xFD,
                0x03, 0x83, 0x43, 0xC3, 0x23, 0xA3, 0x63, 0xE3, 0x13, 0x93, 0x53, 0xD3, 0x33, 0xB3, 0x73, 0xF3,
                0x0B, 0x8B, 0x4B, 0xCB, 0x2B, 0xAB, 0x6B, 0xEB, 0x1B, 0x9B, 0x5B, 0xDB, 0x3B, 0xBB, 0x7B, 0xFB,
                0x07, 0x87, 0x47, 0xC7, 0x27, 0xA7, 0x67, 0xE7, 0x17, 0x97, 0x57, 0xD7, 0x37, 0xB7, 0x77, 0xF7,
                0x0F, 0x8F, 0x4F, 0xCF, 0x2F, 0xAF, 0x6F, 0xEF, 0x1F, 0x9F, 0x5F, 0xDF, 0x3F, 0xBF, 0x7F, 0xFF
        };

static const unsigned int LEDColToMatrixIndex[] = {2, 4, 6, 8, 10, 12, 14, 13, 11, 9, 7, 5, 3, 1, 0};

static const unsigned char LEDtoREG[] = {0x03, 0x05, 0x07, 0x09, 0x0A, 0x0D, 0x0F, 0x0E, 0x0C, 0x0A, 0x08, 0x06, 0x04, 0x02, 0x00};

extern void charlieplex_register_write_byte(struct metal_i2c *i2c, unsigned char reg_addr, unsigned char reg_data){
    unsigned char buf[REG_WR_MIN_LEN] = {reg_addr, reg_data};
    metal_i2c_write(i2c, CHARLIEPLEX_I2C_ADDR, REG_WR_MIN_LEN, buf, METAL_I2C_STOP_ENABLE);
}

// Description: Set Command Register Data
extern void charlieplex_set_page(struct metal_i2c *i2c, unsigned char page_num){
    charlieplex_register_write_byte(i2c, CMD_REG, page_num);  // Set Command Register to Desired Page
}


extern struct metal_i2c *charlieplex_init(unsigned int baud){
    struct metal_i2c *i2c = NULL;
    i2c = metal_i2c_get_device(0);
    if(i2c == NULL) return NULL;

    metal_i2c_init(i2c, baud, METAL_I2C_MASTER);

    // Set Shutdown Mode: Normal
    charlieplex_set_page(i2c, 0x0B);
    charlieplex_register_write_byte(i2c, SHUTDOWN_REG, NORMAL_OPERATION);

    charlieplex_reset_fade(i2c);

    return i2c;
}


extern void charlieplex_register_write_multibyte_data(struct metal_i2c *i2c, unsigned char page_num, unsigned char response_reg, unsigned char *data, unsigned int data_len){
    unsigned char buf[data_len + 1];
    buf[0] = response_reg;  // Load Response Reg Address into data buffer first

    // Fill Data Buffer
    for(int i = 0; i < data_len; i++){
        buf[i + 1] = data[i];  // Write data to buffer without overwriting register addresses
    }

    charlieplex_set_page(i2c, page_num);  // Select Proper Page
    metal_i2c_write(i2c, CHARLIEPLEX_I2C_ADDR, (data_len + 1), buf, METAL_I2C_STOP_ENABLE);
}

extern void charlieplex_write_pixel(unsigned int x, unsigned int y, unsigned char duty_cycle){
    //unsigned int reversed[6] = {0x00, }
}


// BAD IMPLEMENTATION BELOW
extern void write_charlieplex_led_data(struct metal_i2c *i2c, unsigned char page_num, unsigned char *raw_data){
    unsigned char form_data[LED_CTRL_REG_LEN];

    // Copy Raw LED data to temporary array
    unsigned char temp[LED_X_MAX];
    for(int u = 0; u < LED_X_MAX; u++){
        temp[u] = raw_data[u];
    }

    unsigned char reversed_index[] = {2, 6, 8, 10, 12, 14};
    unsigned int reversed_len = 6;
    for(int f = 0; f < reversed_len; f++){
        temp[reversed_index[f]] = ROTATE_REVERSE(temp[f]);
    }

    // Map the raw data to the hardware
    for(int y = 0; y < LED_X_MAX; y++){
        raw_data[LEDColToMatrixIndex[y]] = temp[y];
    }

    //unsigned char reversed_index[] = {3, 7, 9, 11, 13, 15};  // Index of Reversed LED Registers


    // Format Data using Raw Data (ie. 15 Byte unsigned char array)
    unsigned int raw_index = 0;  // Index of Raw Data Pointer
    for(int i = 0; i < LED_CTRL_REG_LEN; i++){
        form_data[i] = 0x00;
        if(raw_index < LED_X_MAX){
            // Only Increment Raw Index when writing to formatted data array
            if(i != 1){
                /*for(int x = 0; x < reversed_len; x++){
                    if(i == reversed_index[x]) raw_data[raw_index] = ROTATE_REVERSE(BitReverseTable256[raw_data[raw_index]]);  // If Reg Reversed, reverse led data
                }*/

                //if((i % 2) == 0) raw_data[raw_index] = ROTATE(raw_data[raw_index]);  // If Reg is rotated in hardware, rotate led data
                form_data[i] = raw_data[raw_index];
                raw_index++;
            }
        }

        charlieplex_register_write_multibyte_data(i2c, page_num, LED_CTRL_REG_START, form_data, LED_CTRL_REG_LEN);
    }
}

extern void charlieplex_set_fade(struct metal_i2c *i2c, unsigned char fade_out_time, unsigned char fade_in_time, unsigned char extinguish_time){
    charlieplex_set_page(i2c, 0x0B);  // Set Page to Function Register

    unsigned char bc1_reg_data = (fade_in_time | (fade_out_time << 4));
    charlieplex_register_write_byte(i2c, BC1_REG, bc1_reg_data);


    unsigned char bc2_reg_data = (extinguish_time | (1 << 4));
    charlieplex_register_write_byte(i2c, BC2_REG, bc2_reg_data);
}

extern void charlieplex_reset_fade(struct metal_i2c *i2c){
    charlieplex_set_page(i2c, 0x0B);
    charlieplex_register_write_byte(i2c, BC1_REG, 0x00);
    charlieplex_register_write_byte(i2c, BC2_REG, 0x00);
}

// Hopefully a better implementation than write_charlieplex_led_data(...)
extern void charlieplex_write_led_page(struct metal_i2c *i2c, unsigned char page_num, unsigned char *raw_data){
    // TODO: Use new LUT to map 15 Bytes of LED Data to the 15 Respective Registers in Charlieplex LED Driver IC
}