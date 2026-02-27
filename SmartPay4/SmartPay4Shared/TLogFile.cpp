//******************************************************************************
#include "..\SmartPay4Shared\RecSort.h"
//******************************************************************************
#include "TLogRecord.h"
#include "TLogFile.h"
//******************************************************************************

extern CSysset Sysset;

//******************************************************************************

CTLogFile::CTLogFile ( const char* szUserID, const char* szFilename )
	: m_strUserID(szUserID)
	, m_strTLogFile(szFilename)
{
	Reset();

	if ( m_strTLogFile == "" )									// if no filename passed use default & import any waiting updates
	{
		m_strTLogFile = Filenames.GetPurchaseHistoryFilename ( szUserID );	// UserID maybe "" for cash account log

		CString strUpdateFilename = Filenames.GetPurchaseHistoryUpdateFilename ( szUserID );

		if ( ::FileExists ( strUpdateFilename ) == TRUE )		// try & open updates 
		{
			if ( Merge ( strUpdateFilename ) == TRUE )			// yes - merge in
				CFileRecycler::SendToRecycleBin ( strUpdateFilename );		// delete update file
		}
	}
}

//**********************************************************************

void CTLogFile::Reset()
{
	m_bEOF1 = TRUE;
	m_strTLogLine1 = "";
	m_strTLogLastLine1 = "";

	m_bEOF2 = TRUE;
	m_strTLogLine2 = "";
}

//********************************************************************

bool CTLogFile::Open()
{
	if ( m_fileIn1.Open ( m_strTLogFile, "rb" ) == TRUE )
	{
		m_bEOF1 = FALSE;							/* no main file */
		return TRUE;
	}
	return FALSE;
}

void CTLogFile::Close()
{
	m_fileIn1.Close();
}

//********************************************************************

bool CTLogFile::ReadLine ( CTLogRecord* record )
{
	record->Reset();								// clear record before read
	return record->ReadLine ( &m_fileIn1 );
}

/******************************************************************************/

bool CTLogFile::AppendFile ( const char* szFilename )
{
	bool bReply = FALSE;

	Reset();
	if ( m_fileOut.Open ( m_strTLogFile, "ab" ) == TRUE )
	{
		if ( m_fileIn2.Open ( szFilename, "rb" ) == TRUE )
		{
			m_bEOF2 = FALSE;
			while (ReadLine2() == TRUE)			// read line from new file
			{
				bReply = WriteLine2();
			}

			m_fileIn2.Close();
		}

		m_fileOut.Close();
	}

	return bReply;
}

/******************************************************************************/

int CTLogFile::GetCombinedPercentPosition ( int nTotalSize )
{
	if ( nTotalSize == 0 )
		return 0;

	double dValue = (double)( m_fileIn1.Tell() + m_fileIn2.Tell() ) * 100;
	return (int)( dValue / (double)nTotalSize );
}

//**********************************************************************

bool CTLogFile::Merge ( const char* szFilename )
{
	CSSDate dateFirst;
	if ( GetFirstDate ( dateFirst, szFilename ) == FALSE )	/* try & get first date in audit file */
	{
		CModuleGlobals::MessageBeepExclamation();
		CString strMess;
		strMess.Format ( "Invalid first line(s) in TLog file %s", szFilename );
		CModuleGlobals::PrompterError( strMess );
		return FALSE;
	}

	int nLineCount = ::GetFileLineCount ( szFilename );
	if ( nLineCount == 0 )
		return TRUE;

	if ( nLineCount > 1 )								// lines may be from more that 1 ecr therefore dates may not be in order
	{
		if ( Sort ( szFilename ) == FALSE )				// sort file into date order
			return FALSE;								// sort failed
	}

	if ( AppendCheck ( szFilename ) == TRUE )			// no need to merge - just append
		return AppendFile ( szFilename );

	Reset();
	if (m_fileIn2.Open(szFilename, "rb") == FALSE)
	{
		return FALSE;
	}

	m_bEOF2 = FALSE;

	CString strTmpName;
	strTmpName.Format ( "%s\\SysFiles\\tlogtemp.$$$", 
		(const char*) Sysset.GetProgramPath() );

	if ( m_fileOut.Open ( strTmpName, "wb" ) == FALSE )
	{
		m_fileIn2.Close();							/* close new file */
		return FALSE;
	}

	bool bReply;
	Open();											/* fpin1 = open main file */

	int nFilesize = m_fileIn1.GetLength() + m_fileIn2.GetLength();

	ReadLine1();									/* read line from main file */
	ReadLine2();									/* read line from new file */

	while ( m_bEOF1 == FALSE )
	{
		if ( m_bEOF2 == TRUE )						/* see if have new data */
		{
			bReply = WriteLine1();					/* no just save main file line */
			ReadLine1();							/* read next line from main file */
			continue;
		}

		if ( m_strTLogLine1 == m_strTLogLine2 )		/* 2 lines exactly the same */
		{
			bReply = WriteLine1();					/* just save main file line */
			ReadLine1();							/* read next line from main file */
			ReadLine2();							/* read next line from main file */
			continue;
		}

		CTLogRecord recIn1 ( m_strTLogLine1 );
		CSSDateTime dt1 ( recIn1.GetDate(), recIn1.GetTime() );

		if ( dt1.IsSet() == FALSE )
		{
			ReadLine1();								/* skip invalid line */
			continue;
		}		

		CTLogRecord recIn2 ( m_strTLogLine2 );
		CSSDateTime dt2 ( recIn2.GetDate(), recIn2.GetTime() );

		if ( dt2.IsSet() == FALSE )
		{
			ReadLine2();								/* skip invalid line */
			continue;
		}		

		if ( dt1 <= dt2 )
		{
			bReply = WriteLine1();
			ReadLine1();							// read next line from main file
		}
		else
		{
			bReply = WriteLine2();					// read next line from new file
			ReadLine2();
		}
	}

	while ( m_bEOF2 == FALSE )						/* read line and strip crlf */
	{
		bReply = WriteLine2();						/* just write line */
		ReadLine2();								/* get next new line */
	}

	m_fileIn1.Close();
	m_fileIn2.Close();
	m_fileOut.Close();

	if ( bReply == TRUE )
	{
		CFileRecycler::SendToRecycleBin ( m_strTLogFile );
		rename ( strTmpName, m_strTLogFile );		/* rename new merge file to audit file */
	}
	return bReply;
}

