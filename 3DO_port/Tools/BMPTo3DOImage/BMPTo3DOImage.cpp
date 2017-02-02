// RasterSoft 3DO tools by Charles Doty/RasterSoft
// http://www.rastersoft.net
// Released free of any licenses
// Credit would be appreciated, but not required.

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define	WORDSwap(type) (((type >> 8) & 0x00FF) | ((type << 8)  & 0xFF00))
#define	DWORDSwap(type) (((type >> 24) & 0x000000FF) | ((type >> 8) & 0x0000FF00) | ((type << 8)  & 0x00FF0000) | ((type << 24) & 0xFF000000))

DWORD	Width;
DWORD	Height;
BYTE	*BmpBuffer;

void Usage();
bool Convert(char *Filename, char *ImageFilename);
bool LoadBMP(char *Filename);

int main(int argc, char *argv[])
{
	printf("BMPTo3DOImage v1.0b\n");
	printf("\tBy Charles Doty\n");

	if (argc < 3)
	{
		Usage();

		return	1;
	}

	if (false == Convert(argv[1], argv[2]))
	{
		return	1;
	}
	
	return	0;
}

void Usage()
{
	printf("Usage: BMPTo3DOImage Infile.bmp Outfile.imag\n");
}

bool Convert(char *Filename, char *ImageFilename)
{
	// Load bitmap into BmpBuffer. Memory is allocated in LoadBMP.
	if (false == LoadBMP(Filename))
	{
		return	false;
	}

	DWORD	*WriteBuffer	= new DWORD[Width * Height / 2];

	if (NULL == WriteBuffer)
	{
		printf("Unable to allocate memory.\n");
		
		return	false;
	}

	for (DWORD OuterLoop = 0; OuterLoop < Height; OuterLoop += 2)
	{
		for (DWORD InnerLoop = 0; InnerLoop < Width; InnerLoop++)
		{
			DWORD	Value;

			BYTE	Red		= (BmpBuffer[(OuterLoop * Width + InnerLoop) * 3] >> 3);
			BYTE	Green	= (BmpBuffer[(OuterLoop * Width + InnerLoop) * 3 + 1] >> 3);
			BYTE	Blue	= (BmpBuffer[(OuterLoop * Width + InnerLoop) * 3 + 2] >> 3);

			Value	= ((DWORD)((Red << 10) | (Green << 5) | Blue));
			Value <<= 16;
			
			Red		= (BmpBuffer[((OuterLoop + 1) * Width + InnerLoop) * 3] >> 3);
			Green	= (BmpBuffer[((OuterLoop + 1) * Width + InnerLoop) * 3 + 1] >> 3);
			Blue	= (BmpBuffer[((OuterLoop + 1) * Width + InnerLoop) * 3 + 2] >> 3);

			Value	|= ((DWORD)((Red << 10) | (Green << 5) | Blue));

			WriteBuffer[(OuterLoop / 2) * Width + InnerLoop]	= DWORDSwap(Value);
		}
	}

	// Write image file
	FILE	*Handle	= fopen(ImageFilename, "wb");
	
	if (NULL == Handle)
	{
		printf("Unable to create image file %s.\n", ImageFilename);
	
		return	false;
	}
	
	// Write image tag (IMAG)
	DWORD	Value	= 'GAMI';
	fwrite(&Value, sizeof(DWORD), 1, Handle);
	
	// Write image chunk size
	Value	= 0x1C;
	Value	= DWORDSwap(Value);
	fwrite(&Value, sizeof(DWORD), 1, Handle);
	
	// Write image width
	Value	= Width;
	Value	= DWORDSwap(Value);
	fwrite(&Value, sizeof(DWORD), 1, Handle);
	
	// Write image height
	Value	= Height;
	Value	= DWORDSwap(Value);
	fwrite(&Value, sizeof(DWORD), 1, Handle);
	
	// Write image stride
	Value	= Width * 2;
	Value	= DWORDSwap(Value);
	fwrite(&Value, sizeof(DWORD), 1, Handle);
	
	// From form3do.h:
	// Bits per pixel
	BYTE	Value8	= 16;
	fwrite(&Value8, sizeof(BYTE), 1, Handle);

	// Components (RGB = 3)
	Value8	= 3;
	fwrite(&Value8, sizeof(BYTE), 1, Handle);

	// Bit planes
	Value8	= 1;
	fwrite(&Value8, sizeof(BYTE), 1, Handle);

	// Color space (0 = RGB)
	Value8	= 0;
	fwrite(&Value8, sizeof(BYTE), 1, Handle);

	// Compression (0 = None)
	Value8	= 0;
	fwrite(&Value8, sizeof(BYTE), 1, Handle);

	// Corner weight flags (0 = None)
	Value8	= 0;
	fwrite(&Value8, sizeof(BYTE), 1, Handle);

	// Pixel order (0 is row by column, 1 is pixel pairs)
	Value8	= 1;
	fwrite(&Value8, sizeof(BYTE), 1, Handle);

	// Version
	Value8	= 0;
	fwrite(&Value8, sizeof(BYTE), 1, Handle);

	// Write image tag (PDAT)
	Value	= 'TADP';
	fwrite(&Value, sizeof(DWORD), 1, Handle);

	int	iExtraBit	= (Width * Height * 2) % 4;
	
	// Write chunk size.
	Value	= Width * Height * 2 + 8;
	
	if (iExtraBit != 0)
	{
		// Pad out to DWORD.
		Value	+= 2;
	}
	
	// Write buffer size
	Value	= DWORDSwap(Value);	
	fwrite(&Value, sizeof(DWORD), 1, Handle);
	
	// Write converted graphics
	fwrite(WriteBuffer, sizeof(WORD), Width * Height, Handle);
	
	if (iExtraBit != 0)
	{
		// Write an extra word to pad out to DWORD.
		fwrite(WriteBuffer, sizeof(WORD), 1, Handle);
	}
	
	fclose(Handle);	

	return	true;
}

