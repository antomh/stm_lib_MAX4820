/*
 * max4820.c
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

#include "max4820.h"

extern SPI_HandleTypeDef SPIx;
extern UART_HandleTypeDef huart1;

void uart_print(char *print_data, ...)
{
    char sbuf[1024];
    va_list argptr;
    va_start(argptr, print_data);
    vsprintf(sbuf, print_data, argptr);
    va_end(argptr);

    HAL_UART_Transmit(&huart1, (uint8_t*)sbuf, strlen(sbuf), HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart1, (uint8_t*)"\n\r", 2, HAL_MAX_DELAY);
}

uint8_t max_init(MAX_TypeDefChip *max)
{
    /* Define amount of chips */
    if (CHIP_AMOUNT < 1 || CHIP_AMOUNT > 4)
    {
        return HAL_ERROR;
    }
    max->ch_am = CHIP_AMOUNT;

    /* Reset MAX state */
    uint8_t tmpd[CHIP_AMOUNT] = {0x00};
    for (int i = 0; i < CHIP_AMOUNT; ++i)
        tmpd[i] = 0x00;
    HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);
    for (int i = 0; i < CHIP_AMOUNT; ++i)
    {
        HAL_SPI_Transmit(&SPIx, &tmpd[i], 1, HAL_MAX_DELAY);
    }
    HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);

    return HAL_OK;
}


uint8_t max_set_out(MAX_TypeDefChip *max)
{
    /* CS is low to start transmition */
    HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);

    /* Transmit state */
    for (int i = (max->ch_am - 1); i >= 0; --i)
    {
        HAL_SPI_Transmit(&SPIx, &(max->chip[i]), 1, HAL_MAX_DELAY);
    }

    /* Set state */
    HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);

    return HAL_OK;
}


uint8_t max_read(MAX_TypeDefChip *max, uint8_t *pData)
{
    /* CS is low to start transmition */
    HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);


    uint8_t null_dat = 0x00;
    /* Read state */
    for (int i = (max->ch_am - 1); i >= 0; --i)
    {
        HAL_SPI_TransmitReceive(&SPIx, &null_dat, (pData + i), 1, HAL_MAX_DELAY);
    }
#ifdef DEBUGMODE
    uart_print("MAX data: %x :: %x :: %x :: %x :: %x", pData, *(pData), *(pData+1), *(pData+2), *(pData+3));
#endif
    /* Transmit state */
    for (int i = (max->ch_am - 1); i >= 0; --i)
    {
        HAL_SPI_Transmit(&SPIx, (pData + i), 1, HAL_MAX_DELAY);
    }


    /* Set state */
    HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);

    return HAL_OK;
}
