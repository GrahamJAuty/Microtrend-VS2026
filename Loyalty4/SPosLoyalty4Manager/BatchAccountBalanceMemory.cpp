//$$******************************************************************
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRowAccountFull.h"
//$$******************************************************************
#include "BatchAccountBalanceMemory.h"
//$$******************************************************************

CBatchAccountBalanceMemory::CBatchAccountBalanceMemory()
{
	m_bAddPurse1 = FALSE;
	m_bAddPurse2 = FALSE;
	m_bAddPoints = FALSE;
	m_bAddPurse1SpendTD = FALSE;
	m_bAddPurse2SpendTD = FALSE;
	m_bAddCashSpendTD = FALSE;
	m_bAddPointsTD = FALSE;
	m_bClearPurse1 = FALSE;
	m_bClearPurse2 = FALSE;
	m_bClearPoints = FALSE;
	m_bClearPurse1SpendTD = FALSE;
	m_bClearPurse2SpendTD = FALSE;
	m_bClearCashSpendTD = FALSE;
	m_bClearPointsTD = FALSE;
	m_dPurse1 = 0.0;
	m_dPurse2 = 0.0;
	m_nPoints = 0;
	m_dPurse1SpendTD = 0.0;
	m_dPurse2SpendTD = 0.0;
	m_dCashSpendTD = 0.0;
	m_nPointsTD = 0;
}

//$$******************************************************************

void CBatchAccountBalanceMemory::TidyOptions()
{
	bool bAllowPurse1 = System.GetBatchUpdatePurse1Flag();
	bool bAllowPurse2 = System.GetEnablePurse2Flag() && System.GetBatchUpdatePurse2Flag();
	bool bAllowPoints = System.GetBatchUpdatePointsFlag();
	bool bAllowToDate = System.GetBatchUpdateSpendTDFlag();

	m_bAddPurse1 &= bAllowPurse1;
	m_bClearPurse1 &= (bAllowPurse1 & !m_bAddPurse1);

	m_bAddPurse2 &= bAllowPurse2;
	m_bClearPurse2 &= (bAllowPurse2 & !m_bAddPurse2);

	m_bAddPoints &= bAllowPoints;
	m_bClearPoints &= (bAllowPoints & !m_bAddPoints);

	m_bAddPurse1SpendTD &= (bAllowPurse1 & bAllowToDate);
	m_bClearPurse1SpendTD &= (bAllowPurse1 & bAllowToDate & !m_bAddPurse1SpendTD);

	m_bAddPurse2SpendTD &= (bAllowPurse2 & bAllowToDate);
	m_bClearPurse2SpendTD &= (bAllowPurse2 & bAllowToDate & !m_bAddPurse2SpendTD);

	m_bAddPointsTD &= (bAllowPoints & (bAllowToDate | m_bAddPoints));
	m_bClearPointsTD &= (bAllowPoints & bAllowToDate & !m_bAddPointsTD);

	m_bAddCashSpendTD &= bAllowToDate;
	m_bClearCashSpendTD &= (bAllowToDate & !m_bAddCashSpendTD);

	if (m_bClearPurse1) m_dPurse1 = 0.0;
	if (m_bClearPurse2) m_dPurse2 = 0.0;
	if (m_bClearPoints) m_nPoints = 0;
	if (m_bClearPurse1SpendTD) m_dPurse1SpendTD = 0.0;
	if (m_bClearPurse2SpendTD) m_dPurse2SpendTD = 0.0;
	if (m_bClearCashSpendTD) m_dCashSpendTD = 0.0;
	if (m_bClearPointsTD) m_nPointsTD = 0;

	if ((false == bAllowToDate) && (true == m_bAddPointsTD))
	{
		m_nPointsTD = m_nPoints;
	}
}

//$$******************************************************************

