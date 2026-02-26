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

void CMyMifare::OnGotUID(const char* szUID)
{
	CString strIn = szUID;

	if (strIn.GetLength() <= 8)												// MiFare=8 or less 
	{
		if (m_nDataFormat == nMIFARE_DATAFORMAT_PAXTON)
		{
			unsigned long ulUID = strtoul(strIn, NULL, 16);

			CString strCardNo = "";
			strCardNo.Format("%u", ulUID);

			if (strCardNo.GetLength() > 8)
			{
				m_strUID = strCardNo.Right(8);	// only use 8 trailing characters
			}
			else
			{
				m_strUID = strCardNo;
			}

			m_strUID.TrimLeft('0');
		}

		else if (m_nDataFormat == nMIFARE_DATAFORMAT_INNERRANGE)
		{
			if (strIn.GetLength() == 8)
			{
				strIn = strIn.Mid(2);								// loose first part 6B807755 => 807755 
			}

			unsigned long ulUID = strtoul(strIn, NULL, 16);
			m_strUID.Format("%u", ulUID);
		}
		else															// m_nDataFormat == nMIFARE_DATAFORMAT_STANDARD )
		{
			unsigned long ulUID = strtoul(strIn, NULL, 16);
			m_strUID.Format("%u", ulUID);
		}
	}
	else
	{
		m_strUID = szUID;										// HID length = 16
	}
}
	
//$$******************************************************************

void CMyMifare::OnDisplayRemoveCard()
{
	if (m_pWnd != NULL)
	{
		m_pWnd->SetWindowText(szREMOVE_CARD);
	}
}

//$$******************************************************************

void CMyMifare::OnDisplayPresentCard()
{
	if (m_bWaitForCard == TRUE)
	{
		if (m_pWnd != NULL)
		{
			m_pWnd->SetWindowText(m_strPresentPrompt);
		}
	}
}

//$$******************************************************************

bool CMyMifare::SelectReader ( const char* szReader, const char* szPort, const char* szBaud )
{
	return CMifareReader::SelectReader ( szReader, szPort, szBaud );
}

//$$******************************************************************

int CMyMifare::GetUID( bool bWaitForCard, bool bLogRead )
{
	return CMifareReader::GetUID( bWaitForCard, bLogRead );
}

//$$******************************************************************

