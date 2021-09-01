
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

    unsigned char pwm_buf[PWM_REG_LEN];
    for(int x = 0; x < PWM_REG_LEN; x++){
        pwm_buf[x] = 0x0F;
    }

    //pwm_buf[1] = 0xFF;

    charlieplex_register_write_multibyte_data(i2c, 0x00, PWM_REG_START, pwm_buf, PWM_REG_LEN);

    metal_fini();
}