bool LoadBMP(char *Filename)
{
	BYTE	*FileBuffer	= NULL;

	FILE	*Handle	= fopen(Filename, "rb");

	if (NULL == Handle)
	{
		printf("Unable to load BMP %s.\n", Filename);

		return	false;
	}
		
	fseek(Handle, 0, SEEK_END);

	DWORD	Length	= ftell(Handle);
	rewind(Handle);

	FileBuffer	= new BYTE[Length];

	if (NULL == FileBuffer)
	{
		printf("Unable to allocate memory.\n");

		return	false;
	}
	
	fread(FileBuffer, 1, Length, Handle);

	fclose(Handle);

	BYTE	*FilePointer	= FileBuffer;

	BITMAPFILEHEADER	*FileHeader	= (BITMAPFILEHEADER *)FilePointer;
	DWORD	BitmapSize				= FileHeader->bfSize;
	DWORD	Offset					= FileHeader->bfOffBits;

	FilePointer	+=	sizeof(BITMAPFILEHEADER);

	BITMAPINFO	*BmpInfo	= (BITMAPINFO *)FilePointer;

	if (BmpInfo->bmiHeader.biCompression != BI_RGB || BmpInfo->bmiHeader.biBitCount != 24)
	{
		printf("BMP is not 24 bit, or is compressed.\n");

		return	false;
	}

	long	BMPWidth	= BmpInfo->bmiHeader.biWidth;
	long	BMPHeight	= BmpInfo->bmiHeader.biHeight;
	long	Increment	= (BMPWidth * BmpInfo->bmiHeader.biBitCount / 8);

	FilePointer	= FileBuffer + Offset;
		
	if (BMPHeight > 0)
	{
		Increment	= -(BMPWidth * 2 * BmpInfo->bmiHeader.biBitCount / 8);
		FilePointer	+= (BMPWidth * BMPHeight * BmpInfo->bmiHeader.biBitCount / 8) - 
			(BMPWidth * BmpInfo->bmiHeader.biBitCount / 8);
	}

	else
	{
		BMPHeight		= -BMPHeight;
	}

	Width	= BMPWidth;
	Height	= BMPHeight;

	if ((Width % 4) != 0)
	{
		printf("Width must be a multiple of 4\n");

		return	false;
	}
	
	if ((Height % 2) != 0)
	{
		printf("Height must be a multiple of 2\n");

		return	false;
	}
	
	BmpBuffer	= new BYTE[Width * Height * 3];

	if (NULL == BmpBuffer)
	{
		printf("Unable to allocate memory.\n");

		return	false;
	}
	
	BYTE	*TmpBuffer	= BmpBuffer;

	for (long OuterLoop = 0; OuterLoop < abs(BMPHeight); OuterLoop++)
	{
		for (long InnerLoop = 0; InnerLoop < BMPWidth; InnerLoop++)			
		{		
			BYTE	Blue	= FilePointer[OuterLoop * BMPWidth * 3 + InnerLoop * 3 + 0];
			BYTE	Green	= FilePointer[OuterLoop * BMPWidth * 3 + InnerLoop * 3 + 1];
			BYTE	Red		= FilePointer[OuterLoop * BMPWidth * 3 + InnerLoop * 3 + 2];
			
			*TmpBuffer	= Red;
			TmpBuffer++;

			*TmpBuffer	= Green;
			TmpBuffer++;

			*TmpBuffer	= Blue;
			TmpBuffer++;
		}

		FilePointer	+= Increment;
	}
		
	if (FileBuffer != NULL)
	{
		delete [] FileBuffer;

		FileBuffer	= NULL;
	}

	return	true;
}
