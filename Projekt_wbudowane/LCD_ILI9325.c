#include "LCD_ILI9325.h"
#include "Open1768_LCD.h"

void delay_ms(uint16_t ms)
{
   uint16_t i,j;
   for( i = 0; i < ms; i++ )
   {
      for( j = 0; j < 1141; j++ );
   }
}

void init_ILI9325(void) {
   //Run only if DeviceCode is 0x9325 or 0x9328

   lcdWriteReg(0xE5, 0x78F0); /* set SRAM internal timing */
   lcdWriteReg(0x01, 0x0100); /* set Driver Output Control */
   lcdWriteReg(0x02, 0x0700); /* set 1 line inversion */
   lcdWriteReg(0x03, 0x1030); /* set GRAM write direction and BGR=1 */
   lcdWriteReg(0x04, 0x0000); /* Resize register */
   lcdWriteReg(0x08, 0x0207); /* set the back porch and front porch */
   lcdWriteReg(0x09, 0x0000); /* set non-display area refresh cycle ISC[3:0] */
   lcdWriteReg(0x0A, 0x0000); /* FMARK function */
   lcdWriteReg(0x0C, 0x0000); /* RGB interface setting */
   lcdWriteReg(0x0D, 0x0000); /* Frame marker Position */
   lcdWriteReg(0x0F, 0x0000); /* RGB interface polarity */
   /*************Power On sequence ****************/
   lcdWriteReg(0x10, 0x0000); /* SAP, BT[3:0], AP, DSTB, SLP, STB */
   lcdWriteReg(0x11, 0x0007); /* DC1[2:0], DC0[2:0], VC[2:0] */
   lcdWriteReg(0x12, 0x0000); /* VREG1OUT voltage */
   lcdWriteReg(0x13, 0x0000); /* VDV[4:0] for VCOM amplitude */
   lcdWriteReg(0x07, 0x0001);
   delay_ms(200);
   /* Dis-charge capacitor power voltage */
   lcdWriteReg(0x10, 0x1090); /* SAP, BT[3:0], AP, DSTB, SLP, STB */
   lcdWriteReg(0x11, 0x0227); /* Set DC1[2:0], DC0[2:0], VC[2:0] */
   delay_ms(50);                      /* Delay 50ms */
   lcdWriteReg(0x12, 0x001F);
   delay_ms(50);                      /* Delay 50ms */
   lcdWriteReg(0x13, 0x1500); /* VDV[4:0] for VCOM amplitude */
   lcdWriteReg(0x29, 0x0027); /* 04 VCM[5:0] for VCOMH */
   lcdWriteReg(0x2B, 0x000D); /* Set Frame Rate */
   delay_ms(50);                      /* Delay 50ms */
   lcdWriteReg(0x20, 0x0000); /* GRAM horizontal Address */
   lcdWriteReg(0x21, 0x0000); /* GRAM Vertical Address */
   /* ----------- Adjust the Gamma Curve ---------- */
   lcdWriteReg(0x30, 0x0000);
   lcdWriteReg(0x31, 0x0707);
   lcdWriteReg(0x32, 0x0307);
   lcdWriteReg(0x35, 0x0200);
   lcdWriteReg(0x36, 0x0008);
   lcdWriteReg(0x37, 0x0004);
   lcdWriteReg(0x38, 0x0000);
   lcdWriteReg(0x39, 0x0707);
   lcdWriteReg(0x3C, 0x0002);
   lcdWriteReg(0x3D, 0x1D04);
   /* ------------------ Set GRAM area --------------- */
   lcdWriteReg(0x50, 0x0000); /* Horizontal GRAM Start Address */
   lcdWriteReg(0x51, 0x00EF); /* Horizontal GRAM End Address */
   lcdWriteReg(0x52, 0x0000); /* Vertical GRAM Start Address */
   lcdWriteReg(0x53, 0x013F); /* Vertical GRAM Start Address */
   lcdWriteReg(0x60, 0xA700); /* Gate Scan Line */
   lcdWriteReg(0x61, 0x0001); /* NDL,VLE, REV */
   lcdWriteReg(0x6A, 0x0000); /* set scrolling line */
   /* -------------- Partial Display Control --------- */
   lcdWriteReg(0x80, 0x0000);
   lcdWriteReg(0x81, 0x0000);
   lcdWriteReg(0x82, 0x0000);
   lcdWriteReg(0x83, 0x0000);
   lcdWriteReg(0x84, 0x0000);
   lcdWriteReg(0x85, 0x0000);
   /* -------------- Panel Control ------------------- */
   lcdWriteReg(0x90, 0x0010);
   lcdWriteReg(0x92, 0x0600);
   lcdWriteReg(0x07, 0x0133); /* 262K color and display ON */

   delay_ms(50);
}
