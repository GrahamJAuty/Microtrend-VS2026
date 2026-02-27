//**********************************************************************
#include "..\SQLSchema\SQLNamespaces.h"
//**********************************************************************
#include "..\SQLDb.h"
#include "..\SQLDefines.h"
#include "..\SQLHelpers.h"
//**********************************************************************
#include "SQLRecordSetUserText.h"
//**********************************************************************
#include "SQLRepositoryUserText.h"
//**********************************************************************

CSQLRepositoryUserText::CSQLRepositoryUserText()
{
	m_strBioConsent = "Bio Consent";
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryUserText::SelectRow(CSQLRowUserText& RowUT, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetUserText RecordSet(pDatabase, 
			RSParams_UserText_NormalByFieldLabel{ RowUT.GetFieldLabel() });

		if (RecordSet.Open(CRecordset::forwardOnly, NULL, CRecordset::readOnly) == FALSE)
		{
			Result.SetSQLError(SQLCRUD_ERR_RESET);
		}
		else
		{
			if (RecordSet.IsEOF() == TRUE)
			{
				Result.SetSQLError(SQLCRUD_ERR_NOMATCH);
			}
			else
			{
				RecordSet.SaveToUserTextRow(RowUT);
				Result.SetSQLError(SQLCRUD_ERR_NONE);
			}

			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryUserText::UpdateRow(CSQLRowUserText& RowUT, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetUserText RecordSet(pDatabase,
			RSParams_UserText_NormalByFieldLabel{ RowUT.GetFieldLabel() });

		if (RecordSet.Open(CRecordset::dynaset, NULL, CRecordset::none) == FALSE)
		{
			Result.SetSQLError(SQLCRUD_ERR_RESET);
		}
		else
		{
			if (RecordSet.IsEOF() == TRUE)
			{
				Result.SetSQLError(SQLCRUD_ERR_NOMATCH);
			}
			else
			{
				RecordSet.Edit();
				RecordSet.LoadFromUserTextRow(RowUT);
				Result.SetSQLError(RecordSet.WrappedUpdate());
			}
			
			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryUserText::InsertRow(CSQLRowUserText& RowUT, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetUserText RecordSet(pDatabase,
			RSParams_UserText_NormalNoParams{});

		RecordSet.SetInsertFilterString();

		if (RecordSet.Open(CRecordset::dynaset, NULL, CRecordset::none) == FALSE)
		{
			Result.SetSQLError(SQLCRUD_ERR_RESET);
		}
		else
		{
			RecordSet.AddNew();
			RecordSet.LoadFromUserTextRow(RowUT);
			Result.SetSQLError(RecordSet.WrappedUpdate());
			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryUserText::UpsertRow(CSQLRowUserText& RowUT, CDatabase* pDatabase)
{
	CSQLResultInfo Result = RowExists(RowUT.GetFieldLabel(), pDatabase);

	if (Result.GetSQLError() == SQLCRUD_ERR_NONE)
	{
		if (Result.GetSQLResult() == 0 )
		{
			Result = InsertRow(RowUT, pDatabase);
		}
		else
		{
			Result = UpdateRow(RowUT, pDatabase);
		}
	}

	return Result;
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryUserText::DeleteRow(CSQLRowUserText& RowUT, CDatabase* pDatabase)
{
	auto operation = [&]() -> CSQLResultInfo
	{
		CSQLResultInfo Result;
		CSQLRecordSetUserText RecordSet(pDatabase,
			RSParams_UserText_NormalByFieldLabel{ RowUT.GetFieldLabel() });

		if (RecordSet.Open(CRecordset::dynaset, NULL, CRecordset::none) == FALSE)
		{
			Result.SetSQLError(SQLCRUD_ERR_RESET);
		}
		else
		{
			if (RecordSet.IsEOF() == FALSE)
			{
				RecordSet.Delete();
				Result.SetSQLError(SQLCRUD_ERR_NONE);
			}
			else
			{
				Result.SetSQLError(SQLCRUD_ERR_NOMATCH);
			}

			RecordSet.Close();
		}
		
		return Result;
	};
	
	return CSQLHelpers::ExecuteWithExceptionHandling(operation);
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryUserText::RowExists(CString strFieldLabel, CDatabase* pDatabase)
{
	CSQLRecordSetUserText RecordSet(pDatabase,
		RSParams_UserText_NormalByFieldLabel{ strFieldLabel }, TRUE);

	return RecordSet.CheckRowExists();
}

//**********************************************************************

void CSQLRepositoryUserText::CreateDefaultTexts(CDatabase* pDatabase)
{
	for (int n = 1; n <= 8; n++)
	{
		if (GetInfoText(n) != "")
		{
			return;
		}
	}

	SetUserText(Account::Info1.Label, "Info1", pDatabase);
	SetUserText(Account::Info2.Label, "Info2", pDatabase);
	SetUserText(Account::Info3.Label, "Year", pDatabase);
	SetUserText(Account::Info4.Label, "Class", pDatabase);
	SetUserText(Account::Info5.Label, "Info5", pDatabase);
	SetUserText(Account::Info6.Label, "AdNo", pDatabase);
	SetUserText(Account::Info7.Label, "UPN", pDatabase);
	SetUserText(Account::Info8.Label, "MIS_ID", pDatabase);
}

//**********************************************************************

CString CSQLRepositoryUserText::GetUserText(CString strLabel, CString strDefault)
{
	CString strResult = "";

	CSQLRowUserText RowUT;
	RowUT.SetFieldLabel(strLabel);
	
	if (SelectRow(RowUT, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
	{
		strResult = RowUT.GetUserText();
	}

	if ( strResult == "")
	{
		strResult = strDefault;
	}

	return strResult;
}

//**********************************************************************

CSQLResultInfo CSQLRepositoryUserText::SetUserText(CString strLabel, CString strText, CDatabase* pDatabase, bool bUseFlag)
{
	CSQLRowUserText RowUT;
	RowUT.SetFieldLabel(strLabel);
	RowUT.SetUserText(strText);
	RowUT.SetUseFlag(bUseFlag);
	return UpsertRow(RowUT, pDatabase);
}

//**********************************************************************

CString CSQLRepositoryUserText::GetTickText(int nTickNo)
{
	CString strText = "";

	switch (nTickNo)
	{
	case 1:
		strText = GetTick1Text();
		break;

	case 2:
		strText = GetTick2Text();
		break;

	case 3:
		strText = GetTick3Text();
		break;

	case 4:
		strText = GetTick4Text();
		break;

	}

	return strText;
}

//**********************************************************************

CString CSQLRepositoryUserText::GetTick1Text(CString strDefault)
{ 
	if (System.GetBioConsentTickNo() == 1)
	{
		return m_strBioConsent;
	}
	else
	{
		return GetUserText(Account::Tick1.Label, strDefault);
	}
}

//**********************************************************************

CString CSQLRepositoryUserText::GetTick2Text(CString strDefault)
{
	if (System.GetBioConsentTickNo() == 2)
	{
		return m_strBioConsent;
	}
	else
	{
		return GetUserText(Account::Tick2.Label, strDefault);
	}
}

//**********************************************************************

CString CSQLRepositoryUserText::GetTick3Text(CString strDefault)
{
	if (System.GetBioConsentTickNo() == 3)
	{
		return m_strBioConsent;
	}
	else
	{
		return GetUserText(Account::Tick3.Label, strDefault);
	}
}

//**********************************************************************

CString CSQLRepositoryUserText::GetTick4Text(CString strDefault)
{
	if (System.GetBioConsentTickNo() == 4)
	{
		return m_strBioConsent;
	}
	else
	{
		return GetUserText(Account::Tick4.Label, strDefault);
	}
}

//**********************************************************************

CString CSQLRepositoryUserText::GetInfoText(int nInfoNo)
{
	CString strText = "";

	switch (nInfoNo)
	{
	case 1:
		strText = GetInfo1Text();
		break;

	case 2:
		strText = GetInfo2Text();
		break;

	case 3:
		strText = GetInfo3Text();
		break;

	case 4:
		strText = GetInfo4Text();
		break;

	case 5:
		strText = GetInfo5Text();
		break;

	case 6:
		strText = GetInfo6Text();
		break;

	case 7:
		strText = GetInfo7Text();
		break;

	case 8:
		strText = GetInfo8Text();
		break;
	}

	return strText;
}

//**********************************************************************

CString CSQLRepositoryUserText::GetDisplayInfoText(int nInfoNo)
{
	CString strLabel = "";
	CString strResult = "";

	switch (nInfoNo)
	{
	case 1:
		strLabel = Account::Info1.Label;
		break;

	case 2:
		strLabel = Account::Info2.Label;
		break;

	case 3:
		strLabel = Account::Info3.Label;
		break;

	case 4:
		strLabel = Account::Info4.Label;
		break;

	case 5:
		strLabel = Account::Info5.Label;
		break;

	case 6:
		strLabel = Account::Info6.Label;
		break;

	case 7:
		strLabel = Account::Info7.Label;
		break;

	case 8:
		strLabel = Account::Info8.Label;
		break;
	}

	if (strLabel != "")
	{
		strResult = GetDisplayText(strLabel, GetInfoText(nInfoNo));
	}

	return strResult;
}

//**********************************************************************

CString CSQLRepositoryUserText::GetDisplayMemberText()
{
	CString strDisplayText = GetDisplayText(Account::MemberID.Label, GetMemberIDText());
	return strDisplayText;
}

//**********************************************************************

CString CSQLRepositoryUserText::GetDisplayExtRef1Text()
{
	CString strDisplayText = GetDisplayText(Account::ExternalRef1.Label, GetExtRef1Text());
	return strDisplayText;
}

//**********************************************************************

CString CSQLRepositoryUserText::GetDisplayExtRef2Text()
{
	CString strDisplayText = GetDisplayText(Account::ExternalRef2.Label, GetExtRef2Text());
	return strDisplayText;
}

//**********************************************************************

CString CSQLRepositoryUserText::GetDbDisplayText( CString strLabel)
{
	CString strText = "";

	if (strLabel == Account::Tick1.Label)
	{
		strText = GetTick1Text();
	}
	if (strLabel == Account::Tick2.Label)
	{
		strText = GetTick2Text();
	}
	if (strLabel == Account::Tick3.Label)
	{
		strText = GetTick3Text();
	}
	if (strLabel == Account::Tick4.Label)
	{
		strText = GetTick4Text();
	}
	else
	{
		strText = GetUserText(strLabel);
	}

	return GetDisplayText(strLabel, strText);
}

//**********************************************************************

CString CSQLRepositoryUserText::GetDisplayText(CString strLabel, CString strText)
{
	CString strDisplayText = strLabel;

	if (strText != "")
	{
		if (strDisplayText != strText)
		{
			CString strBuf = "";
			strBuf.Format("%s { %s }",
				(const char*)strDisplayText,
				(const char*)strText);

			strDisplayText = strBuf;
		}
	}

	return strDisplayText;
}

//**********************************************************************

bool CSQLRepositoryUserText::IsExtRef2Reqd()
{
	bool bResult = FALSE;

	CSQLRowUserText RowUT;
	RowUT.SetFieldLabel(Account::ExternalRef2.Label);
	
	if (SelectRow(RowUT, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
	{
		bResult = RowUT.GetUseFlag();
	}

	return bResult;
}

//**********************************************************************

CString CSQLRepositoryUserText::GetDbLabel(CString strDisplayText)
{
	CString strText = strDisplayText;

	int nIndex;
	if ((nIndex = strText.Find(" { ")) != -1)
	{
		strText.SetAt(nIndex, 0x00);
	}

	return strText;
}

//**********************************************************************
