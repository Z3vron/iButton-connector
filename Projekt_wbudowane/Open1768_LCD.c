/****************************************Copyright (c)**************************************************
**
**                                 http://www.powermcu.com
**
**--------------File Info-------------------------------------------------------------------------------
** File name:         Open1768.c
** Descriptions:      Has been tested SSD1289 & ILI9325
**------------------------------------------------------------------------------------------------------
** Created by:         AVRman
** Created date:      2012-3-10
** Version:            1.3
** Descriptions:      The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Descriptions:
********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "Open1768_LCD.h"
#include <stdlib.h>

#ifdef LCD_SDD1289
   #include "LCD_SDD1289.h"
#elif  LCD_ILI9325
   #include "LCD_ILI9325.h"
#else
   #define ADRX_RAM 0x00
   #define ADRY_RAM 0x01
   #define LCD_MAX_X  20
   #define LCD_MAX_Y  20
#endif

//Header needed because of inline function definition
void wait_delay(int count);

/*******************************************************************************
* Function Name  : Lcd_Configuration
* Description    : Configures LCD Control lines
* Input          : None
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
void lcdConfiguration(void)
{
   /* Configure the LCD Control pins
       set
       P0[23..20]
       P1[25..23]
       as output.
   */

   LPC_GPIO0->FIODIR |= PIN_RS | PIN_RD | PIN_CS | PIN_WR;
   LPC_GPIO1->FIODIR |= PIN_LE | PIN_DIR | PIN_EN;
   LPC_GPIO0->FIOSET = PIN_RS | PIN_RD | PIN_CS | PIN_WR;
   LPC_GPIO1->FIOSET = PIN_LE | PIN_DIR | PIN_EN;
}

/*******************************************************************************
* Function Name  : LCD_Send
* Description    : LCDд���
* Input          : - byte: byte to be sent
* Output         : None
* Return         : None
* Attention       : None
*******************************************************************************/
void lcdSend (uint16_t byte)
{
   LPC_GPIO2->FIODIR |= 0xFF;        /* P2.0...P2.7 Output */
   LCD_DIR(1)                        /* Interface A->B */
   LCD_EN(0)                         /* Enable 2A->2B */
   LPC_GPIO2->FIOPIN =  byte;        /* Write D0..D7 */
   LCD_LE(1)
   LCD_LE(0)                         /* latch D0..D7   */
   LPC_GPIO2->FIOPIN =  byte >> 8;   /* Write D8..D15 */
}

/*******************************************************************************
* Function Name  : wait_delay
* Description    : Delay Time
* Input          : - nCount: Delay Time
* Output         : None
* Return         : None
* Return         : None
* Attention       : None
*******************************************************************************/
__INLINE void wait_delay(int count)
{
   volatile unsigned int tmp = count;
   while(tmp--);
}

/*******************************************************************************
* Function Name  : LCD_Read
* Description    : LCD
* Input          : - byte: byte to be read
* Output         : None
* Attention       : None
*******************************************************************************/
uint16_t lcdRead (void)
{
   uint16_t valLSB, valMSB;

   LPC_GPIO2->FIODIR &= ~(0xFF);         /* P2.0...P2.7 Input */
   LCD_DIR(0);                           /* Interface B->A */
   LCD_EN(0);                            /* Enable 2B->2A */
   wait_delay(50);                       /* delay some times */
   valMSB = LPC_GPIO2->FIOPIN0;          /* Read D8..D15 */
   LCD_EN(1);                            /* Enable 1B->1A */
   wait_delay(50);                       /* delay some times */
   //value = (value << 8) |
   valLSB = LPC_GPIO2->FIOPIN0;          /* Read D0..D7 */
   LCD_DIR(1);
   return  (valMSB << 8 | valLSB);
}

