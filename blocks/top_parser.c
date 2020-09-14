#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main()
{
    float usage;
    char name[100];
    while(1)
    {
        FILE *tp = fopen("/tmp/top_grepped", "r");
        usleep(2050000);
        fgets(name, 100, tp);
        pclose(tp);
        for(int i = 95; i < 100; i++)
            if(name[i] == ','){ name[i] = '.'; break; }
        sscanf(&name[94], "%f", &usage);
        for(int i = 0; i < 100; i++)
            if(name[i] == ' ')
            {
                if(name[i-7] == 'P' && name[i+1] == 'F')
                {
                    name[i] = '_';
                    if(name[i+6] == ' ') name[i+6] = '_';
                }
                else { name[i] = 0; break; }
            }
        if(strlen(name) > 15)
            if(name[0] == '/' || name[0] == '.' || name[1] == ':')
            {
                for(int i = 0; i < 100; i++)
                {
                    if(name[i] == 0)
                    {
                        for(int j = i; j >= 0 ; j--)
                        {
                            if(name[j] == '/' || name[j] == '\\')
                            {
                                int k;
                                for(k = 0; name[j+k] != 0; k++)
                                    name[k] = name[j+k+1];
                                name[k-1] = 0;
                                break;
                            }
                        }
                        break;
                    }
                }
            }
	char out[25];
	int len = strlen(name);
	strcpy(out, name);
	for(int i = 0; i < 16-len; i++)
	    out[i+len] = ' ';
	int x = 15;
	if(len > 16)
	    out[15] = '+';
	if(usage < 10)
	    x++;
	sprintf(&out[x], " %.1f%%", usage);
        FILE *of = fopen("/tmp/top_process", "w+");
	fputs(out, of);
	fclose(of);
    }
}
