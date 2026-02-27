//******************************************************************************
#include "..\SmartPay4Shared\RepmanDefines.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLTable_ClosingBalance\SQLRepositoryClosingBalance.h"
//******************************************************************************
#include "SmartPayAuditFile.h"
//******************************************************************************

CSmartPayAuditFile::CSmartPayAuditFile ( const char* szFilename )
{
	m_strAuditFile = szFilename;
	m_bAuditOpen = FALSE;
	m_bArrayMode = FALSE;
	m_pAuditArray = NULL;
	m_nNextArrayLine = 0;
}

//******************************************************************************

CSmartPayAuditFile::CSmartPayAuditFile(CStringArray* pArray)
{
	m_strAuditFile = "";
	m_bAuditOpen = FALSE;
	m_bArrayMode = TRUE;
	m_pAuditArray = pArray;
	m_nNextArrayLine = 0;
}

//******************************************************************************

CSmartPayAuditFile::~CSmartPayAuditFile()
{
	if (TRUE == m_bAuditOpen)								// just in case
	{
		Close();											// should never happen
	}
}

//******************************************************************************

int CSmartPayAuditFile::ValidateFile()
{
	if (Open() == FALSE)
	{
		return nREPORT_NOSOURCE;
	}

	CSSDate date;
	bool bOk = GetFirstDate ( date );									// try & get first date in audit file
	Close();															// don't leave audit file open

	return ( bOk == TRUE ) ? nREPORT_NOERROR : nREPORT_INVALIDSOURCE;	// see if have first date in audit file
}

//******************************************************************************

bool CSmartPayAuditFile::Open()
{
	return ( m_bAuditOpen = m_fileIn1.Open ( m_strAuditFile, "rb" ) );
}

//******************************************************************************

void CSmartPayAuditFile::Close()
{
	if ( m_bAuditOpen == TRUE )
	{
		m_fileIn1.Close();								// close main audit file if opened
		m_bAuditOpen = FALSE;
	}
}

//******************************************************************************

bool CSmartPayAuditFile::ReadLine ( CSQLAuditRecord_base* pAtc )
{
	if (FALSE == m_bArrayMode)
	{
		return pAtc->ReadLine(&m_fileIn1);
	}
	else
	{
		return pAtc->ReadLineFromArray(m_pAuditArray,m_nNextArrayLine);
	}
}

//******************************************************************************
// get first date of opened audit file

bool CSmartPayAuditFile::GetFirstDate ( CSSDate& date )
{
	CSQLAuditRecord_base atc;
	if ( atc.ReadLine( &m_fileIn1 ) == TRUE )
	{
		date.SetDate ( atc.GetDateFiled() );
		return date.IsSet();					// check for valid date
	}
	return FALSE;
}

//******************************************************************************
// Jump into file before start of given date
// match on either actual 'Filed Date' or ' LastUsed date' 

void CSmartPayAuditFile::JumpIn ( const char* szDate, bool bUseFiledDate )
{
	CSSDate date1 ( szDate );

	int nStartFrom	= 0L;
	int nOffset		= 0L;
	int nLowest		= 0L;
	int nHighest	= m_fileIn1.GetLength();
 
	int nAdjustment = ( bUseFiledDate == TRUE ) ? 1000 : 64000;		// make bigger allowance as not sorted on LastUsedDate

	CSQLAuditRecord_base atc;
	while ( nLowest < ( nHighest - nAdjustment ) )		// adjustment to allow for jumping into middle of a line
	{
		nOffset = ( nLowest + nHighest ) /2;
		int nChar = 0;

		while ( m_fileIn1.Seek ( (long)nOffset, SEEK_SET ) == 0 )
		{
			nChar = m_fileIn1.GetChar();

			if (nChar == EOF || nChar == '\x00a')		// end of line
			{
				break;
			}

			++nOffset;
		}

		if (nChar == EOF)								// end if file
		{
			break;
		}

		if (ReadLine(&atc) == TRUE)					// read next audit line start
		{
			CString strDate = "";

			if (bUseFiledDate == FALSE)
			{
				strDate = atc.GetDateLastUsed();	// not set so use audit date
			}
			else
			{
				strDate = atc.GetDateFiled();		// filed date
			}

			CSSDate date2(strDate);

			if (date1 > date2)
			{
				nLowest = m_fileIn1.Tell();		// date require is past middle
			}
			else
			{
				nHighest = m_fileIn1.Tell();	// date before middle
			}
		}
	}

	m_fileIn1.Seek ( (long)nLowest, SEEK_SET );							// start at the beginning
}


int CSmartPayAuditFile::GetFileSize()
{ 
	if (FALSE == m_bArrayMode)
	{
		return m_fileIn1.GetLength();
	}
	else
	{
		return m_pAuditArray->GetSize();
	}
}

//******************************************************************************

int CSmartPayAuditFile::GetFilePos() 
{
	if (FALSE == m_bArrayMode)
	{
		return m_fileIn1.Tell();
	}
	else
	{
		return m_nNextArrayLine;
	}
}

//******************************************************************************

void CSmartPayAuditFile::SetFilePos(int nPos) 
{ 
	if (FALSE == m_bArrayMode)
	{
		m_fileIn1.Seek((long)nPos, SEEK_SET);
	}
	else
	{
		m_nNextArrayLine = nPos;
	}
}

//******************************************************************************

int CSmartPayAuditFile::GetPercentPosition() 
{ 
	if (FALSE == m_bArrayMode)
	{
		return m_fileIn1.GetPercentPosition();
	}
	else
	{
		int nSize = m_pAuditArray->GetSize();
		
		if ( nSize <= 0 )
		{
			return 100;
		}
		else
		{
			return ((m_nNextArrayLine * 100) / nSize);
		}
	}
}

//******************************************************************************
