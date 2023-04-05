#include "LPC17xx.h"                	// Device header
#include "PIN_LPC17xx.h"            	// Keil::Device:PIN
#include "Board_LED.h"                  // ::Board Support:LED
#include "Open1768_LCD.h"
#include "LCD_ILI9325.h"
#include "asciiLib.h"
#include "Board_Joystick.h"             // ::Board Support:Joystick

#include <string.h>
#include <stdbool.h>

#define CODE_BYTES 8
#define BYTE 8

volatile uint32_t us_ticks = 0;
volatile uint16_t isBlinking = 0;

void SysTick_Handler(void)
{
	++us_ticks;
}

void delay(uint32_t time_in_us)
{
    us_ticks = 0;
    while(us_ticks < time_in_us);
}

void print(char string[])
{
	for (int i = 0; i < strlen(string); ++i)
	{
		while ((LPC_UART0->LSR & (1<<5)) == 0);
		LPC_UART0->THR = string[i];
	}
}

void clearScreen()
{
	lcdWriteReg(ADRX_RAM,0);
	lcdWriteReg(ADRY_RAM,0);		
	for (uint16_t i = 0; i < LCD_MAX_X ; i++){
		for (uint16_t j = 0; j < LCD_MAX_Y; j++){
			lcdWriteReg(DATA_RAM,LCDWhite);
		}
	}
}

void writeASCII(unsigned char c, unsigned x, unsigned y)
{
		unsigned char buffer[16];
		GetASCIICode(1, buffer, c);
		
		for (uint16_t i = 0; i < 16 ; i++){
			unsigned char mask = 1;
			for (uint16_t j = 0; j < 8; j++){
				if ((buffer[i] & mask) != 0)
				{
					lcdWriteReg(ADRX_RAM,i + x);
					lcdWriteReg(ADRY_RAM,j + y);
					lcdWriteReg(DATA_RAM,LCDBlack);
				}
				mask<<=1;
			}
		}
}

void writeString(char string[], unsigned x, unsigned y)
{
	for (int i = 0; i < strlen(string); i++)
	{
		writeASCII(string[i], x, y + (-i) * 8);
	}
}

uint32_t checkPin()
{
	if ((LPC_GPIO2->FIOPIN & (1<<11)) != 0)
		return 1;
	else
		return 0;
}

void sendZero()
{
    LPC_GPIO2->FIODIR |= (1<<11);
}

void sendOne()
{
    LPC_GPIO2->FIODIR &= ~(1<<11);
}

void resetAndPresence()
{
	sendZero();
	delay(480);
	sendOne();
	delay(70);

	/*if(checkPin() == 0)
		writeString("Init success", 100, 300);
	else
		writeString("Init failure", 100, 300);*/
}

void writeByte(char byte)
{
	char mask = 1;
	
	for (int i = 0; i < BYTE; ++i)
	{
		if ((byte & mask) != 0)
		{
			sendZero();
			delay(8);		
			sendOne();
			delay(70);
		}
		else
		{
			sendZero();
			delay(100);		
			sendOne();
			delay(10);
		}
		mask <<= 1;
	}
}

char readByte()
{
	char result = 0;
	char mask = 1;
	for (int i = 0; i < BYTE; ++i)
	{
		sendZero();
		delay(3);		
		sendOne();
		delay(15);
		if (checkPin() == 1)
		{
			result |= mask;
			print("1");
		}
		else
			print("0");
		delay(45);
		mask <<= 1;
	}
	
	return result;
}

void readCode(char inputCode[])
{
	for (int i = 0; i < CODE_BYTES; ++i)
	{
		inputCode[i] = readByte();
		print(" ");
	}
}

char areCodesEqual(char firstCode[], char secondCode[], uint16_t numberOfBytesToCheck)
{
	for (int i = 0; i < numberOfBytesToCheck; ++i)
		if (firstCode[i] != secondCode[i])
			return 0;

	return 1;
}

void configureValidCode(char code[])
{
	clearScreen();
	writeString("Configure code", 100, 300);
	readCode(code);
}

void printCode(char code[]){
	print(code);
}

uint16_t isIbutton(char code[]){
	return code[0] == 12 && code[4] == code[5] == code[6] == 0;
}

void turnOnIbuttonLed(){
	LPC_GPIO0->FIOSET = 0xffffffff;
}

void turnOffIbuttonLed(){
	LPC_GPIO0->FIOCLR = 0xffffffff;
}
	
void startBlink(){
	while (1){
		turnOnIbuttonLed();
		delay(500000);
		turnOffIbuttonLed();
		delay(500000);
	}
}

int main()
{
	LPC_GPIO0->FIODIR = 0xffffffff;
	Joystick_Initialize();
	LPC_UART0->LCR = 0b10000011;
	LPC_UART0->DLL = 10;
	LPC_UART0->DLM = 0;
	LPC_UART0->LCR = 0b11;
	LPC_UART0->FDR = (5<<0) | (14<<4);	
	PIN_Configure(0,2,1,0,0);
	PIN_Configure(0,3,1,0,0);
	
	SysTick_Config(SystemCoreClock / 1000000);
	PIN_Configure(2, 11, 0, PIN_PINMODE_TRISTATE, 0);
	
	LPC_GPIO2->FIOCLR = 1<<11;

	lcdConfiguration();
	init_ILI9325();
	clearScreen();
	char validCode[CODE_BYTES] = {12, 223, 220, 37, 0, 0, 0, 107};
	char currentCode[CODE_BYTES];
	char emptyCode[CODE_BYTES] = {255, 255, 255, 255, 255, 255, 255, 255};
	
	resetAndPresence();
	while (1)
	{
		if (Joystick_GetState() == JOYSTICK_CENTER)
		{
			
			print("Change valid code\n\r");
			turnOnIbuttonLed();
			writeString("Change valid code", 100, 210);
			readCode(currentCode);
			while (!isIbutton(currentCode))
			{
				print("\n\r");
				writeByte(0x33);
				readCode(currentCode);
				delay(500000);
			}
			memcpy(validCode, currentCode, CODE_BYTES);
			clearScreen();
			writeString("Code changed - press center button", 100, 290);
			while (1)
			{
				if (Joystick_GetState() == JOYSTICK_CENTER ){
					delay(1000000);
					turnOffIbuttonLed();
					break;
				}
			}
			
		}
		writeByte(0x33);
		readCode(currentCode);	
		print("\n\r");
		if (areCodesEqual(currentCode, emptyCode, CODE_BYTES))
			clearScreen(); 
		else if (areCodesEqual(validCode, currentCode, CODE_BYTES)){
			writeString("Access granted", 100, 200);
			turnOnIbuttonLed();
			delay(1000000);
			clearScreen();			
			turnOffIbuttonLed();
		}
		else{
			writeString("Access denied", 100, 200);
			delay(1000000);
			clearScreen();
		}

	}
    return 0;
}