bool CBatchAccountBalanceMemory::HaveUpdates()
{
	if (FALSE == System.GetBatchUpdateSpendTDFlag())
	{
		m_nPointsTD = m_nPoints;
	}

	if ((TRUE == m_bAddPurse1) && (m_dPurse1 != 0.0))
	{
		return TRUE;
	}

	if (TRUE == m_bClearPurse1)
	{
		return TRUE;
	}

	if ((TRUE == m_bAddPurse1SpendTD) && (m_dPurse1SpendTD != 0.0))
	{
		return TRUE;
	}

	if (TRUE == m_bClearPurse1SpendTD)
	{
		return TRUE;
	}

	if ((TRUE == m_bAddPurse2) && (m_dPurse2 != 0.0))
	{
		return TRUE;
	}

	if (TRUE == m_bClearPurse2)
	{
		return TRUE;
	}

	if ((TRUE == m_bAddPurse2SpendTD) && (m_dPurse2SpendTD != 0.0))
	{
		return TRUE;
	}

	if (TRUE == m_bClearPurse2SpendTD)
	{
		return TRUE;
	}

	if ((TRUE == m_bAddPoints) && (m_nPoints != 0))
	{
		return TRUE;
	}

	if (TRUE == m_bClearPoints)
	{
		return TRUE;
	}

	if ((TRUE == m_bAddPointsTD) && (m_nPointsTD != 0))
	{
		return TRUE;
	}

	if (TRUE == m_bClearPointsTD)
	{
		return TRUE;
	}

	if ((TRUE == m_bAddCashSpendTD) && (m_dCashSpendTD != 0.0))
	{
		return TRUE;
	}

	if (TRUE == m_bClearCashSpendTD)
	{
		return TRUE;
	}

	return FALSE;
}

//$$******************************************************************

bool CBatchAccountBalanceMemory::UpdateRecord(CSQLRowAccountFull& RowAccount)
{
	if (FALSE == System.GetBatchUpdateSpendTDFlag())
	{
		m_nPointsTD = m_nPoints;
	}

	bool bReply = FALSE;

	// Purse

	if (m_bAddPurse1)
	{
		RowAccount.AddToPurse1(m_dPurse1);
		bReply = TRUE;
	}
	else if (TRUE == m_bClearPurse1)
	{
		if (RowAccount.GetPurse1Balance() != 0.0)
		{
			RowAccount.SetPurse1Balance(0);
			bReply = TRUE;
		}
	}

	// Purse2

	if (TRUE == m_bAddPurse2)
	{
		RowAccount.AddToPurse2(m_dPurse2);
		bReply = TRUE;
	}
	else if (TRUE == m_bClearPurse2)
	{
		if (RowAccount.GetPurse2Balance() != 0.0)
		{
			RowAccount.SetPurse2Balance(0);
			bReply = TRUE;
		}
	}

	// Points

	if (TRUE == m_bAddPoints)
	{
		RowAccount.AddToPoints(m_nPoints);
		bReply = TRUE;
	}
	else if (TRUE == m_bClearPoints)
	{
		if (RowAccount.GetPoints() != 0)
		{
			RowAccount.SetPoints(0);
			bReply = TRUE;
		}
	}

	// Points TD

	if (TRUE == m_bAddPointsTD)
	{
		RowAccount.AddToPointsToDate(m_nPointsTD);
		bReply = TRUE;
	}
	else if (TRUE == m_bClearPointsTD)
	{
		if (RowAccount.GetPointsToDate() != 0)
		{
			RowAccount.SetPointsToDate(0);
			bReply = TRUE;
		}
	}

	// Purse1 Spend

	if (TRUE == m_bAddPurse1SpendTD)
	{
		RowAccount.AddToPurse1SpendToDate(m_dPurse1SpendTD);
		bReply = TRUE;
	}
	else if (TRUE == m_bClearPurse1SpendTD)
	{
		if (RowAccount.GetPurse1SpendToDate() != 0.0)
		{
			RowAccount.SetPurse1SpendToDate(0);
			bReply = TRUE;
		}
	}

	// Purse2 Spend

	if (TRUE == m_bAddPurse2SpendTD)
	{
		RowAccount.AddToPurse2SpendToDate(m_dPurse2SpendTD);
		bReply = TRUE;
	}
	else if (TRUE == m_bClearPurse2SpendTD)
	{
		if (RowAccount.GetPurse2SpendToDate() != 0.0)
		{
			RowAccount.SetPurse2SpendToDate(0);
			bReply = TRUE;
		}
	}

	// Cash Spend

	if (TRUE == m_bAddCashSpendTD)
	{
		RowAccount.AddToCashSpendToDate(m_dCashSpendTD);
		bReply = TRUE;
	}
	else if (TRUE == m_bClearCashSpendTD)
	{
		if (RowAccount.GetCashSpendToDate() != 0.0)
		{
			RowAccount.SetCashSpendToDate(0);
			bReply = TRUE;
		}
	}

	return bReply;
}

//$$******************************************************************
