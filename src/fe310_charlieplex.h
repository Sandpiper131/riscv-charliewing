// Title: Charlieplex Freedom Edition
// Author: Luke LaCasse
// Date Created: 8/29/21
// Date Modified: 8/31/21  TODO: Auto Generate Date Modified!!
// Description: An open source library to drive the Adafruit Charlieplex 15x7 LED Matrix FeatherWing using
//              a Sparkfun ThingPlus Sifive Freedom FE310-G002 Microcontroller Dev Board.
//
//                                 @@@ TLDR; OPEN SOVRCE BLINKEN LIGHTS @@@


// *** START NOTES ***
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
// *** END NOTES ***

#ifndef RISCV_CHARLIEWING_FE310_CHARLIEPLEX_H
#define RISCV_CHARLIEWING_FE310_CHARLIEPLEX_H

// Include Relevant Freedom Metal Libraries
#include <metal/i2c.h>
#include <metal/cpu.h>

#define CHARLIEPLEX_I2C_ADDR 0x74

#define LED_X_MAX 15
#define LED_X_MIN 0

#define LED_Y_MAX 7
#define LED_Y_MIN 0

#define ROTATE(led_data) (led_data << 1)
#define ROTATE_REVERSE(led_data) (led_data >> 1)
#define REG_LEN(reg_addr_end, reg_addr_start) (reg_addr_end - reg_addr_start)  // Calculate Length in Bytes of Register

// #############################
// ### IS31FL3731 Memory Map ###
// #############################

// *** Command Register Memory Map ***
#define CMD_REG 0xFD
// *** End Command Register Memory Map ***


// *** Frame Register Memory Map ***

// LED Control Register
#define LED_CTRL_REG_START 0x00
#define LED_CTRL_REG_END 0x11
#define LED_CTRL_REG_LEN REG_LEN(LED_CTRL_REG_END, LED_CTRL_REG_START)

// Blink Control Register
#define BLINK_CTRL_START 0x12
#define BLINK_CTRL_END 0x23
#define BLINK_CTRL_LEN REG_LEN(BLINK_CTRL_END, BLINK_CTRL_START)  // Length in Bytes of Blink Control Register

// PWM Register
#define PWM_REG_START 0x24
#define PWM_REG_END 0xB3
#define PWM_REG_LEN REG_LEN(PWM_REG_END, PWM_REG_START)

// *** End Frame Register Memory Map ***


// *** Function Register Memory Map ***

// Configuration Register
#define CFG_REG 0x00       // Configuration Register
#define PIC_DISP_REG 0x01  // Picture Display Register
#define AP_CTRL_REG1 0x02  // Auto Play Control Register 1
#define AP_CTRL_REG2 0x03  // Auto Play Control Register 2
#define DISP_OPT_REG 0x05  // Display Option Register
#define AUDIO_SYNC_REG     // Audio Synchronization Register


// Shutdown Register
#define SHUTDOWN_REG 0x0A
#define NORMAL_OPERATION 0x01
#define SHUTDOWN_MODE 0x00

// *** End Function Register Memory Map ***

#define CMD_BUF_LEN 2  // Length of Buffer sent to Charlie
#define REG_WR_MIN_LEN 2  // Length of 1 Byte Address + 1 Byte Register Data

#define I2C_BAUDRATE 100000

#define LEN_2ARG 2  // Length of two byte arguments

// Define FE310 Charlieplex Functions
extern void charlieplex_register_write_byte(struct metal_i2c *i2c, unsigned char reg_addr, unsigned char reg_data);
extern void charlieplex_set_cmd_reg(struct metal_i2c *i2c, unsigned char page_num);
extern struct metal_i2c *charlieplex_init(unsigned int baud);
extern void charlieplex_register_write_multibyte_data(struct metal_i2c *i2c, unsigned char page_num, unsigned char response_reg, unsigned char *data, unsigned int data_len);
extern void charlieplex_write_pixel(unsigned int x, unsigned int y, unsigned char duty_cycle);
extern void write_charlieplex_led_data(struct metal_i2c *i2c, unsigned char page_num, unsigned char *raw_data);

#endif //RISCV_CHARLIEWING_FE310_CHARLIEPLEX_H
