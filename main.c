#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <math.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <stdbool.h>

double *get_double(char *prompt, double *doubleptr);
int msleep(long msec);
double generate_crash_num();
void display_game(double mult, double win, double balance, int crash, int cashout);
double truncate(double num, int dp);
bool double_compare(double a, double b);
bool keyhit();

int main(void)
{
    double balance = 100.00;
    double multiplier = 0.00;
    double currentwin = 0.00;
    bool cashout = false;
    bool crash = false;
    double betamount;
    double crashnum;

    srand(time(0));
    while (truncate(balance, 2) > 0.00)
    {
        system("clear");
        display_game(multiplier, currentwin, balance, crash, cashout);
        multiplier = 0.00;
        currentwin = 0.00;
        cashout = false;
        crash = false;
        do
        {
            if (!get_double("bet amount: ", &betamount))
            {
                return 1;
            }
        } while (betamount < 0.01 || betamount > balance);
        balance -= truncate(betamount, 2);
        multiplier += 1;
        crashnum = truncate(generate_crash_num(), 2);
        while (!double_compare(multiplier, crashnum))
        {
            system("clear");
            display_game(multiplier, currentwin, balance, crash, cashout);
            multiplier += 0.01;
            currentwin = betamount * multiplier;
            if (!cashout)
            {
                printf("Press any [\033[0;32mKEY\033[0m] to cashout");
                fflush(stdout);
            }
            if (keyhit() && !cashout)
            {
                balance += currentwin;
                cashout = true;
            }
            msleep(90);
        }
        crash = true;
    }
    system("clear");
    display_game(multiplier, currentwin, balance, crash, cashout);
    printf("Game Over!\n");
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

bool double_compare(double a, double b)
{
    if (fabs(a - b) < 1e-9)
    {
        return true;
    }
    return false;
}

bool keyhit()
{
    struct termios term;
    tcgetattr(0, &term);

    /* Disable line buffering in stdin */
    struct termios term2 = term;
    term2.c_lflag &= ~ICANON;
    tcsetattr(0, TCSANOW, &term2);

    int byteswaiting;
    ioctl(0, FIONREAD, &byteswaiting);

    for (int i = 0; i < byteswaiting; i++)
    {
        getchar();
    }
    /*Revert changes to avoid leaving terminal in weird state*/
    tcsetattr(0, TCSANOW, &term);
    return byteswaiting > 0;
}
