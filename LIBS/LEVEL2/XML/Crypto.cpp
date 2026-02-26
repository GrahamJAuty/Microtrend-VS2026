/**********************************************************************/

#include <wincrypt.h>
#pragma comment (lib, "crypt32.lib")

#define SHA1_LEN 20		// 20 bytes ( 160 bits )

/**********************************************************************/
/* Helper Routines											*/
/**********************************************************************/

static BYTE HexToNibble ( char c )		// c = '0'-'9', 'a'-'f', 'A'-'F'	returns 0x00 to 0x0f
{
	return ((( c & 0x40 ) != 0 ) ? c + 9 : c ) & 0x0f;
}

static BYTE HexToByte ( const char* szHex )		// "2A" -> 0x2a, "E" -> 0x0e
{
	BYTE b;

	if ( szHex[0] == 0 )									// length = 0
		b = 0;
	else if ( szHex[1] == 0 )								// length = 1
		b = HexToNibble ( szHex[0] );
	else													// length >= 2
		b = ( HexToNibble(szHex[0]) << 4 ) | HexToNibble(szHex[1]);

	return b;
}

static BYTE* HexToBinary ( const char* szHex, DWORD& dwDestLen )		// MUST delete returned buffer when finished (unless NULL)
{
	int nLength = strlen(szHex);

	BYTE* pDest = NULL;
	dwDestLen = nLength/2 + nLength%2;	// in case odd number of hex chars

	if ( dwDestLen > 0 )
	{
		pDest = new BYTE[dwDestLen];

		int i = 0;
		DWORD n = 0;

		while ( ( i < nLength ) && ( n < dwDestLen ) )
		{
			pDest[n++] = HexToByte ( szHex+i );
			i += 2;
		}
	}

	return pDest;		// MUST delete returned buffer when finished (unless NULL)
}

/**********************************************************************/

static const char HexDigits[] = "0123456789abcdef";

static CString ByteToHex ( BYTE b )
{
	CString strHex;

	strHex += HexDigits [ b >> 4 ];
	strHex += HexDigits [ b & 0x0f ];

	return strHex;
}

static CString BinaryToHex ( BYTE* pSource, DWORD dwLength )
{
	CString strHex;

	for ( DWORD i = 0 ; i < dwLength ; i++ )
		strHex += ByteToHex ( pSource[i] );

	return strHex;
}

/**********************************************************************/
/* Binary Routines											*/
/**********************************************************************/

CString BinaryToBase64 ( const BYTE* pSource, DWORD dwSourceLen )
{
	CString strDest;

	if ( dwSourceLen > 0 )
	{
		DWORD dwDestLen = 0;
		if ( CryptBinaryToString ( pSource, dwSourceLen, CRYPT_STRING_BASE64, NULL, &dwDestLen ) == TRUE )	// calculate dest length
		{
			DWORD dwBufSize = dwDestLen;
			char* pDest = new char[dwBufSize];
			
			if ( pDest != NULL )
			{
				if ( CryptBinaryToString ( pSource, dwSourceLen, CRYPT_STRING_BASE64, pDest, &dwDestLen ) == TRUE )
				{
					for ( DWORD i = 0 ; ( i < dwDestLen ) && ( i < dwBufSize ) ; i++ )
					{
						char c = pDest[i];
						if ( c != 0x0d && c != 0x0a )		// discard any number of crs or lfs
							strDest += pDest[i];
					}
				}
				delete [] pDest;
			}
		}
	}

	return strDest;
}

/**********************************************************************/

BYTE* Base64ToBinary ( const char* szSource, DWORD& dwDestLen )
{
	BYTE* pDest = NULL;

	DWORD dwSourceLen = strlen(szSource);
	if ( dwSourceLen > 0 )
	{
		if ( CryptStringToBinary ( (LPCSTR) szSource, dwSourceLen, CRYPT_STRING_BASE64, NULL, &dwDestLen, NULL, NULL ) == TRUE )	// calculate dest length
		{
			if ( ( pDest = new BYTE[dwDestLen] ) != NULL )
			{
				CryptStringToBinary ( (LPCSTR) szSource, dwSourceLen, CRYPT_STRING_BASE64, pDest, &dwDestLen, NULL, NULL );
			}
		}
	}

	return pDest;
}

