#include "Serializable.h"

#include <iostream>
#include <string>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#define MAX_NAME_SIZE 80

class Jugador: public Serializable
{
public:
    Jugador(const char * _n, int16_t _x, int16_t _y):x(_x),y(_y)
    {
        strncpy(name, _n, MAX_NAME_SIZE);
    };

    virtual ~Jugador(){};

    void to_bin()
    {
      alloc_data(sizeof(x) + sizeof(y) + MAX_NAME_SIZE);
      char* p = _data;
      p += sizeof(_size);
      memcpy(p, name, MAX_NAME_SIZE);
      p += MAX_NAME_SIZE;
      memcpy(p, &x, sizeof(x));
      p += sizeof(x);
      memcpy(p, &y, sizeof(x));
    }

    int from_bin(char * data)
    {
      memcpy(&_size, data, sizeof(int32_t));
      alloc_data(_size);
      data += sizeof(_size);
      memcpy(name, data, MAX_NAME_SIZE);
      data += MAX_NAME_SIZE;
      memcpy(&x, data, sizeof(x));
      data += sizeof(x);
      memcpy(&y, data, sizeof(y));
      return 0;

    }

public:
    char name[MAX_NAME_SIZE];

    int16_t x;
    int16_t y;
};

int main(int argc, char **argv)
{
  Jugador one = Jugador("", 3, 3);
  int fd = open("Player1.data", O_RDONLY);
  char buf[3000];
  read(fd, &buf, 3000);
  one.from_bin(buf);
  close(fd);
  std::cout << "Player: "<< one.name<< " is on: "<<one.x <<", "<< one.y << '\n';
  return 0;
}
