/**********************************************************************/
#pragma once
/**********************************************************************/

CString BinaryToBase64 ( const BYTE* pSource, DWORD dwSourceLen );
BYTE* Base64ToBinary ( const char* szSource, DWORD& dwDestLen );		// MUST delete returned buffer when finished (unless NULL)

CString HexToBase64 ( const char* szHex );
CString Base64ToHex ( const char* szSource );

CString AsciiToBase64 ( const char* szSource );
CString Base64ToAscii ( const char* szSource );

DWORD BinaryToSHA1Binary ( const BYTE* pSource, DWORD dwSourceLen, BYTE* pDest );
CString AsciiToSHA1Hex ( const char* szSource );

/**********************************************************************/
