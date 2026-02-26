/**********************************************************************/

static WORD GetBGR565 ( BYTE r, BYTE g, BYTE b )
{
	WORD bb = ( WORD(b) << 8 ) & 0xf800;
	WORD gg = ( WORD(g) << 3 ) & 0x07e0;
	WORD rr = ( WORD(r) >> 3 ) & 0x001f;
	WORD bgr565 = bb | gg | rr;

	return bgr565;
}

/**********************************************************************/

static bool SaveImageAsBGR565 ( CImage& image, const char* szFilename )
{
	int nWidth = image.GetWidth();
	int nHeight = image.GetHeight();

	WORD  wFileType = 0x4d42;			// File type, always 4D42h ("BM")
	DWORD dwFileSize = nWidth*nHeight*2;	// Size of the file in bytes (rgb565 = 2 bytes per pixel)
	WORD  wReserved1 = 0;				// Always 0
	WORD  wReserved2 = 0;				// Always 0
	DWORD dwBitmapOffset = 66;			// Starting position of image data in bytes

	DWORD dwSize = 40;					// Size of this header in bytes
	DWORD dwWidth = nWidth;				// Image width in pixels
	DWORD dwHeight = nHeight;			// Image height in pixels
	WORD  wPlanes = 1;					// Number of color planes
	WORD  wBitsPerPixel = 16;			// Number of bits per pixel
	DWORD dwCompression = 3;				// Compression methods used
	DWORD dwSizeOfBitmap = 0;			// Size of bitmap in bytes
	DWORD dwHorzResolution = 0;			// Horizontal resolution in pixels per meter
	DWORD dwVertResolution = 0;			// Vertical resolution in pixels per meter
	DWORD dwColorsUsed = 0;				// Number of colors in the image
	DWORD dwColorsImportant = 0;			// Minimum number of important colors

	DWORD dwRedMask = 0x0000f800;			// Mask identifying bits of red component
	DWORD dwGreenMask = 0x000007e0;		// Mask identifying bits of green component
	DWORD dwBlueMask = 0x0000001f;		// Mask identifying bits of blue component

	CSSFile file;
	if ( file.Open ( szFilename, "wb" ) == TRUE )
	{
		int nPitch = image.GetPitch();		// if pitch > 0 points to start of top row, if pitch < 0 points to start of bottom row

		int nBytesPerPixel = abs(nPitch)/nWidth;

		file.Write ( &wFileType, sizeof(wFileType), 1 );
		file.Write ( &dwFileSize, sizeof(dwFileSize), 1 );
		file.Write ( &wReserved1, sizeof(wReserved1), 1 );
		file.Write ( &wReserved2, sizeof(wReserved2), 1 );
		file.Write ( &dwBitmapOffset, sizeof(dwBitmapOffset), 1 );

		file.Write ( &dwSize, sizeof(dwSize), 1 );
		file.Write ( &dwWidth, sizeof(dwWidth), 1 );
		file.Write ( &dwHeight, sizeof(dwHeight), 1 );
		file.Write ( &wPlanes, sizeof(wPlanes), 1 );
		file.Write ( &wBitsPerPixel, sizeof(wBitsPerPixel), 1 );
		file.Write ( &dwCompression, sizeof(dwCompression), 1 );
		file.Write ( &dwSizeOfBitmap, sizeof(dwSizeOfBitmap), 1 );
		file.Write ( &dwHorzResolution, sizeof(dwHorzResolution), 1 );
		file.Write ( &dwVertResolution, sizeof(dwVertResolution), 1 );
		file.Write ( &dwColorsUsed, sizeof(dwColorsUsed), 1 );
		file.Write ( &dwColorsImportant, sizeof(dwColorsImportant), 1 );

		file.Write ( &dwRedMask, sizeof(dwRedMask), 1 );
		file.Write ( &dwGreenMask, sizeof(dwGreenMask), 1 );
		file.Write ( &dwBlueMask, sizeof(dwBlueMask), 1 );

		BYTE *ptr;

		if ( nPitch > 0 )
		{
			ptr = (BYTE*)image.GetBits();		// start of first line
		}
		else
		{
			ptr = (BYTE*)image.GetBits();		// start of last line
			ptr -= (abs(nPitch)*(nHeight-1));	// start of first line
		}

		for ( int y = 0 ; y < nHeight ; y++ )
		{
			for ( int x = 0 ; x < nWidth ; x++ )
			{
				int nOff = x * nBytesPerPixel;
				WORD bgr565 = GetBGR565 ( ptr[nOff], ptr[nOff+1], ptr[nOff+2] );
				file.Write ( &bgr565, sizeof(bgr565), 1 );
			}
			ptr += abs(nPitch);
		}

		file.Close();
	}

	return TRUE;
}

/**********************************************************************/

bool ResizeImageToBGR565 ( const char* szSourceFilename, const char* szDestFilename, int nMaxWidth, int nMaxHeight, bool bKeepAspectRatio )
{
	CImage imageSource;
	imageSource.Load ( szSourceFilename );		// load existing image

	int nWidth = imageSource.GetWidth();
	int nHeight = imageSource.GetHeight();

	if ( nWidth > nMaxWidth || nHeight > nMaxHeight )		// need to resize
	{
		int nNewWidth = nMaxWidth;
		int nNewHeight= nMaxHeight;

		if ( bKeepAspectRatio == TRUE )
		{
			nNewWidth = nMaxWidth;
			nNewHeight = ( nNewWidth * nHeight ) / nWidth;
			if (( nNewHeight % 2 ) != 0 )		// if odd, increment (this is what flakey does)
				nNewHeight++;
 
			if ( nNewHeight > nMaxHeight )
			{
				nNewHeight = nMaxHeight;
				nNewWidth = ( nNewHeight * nWidth ) / nHeight;
				if (( nNewWidth % 2 ) != 0 )	// if odd, increment (this is what flakey does)
					nNewWidth++;
			}
		}

		HDC hdcSource = imageSource.GetDC();
		HBITMAP bmpMem = CreateCompatibleBitmap ( hdcSource, nNewWidth, nNewHeight );
		HDC hdcMem = CreateCompatibleDC ( hdcSource );
		HBITMAP bmpOld = (HBITMAP) SelectObject ( hdcMem, bmpMem );

		SetStretchBltMode ( hdcMem, COLORONCOLOR );
		imageSource.StretchBlt ( hdcMem, CRect(0,0,nNewWidth,nNewHeight), SRCCOPY );
		imageSource.ReleaseDC();

	 	CImage imageDest;
		imageDest.Attach ( bmpMem );

		SaveImageAsBGR565 ( imageDest, szDestFilename );

		imageDest.Detach();

		SelectObject ( hdcMem, bmpOld );	// select original bmp
		DeleteObject ( bmpMem );
	}
	else		// no need to resize
	{
		SaveImageAsBGR565 ( imageSource, szDestFilename );
	}

	return TRUE;
}

/**********************************************************************/
