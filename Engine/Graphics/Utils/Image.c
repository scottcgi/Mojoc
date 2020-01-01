/*
 * Copyright (c) scott.cgi All Rights Reserved.
 *
 * This source code belongs to project Mojoc, which is a pure C Game Engine hosted on GitHub.
 * The Mojoc Game Engine is licensed under the MIT License, and will continue to be iterated with coding passion.
 *
 * License  : https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 * GitHub   : https://github.com/scottcgi/Mojoc
 * CodeStyle: https://github.com/scottcgi/Mojoc/wiki/Code-Style
 *
 * Since    : 2013-08-30
 * Update   : 2019-1-19
 * Author   : scott.cgi
 */


#include <stdbool.h>
#include <setjmp.h>

#include "Engine/Graphics/Utils/Image.h"
#include "Engine/Toolkit/Platform/File.h"
#include "Engine/Toolkit/Platform/Log.h"
#include "Engine/ThirdParty/PNG/Include/Android/png.h"


/**
 * Callback for libpng read data.
 */
static void ReadPNGData(png_structp pngPtr, png_bytep data, png_size_t length)
{
    AFile->Read(png_get_io_ptr(pngPtr), data, length);
}


static void* CreatePixelDataFromPNG(const char* resourceFilePath, float* outWidth, float* outHeight)
{
    void* pixelData = NULL;
    File* file       = NULL;

    do
    {
        file = AFile->Open(resourceFilePath);

        unsigned char head[8];
        AFile->Read(file, head, 8);
        if (png_sig_cmp(head, 0, 8))
        {
            ALog_E("AImage CreatePixelDataFromPNG file is not PNG, %s", resourceFilePath);
            break;
        }

        // create and initialize the png_struct with the desired error handler
        // functions.  If you want to use the default stderr and longjump method,
        // you can supply NULL for the last three parameters.  We also supply the
        // the compiler header file version, so that we know if the application
        // was compiled with a compatible version of the library.  REQUIRED
        png_structp pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (!pngPtr)
        {
            ALog_E("AImage CreatePixelDataFromPNG png_create_read_struct error, %s", resourceFilePath);
            break;
        }

        // allocate / initialize the memory for image information - REQUIRED
        png_infop infoPtr = png_create_info_struct(pngPtr);
        if (infoPtr == NULL)
        {
            png_destroy_read_struct(&pngPtr, NULL, NULL);
            ALog_E("AImage CreatePixelDataFromPNG png_create_info_struct infoPtr error, %s", resourceFilePath);
            break;
        }

        png_infop endInfo = png_create_info_struct(pngPtr);
        if (endInfo == NULL)
        {
            png_destroy_read_struct(&pngPtr, &infoPtr, NULL);
            ALog_E("AImage CreatePixelDataFromPNG png_create_info_struct endInfo error, %s", resourceFilePath);
            break;
        }

        // set error handling if you are using the setjmp/longjmp method (this is
        // the normal method of doing things with libpng).  REQUIRED unless you
        // set up your own error handlers in the png_create_read_struct() earlier.
        if (setjmp(png_jmpbuf(pngPtr)))
        {
          // free all of the memory associated with the png_ptr and info_ptr
          png_destroy_read_struct(&pngPtr, &infoPtr, &endInfo);
          ALog_E("AImage CreatePixelDataFromPNG during setjmp error, %s", resourceFilePath);
          break;
        }

        // if you are using replacement read functions, instead of calling
        // png_init_io() here you would call:
        // where user_io_ptr is a structure you want available to the callbacks
        png_set_read_fn(pngPtr, file, ReadPNGData);

        // if we have already read some of the signature
        png_set_sig_bytes(pngPtr, 8);

        // the call to png_read_info() gives us all of the information from the
        // PNG file before the first IDAT (image data chunk).  REQUIRED
        png_read_info(pngPtr, infoPtr);

        // note that png_get_IHDR() returns 32-bit data into
        // the application's outWidth and outHeight variables
        // This is an unsafe situation if these are 16-bit variables
        png_uint_32 pngWidth  = png_get_image_width (pngPtr, infoPtr);
        png_uint_32 pngHeight = png_get_image_height(pngPtr, infoPtr);

        int         bitDepth  = png_get_bit_depth (pngPtr, infoPtr);
        int         colorType = png_get_color_type(pngPtr, infoPtr);

        *outWidth             = (float) pngWidth;
        *outHeight            = (float) pngHeight;

        // force palette images to be expanded to 24-bit RGB
        // it may include alpha channel
        if (colorType == PNG_COLOR_TYPE_PALETTE)
        {
            png_set_palette_to_rgb(pngPtr);
        }

        // low-bit-depth grayscale images are to be expanded to 8 bits
        if (colorType == PNG_COLOR_TYPE_GRAY && bitDepth < 8)
        {
            png_set_expand_gray_1_2_4_to_8(pngPtr);
        }

        // expand any tRNS chunk data into a full alpha channel
        if (png_get_valid(pngPtr, infoPtr, PNG_INFO_tRNS))
        {
            png_set_tRNS_to_alpha(pngPtr);
        }

        // reduce images with 16-bit samples to 8 bits
        if (bitDepth == 16)
        {
            png_set_strip_16(pngPtr);
        }

        // expand grayscale images to RGB
        if (colorType == PNG_COLOR_TYPE_GRAY || colorType == PNG_COLOR_TYPE_GRAY_ALPHA)
        {
            png_set_gray_to_rgb(pngPtr);
        }

        // update the png info struct.
        png_read_update_info(pngPtr, infoPtr);

        // allocate the memory to hold the image using the fields of info_ptr
        unsigned long rowBytes = png_get_rowbytes(pngPtr, infoPtr);

        // allocate the pixel data as a big block, to be given to openGL
        pixelData = png_malloc(pngPtr, rowBytes * pngHeight);
        if (pixelData == NULL)
        {
            png_destroy_read_struct(&pngPtr, &infoPtr, &endInfo);
            ALog_E("AImage CreatePixelDataFromPNG malloc PNG pixel data error, %s", resourceFilePath);
            break;
        }

        // turn on interlace handling REQUIRED if you are not using
        // png_read_image() To see how to handle interlacing passes
        // see the png_read_row() method below:
        int numberPasses = png_set_interlace_handling(pngPtr);
        for (int pass = 0; pass < numberPasses; ++pass)
        {
            for (int row = 0; row < pngHeight; ++row)
            {
               png_read_row(pngPtr, ((unsigned char*) pixelData + (row * rowBytes)), NULL);
            }
        }

        // read rest of file, and get additional chunks in info_ptr - REQUIRED
        png_read_end(pngPtr, infoPtr);

        // at this point you have read the entire image
        // clean up after the read, and free any memory allocated - REQUIRE
        png_destroy_read_struct(&pngPtr, &infoPtr, &endInfo);

    }
    while (false);

    AFile->Close(file);

    return pixelData;
}


struct AImage AImage[1] =
{{
    CreatePixelDataFromPNG,
}};

