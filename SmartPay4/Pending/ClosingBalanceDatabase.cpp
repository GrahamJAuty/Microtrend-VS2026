#include "ClosingBalanceDatabase.h"

//*****************************************************************************

static char* szHEADER			= "K1,D1,T1,D2,T2,A1,A2,A3,C1,C3";				// A1 = P1, A2 = p2  etc
static const char* szSORTFIELD	= "K1,ascending";								// K1 = date (yyyymmdd ), K2 = time
static int nRETRIES				= 10;

/******************************************************************************/
//	mode = DB_READONLY, DB_READWRITE

int CClosingBalanceDatabase::Open ( const char* szFilename, int nMode )
{
	int nRetries = 10;

	CreateSSDatabase ( szFilename, "", szHEADER, szSORTFIELD, nRETRIES );	// create db if not found
	Close();

	int nReply = DB_ERR_NONE;

	if (nMode == DB_READONLY)
	{
		nReply = OpenSSDatabaseReadOnly(szFilename, szSORTFIELD, nRETRIES);
	}
	else
	{
		nReply = OpenSSDatabaseReadWrite(szFilename, "", szSORTFIELD, nRETRIES);
		
		if (DB_ERR_NONE == nReply)
		{
			int nIndex = GetHeaderIndex(DBClosing::Purse1Credit.Label);			// check for latest header
			if (nIndex == -1)													// not found	
			{
				CSSStringDatabase::Close();										// close file
				nReply = CSSStringDatabase::OpenSSDatabaseReadWriteNewHeader(szFilename, "", szHEADER, szSORTFIELD, nRETRIES);// read and write and modify header (file must exist)
			}
		}
	}

	return nReply;
}	

//******************************************************************

CString CClosingBalanceDatabase::MakeKey ( CSSDate* pDate )
{
	CString strKey = "";
	strKey.Format ( "%4.4d%2.2d%2.2d", pDate->GetYear(), pDate->GetMonth(), pDate->GetDay() );
	return strKey;
}

//******************************************************************

CString CClosingBalanceDatabase::GetDate()
{
	CSSDate date ( CSSDatabase::GetString ( DBClosing::Date.Label ) );
	CString strDate = date.GetDate();
	return strDate;
}

//******************************************************************

CString CClosingBalanceDatabase::GetTime()
{
	CSSTime time ( CSSDatabase::GetString ( DBClosing::Time.Label ) );
	CString strTime = time.GetTime();
	return strTime;
}

//******************************************************************

void CClosingBalanceDatabase::SetTime ( const char* szCsvTime )
{
	CString strCsvTime = szCsvTime;

	if (strCsvTime == "")
	{
		CSSDatabase::Set(DBClosing::Time.Label, CSSTime::GetCurrentTime().GetCSVTime());
	}
	else
	{
		CSSDatabase::Set(DBClosing::Time.Label, szCsvTime);
	}
}

//******************************************************************

void CClosingBalanceDatabase::SetArchived()
{
	CSSDate date;
	date.SetCurrentDate();
	CSSDatabase::Set ( DBClosing::ArchiveDate.Label, date.GetCSVDate() );

	CSSTime time;
	time.SetCurrentTime();
	CSSDatabase::Set ( DBClosing::ArchiveTime.Label, time.GetCSVTime() );
}

//******************************************************************

bool CClosingBalanceDatabase::IsArchived()
{
	CString strDate = CSSDatabase::GetString ( DBClosing::ArchiveDate.Label );
	return ( strDate == "" ) ? FALSE : TRUE;
}

//******************************************************************

