# Eyetooth Gaming Keypad

## DIY keypad inspired by the Ideazon Fang Gamepad

#### Introduction
This is a 37 key gaming keypad set up for the common fps/rpg key mapping, *although that can be changed; see below*.  The software is written for an arduino pro micro board.  Any arduino with sufficient digital IO would probably work.

This project was inspired by the death of my preferred gamepad. None of the currently available gamepads have a layout I like (not to mention the price). 
I also wanted to add a rapid auto key press feature. Some (single-player) games require you to repeatedly press a button at some point. I am physically unable to press the button at the rate many of these games require causing me to get stuck at that point. So I added a mode that will rapidly repeat any keys that are held down.
Additional information may be available on [hackaday](https://hackaday.io/project/167675-eyetooth-gaming-keypad) .

The software now also supports the [Rogue keypad](https://thingiverse) , a similar keypad inspired by the Ideazon Merc keyboard.

#### Features
- key redefinition
- auto key repeat with adjustable repeat rate
- n-key rollover
- software debouncing
 
#### Hardware
Parts:

arduino pro micro
37 cherry MX style switches
37 keycaps 18mm wide
1 keycap 23mm wide
37 1n4148 diodes
5mm LED
220 - 470 ohm resistor for LED
10  20mm long 3mm screws
1 usb micro cable (Anker PowerLine Micro USB)
1 zip tie for cable strain relief
double-sided foam tape to mount the arduino
1 optional 6mm tactile pushbutton switch (for reset)
misc wire
3D printed enclosure

3D Printed Parts:
If using my design for the keyboard chassis, Print with 3 or 4 walls and *no support*.  Support is designed in. The support for the side openings is easily removed and there is a thin layer in the countersunk holes which can be drilled out or even just pushed through.
Test pieces are provided to test the fit of your parts.  The switches push in from the top and should snap into place.  They can pull out however, and if this is a problem, they can be fixed into place with hot glue.
"Top.stl" uses cheap 3mm inserts from ebay.  "Top No Inserts.stl" allows you to just screw the 3mm screws directly into the plastic. This is probably strong enough but this file is untested.  "Bottom Dual Side.stl" has buttons on both sides for a 39-key variant.

The CAD files are available on [Onshape](https://cad.onshape.com/documents/3ab936743b476bf7fd663899/w/58ff50721779d46cd0447fc8/e/3268b355f3ac0041688e7b78) 

WireGuide.png indicates how the switches are connected in the default configuration. keypad.ods is a spreadsheet representation of the keyboard matrix.  
When actually wiring, all keys on a column are directly connected. Each key on a row is connected to the bus for that row through a diode. The diodes are wired with the cathodes in common and the  anode connected to the switch. (the cathode is the end with the band) See, for example, [baldengineer](www.baldengineer.com/share-projects-four-platforms.html) for a complete explaination and schematics; CAUTION: note that my definition of row and column is reversed from that given there.

#### Software
Program with the arduino IDE.  The default key definitions are in keydefs.h and can be edited as desired.  However, there are some restrictions on what will work when using the F9 chord commands. Then number keys 1-5 and 6-0 must each be contiguous left to right. Also F9 must on the last row of the keymap which asigns it to the last DIO pin defined in keypadRows[ROWS] (pin 9 by default).  

On linux, a command line program, showkeys, is very handy for testing. 

The code is written to favor clarity over efficiency since I expect it to spend most of its time looping while waiting for something to happen.  That said, it has only been tested on a 5v 16mHz pro micro. There should be no problem with the 3v 8mHz version, but I can give no guarantee.

There is an option to build a 39 key version which adds 'y' and 'u' keys as left-side buttons. To enable this, uncomment "#define LEFT_KEYS" in eyetooth.ino  The firmware could support 42 keys with trivial modification.

There is an option for the Rogue keypad. To enable this, uncomment "#define ROGUE" in eyetooth.ino  The firmware could support 42 keys with trivial modification.

If you like the physical design but don't want to use this software, you can use the popular [QMK](qmk.fm) firmware, but the customization will be up to you.


#### Instructions
When powered up, the first thing it does is check the eeprom for a "valid data" flag. If found it loads the stored keymap and repeat rate.  It does a validity check but it still might load random values. Therefore, on first use, you should reset all values (F9-C) to their defaults then save to eeprom (F9-F5).

It works like any usb keyboard.
Except for the F9 key chords:

F9 key plus
- Esc&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-> start being keyboard
- Space&nbsp;&nbsp;&nbsp;-> quit keyboard
- X&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-> toggle autofire
- W&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-> faster autofire
- S&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-> slower autofire
- F5&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-> save to eeprom
- C&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-> restore defaults
- Ctrl&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-> redefine key

The F9 chords are configured to the default key positions and ignore any redefinitions
To execute one of these command release all other keys, then press and hold F9. While holding F9 press and release the key for the desired function. Then release F9.

Esc and Space chords are there as insurance that I could reprogram the arduino durring development without too much trouble.  As it turned out I never needed it, but it might be handy in the future.

F9-X chord toggles the auto repeat on and off.  When on the LED flashes on and off.  In repeat mode, any key that is down repeats at 50 msec intervals.

F9-W chord speeds up the repeat rate.  Every time this chord is entered the repeat interval decreases by 5msec up to a maximum of repeating every 5 msec.

F9-S chord slows the repeat rate in the same manner as the W key down to 250msec per repeat.

F9-F5 chord stores the current key map and repeat rate.  Until this is done unplugging the keypad will lose any changes.  You should only use this after testing your current settings.

 F9-C chord restores the default values for the keymap and repeat rate.  F9-C followed by F9-F5 reset to factory values.
 
 F9-Ctrl chord allows you to redefine any key.  When this chord is entered the LED will come on indicating that it is waiting for the key to be redefined.  Press and relase the key you want to redefine. The LED will blink in acknowlegement. Next use the number keys to enter the three digit decimal ascii value of the new definition. (use a leading zero assuming your code is below 100)  The LED will blink as each number is entered, then turn off.  At this point, the new value will be lost if you unplug the keypad or use F9-C. To keep the new value, use F9-F5.
 
For the Rogue keypad: F9-X becomes F9-PrtSc. F9-Esc and F9-Space functions are omitted.

 