#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>

double *get_double(char *prompt, double *doubleptr);
int msleep(long msec);

int main(void)
{   
    return 0;
}

double *get_double(char *prompt, double *doubleptr)
{
    double num;
    char buf[1024];
    int success;
    do
    {
        printf("%s", prompt);
        if (!fgets(buf, 1024, stdin))
        {
            return NULL;
        }
        char *endptr;
        errno = 0;
        num = strtod(buf, &endptr);
        if (errno == ERANGE || endptr == buf)
        {
            success = 0;
        }
        else
        {
            success = 1;
        }
    } while (!success);
    *doubleptr = num;
    return doubleptr;
}

int msleep(long milliseconds)
{
    struct timespec delay;
    int res;
    if (milliseconds < 0)
    {
        errno = EINVAL;
        return -1;
    }
    delay.tv_sec = milliseconds / 1000;
    delay.tv_nsec = (milliseconds % 1000) * 1000000;
    do
    {
        res = nanosleep(&delay, &delay);
    } while (res && errno == EINTR);
    return res;
}