/******************************************************************************/
// Uses FileIn2 & FileOut

bool CTLogFile::Sort ( const char* szFilename )
{
	Reset();

	if ( m_fileIn2.Open ( szFilename, "rb" ) == FALSE )
		return FALSE;
	m_bEOF2 = FALSE;

	int nFilePos = 0;										// start of file
	CRecordSorter sorter;

	while (ReadLine2() == TRUE)							// read line from new file
	{
		CTLogRecord log(m_strTLogLine2);					// extracts just date & time 

		CString strLabel = "";
		strLabel.Format("%4.4d%2.2d%2.2d%2.2d%2.2d",
			log.GetYear(),
			log.GetMonth(),
			log.GetDay(),
			log.GetHour(),
			log.GetMinute());

		sorter.Add(strLabel, nFilePos);

		nFilePos = m_fileIn2.Tell();						// start for next line
	}

	CString strTmpSort = "";
	strTmpSort.Format ( "%s\\SysFiles\\tlogsort.$$$", 
		(const char*) Sysset.GetProgramPath() );

	CString strError = "";

	if ( sorter.GetSize() > 0 )
	{
		if ( m_fileOut.Open ( strTmpSort, "wb" ) == TRUE )
		{
			m_fileIn2.Seek ( 0L, SEEK_SET );					// back to start of file to sort
			m_bEOF2 = FALSE;
			m_strTLogLine2 = "";

			for ( int nIndex = 0 ; nIndex < sorter.GetSize() ; nIndex++ )
			{
				m_fileIn2.Seek ( (long)sorter.GetRecordNo(nIndex), SEEK_SET );
				if ( ReadLine2() == TRUE )
				{
					if ( WriteLine2() == FALSE )				// write line
					{
						strError = "Unable to write to TLog sort output file!";
						break;
					}
				}
			}
			m_fileOut.Close();
		}
		else
			strError = "Unable to create TLog sort output file!";
	}
	m_fileIn2.Close();

//tidy up

	bool bReply = FALSE;

	if ( strError == "" )											// check any errors
	{
		CFileRecycler::SendToRecycleBin ( szFilename );										// delete unsorted file
		if ( ::rename ( strTmpSort, szFilename ) == 0 )		bReply = TRUE;		// ren temp to new sorted file
		else												strError = "Unable to rename TLog sort output file!";
	}

	if ( strError != "" )
	{
		CModuleGlobals::PrompterError ( strError );
	}

	return bReply;
}

/******************************************************************************/

bool CTLogFile::ReadLine1()
{
	bool bReply = FALSE;

	if ( m_bEOF1 == FALSE )
	{
		if ( ( bReply = m_fileIn1.ReadString ( m_strTLogLine1 ) ) == FALSE )
			m_bEOF1 = TRUE;
	}
	return bReply;
}

bool CTLogFile::ReadLine2()
{
	bool bReply = FALSE;

	if ( m_bEOF2 == FALSE )
	{
		if ( ( bReply = m_fileIn2.ReadString ( m_strTLogLine2 ) ) == FALSE )
			m_bEOF2 = TRUE;
	}
	return bReply;
}

/******************************************************************************/

bool CTLogFile::WriteLine1()
{
	m_strTLogLastLine1 = m_strTLogLine1;				/* copy of last line written */
	return m_fileOut.WriteLine ( m_strTLogLine1 );
}

bool CTLogFile::WriteLine2()
{
	return m_fileOut.WriteLine ( m_strTLogLine2 );
}

/**********************************************************************/
// get first date in tlog file

