/**********************************************************************/
#include "md5.hpp"
/**********************************************************************/

CString CSSMD5::CalcMD5 ( const char* szData )
{
	CString strBuf = "*** SharpSoft ***";
	strBuf += szData;

	MD5_CTX mdContext;
	MD5Init ( &mdContext );
	MD5Update ( &mdContext, (BYTE*)((const char*) strBuf), strBuf.GetLength() );
	MD5Final ( &mdContext );

	CString strChecksum;
	for ( int i = 0 ; i < 16 ; i++ )
	{
		CString strTemp;
		strTemp.Format ( "%2.2x", mdContext.digest[i] );
		strChecksum += strTemp;
	}
	return strChecksum;
}

/**********************************************************************/

void CSSMD5::GetKey16 ( const char* szPassword, unsigned char* pKey )
{
	CString strBuf = "*** SharpSoft ***";
	strBuf += szPassword;

	MD5_CTX mdContext;
	MD5Init ( &mdContext );
	MD5Update ( &mdContext, (BYTE*)((const char*) strBuf), strBuf.GetLength() );
	MD5Final ( &mdContext );

	for ( int i = 0 ; i < 16 ; i++ )
		pKey[i] = mdContext.digest[i];
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CSSMD5Stream::CSSMD5Stream()
{
	m_bCalledBegin = FALSE;
	m_bCalledGetResult = FALSE;
	m_strResult = "";
}

/**********************************************************************/

void CSSMD5Stream::Begin()
{
	if ( (FALSE == m_bCalledBegin) && ( FALSE == m_bCalledGetResult ) )
	{
		CString strBuf = "*** SharpSoft ***";

		MD5Init(&m_mdContext);
		MD5Update(&m_mdContext, (BYTE*)((const char*)strBuf), strBuf.GetLength());

		m_bCalledBegin = TRUE;
	}
}

/**********************************************************************/

void CSSMD5Stream::Process(const char* szData)
{
	if ((TRUE == m_bCalledBegin) && (FALSE == m_bCalledGetResult))
	{
		CString strBuf = szData;
		MD5Update(&m_mdContext, (BYTE*)((const char*)strBuf), strBuf.GetLength());
	}
}

/**********************************************************************/

CString CSSMD5Stream::GetResult()
{
	if ((TRUE == m_bCalledBegin) && (FALSE == m_bCalledGetResult))
	{
		m_strResult = "";

		MD5Final(&m_mdContext);

		for (int i = 0; i < 16; i++)
		{
			CString strTemp;
			strTemp.Format("%2.2x", m_mdContext.digest[i]);
			m_strResult += strTemp;
		}

		m_bCalledGetResult = TRUE;
	}

	return m_strResult;
}

/**********************************************************************/
