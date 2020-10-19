/*
    eyetooth.h                                                        
                                                                        
 Copyright 2019 John C Silvia                                           
                                                                        
 This file is part of eyetooth.                                            
                                                                        
    eyetooth is free software: you can redistribute it and/or modify       
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or   
    (at your option) any later version.                                 
                                                                        
    eyetooth is distributed in the hope that it will be useful,            
    but WITHOUT ANY WARRANTY; without even the implied warranty of      
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the       
    GNU General Public License for more details.                        
                                                                        
    You should have received a copy of the GNU General Public License   
    along with eyetooth.  If not, see <http://www.gnu.org/licenses/>.      
*/
#ifndef eyetooth_h
#define eyetooth_h

  
#include "TimerOne.h"
#include <Keyboard.h>
#include <EEPROM.h>


#define LED   2         // DIO pin with LED connected
#define FLASHPERSEC 4   // flash rate for LED

#define POLLPERIOD  2000   // 2 msec
#define DEBOUNCE    0x1F   // require 5 ups or downs in a row (10msec)

#define AFaddr ROWS * COLS // eeprom addr to save autofire rate (just past keys array) 
byte magicnum = 0xAA; // arbitrary byte written to eeprom (its absence means no valid data)

struct RowCol
{
  int row;
  int col;
};

volatile bool keystate[COLS][ROWS]; // array for current state of all keys
bool formerstate[COLS][ROWS];       // array of previous key state
volatile bool anykey;               // is any key currently pressed
bool sendkeys = false;              // should key presses be output
bool funkey = false;                // is F9 down
bool chorded = false;               // is F9 pressed in combination with 2nd key
 

int autofire = 50;        // rapid-fire keys every 50ms (default)
bool repeating = false;   // auto key repeat on/off

// msecs per LED blink
unsigned int blinkrate = (1000 / (POLLPERIOD/1000)) / FLASHPERSEC;

// map rows and columns to key
// will contain the ascii values used by Keyboard.write
char keys[ROWS][COLS];

// debounce history of key array 
// one byte per key, each bit is previous state of key
// 1 = up, 0 = pressed.
byte bouncy[COLS][ROWS];

int tick = 0;   // timer ticks updated by interrupt routine
int lit = 0;    // current state of LED

#endif
