#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int convert(string input);

int main(void)
{
    string input = get_string("Enter a positive integer: ");

    for (int i = 0, n = strlen(input); i < n; i++)
    {
        if (!isdigit(input[i]))
        {
            printf("Invalid Input!\n");
            return 1;
        }
    }

    // Convert string to int
    printf("%i\n", convert(input));
}

int convert(string input)
{
    // TODO
    int n = strlen(input);
    if (n == 0)
    {
        return 0;
    }
    int N = input[n - 1] - '0';
    char shortened_input[n];
    strncpy(shortened_input, input, n - 1);
    shortened_input[n - 1] = '\0';
    return N + 10 * convert(shortened_input);
}
