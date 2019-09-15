/*
    nonvolatile.ino                                                        
                                                                        
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

// retrieve values from eeprom
void remember() {
  
  if(magicnum == EEPROM.read(AFaddr + 1)) {  // is valid data possible?
    autofire = (int)EEPROM.read(AFaddr);
    restore();
    if(!autofire) incept(); //initialize anyway if no value for autofire    
  } else {
    // if no valid keymap in eeprom use default
    primekeys();
  }
}

// save persistent values to eeprom
void record() {
  EEPROM.update(AFaddr, (byte)autofire);
  
  for (int i=0; i<ROWS; i++) {
    for (int j=0; j<COLS; j++) {
      EEPROM.update((i*COLS) + j, keys[i][j]);
    }
  }
  // mark valid data available
  EEPROM.update(AFaddr + 1, magicnum);
}

// restore persistent values from eeprom
void restore() {
  autofire = (int)EEPROM.read(AFaddr);

  for (int i=0; i<ROWS; i++) {
    for (int j=0; j<COLS; j++) {
      keys[i][j] = EEPROM.read((i*COLS) + j);
    }
  }
}
