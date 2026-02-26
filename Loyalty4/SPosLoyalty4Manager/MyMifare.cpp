//$$******************************************************************
#include "SimulateReaderDlg.h"
//$$******************************************************************
#include "MyMifare.h"
//$$******************************************************************
static const char* szREMOVE_CARD	= "Remove card";
//$$******************************************************************

CMyMifare::CMyMifare()
{
	m_strUID = "";
	m_pWnd = NULL;

	m_bWaitForCard = FALSE;
	m_strPresentPrompt = "Present card";
	m_nDataFormat = nMIFARE_DATAFORMAT_STANDARD;
}

//$$******************************************************************

void CMyMifare::OnGotUID ( const char* szUID )
{
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
	
//$$******************************************************************

void CMyMifare::OnDisplayRemoveCard()
{
	if ( m_pWnd != NULL )
		m_pWnd->SetWindowText ( szREMOVE_CARD );
}

//$$******************************************************************

void CMyMifare::OnDisplayPresentCard()
{
	if ( m_bWaitForCard == TRUE )
	{
		if ( m_pWnd != NULL )
			m_pWnd->SetWindowText ( m_strPresentPrompt );
	}
}

//$$******************************************************************

bool CMyMifare::SelectReader ( const char* szReader, const char* szPort, const char* szBaud )
{
	bool bResult = FALSE;

	switch( CServerData::GetSimulateReaderType() )
	{
	case 1:
		bResult = TRUE;
		break;

	case 2:
		{
			CString strMsg;
			strMsg.Format( "Reader = %s\n\nPort = %s\n\nBaud = %s\n\nAllow select reader ?",
				szReader,
				szPort,
				szBaud );

			bResult = ( Prompter.YesNo( strMsg, "Simulated Card Reader", TRUE ) == IDYES );

			if ( FALSE == bResult )
				m_strError = "Simulated fail on select reader";
		}
		break;

	case 0:
	default:
		bResult = CMifareReader::SelectReader ( szReader, szPort, szBaud );
		break;
	}

	return bResult;
}

//$$******************************************************************

int CMyMifare::GetUID( bool bWaitForCard, bool bLogRead )
{
	int nResult = 0;

	switch( CServerData::GetSimulateReaderType() )
	{
	case 1:
	case 2:
		{
			CSimulateReaderDlg dlg(NULL);
			dlg.DoModal();
			
			nResult = dlg.m_nResult;
		
			switch( nResult )
			{
			case 0:
				m_strUID = dlg.m_strInputMiFareNo;
				break;

			case 1:
				m_strError = "Simulated card read error";
				break;
			}
		}
		break;

	case 0:
	default:
		nResult = CMifareReader::GetUID( bWaitForCard, bLogRead );
		break;
	}
	
	return nResult;
}

//$$******************************************************************
