//*******************************************************************
#include "CkDateTime.h"
#include "CkGlobal.h"
//*******************************************************************
#include <io.h>
#include <math.h>
#include <share.h>
//*******************************************************************
#include <memory>
//*******************************************************************
#include <chrono>
#include <iomanip>
#include <sstream>
//*******************************************************************
#include "GlobalFunctions.h"
//*******************************************************************
extern CSysset Sysset;
//*********************************************************************

//*******************************************************************
#ifndef SYSTEMTYPE_BACKGROUND
//*******************************************************************
#ifndef SYSTEMTYPE_SERVER
//*******************************************************************
#ifndef SYSTEMTYPE_EMAILBATCHSEND
//*******************************************************************

void DeleteWithConfirmation(const char* szMask, const char* szTitle, CWnd* pParent)
{
	int nCount = 0;
	CString strMsg = "";
	UINT nType = MB_ICONINFORMATION;

	if ((nCount = ::CountFiles(szMask)) == 0)
	{
		strMsg = "No files were found to delete!";
	}
	else
	{
		CString strMsg = "";
		strMsg.Format("%d files will be deleted.\n\nDo you wish to continue?", nCount);
		if (Prompter.YesNo(strMsg, szTitle) == IDYES)
		{
			CFileRecycler::SendToRecycleBin(szMask);

			if ((nCount = ::CountFiles(szMask)) == 0)
			{
				strMsg = "All nominated files have been deleted.";
			}
			else
			{
				strMsg.Format("Not all files could be deleted, %d remain!", nCount);
				nType = MB_ICONEXCLAMATION;
			}
		}
	}

	if (strMsg != "")
	{
		Prompter.DisplayMessage(strMsg, szTitle, nType);
	}
}

//*******************************************************************

bool ValidateFolderAccess ( const char* szPath )
{
	bool bReply = FALSE;
	CString strFilename = "";
	strFilename.Format ( "%s\\%s", 
		szPath, 
		(const char*) GetUniqueTempFilename("$$$") );			// cretae a temp file

	CSSFile file;
	if ( ( bReply = file.Open ( strFilename, "wb" ) ) == TRUE )
	{
		file.Close();
		CFileRecycler::SendToRecycleBin ( strFilename );
	}
	return bReply;
}

/**********************************************************************/

CString GetTidyAppName( bool bServer )
{
	CString strAppName = bServer ? "SPos SmartPay Server" : "SPos SmartPay Manager"; 
	return strAppName;
}

/**********************************************************************/

void GetDataProgramPath( CString& strPath )
{
	CString strTemp = Sysset.GetProgramPath();
	strTemp += "\\";
	strTemp += strPath;
	strPath = strTemp;
}

/**********************************************************************/

INT_PTR DoFileDialog(CFileDialog& dlg)
{
	AutoShutdownHelper.SetTimerBlock2(TRUE);
	INT_PTR nResult = dlg.DoModal();
	AutoShutdownHelper.SetTimerBlock2(FALSE);
	return nResult;
}

/**********************************************************************/

bool DoBrowseFolder(CString& strPathname, const char* szText, const char* szStartFolder, CWnd* pWnd)
{
	AutoShutdownHelper.SetTimerBlock2(TRUE);
	bool bResult = BrowseFolder(strPathname, szText, szStartFolder, pWnd);
	AutoShutdownHelper.SetTimerBlock2(FALSE);
	return bResult;
}

/**********************************************************************/

int DoMessageBox( CWnd* pParent, LPCTSTR lpszText, LPCTSTR lpszCaption, UINT nType )
{
	AutoShutdownHelper.SetTimerBlock2(TRUE);
	int nResult = pParent->MessageBox(lpszText, lpszCaption, nType);
	AutoShutdownHelper.SetTimerBlock2(FALSE);
	return nResult;
}

/**********************************************************************/

int DoAfxMessageBox( LPCTSTR lpszText, UINT nType )
{
	AutoShutdownHelper.SetTimerBlock2(TRUE);
	int nResult = AfxMessageBox(lpszText, nType);
	AutoShutdownHelper.SetTimerBlock2(FALSE);
	return nResult;
}

//********************************************************************
#endif
//********************************************************************
#endif
//********************************************************************
#endif
//********************************************************************

//********************************************************************
#ifndef SYSTEMTYPE_EMAILBATCHSEND
//********************************************************************
#ifndef SMP_TRAYICON
//********************************************************************

