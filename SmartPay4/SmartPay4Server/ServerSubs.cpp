//*******************************************************************
#include "..\SmartPay4Shared\SQLTable_AuditPeriod\SQLRowSetAuditPeriod.h"
#include "..\SmartPay4Shared\SQLTable_Group\SQLRepositoryGroup.h"
//*******************************************************************
#include "ServerSubs.h"
//*******************************************************************

bool CServerSubs::IsPurseAllowed ( int nPurseNo, CSQLRowAccountFull& RowAccount, CSQLRowGroupFull& RowGroup, CSSDate& date, CSSTime& time )
{
	bool bPurseAllowed = TRUE;											// assume allowed

	if ( nPurseNo == 2 )
	{
		switch (RowGroup.GetRefreshType())
		{
		case nREFRESHTYPE_DISABLED:
			bPurseAllowed = FALSE;
			break;

		case nREFRESHTYPE_DAILY:
		case nREFRESHTYPE_PERIOD:
			{
				int nDayNo = date.GetDayOfWeek();							// 1=sun, 2=mon, 0=not set
				bPurseAllowed = RowGroup.IsRefreshAllowed(nDayNo - 1);		// 0=sun, 1=mon etc..
			}
			break;
		}


//		else if ( pDbGroup->GetRefreshType() != nREFRESHTYPE_MANUAL )	//v1.07a 16/12/2015
//		{ 
//			int nDayNo = date.GetDayOfWeek();							// 1=sun, 2=mon, 0=not set
//			bPurseAllowed = pDbGroup->IsRefreshAllowed(nDayNo-1);		// 0=sun, 1=mon etc..
//		}

	}

	if ( bPurseAllowed == TRUE )										// see if still allowed
	{
		int nPeriod = SQLRowSetAuditPeriod.GetPeriodIndex ( time.GetTime() );
			
		switch( RowGroup.GetMaxSpendType() )
		{
		case nGRPSPENDTYPE_PERIOD_LOCKED:
			bPurseAllowed = RowGroup.IsPurseAllowed ( nPurseNo, nPeriod );
			break;

		case nGRPSPENDTYPE_PERIOD_RECORD1:
			bPurseAllowed = RowAccount.IsPurseAllowed ( nPurseNo, nPeriod );
			break;

		case nGRPSPENDTYPE_PERIOD_RECORD2:
			if (RowAccount.GetOwnMaxSpend() == TRUE)
			{
				bPurseAllowed = RowAccount.IsPurseAllowed(nPurseNo, nPeriod);
			}
			else
			{
				bPurseAllowed = RowGroup.IsPurseAllowed(nPurseNo, nPeriod);
			}
			break;
		}
	}
	return bPurseAllowed;
}

//*******************************************************************

void CServerSubs::GetMaxSpendFlags(CSQLRowAccountFull& RowAccount, CSQLRowGroupFull& RowGroup, bool& bDaily, bool& bGroup )
{
	switch( RowGroup.GetMaxSpendType() )
	{
	case nGRPSPENDTYPE_DAILY_RECORD1:
		bDaily = TRUE;
		bGroup = FALSE;
		break;

	case nGRPSPENDTYPE_DAILY_RECORD2:
		bDaily = TRUE;
		bGroup = (RowAccount.GetOwnMaxSpend() == FALSE );
		break;

	case nGRPSPENDTYPE_PERIOD_LOCKED:
		bDaily = FALSE;
		bGroup = TRUE;
		break;

	case nGRPSPENDTYPE_PERIOD_RECORD1:
		bDaily = FALSE;
		bGroup = FALSE;
		break;

	case nGRPSPENDTYPE_PERIOD_RECORD2:
		bDaily = FALSE;
		bGroup = (RowAccount.GetOwnMaxSpend() == FALSE );
		break;

	case nGRPSPENDTYPE_DAILY_LOCKED:
	default:
		bGroup = TRUE;
		bDaily = TRUE;
		break;
	}
}

//*******************************************************************
