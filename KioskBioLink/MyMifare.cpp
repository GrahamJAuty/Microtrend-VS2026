//**********************************************************************
#include "MyMifare.h"
//**********************************************************************

CMyMifare::CMyMifare()
{
	m_strUID		= "";
	m_pWnd			= NULL;
	m_pStaticPrompt = NULL;

	m_bWaitForCard			= FALSE;
	m_strCardReferenceText	= "card";
	m_nDataFormat			= nMIFARE_DATAFORMAT_STANDARD;
}

//**********************************************************************

void CMyMifare::OnGotUID ( const char* szUID )
{
//***********************************
//	CString strIn = szUID;
//
//	if ( strIn.GetLength() <= 8 )								// MiFare=8 or less 
//	{
//		m_nMiFareType = 1;
//		unsigned long ulUID = strtoul ( strIn, NULL, 16 );
//		m_strUID.Format ( "%u", ulUID );
//	}
//	else if ( strIn.GetLength() == 12 )							// innerrange ( Eg "4E9C8A159000" => "8A9C4E8000" =  595327877120 )
//	{
//		m_nMiFareType = 2;
//		CString strID;
//		strID  = strIn.Mid ( 4,2 );
//		strID += strIn.Mid ( 2,2 );
//		strID += strIn.Mid ( 0,2 );
//
//		unsigned long uln = strtoul ( strIn.Mid ( 8,4 ) , NULL, 16 );
//		uln &= 0xc000;	
//
//		CString strBuf;		strBuf.Format ( "%4.4x", uln );
//		strID += strBuf;
//
//		unsigned __int64 n64 = _strtoui64 ( strID, NULL, 16 );
//		m_strUID.Format ( "%I64d", n64 );
//	}
//	else
//	{
//		m_nMiFareType = 3;										// HID length = 16
//		m_strUID = szUID;
//	}
//*******************************************************
	CString strIn = szUID;

	if ( strIn.GetLength() <= 8 )												// MiFare=8 or less 
	{
		if ( m_nDataFormat == nMIFARE_DATAFORMAT_PAXTON )
		{
			unsigned long ulUID = strtoul ( strIn, NULL, 16 );

			CString strCardNo;
			strCardNo.Format ( "%u", ulUID );

			if ( strCardNo.GetLength() > 8 )	m_strUID = strCardNo.Right(8);	// only use 8 trailing characters
			else								m_strUID = strCardNo;

			m_strUID.TrimLeft ( '0' );
		}

		else if ( m_nDataFormat == nMIFARE_DATAFORMAT_INNERRANGE )
		{
			if ( strIn.GetLength() == 8 )
				strIn = strIn.Mid ( 2 );								// loose first part 6B807755 => 807755 

			unsigned long ulUID = strtoul ( strIn, NULL, 16 );
			m_strUID.Format ( "%u", ulUID );
		}
		else															// m_nDataFormat == nMIFARE_DATAFORMAT_STANDARD )
		{
			unsigned long ulUID = strtoul ( strIn, NULL, 16 );
			m_strUID.Format ( "%u", ulUID );
		}
	}
	else
		m_strUID = szUID;										// HID length = 16
}

//**********************************************************************

void CMyMifare::OnDisplayRemoveCard()
{
	CString strPrompt;	strPrompt.Format ( "Remove %s", m_strCardReferenceText );

	if ( m_pStaticPrompt != NULL )
		m_pStaticPrompt->SetWindowText ( strPrompt );
	else
	{
		if ( m_pWnd != NULL )
			m_pWnd->SetWindowText ( strPrompt );
	}
}

void CMyMifare::OnDisplayPresentCard()
{
	if ( m_bWaitForCard == TRUE )
	{
		CString strPrompt;	strPrompt.Format ( "Present %s", m_strCardReferenceText );

		if ( m_pStaticPrompt != NULL )
			m_pStaticPrompt->SetWindowText ( strPrompt );
		else
		{
			if ( m_pWnd != NULL )
				m_pWnd->SetWindowText ( strPrompt );
		}
	}
}

//**********************************************************************
