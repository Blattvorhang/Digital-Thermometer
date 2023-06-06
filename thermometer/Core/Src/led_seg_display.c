#include "led_seg_display.h"
#include <stdlib.h>
#include <stdio.h>

__IO uint8_t dig_array[4];


const uint8_t seg_table[16] = {
	0x3f,		//"0"
	0x06,		//"1"
	0x5b,		//"2"
	0x4f,		//"3"
	0x66,		//"4"
	0x6d,		//"5"
	0x7d,		//"6"
	0x07,		//"7"
	0x7f,		//"8"
	0x6f,		//"9"
	0x77,		//"A"
	0x7c,		//"b"
	0x39,		//"C"
	0x5e,		//"d"
	0x79,		//"E"
	0x71		//"F"
};

void select_digit(uint8_t dig);
void write_segment(uint8_t seg);
//void display_ascii_str(char* p);
	

void LEDSEG_Display_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, GPIO_PIN_SET);

  /*Configure GPIO pins : PB0 PB1 PB2 PB3
                           PB4 PB5 PB6 PB7 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PD0 PD1 PD2 PD3 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
}

void DisplayCallback(void)
{
	uint8_t dig;
	uint32_t tick = HAL_GetTick();
	if((tick%LED_SEG_REFRESH_PERIOD) == 0)
	{
		dig = (tick/LED_SEG_REFRESH_PERIOD)%4;
		select_digit(dig);
		if(dig_array[dig]== 0xff)		//digit is off
		{
			write_segment(0x00);			
		}
		else if((dig_array[dig]&0x80) != 0)		//display minus
		{
			write_segment(0x40);	
		}
		else if((dig_array[dig]&0x10) != 0)		//add dot
		{
			write_segment(seg_table[dig_array[dig]&0x0f] | 0x80);
		}
		else
		{
			write_segment(seg_table[dig_array[dig]&0x0f]);
		}
	
	}
}


void select_digit(uint8_t dig)
{
	switch(dig)
	{
		case 0:
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_SET);
			break;
		case 1:
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_SET);
			break;
		case 2:
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_SET);
			break;
		case 3:
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_RESET);
			break;
		
		default:
			break;
	}
}


void write_segment(uint8_t seg)
{
	HAL_GPIO_WritePin(GPIOB,0x00FF,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB,seg,GPIO_PIN_SET);
}
