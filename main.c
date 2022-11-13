#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <math.h>

double *get_double(char *prompt, double *doubleptr);
int msleep(long msec);
double generate_crash_num();
void display_game(double mult, double win, double balance, int crash, int cashout);
double truncate(double num, int dp);

int main(void)
{
    srand(time(0));
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

double generate_crash_num()
{
    /* NOTE: seed needs to be set outside the function */
    if (rand() % 33 == 0)
    {
        return 1;
    }
    double zero_to_one = (double)rand() / (double)RAND_MAX;
    return 0.1 + .99 / (zero_to_one);
}

void display_game(double mult, double win, double balance, int crash, int cashout)
{
    if (crash)
    {
        printf("multiplier:  \033[0;31m%.2lfx\033[0m\n", mult);
    }
    else
    {
        printf("multiplier:  %.2lfx\n", mult);
    }
    if (!cashout && !crash)
    {
        printf("current win: \033[0;32m%.2lf$\033[0m\n", win);
    }
    else
    {
        printf("current win:\n");
    }
    printf("\nbalance: %.2lf$\n", balance);
}

double truncate(double num, int dp)
{
    return floor(num * pow(10, dp)) / pow(10, dp);
}
