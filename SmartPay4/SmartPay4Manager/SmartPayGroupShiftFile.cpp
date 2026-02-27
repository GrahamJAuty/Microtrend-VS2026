//******************************************************************************
#include "..\SmartPay4Shared\RepmanDefines.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLTable_ClosingBalance\SQLRepositoryClosingBalance.h"
//******************************************************************************
#include "SmartPayGroupShiftFile.h"
//******************************************************************************

bool CSmartPayGroupShiftLogLine::Read(CString str)
{
	bool bResult = FALSE;
	CCSV csv(str);

	if (csv.GetInt(0) == 1)
	{
		SetActionDate(csv.GetString(1));
		SetActionTime(csv.GetString(2));
		SetActionSource(csv.GetInt(3));
		SetUserID(csv.GetInt64(4));
		SetUsername(csv.GetString(5));
		SetCurrentGroupNo(csv.GetInt(6));
		SetOriginalStatus(csv.GetInt(7));
		SetOriginalDateFrom(csv.GetString(8));
		SetOriginalDateTo(csv.GetString(9));
		SetOriginalGroupNo(csv.GetInt(10));
		SetEditedStatus(csv.GetInt(11));
		SetEditedDateFrom(csv.GetString(12));
		SetEditedDateTo(csv.GetString(13));
		SetEditedGroupNo(csv.GetInt(14));
		bResult = TRUE;
	}

	return bResult;
}

//******************************************************************************

CString CSmartPayGroupShiftLogLine::GetActionSourceString()
{
	CString strResult = "";

	switch (m_nActionSource)
	{
	case 0:
		strResult = "mgr";
		break;

	case 1:
		strResult = "serv";
		break;

	case 2:
		strResult = "bgnd";
		break;

	case 3:
		strResult = "edit-sft";
		break;

	case 4:
		strResult = "edit-acc";
		break;

	default:
		strResult = "???";
		break;
	}

	return strResult;
}

//******************************************************************************

CString CSmartPayGroupShiftLogLine::GetStatusString(int nStatus)
{
	CString strResult = "";

	switch (nStatus)
	{
	case 0:
		strResult = "None";
		break;

	case 1:
		strResult = "Current";
		break;

	case 2:
		strResult = "Future";
		break;

	default:
		strResult = "???";
		break;
	}

	return strResult;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

CSmartPayGroupShiftFile::CSmartPayGroupShiftFile ( const char* szFilename )
{
	m_strFilename = szFilename;
	m_bLogFileOpen = FALSE;
}

//******************************************************************************

CSmartPayGroupShiftFile::~CSmartPayGroupShiftFile()
{
	if ( TRUE == m_bLogFileOpen)
	{
		Close();
	}
}

//******************************************************************************

bool CSmartPayGroupShiftFile::Open()
{
	m_bLogFileOpen = m_fileIn1.Open(m_strFilename, "rb");
	return m_bLogFileOpen;
}

//******************************************************************************

void CSmartPayGroupShiftFile::Close()
{
	if ( TRUE == m_bLogFileOpen )
	{
		m_fileIn1.Close();
		m_bLogFileOpen = FALSE;
	}
}

//******************************************************************************

bool CSmartPayGroupShiftFile::ReadLine(CSmartPayGroupShiftLogLine& line)
{
	bool bResult = FALSE;

	if (TRUE == m_bLogFileOpen)
	{
		CString str = "";
		if (m_fileIn1.ReadString(str) == TRUE)
		{
			bResult = line.Read(str);
		}
	}

	return bResult;
}

//******************************************************************************
//******************************************************************************
// Jump into file before start of given date

void CSmartPayGroupShiftFile::JumpIn ( const char* szDate )
{
	CSSDate date1 ( szDate );

	int nStartFrom	= 0L;
	int nOffset		= 0L;
	int nLowest		= 0L;
	int nHighest	= m_fileIn1.GetLength();
 
	int nAdjustment = 1000;
	
	CSmartPayGroupShiftLogLine LogLine;
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

		if (ReadLine(LogLine) == TRUE)					// read next audit line start
		{
			CString strDate = LogLine.GetActionDate();

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

//******************************************************************************

int CSmartPayGroupShiftFile::GetFileSize()
{ 
	return m_fileIn1.GetLength();
}

//******************************************************************************

int CSmartPayGroupShiftFile::GetFilePos()
{
	return m_fileIn1.Tell();
}

//******************************************************************************

void CSmartPayGroupShiftFile::SetFilePos(int nPos)
{ 
	m_fileIn1.Seek((long)nPos, SEEK_SET);
}

//******************************************************************************

int CSmartPayGroupShiftFile::GetPercentPosition()
{ 
	return m_fileIn1.GetPercentPosition();
}

//******************************************************************************
