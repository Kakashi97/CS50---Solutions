#include <cs50.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int count_letters(string);
int count_words(string);
int count_sentences(string);
int main(void)
{
    string text = get_string("Text: ");
    printf("%s\n", text);
    float L = count_letters(text) * 100.00 / count_words(text);
    float S = count_sentences(text) * 100.00 / count_words(text);
    int index = round(0.0588 * L - 0.296 * S - 15.8);
    if (index <= 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }
}

int count_letters(string text)
{
    int length = strlen(text), count = 0;
    for (int i = 0; i < length; i++)
    {
        if ((text[i] <= 90 && text[i] >= 65) || (text[i] <= 122 && text[i] >= 97))
        {
            count++;
        }
    }
    return count;
}
int count_words(string text)
{
    int length = strlen(text), count = 0;
    for (int i = 0; i < length; i++)
    {
        if (text[i] == ' ')
        {
            count++;
        }
    }
    return count + 1;
}
int count_sentences(string text)
{
    int length = strlen(text), count = 0;
    for (int i = 0; i < length; i++)
    {
        if (text[i] == '.' || text[i] == '?' || text[i] == '!')
        {
            count++;
        }
    }
    return count;
}
