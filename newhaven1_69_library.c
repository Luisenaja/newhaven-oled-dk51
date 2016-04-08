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

/************************************************************************************
 *                                                                                  *
 * Integration of NewHaven OLED 1.69 display on Nordic Semiconductor NRF51 DK board *
 * Based on project https://github.com/NewhavenDisplay/NHD-1.69-160128ASC3_Example  *
 *                                                                                  *
 ************************************************************************************/

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

#include <stdbool.h>
#include <stdint.h>
#include "newhaven1_69_library.h"
#include "common.h"
#include "nrf_drv_spi.h"
#include "SEGGER_RTT.h"
#include "boards.h"
#include "app_util_platform.h"

static const nrf_drv_spi_t m_spi_master = NRF_DRV_SPI_INSTANCE(0);

// Data buffers.
static uint8_t m_tx_data[TX_RX_BUF_LENGTH] = {0}; /**< A buffer with data to transfer. */
static uint8_t m_rx_data[0] = {};                 /**< A buffer for incoming data. */

/** @file
* @brief Common functions used by display library
*
*/

uint32_t send_spi_data_command(uint8_t * const p_tx_data, uint8_t * const p_rx_data, const uint16_t  txlen, const uint16_t  rxlen)
{

    uint32_t err_code = nrf_drv_spi_transfer(&m_spi_master, p_tx_data, txlen, p_rx_data, rxlen);

    if (err_code != NRF_SUCCESS) {
        SEGGER_RTT_printf(0, "\x1B[31m[ERROR] on transfer :%d Resetting...\x1B[0m\n", err_code);
        return err_code;
    }
    return err_code;
}

void write_command(uint8_t d)       // send command to OLED
{
    m_tx_data[0] = d;
    digitalWrite(RS_PIN, LOW);
    send_spi_data_command(m_tx_data, m_rx_data, 1, 0);
}

void write_data(uint8_t d)       // send data to OLED
{
    m_tx_data[0] = d;
    digitalWrite(RS_PIN, HIGH);
    send_spi_data_command(m_tx_data, m_rx_data, 1, 0);
}

void OLED_SetColumnAddress_160128RGB(unsigned char x_start, unsigned char x_end)    // set column address start + end
{
    write_command(0x17);
    write_data(x_start);
    write_command(0x18);
    write_data(x_end);
}

void OLED_SetRowAddress_160128RGB(unsigned char y_start, unsigned char y_end)    // set row address start + end
{
    write_command(0x19);
    write_data(y_start);
    write_command(0x1A);
    write_data(y_end);
}


void OLED_WriteMemoryStart_160128RGB()    // write to RAM command
{
    write_command(0x22);
}

void OLED_SetPosition_160128RGB(unsigned char x_pos, unsigned char y_pos)    // set x,y address
{
    write_command(0x20);
    write_data(x_pos);
    write_command(0x21);
    write_data(y_pos);
}

void OLED_FillScreen_160128RGB(unsigned long color)    // fill screen with a given color
{
    OLED_SetPosition_160128RGB(0, 0);
    OLED_WriteMemoryStart_160128RGB();

    for (uint16_t y = TFT_HEIGHT; y > 0; y--) {
        for (uint16_t x = TFT_WIDTH; x > 0; x--) {
            write_data(((color >> 16) & 0b11111100));
            write_data(((color >> 8) & 0b11111100));
            write_data((color & 0b11111100));
        }
    }
}

void draw_bitmap(const uint32_t *image, uint16_t bitmap_width, uint16_t bitmap_height)
{
    int i = 0;
    OLED_SetPosition_160128RGB(0, 0);
    OLED_WriteMemoryStart_160128RGB();

    for (uint16_t y = 0; y < bitmap_height; y++) {

        for (uint16_t x = 0; x < bitmap_width; x++) {

            write_data(((image[i] >> 16) & 0b11111100));
            write_data(((image[i] >> 8) & 0b11111100));
            write_data((image[i] & 0b11111100));

            i++;
        }
    }
}

void OLED_Init_160128RGB()      //OLED initialization
{
    digitalWrite(RES_PIN, LOW);
    delay(2);
    digitalWrite(RES_PIN, HIGH);
    delay(2);

    // display off, analog reset
    write_command(0x04);
    write_data(0x01);
    delay(1);

    // normal mode
    write_command(0x04);
    write_data(0x00);
    delay(1);

    // display off
    write_command(0x06);
    write_data(0x00);
    delay(1);

    // turn on internal oscillator using external resistor
    write_command(0x02);
    write_data(0x01);

    // 90 hz frame rate, divider 0
    write_command(0x03);
    write_data(0x30);

    // duty cycle 127
    write_command(0x28);
    write_data(0x7F);

    // start on line 0
    write_command(0x29);
    write_data(0x00);

    // rgb_if
    write_command(0x14);
    write_data(0x31);

    // Set Memory Write Mode
    write_command(0x16);
    write_data(0x76);

    // driving current r g b (uA)
    write_command(0x10);
    write_data(0x45);
    write_command(0x11);
    write_data(0x34);
    write_command(0x12);
    write_data(0x33);

    // precharge time r g b
    write_command(0x08);
    write_data(0x04);
    write_command(0x09);
    write_data(0x05);
    write_command(0x0A);
    write_data(0x05);

    // precharge current r g b (uA)
    write_command(0x0B);
    write_data(0x9D);
    write_command(0x0C);
    write_data(0x8C);
    write_command(0x0D);
    write_data(0x57);

    // Set Reference Voltage Controlled by External Resister
    write_command(0x80);
    write_data(0x00);

    // mode set
    write_command(0x13);
    write_data(0xA0);

    OLED_SetColumnAddress_160128RGB(0, 159);
    OLED_SetRowAddress_160128RGB(0, 127);

    // Display On
    write_command(0x06);
    write_data(0x01);
}

void tft_setup()
{
    pinMode(LVL_OEN, OUTPUT);                   // configure LVL_OEN as output
    digitalWrite(LVL_OEN, LOW);
    pinMode(LVL_DIR, OUTPUT);                   // configure LVL_DIR as output
    digitalWrite(LVL_DIR, HIGH);
    pinMode(RS_PIN, OUTPUT);                    // configure RS_PIN as output
    pinMode(RES_PIN, OUTPUT);                   // configure RES_PIN as output
    pinMode(PS_PIN, OUTPUT);                    // configure PS_PIN as output
    pinMode(CPU_PIN, OUTPUT);                   // configure CPU_PIN as output
    digitalWrite(LVL_OEN, LOW);
    digitalWrite(CS_PIN, HIGH);                 // set CS_PIN
    digitalWrite(PS_PIN, LOW);                  // reset PS_PIN

    nrf_drv_spi_config_t const config =
    {
        .sck_pin  = SCL_PIN,
        .mosi_pin = SDI_PIN,
        .miso_pin = SDO_PIN,
        .ss_pin   = CS_PIN,
        .irq_priority = APP_IRQ_PRIORITY_LOW,
        .orc          = 0xFF,
        .frequency    = NRF_DRV_SPI_FREQ_8M,
        .mode         = NRF_DRV_SPI_MODE_0,
        .bit_order    = NRF_DRV_SPI_BIT_ORDER_MSB_FIRST,
    };

    ret_code_t err_code = nrf_drv_spi_init(&m_spi_master, &config, NULL);
    APP_ERROR_CHECK(err_code);

    OLED_Init_160128RGB();
}
