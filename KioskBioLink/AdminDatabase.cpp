/**********************************************************************/
#include "SystemData.h"
/**********************************************************************/
#include "AdminDatabase.h"
/**********************************************************************/
static const char* szHEADER		= "RefNo,Date,Time";
static const char* szSORTFIELD	= "RefNo,ascending";
/**********************************************************************/

CAdminDatabase::CAdminDatabase()
{
	m_strFilename	= System.m_strAdminDatabaseFilename;
}

/******************************************************************************/
//	mode = DB_READONLY, DB_READWRITE

int CAdminDatabase::OpenForSharing(int nMode)
{
	int nRetries = 10;

	CreateSSDatabase(m_strFilename, "", szHEADER, szSORTFIELD, nRetries);	// create db if not found
	Close();

	if (nMode == DB_READONLY)
	{
		return OpenSSDatabaseReadOnly(m_strFilename, szSORTFIELD, nRetries);
	}

	return OpenSSDatabaseReadWrite(m_strFilename, "", szSORTFIELD, nRetries);
}

/**********************************************************************/

const char* CAdminDatabase::GetHeader()
{
	return szHEADER;
}

/**********************************************************************/

const char* CAdminDatabase::FormatKey(const char* szKey)
{
	m_strKey = szKey;
	AddLeading(m_strKey, AdminDB::UserID.Max, ' ');
	return m_strKey;
}

//*********************************************************************
//*** validation checking**********************************************
//*********************************************************************

void CAdminDatabase::SetDateField ( const char* szLabel, const char* szDate )
{
	bool bOk = TRUE;

	CString strDate = szDate;
	if ( strDate.GetLength() > 0 )
	{
		CSSDate date ( szDate );
		bOk = date.IsSet();
	}
	
	if (TRUE == bOk)
	{
		CSSDatabase::Set(szLabel, szDate);
	}
}

//*********************************************************************

void CAdminDatabase::SetTimeField(const char* szLabel, const char* szTime)
{
	bool bOk = TRUE;

	CString strTime = szTime;
	if (strTime.GetLength() > 0)
	{
		CSSTime time(szTime);
		bOk = time.IsSet();
	}

	if (TRUE == bOk)
	{
		CSSDatabase::Set(szLabel, szTime);
	}
}

/*********************************************************************/

void CAdminDatabase::SetField(const char* strLabel, const char* szText, int nMax)
{
	CString strText = szText;
	strText.TrimLeft(' ');							// loose leading spaces

	int nLen = strText.GetLength();

	if (nLen > nMax)
	{
		strText = strText.Left(nMax);
		nLen = nMax;
	}

	for (int i = 0; i < nLen; i++)
	{
		if (strText.GetAt(i) < ' ')			//	conver Ctrl chars to space
		{
			strText.SetAt(i, ' ');
		}
	}

	CSSDatabase::Set(strLabel, strText);
}

//*******************************************************************

void CAdminDatabase::Update(const char* szUserID)
{
	if (OpenForSharing(DB_READWRITE) == DB_ERR_NONE)
	{
		if (FindRecord(szUserID) == FALSE)
		{
			AddBlankRecord(szUserID);
			SetUserID(szUserID);
		}

		SetDateLastUsed(CSSDate::GetCurrentDate().GetDate());
		SetTimeLastUsed(CSSTime::GetCurrentTime().GetTime());

		Close();
	}
}

//*******************************************************************

bool CAdminDatabase::Exists(const char* szUserID)
{
	if (OpenForSharing(DB_READONLY) == DB_ERR_NONE)
	{
		return FindRecord(szUserID);
	}

	return FALSE;
}

//*******************************************************************
