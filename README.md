# Chip8-EMU

Basically the hello world equivalent of emulator develpment. This is a warmup for developing a Z80/6502 Emulator, so I hope you enjoy it!

## Changelog (Version 1.5.2)

Rom Loading is now done from system file manager, meaning path's do not have to be pasted into command line. Emulator only excepts .ch8 files. Rom Selection uses [TinyFileDialogs](https://sourceforge.net/projects/tinyfiledialogs/).  
Additionally, Quirks can now be turned on/off using function keys. Refer to this chart below (note all quirks are off by default)  
F1: Reset Flag Register (VF) to Zero  
F2: Increment Index Register (Fx55, Fx65)  
F3: Wait for Vertical Sync Interrupt  
F4: Clip Sprites instead of Wrapping  
F5: Shift Operations only operate on VX  
F6: BNNN uses VX instead of V0  

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
- [ ] SCHIP 1.0/1.1 Support
- [ ] TI-84 Plus CE Port
- [x] GUI Rom Loading & Configuration

## Credits/Thanks  

Thank you to ZX Dunny, AEstus Vitae, Janitor Raus, and the EmuDev discord for their tough love and help with this project.