/*******************************************************************************
* Function Name  : LCD_WriteIndex
* Output         : None
* Return         : None
* Attention       : None
*******************************************************************************/
void lcdWriteIndex(uint16_t index)
{
   /**********************************
   // ** nCS      ---\________/------*
   // ** RS       ----\______/-------*
   // ** nRD      -------------------*
   // ** nWR      -----\___/---------*
   // ** DB[0-15] ------[###]--------*
   **********************************/
   LCD_CS(0);
   LCD_RS(0);
   LCD_RD(1);
   lcdSend( index );
   wait_delay(50);
   LCD_WR(0);
   wait_delay(1);
   LCD_WR(1);
   LCD_CS(1);
}

/*******************************************************************************
* Function Name  : LCD_WriteData
* Output         : None
* Return         : None
* Attention       : None
*******************************************************************************/
void lcdWriteData(uint16_t data)
{
   /**********************************
   // ** nCS      ---\________/-----**
   // ** RS       ------------------**
   // ** nRD      ------------------**
   // ** nWR      -----\___/--------**
   // ** DB[0-15] ------[###]-------**
   **********************************/
   LCD_CS(0);
   LCD_RS(1);
   lcdSend( data );
   LCD_WR(0);
   wait_delay(1);
   LCD_WR(1);
   LCD_CS(1);
}

/*******************************************************************************
* Function Name  : LCD_ReadData
* Input          : None
* Output         : None
* Attention       : None
*******************************************************************************/
uint16_t lcdReadData(void)
{
   /**********************************
   // ** nCS      ---\________/-----**
   // ** RS       ------------------**
   // ** nRD      -----\___/--------**
   // ** nWR      ------------------**
   // ** DB[0-15] ------[###]-------**
   **********************************/
   uint16_t value;

   LCD_CS(0);
   LCD_RS(1);
   LCD_WR(1);
   LCD_RD(0);
   wait_delay(1);
   value = lcdRead();

   LCD_RD(1);
   LCD_CS(1);

   return value;
}

/*******************************************************************************
* Function Name  : lcdWriteReg
* Description    : Writes to the selected LCD register.
* Input          : - LCD_Reg: address of the selected register.
*                  - LCD_RegValue: value to write to the selected register.
* Output         : None
* Return         : None
* Attention       : None
*******************************************************************************/
void lcdWriteReg(uint16_t LCD_Reg,uint16_t LCD_RegValue)
{
   /* Write 16-bit Index, then Write Reg */
   lcdWriteIndex(LCD_Reg);
   /* Write 16-bit Reg */
   lcdWriteData(LCD_RegValue);
}

/*******************************************************************************
* Function Name  : lcdWriteReg
* Description    : Reads the selected LCD Register.
* Input          : None
* Output         : None
* Return         : LCD Register Value.
* Attention       : None
*******************************************************************************/
uint16_t lcdReadReg(uint16_t LCD_Reg)
{
   uint16_t LCD_RAM;

   /* Write 16-bit Index (then Read Reg) */
   lcdWriteIndex(LCD_Reg);
   /* Read 16-bit Reg */
   LCD_RAM = lcdReadData();
   return LCD_RAM;
}

/*******************************************************************************
* Function Name  : LCD_SetCursor
* Description    : Sets the cursor position.
* Input          : - Xpos: specifies the X position.
*                  - Ypos: specifies the Y position.
* Output         : None
* Return         : None
* Attention       : None
*******************************************************************************/
void lcdSetCursor(uint16_t Xpos, uint16_t Ypos)
{
    #if  ( DISP_ORIENTATION == 90 ) || ( DISP_ORIENTATION == 270 )

    uint16_t temp = Xpos;

   Xpos = Ypos;
   Ypos = ( LCD_MAX_X - 1 ) - temp;

   #elif  ( DISP_ORIENTATION == 0 ) || ( DISP_ORIENTATION == 180 )

   #endif


   lcdWriteReg(ADRX_RAM, Xpos );
   lcdWriteReg(ADRY_RAM, Ypos );
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
