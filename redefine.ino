/*
    redefine.ino                                                        
                                                                        
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

// reinitialize everything to default values
void incept() {
  autofire = 50;
  primekeys();
}


// change the definition of a key
void redefine() {  
  int   code = 0;
  struct RowCol locus;

  // signal that we're redefining
  light(HIGH);

  // get key to be redefined
  locus = changeling();
  flicker(500);   // long flash LED to signal got it

  // get decimal value for new key definition
  for(int i=100; i>0; i/=10) {
    code += getnum() * i;
    flicker(100);    // flash LED to signal got a number
  }
  // change char look-up table
  keys[locus.row][locus.col] = code;
  
  light(LOW);
}


// get the key to be redefined
RowCol changeling() {
  bool    curstate;
  struct RowCol locus = {ROWS, COLS};
  
  // wait for indication of key to be redefined
  while (locus.row == ROWS) {
    for (int i=0; i<ROWS; i++) {
      for (int j=0; j<COLS; j++) {
        curstate = keystate[j][i];

        // wait for key press/release
        if(curstate != formerstate[j][i]) {
          formerstate[j][i] = curstate;
          // only act on key release
          if(!curstate) {
            locus.row = i;
            locus.col = j;
          }
        }
      }
    }  
  }
  return (locus);
}

// get a numeric digit from keypad input
int getnum() {
  bool  curstate;
  int   row = ROWS;
  int   col = 0; 
  int   value;
  int   r, c;

  // wait for a number key
  while (row == ROWS) {
    // only process number keys
    for (int i=0; i<2; i++) {
      r = i ? NUMBER_ROW1 : NUMBER_ROW2 ;
      for (c=NUMBER_COL_START; c<=NUMBER_COL_END; c++) {
        curstate = keystate[c][r];

        // wait for key press/release
        if(curstate != formerstate[c][r]) {
          formerstate[c][r] = curstate;
          // only act on key release
          if(!curstate) {
            row = r;
            col = c;
          }
        }
      }
    }    
  }
  // calc key pressed from coordinates
  value = (row == NUMBER_ROW2) ? 5 : 0;
  value = (value + (col - (NUMBER_COL_START - 1))) % 10;

  return(value);
}