/**********************************************************************/
/* Hex Routines											*/
/**********************************************************************/

CString HexToBase64 ( const char* szHex )		// eg: "1d29ab734b0c9585240069a6e4e3e91b61da1969" converts to "HSmrc0sMlYUkAGmm5OPpG2HaGWk="
{
	CString strDest;

	DWORD dwBytes;
	BYTE* pBuf = HexToBinary ( szHex, dwBytes );
	if ( pBuf != NULL )
	{
		strDest = BinaryToBase64 ( pBuf, dwBytes );
		delete [] pBuf;
	}

	return strDest;
}

/**********************************************************************/

CString Base64ToHex ( const char* szSource )
{
	CString strHex;

	DWORD dwDestLength;
	BYTE* pBuf = Base64ToBinary ( szSource, dwDestLength );
	if ( pBuf != NULL )
	{
		strHex = BinaryToHex ( pBuf, dwDestLength );
		delete [] pBuf;
	}

	return strHex;
}

/**********************************************************************/
/* Ascii Routines											*/
/**********************************************************************/

CString AsciiToBase64 ( const char* szSource )
{
	return BinaryToBase64 ( (BYTE*) szSource, strlen(szSource) );
}

/**********************************************************************/

CString Base64ToAscii ( const char* szSource )
{
	CString strAscii;

	DWORD dwDestLen;
	BYTE* pBuf = Base64ToBinary ( szSource, dwDestLen );
	if ( pBuf != NULL )
	{
		for ( DWORD i = 0 ; i < dwDestLen ; i++ )
			strAscii += pBuf[i];

		delete [] pBuf;
	}

	return strAscii;
}

/**********************************************************************/
/* SHA1 Routines											*/
/**********************************************************************/

DWORD BinaryToSHA1Binary ( const BYTE* pSource, DWORD dwSourceLen, BYTE* pDest )
{
	DWORD dwStatus;
	HCRYPTPROV hProv = 0;

	if ( CryptAcquireContext ( &hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT ) == TRUE )	// Get handle to the crypto provider
	{
		HCRYPTHASH hHash = 0;

		if ( CryptCreateHash ( hProv, CALG_SHA1, 0, 0, &hHash ) == TRUE )
		{
			if ( CryptHashData ( hHash, pSource, dwSourceLen, 0 ) == TRUE )
			{
				DWORD dwHashLen = SHA1_LEN;

				if ( CryptGetHashParam ( hHash, HP_HASHVAL, pDest, &dwHashLen, 0 ) == TRUE )
					dwStatus = 0;				// success
				else
					dwStatus = GetLastError();	// CryptGetHashParam failed
			}
			else
				dwStatus = GetLastError();	// CryptHashData failed

			CryptDestroyHash ( hHash );
		}
		else
			dwStatus = GetLastError();	// CryptCreateHash failed

		CryptReleaseContext ( hProv, 0 );
	}
	else
		dwStatus = GetLastError();	// CryptAcquireContext failed

	return dwStatus; 
}

/**********************************************************************/

CString AsciiToSHA1Hex ( const char* szSource )
{
	CString strHash;

	BYTE ucHashBuf[SHA1_LEN];
	if ( BinaryToSHA1Binary ( (BYTE*) szSource, strlen(szSource), ucHashBuf ) == 0 )
	{
		strHash = BinaryToHex ( ucHashBuf, SHA1_LEN );
	}

	return strHash;
}

/**********************************************************************/
/*
BYTE buf[SHA1_LEN];
BinaryToSHA1Binary ( (BYTE*)"x3JJHMbDL1EzLkh9GBhXDw==258EAFA5-E914-47DA-95CA-C5AB0DC85B11", strlen("x3JJHMbDL1EzLkh9GBhXDw==258EAFA5-E914-47DA-95CA-C5AB0DC85B11"), buf );

char buf1[SHA1_LEN*2];
BinaryToBase64 ( buf, SHA1_LEN, buf1, sizeof(buf1) );

StringToSHA1Hex ( "mariusbancila" );
*/
/**********************************************************************/
