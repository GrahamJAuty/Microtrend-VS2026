//**********************************************************************
#include "..\SmartPay4Shared\ServerData.h"
//**********************************************************************
#include "BromComData.h"
//**********************************************************************

CBromComData::CBromComData()
{
	CString strPath = System.GetWebPaymentPath("", nWEBPAYMENT_BROMCOM);	// creates folder if not found
	if (::ExistSubdirectory(strPath) == FALSE)
	{
		::MakeSubdirectory(strPath);
	}

	m_strFilename = System.GetWebPaymentPath("BromCom.dat", nWEBPAYMENT_BROMCOM);
}

//**********************************************************************

void CBromComData::Read()
{
	CSSIniFile file(SS_INI_AES);
	if (file.Read(m_strFilename) == TRUE)
	{
		m_bKeepXmlLogin = file.GetBool(BromComDataFields::KeepXmlLogin, m_bKeepXmlLogin);
		m_bKeepXmlFiles = file.GetBool(BromComDataFields::KeepXmlFiles, m_bKeepXmlFiles);
		m_strSchoolID = file.GetString(BromComDataFields::SchoolID, "");
		m_strUsername = file.GetString(BromComDataFields::Username, "");
		m_strPassword = file.GetString(BromComDataFields::Password, "");
		m_strLastDate = file.GetString(BromComDataFields::LastDate, "20220101");
		m_strLastTime = file.GetString(BromComDataFields::LastTime, "000000");
		m_nLastOrderID = file.GetInt(BromComDataFields::LastOrderID, 0);
		m_strLastOrderSQLDateTime = file.GetString(BromComDataFields::LastOrderSQLDateTime, "");
		SetSQLFilterAdjustMinutes(file.GetInt(BromComDataFields::SQLFilterAdjustMinutes, 0));
		SetPurse1OnlyBalanceFlag(file.GetBool(BromComDataFields::Purse1OnlyBalance, FALSE));
		m_strPurse2RefreshText = file.GetString(BromComDataFields::Purse2RefreshText, "");
	}
}

//**********************************************************************

void CBromComData::Save()
{
	CSSIniFile file(SS_INI_AES);

	file.Set(BromComDataFields::KeepXmlLogin, m_bKeepXmlLogin);
	file.Set(BromComDataFields::KeepXmlFiles, m_bKeepXmlFiles);
	file.Set(BromComDataFields::SchoolID, m_strSchoolID);
	file.Set(BromComDataFields::Username, m_strUsername);
	file.Set(BromComDataFields::Password, m_strPassword);
	file.Set(BromComDataFields::LastDate, m_strLastDate);
	file.Set(BromComDataFields::LastTime, m_strLastTime);
	file.Set(BromComDataFields::LastOrderID, m_nLastOrderID);
	file.Set(BromComDataFields::LastOrderSQLDateTime, m_strLastOrderSQLDateTime);
	file.Set(BromComDataFields::SQLFilterAdjustMinutes, m_nSQLFilterAdjustMinutes);
	file.Set(BromComDataFields::Purse1OnlyBalance, GetPurse1OnlyBalanceFlag());
	file.Set(BromComDataFields::Purse2RefreshText, m_strPurse2RefreshText);

	file.Write(m_strFilename);
}

//**********************************************************************

CString CBromComData::GetSchoolIDForConnection() const
{
	if (Server.GetBromComTestModeFlag() == FALSE)
	{
		return m_strSchoolID;
	}
	else
	{
		return "20001";
	}
}

//**********************************************************************

CString CBromComData::GetUsernameForConnection() const
{
	if (Server.GetBromComTestModeFlag() == FALSE)
	{
		return m_strUsername;
	}
	else
	{
		return "SPOS";
	}
}

//**********************************************************************

CString CBromComData::GetPasswordForConnection() const
{
	if (Server.GetBromComTestModeFlag() == FALSE)
	{
		return m_strPassword;
	}
	else
	{
		return "tS85'm7BqW9U-7qd";
	}
}

//**********************************************************************

bool CBromComData::IsSet()
{
	if (m_strSchoolID == "")
	{
		return FALSE;
	}

	if (m_strUsername == "")
	{
		return FALSE;
	}

	if (m_strPassword == "")
	{
		return FALSE;
	}

	return TRUE;
}

