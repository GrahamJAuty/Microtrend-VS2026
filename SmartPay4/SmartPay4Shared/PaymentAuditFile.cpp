//******************************************************************************
#include "PaymentAuditFile.h"
//******************************************************************************

CEposPaymentAuditFile::CEposPaymentAuditFile ( const char* szFilename )
	: m_strAuditFile(szFilename)
	, m_bAuditOpen(FALSE)
{
	if ( m_strAuditFile == "" )
		m_strAuditFile = Filenames.GetPaymentAuditFilename();
}

CEposPaymentAuditFile::~CEposPaymentAuditFile()
{
	if ( m_bAuditOpen == TRUE )								// just in case
		Close();											// should never happen
}

//******************************************************************************

bool CEposPaymentAuditFile::Open()
{
	return ( m_bAuditOpen = m_fileIn1.Open ( m_strAuditFile, "rb" ) );
}

void CEposPaymentAuditFile::Close()
{
	if ( m_bAuditOpen == TRUE )
	{
		m_fileIn1.Close();								// close main audit file if opened
		m_bAuditOpen = FALSE;
	}
}

//*******************************************************************

bool CEposPaymentAuditFile::ReadLine ( CPaymentAuditRecord* pAtc )
{
	return pAtc->ReadEntry ( &m_fileIn1 );
}

//*******************************************************************
// get first date of opened audit file

bool CEposPaymentAuditFile::GetFirstDate ( CSSDate& date )
{
	CPaymentAuditRecord atc;
	if ( atc.ReadEntry( &m_fileIn1 ) == TRUE )
	{
		date.SetDate ( atc.GetDateFiled() );
		return date.IsSet();					// check for valid date
	}
	return FALSE;
}

//******************************************************************************
// Jump into file before start of given date
// match on either actual 'Filed Date' or ' LastUsed date' 

void CEposPaymentAuditFile::JumpIn ( const char* szDate, bool bUseFiledDate )
{
	CSSDate date1 ( szDate );

	int nStartFrom	= 0L;
	int nOffset		= 0L;
	int nLowest		= 0L;
	int nHighest	= m_fileIn1.GetLength();
 
	int nAdjustment = ( bUseFiledDate == TRUE ) ? 1000 : 64000;		// make bigger allowance as not sorted on LastUsedDate

	CPaymentAuditRecord atc;
	while ( nLowest < ( nHighest - nAdjustment ) )		// adjustment to allow for jumping into middle of a line
	{
		nOffset = ( nLowest + nHighest ) /2;
		int nChar;

		while ( m_fileIn1.Seek ( (long)nOffset, SEEK_SET ) == 0 )
		{
			nChar = m_fileIn1.GetChar();

			if ( nChar == EOF || nChar == '\x00a' )		// end of line
				break;

			++nOffset;
		}

		if ( nChar == EOF )								// end if file
			break;

		if ( ReadLine( &atc ) == TRUE )					// read next audit line start
		{
			CString strDate;
			if ( bUseFiledDate == FALSE )			strDate = atc.GetDateLastUsed();	// not set so use audit date
			else									strDate = atc.GetDateFiled();		// filed date

			CSSDate date2 ( strDate );

			if ( date1 > date2 )	nLowest = m_fileIn1.Tell();		// date require is past middle
			else					nHighest = m_fileIn1.Tell();	// date before middle
		}
	}

	m_fileIn1.Seek ( (long)nLowest, SEEK_SET );							// start at the beginning
}

//******************************************************************************
