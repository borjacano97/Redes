#include <time.h>
#include <stdio.h>
#include <locale.h>

int main(int argc, char** argv)
{
	setlocale(LC_ALL, "es_ES");
	time_t rawtime;
	tm * timeinfo;

  	time (&rawtime);
  	timeinfo = localtime (&rawtime);


	char buffer[256];

	strftime(buffer, 256, "Estamos en el año %Y\nHoy es %A, %R\n", timeinfo);
	puts(buffer);
	//std::cin.get();
}
