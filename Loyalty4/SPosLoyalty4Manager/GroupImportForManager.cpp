//*****************************************************************************************
// Import group records 
//*****************************************************************************************
#include "resource.h"
//*****************************************************************************************
#include "..\SPosLoyalty4Shared\GroupHeaderHelpers.h"
#include "..\SPosLoyalty4Shared\SQLDefines.h"
#include "..\SPosLoyalty4Shared\SQLNamespaces.h"
#include "..\SPosLoyalty4Shared\SQLTable_Group\SQLRepositoryGroup.h"
#include "..\SPosLoyalty4Shared\SQLTable_GroupTopupBonus\SQLRepositoryGroupTopupBonus.h"
//*****************************************************************************************
#include "WorkingDlg.h"
//**********************************************************************
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

bool CImportGroupForManager::DirectImport(int nImportMethod, bool bSilent)
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
	
	::SendToRecycleBin(Filenames.GetDBImportExceptionsFilename());

	CString strImportLine;
	while (file.ReadString(strImportLine) == TRUE)
	{
		++m_nRead;
		dlgWorking.SetCaption2PercentDone(file.GetPercentPosition(), FALSE);
		ImportLine(nImportMethod, arrayHeader, strImportLine);
	}

	file.Close();
	
	return TRUE;
}

/**********************************************************************/

