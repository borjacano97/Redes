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
      memcpy(_data, name, MAX_NAME_SIZE);
      memcpy(_data, &x, sizeof(x));
      memcpy(_data, &y, sizeof(x));
    }

    int from_bin(char * data)
    {
      memcpy(name, data, MAX_NAME_SIZE);
      data += MAX_NAME_SIZE;
      memcpy(&x, data, sizeof(x));
      data += sizeof(x);
      memcpy(&y, data, sizeof(x));
      return 0;
    }

public:
    char name[MAX_NAME_SIZE];

    int16_t x;
    int16_t y;
};

int main(int argc, char **argv)
{
  Jugador one = Jugador("player one", 12, 354);
  one.to_bin();
  int fd = open("Player1.data", O_CREAT|O_WRONLY|O_TRUNC);
  write(fd, one.data(), one.size());
  close(fd);
  std::cout << "Done! Copied a file of "<<one.size()<<" bytes." << '\n';

}
