# gbaos

**WARNING**: This is project has been superseded by
[BradypOS](https://github.com/tgesthuizen/BradypOS/).

This project provides a very WIP kernel and userland for the Gameboy
Advance, nicely linking into a GBA cartridge you can run on the actual
console or an emulator like `mgba`.

# Next steps

- Finish ROMFS reading
- Write a memory manager
- Figure out SWI handler and timer interrupts
- Load and relocate ELF files from ROMFS
- Run the first user process

... (distant future)
- Port the whole thing over to the RP2040
