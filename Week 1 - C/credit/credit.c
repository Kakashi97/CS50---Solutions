#include <cs50.h>
#include <stdio.h>

int card_length(long);
long starting_digits(long);
int main(void)
{
    long N = get_long("Number: ");
    int s1 = 0, s2 = 0, S = 0, r = 0, a = 0;
    long n = N;
    while (n > 0)
    {
        r = n % 10;
        s1 += r;
        n = (n - r) / 10;
        r = n % 10;
        a = 2 * r;
        while (a > 0)
        {
            int b = 0;
            b = a % 10;
            s2 += b;
            a = (a - b) / 10;
            b = a % 10;
            s2 += b;
            a = a / 10;
        }
        S = s1 + s2;
        n = n / 10;
    }

    if (S % 10 == 0)
    {
        if (card_length(N) == 15 && (starting_digits(N) == 34 || starting_digits(N) == 37))
        {
            printf("AMEX\n");
        }
        else if (card_length(N) == 16 && (starting_digits(N) == 51 || starting_digits(N) == 52 || starting_digits(N) == 53 ||
                                          starting_digits(N) == 54 || starting_digits(N) == 55))
        {
            printf("MASTERCARD\n");
        }
        else if ((card_length(N) == 13 || card_length(N) == 16) && (starting_digits(N) / 10 == 4))
        {
            printf("VISA\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }
    return 0;
}

int card_length(long n)
{
    int i = 0;
    while (n > 0)
    {
        n = n / 10;
        i++;
    }
    return i;
}

long starting_digits(long n)
{
    int length = card_length(n);
    for (int i = 0; i < length - 2; i++)
    {
        n = n / 10;
    }
    return n;
}
