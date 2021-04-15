#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <uchar.h>

unsigned long long int total, idle, *lastTotal, *lastIdle;
static const unsigned int bar_height = 18;

int get_thread_count()
{
    FILE *tp = popen("nproc", "r");
    char ts[3];
    fgets(ts, 3, tp);
    pclose(tp);
    int t;
    sscanf(ts, "%d", &t);
    return t;
}

int main(int argc, char **argv)
{
    float delay = 1.f;
    if(argc > 1) delay = atof(argv[1]);
    int t = get_thread_count();
    lastTotal = malloc(sizeof(long long int)*(t+1));
    lastIdle  = malloc(sizeof(long long int)*(t+1));
    for(int i = 0; i <= t; i++)
        lastTotal[i] = lastIdle[i] = 1;
    char *line = malloc(100);
    size_t size;
    for(;;)
    {
        FILE *stat = fopen("/proc/stat", "r");
        if(stat == NULL)
            printf("Could not open /proc/stat");
        FILE *out = fopen("/tmp/cpu", "w+");
        for(int i = 0; i <= t; i++)
        {
            getline(&line, &size, stat);
            char line_b[100];
            strcpy(line_b, line);
            strtok(line_b, " ");
            total = 0;
            char *token;
            for(int j = 0; j < 7; j++)
            {
                token = strtok(NULL, " ");
                total += atoi(token);
                if(j == 3) idle =  atoi(token);
		if(j == 4) idle += atoi(token); // count 'iowait' as idle
            }
            float o = (1.0f-(double)(idle-lastIdle[i])/(total-lastTotal[i]));
            if(i == 0)
            {
                char p[3] = "\0\0";
                if(o < 1)
                    p[0] = ' ';
                if(o < 0.1f)
                    p[1] = ' ';
                fprintf(out, "%s%d%% ", p, (int)(o*100));
            }
            else
            {
                int m = (o*bar_height);
                int r = 40, g = 0, b = 255;
                if(m)
                {
                    r += (float)215/bar_height*m;
                    b -= (float)215/bar_height*m;
                    g = (r+b)/5;
                }

                char hex[7];
                char *hexer = "0123456789abcdef";
                hex[0] = hexer[r/16];
                hex[1] = hexer[r%16];
                hex[2] = hexer[g/16];
                hex[3] = hexer[g%16];
                hex[4] = hexer[b/16];
                hex[5] = hexer[b%16];
                hex[6] = 0;

                fprintf(out, "^c#%s^^r0,%d,12,%d^", hex, bar_height-m, bar_height);
                if(i != 4)fprintf(out, "^f%d^", bar_height);
                else fprintf(out, "^f12^");
            }

            lastTotal[i] = total;
            lastIdle[i] = idle;
        }
        fprintf(out, "^d^");
        fclose(stat);
        fclose(out);
        usleep(delay * 1000000);
    }
}
