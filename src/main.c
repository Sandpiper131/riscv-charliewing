
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
        if(i % 2 == 0){
            data_buf[i] = 0xaa;
        }else{
            data_buf[i] = (0xaa >> 1);
        }
    }

    //charlieplex_register_write_multibyte_data(i2c, 0x00, 0x00, data_buf, 17);
    write_charlieplex_led_data(i2c, 0x00, data_buf);

    for(int x = 0; x < data_len; x++){
        /*if(x % 2 == 0){
            data_buf[x] = (0xaa >> 1);
        }else{
            data_buf[x] = 0xaa;
        }*/
        data_buf[x] = 0x01;
    }

    write_charlieplex_led_data(i2c, 0x01, data_buf);

    unsigned char pwm_buf[PWM_REG_LEN];
    for(int x = 0; x < PWM_REG_LEN; x++){
        pwm_buf[x] = 0x0F;
    }

    //pwm_buf[1] = 0xFF;

    // Write MV Letters to Display
    unsigned char m5_display[LED_X_MAX] = {0x7F, 0x60, 0x18, 0x06, 0x75, 0x00, 0x1F, 0x40, 0x80, 0x40, 0x1F, 0x00, 0x00, 0xCF};
    write_charlieplex_led_data(i2c, 0x02, m5_display);

    charlieplex_register_write_multibyte_data(i2c, 0x00, PWM_REG_START, pwm_buf, PWM_REG_LEN);
    charlieplex_register_write_multibyte_data(i2c, 0x01, PWM_REG_START, pwm_buf, PWM_REG_LEN);
    charlieplex_register_write_multibyte_data(i2c, 0x02, PWM_REG_START, pwm_buf, PWM_REG_LEN);



    // Configure Breathing
    charlieplex_set_page(i2c, 0x0B);
    charlieplex_register_write_byte(i2c, 0x00, 0x08);
    charlieplex_register_write_byte(i2c, 0x02, 0x03);  // Auto Loop 2 Frames
    charlieplex_register_write_byte(i2c, 0x03, 0x17);
    charlieplex_register_write_byte(i2c, 0x08, 0x44);
    charlieplex_register_write_byte(i2c, 0x09, 0x14);


    /*
    while(1){
        charlieplex_register_write_byte(i2c, 0x01, 0x01);
        charlieplex_register_write_byte(i2c, 0x01, 0x00);
    }
    */

    metal_fini();
}
