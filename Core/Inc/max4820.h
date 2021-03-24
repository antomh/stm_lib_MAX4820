/*
 * max4820.h
 *
 *  Created on: Mar 13, 2021
 *  Author: Anton Shein<anton-shein2008@yandex.ru>
 *  ----------------------------------------------------------------------
 *  Copyright (C) Anton Shein, 2021
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  any later Bversion.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *  ----------------------------------------------------------------------
 */

#ifndef INC_MAX4820_H_
#define INC_MAX4820_H_

#include "main.h"
#include "stdio.h"
#include "string.h"
#include "stdarg.h"

#define DEBUGMODE

#define SPIx            hspi1
#define CHIP_AMOUNT     4

#define MAX_PIN_0       (uint8_t)(1 << 0)
#define MAX_PIN_1       (uint8_t)(1 << 1)
#define MAX_PIN_2       (uint8_t)(1 << 2)
#define MAX_PIN_3       (uint8_t)(1 << 3)
#define MAX_PIN_4       (uint8_t)(1 << 4)
#define MAX_PIN_5       (uint8_t)(1 << 5)
#define MAX_PIN_6       (uint8_t)(1 << 6)
#define MAX_PIN_7       (uint8_t)(1 << 7)

/* Define structs for the init */

typedef struct
{
    uint8_t chip[CHIP_AMOUNT];      // if you fill chip[m] when m > CHIP_AMOUNT error occures
    uint8_t ch_am;
} MAX_TypeDefChip;


/*
 * @brief   Debug function
 */
void uart_print(char *print_data, ...);

/*
 * @brief   Init MAX4820 chip
 */
uint8_t max_init(MAX_TypeDefChip *max);

/*
 * @brief   Set state on OUT pins
 */
uint8_t max_set_out(MAX_TypeDefChip *max);

/*
 * @brief   Read data from MAX
 */
uint8_t max_read(MAX_TypeDefChip *max, uint8_t *pData);


#endif /* INC_MAX4820_H_ */