//**********************************************************************

CString CBromComData::GetXmlLoginPath ( const char* szFilename )
{
	CString strPathname = System.GetWebPaymentPath ( szFilename, nWEBPAYMENT_BROMCOM );
	return strPathname;	
}

//**********************************************************************

CString CBromComData::GetSQLFilterDateTime()
{
	COleDateTime dateFilter = COleDateTime::GetCurrentTime();

	{
		if (m_strLastDate.GetLength() == 8)
		{
			COleDateTime dateTry = COleDateTime(atoi(m_strLastDate.Left(4)), atoi(m_strLastDate.Mid(4, 2)), atoi(m_strLastDate.Right(2)), 0, 0, 0);
			if (dateTry.m_status == COleDateTime::valid)
			{
				dateFilter = dateTry;
			}
		}
	}

	{
		if (m_strLastTime.GetLength() == 6)
		{
			COleDateTime dateTry = COleDateTime(dateFilter.GetYear(), dateFilter.GetMonth(), dateFilter.GetDay(), atoi(m_strLastTime.Left(2)), atoi(m_strLastTime.Mid(2, 2)), atoi(m_strLastTime.Right(2)));
			if (dateTry.m_status == COleDateTime::valid)
			{
				dateFilter = dateTry;
			}
		}
	}

	if ( (m_nSQLFilterAdjustMinutes > 0) && (m_nSQLFilterAdjustMinutes <= 9999) )
	{
		dateFilter -= COleDateTimeSpan(0, 0, m_nSQLFilterAdjustMinutes, 0);
	}

	/*
	//REMOVED BST LOGIC FOR NOW
	//MAY NEED TO RE-ADD THIS WHEN BROMCOM CONFIRM FIX FOR THEIR TIMEZONE HANDLING

	int nYear = dateFilter.GetYear();
	int nMonth = dateFilter.GetMonth();
	int nDay = dateFilter.GetDay();
	
	int nLastMarchSunday = 32 - ( COleDateTime(nYear, 3, 31, 0, 0, 0).GetDayOfWeek() );
	int nLastOctoberSunday = 32 - (COleDateTime(nYear, 10, 31, 0, 0, 0).GetDayOfWeek());
	
	//IF THE TIME IS BEFORE 3AM ON THE DAY WHEN BST STARTS OR ENDS, THEN SET THE TIME
	//TO 12:59AM TO AVOID AMBIGUITY. ANY TRANSACTIONS THAT ARE IMPORTED AGAIN AS A 
	//RESULT OF THIS WILL BE FILTERED OUT BY THE DUPLICATION CHECKER. 
	if ( ( (3 == nMonth) && (nDay == nLastMarchSunday )) || 
		 (( 10 == nMonth) && (nDay == nLastOctoberSunday )) )
	{
		if (dateFilter.GetHour() < 3)
		{
			dateFilter = COleDateTime(nYear, nMonth, nDay, 12, 59, 0); 
		}
	}

	COleDateTime bstStart(nYear, 3, nLastMarchSunday, 1, 0, 0); // 1am
	COleDateTime bstEnd(nYear, 10, nLastOctoberSunday, 2, 0, 0); // 2am

	bool isBST = ((dateFilter >= bstStart) && (dateFilter < bstEnd));

	if (TRUE == isBST)
	{
		dateFilter -= COleDateTimeSpan(0, 1, 0, 0);
	}
	*/

	CString strResult = "";
	strResult.Format("%04d-%02d-%02dT%02d:%02d:%02dZ",
		dateFilter.GetYear(),
		dateFilter.GetMonth(),
		dateFilter.GetDay(),
		dateFilter.GetHour(),
		dateFilter.GetMinute(),
		dateFilter.GetSecond());

	return strResult;
}

//**********************************************************************

void CBromComData::SetSQLFilterAdjustMinutes(int nMinutes)
{
	if ( ( nMinutes >= 0 ) && ( nMinutes <= 9999 ) )
	{
		m_nSQLFilterAdjustMinutes = nMinutes;
	}
}

//**********************************************************************
