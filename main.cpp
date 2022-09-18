typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;
typedef signed int s32;
typedef signed short s16;
typedef signed char s8;

enum ScreenDimensions { Width = 240, Height = 160 };

#define MEM_IO 0x04000000
#define MEM_VRAM 0x06000000

#define video_mode_reg (*(volatile u32 *)MEM_IO)
#define video_mem ((volatile u16 *)MEM_VRAM)

enum Colors {
  Black = 0x0000,
  Red = 0x001F,
  Lime = 0x03E0,
  Yellow = 0x03FF,
  Blue = 0x7C00,
  Mag = 0x7C1F,
  Cyan = 0x7FE0,
  White = 0x7FFF,
};

static u16 make_color(int red, int green, int blue) {
  return red | (green << 5) | (blue << 10);
}

int main(int argc, char **argv) {
  video_mode_reg = 0x403;
  for (int i = 0; i < Width * Height; ++i) {
    video_mem[i] = Red;
  }
  return 0;
}
