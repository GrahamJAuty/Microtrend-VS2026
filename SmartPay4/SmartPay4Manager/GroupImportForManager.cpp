//*****************************************************************************************
// Import group records 
//*****************************************************************************************
#include "resource.h"
//*****************************************************************************************
#include "..\SmartPay4Shared\GroupHeaderHelpers.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\WorkingDlg.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
#include "..\SmartPay4Shared\SQLTable_Group\SQLRepositoryGroup.h"
//*****************************************************************************************
#include "GroupImportForManager.h"
//**********************************************************************

CImportGroupForManager::CImportGroupForManager(const char* szFilename, CWnd* pParent)
{
	m_strFilename = szFilename;
	m_pParent = pParent;
	m_nIgnored = 0;
	m_nRead = 0;
	m_nUsed = 0;
	m_nInvalid = 0;
	m_nAdded = 0;
	
	m_nIndexGroupNo = -1;
	m_strTitle = "CSV Import";
}

/**********************************************************************/

CImportGroupForManager::~CImportGroupForManager()
{
}

/**********************************************************************/

bool CImportGroupForManager::IsValidFileForUpgrade()
{
	int nCount = ::GetFileLineCount(m_strFilename);
	if (nCount < 2)
	{
		return FALSE;
	}

	CSSFile file1;
	if (file1.Open(m_strFilename, "rb") == FALSE)
	{
		return FALSE;
	}

	CCSV csvHeader;
	bool bReply = file1.Read(csvHeader);					// read header line 
	file1.Close();

	if (bReply == FALSE)
	{
		return FALSE;
	}

	// scan header line
	m_nIndexGroupNo = -1;

	for (int n = 0; n < csvHeader.GetSize(); n++)
	{
		CString strField = csvHeader.GetString(n);

		if ( GroupHeaderHelpers.GetNumericField(strField) == GROUPFIELD_GROUPNO )
		{ 
			m_nIndexGroupNo = n;
		}
	}

	return (m_nIndexGroupNo != -1 );
}

//**********************************************************************
// must call IsValidFile before calling Import as that validates file for input

bool CImportGroupForManager::DirectImport(bool bSilent)
{
	CSSFile file;
	if (file.Open(m_strFilename, "rb") == FALSE)
	{
		return FALSE;
	}

	CCSV csvHeader;
	if (file.Read(csvHeader) == FALSE)					// read header line 
	{
		return FALSE;
	}

	CArray<int, int> arrayHeader;
	GroupHeaderHelpers.CreateNumericHeader(csvHeader, arrayHeader);

	CWorkingDlg dlgWorking("Importing Group Data", FALSE);
	dlgWorking.Create();
	
	CString strImportLine;
	while (file.ReadString(strImportLine) == TRUE)
	{
		++m_nRead;
		dlgWorking.SetCaption2PercentDone(file.GetPercentPosition(), FALSE);
		ImportLine(arrayHeader, strImportLine);
	}

	file.Close();
	
	return TRUE;
}

/**********************************************************************/

