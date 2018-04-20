#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
	#include <iostream>



int main(int argc, char* argv[]) {
   /* Comprobar la ocurrencia de error y notificarlo con la llamada perror(3) */
	if (setuid(0) < 0)
	{
		char *errstr = strerror(errno);

		std::cout <<"ERROR: int function 'main'. Error code: " << errno << " Error message: "
					  << errstr << std::endl;

		//printf("ERROR: in function 'main'. Error code: %i	 .Error message: %s", errno, errstr);
 	}
    return 1;
}
