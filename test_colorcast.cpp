#include <iostream>


int main(int argc, char const *argv[])
{
  uint32_t a = 0x610002;
  a = 0b011111110000000000000000;
  char* b = reinterpret_cast<char*>(&a);
  std::cout << (uint16_t)b[2] << ' ' << (uint32_t)*(b+1) << ' ' << (uint32_t)b[0] << std::endl;

  uint8_t c[4] = { 0x00, 0x0f, 0x00, 0x10 };
  uint32_t* d = reinterpret_cast<uint32_t*>(&c);
  std::cout << *d << std::endl;
  return 0;
}
