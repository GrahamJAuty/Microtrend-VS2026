//*******************************************************************
#include "EmailAlertHandler.h"
//*******************************************************************

CEmailAlertHandler::CEmailAlertHandler()
{
}

//*******************************************************************

bool CEmailAlertHandler::IsValidLowBalanceRecord(CSQLRowAccountFull& RowAccount)
{
	bool bReply = FALSE; 
	
	if ( ( bReply = IsValid (RowAccount) ) == TRUE )					// check record is issued / active etc
	{
		if (m_nLowBalanceTickNo != 0)									// 0=none specfic else user tick 1-6
		{
			bReply = RowAccount.GetTick(m_nLowBalanceTickNo);
		}
	}

	return bReply;
}

//*******************************************************************

bool CEmailAlertHandler::IsValidPurchaseRecord(CSQLRowAccountFull& RowAccount)
{
	bool bReply = FALSE; 
	
	if ((bReply = IsValid(RowAccount)) == TRUE)				// check record is issued / active etc
	{
		if (m_nPurchaseTickNo != 0)								// 0=none specfic else user tick 1-4
		{
			bReply = RowAccount.GetTick(m_nPurchaseTickNo);
		}
	}

	return bReply;
}

//*******************************************************************

bool CEmailAlertHandler::IsValidPointsAchievementRecord(CSQLRowAccountFull& RowAccount)
{
	bool bReply = FALSE; 
	
	if ((bReply = IsValid(RowAccount)) == TRUE)				// check record is issued / active etc
	{
		if (m_nPointsAchievementTickNo != 0)						// 0=none specfic else user tick 1-4
		{
			bReply = RowAccount.GetTick(m_nPointsAchievementTickNo);
		}
	}

	return bReply;
}

//*******************************************************************

bool CEmailAlertHandler::IsValidWeeklyPointsUpdateRecord(CSQLRowAccountFull& RowAccount)
{
	bool bReply = FALSE; 
	
	if ((bReply = IsValid(RowAccount)) == TRUE)				// check record is issued / active etc
	{
		if (m_nWeeklyPointsTickNo != 0)							// 0=none specfic else user tick 1-4
		{
			bReply = RowAccount.GetTick(m_nWeeklyPointsTickNo);
		}
	}

	return bReply;
}

//*******************************************************************

bool CEmailAlertHandler::IsValid( CSQLRowAccountFull& RowAccount)
{
	if (System.GetInterfaceType() == nINTERFACE_MIFAREv1)			// UserID = Mifare number
	{
		if (RowAccount.IsAlertSet() == TRUE)						// Hotlisted
		{
			return FALSE;
		}
	}
	else if (System.GetInterfaceType() == nINTERFACE_MIFAREv2)		//26/04/2016 - v1.11c
	{
		if (RowAccount.GetExternalRef1() == "")						// not been issued
		{
			return FALSE;
		}
	}

	if (RowAccount.HasUserExpired() == TRUE)
	{
		return FALSE;
	}

	if (RowAccount.GetInactive() == TRUE)
	{
		return FALSE;
	}
	
	CString strEmail = GetEmailAddress (RowAccount);
	return (strEmail != "");
}

//*******************************************************************

CString CEmailAlertHandler::GetEmailAddress (CSQLRowAccountFull& RowAccount)
{
	CString strEmailAddress = ( m_nEmailContactNo == 0 ) ? RowAccount.GetEmail() : RowAccount.GetInfo(m_nEmailContactNo);
	return strEmailAddress;
}

//*******************************************************************
