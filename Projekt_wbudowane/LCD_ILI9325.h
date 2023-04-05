/**
 * \file lcdas.h
 */


#ifndef __LCD_SYMBOLS_INH
#define __LCD_SYMBOLS_INH


//#define OSCIL_ON 0x0000      //Oscillator -- moved to Open1786.h
#define ENTRYM   0x0003    //Entry Mode
#define HADRPOS_RAM_START 0x0050    //Horizontal RAM address position
#define HADRPOS_RAM_END   0x0051  //Horizontal RAM address position
#define VADRPOS_RAM_START 0x0052    //Vertical RAM address position Start
#define VADRPOS_RAM_END   0x0053    //Vertical RAM address position End
#define ADRX_RAM 0x20    //RAM address set X
#define ADRY_RAM 0x21    //RAM address set Y
#define DATA_RAM 0x0022 //RAM data

/**
 * Defines initial rotation of the screen.
 */
#define DISP_ORIENTATION  0  /* angle 0 90 */

#if  ( DISP_ORIENTATION == 90 ) || ( DISP_ORIENTATION == 270 )

/**
 * Screen width
 */
#define  LCD_MAX_X  320
/**
 * Screen height
 */
#define  LCD_MAX_Y  240

#elif  ( DISP_ORIENTATION == 0 ) || ( DISP_ORIENTATION == 180 )

/**
 * Screen width
 */
#define  LCD_MAX_X  240
/**
 * Screen height
 */
#define  LCD_MAX_Y  320

#endif


/**
 * \defgroup LCDColors Predefined colors for the LCD
 *
 * Colors in rgb565 format defined as 16-bit unsigned numbers ready to use by the programmer
 * @{
 */
#define LCDWhite           0xFFFF
#define LCDBlack           0x0000
#define LCDGrey            0xa534
#define LCDBlue            0x001F
#define LCDBlueSea         0x05BF
#define LCDPastelBlue      0x051F
#define LCDViolet          0xB81F
#define LCDMagenta         0xF81F
#define LCDRed             0xF800
#define LCDGinger          0xFAE0
#define LCDGreen           0x07E0
#define LCDCyan            0x7FFF
#define LCDYellow          0xFFE0
/**
 * @}
 */
#endif

#ifndef __LCD_ILI9325_H
#define __LCD_ILI9325_H

/*****************************
 *  LCD controler configuration
 */
void init_ILI9325(void);

#endif

