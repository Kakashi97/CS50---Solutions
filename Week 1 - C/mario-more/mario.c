#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int x;
    do
    {
        x = get_int("Choose a number between 1 and 8\n");
    }
    while (x < 1 || x > 8);
    for (int i = 0; i < x; i++)
    {
        for (int j = 0; j < x - i - 1; j++)
        {
            printf(" ");
        }

        for (int k = 0; k < i + 1; k++)
        {
            printf("#");
        }

        printf("  ");

        for (int k = 0; k < i + 1; k++)
        {
            printf("#");
        }
    }
}
