# Chip-8 Emulator (Version 1.4.2)

Basically the hello world equivalent of emulator develpment. I decided to develop this first before I try something more complex like a 6502 or Z80 Emulator, but I'm proud of the initial state this is in.

## Changelog (Version 1.4.2)

So much cleanup and fixes I don't even kow where to begin. Thank you to ZX Dunny, AEstus Vitae, and Janitor Raus for their tough love and help with this project.

## Installation  

Once SDL3 is installed on your system, follow these steps to install and run the Emulator:  

```bash  
git clone
cd CHIP8-EMU
make clean && make
./bin/chip8
```  

## Roadmap  

I do not have any plans to implement Audio at this time, as I want to end up porting this to the TI 84 Plus CE, which obviously has no sound. Besides that, here's a few other features I'd want to add.

- [x] Improve Input Delay
- [x] Load Roms from the Command Line
- [x] Opcode Function Pointers
- [ ] Super Chip-48 Support
- [ ] TI-84 Plus CE Port
- [ ] GUI Rom Loading
