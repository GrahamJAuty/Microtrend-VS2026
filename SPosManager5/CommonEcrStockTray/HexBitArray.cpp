/**********************************************************************/
 
/**********************************************************************/
#include "HexBitArray.h"
/**********************************************************************/

CHexBitArray::CHexBitArray( int nBits )
{
	m_nBits = nBits;
	if ( m_nBits < 1 ) m_nBits = 1;
}

/**********************************************************************/

bool CHexBitArray::GetBit( int nBit )
{
	bool bResult = FALSE;
	if ( ( nBit >= 0 ) && ( nBit < m_nBits ) )
	{
		int nChunkIdx = nBit / 4;
		if ( ( nChunkIdx >= 0 ) && ( nChunkIdx < m_arrayChunks.GetSize() ) )
		{
			int nChunk = m_arrayChunks.GetAt( nChunkIdx );

			switch( nBit % 4 )
			{
			case 0: bResult = ( ( nChunk & 8 ) != 0 );	break;
			case 1:	bResult = ( ( nChunk & 4 ) != 0 );	break;
			case 2: bResult = ( ( nChunk & 2 ) != 0 );	break;
			case 3: bResult = ( ( nChunk & 1 ) != 0 );	break;
			}
		}
	}
	return bResult;
}

/**********************************************************************/

void CHexBitArray::SetBit( int nBit, bool bFlag )
{
	if ( ( nBit >= 0 ) && ( nBit < m_nBits ) )
	{
		int nChunkIdx = nBit / 4;

		if ( TRUE == bFlag )
		{
			int nSize = m_arrayChunks.GetSize();
			while( nSize++ <= nChunkIdx )
				m_arrayChunks.Add(0);

			int nChunk = m_arrayChunks.GetAt( nChunkIdx );
			
			switch( nBit % 4 )
			{
			case 0: nChunk |= 8;	break;
			case 1:	nChunk |= 4;	break;
			case 2: nChunk |= 2;	break;
			case 3: nChunk |= 1;	break;
			}

			m_arrayChunks.SetAt( nChunkIdx, nChunk );
		}
		else
		{
			if ( nChunkIdx < m_arrayChunks.GetSize() )
			{
				int nChunk = m_arrayChunks.GetAt( nChunkIdx );

				switch( nBit % 4 )
				{
				case 0: nChunk &= 7;	break;
				case 1:	nChunk &= 11;	break;
				case 2: nChunk &= 13;	break;
				case 3: nChunk &= 14;	break;
				}

				m_arrayChunks.SetAt( nChunkIdx, nChunk );
			}
		}
	}
}

/**********************************************************************/

void CHexBitArray::TranslateFromHexString( CString& strHex )
{
	m_arrayChunks.RemoveAll();

	int nMaxChunks1 = strHex.GetLength();
	int nMaxChunks2 = ( ( m_nBits - 1 ) / 4 ) + 1;

	if ( nMaxChunks2 < nMaxChunks1 )
		nMaxChunks1 = nMaxChunks2;

	for ( int n = 0; n < nMaxChunks1; n++ )
	{
		char c = strHex.GetAt(n);
		if ( ( c >= '0' ) && ( c <= '9' ) )
			m_arrayChunks.Add( c - '0' );
		else if ( ( c >= 'A' ) && ( c <= 'F' ) )
			m_arrayChunks.Add( c - 'A' + 10 );
		else if ( ( c >= 'a' ) && ( c <= 'f' ) )
			m_arrayChunks.Add( c - 'a' + 10 );
		else
			break;
	}
}

/**********************************************************************/

void CHexBitArray::TranslateToHexString( CString& strHex )
{
	strHex = "";
	for ( int n = 0; n < m_arrayChunks.GetSize(); n++ )
	{
		int nChunk = m_arrayChunks.GetAt(n);
		if ( ( nChunk >= 0 ) && ( nChunk <= 9 ) )
		{
			char c = '0' + nChunk;
			strHex += c;
		}
		else if ( ( nChunk >= 10 ) && ( nChunk <= 15 ) )
		{
			char c = 'A' + nChunk - 10;
			strHex += c;
		}
		else
			break;
	}
}

/**********************************************************************/
