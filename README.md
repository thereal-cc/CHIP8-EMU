# Chip8-EMU

Basically the hello world equivalent of emulator develpment. This is a warmup for developing a Z80/6502 Emulator, so I hope you enjoy it!

## Changelog (Version 1.4.5)

Bug Fixes/Refactoring, plus the beginnings of a SChip Implementation. Likely will try to port this to the TI 84+ CE before I come back to implement Super Chip8.

## Installation  

Once SDL3 is installed on your system, follow these steps to install and run the Emulator:  

```bash  
git clone
cd CHIP8-EMU
make clean && make
./bin/chip8 ./path/to/rom
```  

## Roadmap  

I do not have any plans to implement Audio at this time, as I want to end up porting this to the TI 84 Plus CE, which obviously has no sound. Besides that, here's a few other features I'd want to add.

- [x] Improve Input Delay
- [x] Load Roms from the Command Line
- [x] Opcode Function Pointers
- [ ] SCHIP 1.0/1.1 Support
- [ ] TI-84 Plus CE Port
- [ ] GUI Rom Loading & Configuration

## Credits/Thanks  

Thank you to ZX Dunny, AEstus Vitae, Janitor Raus, and the EmuDev discord for their tough love and help with this project.
