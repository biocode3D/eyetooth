/*
    keys.ino                                                        
                                                                        
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

// keypad setup
void teething() {
  // Set all keypad pins as inputs 
  for (int i=0; i<COLS; i++)
  {
    pinMode(keypadCols[i], INPUT_PULLUP);
  }
  for (int i=0; i<ROWS; i++)
  {
    pinMode(keypadRows[i], INPUT_PULLUP);
  }
    
  // prepare debouncing array
  for(int i=0; i<ROWS; i++) {
    for(int j=0; j<COLS; j++) {
      bouncy[j][i] = 0xFF;  
    }
  }
}

// main loop
void eyetooth() {
  static int past;

  // wait for keyscan update
  //  by default runs every 2 msec
  if(past != tick) {
    
    // in autofire mode, send phony key events
    if(repeating) {
      blinkLED();
      if(tick >= autofire) {
        tick = 0;
        burst();
      }
    } else tick = 0;
    
    // handle key press/release events
    processkeys();

    // prepare for next scan
    past = tick;
  }
}


// take action when any key changes state
void processkeys() {  
  bool curstate;
 
  // look for standard key changes and dispatch them
  anykey = false;  
  for (int i=0; i<ROWS - 1; i++) { // last row handled below
    for (int j=0; j<COLS; j++) {
      curstate = keystate[j][i];
      
      if(curstate) anykey = true; // keep track if any key is down

      doakey(j,i,curstate); // process this key
    }  
  }  

// Rogue map doesn't have return key or short row
#ifdef ROGUE
  // Special handling of row with F9 for merc layout
  for (int j=0; j<COLS; j++) {
    // skip F9
    if(j != 3) {
      curstate = keystate[j][LOADROW];
      if(curstate) anykey = true;
      doakey(j,LOADROW,curstate);
    }
  }
#else
  // now handle special case of short row

  // first check for Return key event
  //   note that chording F9 - Return not supported
  curstate = keystate[RETCOL][LOADROW];
  
  if(curstate != formerstate[RETCOL][LOADROW]) {
    formerstate[RETCOL][LOADROW] = curstate;
    type(RETCOL,LOADROW,curstate);
    anykey |= curstate;
  }   

  #ifdef LEFT_KEYS
    // if left-side keys added
    for (int j=0; j<2; j++) {
      doakey(j,LOADROW,curstate);
  }
  #endif
#endif
  
  // handle special case of top button (F9 - Q.Load)
  // this is the point of handling this row differently
  curstate = keystate[LOADCOL][LOADROW];
  if(!anykey) {      // ignore altogether if any other key is already down
    
    // F9 key release
    if(formerstate[LOADCOL][LOADROW] && !curstate) { 
      if(!chorded && funkey) {        // this was regular keypress
        type(LOADCOL,LOADROW,true);   // send keydown
        // don't like doing the following
        // but prob harmless considering it's for reloading a save
        delay(F9DELAY);            // pause necessary(!) for some games
        type(LOADCOL,LOADROW,false);  // send keyup    
      } 
      // release from chording mode
      formerstate[LOADCOL][LOADROW] = false;
      chorded = false;
      funkey = false;
    // F9 key press
    } else if(!formerstate[LOADCOL][LOADROW] && curstate) {
      // enter chording mode only if no other key is down
      //   if a key _was_ down, no action will be taken on release
      //   and no key chords will be acknowleged
      if(!anykey) funkey = true;  
      // save keypress but don't send event, wait for release
      formerstate[LOADCOL][LOADROW] = true;
    }
  }
}

// process one key
void doakey(int j, int i, bool curstate) {
  if(curstate != formerstate[j][i]) {
    formerstate[j][i] = curstate;
    // are we handling a F9 chord event?
    if(funkey){           
      chorded = true;  // signal that this is NOT a Q.Save
      special(j,i,curstate);
    } else {                  
      type(j,i,curstate);
    }
  }
}


// send additional key release then key press
// for any keys that are currently down (except F9)
void burst() {
  for (int i=0; i<ROWS - 1; i++) {  // skips last row 
    for (int j=0; j<COLS; j++) {
      if(formerstate[j][i]) {       // is key down?
        if(!keystate[LOADCOL][LOADROW]) {  // skip if doing chord
          type(j,i,false);   // send keyup    
          type(j,i,true);    // send keydown
        }
      }
    }
  }
  // handle last row
  // allows autorepeat on Return key
  if(formerstate[RETCOL][LOADROW]) {          
    type(RETCOL,LOADROW,false);   // send keyup    
    type(RETCOL,LOADROW,true);    // send keydown
  }
  
#ifdef LEFT_KEYS
  // if left-side keys added
  for (int j=0; j<2; j++) {
    type(j,LOADROW,false);   // send keyup    
    type(j,LOADROW,true);    // send keydown
  }
#endif
}

// send keystrokes out on USB
void type(int col, int row, bool state){
  if(sendkeys) {
    if(state) Keyboard.press(keys[row][col]);
    else Keyboard.release(keys[row][col]);
  }
}


// handle keystrokes intended for internal use
void special(int col, int row, bool state) {

  // only act on key up
  if(state) return;

  int action = row + (col<<4);
  switch(action) {
    case 0x61:    // col 6, row 1 pressed (X)
      // toggle autofire 
      repeating = !repeating;
      
      if(!repeating) {
        // might be mid flash so turn off led
        light(LOW);
        // process key-up in case F9 still pressed
        if(!keystate[6][1]) {
          formerstate[6][1]=false;
          type(6,1,false);
        }
      }            
      unchord();
      break;
    case 0x32:    // col 3, row 2 pressed (W)
      // speed up autofire
      autofire -= 5;
      if(autofire < 5) autofire = 5; //no faster than ~5ms
      break;
    case 0x33:    // col 3, row 3 pressed (S)
      // slow down autofire
      autofire += 5;  
      if(autofire > 250) autofire = 250; //no slower than ~250ms (one bye of eeprom)
      break;
    case 0x60:    // col 6, row 0 pressed (F5)
      // save current settings
      record();  
      break;
#ifdef ROGUE      
    case 0x24:    // col 4, row 4 pressed (C)
#else  
    case 0x44:    // col 2, row 4 pressed (C)
#endif    
      // reset to default values
      incept();
      break;
#ifndef ROGUE      
    case 0x34:    // col 3, row 4 pressed (Space)
      // quit being a keyboard
      if(sendkeys) Keyboard.end();
      sendkeys = false;
      light(HIGH);
      break;
    case 0x00:    // col 0, row 0 pressed (Esc)
      // start being a keyboard
      if(!sendkeys) Keyboard.begin(); 
      sendkeys = true;
      light(LOW);
      break;
#endif      
    case 0x04:    // col 0, row 4 pressed (LCtrl)
      // redefine a key
      unchord();  // wait for F9 up  
      redefine();
      break;

    default:
      // ignore?
      ;
  }  
}

 
// wait for F9 release
void unchord() { 
  while(formerstate[LOADCOL][LOADROW]) {
    if(!keystate[LOADCOL][LOADROW]) {
      formerstate[LOADCOL][LOADROW] = false;
      chorded = false;
      funkey = false;    
    }  
  }
}


// copy default keymap to working keymap
void primekeys() {
  for (int i=0; i<ROWS; i++) {
    for (int j=0; j<COLS; j++) {
      keys[i][j] = defaultkeys[i][j];
    }
  }
}



// interrupt routine to check state of all keys
// also updates timer ticks
void scankeys() {
   
  noInterrupts();
  
  // increment timer tick (msecs)
  tick += POLLPERIOD/1000;

  for (int i=0; i<ROWS; i++) {
    // activate one row
    pinMode(keypadRows[i], OUTPUT);
    digitalWrite(keypadRows[i], LOW);
    
    // read all columns for this row
    for (int j=0; j<COLS; j++) {
      // shift current key state into byte for this key
      bouncy[j][i] = ((bouncy[j][i]<<1) | (byte)digitalRead(keypadCols[j])) & DEBOUNCE;
      // if key up for the last n scans (as determined by number of 1s in DEBOUCE
      if(bouncy[j][i] == DEBOUNCE) {
        keystate[j][i] = false;
        // if key down for the last n scans
      } else if(bouncy[j][i] == 0) {
        keystate[j][i] = true;
      }
    }
    
    // reset this row
    pinMode(keypadRows[i], INPUT_PULLUP);
  }
  interrupts();
}