bool CImportGroupForManager::ImportLine(CArray<int, int>& arrayHeader, const char* szImportLine)
{
	CCSV csvData(szImportLine);

	int nGroupNo = csvData.GetInt(m_nIndexGroupNo);
	if ( ( nGroupNo >= Group::GroupNo.Min) && ( nGroupNo <= Group::GroupNo.Max))
	{
		CSQLRowGroupFull RowGroup;
		RowGroup.SetGroupNo(nGroupNo);

		CSQLRepositoryGroup RepoGroup;
		if (RepoGroup.SelectRow(RowGroup, NULL).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			if (RepoGroup.InsertRow(RowGroup, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
			{
				++m_nAdded;
			}
		}

		++m_nUsed;
		ImportFields(arrayHeader, &csvData, RowGroup);
	}
	else
	{
		++m_nInvalid;
	}

	return TRUE;
}

//*****************************************************************************************

void CImportGroupForManager::ImportFields(CArray<int, int>& arrayHeader, CCSV* pCsvData, CSQLRowGroupFull& RowGroup)
{
	for (int i = 0; i < arrayHeader.GetSize(); i++)
	{
		CString strField = pCsvData->GetString(i);
		if (strField == "")
		{
			continue;
		}

		int nField = pCsvData->GetInt(i);
		bool bField = pCsvData->GetBool(i);
		double dField = pCsvData->GetDouble(i);

		switch (arrayHeader.GetAt(i))
		{
			// already have cardno
		case GROUPFIELD_GROUPNO:
			break;

		case GROUPFIELD_GROUPNAME:
			RowGroup.SetGroupName(strField);
			break;

		case GROUPFIELD_USERTYPE:
			RowGroup.SetUserType(nField);
			break;

		case GROUPFIELD_SETNO:
			RowGroup.SetSetNo(nField);
			break;

		case GROUPFIELD_PRICELEVEL:
			RowGroup.SetPriceLevel(nField);
			break;

		case GROUPFIELD_VATSHIFT:
			RowGroup.SetVatShiftFlag(bField ? 1 : 0);
			break;

		case GROUPFIELD_REFRESH_TYPE:
			RowGroup.SetRefreshType(nField);
			break;

		case GROUPFIELD_REFRESH_FLAGS:
			RowGroup.SetRefreshFlags(nField);
			break;

		case GROUPFIELD_REFRESH_VALUE:
			RowGroup.SetRefreshValue(dField);
			break;

		case GROUPFIELD_REFRESH_APPEND:
			RowGroup.SetRefreshAppendFlag(bField ? 1 : 0);
			break;

		case GROUPFIELD_REFRESH_TYPE_PERIOD1:
			RowGroup.SetRefreshTypePeriod1(nField);
			break;

		case GROUPFIELD_REFRESH_TYPE_PERIOD2:
			RowGroup.SetRefreshTypePeriod2(nField);
			break;

		case GROUPFIELD_REFRESH_TYPE_PERIOD3:
			RowGroup.SetRefreshTypePeriod3(nField);
			break;

		case GROUPFIELD_REFRESH_TYPE_PERIOD4:
			RowGroup.SetRefreshTypePeriod4(nField);
			break;

		case GROUPFIELD_REFRESH_TYPE_PERIOD5:
			RowGroup.SetRefreshTypePeriod5(nField);
			break;

		case GROUPFIELD_REFRESH_TYPE_PERIOD6:
			RowGroup.SetRefreshTypePeriod6(nField);
			break;

		case GROUPFIELD_REFRESH_VALUE_PERIOD1:
			RowGroup.SetRefreshValuePeriod1(dField);
			break;

		case GROUPFIELD_REFRESH_VALUE_PERIOD2:
			RowGroup.SetRefreshValuePeriod2(dField);
			break;

		case GROUPFIELD_REFRESH_VALUE_PERIOD3:
			RowGroup.SetRefreshValuePeriod3(dField);
			break;

		case GROUPFIELD_REFRESH_VALUE_PERIOD4:
			RowGroup.SetRefreshValuePeriod4(dField);
			break;

		case GROUPFIELD_REFRESH_VALUE_PERIOD5:
			RowGroup.SetRefreshValuePeriod5(dField);
			break;

		case GROUPFIELD_REFRESH_VALUE_PERIOD6:
			RowGroup.SetRefreshValuePeriod6(dField);
			break;

		case GROUPFIELD_PURSE1_ALLOW:
			RowGroup.SetPurse1Allowed(nField);
			break;

		case GROUPFIELD_PURSE2_ALLOW:
			RowGroup.SetPurse2Allowed(nField);
			break;

		case GROUPFIELD_PURSE3_ALLOW:
			RowGroup.SetPurse3Allowed(nField);
			break;

		case GROUPFIELD_MAXSPEND_TYPE:
			RowGroup.SetMaxSpendType(nField);
			break;

		case GROUPFIELD_MAXSPEND_PURSE1_PERIOD1:
			RowGroup.SetMaxSpendPurse1Period1(dField);
			break;

		case GROUPFIELD_MAXSPEND_PURSE1_PERIOD2:
			RowGroup.SetMaxSpendPurse1Period2(dField);
			break;

		case GROUPFIELD_MAXSPEND_PURSE1_PERIOD3:
			RowGroup.SetMaxSpendPurse1Period3(dField);
			break;

		case GROUPFIELD_MAXSPEND_PURSE1_PERIOD4:
			RowGroup.SetMaxSpendPurse1Period4(dField);
			break;

		case GROUPFIELD_MAXSPEND_PURSE1_PERIOD5:
			RowGroup.SetMaxSpendPurse1Period5(dField);
			break;

		case GROUPFIELD_MAXSPEND_PURSE1_PERIOD6:
			RowGroup.SetMaxSpendPurse1Period6(dField);
			break;

		case GROUPFIELD_MAXSPEND_PURSE3_PERIOD1:
			RowGroup.SetMaxSpendPurse3Period1(dField);
			break;

		case GROUPFIELD_MAXSPEND_PURSE3_PERIOD2:
			RowGroup.SetMaxSpendPurse3Period2(dField);
			break;

		case GROUPFIELD_MAXSPEND_PURSE3_PERIOD3:
			RowGroup.SetMaxSpendPurse3Period3(dField);
			break;

		case GROUPFIELD_MAXSPEND_PURSE3_PERIOD4:
			RowGroup.SetMaxSpendPurse3Period4(dField);
			break;

		case GROUPFIELD_MAXSPEND_PURSE3_PERIOD5:
			RowGroup.SetMaxSpendPurse3Period5(dField);
			break;

		case GROUPFIELD_MAXSPEND_PURSE3_PERIOD6:
			RowGroup.SetMaxSpendPurse3Period6(dField);
			break;

		case GROUPFIELD_MAXOVERDRAFT_PURSE1_PERIOD1:
			RowGroup.SetMaxOverdraftPurse1Period1(dField);
			break;

		case GROUPFIELD_MAXOVERDRAFT_PURSE1_PERIOD2:
			RowGroup.SetMaxOverdraftPurse1Period2(dField);
			break;

		case GROUPFIELD_MAXOVERDRAFT_PURSE1_PERIOD3:
			RowGroup.SetMaxOverdraftPurse1Period3(dField);
			break;

		case GROUPFIELD_MAXOVERDRAFT_PURSE1_PERIOD4:
			RowGroup.SetMaxOverdraftPurse1Period4(dField);
			break;

		case GROUPFIELD_MAXOVERDRAFT_PURSE1_PERIOD5:
			RowGroup.SetMaxOverdraftPurse1Period5(dField);
			break;

		case GROUPFIELD_MAXOVERDRAFT_PURSE1_PERIOD6:
			RowGroup.SetMaxOverdraftPurse1Period6(dField);
			break;

		case GROUPFIELD_MAXOVERDRAFT_PURSE3_PERIOD1:
			RowGroup.SetMaxOverdraftPurse3Period1(dField);
			break;

		case GROUPFIELD_MAXOVERDRAFT_PURSE3_PERIOD2:
			RowGroup.SetMaxOverdraftPurse3Period2(dField);
			break;

		case GROUPFIELD_MAXOVERDRAFT_PURSE3_PERIOD3:
			RowGroup.SetMaxOverdraftPurse3Period3(dField);
			break;

		case GROUPFIELD_MAXOVERDRAFT_PURSE3_PERIOD4:
			RowGroup.SetMaxOverdraftPurse3Period4(dField);
			break;

		case GROUPFIELD_MAXOVERDRAFT_PURSE3_PERIOD5:
			RowGroup.SetMaxOverdraftPurse3Period5(dField);
			break;

		case GROUPFIELD_MAXOVERDRAFT_PURSE3_PERIOD6:
			RowGroup.SetMaxOverdraftPurse3Period6(dField);
			break;

		case GROUPFIELD_DEPTINHIBIT_PURSE1:
			RowGroup.SetDeptInhibitPurse1(strField);
			break;

		case GROUPFIELD_DEPTINHIBIT_PURSE2:
			RowGroup.SetDeptInhibitPurse2(strField);
			break;

		case GROUPFIELD_DEPTINHIBIT_PURSE3:
			RowGroup.SetDeptInhibitPurse3(strField);
			break;

		case GROUPFIELD_DEPTINHIBIT_CASH:
			RowGroup.SetDeptInhibitCash(strField);
			break;

		case GROUPFIELD_LOWBALANCE_PURSE1:
			RowGroup.SetLowBalancePurse1(dField);
			break;

		case GROUPFIELD_LOWBALANCE_PURSE2:
			RowGroup.SetLowBalancePurse2(dField);
			break;

		case GROUPFIELD_LOWBALANCE_PURSE3:
			RowGroup.SetLowBalancePurse3(dField);
			break;

		case GROUPFIELD_POINTS_ACHIEVEMENT:
			RowGroup.SetPointsAchievement(nField);
			break;

		case GROUPFIELD_REVALUE_FLAG:
			RowGroup.SetFileRevalueFlag(bField);
			break;

		case GROUPFIELD_REVALUE_DISABLE:
			RowGroup.SetRevalueDisable(bField);
			break;

		case GROUPFIELD_REVALUE_TYPE:
			RowGroup.SetRevalueType(nField);
			break;

		case GROUPFIELD_REVALUE_TRIP1:
			RowGroup.SetRevalueTrip1(dField);
			break;

		case GROUPFIELD_REVALUE_TRIP2:
			RowGroup.SetRevalueTrip2(dField);
			break;

		case GROUPFIELD_REVALUE_POINTS:
			RowGroup.SetRevaluePoints(nField);
			break;

		case GROUPFIELD_REVALUE_BONUS:
			RowGroup.SetRevalueBonus(dField);
			break;

		case GROUPFIELD_REWARD_FLAG:
			RowGroup.SetFileRewardFlag(bField);
			break;

		case GROUPFIELD_REWARD_DISABLE:
			RowGroup.SetRewardDisable(bField);
			break;

		case GROUPFIELD_REWARD_TYPE:
			RowGroup.SetRewardType(nField);
			break;

		case GROUPFIELD_REWARD_TRIP:
			RowGroup.SetRewardTrip(dField);
			break;

		case GROUPFIELD_REWARD_POINTS:
			RowGroup.SetRewardPoints(nField);
			break;

		case GROUPFIELD_REWARD_BONUS:
			RowGroup.SetRewardBonus(dField);
			break;

		case GROUPFIELD_REWARD_LIMIT:
			RowGroup.SetRewardLimit(dField);
			break;

		case GROUPFIELD_REWARD_TRIGGER:
			RowGroup.SetRewardTrigger(dField);
			break;

		case GROUPFIELD_REDEEM_FLAG:
			RowGroup.SetFileRedeemFlag(bField);
			break;

		case GROUPFIELD_REDEEM_DISABLE:
			RowGroup.SetRedeemDisable(dField);
			break;

		case GROUPFIELD_REDEEM_TRIP:
			RowGroup.SetRedeemTrip(nField);
			break;

		case GROUPFIELD_REDEEM_VALUE:
			RowGroup.SetRedeemValue(dField);
			break;

		case GROUPFIELD_REDEEM_PURSE:
			RowGroup.SetRedeemPurse(nField);
			break;
		}
	}

	CSQLRepositoryGroup RepoGroup;
	RepoGroup.UpdateRow(RowGroup, NULL);
}

//******************************************************************

