# Arduino-MIDI-Clocker
Arduino based USB Midi to clock sync converter, Clock divider and Trigger sequencer


Its main purpose was to sync my small synths and pocket operators which dont have usb midi, but it turned out to be quite usable for directly triggering drums in a modular rack or drum machine. Pink LEDs are the triggers going to the 4 outputs. Green LEDs are individual resets for each channel. 
Thera are 3 shift buttons (delay, reset and subdivision) to acces different parameters with the knobs:
Mode 1 (not holding any button):
The Knobs change the division of the incoming midi clock in powers of two (half time double time, quadruple time)
Mode 2 (holding subdiv button):
This gives acces to other divisions of the midi clock. The Knobs change the clock division with odd multiples in between.
Mode 3 (holding reset button):
Lets you change the reset for each channel to 1 16th1, 1 eights , 1 quarter, 1 half, 1 bar, 4 bars, none)
Mode 4 (holding delay button):
The Knobs acces indivisual time delays the each channel from 0-50ms. This is for syncing multiple devices with a DAW. Im setting my midi clock sync output in ableton to be 50 ms early (-50ms). Then i have individual control over the latency for Gear that might be synced or triggered by one of the channels!
The values only get changed if the corresponding mode is on and the pot is moved a certain amount, and gets saved 5 seconds afterwards (all of these prameters like pulse duration can easily be modified in the code) to have the values the same after a new powerup!
The toggle switches are for breaking the ground connection to the triggered device to elimimate ground loop of needed.
BOM:
Arduino Pro Micro
4 Jacks
4 Pots (i used b50k but it doesnt really matter)
8 LEDs 
8 Resistors (>=470R )
3 push buttons 
4 toggle switches
