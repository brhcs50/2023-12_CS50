#include "helpers.h"
#include "math.h"
#include "stdio.h"

//-- i think these are all kind of like mario, with an outer loop to go over the rows, and an inner loop to go over the pixels
//one-at-a-time
// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    //-- average the rgb values of each pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //-- I have to make each color a float or I get the wrong int value with "round"
            //-- I figured this out with the Sepia part later in the code
            //-- [i][j] represents a pixel location
            float Red = image[i][j].rgbtRed;
            float Green = image[i][j].rgbtGreen;
            float Blue = image[i][j].rgbtBlue;

            //-- calculate average value of RGB (from instructions)
            //-- "round" from math.h library
            int colavg = round((Red + Green + Blue) / 3);

            //-- assign the average value to each pixel
            image[i][j].rgbtRed = colavg;
            image[i][j].rgbtGreen = colavg;
            image[i][j].rgbtBlue = colavg;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //-- I need to save the original RGB values somepalce or they will get over-written doing the calculation later,
            //-- the names are from "Sepia" instructions
            float originalRed = image[i][j].rgbtRed;
            float originalGreen = image[i][j].rgbtGreen;
            float originalBlue = image[i][j].rgbtBlue;

            //-- temp values
            int sepiaRed = round(.393 * originalRed + .769 * originalGreen + .189 * originalBlue);
            int sepiaGreen = round(.349 * originalRed + .686 * originalGreen + .168 * originalBlue);
            int sepiaBlue = round(.272 * originalRed + .534 * originalGreen + .131 * originalBlue);

            //-- set values to 255 if they are too high
            if (sepiaRed > 255)
            {
                sepiaRed = 255;
            }
            if (sepiaGreen > 255)
            {
                sepiaGreen = 255;
            }
            if (sepiaBlue > 255)
            {
                sepiaBlue = 255;
            }

            //-- assign final temporary sepia colors to the pixel location
            image[i][j].rgbtRed = sepiaRed;
            image[i][j].rgbtGreen = sepiaGreen;
            image[i][j].rgbtBlue = sepiaBlue;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        //-- reflectign basically flips on the center axis, ie "width/2"
        {
            //-- need to store the top left value temporarily, and swap it with top right, iterating across the image
            RGBTRIPLE tempstore = image[i][j];

            image[i][j] = image[i][width - 1 - j];
            //-- top left pixel becomes the opposite side by
            //-- staying on the same line but subtracting 1 and the column number from the total width
            //-- ex: 1,1 becomes 1,[25-1-1]

            image[i][width - 1 - j] = tempstore;
            //-- saving this to iterate on the result each time
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    return;
}
