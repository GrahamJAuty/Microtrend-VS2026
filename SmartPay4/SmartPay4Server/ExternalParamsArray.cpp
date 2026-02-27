//**********************************************************************
#include "ExternalCommandSigner.h"
//**********************************************************************
#include "ExternalParamsArray.h"
//**********************************************************************

CExternalParamInfo::CExternalParamInfo()
{
	m_strParam = "";
	m_bFound = FALSE;
}

//**********************************************************************
//**********************************************************************
//**********************************************************************

CExternalCommandInfo::CExternalCommandInfo()
{
	m_strCommand = "";
	m_nCommandType = 0;
}

//**********************************************************************
//**********************************************************************
//**********************************************************************

CExternalParamsArray::CExternalParamsArray()
{
	AddCommand("AUTHENTICATE", EXTCOMMAND_AUTHENTICATE);
	AddCommand("USER", EXTCOMMAND_USER);
	AddCommand("BALANCE", EXTCOMMAND_BALANCE);
	AddCommand("PAYMENT", EXTCOMMAND_PAYMENT);
	AddCommand("RESETPASSWORD", EXTCOMMAND_RESETPASSWORD);
	AddCommand("CREATEUSER", EXTCOMMAND_CREATEUSER);
	AddCommand("USEREXISTS", EXTCOMMAND_USEREXISTS);

	m_nParamIdxUserName = AddParam("EXTUSERNAME");
	m_nParamIdxPassword = AddParam("EXTPASSWORD");
	m_nParamIdxPasswordPT = AddParam("EXTPASSWORD#");
	m_nParamIdxUserID = AddParam("USERID");
	m_nParamIdxLoyaltyAmount = AddParam("LOYALTY");
	m_nParamIdxOtherAmount = AddParam("OTHER");
	m_nParamIdxForename = AddParam("FORENAME");
	m_nParamIdxSurname = AddParam("SURNAME");
	m_nParamIdxInfo3 = AddParam("INFO3");
	m_nParamIdxInfo4 = AddParam("INFO4");
	m_nParamIdxGender = AddParam("GENDER");
	m_nParamIdxGroup = AddParam("GROUP");
	m_nParamIdxDOB = AddParam("DOB");
	m_nParamIdxItems = AddParam("ITEMS");

	ResetParams();
}

//**********************************************************************

void CExternalParamsArray::ResetParams()
{
	m_strUserName = "";
	m_strPassword = "";
	m_strUserID = "";
	m_nLoyaltyAmount = 0;
	m_nOtherAmount = 0;
	m_strForename = "";
	m_strSurname = "";
	m_strInfo3 = "";
	m_strInfo4 = "";
	m_strGender = "";
	m_nGroup = 1;
	m_strDOB = "";
	m_strItems = "";
}

//**********************************************************************

void CExternalParamsArray::AddCommand(CString strCommand, int nCommandType)
{
	CExternalCommandInfo info;
	info.m_strCommand = strCommand;
	info.m_nCommandType = nCommandType;
	m_arrayCommands.Add(info);
}

//**********************************************************************

int CExternalParamsArray::AddParam( CString strParam )
{
	CExternalParamInfo info;
	info.m_strParam = strParam;
	info.m_bFound = FALSE;
	m_arrayParams.Add(info);
	
	return (m_arrayParams.GetSize() - 1);
}

//**********************************************************************

