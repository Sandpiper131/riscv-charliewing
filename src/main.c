
#include <metal/init.h>
#include <metal/cpu.h>
#include <fe310_charlieplex.h>


int main() {
    metal_init();


    struct metal_i2c *i2c = NULL;
    i2c = charlieplex_init(I2C_BAUDRATE);

    if(i2c == NULL) return 1;  // Return Error 1 if init fails

    unsigned char data_len = LED_X_MAX;
    unsigned char data_buf[data_len];
    for(int i = 0; i < data_len; i++){
        data_buf[i] = 0xFF;
    }


    // Mapping Out CharliePlex [ROW NUM | COL NUM | LED BITS | LED LOC ]
    //data_buf[0] = 0xFE;  // ROW 0-6 COL 14 [Right to Left] (BIT 1:7) CA1
    //data_buf[1] = 0xFF;  // NC CB1
    //data_buf[2] = 0xFE;  // R0-6 C13 (1:7) CA2
    //data_buf[3] = 0x7F;  // R6-0 C0 (0:6) CB2
    //data_buf[4] = 0xFE;  // R0-6 C12 (1:7) CA3
    //data_buf[5] = 0x7F;  // R0-6 C1 (0:6) CB3
    //data_buf[6] = 0xFE;  // R0-6 C11 (1:7) CA4
    //data_buf[7] = 0x70;  // R6-0 C2 (0:6) CB4
    //data_buf[8] = 0xFE;  // R0-6 C10 (1:7) CA5
    //data_buf[9] = 0x7F;  // R6-0 C3 (0:6) CB5
    //data_buf[10] = 0xFE; // R0-6 C9 (1:7) CA6
    //data_buf[11] = 0x7F; // R6-0 C4 (0:6) CB6
    //data_buf[12] = 0xFE; // R0-6 C8 (1:7) CA7
    //data_buf[13] = 0x7F; // R6-0 C5 (0:6) CB7
    //data_buf[14] = 0xFE; // R0-6 C7 (1:7) CA8
    //data_buf[15] = 0x7F; // R6-0 C6 (0:6) CB8



    //charlieplex_register_write_multibyte_data(i2c, 0x00, 0x00, data_buf, 17);
    write_charlieplex_led_data(i2c, 0x00, data_buf);

    unsigned char pwm_buf[PWM_REG_LEN];
    for(int x = 0; x < PWM_REG_LEN; x++){
        pwm_buf[x] = 0x1F;
    }

    //pwm_buf[1] = 0xFF;

    charlieplex_register_write_multibyte_data(i2c, 0x00, PWM_REG_START, pwm_buf, PWM_REG_LEN);

    metal_fini();
}