bool CImportGroupForManager::ImportLine(int nImportMethod, CArray<int, int>& arrayHeader, const char* szImportLine)
{
	CCSV csvData(szImportLine);

	int nGroupNo = csvData.GetInt(m_nIndexGroupNo);
	if ( ( nGroupNo >= Group::GroupNo.Min) && ( nGroupNo <= Group::GroupNo.Max))
	{
		CSQLRowGroup RowGroup;
		RowGroup.SetGroupNo(nGroupNo);

		CSQLRepositoryGroup repoGroup;
		bool bExists = (repoGroup.SelectRow(RowGroup, NULL).GetSQLError() == SQLCRUD_ERR_NONE);
		
		if (bExists == FALSE)
		{
			if (repoGroup.InsertRow(RowGroup, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
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
// CAUTION: Any fields change in here MUST also be added to DatabaseImport in LoyaltyServer 
//*****************************************************************************************

void CImportGroupForManager::ImportFields ( CArray<int,int>& arrayHeader, CCSV* pCsvData, CSQLRowGroup& RowGroup )
{
	CSQLRowGroupTopupBonus RowTopupBonus1;
	CSQLRowGroupTopupBonus RowTopupBonus2;
	RowTopupBonus1.SetGroupNo(RowGroup.GetGroupNo());
	RowTopupBonus1.SetTopupNo(1);
	RowTopupBonus1.SetPurseNo(1);
	RowTopupBonus1.SetBonusFlag(FALSE);
	RowTopupBonus2.SetGroupNo(RowGroup.GetGroupNo());
	RowTopupBonus2.SetTopupNo(2);
	RowTopupBonus2.SetPurseNo(1);
	RowTopupBonus2.SetBonusFlag(FALSE);

	for (int i = 0; i < arrayHeader.GetSize(); i++)
	{
		CString strField = pCsvData->GetString(i);
		if (strField == "")
		{
			continue;
		}

		int nField = pCsvData->GetInt(i);
		double dField = pCsvData->GetDouble(i);
		bool bField = pCsvData->GetBool(i);

		switch (arrayHeader.GetAt(i))
		{
		// already have cardno
		case GROUPFIELD_GROUPNO:
			break;

		case GROUPFIELD_GROUPNAME:
			RowGroup.SetGroupName(strField);
			break;

		case GROUPFIELD_SCHEMENO:
			RowGroup.SetSchemeNo(nField);
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
			RowGroup.SetRefreshAppend(nField);
			break;

		case GROUPFIELD_PURSE1_ALLOW:
			RowGroup.SetPurse1Allowed(nField);
			break;

		case GROUPFIELD_PURSE2_ALLOW:
			RowGroup.SetPurse2Allowed(nField);
			break;

		case GROUPFIELD_NOPLUBONUS:
			RowGroup.SetNoPluBonusFlag(bField);
			break;

		case GROUPFIELD_NODEPTBONUS:
			RowGroup.SetNoDeptBonusFlag(bField);
			break;

		case GROUPFIELD_MAXSPEND_TYPE:
			RowGroup.SetMaxSpendType(nField);
			break;

		case GROUPFIELD_MAXSPEND_PERIOD1:
			RowGroup.SetMaxSpendPeriod1(dField);
			break;

		case GROUPFIELD_MAXSPEND_PERIOD2:
			RowGroup.SetMaxSpendPeriod2(dField);
			break;

		case GROUPFIELD_MAXSPEND_PERIOD3:
			RowGroup.SetMaxSpendPeriod3(dField);
			break;

		case GROUPFIELD_MAXSPEND_PERIOD4:
			RowGroup.SetMaxSpendPeriod4(dField);
			break;

		case GROUPFIELD_MAXSPEND_PERIOD5:
			RowGroup.SetMaxSpendPeriod5(dField);
			break;

		case GROUPFIELD_MAXSPEND_PERIOD6:
			RowGroup.SetMaxSpendPeriod6(dField);
			break;

		case GROUPFIELD_MAXOVERDRAFT_PERIOD1:
			RowGroup.SetMaxOverdraftPeriod1(dField);
			break;

		case GROUPFIELD_MAXOVERDRAFT_PERIOD2:
			RowGroup.SetMaxOverdraftPeriod2(dField);
			break;

		case GROUPFIELD_MAXOVERDRAFT_PERIOD3:
			RowGroup.SetMaxOverdraftPeriod3(dField);
			break;

		case GROUPFIELD_MAXOVERDRAFT_PERIOD4:
			RowGroup.SetMaxOverdraftPeriod4(dField);
			break;

		case GROUPFIELD_MAXOVERDRAFT_PERIOD5:
			RowGroup.SetMaxOverdraftPeriod5(dField);
			break;

		case GROUPFIELD_MAXOVERDRAFT_PERIOD6:
			RowGroup.SetMaxOverdraftPeriod6(dField);
			break;

		case GROUPFIELD_REVALUE_FLAG:
			RowTopupBonus1.SetBonusFlag(bField);
			RowTopupBonus2.SetBonusFlag(bField);
			break;

		case GROUPFIELD_REVALUE_DISABLE:
			RowTopupBonus1.SetBonusDisable(bField);
			RowTopupBonus2.SetBonusDisable(bField);
			break;

		case GROUPFIELD_REVALUE_TRIP1:
			RowTopupBonus1.SetBonusTrip(dField);
			break;

		case GROUPFIELD_REVALUE_TRIP2:
			RowTopupBonus2.SetBonusTrip(dField);
			break;

		case GROUPFIELD_REVALUE_TYPE:
			RowTopupBonus1.SetBonusType(nField);
			RowTopupBonus2.SetBonusType(nField);
			break;

		case GROUPFIELD_REVALUE_POINTS:
			RowTopupBonus1.SetBonusPoints(nField);
			RowTopupBonus2.SetBonusPoints(nField);
			break;

		case GROUPFIELD_REVALUE_BONUS:
			RowTopupBonus1.SetBonusValue(dField);
			RowTopupBonus2.SetBonusValue(dField);
			break;

		case GROUPFIELD_REVALUE_PERCENT:
			RowTopupBonus1.SetBonusPercent(nField);
			RowTopupBonus2.SetBonusPercent(nField);
			break;

		case GROUPFIELD_REWARD_FLAG:
			RowGroup.SetRewardFlag(bField);
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
			RowGroup.SetRedeemFlag(bField);
			break;

		case GROUPFIELD_REDEEM_DISABLE:
			RowGroup.SetRedeemDisable(bField);
			break;

		case GROUPFIELD_REDEEM_TYPE:
			RowGroup.SetRedeemType(nField);
			break;

		case GROUPFIELD_REDEEM_TRIP:
			RowGroup.SetRedeemTrip(nField);
			break;

		case GROUPFIELD_REDEEM_VALUE:
			RowGroup.SetRedeemValue(dField);
			break;
		}
	}

	CSQLRepositoryGroup repoGroup;
	CSQLRepositoryGroupTopupBonus repoBonus;

	repoGroup.UpdateRow(RowGroup,NULL);
	repoBonus.UpsertRow(RowTopupBonus1, NULL);
	repoBonus.UpsertRow(RowTopupBonus2, NULL);
}

//******************************************************************

