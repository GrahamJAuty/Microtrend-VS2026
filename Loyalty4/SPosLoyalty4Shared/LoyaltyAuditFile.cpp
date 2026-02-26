/******************************************************************************/
#include "RepmanDefines.h"
/******************************************************************************/
#include "LoyaltyAuditFile.h"
/******************************************************************************/
// if auditfile = "" set to main audit filename when used

CLoyaltyAuditFile::CLoyaltyAuditFile ( const char* szFilename )
{
	m_strAuditFile = szFilename;
	m_bAuditOpen = FALSE;
}

/******************************************************************************/

CLoyaltyAuditFile::~CLoyaltyAuditFile()
{
	if (TRUE == m_bAuditOpen)
	{
		Close();
	}
}

/******************************************************************************/

bool CLoyaltyAuditFile::OpenReadOnly()
{
	if (m_strAuditFile == "")
	{
		m_strAuditFile = Filenames.GetAuditFilename();
	}

	m_bAuditOpen = m_fileIn1.Open ( m_strAuditFile, "rb" );

	return m_bAuditOpen;
}

/******************************************************************************/

void CLoyaltyAuditFile::Close()
{
	m_fileIn1.Close();								// close main audit file if opened
	m_bAuditOpen = FALSE;
}

//*******************************************************************
// Check audit file ok to report on

int CLoyaltyAuditFile::ValidateFile()
{
	if (OpenReadOnly() == FALSE)					// open audit file
	{
		return nREPORT_NOSOURCE;
	}

	CSSDate date;
	bool bOk = GetFirstDate ( date );				// try & get first date in audit file
	Close();										// close audit file

	return ( bOk == TRUE ) ? nREPORT_NOERROR : nREPORT_INVALIDSOURCE;
}

//*******************************************************************

bool CLoyaltyAuditFile::ReadLine ( CAuditRecord_base* pAtc )
{
	return pAtc->ReadLine ( &m_fileIn1 );
}

//*******************************************************************
// get first date of opened audit file

bool CLoyaltyAuditFile::GetFirstDate ( CSSDate& date )
{
	CAuditRecord_base atc;
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

void CLoyaltyAuditFile::JumpIn(const char* szDate, bool bUseFiledDate)
{
	CSSDate date1(szDate);

	long lStartFrom = 0L;
	long lOffset = 0L;
	long lLowest = 0L;
	long lHighest = m_fileIn1.GetLength();

	int nAdjustment = (bUseFiledDate == FALSE) ? 64000 : 1000;		// make bigger allowance as not sorted on FiledDate

	CAuditRecord_base atc;
	while (lLowest < (lHighest - nAdjustment))		// adjustment to allow for jumping into middle of a line
	{
		lOffset = (lLowest + lHighest) / 2;
		int nChar = 0;

		while (m_fileIn1.Seek(lOffset, SEEK_SET) == 0)
		{
			nChar = m_fileIn1.GetChar();

			if (nChar == EOF || nChar == '\x00a')		// end of line
			{
				break;
			}

			++lOffset;
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
				lLowest = m_fileIn1.Tell();		// date require is past middle
			}
			else
			{
				lHighest = m_fileIn1.Tell();	// date before middle
			}
		}
	}

	m_fileIn1.Seek(lLowest, SEEK_SET);							// start at the beginning
}

//******************************************************************************
