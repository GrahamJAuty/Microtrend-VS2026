//*******************************************************************
#include "GlobalFunctions.h"
#include "SystemData.h"
//*******************************************************************
#include "UserIDRecord.h"
//*******************************************************************

//static const char* szERRORCODE_DBACCESS		= "ER=101";
//static const char* szERRORTEXT_DBACCESS		= "EM=Unable to access SmartPay Database";

//static const char* szERRORCODE_UNKNOWNID		= "ER=102";
//static const char* szERRORTEXT_UNKNOWNID		= "EM=Unknown User ID";

//static const char* szERRORCODE_UNKNOWNGROUP	= "ER=103";
//static const char* szERRORTEXT_UNKNOWNGROUP	= "EM=Unknown User Group";

//static const char* szERRORCODE_IDEXISTS		= "ER=104";
//static const char* szERRORTEXT_IDEXISTS		= "EM=User ID already exists";

//static const char* szERRORCODE_ADDRECORD		= "ER=105";
//static const char* szERRORTEXT_ADDRECORD		= "EM=Unable to add new record";

// warnings

//static const char* szERRORCODE_EXPIRED		= "ER=201";
//static const char* szERRORTEXT_EXPIRED		= "EM=User expired";

//static const char* szERRORCODE_ALERT			= "ER=202";
//static const char* szERRORTEXT_ALERT			= "EM=User Alert";

//static const char* szERRORCODE_INACTIVE		= "ER=203";
//static const char* szERRORTEXT_INACTIVE		= "EM=User set to Inactive";

//static const char* szERRORCODE_INACTIVE		= "ER=204";
//static const char* szERRORTEXT_INACTIVE		= "EM=Kiosk blocked for this account";

//*******************************************************************

CUserIDRecord::CUserIDRecord(void)
{
	Clear();
}

//*******************************************************************

void CUserIDRecord::Clear()
{
	m_strInternalError = "";
	m_strServerError = "";
	m_nMessageNo = nMESSAGE_NONE;
	m_bRecordExists = TRUE;
	/*****/
	m_strUserID = "";
	m_strUSID = "";
	m_strMFID = "";
	m_strMF2ID = "";
	m_nGroupNo = 0;
	m_strUserName = "";
	m_strForename = "";
	m_strSurname = "";
	m_strImage = "";
	m_dPurse1 = 0.0;
	m_dAvailableSpend = 0.0;
	m_dSpendLimit = 0.0;
	m_dPurse3 = 0.0;
	m_dAvailableSpend3 = 0.0;
	m_dSpendLimit3 = 0.0;
	m_nPointsBalance = 0;
	m_nPointsTD = 0;
	m_dateLastUsed.SetDate("");
	m_timeLastUsed.SetTime("");
}

//*******************************************************************

void CUserIDRecord::CopyFrom(CUserIDRecord& source)
{
	m_strInternalError = source.m_strInternalError;
	m_strServerError = source.m_strServerError;
	m_nMessageNo = source.m_nMessageNo;
	m_bRecordExists = source.m_bRecordExists;
	m_strUserID = source.m_strUserID;
	m_strUSID = source.m_strUSID;
	m_strMFID = source.m_strMFID;
	m_strMF2ID = source.m_strMF2ID;
	m_nGroupNo = source.m_nGroupNo;
	m_strUserName = source.m_strUserName;
	m_strForename = source.m_strForename;
	m_strSurname = source.m_strSurname;
	m_dPurse1 = source.m_dPurse1;
	m_dAvailableSpend = source.m_dAvailableSpend;
	m_dSpendLimit = source.m_dSpendLimit;
	m_dPurse3 = source.m_dPurse3;
	m_dAvailableSpend3 = source.m_dAvailableSpend3;
	m_dSpendLimit3 = source.m_dSpendLimit3;
	m_strImage = source.m_strImage;
	m_nPointsBalance = source.m_nPointsBalance;
	m_nPointsTD = source.m_nPointsTD;
	m_dateLastUsed = source.m_dateLastUsed;
	m_timeLastUsed = source.m_timeLastUsed;
}	

//*******************************************************************

const bool CUserIDRecord::MatchField(const CString& strField, const CString strPrefix, CString& strValue)
{
	bool bResult = FALSE;	
	strValue = "";

	if (strField.Left(strPrefix.GetLength()) == strPrefix) 
	{
		strValue = strField.Mid(strPrefix.GetLength());
		bResult = TRUE;
	}

	return bResult;
}