int Compare8601Times(const CString strTime1, const CString strTime2)
{
	CkDateTime time1, time2;

	if ((!time1.SetFromTimestamp(strTime1)) || (!time2.SetFromTimestamp(strTime2)))
	{
		return -2;
	}

	int nDiffSeconds = time1.DiffSeconds(time2);

	if (nDiffSeconds < 0)
	{
		return -1;
	}
	else if (nDiffSeconds > 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

//********************************************************************

void ConvertOleDateTimeTo8601Time( const COleDateTime oleTime, CString& strTime )
{
	strTime = "";
	CkDateTime time;
	time.SetFromOleDate(true, oleTime.m_dt);
	strTime = time.getAsIso8601("YYYY-MM-DDThh:mm:ss:sssTZD", false);
}	

//********************************************************************

void Convert8601TimeToUKTime(const CString strTime1, CString& strTime2)
{
	strTime2 = "";

	CkDateTime time1;
	if ( time1.SetFromTimestamp(strTime1) == FALSE )
	{
		return;
	}

	strTime2 = time1.getAsIso8601("YYYY-MM-DDThh:mm:ss", true);
}

//********************************************************************
#endif
//********************************************************************
#endif
//********************************************************************

int HexToInt(const char* szHex)
{
	int nResult = 0;

	CString strHex = szHex;
	TrimSpacesFromString(strHex, TRUE);

	for (int nPos = 0; nPos < strHex.GetLength(); nPos++)
	{
		nResult *= 16;

		char c = strHex.GetAt(nPos);

		if (c >= '0' && c <= '9')
		{
			nResult += c - '0';
		}
		else if (c >= 'A' && c <= 'F')
		{
			nResult += (c + 10 - 'A');
		}
		else if (c >= 'a' && c <= 'f')
		{
			nResult += (c + 10 - 'a');
		}
	}
	return nResult;
}

/**********************************************************************/

bool TestNumeric(const CString& strText, int nMaxLen)
{
	if ((nMaxLen > 0) && ((strText.GetLength() == 0) || (strText.GetLength() > nMaxLen)))
	{
		return FALSE;
	}

	for (int nPos = 0; nPos < strText.GetLength(); nPos++)
	{
		char c = strText.GetAt(nPos);
		if ((c < '0') || (c > '9'))
		{
			return FALSE;
		}
	}
	return TRUE;
}

/**********************************************************************/

bool TestNumericHex(const CString& strText, int nMaxLen)
{
	if ((nMaxLen > 0) && ((strText.GetLength() == 0) || (strText.GetLength() > nMaxLen)))
	{
		return FALSE;
	}

	for (int nPos = 0; nPos < strText.GetLength(); nPos++)
	{
		char c = strText.GetAt(nPos);

		if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'))
		{
			continue;
		}
		else
		{
			return FALSE;
		}
	}
	return TRUE;
}

/**********************************************************************/

bool FileExists  ( const char* szFile )
{ 
	return ( _access ( szFile, 0 ) == 0 ) ? TRUE : FALSE;
}

/**********************************************************************/

CString OpenFileFailureMessage ( const char* szFilename, int nMode )
{
	CString strMsg = "";
	strMsg.Format ( "Cannot open file ' %s ' for ", szFilename );

	if (nMode == DB_READONLY)
	{
		strMsg += "Reading!";
	}
	else
	{
		strMsg += "Read/Write!";
	}

	return strMsg;
}

//*********************************************************************
#ifndef SYSTEMTYPE_EMAILBATCHSEND
//*********************************************************************

bool IsAccountInuse ( const char* szCardNo )
{
	bool bReply = FALSE;
	CString strFilename = Filenames.GetRecordInUseFilename ( szCardNo );

	int nRetries = 1;
	CSSFile file;
	if (file.Open(strFilename, "wb", _SH_DENYRW, nRetries) == FALSE)
	{
		bReply = TRUE;
	}
	else
	{
		file.Close();
		CFileRecycler::SendToRecycleBin ( strFilename );
	}

	return bReply;
}

//*********************************************************************
// remove a number of lines from start of given file ( & save in dest file if reqd )

bool RemoveFileLines ( const char* szSourceFilename, int nLines, bool bHaveHeader, const char* szDestFilename )
{
	if (nLines < 1)
	{
		return TRUE;										// nothing to do
	}

	CSSFile fileIn;
	if (fileIn.Open(szSourceFilename, "rb") == FALSE)
	{
		return FALSE;
	}

	CString strHeader = "";
	if ( bHaveHeader == TRUE )								// if using headers - check destinatioon file has one
	{
		if ( fileIn.ReadLine ( strHeader ) == FALSE )		// try & read header line
		{
			fileIn.Close();
			return FALSE;									// no data
		}
	}

// if see have destination file to save removed lines to

	bool bDestOutputReqd = FALSE;

	CSSFile fileDest;
	CString strDestFilename = szDestFilename;
	if ( strDestFilename != "" )								// see if destination filename passed to save removed lines in
	{
		CString strMode = "wb";
		bool bFileExists = FALSE;								// assume file does not exist
		
		if ((bFileExists = ::FileExists(strDestFilename)) == TRUE)
		{
			strMode = "ab";										// append to existing file
		}

		fileDest.Open ( strDestFilename, strMode );

		if (bFileExists == FALSE)
		{
			if (bHaveHeader == TRUE)
			{
				fileDest.WriteString(strHeader);				// line already has cr/lf
			}
		}

		bDestOutputReqd = TRUE;
	}

// start scan

	CString strTmpFilename = Filenames.GetTempPathFilename ( ::GetUniqueTempFilename("$$1") );		// temp file

	bool bReply = FALSE;
	CSSFile fileOut;
	if ( ( bReply = fileOut.Open ( strTmpFilename, "wb" ) ) == TRUE )
	{
		if (bHaveHeader == TRUE)
		{
			fileOut.WriteString(strHeader);					// line already has cr/lf
		}

		CString strBuf = "";
		for ( int i = 0 ; i < nLines ; i++ )
		{
			fileIn.ReadLine ( strBuf );

			if ( bDestOutputReqd == TRUE )
			{
				fileDest.WriteString ( strBuf );				// line already has cr/lf
			}
		}

		while (fileIn.ReadLine(strBuf) == TRUE)
		{
			bReply = fileOut.WriteString(strBuf);			// line already has cr/lf
		}

		fileOut.Close();
	}
	fileIn.Close();

	if (TRUE == bDestOutputReqd)
	{
		fileDest.Close();
	}

//tidy up filea

	if ( bReply == TRUE )
	{
		CFileRecycler::SendToRecycleBin ( szSourceFilename );
		int nRenameResult = ::rename ( strTmpFilename, szSourceFilename );
	}
	else
	{
		CFileRecycler::SendToRecycleBin ( strTmpFilename );
		
		if (TRUE == bDestOutputReqd)
		{
			CFileRecycler::SendToRecycleBin(strDestFilename);
		}
	}

	return bReply;
}

//********************************************************************

bool CheckDoubleMatch(double dValue1, double dValue2)
{
	CString strValue1 = "";
	strValue1.Format("%.*lf", System.GetDPValue(), dValue1);	// lose rounding errors on value
	
	CString strValue2 = "";
	strValue2.Format("%.*lf", System.GetDPValue(), dValue2);	// lose rounding errors on value

	return (strValue1 == strValue2) ? TRUE : FALSE;
}

//********************************************************************
#endif
//********************************************************************

int CountFiles(const char* szFileMask)
{
	int nCount = 0;

	CFileFind FileFinder;
	BOOL bWorking = FileFinder.FindFile(szFileMask);
	while (bWorking)
	{
		bWorking = FileFinder.FindNextFile();
		if (FileFinder.IsDots() == TRUE || FileFinder.IsDirectory() == TRUE)
		{
			continue;
		}

		++nCount;
	}

	return nCount;
}

//*********************************************************************
// Copy max lines from one file to another

int CopyFileLines ( const char* szSourceFilename, const char* szDestFilename, int nMaxLines, bool bHaveHeader  )
{
	int nCount = -1;

	CSSFile fileSource;
	if ( fileSource.Open ( szSourceFilename, "rb" ) == TRUE )
	{
		CSSFile fileDest;
		if ( fileDest.Open ( szDestFilename, "wb" ) == TRUE )
		{
			CString strBuf;
			bool bOk = TRUE;

			if (bHaveHeader == TRUE)
			{
				if ((bOk = fileSource.ReadLine(strBuf)) == TRUE)
				{
					bOk = fileDest.WriteString(strBuf);		// line already has cr/lf
				}
			}

			if ( bOk == TRUE )
			{
				nCount = 0;

				for ( int i = 0 ; i < nMaxLines ; i++ )
				{
					if (fileSource.ReadLine(strBuf) == FALSE)
					{
						break;
					}

					if (fileDest.WriteString(strBuf) == TRUE)			// line already has cr/lf
					{
						++nCount;
					}
				}
			}
			fileDest.Close();
		}
		fileSource.Close();
	}

	return nCount;
}

//********************************************************************

void SendToRecycleBin ( CStringArray* pArray )
{
	for (int i = 0; i < pArray->GetSize(); i++)
	{
		remove(pArray->GetAt(i));
	}
}

//********************************************************************

CString FormatGroupNo ( int nGroupNo )
{
	CString strBuf = "";
	strBuf.Format ( "%2.2d", nGroupNo );
	return strBuf;
}

//********************************************************************

CString FormatInt64Value(__int64 nValue)
{
	CString strBuf = "";
	strBuf.Format("%I64d", nValue);
	return strBuf;
}

//********************************************************************

CString FormatIntValue ( int nValue )
{
	CString strBuf = "";
	
	if (nValue != 0L)
	{
		strBuf.Format("%+d", nValue);
	}
	else
	{
		strBuf = "0";
	}

	return strBuf;
}

//********************************************************************

CString FormatDoubleValue(double dValue)
{
	CString strBuf = "";

	if (dValue != 0.0)
	{
		strBuf.Format("%+.2lf", dValue);
	}
	else
	{
		strBuf = "0.00";
	}

	return strBuf;
}

//********************************************************************

double RoundDoubleValue ( double dValue )
{
	CString strValue = "";
	strValue.Format ( "%.2lf", dValue );			// lose rounding errors on value
	return atof ( (const char*)strValue );
}

//********************************************************************

CString FormatQty ( double dQtyValue )
{
	double n = 0.0;
	int nDps = ( modf( dQtyValue, &n ) != 0.0 ) ? 1 : 0;

	CString strValue = "";
	strValue.Format ( "%.*lf", nDps, dQtyValue );
	return strValue;
}

/******************************************************************************/
// count number of lines in a file 

int GetFileLineCount(const char* szFilename)
{
	int nCount = 0;

	CSSFile file;
	if (file.Open(szFilename, "rb") == TRUE)
	{
		CString strBuf;
		while (file.ReadLine(strBuf) == TRUE)
		{
			if (strBuf.GetLength() > 2)			// cr / lf
			{
				++nCount;
			}
		}

		file.Close();
	}

	return nCount;
}

//*******************************************************************
// szpathname = "c:\path1\path2\fred.dat, strFilename = fred.dat

CString GetFileExtension(const char* szFilename)
{
	CString strExtension = "";
	CString strFilename = szFilename;

	for (int i = strFilename.GetLength() - 1; i >= 0; i--)
	{
		unsigned char uc = strFilename.GetAt(i);

		if (uc == '.')
		{
			break;
		}

		strExtension += uc;
	}

	strExtension.MakeReverse();
	return strExtension;
}

//*******************************************************************
// szpathname = "c:\path1\path2\fred.dat", strFilename = "c:\path1\path2\fred"

CString StripFileExtension ( const char* szFilename )
{
	CString strFilename = szFilename;

	for ( int i = strFilename.GetLength() - 1 ; i >= 0 ; i-- )
	{
		unsigned char uc = strFilename.GetAt(i);

		if ( uc == '.' )
		{
			strFilename.SetAt(i,0x0);
			break;
		}
	}

	return strFilename;
}

//*******************************************************************
// szpathname = "c:\path1\path2\fred.dat, strFilename = fred.dat

CString StripPathname ( const char* szPathname )
{
	CString strPathname = szPathname;

	unsigned char uc;
	CString strFilename;

	for ( int i = strPathname.GetLength() - 1 ; i >= 0 ; i-- )
	{
		uc = strPathname.GetAt(i);

		if (uc == '//')
		{
			break;
		}

		if (uc == ':')
		{
			break;
		}

		if (uc == '\\')
		{
			break;
		}

		strFilename += uc;
	}

	strFilename.MakeReverse();
	return strFilename;
}

//*******************************************************************
// szfullpath = "c:\path1\path2\fred.dat, strpath = c:\path1\path2

CString StripFilename(const char* szFullPathname)
{
	CString strPath = "";
	StripFilename(szFullPathname, strPath);
	return strPath;
}

//*******************************************************************

void StripFilename(const char* szFullPathname, CString& strPath)
{
	strPath = "";

	CString strFullPathname = szFullPathname;
	int nFullPathLength = strFullPathname.GetLength();

	if (0 == nFullPathLength)							// no pathname
	{
		return;
	}

	CString strFilename = StripPathname(strFullPathname);		// strip trailing filename from path

	int nFilenameLength = strFilename.GetLength();		// make sure have a filename
	if (0 == nFilenameLength)
	{
		return;
	}

	if (nFullPathLength > nFilenameLength + 1)		// lose filename from rest of pathname
	{
		strPath = strFullPathname.Left(nFullPathLength - (nFilenameLength + 1));
	}
}

//*******************************************************************
//*** append data file ( DO NOT USE FOR AUDIT FILES )****************
//***                  ( Use AuditFile::AppendAuditFile function )  *
//*******************************************************************

bool AppendDataFile(const char* szSourceFilename, const char* szDestFilename, bool bHaveHeader)
{
	CString strBuf = "";
	CSSFile fileOut;

	if (TRUE == bHaveHeader)								// if using headers - check destinatioon file has one
	{
		bHaveHeader = FALSE;									// assume not
		if (fileOut.Open(szDestFilename, "rb") == TRUE)	// file may not exist
		{
			bHaveHeader = fileOut.ReadLine(strBuf);				// try & read header line
		}

		fileOut.Close();
	}

	CSSFile fileIn;
	if (fileIn.Open(szSourceFilename, "rb") == FALSE)
	{
		return FALSE;
	}

	if (fileOut.Open(szDestFilename, "ab") == FALSE)
	{
		fileIn.Close();
		return FALSE;
	}

	bool bReply = TRUE;

	if (bHaveHeader == TRUE)
	{
		fileIn.ReadLine(strBuf);						// skip header line
	}

	while (fileIn.ReadLine(strBuf) == TRUE)
	{
		bReply = fileOut.WriteString(strBuf);			// line already has cr/lf
	}

	fileIn.Close();
	fileOut.Close();

	return bReply;
}

//*******************************************************************

void TrimSpacesFromString( CString& strText, bool bRemoveLeadingZeros )
{
	strText.TrimLeft ( ' ' );
	strText.TrimRight ( ' ' );
	
	if ( TRUE == bRemoveLeadingZeros ) 
	{
		strText.TrimLeft ( '0' );
	}
}

//*******************************************************************

CString RemoveSpaces ( const char* szText )
{
	CString strBuf = szText;
	CString strOutput = "";

	int nLength = strBuf.GetLength();

	for (int i = 0; i < nLength; i++)
	{
		unsigned char uc = strBuf.GetAt(i);

		if (uc != ' ')
		{
			strOutput += uc;
		}
	}

	return strOutput;
}

//*******************************************************************

bool IsStringNumeric(const char* szBuf, int nMin, int nMax)
{
	CString strBuf = szBuf;
	int nLength = strBuf.GetLength();

	if (nLength < nMin)
	{
		return FALSE;
	}

	if ((nMax > 0) && (nLength > nMax))
	{
		return FALSE;
	}

	bool bReply = TRUE;
	for (int i = 0; i < nLength; i++)
	{
		unsigned char uc = strBuf.GetAt(i);

		if ((uc < '0') || (uc > '9'))
		{
			bReply = FALSE;
			break;
		}
	}
	return bReply;
}

//*******************************************************************
// create a dd/mm/yyyy date from 28 Feb... or 10.08.02 etc...

CString ExtractDate(const char* szDateString)
{
	CString strDate = szDateString;
	CString strReply = "";

	char nDelimitor = '/';									// 28/01.... 
	if (strDate.Find(nDelimitor) == -1)				// '/' not found
	{
		nDelimitor = ' ';									// 28 Jan....
		if (strDate.Find(nDelimitor) == -1)			// ' ' not found
		{
			nDelimitor = '-';								// 28-Jan....
			if (strDate.Find(nDelimitor) == -1)		// '-' not found
			{
				nDelimitor = '.';							// 28.Jan....
				if (strDate.Find(nDelimitor) == -1)	// '.' not found
				{
					nDelimitor = 0;							// no delimitor
				}
			}
		}
	}

	strReply = strDate;										// default to return what was sent 'as is'

	if (nDelimitor != 0)									// see if found
	{
		CCSV csvDate(strDate, nDelimitor);				//allow for 28-Jan-90 & 28-Jan-1990

		int nDay = csvDate.GetInt(0);
		int nMonth = ::GetMonthNo(csvDate.GetString(1));	// may be 01/ Jan / January
		int nYear = csvDate.GetInt(2);

		if (nMonth != 0)
		{
			if (nYear < 100)
			{
				CSSDate dateToday;
				dateToday.SetCurrentDate();
				int nYearNow = dateToday.GetYear() - 2000;

				if (nYear > nYearNow)
				{
					nYear += 1900;
				}
				else
				{
					nYear += 2000;
				}
			}

			strReply.Format("%2.2d/%2.2d/%4.4d", nDay, nMonth, nYear);
		}
	}

	return strReply;
}

//*******************************************************************
// szMonth = 01 or Jan or January

int GetMonthNo ( const char* szMonth )
{
	int nMonth = 0;
	CString strMonth = szMonth;

	if ( strMonth.GetLength() < 3 )
	{
		if (::IsStringNumeric(strMonth, 1, 2) == TRUE)
		{
			nMonth = atoi(strMonth);
		}
	}
	else
	{
		if (strMonth.GetLength() > 3)
		{
			strMonth.SetAt(3, 0);
		}
			
		if (strMonth.CompareNoCase("Jan") == 0)
		{
			nMonth = 1;
		}
		else if (strMonth.CompareNoCase("Feb") == 0)
		{
			nMonth = 2;
		}
		else if (strMonth.CompareNoCase("Mar") == 0)
		{
			nMonth = 3;
		}
		else if (strMonth.CompareNoCase("Apr") == 0)
		{
			nMonth = 4;
		}
		else if (strMonth.CompareNoCase("May") == 0)
		{
			nMonth = 5;
		}
		else if (strMonth.CompareNoCase("Jun") == 0)
		{
			nMonth = 6;
		}
		else if (strMonth.CompareNoCase("Jul") == 0)
		{
			nMonth = 7;
		}
		else if (strMonth.CompareNoCase("Aug") == 0)
		{
			nMonth = 8;
		}
		else if (strMonth.CompareNoCase("Sep") == 0)
		{
			nMonth = 9;
		}
		else if (strMonth.CompareNoCase("Oct") == 0)
		{
			nMonth = 10;
		}
		else if (strMonth.CompareNoCase("Nov") == 0)
		{
			nMonth = 11;
		}
		else if (strMonth.CompareNoCase("Dec") == 0)
		{
			nMonth = 12;
		}
	}

	return nMonth;
}

//*******************************************************************

CString FormatXmlDate ( CSSDate* pDate, CSSTime* pTime )
{
	CString strXmlDate = "";

	if (pTime == NULL)
	{
		strXmlDate.Format("%4.4d-%2.2d-%2.2d", pDate->GetYear(), pDate->GetMonth(), pDate->GetDay());
	}
	else
	{
		strXmlDate.Format("%4.4d-%2.2d-%2.2dT%2.2d:%2.2d:%2.2d", pDate->GetYear(), pDate->GetMonth(), pDate->GetDay(), pTime->GetHour(), pTime->GetMinute(), pTime->GetSecond());
	}

	return strXmlDate;
}

//*******************************************************************

CString FormatXmlDate ( const char* szDate, const char* szTime )
{
	CString strXmlDate = "";

	CSSDate date ( szDate );
	CSSTime time ( szTime );

	if (time.IsSet() == FALSE)
	{
		strXmlDate = FormatXmlDate(&date);
	}
	else
	{
		strXmlDate = FormatXmlDate(&date, &time);
	}

	return strXmlDate;
}

//*******************************************************************
// create a temp file name ( default file extension = $$$ )

// $$$ - Global  - Validate folder access
// $$1 - Global  - Remove File Lines

// $02 - Server  - EPOS temp payment Auditfile

// $$5 - Manager - Import Accounts
// $03 - Manager - Archive ATC
// $04 - Manager - Archive Epos payment audit
// $05 - Manager - Write ATC archive
// $06 - Manager - csv import
// $07 - Manager - topup import
// $08 - Manager - custom csv import

// $09 - Manager - ParentMail export
// $10 - Manager - Schoolcomms send balance
// $10 - Manager - Squid acknowledgement
// $11 - Manager - Schoolcomms send Transactions
// $11 - Manager - Tucasi Json balances
// $12 - Manager - Tucasi Json transactions
// $12 - Manager - Schoolcomms Disassociate card
// $12 - Manager - WebPayment Import Log - resend data
// $14 - Manager - ParentPay Resend account match ata
// $15 - Manager - ParentPay Resend payment echo data
// $16 - Manager - ParentPay Resend transactions
// $20 - Manager - Resend webpayments
// $22 - Manager - Webpayment get unlink accounts
// $23 - Manager - Webpayment link accounts
// $31 - Manager - Webpayment purchase file Diner
// $32 - Manager - Webpayment purchase file secondary service (WisePay)

CString GetUniqueTempFilename  ( const char* szFileExtension, const char* szPrefix )
{
	CString strFilename = "";

	CTime t = CTime::GetCurrentTime();
	strFilename.Format ( "%s-%d%2.2d%2.2d%2.2d%2.2d%2.2d.%s", szPrefix, t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond(), szFileExtension );

	return strFilename;
}

//*******************************************************************

CString CompileName(const char* szName1, const char* szName2, char cSeparator)
{
	CString strName = szName1;
	strName.TrimLeft(' ');
	strName.TrimRight(' ');

	CString strName2 = szName2;
	strName2.TrimLeft(' ');
	strName2.TrimRight(' ');

	if (strName2 != "")
	{
		if (strName != "")
		{
			strName += cSeparator;
		}

		strName += strName2;
	}

	return strName;
}

//*******************************************************************

bool LogError ( const char* szError, const char* szFilename )
{
	bool bReply = FALSE;

	CSSFile file;
	if ( ( bReply = file.Open ( szFilename, "ab" ) ) == TRUE )
	{
		CTime tm = CTime::GetCurrentTime();
		
		CCSV csv;
		csv.Add ( tm.Format ( "%d/%m/%Y" ) );
		csv.Add ( tm.Format ( "%H:%M:%S" ) );
		csv.Add ( szError );
		file.Write ( csv );
		file.Close();
	}

	return bReply;
}


//*******************************************************************
//	"..\Log\yyyy\SysError.dat"
//	"..\Log\yyyy\pfilelog-yyyymmdd.dat"
//	"..\Log\yyyy\sfilelog-yyyymm.dat"
//  "..\Log\yyyy\sx-yyyymm.dat"

bool LogSysFileError(const char* szError, const char* szFilename, const char* szPrefixText)
{
	bool bReply = FALSE;
	CString strFilename = szFilename;
	CString strPrefixText = szPrefixText;

	CTime tm = CTime::GetCurrentTime();

	CString strFolder;
	strFolder.Format("%s\\SysFiles\\Logs\\%d",
		(const char*)Sysset.GetProgramPath(),
		tm.GetYear());

	if (::ExistSubdirectory(strFolder) == FALSE)
	{
		::MakeSubdirectory(strFolder);
	}

	if (strFilename == "")
	{
		strFilename = "SysError.dat";
	}

	CString strPathFilename = "";
	strPathFilename.Format("%s\\%s",
		(const char*)strFolder,
		(const char*)strFilename);

	CSSFile file;
	if ((bReply = file.Open(strPathFilename, "ab")) == TRUE)
	{
		CCSV csv;
		csv.Add(tm.Format("%d/%m/%Y"));
		csv.Add(tm.Format("%H:%M:%S"));

		if (strPrefixText != "")									// check have a additional text
		{
			csv.Add(strPrefixText);
		}

		csv.Add(szError);
		file.Write(csv);
		file.Close();
	}

	return bReply;
}

//*******************************************************************

void CreateSubdirectory ( const char* szDir )
{
	CString strDir = szDir;
	if (::ExistSubdirectory(strDir) == FALSE)
	{
		::MakeSubdirectory(strDir);
	}
}

/**********************************************************************/

void GetPCPathHost( CString& strPath )
{
	CString strTemp = Sysset.GetPCPathHost();
	strTemp += "\\";
	strTemp += strPath;
	strPath = strTemp;
}

/**********************************************************************/

void GetPCPathClient(CString& strPath)
{
	CString strTemp = Sysset.GetPCPathClient();
	strTemp += "\\";
	strTemp += strPath;
	strPath = strTemp;
}

/**********************************************************************/

void GetSyssetProgramPath( CString& strPath )
{
	CString strTemp = Sysset.GetSyssetProgramPath();
	strTemp += "\\";
	strTemp += strPath;
	strPath = strTemp;
}

/**********************************************************************/

int Rename_Trap(CString strSource, CString strDest)
{
	int nResult = rename(strSource, strDest);

	int nTrap = 0;
	if (nResult != 0)
	{
		nTrap = errno;
	}

	return nResult;
}

/**********************************************************************/

int CompareDoubles(double dDouble1, double dDouble2, int nDp)
{
	CString strDouble1 = "", strDouble2 = "";
	strDouble1.Format("%.*f", nDp, dDouble1);
	strDouble2.Format("%.*f", nDp, dDouble2);

	if (strDouble1 == strDouble2)
	{
		return 0;
	}

	if ((strDouble1.Left(1) == "-") != (strDouble2.Left(1) == "-"))
	{
		CString strNegativeZero = "-0";

		if (nDp != 0)
		{
			strNegativeZero += ".";
			::AddTrailing(strNegativeZero, nDp + 3, '0');
		}

		if (strDouble1 == strNegativeZero)
		{
			strDouble1 = strDouble1.Right(strDouble1.GetLength() - 1);
		}

		if (strDouble2 == strNegativeZero)
		{
			strDouble2 = strDouble2.Right(strDouble2.GetLength() - 1);
		}
	}

	if (strDouble1 == strDouble2)
	{
		return 0;
	}
	else if (dDouble1 < dDouble2)
	{
		return -1;
	}
	else
	{
		return 1;
	}
}

/**********************************************************************/

bool CompareNoCase(CString str1, CString str2)
{
	str1.MakeLower();
	str2.MakeLower();
	return (str1 == str2);
}

/**********************************************************************/

CString SimpleFormatIntValue(int nValue)
{
	CString strBuf = "";
	strBuf.Format("%d", nValue);
	return strBuf;
}

//*******************************************************************

CString SimpleFormatInt64Value(__int64 n)
{
	CString strResult = "";
	strResult.Format("%I64d", n);
	return strResult;
}

/**********************************************************************/

CString ConvertISO8601ToSimpleDateTime(CString strISO)
{
	CString strDateTime = "";

	if (strISO.GetLength() < 19)
	{
		return "";
	}

	strDateTime = strISO.Left(4);
	if (::TestNumeric(strDateTime) == FALSE)
	{
		return "";
	}

	if (strISO.Mid(4, 1) != "-")
	{
		return "";
	}

	strDateTime += strISO.Mid(5,2);
	if (::TestNumeric(strDateTime.Right(2)) == FALSE)
	{
		return "";
	}

	if (strISO.Mid(7, 1) != "-")
	{
		return "";
	}

	strDateTime += strISO.Mid(8, 2);
	if (::TestNumeric(strDateTime.Right(2)) == FALSE)
	{
		return "";
	}

	if (strISO.Mid(10, 1) != "T")
	{
		return "";
	}

	strDateTime += strISO.Mid(11, 2);
	if (::TestNumeric(strDateTime.Right(2)) == FALSE)
	{
		return "";
	}

	if (strISO.Mid(13, 1) != ":")
	{
		return "";
	}

	strDateTime += strISO.Mid(14, 2);
	if (::TestNumeric(strDateTime.Right(2)) == FALSE)
	{
		return "";
	}

	if (strISO.Mid(16, 1) != ":")
	{
		return "";
	}

	strDateTime += strISO.Mid(17, 2);
	if (::TestNumeric(strDateTime.Right(2)) == FALSE)
	{
		return "";
	}

	COleDateTime timeTest = COleDateTime(
		atoi(strDateTime.Left(4)),
		atoi(strDateTime.Mid(4,2)),
		atoi(strDateTime.Mid(6,2)),
		atoi(strDateTime.Mid(8,2)),
		atoi(strDateTime.Mid(10,2)),
		atoi(strDateTime.Right(2)));

	if (timeTest.m_status != COleDateTime::valid)
	{
		return "";
	}
	else
	{
		return strDateTime;
	}
}

/**********************************************************************/

CString ConvertSimpleDateTimeToISO8601(CString strSimple)
{
	CString strISO = "";

	if (strSimple.GetLength() != 14)
	{
		return "";
	}

	if (::TestNumeric(strSimple) == FALSE)
	{
		return "";
	}

	COleDateTime oleTest = COleDateTime(
		atoi(strSimple.Left(4)),
		atoi(strSimple.Mid(4,2)),
		atoi(strSimple.Mid(6,2)),
		atoi(strSimple.Mid(8,2)),
		atoi(strSimple.Mid(10,2)),
		atoi(strSimple.Right(2)));

	if (oleTest.m_status != COleDateTime::valid)
	{
		return "";
	}

	strISO.Format("%4.4d-%2.2d-%2.2dT%2.2d:%2.2d:%2.2d.000Z",
		oleTest.GetYear(),
		oleTest.GetMonth(),
		oleTest.GetDay(),
		oleTest.GetHour(),
		oleTest.GetMinute(),
		oleTest.GetSecond());

	return strISO;
}

/**********************************************************************/

CString ConvertAuditDateTimeToISO8601(CString strDate, CString strTime)
{
	if (strDate.GetLength() != 10)
	{
		return "";
	}

	if (strTime.GetLength() != 5)
	{
		return "";
	}

	CString strSimple = "";
	strSimple += strDate.Right(4);
	strSimple += strDate.Mid(3, 2);
	strSimple += strDate.Left(2);
	strSimple += strTime.Left(2);
	strSimple += strTime.Right(2);
	strSimple += "00";

	return ConvertSimpleDateTimeToISO8601(strSimple);
}

/**********************************************************************/

void AdjustAuditDate(CString& strDate, CString& strTime, int nMinutes)
{
	if (strDate.GetLength() != 10)
	{
		return;
	}

	if (strTime.GetLength() != 5)
	{
		return;
	}

	CString strSimple = "";
	strSimple += strDate.Right(4);
	strSimple += strDate.Mid(3, 2);
	strSimple += strDate.Left(2);
	strSimple += strTime.Left(2);
	strSimple += strTime.Right(2);

	if (::TestNumeric(strSimple) == FALSE)
	{
		return;
	}

	COleDateTime timeAudit = COleDateTime(
		atoi(strSimple.Left(4)),
		atoi(strSimple.Mid(4, 2)),
		atoi(strSimple.Mid(6, 2)),
		atoi(strSimple.Mid(8, 2)),
		atoi(strSimple.Right(2)),
		0);

	if (timeAudit.m_status != COleDateTime::valid)
	{
		return;
	}

	timeAudit += COleDateTimeSpan(0, 0, nMinutes, 0);

	strDate.Format("%2.2d/%2.2d/%4.4d",
		timeAudit.GetDay(),
		timeAudit.GetMonth(),
		timeAudit.GetYear());

	strTime.Format("%2.2d:%2.2d",
		timeAudit.GetHour(),
		timeAudit.GetMinute());
}

/**********************************************************************/

CString GetGCSafeTag(CString strLabel, CString strValue)
{
	CString strResult = "";
	strResult += strLabel;
	strResult += "=";
	strResult += GetGCSafeText(strValue);
	return strResult;
}

/**********************************************************************/

CString GetGCSafeText(CString strOriginal)
{
	CString strResult = strOriginal;
	strResult.Replace(',', ' ');
	strResult.Replace('"', ' ');
	return strResult;
}

/**********************************************************************/

void WriteBackgroundBusyFile()
{
/*
#ifdef SYSTEMTYPE_BACKGROUND
	CSSFile fileInfo;
	if (fileInfo.Open(Filenames.GetBgndServiceBusyTimeFilename(), "wb") == TRUE)
	{
		CString strInfo = "";
		strInfo.Format("PCId = %10.10d",
			Sysset.GetPCId());

		CCSV csv;
		csv.Add(strInfo);
		csv.Add(GetSimpleDateTimeNowString());
		fileInfo.WriteLine(csv.GetLine());
	}
#endif
*/
}

/**********************************************************************/

void CKUnlock()
{
	CkGlobal glob;

	bool success = glob.UnlockBundle("SPOSUK.CB1082028_DzA3mERU2EoF");
	if (success != true) {
		AfxMessageBox(glob.lastErrorText());
		return;
	}

	int status = glob.get_UnlockStatus();
	if (status == 2)
	{
		//UNLOCKED WITH PURCHASED CODE;
	}
	else
	{
		//UNLOCKED IN TRIAL MODE;
	}
}

/**********************************************************************/

CString GetSimpleDateTimeNowString()
{
	COleDateTime timeNow = COleDateTime::GetCurrentTime();
	
	CString strDateTime = "";
	strDateTime.Format("%4.4d%2.2d%2.2d%2.2d%2.2d%2.2d",
		timeNow.GetYear(),
		timeNow.GetMonth(),
		timeNow.GetDay(),
		timeNow.GetHour(),
		timeNow.GetMinute(),
		timeNow.GetSecond());

	return strDateTime;
}

/**********************************************************************/

void GetMessageLogDateTime(CString& strDate, CString& strTime)
{
	auto now = std::chrono::system_clock::now();
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
	std::time_t t = std::chrono::system_clock::to_time_t(now);
	std::tm tm;
	localtime_s(&tm, &t);

	// Format date as DD/MM/YYYY
	std::ostringstream ossDate;
	ossDate << std::setw(2) << std::setfill('0') << tm.tm_mday << "/"
	        << std::setw(2) << std::setfill('0') << (tm.tm_mon + 1) << "/"
	        << std::setw(4) << std::setfill('0') << (tm.tm_year + 1900);
	strDate = CString(ossDate.str().c_str());

	// Format time as HH:MM:SS.mmm
	std::ostringstream ossTime;
	ossTime << std::setw(2) << std::setfill('0') << tm.tm_hour << ":"
	        << std::setw(2) << std::setfill('0') << tm.tm_min << ":"
	        << std::setw(2) << std::setfill('0') << tm.tm_sec << "."
	        << std::setw(3) << std::setfill('0') << ms.count();
	strTime = CString(ossTime.str().c_str());
}

/**********************************************************************/
