//*******************************************************************
//*** exact same file used by LoyaltyManager / LoyaltyServer ********
//*******************************************************************
#include "SQLTable_Group\\SQLRepositoryGroup.h"
//*******************************************************************
#include "SQLDefines.h"
//*******************************************************************
#include "LoyaltySubs.h"
//*******************************************************************
//*** Refresh Purse2 ( if reqd ) ************************************
//*******************************************************************

bool CLoyaltySubs::IsRefreshPurse2Reqd(CSQLRowAccountFull& RowAccount, CSQLRowGroup& RowGroup, CSSDate& dateReference)
{
	bool bGotGroup = FALSE;
	if (RowAccount.GetGroupNo() != RowGroup.GetGroupNo())
	{
		CSQLRepositoryGroup repoGroup;
		RowGroup.SetGroupNo(RowAccount.GetGroupNo());
		bGotGroup = (repoGroup.SelectRow(RowGroup, NULL).GetSQLError() == SQLCRUD_ERR_NONE);
	}
	else
	{
		bGotGroup = TRUE;
	}

	if (FALSE == bGotGroup)
	{
		return FALSE;
	}

	if (RowGroup.GetRefreshType() == nREFRESHTYPE_NORMAL)
	{
		return FALSE;
	}

	bool bRefreshReqd = FALSE;
	CSSDate dateRefreshed(RowAccount.GetPurse2RefreshedDate());

	switch (RowGroup.GetRefreshType())
	{
	case nREFRESHTYPE_DAILY:
		if (dateRefreshed != dateReference)
		{
			int nDayNo = dateReference.GetDayOfWeek();					// 1=sun, 2=mon, 0=not set
			bRefreshReqd = RowGroup.IsRefreshAllowed(nDayNo - 1);		// 0=sun, 1=mon etc..
		}
		break;

	case nREFRESHTYPE_WEEKLY:
	{
		CSSDate dateTo{};
		CSSDate dateFrom{};

		CSSDateRange date(RowGroup.GetRefreshWeeklyDayNo() + 1, 1, 1, 1);
		date.GetRange(szRANGE_THIS_WEEK, dateFrom, dateTo);

		if (dateRefreshed < dateFrom)
		{
			bRefreshReqd = TRUE;
		}
	}
	break;

	case nREFRESHTYPE_MONTHLY:

		bRefreshReqd = TRUE;

		if (dateRefreshed.GetYear() == dateReference.GetYear())		// see if already done this year				
		{
			if (dateRefreshed.GetMonth() == dateReference.GetMonth())	// see if already done this month
			{
				bRefreshReqd = FALSE;									// already done this month & year - no need to do.					
			}
		}
		break;

	case nREFRESHTYPE_YEARLY:
	{
		int nDay = 0;
		int nMonth = 0;
		CSSDate dateFrom{};

		RowGroup.GetRefreshYearDate(nDay, nMonth);
		int nYear = dateReference.GetYear();

		// This years renewal date
		CString strDate = "";
		strDate.Format("%2.2d%2.2d%d", nDay, nMonth, nYear);
		CSSDate dateRenewal(strDate);

		// see if this years renewal date has happened yet.
		if (dateRenewal > dateReference)
		{
			--nYear;
		}

		strDate.Format("%2.2d%2.2d%d", nDay, nMonth, nYear);
		dateFrom.SetDate(strDate);

		if (dateRefreshed < dateFrom)
		{
			bRefreshReqd = TRUE;
		}
	}
	break;
	}

	return bRefreshReqd;
}

//*******************************************************************

double CLoyaltySubs::GetPurse2NextSpendBalance(CSQLRowAccountFull& RowAccount, CSQLRowGroup& RowGroup, bool bForceLoadGroup)
{
	bool bUseRefreshValue = FALSE;

	if (System.GetPurse2PendingRefreshFlag() == TRUE)
	{
		bool bGotGroup = FALSE;
		if ( (RowGroup.GetGroupNo() != RowAccount.GetGroupNo()) || ( TRUE == bForceLoadGroup ) )
		{
			CSQLRepositoryGroup repoGroup;
			RowGroup.SetGroupNo(RowAccount.GetGroupNo());
			bGotGroup = (repoGroup.SelectRow(RowGroup, NULL).GetSQLError() == SQLCRUD_ERR_NONE);
		}
		else
		{
			bGotGroup = TRUE;
		}

		if (TRUE == bGotGroup)
		{
			if (RowGroup.GetRefreshType() != nREFRESHTYPE_NORMAL)
			{
				CSSDate dateToday = CSSDate::GetCurrentDate();

				if (IsRefreshPurse2Reqd(RowAccount, RowGroup, dateToday))
				{
					bUseRefreshValue = TRUE;
				}
			}
		}
	}

	if (FALSE == bUseRefreshValue)
	{
		return RowAccount.GetPurse2Balance();
	}
	else
	{
		return RowGroup.GetRefreshValue();
	}
}

//*******************************************************************
