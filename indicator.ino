/*
    indicator.ino                                                        
                                                                        
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
// turn off/on LED
void light(int state) {
  digitalWrite(LED, state);
  lit = state;
}

// handle flashing LED
void blinkLED() {       
  static unsigned int flasher;
  
  flasher += POLLPERIOD/1000; // incr by #msec since last pass
  if ( flasher % blinkrate == 0 ) {
    if(lit) {
      light(LOW);
    } else {
      light(HIGH);
    }
  }
}

// blink the LED for the designated time
void flicker(int msec) {  
  light(!lit);
  delay(msec);
  light(!lit);
}
