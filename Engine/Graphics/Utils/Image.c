/*
 *
 *
 *  Created on: 2013-08-30
 *      Author: scott.cgi
 */

#include <stdbool.h>
#include <setjmp.h>

#include "Mojoc/Graphics/Utils/Image.h"
#include "Mojoc/Toolkit/Platform/File.h"
#include "Mojoc/Toolkit/Platform/Log.h"
#include "Mojoc/External/PNG/Include/Android/png.h"


/**
 * Callback for libpng read data
 */
static void ReadPngData(png_structp pngPtr, png_bytep data, png_size_t length)
{
	AFile->Read(png_get_io_ptr(pngPtr), data, length);
}

static void* CreatePixelDataFromPng(const char* filePath, float* outWidth, float* outHeight)
{
	void* pixelData = NULL;
	File* filePtr   = NULL;

	do
	{
		filePtr = AFile->Open(filePath);

		unsigned char head[8];
		AFile->Read(filePtr, head, 8);
		if (png_sig_cmp(head, 0, 8))
		{
			ALogE("file %s, is not PNG", filePath);
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
			ALogE("Unable to create PNG structure: %s", filePath);
			break;
		}

		// allocate/initialize the memory for image information.  REQUIRED
		png_infop infoPtr = png_create_info_struct(pngPtr);

		if (infoPtr == NULL)
		{
			png_destroy_read_struct(&pngPtr, NULL, NULL);
			ALogE("unable to create PNG info : %s", filePath);
			break;
		}

	    png_infop endInfo = png_create_info_struct(pngPtr);
	    if (endInfo == NULL)
	    {
	        png_destroy_read_struct(&pngPtr, &infoPtr, NULL);
	        ALogE("unable to create PNG end info : %s", filePath);
	        break;
	    }

	    // set error handling if you are using the setjmp/longjmp method (this is
		// the normal method of doing things with libpng).  REQUIRED unless you
		// set up your own error handlers in the png_create_read_struct() earlier.
	    if (setjmp(png_jmpbuf(pngPtr)))
	    {
		  // free all of the memory associated with the png_ptr and info_ptr
		  png_destroy_read_struct(&pngPtr, &infoPtr, &endInfo);
		  ALogE("readPng error during setjmp : %s", filePath);
		  break;
	    }

		// if you are using replacement read functions, instead of calling
		// png_init_io() here you would call:
		// where user_io_ptr is a structure you want available to the callbacks
	    png_set_read_fn(pngPtr, filePtr, ReadPngData);

	    // if we have already read some of the signature
	    png_set_sig_bytes(pngPtr, 8);

	    // the call to png_read_info() gives us all of the information from the
		// PNG file before the first IDAT (image data chunk).  REQUIRED
		png_read_info(pngPtr, infoPtr);

		// note that png_get_IHDR() returns 32-bit data into
		// the application's width and height variables
		// This is an unsafe situation if these are 16-bit variables
		png_uint_32 pngWidth  = png_get_image_width (pngPtr, infoPtr);
		png_uint_32 pngHeight = png_get_image_height(pngPtr, infoPtr);

		int         bitDepth  = png_get_bit_depth (pngPtr, infoPtr);
		int         colorType = png_get_color_type(pngPtr, infoPtr);

		*outWidth             = (float) pngWidth;
		*outHeight            = (float) pngHeight;

		ALogD("PNG width = %f, height = %f", *outWidth, *outHeight);
		ALogD("PNG bitDepth = %d, colorType = %d", bitDepth, colorType);

		// force palette images to be expanded to 24-bit RGB
		// it may include alpha channel
		if (colorType == PNG_COLOR_TYPE_PALETTE)
		{
			png_set_palette_to_rgb(pngPtr);
			ALogD("png_set_palette_to_rgb");
		}

		// low-bit-depth grayscale images are to be expanded to 8 bits
		if (colorType == PNG_COLOR_TYPE_GRAY && bitDepth < 8)
		{
			png_set_expand_gray_1_2_4_to_8(pngPtr);
			ALogD("png_set_expand_gray_1_2_4_to_8");
		}

		// expand any tRNS chunk data into a full alpha channel
		if (png_get_valid(pngPtr, infoPtr, PNG_INFO_tRNS))
		{
			png_set_tRNS_to_alpha(pngPtr);
			ALogD("png_set_tRNS_to_alpha");
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
			ALogD("png_set_gray_to_rgb");
		}

		// update the png info struct.
	    png_read_update_info(pngPtr, infoPtr);

		// allocate the memory to hold the image using the fields of info_ptr
		unsigned int rowBytes = png_get_rowbytes(pngPtr, infoPtr);
		ALogD("row size: %d bytes", rowBytes);

		// allocate the pixel data as a big block, to be given to openGL
	    pixelData = png_malloc(pngPtr, rowBytes * pngHeight);
	    if (pixelData == NULL)
	    {
	    	png_destroy_read_struct(&pngPtr, &infoPtr, &endInfo);
	    	ALogE("unable to allocate PNG pixel data while loading %s", filePath);
	    	break;
	    }

	    // turn on interlace handling REQUIRED if you are not using
	    // png_read_image() To see how to handle interlacing passes
	    // see the png_read_row() method below:
	    int numberPasses = png_set_interlace_handling(pngPtr);
	    ALogD("interlacing passes = %d", numberPasses);
	    for (int pass = 0; pass < numberPasses; pass++)
	    {
	        for (int row = 0; row < pngHeight; row++)
	        {
	           png_read_row(pngPtr, ((unsigned char*)pixelData + (row * rowBytes)), NULL);
	        }
	    }

	    // read rest of file, and get additional chunks in info_ptr - REQUIRED
	    png_read_end(pngPtr, infoPtr);

	    // at this point you have read the entire image
	    // clean up after the read, and free any memory allocated - REQUIRE
	    png_destroy_read_struct(&pngPtr, &infoPtr, &endInfo);

	}
	while (false);

	AFile->Close(filePtr);

	return pixelData;
}

struct AImage AImage[1] =
{
    CreatePixelDataFromPng,
};

