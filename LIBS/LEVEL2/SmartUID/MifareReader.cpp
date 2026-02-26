/**********************************************************************/

#include "local.h"

/**********************************************************************/

CMifareReader::CMifareReader()
{
	m_pReader = NULL;
	m_strReader = "";
	m_strError = "";
}

/**********************************************************************/

CMifareReader::~CMifareReader()
{
	if ( m_pReader != NULL )
	{
		DeselectReader();
		delete m_pReader;
		m_pReader = NULL;
	}
}

/**********************************************************************/

int CMifareReader::GetReaders ( CStringArray& arReaders )
{
	int nCount = CGemaltoProxDU::GetReaders ( arReaders );

	nCount += CGemaltoProxP2::GetReaders ( arReaders );

	nCount += COmnikey5x21Contactless::GetReaders ( arReaders );

	return nCount;
}

/**********************************************************************/

bool CMifareReader::SelectReader(const char* szReader, const char* szPort, const char* szBaud, bool bQuickScan)
{
	m_strReader = szReader;

	if (m_pReader != NULL)			// delete currently selected reader
	{
		DeselectReader();
		delete m_pReader;
		m_pReader = NULL;
	}

	if (CGemaltoProxDU::IsValid(szReader) == TRUE)		// Gemalto Prox-DU Contactless or Gemalto Prox-SU Contactless
	{
		m_pReader = new CGemaltoProxDU(this);
		m_pReader->SetQuickScanFlag(bQuickScan);
		return TRUE;
	}
	else if (CGemaltoProxP2::IsValid(szReader) == TRUE)	// Gemalto Prox-P2 Contactless
	{
		m_pReader = new CGemaltoProxP2(this);
		m_pReader->SetQuickScanFlag(bQuickScan);
		return m_pReader->OpenPort(szPort, szBaud);
	}
	else if (COmnikey5x21Contactless::IsValid(szReader) == TRUE)	// Omnikey 5x21 contactless
	{
		m_pReader = new COmnikey5x21Contactless(this);
		m_pReader->SetQuickScanFlag(bQuickScan);
		return TRUE;
	}

	m_strError = "Unknown reader";

	return FALSE;
}

/**********************************************************************/

void CMifareReader::DeselectReader()
{
	if ( m_pReader != NULL )
		m_pReader->ClosePort();
}

/**********************************************************************/

void CMifareReader::SetUIDFormat ( int n )
{
	if ( m_pReader != NULL )
		m_pReader->SetUIDFormat ( n );
}

/**********************************************************************/

int CMifareReader::GetUID ( bool bWaitForCard, bool bLogRead )
{
	if ( m_pReader != NULL )
		return m_pReader->GetUID ( m_strReader, bWaitForCard, bLogRead );
	else
	{
		m_strError = "Reader not selected";
		return 1;		// error
	}
}

/**********************************************************************/

void CMifareReader::Terminate()
{
	if ( m_pReader != NULL )
		m_pReader->Terminate();
}

/**********************************************************************/

const char* CMifareReader::GetError()
{
	if ( m_pReader != NULL )
		return m_pReader->GetError();
	else
		return m_strError;
}

/**********************************************************************/