bool CTLogFile::GetFirstDate ( CSSDate& date, const char* szFilename )
{
	CSSFile file;
	CTLogRecord tlogRecord;
	bool bReply = FALSE;

	if ( file.Open ( szFilename, "rb" ) == TRUE )
	{
		tlogRecord.ReadLine ( &file, FALSE );			// only read date/time prefix on line

		date.SetDate ( tlogRecord.GetDate() );
		bReply = date.IsSet();					// check for valid date
		file.Close();
	}
	return bReply;

}

/******************************************************************************/
// compare given date&time with last date & time in audit file to see if ok
// to append to end of atc file

bool CTLogFile::AppendCheck ( const char* szFilename )
{
	CSSFile file;
	CTLogRecord rec1;

	if ( file.Open ( szFilename, "rb" ) == TRUE )
	{
		rec1.ReadLine ( &file, FALSE );					// only read date/time prefix on line
		file.Close();
	}

	CSSDateTime dtFirst ( rec1.GetDate(), rec1.GetTime() );	// first date in new file

	if ( file.Open ( m_strTLogFile, "rb" ) == TRUE )	// main audit file
	{
		file.Seek ( 0L, SEEK_END );
		int nOffset = file.Tell();
		nOffset -= 3;									// lose last cr/lf in file

		CString strBuf;
		while ( file.Seek ( (long)nOffset, SEEK_SET ) == 0 )
		{
			int nChar;
			if ( ( nChar = file.GetChar() ) == EOF )	// error
				break;

			if ( nChar == '\x00a' )						// end of previous line?
				break;

			strBuf += (char)nChar;							// read line backwards
			--nOffset;
		}
		file.Close();

		if ( strBuf.GetLength() > 0 )				// see if have any data ddmmyyyy,hhmm etc
		{
			strBuf.MakeReverse();					// put line correct way round
			CTLogRecord rec2 ( strBuf );

			CSSDateTime dtLast ( rec2.GetDate(), rec2.GetTime() );// last date in audit file

			if ( dtFirst <= dtLast )				// date is before (may be identical line) last date in atc file
				return FALSE;						// don't append - need to merge (if identical will get removed by merge)
		}
	}

	return TRUE;									// date is after last date in atc file, so append
}

/******************************************************************************/
// Jump into file within 1K before start of given date

void CTLogFile::JumpIn ( const char* szDate )
{
	CSSDate date1 ( szDate );

	int nStartFrom = 0;
	int nOffset	= 0;
	int nLowest	= 0;
	int nHighest	= m_fileIn1.GetLength();
 
	while ( nLowest < ( nHighest - 1000 ) )				// 1000 off max to allow for jumping into middle of a line
	{													// jumps to nearest 1k
		nOffset = ( nLowest + nHighest ) / 2;
		int nChar;

		while ( m_fileIn1.Seek ( (long)nOffset, SEEK_SET ) == 0 )
		{
			if ( ( nChar = m_fileIn1.GetChar() ) == EOF )	// work backwards to find start of line
				break;										// as some lines can be very long

			if ( nChar == '\x00a' )						// end of previous line
				break;

			--nOffset;
		}

		if ( nChar == EOF )								// start of file
			break;

		if ( ReadLine1() == TRUE )						// read next audit line start
		{
			CTLogRecord rec ( (const char*)m_strTLogLine1 );			// extract date & time
			CSSDate date2 ( rec.GetDate() );	

			if ( date1 > date2 )
				nLowest = m_fileIn1.Tell();				// start date past the end of the line
			else
			{												// start date before start of this line
				int nLen = m_strTLogLine1.GetLength() + 2;	// adjust offset so end is before start of the line + crlf
				nHighest = ( m_fileIn1.Tell() - nLen );
			}
		}
	}

	m_fileIn1.Seek ( (long)nLowest, SEEK_SET );				// start at the beginning
}

//**********************************************************************
//  Extract purcahses between given dates & saved to filename

int CTLogFile::Extract ( const char* szFilename, const char* szDateFrom, const char* szTimeFrom, const char* szDateTo, const char* szTimeTo )
{
	int nCount = -1;
	CSSDate dateFrom ( szDateFrom );
	CSSTime timeFrom ( szTimeFrom );
	CSSDate dateTo ( szDateTo );
	CSSTime timeTo ( szTimeTo );

	if ( Open() == TRUE )
	{
		if ( m_fileOut.Open ( szFilename, "wb" ) == TRUE )
		{
			nCount = 0;
			JumpIn ( szDateFrom );

			CTLogRecord tlogRecord;
			while ( ReadLine ( &tlogRecord ) == TRUE )
			{
				if ( tlogRecord.m_date < dateFrom )
					continue;

				if ( tlogRecord.m_date == dateFrom && tlogRecord.m_time < timeFrom )
					continue;

				if ( dateTo.IsSet() == TRUE )
				{
					if ( tlogRecord.m_date > dateTo )
						break;

					if ( tlogRecord.m_date == dateTo && tlogRecord.m_time > timeTo )
						break;
				}

				if ( tlogRecord.WriteLine ( &m_fileOut ) == TRUE )
					++nCount;
			}
			m_fileOut.Close();

			if ( nCount == 0 )
				CFileRecycler::SendToRecycleBin ( szFilename );							// delete empty file
		}
		Close();
	}
	return nCount;
}

//*********************************************************