/**
 * Bob Somers
 * rsomers@calpoly.edu
 * CPE 473, Winter 2010
 * Cal Poly, San Luis Obispo
 */

#include "Image.hpp"

using namespace Eigen;

Image::Image(int width, int height)
{
    _width = width;
    _height = height;
    _max = 1.0;

    // allocate the first dimension, "width" number of color_t pointers...
    _pixmap = (Vector3f **)malloc(sizeof(Vector3f *) * _width);

    // allocate the second dimension, "height" number of color_t structs...
    for (int i = 0; i < _width; i++)
    {
        _pixmap[i] = (Vector3f *)malloc(sizeof(Vector3f) * _height);
    }
}

Image::~Image()
{
    // free each column of pixels first...
    for (int i = 0; i < _width; i++)
    {
        free(_pixmap[i]);
    }

    // free the rows of pixels second...
    free(_pixmap);
}

void Image::WriteTga(char *outfile, bool scale_color)
{
    FILE *fp = fopen(outfile, "w");
    if (fp == NULL)
    {
        perror("ERROR: Image::WriteTga() failed to open file for writing!\n");
        exit(EXIT_FAILURE);
    }
    
    // write 24-bit uncompressed targa header
    // thanks to Paul Bourke (http://local.wasp.uwa.edu.au/~pbourke/dataformats/tga/)
    putc(0, fp);
    putc(0, fp);
    
    putc(2, fp); // type is uncompressed RGB
    
    putc(0, fp);
    putc(0, fp);
    putc(0, fp);
    putc(0, fp);
    putc(0, fp);
    
    putc(0, fp); // x origin, low byte
    putc(0, fp); // x origin, high byte
    
    putc(0, fp); // y origin, low byte
    putc(0, fp); // y origin, high byte

    putc(_width & 0xff, fp); // width, low byte
    putc((_width & 0xff00) >> 8, fp); // width, high byte

    putc(_height & 0xff, fp); // height, low byte
    putc((_height & 0xff00) >> 8, fp); // height, high byte

    putc(24, fp); // 24-bit color depth

    putc(0, fp);

    // write the raw pixel data in groups of 3 bytes (BGR order)
    for (int y = 0; y < _height; y++)
    {
        for (int x = 0; x < _width; x++)
        {
            // if color scaling is on, scale 0.0 -> _max as a 0 -> 255 unsigned byte
            unsigned char rbyte, gbyte, bbyte;
            if (scale_color)
            {
                rbyte = (unsigned char)((_pixmap[x][y][0] / _max) * 255);
                gbyte = (unsigned char)((_pixmap[x][y][1] / _max) * 255);
                bbyte = (unsigned char)((_pixmap[x][y][2] / _max) * 255);
            }
            else
            {
                double r = (_pixmap[x][y][0] > 1.0) ? 1.0 : _pixmap[x][y][0];
                double g = (_pixmap[x][y][1] > 1.0) ? 1.0 : _pixmap[x][y][1];
                double b = (_pixmap[x][y][2] > 1.0) ? 1.0 : _pixmap[x][y][2];
                rbyte = (unsigned char)(r * 255);
                gbyte = (unsigned char)(g * 255);
                bbyte = (unsigned char)(b * 255);
            }
            putc(bbyte, fp);
            putc(gbyte, fp);
            putc(rbyte, fp);
        }
    }

    fclose(fp);
}

void Image::GenTestPattern()
{
    Vector3f pxl = Vector3f(0,0,0);
    int i, j, color;
    float radius, dist;
    
    // draw a rotating color checkerboard (RGB) in a 25x25 pixel grid
    for (int x = 0; x < _width; x++)
    {
        for (int y = 0; y < _height; y++)
        {
            i = x / 25;
            j = y / 25;
            color = (i + j) % 3;
            
            switch (color)
            {
                case 0: // red
                    pxl[0] = 1.0; pxl[1] = 0.0; pxl[2] = 0.0;
                    break;

                case 1: // green
                    pxl[0] = 0.0; pxl[1] = 1.0; pxl[2] = 0.0;
                    break;

                case 2: // blue
                    pxl[0] = 0.0; pxl[1] = 0.0; pxl[2] = 1.0;
                    break;
            }

            pixel(x, y, pxl);
        } 
    }

    // draw a black circle in the top left quadrant (centered at (i, j))
    pxl[0] = 0.0; pxl[1] = 0.0; pxl[2] = 0.0;
    i = _width / 4;
    j = 3 * _height / 4;
    radius = (((float)_width / 4.0) < ((float)_height / 4.0)) ? (float)_width / 4.0 : (float)_height / 4.0;
    for (int x = 0; x < _width; x++)
    {
        for (int y = 0; y < _height; y++)
        {
            dist = sqrtf((float)((x - i) * (x - i)) + (float)((y - j) * (y - j)));
            if (dist <= (float)radius)
            {
                pixel(x, y, pxl);
            }
        }
    }
    
    // draw a white circle in the lower right quadrant (centered at (i, j))
    pxl[0] = 1.0; pxl[1] = 1.0; pxl[2] = 1.0;
    i = 3 * _width / 4;
    j = _height / 4;
    radius = (((float)_width / 4.0) < ((float)_height / 4.0)) ? (float)_width / 4.0 : (float)_height / 4.0;
    for (int x = 0; x < _width; x++)
    {
        for (int y = 0; y < _height; y++)
        {
            dist = sqrtf((float)((x - i) * (x - i)) + (float)((y - j) * (y - j)));
            if (dist <= (float)radius)
            {
                pixel(x, y, pxl);
            }
        }
    }
}

Eigen::Vector3f Image::pixel(int x, int y)
{
    if (x < 0 || x > _width - 1 ||
        y < 0 || y > _height - 1)
    {
        // catostrophically fail
        fprintf(stderr, "ERROR: Image::pixel(%d, %d) outside range of the image!\n", x, y);
        exit(EXIT_FAILURE);
    }
    
    return _pixmap[x][y];
}

void Image::pixel(int x, int y, Eigen::Vector3f pxl)
{
    if (x < 0 || x > _width - 1 ||
        y < 0 || y > _height - 1)
    {
        // catostrophically fail
        fprintf(stderr, "ERROR: Image::pixel(%d, %d, pixel) outside range of the image!\n", x, y);
        exit(EXIT_FAILURE);
    }
    
    _pixmap[x][y] = pxl;

    // update the max color if necessary
    _max = (pxl[0] > _max) ? pxl[0] : _max;
    _max = (pxl[1] > _max) ? pxl[1] : _max;
    _max = (pxl[2] > _max) ? pxl[2] : _max;
}

