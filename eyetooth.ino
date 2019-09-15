/*
    eyetooth.ino                                                        
                                                                        
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

#include "keydefs.h"
#include "eyetooth.h"
  
void setup() {
  // initialize keys
  teething();

  // initialize timer1, and set a 2 msec period
  Timer1.initialize(POLLPERIOD);
  Timer1.attachInterrupt(scankeys);  

  // retrieve stored keymapping and key repeat rate
  remember();
  // values from eeprom still could be random. 
  // On first use, should reset all values (F9-C)
  // then save known state. (F9-F5)
    
  // config LED 
  pinMode(LED, OUTPUT);
  
  light(LOW);
  Keyboard.begin();
  sendkeys = true;
}  


void loop() {
  // act on keypad input
  eyetooth();
}
