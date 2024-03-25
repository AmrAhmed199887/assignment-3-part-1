#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>


int main(int argc, char*argv[])
{


if(argc == 3)
{

FILE *ptr;

ptr = fopen(argv[1],"w");

if(ptr == NULL)
{
openlog(NULL,0,LOG_USER);
syslog(LOG_ERR,"can't create the file");
exit(1);
}

fputs(argv[2],ptr);

fclose(ptr);

openlog(NULL,0,LOG_USER);
syslog(LOG_DEBUG,"Writing %s to %s",argv[1],argv[2]);

}
else
{
openlog(NULL,0,LOG_USER);
syslog(LOG_ERR,"invaild number of arguments : %d",argc);

}

return 0;
}
