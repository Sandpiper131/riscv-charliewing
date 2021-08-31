
#include <metal/init.h>
#include <metal/cpu.h>
#include <fe310_charlieplex.h>

// Prototypes for New fe310_charliplex function
unsigned char *format_charlieplex_led_data(unsigned char *raw_data, unsigned int data_len){
    unsigned char form_data[LED_CTRL_REG_LEN];

    // Format Data using Raw Data (ie. 15 Byte unsigned char array)
    unsigned int raw_index = 0;  // Index of Raw Data Pointer
    for(int i = 0; i < LED_CTRL_REG_LEN; i++){
        form_data[i] = 0;
        if(raw_index < LED_X_MAX){

            // Only Increment Raw Index when writing to formatted data array
            if(i < 2 || i > 2){
                form_data[i] = raw_data[raw_index];
                raw_index++;
            }
        }


    }
}

int main() {
    metal_init();


    struct metal_i2c *i2c = NULL;
    i2c = charlieplex_init(I2C_BAUDRATE);

    if(i2c == NULL) return 1;  // Return Error 1 if init fails

    unsigned char data_buf[LED_CTRL_REG_LEN];
    for(int i = 0; i < LED_CTRL_REG_LEN; i++){
        data_buf[i] = 0x00;
    }

    // Mapping Out CharliePlex [ROW NUM | COL NUM | LED BITS | LED LOC ]
    //data_buf[0] = 0xFE;  // ROW 0-6 COL 14 [Right to Left] (BIT 1:7) CA1
    //data_buf[1] = 0xFF;  // NC CB1
    //data_buf[2] = 0xFE;  // R0-6 C13 (1:7) CA2
    //data_buf[3] = 0x7F;  // R6-0 C0 (0:6) CB2
    //data_buf[4] = 0xFE;  // R0-6 C12 (1:7) CA3
    //data_buf[5] = 0x7F;  // R0-6 C1 (0:6) CB3
    //data_buf[6] = 0xFE;  // R0-6 C11 (1:7) CA4
    data_buf[7] = 0x70;  // R6-0 C2 (0:6) CB4
    //data_buf[8] = 0xFE;  // R0-6 C10 (1:7) CA5
    //data_buf[9] = 0x7F;  // R6-0 C3 (0:6) CB5
    //data_buf[10] = 0xFE; // R0-6 C9 (1:7) CA6
    //data_buf[11] = 0x7F; // R6-0 C4 (0:6) CB6
    //data_buf[12] = 0xFE; // R0-6 C8 (1:7) CA7
    //data_buf[13] = 0x7F; // R6-0 C5 (0:6) CB7
    //data_buf[14] = 0xFE; // R0-6 C7 (1:7) CA8
    //data_buf[15] = 0x7F; // R6-0 C6 (0:6) CB8


    charlieplex_register_write_multibyte_data(i2c, 0x00, 0x00, data_buf, 17);

    unsigned char pwm_buf[PWM_REG_LEN];
    for(int x = 0; x < PWM_REG_LEN; x++){
        pwm_buf[x] = 0x0F;
    }

    //pwm_buf[1] = 0xFF;

    charlieplex_register_write_multibyte_data(i2c, 0x00, PWM_REG_START, pwm_buf, PWM_REG_LEN);

    metal_fini();
}