int CExternalParamsArray::Init(CString strCommand)
{
	m_arrayParamsWanted.RemoveAll();

	int nCommandType = 0;
	strCommand.MakeUpper();

	for (int n = 0; n < m_arrayCommands.GetSize(); n++)
	{
		if (strCommand == m_arrayCommands[n].m_strCommand)
		{
			nCommandType = m_arrayCommands[n].m_nCommandType;
			break;
		}
	}

	switch (nCommandType)
	{
	case EXTCOMMAND_AUTHENTICATE:
		m_arrayParamsWanted.Add(m_nParamIdxUserName);
		m_arrayParamsWanted.Add(m_nParamIdxPassword);
		m_arrayParamsWanted.Add(m_nParamIdxPasswordPT);
		break;

	case EXTCOMMAND_USER:
		m_arrayParamsWanted.Add(m_nParamIdxUserID);
		break;

	case EXTCOMMAND_BALANCE:
		m_arrayParamsWanted.Add(m_nParamIdxUserID);
		break;

	case EXTCOMMAND_PAYMENT:
		m_arrayParamsWanted.Add(m_nParamIdxUserID);
		m_arrayParamsWanted.Add(m_nParamIdxLoyaltyAmount);
		m_arrayParamsWanted.Add(m_nParamIdxOtherAmount);
		m_arrayParamsWanted.Add(m_nParamIdxItems);
		break;

	case EXTCOMMAND_RESETPASSWORD:
		m_arrayParamsWanted.Add(m_nParamIdxUserName);
		m_arrayParamsWanted.Add(m_nParamIdxPassword);
		m_arrayParamsWanted.Add(m_nParamIdxPasswordPT);
		break;

	case EXTCOMMAND_CREATEUSER:
		m_arrayParamsWanted.Add(m_nParamIdxUserName);
		m_arrayParamsWanted.Add(m_nParamIdxPassword);
		m_arrayParamsWanted.Add(m_nParamIdxPasswordPT);
		m_arrayParamsWanted.Add(m_nParamIdxForename);
		m_arrayParamsWanted.Add(m_nParamIdxSurname);
		m_arrayParamsWanted.Add(m_nParamIdxInfo3);
		m_arrayParamsWanted.Add(m_nParamIdxInfo4);
		m_arrayParamsWanted.Add(m_nParamIdxGender);
		m_arrayParamsWanted.Add(m_nParamIdxGroup);
		m_arrayParamsWanted.Add(m_nParamIdxDOB);
		break;

	case EXTCOMMAND_USEREXISTS:
		m_arrayParamsWanted.Add(m_nParamIdxUserName);
		break;
	}

	return nCommandType;
}

//**********************************************************************

void CExternalParamsArray::GetParams(CStringArray& arrayLines, CExternalReplyArray& arrayError)
{
	arrayError.RemoveAll();

	for (int n = 0; n < m_arrayParams.GetSize(); n++)
	{
		m_arrayParams[n].m_bFound = FALSE;
	}

	ResetParams();

	/****/

	int nParamPos = -1;
	
	int nLinePos = 1;
	while ( nLinePos < arrayLines.GetSize() )
	{
		CString strLine = arrayLines.GetAt(nLinePos++);

		if (nParamPos < 0)
		{
			strLine.MakeUpper();
			if (strLine == CExternalCommandSigner::GetDelimiter())
			{
				return;
			}

			for (int n = 0; n < m_arrayParamsWanted.GetSize(); n++)
			{
				int nPos = m_arrayParamsWanted.GetAt(n);
				if (strLine == m_arrayParams[nPos].m_strParam)
				{
					if (m_arrayParams[nPos].m_bFound == FALSE)
					{
						m_arrayParams[nPos].m_bFound = TRUE;
						nParamPos = nPos;
						break;
					}
					else
					{
						arrayError.CreateErrorReply("5", "Duplicate parameter", strLine);
						return;
					}
				}
			}

			if (nParamPos < 0)
			{
				arrayError.CreateErrorReply("6", "Unexpected parameter", strLine);
				return;
			}
		}
		else
		{
			if (nParamPos == m_nParamIdxUserName)
			{
				m_strUserName = strLine;
				m_strUserName.MakeLower();
			}
			else if (nParamPos == m_nParamIdxPassword)
			{
				m_strPassword = strLine;
			}
			else if (nParamPos == m_nParamIdxPasswordPT)
			{
				m_strPassword = CSSMD5::CalcMD5(strLine);
			}
			else if (nParamPos == m_nParamIdxUserID)
			{
				m_strUserID = strLine;
			}
			else if (nParamPos == m_nParamIdxLoyaltyAmount)
			{
				m_nLoyaltyAmount = atoi(strLine);
			}
			else if (nParamPos == m_nParamIdxOtherAmount)
			{
				m_nOtherAmount = atoi(strLine);
			}
			else if (nParamPos == m_nParamIdxForename)
			{
				m_strForename = strLine;
			}
			else if (nParamPos == m_nParamIdxSurname)
			{
				m_strSurname = strLine;
			}
			else if (nParamPos == m_nParamIdxInfo3)
			{
				m_strInfo3 = strLine;
			}
			else if (nParamPos == m_nParamIdxInfo4)
			{
				m_strInfo4 = strLine;
			}
			else if (nParamPos == m_nParamIdxGender)
			{
				m_strGender = strLine.Left(1);
				m_strGender.MakeUpper();
			}
			else if (nParamPos == m_nParamIdxGroup)
			{
				m_nGroup = atoi(strLine);
			}
			else if (nParamPos == m_nParamIdxDOB)
			{
				m_strDOB = strLine;
			}
			else if (nParamPos == m_nParamIdxItems)
			{
				m_strItems = strLine;
			}

			nParamPos = -1;
		}
	}
}

//**********************************************************************
