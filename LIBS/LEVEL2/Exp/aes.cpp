/**********************************************************************/

#include "rijndael.h"

/**********************************************************************/

int CSSAES::Encrypt ( const unsigned char* pSource, int nLength, unsigned char* pDest, const char* szPassword )
{
	unsigned char Key[16] = { 0 };	//the Rijndael key
	CSSMD5::GetKey16 ( szPassword, (unsigned char*) &Key );

	Rijndael r;

	int nReply = r.init ( Rijndael::Mode::CBC, Rijndael::Direction::Encrypt, Key, Rijndael::KeyLength::Key16Bytes, 0 );	// 16, 24, 32

	if ( nReply == 0 )
		nReply = r.padEncrypt ( pSource, nLength, pDest );

	return nReply;
}

/**********************************************************************/

int CSSAES::Decrypt ( const unsigned char* pSource, int nLength, unsigned char* pDest, const char* szPassword )
{
	unsigned char Key[16] = { 0 };	//the Rijndael key
	CSSMD5::GetKey16 ( szPassword, (unsigned char*) &Key );

	Rijndael r;

	int nReply = r.init ( Rijndael::Mode::CBC, Rijndael::Direction::Decrypt, Key, Rijndael::KeyLength::Key16Bytes, 0 );	// 16, 24, 32

	if ( nReply == 0 )
		nReply = r.padDecrypt ( pSource, nLength, pDest );

	return nReply;
}

/**********************************************************************/
