//$$******************************************************************
#include "..\SPosLoyalty4Shared\AccountFieldValidator.h"
#include "..\SPosLoyalty4Shared\Consolidation.h"
#include "..\SPosLoyalty4Shared\ReportConsolidationArray.h"
//$$******************************************************************
#include "BatchPropertyPage1.h"
#include "LoyaltyManager.h"
//$$******************************************************************
#include "BatchUpdateHelpers.h"
//$$******************************************************************

CBatchUpdateHelpers::CBatchUpdateHelpers()
{
	m_bIncludeValid = FALSE;
	m_bIncludeHotlisted = FALSE;
	m_bIncludeExpired = FALSE;
	m_bIncludeInactive = FALSE;
	m_nCardRangeType = 0;
	m_nRangeFromCardNo = 1;
	m_nRangeToCardNo = 1;
	m_nGroupSel = 0;
	m_nSingleGroupNo = 0;
	m_nGenderSel = 0;
	m_strLabel = "";
	m_strComment = "";
	m_strAlertFilter = "";
	m_bBirthdayFilter = FALSE;
}

//$$******************************************************************

bool CBatchUpdateHelpers::HaveRecords()
{
	return (m_bIncludeValid || m_bIncludeHotlisted || m_bIncludeExpired || m_bIncludeInactive);
}

//$$******************************************************************

void CBatchUpdateHelpers::GetSQLFilters(CDbExportHandler& DbExportHandler)
{
	if (0 == m_nGroupSel)
	{
		DbExportHandler.SetGroupNo(0);
	}
	else
	{
		DbExportHandler.SetGroupNo(m_nSingleGroupNo);
	}

	switch (m_nGenderSel)
	{
	case 1:
	case 2:
	case 3:
		DbExportHandler.SetGenderOption(1);
		DbExportHandler.SetGenderCode(m_nGenderSel - 1);
		break;

	default:
		DbExportHandler.SetGenderOption(0);
		DbExportHandler.SetGenderCode(0);
		break;
	}
}

//$$******************************************************************

bool CBatchUpdateHelpers::NonSQLFilter( CSQLRowAccountFull& RowAccount)
{
	if (m_nCardRangeType == nBATCH_LIST)
	{
		return m_Picker.IsValid(RowAccount.GetUserIDString(), Filenames.GetReportCardListFilename(m_strLabel));
	}

	__int64 nCardNo = RowAccount.GetUserID();

	if (nCardNo < m_nRangeFromCardNo || nCardNo > m_nRangeToCardNo)
	{
		return FALSE;
	}

	if (TRUE == m_bBirthdayFilter)
	{
		COleDateTime oleDate;
		if (CAccountFieldValidator::StringToOleDate(RowAccount.GetDOB(), oleDate) == FALSE)
		{
			return FALSE;
		}

		int nMonthBeforeBirthday = oleDate.GetMonth() - 1;
		if (nMonthBeforeBirthday < 1)
		{
			nMonthBeforeBirthday = 12;
		}

		COleDateTime timeNow = COleDateTime::GetCurrentTime();

		if (timeNow.GetMonth() != nMonthBeforeBirthday)
		{
			return FALSE;
		}
	}

	bool bOK = FALSE;
	bool bIsHotlisted = RowAccount.IsCardHotlisted();
	bool bIsInactive = RowAccount.GetInactive();
	bool bIsExpired = RowAccount.HasCardExpired();

	if ((TRUE == m_bIncludeValid) && (FALSE == bIsHotlisted) && (FALSE == bIsExpired) && (FALSE == bIsInactive))
	{
		bOK = TRUE;
	}
	else if ((TRUE == m_bIncludeHotlisted) && (TRUE == bIsHotlisted))
	{
		bOK = TRUE;
	}

	if ((TRUE == m_bIncludeExpired) && (TRUE == bIsExpired))
	{
		bOK = TRUE;
	}

	if ((TRUE == m_bIncludeInactive) && (TRUE == bIsInactive))
	{
		bOK = TRUE;
	}

	// validate on alert
	if (bOK == TRUE)
	{
		bOK = CAlertText::ValidateRecord(RowAccount,m_strAlertFilter);
	}

	return bOK;
}

//$$******************************************************************

