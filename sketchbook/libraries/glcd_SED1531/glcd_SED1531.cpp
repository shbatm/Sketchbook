/*
 * 06-02-2013 Start of porting glcdSED1531 library to arduino.
 * library porting from: 
 * http://sourceforge.net/projects/glcdsed1531lib/
 * 
 * 
 *    Target independent implementation of the GLCD SED1531 library.
 *    The dependent mappings of the I/O pins must be included before this
 *    file.
 *
 *    Copyright 2013 Peter van Merkerk
 *
 *    This file is part of the GLCD SED1531 library.
 *
 *    The GLCD SED1531 library is free software: you can redistribute it and/or
 *    modify it under the terms of the GNU General Public License as published
 *    by the Free Software Foundation, either version 3 of the License, or (at
 *    your option) any later version.
 *
 *    The GLCD SED1531 library is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 *    Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License along with
 *    the GLCD SED1531 library If not, see http://www.gnu.org/licenses/.
 * 
 * */

#include <glcd_SED1531.h>
#include "Arduino.h"
/* Fallback defines */

#if !defined(GLCD_FLASH)
#define GLCD_FLASH(type, name) const type name
#endif

#if !defined(GLCD_FLASH_ARRAY)
#define GLCD_FLASH_ARRAY(element_type, name) GLCD_FLASH(element_type, name[])
#endif

#if !defined(GLCD_FLASH_STRING)
#define GLCD_FLASH_STRING(name, value) GLCD_FLASH_ARRAY(char, name) = value
#endif

#if !defined(GLCD_FLASH_PTR)
#define GLCD_FLASH_PTR(type) const type*
#endif

#if !defined(GLCD_IO_DELAY_READ)
#define GLCD_IO_DELAY_READ()
#endif

#if !defined(GLCD_IO_DELAY_WRITE)
#define GLCD_IO_DELAY_WRITE()
#endif

#if !defined(GLCD_IO_INIT)
#define GLCD_IO_INIT()
#endif

/*Types*/

typedef GLCD_FLASH_PTR(char) glcd_font_data_ptr;

typedef struct glcd_font_struct
{
    unsigned char first;
    unsigned char interspacing;
    GLCD_FLASH_PTR(glcd_font_data_ptr) lookup;
} glcd_font_t;

typedef GLCD_FLASH_PTR(glcd_font_t) glcd_font_ptr;

typedef GLCD_FLASH_PTR(char) glcd_flash_data_ptr;
typedef GLCD_FLASH_PTR(char) glcd_flash_text_ptr;

/*glcd Commands*/
#define commandDisplayOff 					(0xAE)
#define commandDisplayOn  					(0xAF)
#define commandInitialLine(start) 			(0x40|(start))
#define commandSetPage(page) 				(0xB0 | (page))
#define commandSetCollumnHight(col_high) 	(0x10|(col_high))
#define commandSetCollumnLow(col_low) 		(col_low)
#define commandAdcNormal					(0xA0)
#define commandAdcReverse 					(0xA1)
#define commandRMW 							(0xE0) //RMW read modify write
#define commandDisplayNormal 				(0xA6)
#define commandDisplayReverse 				(0xA7)
#define commandEntireDisplayNormal 			(0xA4)
#define commandEntireDisplayOn 				(0xA5)
#define commandLcdBias1_8 					(0xA2)
#define commandLcdBias1_6 					(0xA3)
#define commandEnd 							(0xEE)
#define commandReset 						(0xE2)
#define commandPowerNone 					(0x28)
#define commandPowerFollower 				(0x29)
#define commandPowerRegulator 				(0x2A)
#define commandPowerBooster 				(0x2C)
#define commandPowerAll 					(0x2F)
#define commandElectronicControl(voltage) 	(0x80|(voltage))

/*glcd status bits*/
#define BusyStatus 		(0x80)
#define ADCStatus 		(0x40)
#define ON_OF_Status 	(0x20)
#define ResetStatus 	(0x10)

/*Colors*/
#define CLEAR 	(0x00)
#define SET 	(0x01)
#define INVERT 	(0x02)

/*Indicators*/
#define INDICATOR_0 (20)
#define INDICATOR_1 (31)
#define INDICATOR_2 (32)
#define INDICATOR_3 (57)
#define INDICATOR_4 (69)
#difene INDICATOR_5 (78)

glcd::glcd(){}
