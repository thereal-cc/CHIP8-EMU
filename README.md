# Chip8-CE

Port of my CHIP8 Emulator to the TI-84 Plus CE. Not running at 100% speed yet, which I attribute to the fact that it's written in C and not eZ80 assembly. To load roms, upload your .ch8 files to [this website](https://leong.sh/chip84ceromcreator/) and make sure the name is less then 20 characters. From there, just load the Appvar onto your calculator, and type in its name when prompted.  

## Changelog: Version 1.1

Quirks can be adjusted using the following keys:  
Math: Reset Flag Register (VF) to Zero  
Apps: Increment Index Register (Fx55, Fx65)  
PRGM: Wait for Vertical Sync Interrupt  
VARS: Clip Sprites instead of Wrapping  
Clear: Shift Operations only operate on VX  
Power (^): BNNN uses VX instead of V0  

The 6 squares at the top of the screen indicates whether or not quirks are on or off. A white square indicates intended behavior, while a green square indicates a quirk. Squares from left to right map to the description above going from top to bottom (Far left is VF, next is increment Index Register, etc)

## Installation  

Once the CEdev toolchain is installed, you can clone this repository and run the following commands to compile the emulator.

```bash  
git clone
cd Chip8-CE
make clean && make
```  

## Roadmap  

My two biggest goals at this point are improving speed and adding SChip 1.1 support. I'm also hoping to add a better UI for adjusting quirks and settings in the future.

## Credits/Thanks  

Thank you to ZX Dunny, AEstus Vitae, Janitor Raus, and the EmuDev discord for their tough love and help with this project.
