#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int avg = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);
            image[i][j].rgbtBlue = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtRed = avg;
        }
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][width - j - 1];
            image[i][width - j - 1] = temp;
        }
    }
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE tmp[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            double count = 0, sum_rgbtGreen = 0, sum_rgbtBlue = 0, sum_rgbtRed = 0;
            for (int r = -1; r < 2; r++)
            {
                for (int c = -1; c < 2; c++)
                {
                    if (i + r < 0 || i + r > height - 1)
                    {
                        continue;
                    }

                    if (j + c < 0 || j + c > width - 1)
                    {
                        continue;
                    }
                    sum_rgbtBlue += image[i + r][j + c].rgbtBlue;
                    sum_rgbtGreen += image[i + r][j + c].rgbtGreen;
                    sum_rgbtRed += image[i + r][j + c].rgbtRed;
                    count++;
                }
            }
            tmp[i][j].rgbtBlue = round(sum_rgbtBlue / count);
            tmp[i][j].rgbtGreen = round(sum_rgbtGreen / count);
            tmp[i][j].rgbtRed = round(sum_rgbtRed / count);
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = tmp[i][j];
        }
    }
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    RGBTRIPLE tmp[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            double wsumx_rgbtGreen = 0, wsumx_rgbtBlue = 0, wsumx_rgbtRed = 0, wsumy_rgbtGreen = 0, wsumy_rgbtBlue = 0,
                   wsumy_rgbtRed = 0;
            for (int r = -1; r < 2; r++)
            {
                for (int c = -1; c < 2; c++)
                {
                    if (i + r < 0 || i + r > height - 1)
                    {
                        continue;
                    }

                    if (j + c < 0 || j + c > width - 1)
                    {
                        continue;
                    }
                    wsumx_rgbtBlue += image[i + r][j + c].rgbtBlue * Gx[r + 1][c + 1];
                    wsumx_rgbtGreen += image[i + r][j + c].rgbtGreen * Gx[r + 1][c + 1];
                    wsumx_rgbtRed += image[i + r][j + c].rgbtRed * Gx[r + 1][c + 1];

                    wsumy_rgbtBlue += image[i + r][j + c].rgbtBlue * Gy[r + 1][c + 1];
                    wsumy_rgbtGreen += image[i + r][j + c].rgbtGreen * Gy[r + 1][c + 1];
                    wsumy_rgbtRed += image[i + r][j + c].rgbtRed * Gy[r + 1][c + 1];
                }
            }
            int sobelBlue = round(sqrt(pow(wsumx_rgbtBlue, 2) + pow(wsumy_rgbtBlue, 2))),
                sobelGreen = round(sqrt(pow(wsumx_rgbtGreen, 2) + pow(wsumy_rgbtGreen, 2))),
                sobelRed = round(sqrt(pow(wsumx_rgbtRed, 2) + pow(wsumy_rgbtRed, 2)));

            tmp[i][j].rgbtBlue = (sobelBlue > 255) ? 255 : sobelBlue;
            tmp[i][j].rgbtGreen = (sobelGreen > 255) ? 255 : sobelGreen;
            tmp[i][j].rgbtRed = (sobelRed > 255) ? 255 : sobelRed;
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = tmp[i][j];
        }
    }
}
