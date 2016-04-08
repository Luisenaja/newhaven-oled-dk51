/*********************************************************************************
 * The MIT License (MIT)                                                         *
 * <p/>                                                                          *
 * Copyright (c) 2016 Bertrand Martel                                            *
 * <p/>                                                                          *
 * Permission is hereby granted, free of charge, to any person obtaining a copy  *
 * of this software and associated documentation files (the "Software"), to deal *
 * in the Software without restriction, including without limitation the rights  *
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell     *
 * copies of the Software, and to permit persons to whom the Software is         *
 * furnished to do so, subject to the following conditions:                      *
 * <p/>                                                                          *
 * The above copyright notice and this permission notice shall be included in    *
 * all copies or substantial portions of the Software.                           *
 * <p/>                                                                          *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR    *
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,      *
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE   *
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER        *
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, *
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN     *
 * THE SOFTWARE.                                                                 *
 *********************************************************************************/

/*********************************************************************************/
/********************* NEWHAVEN  NOTICE ******************************************/
/*********************************************************************************
 *  Pick one up today in the Newhaven Display shop!                              *
 * ------> http://www.newhavendisplay.com/nhd169160128asc3-p-9288.html           *
 *                                                                               *
 * This code is written for the Arduino Uno.                                     *
 * Copyright (c) 2015 - Newhaven Display International, LLC.                     *
 *                                                                               *
 * Newhaven Display invests time and resources providing this open source code,  *
 * please support Newhaven Display by purchasing products from Newhaven Display! *
 *                                                                               *
 *********************************************************************************/
 
#ifndef NEWHAVEN1_69_LIBRARY_H
#define NEWHAVEN1_69_LIBRARY_H

#include <stdbool.h>
#include <stdint.h>

/** @file
* @brief NewHaven 1.68 OLED display library for NRF51 DK
*/

#define TX_RX_BUF_LENGTH         254                 /**< SPI transaction buffer length. */

#define   SCL_PIN  ARDUINO_13_PIN    // SCL (serial mdoe) signal connected to pin 13
#define   SDI_PIN  ARDUINO_11_PIN    // SDI (serial mode) signal connected to pin 11
#define   SDO_PIN  ARDUINO_12_PIN
#define     E_PIN  ARDUINO_10_PIN    // E (6800 mode) signal connected to pin 10
#define    RD_PIN  ARDUINO_10_PIN    // /RD (8080 mode) signal connected to pin 10
#define    RW_PIN  ARDUINO_9_PIN    // R/W (6800 mode) signal connected to pin 9
#define    WR_PIN  ARDUINO_9_PIN    // /WR (8080 mode) signal connected to pin 9
#define   RES_PIN  ARDUINO_6_PIN    // /RES signal connected to pin 6
#define    CS_PIN  ARDUINO_5_PIN    // /CS signal connected to pin 5
#define    RS_PIN  ARDUINO_4_PIN    // RS signal connected to pin 4
#define    PS_PIN  ARDUINO_A0_PIN    // PS signal connected to pin A0
#define   CPU_PIN  ARDUINO_A1_PIN    // CPU signal connected to pin A1
#define   LVL_DIR  ARDUINO_A2_PIN    // DIR (direction control) signal of level shifter IC connected to pin A2
#define   LVL_OEN  ARDUINO_A3_PIN    // /OE (output enable) signal of level shifter IC connected to pin A3

#define TFT_WIDTH  160
#define TFT_HEIGHT 128

uint32_t send_spi_data_command(uint8_t * const p_tx_data, uint8_t * const p_rx_data, const uint16_t  txlen, const uint16_t  rxlen);

void write_command(uint8_t d);

void write_data(uint8_t d);

void OLED_SetColumnAddress_160128RGB(unsigned char x_start, unsigned char x_end);

void OLED_SetRowAddress_160128RGB(unsigned char y_start, unsigned char y_end);

void OLED_WriteMemoryStart_160128RGB();

void OLED_SetPosition_160128RGB(unsigned char x_pos, unsigned char y_pos);

void OLED_FillScreen_160128RGB(unsigned long color);

void draw_bitmap(const uint32_t *image, uint16_t bitmap_width, uint16_t bitmap_height);

void OLED_Init_160128RGB();

void tft_setup();

#endif //NEWHAVEN1_69_LIBRARY_H