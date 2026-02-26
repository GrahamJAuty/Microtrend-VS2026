/**********************************************************************/
#include "SPOSKeyboardColourTable.h"
/**********************************************************************/

CSPOSKeyboardColourTable::CSPOSKeyboardColourTable()
{
	Reset();
}

/**********************************************************************/

void CSPOSKeyboardColourTable::Reset()
{
	m_strFilename = "";
	SetDefaultRGB( 1, 0x0 );
	SetDefaultRGB( 2, 0xA80000 );
	SetDefaultRGB( 3, 0xFF00 );
	SetDefaultRGB( 4, 0xFF );
	SetDefaultRGB( 5, 0x54FFFF );
	SetDefaultRGB( 6, 0xA8A800 );
	SetDefaultRGB( 7, 0xFFFF54 );
	SetDefaultRGB( 8, 0xA8 );
	SetDefaultRGB( 9, 0xDDFFFF );
	SetDefaultRGB( 10, 0x711309 );
	SetDefaultRGB( 11, 0xFFD975 );
	SetDefaultRGB( 12, 0xF0B000 );
	SetDefaultRGB( 13, 0xFFFF );
	SetDefaultRGB( 14, 0x3A3A3A );
	SetDefaultRGB( 15, 0xEBEBEB );
	SetDefaultRGB( 16, 0xFFFFFF );
	SetDefaultRGB( 17, 0xDDDDDD );
	SetDefaultRGB( 18, 0x711309 );
	SetDefaultRGB( 19, 0x999999 );
	SetDefaultRGB( 20, 0x777777 );
	SetDefaultRGB( 21, 0x89675F );
	SetDefaultRGB( 22, 0xC9AD8B );
	SetDefaultRGB( 23, 0xCACA00 );
	SetDefaultRGB( 24, 0xCCCCCC );
	SetDefaultRGB( 25, 0xFFFF80 );
	SetDefaultRGB( 26, 0xDDDDDD );
	SetDefaultRGB( 27, 0xDDFFFF );
	SetDefaultRGB( 28, 0xBBBBBB );
	SetDefaultRGB( 29, 0x888888 );
	SetDefaultRGB( 30, 0x89675F );
	SetDefaultRGB( 31, 0xEEEEEE );
	SetDefaultRGB( 32, 0xCCCCCC );
	SetDefaultRGB( 33, 0xEFEFEF );
	SetDefaultRGB( 34, 0xDFDFDF );
	SetDefaultRGB( 35, 0xAFAFAF );
	SetDefaultRGB( 36, 0x8F8F8F );
	SetDefaultRGB( 37, 0x7F7F7F );
	SetDefaultRGB( 38, 0x5F5F5F );
	SetDefaultRGB( 39, 0x3F3F3F );
	SetDefaultRGB( 40, 0xFFC6C6 );
	SetDefaultRGB( 41, 0xFFA4A4 );
	SetDefaultRGB( 42, 0xFF2828 );
	SetDefaultRGB( 43, 0x750000 );
	SetDefaultRGB( 44, 0xC6FFC6 );
	SetDefaultRGB( 45, 0xA4FFA4 );
	SetDefaultRGB( 46, 0x28FF28 );
	SetDefaultRGB( 47, 0x7500 );
	SetDefaultRGB( 48, 0x96FEF7 );
	SetDefaultRGB( 49, 0x3EFDEF );
	SetDefaultRGB( 50, 0xC6C6FF );
	SetDefaultRGB( 51, 0xA4A4FF );
	SetDefaultRGB( 52, 0x2828FF );
	SetDefaultRGB( 53, 0x75 );
	SetDefaultRGB( 54, 0x158AFF );
	SetDefaultRGB( 55, 0x9DBDFF );
	SetDefaultRGB( 56, 0x938071 );
	SetDefaultRGB( 57, 0xB94476 );
	SetDefaultRGB( 58, 0xC11CA5 );
	SetDefaultRGB( 59, 0x227B3F );
	SetDefaultRGB( 60, 0xFF );
	SetDefaultRGB( 61, 0xFF00 );
	SetDefaultRGB( 62, 0xFF0000 );
	SetDefaultRGB( 63, 0xFF00FF );
	SetDefaultRGB( 64, 0xFFFF01 );

	RestoreDefaultRGB(1,64);
}

/**********************************************************************/

int CSPOSKeyboardColourTable::GetCurrentRGB( int nColourId )
{
	int nRGB = 0x0;
	if ( nColourId >= 1 && nColourId <= 64 )
		nRGB = m_nCurrentRGB[ nColourId - 1];

	return nRGB;
}

/**********************************************************************/

int CSPOSKeyboardColourTable::GetDefaultRGB( int nColourId )
{
	int nRGB = 0x0;
	if ( nColourId >= 1 && nColourId <= 64 )
		nRGB = m_nDefaultRGB[ nColourId - 1];

	return nRGB;
}

/**********************************************************************/