//*******************************************************************

bool CUserIDRecord::ExtractUserData ( const char* szLine )
{
	bool bReply = TRUE;
	bool bIgnoreErrorMessage = FALSE;

	Clear();

	CCSV csv ( szLine );

	for (int nIndex = 0; nIndex < csv.GetSize(); nIndex++)
	{
		CString strField = csv.GetString(nIndex);
		CString strValue = "";

		if (MatchField(strField, "ID=", strValue))
		{
			SetUserID(strValue);
		}
		else if (MatchField(strField, "USID=", strValue))
		{
			m_strUSID = strValue;
		}
		else if (MatchField(strField, "MFID=", strValue))
		{
			m_strMFID = strValue;
		}
		else if (MatchField(strField, "MF2ID=", strValue))
		{
			m_strMF2ID = strValue;
		}
		else if (MatchField(strField, "GR=", strValue))
		{
			m_nGroupNo = atoi(strValue);
		}
		else if (MatchField(strField, "N1=", strValue))
		{
			m_strUserName = strValue;
		}
		else if (MatchField(strField, "N2=", strValue))
		{
			m_strForename = strValue;
		}
		else if (MatchField(strField, "N3=", strValue))
		{
			m_strSurname = strValue;
		}

		// points

		else if (MatchField(strField, "PB=", strValue))
		{
			m_nPointsBalance = atoi(strValue);
		}

		else if (MatchField(strField, "PD=", strValue))
		{
			m_nPointsTD = atoi(strValue);
		}

		// purse1

		else if (MatchField(strField, "P1=", strValue))
		{
			m_dPurse1 = atof(strValue) / 100;
		}
		else if (MatchField(strField, "AS=", strValue))
		{
			m_dAvailableSpend = atof(strValue) / 100;
		}
		else if (MatchField(strField, "SL=", strValue))
		{
			m_dSpendLimit = atof(strValue) / 100;
		}

		// purse3

		else if (MatchField(strField, "P3=", strValue))
		{
			m_dPurse3 = atof(strValue) / 100;
		}
		else if (MatchField(strField, "AS3=", strValue))
		{
			m_dAvailableSpend3 = atof(strValue) / 100;
		}
		else if (MatchField(strField, "SL3=", strValue))
		{
			m_dSpendLimit3 = atof(strValue) / 100;
		}

		// other

		else if (MatchField(strField, "LUD=", strValue))
		{
			m_dateLastUsed.SetDate(strValue);
		}
		else if (MatchField(strField, "LUT=", strValue))
		{
			m_timeLastUsed.SetTime(strValue);
		}
		else if (MatchField(strField, "IM=", strValue))
		{
			m_strImage = strValue;
		}
		else if (MatchField(strField, "ER=1", strValue))
		{
			bReply = FALSE;
			m_nMessageNo = nMESSAGE_SERVERERROR;

			if (strValue == "02")
			{
				m_strInternalError = "No record";
				m_bRecordExists = FALSE;
			}
		}

		else if (MatchField(strField, "ER=2", strValue))
		{
			if (strValue == "01")					// Expired
			{
				bReply = FALSE;
				m_strInternalError = "Expired";
				m_nMessageNo = nMESSAGE_EXPIRED;
			}
			else if (strValue == "02")			// Alert message
			{
				if (System.GetDisableAlertsFlag() == TRUE)
				{
					bIgnoreErrorMessage = TRUE;
				}
				else
				{
					m_strInternalError = "Alert";
				}
			}
			else if (strValue == "03")
			{
				m_strInternalError = "Inactive";
				bReply = FALSE;
			}
			else if (strValue == "04")
			{
				m_strInternalError = "Kiosk Use blocked for this account";
				bReply = FALSE;
			}
		}
		else if (MatchField(strField, "EM=", strValue) && (FALSE == bIgnoreErrorMessage))		// error message
		{
			m_strServerError = strValue;
		}
	}

	return bReply;
}

//**************************************************************

CString CUserIDRecord::GetError()
{
	if (m_strServerError != "")
	{
		return m_strServerError;
	}

	if (m_strInternalError != "")
	{
		return m_strInternalError;
	}

	return "";
}

//*******************************************************************

CString CUserIDRecord::GetGroupNoString()
{
	CString strGroupNo = "";
	strGroupNo.Format("%2.2d", m_nGroupNo);
	return strGroupNo;
}

//*******************************************************************
