/*
    keydefs.h                                                        
                                                                        
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

// keyboard constants
#define COLS      7     // number of columns wired to keys
#define ROWS      6     // number of rows
#define LOADROW   5     // row # with save key
#define LOADCOL   3     // col # with save key
#define RETCOL    6     // col # with return key

#define NUMBER_ROW1       1
#define NUMBER_ROW2       0
#define NUMBER_COL_START  1
#define NUMBER_COL_END    5

int keypadCols[COLS] = {10, 16, 14, 15, 18, 19, 20};
int keypadRows[ROWS] =  {4, 5, 6, 7, 8, 9};

// Default mapping of keyboard matrix to key press values

char defaultkeys[ROWS][COLS] = {
    {char(0xB1), '6', '7', '8', '9', '0', char(0xC6)},
    {'j', '1', '2', '3', '4', '5', 'x'},
    {'m', char(0xB3), 'q', 'w', 'e', 'r', 't'},
    {'i', char(0x82), 'a', 's', 'd', 'f', 'g'},
    {char(0x80), char(0x81),'z', ' ', 'c', 'v', 'b'},
    {'y', 'u', 0, char(0xCA), 0, 0, char(0xB0)}
};
/*    
       0    1      2    3       4   5   6
   |  -----------------------------------------
 0 |  Esc   6      7    8       9   0   F5
 1 |  j     1      2    3       4   5   x
 2 |  m     Tab    q    w       e   r   t
 3 |  i     Alt    a    s       d   f   g
 4 |  Ctrl  Shift  z    Space   c   v   b
 5 |  y     u           F9             Return

 "y" and "u" only used on 39 key version
 */  