void CSPOSKeyboardColourTable::SetCurrentRGB( int nColourId, int nRGB )
{
	if ( nColourId >= 1 && nColourId <= 64 )
		m_nCurrentRGB[nColourId - 1] = nRGB;
}

/**********************************************************************/

void CSPOSKeyboardColourTable::SetDefaultRGB( int nColourId, int nRGB )
{
	if ( nColourId >= 1 && nColourId <= 64 )
		m_nDefaultRGB[nColourId - 1] = nRGB;
}

/**********************************************************************/

void CSPOSKeyboardColourTable::RestoreDefaultRGB( int nStartColourId, int nEndColourId)
{
	if ( nStartColourId < 1 || nStartColourId > 64 )
		return;

	if ( nEndColourId < 1 || nEndColourId > 64 )
		return;

	for ( int nColourId = nStartColourId; nColourId <= nEndColourId; nColourId++ )
		m_nCurrentRGB[nColourId - 1] = m_nDefaultRGB[nColourId - 1]; 
}

/**********************************************************************/

int CSPOSKeyboardColourTable::GetCursorColour( int nKeyColour )
{
	int nRed = nKeyColour & 0xFF;
	int nGreen = ( nKeyColour & 0xFF00 ) / 0x100;
	int nBlue = ( nKeyColour & 0xFF0000 ) / 0x10000;

	int nNewRed = ( nRed <= 0x80 ) ? 0xC0 : 0x40;
	int nNewBlue = ( nBlue <= 0x80 ) ? 0xC0 : 0x40;
	int nNewGreen = ( nGreen <= 0x80 ) ? 0xC0 : 0x40;
	
	return nNewRed + ( nNewGreen * 0x100 ) + ( nNewGreen * 0x10000 );
}

/**********************************************************************/

void CSPOSKeyboardColourTable::Read()
{
	if ( ReadInternal() == FALSE )
		RestoreDefaultRGB(1,64);
}

/**********************************************************************/

bool CSPOSKeyboardColourTable::ReadInternal()
{
	CSSFile fileColours;
	if ( fileColours.Open ( m_strFilename, "rb" ) == FALSE )
		return FALSE;

	int nColourId = 1;
	
	CString strBuffer;
	while( ( fileColours.ReadString( strBuffer ) == TRUE ) && ( nColourId <= 64 ) )
	{
		int nLength = strBuffer.GetLength();
		if ( nLength != 8 )
		{
			if ( ( nLength == 0 ) || ( strBuffer.Left(1) != "#" ) )
				return FALSE;
		}
		else
		{
			int nRGB = 0;

			for ( int n = 0; n < strBuffer.GetLength(); n++ )
			{
				nRGB *= 16;

				char c = strBuffer.GetAt( n );

				switch ( c )
				{
				case '0':					break;
				case '1':	nRGB += 1;		break;
				case '2':	nRGB += 2;		break;
				case '3':	nRGB += 3;		break;
				case '4':	nRGB += 4;		break;
				case '5':	nRGB += 5;		break;
				case '6':	nRGB += 6;		break;
				case '7':	nRGB += 7;		break;
				case '8':	nRGB += 8;		break;
				case '9':	nRGB += 9;		break;
				case 'A':	nRGB += 10;		break;
				case 'B':	nRGB += 11;		break;
				case 'C':	nRGB += 12;		break;
				case 'D':	nRGB += 13;		break;
				case 'E':	nRGB += 14;		break;
				case 'F':	nRGB += 15;		break;
				default:	
					return FALSE;	
				}
			}
			SetCurrentRGB( nColourId++, nRGB );
		}
	}

	return TRUE;
}

/**********************************************************************/

void CSPOSKeyboardColourTable::Write()
{
	CSSFile fileColours;
	if ( fileColours.Open ( m_strFilename, "wb" ) == FALSE )
		return;

	for ( int nColourId = 1; nColourId <= 64; nColourId++ )
	{
		if ( ( nColourId == 1 ) || ( ( nColourId % 10 ) == 0 ) )
		{
			CString strLine;
			strLine.Format ( "# %d", nColourId );
			fileColours.WriteLine( strLine );
		}

		CString strHex;
		strHex.Format ( "%8.8X", GetCurrentRGB( nColourId ) );
		fileColours.WriteLine( strHex );
	}
}

/**********************************************************************/

void CSPOSKeyboardColourTable::RememberUserColours()
{
	for ( int n = 0; n < 32; n++ )
		m_nBufferRGB[n] = m_nCurrentRGB[n+32];
}

/**********************************************************************/

void CSPOSKeyboardColourTable::RestoreUserColours()
{
	for ( int n = 0; n < 32; n++ )
		m_nCurrentRGB[n+32] = m_nBufferRGB[n];
}

/**********************************************************************/

bool CSPOSKeyboardColourTable::CompareUserColours()
{
	for ( int n = 0; n < 32; n++ )
		if ( m_nCurrentRGB[n+32] != m_nBufferRGB[n] )
			return TRUE;

	return FALSE;
}

/**********************************************************************/
