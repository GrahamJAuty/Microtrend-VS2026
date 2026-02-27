//$$******************************************************************
#include "BatchAccountBalanceMemory.h"
//$$******************************************************************

CBatchAccountBalanceMemory::CBatchAccountBalanceMemory()
{
	m_bAddPurse1Liability = FALSE;
	m_bAddPurse1Credit = FALSE;
	m_bAddPurse3Liability = FALSE;
	m_bAddPurse3Credit = FALSE;
	m_bAddPoints = FALSE;
	m_bAddPurse1SpendTD = FALSE;
	m_bAddPurse2SpendTD = FALSE;
	m_bAddPurse3SpendTD = FALSE;
	m_bAddPointsTD = FALSE;
	m_bClearPurse1Liability = FALSE;
	m_bClearPurse3Liability = FALSE;
	m_bClearPurse1Credit = FALSE;
	m_bClearPurse3Credit = FALSE;
	m_bClearPurse1SpendTD = FALSE;
	m_bClearPurse2SpendTD = FALSE;
	m_bClearPurse3SpendTD = FALSE;
	m_bClearCashSpendTD = FALSE;
	m_bClearPointsTD = FALSE;
	m_dPurse1Liability = 0.0;
	m_dPurse1Credit = 0.0;
	m_dPurse3Liability = 0.0;
	m_dPurse3Credit = 0.0;
	m_nPoints = 0;
	m_dPurse1SpendTD = 0.0;
	m_dPurse2SpendTD = 0.0;
	m_dPurse3SpendTD = 0.0;
	m_nPointsTD = 0;
}

//$$******************************************************************

void CBatchAccountBalanceMemory::TidyOptions()
{
	bool bAllowPoints = System.GetActiveEnablePointsFlag();
	bool bAllowPurse3 = System.GetEnablePurse3Flag();

	m_bClearPurse1Liability &= (!m_bAddPurse1Liability);
	m_bClearPurse1Credit &= (!m_bAddPurse1Credit);

	m_bAddPurse3Liability &= bAllowPurse3;
	m_bAddPurse3Credit &= bAllowPurse3;
	m_bClearPurse3Liability &= (bAllowPurse3 & !m_bAddPurse3Liability);
	m_bClearPurse3Credit &= (bAllowPurse3 & !m_bAddPurse3Credit);

	m_bAddPoints &= bAllowPoints;
	m_bClearPoints &= (bAllowPoints & !m_bAddPoints);

	m_bClearPurse1SpendTD &= (!m_bAddPurse1SpendTD);
	m_bClearPurse2SpendTD &= (!m_bAddPurse2SpendTD);
	m_bClearPurse3SpendTD &= (bAllowPurse3 & !m_bAddPurse3SpendTD);

	m_bAddPointsTD &= bAllowPoints;
	m_bClearPointsTD &= (bAllowPoints & !m_bAddPointsTD);

	if (m_bClearPurse1Liability)
	{
		m_dPurse1Liability = 0.0;
	}

	if (m_bClearPurse1Credit)
	{
		m_dPurse1Credit = 0.0;
	}

	if (m_bClearPurse3Liability)
	{
		m_dPurse3Liability = 0.0;
	}

	if (m_bClearPurse3Credit)
	{
		m_dPurse3Credit = 0.0;
	}

	if (m_bClearPoints)
	{
		m_nPoints = 0;
	}

	if (m_bClearPurse1SpendTD)
	{
		m_dPurse1SpendTD = 0.0;
	}

	if (m_bClearPurse2SpendTD)
	{
		m_dPurse2SpendTD = 0.0;
	}

	if (m_bClearPurse3SpendTD)
	{
		m_dPurse3SpendTD = 0.0;
	}

	if (m_bClearPointsTD)
	{
		m_nPointsTD = 0;
	}
}

//$$******************************************************************

bool CBatchAccountBalanceMemory::HaveUpdates()
{
	bool bResult = FALSE;

	bResult |= ((TRUE == m_bAddPurse1Liability) && (m_dPurse1Liability != 0.0));
	bResult |= (TRUE == m_bClearPurse1Liability);
	bResult |= ((TRUE == m_bAddPurse1SpendTD) && (m_dPurse1SpendTD != 0.0));
	bResult |= (TRUE == m_bClearPurse1SpendTD);

	bResult |= ((TRUE == m_bAddPurse2SpendTD) && (m_dPurse2SpendTD != 0.0));
	bResult |= (TRUE == m_bClearPurse2SpendTD);

	bResult |= ((TRUE == m_bAddPurse3Liability) && (m_dPurse3Liability != 0.0));
	bResult |= (TRUE == m_bClearPurse3Liability);
	bResult |= ((TRUE == m_bAddPurse3SpendTD) && (m_dPurse3SpendTD != 0.0));
	bResult |= (TRUE == m_bClearPurse3SpendTD);

	bResult |= ((TRUE == m_bAddPoints) && (m_nPoints != 0));
	bResult |= (TRUE == m_bClearPoints);
	bResult |= ((TRUE == m_bAddPointsTD) && (m_nPointsTD != 0));
	bResult |= (TRUE == m_bClearPointsTD);

	bResult |= (TRUE == m_bClearCashSpendTD);

	return bResult;
}

//$$******************************************************************

bool CBatchAccountBalanceMemory::UpdateRecord(CSQLRowAccountFull& RowAccount)
{
	bool bReply = FALSE;

	// Purse

	if (m_bAddPurse1Liability)
	{
		RowAccount.AddToPurse1Liability(m_dPurse1Liability);
		bReply = TRUE;
	}
	else if (TRUE == m_bClearPurse1Liability)
	{
		if (RowAccount.GetPurse1Liability() != 0.0)
		{
			RowAccount.SetPurse1Liability(0.0);
			bReply = TRUE;
		}
	}

	if (m_bAddPurse1Credit)
	{
		bool bAllow = TRUE;

		if (m_dPurse1Credit < 0.0)
		{
			bAllow = (RowAccount.GetPurse1Credit() + m_dPurse1Credit) >= 0.0;
		}

		if (TRUE == bAllow)
		{
			RowAccount.AddToPurse1Credit(m_dPurse1Credit);
			bReply = TRUE;
		}
	}
	else if (TRUE == m_bClearPurse1Credit)
	{
		if (RowAccount.GetPurse1Credit() != 0.0)
		{
			RowAccount.SetPurse1Credit(0.0);
			bReply = TRUE;
		}
	}

	// Purse3

	if (m_bAddPurse3Liability)
	{
		RowAccount.AddToPurse3Liability(m_dPurse3Liability);
		bReply = TRUE;
	}
	else if (TRUE == m_bClearPurse3Liability)
	{
		if (RowAccount.GetPurse3Liability() != 0.0)
		{
			RowAccount.SetPurse3Liability(0.0);
			bReply = TRUE;
		}
	}

	if (m_bAddPurse3Credit)
	{
		bool bAllow = TRUE;

		if (m_dPurse3Credit < 0.0)
		{
			bAllow = (RowAccount.GetPurse3Credit() + m_dPurse3Credit) >= 0.0;
		}

		if (TRUE == bAllow)
		{
			RowAccount.AddToPurse3Credit(m_dPurse3Credit);
			bReply = TRUE;
		}
	}
	else if (TRUE == m_bClearPurse3Credit)
	{
		if (RowAccount.GetPurse3Credit() != 0.0)
		{
			RowAccount.SetPurse3Credit(0.0);
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

	// Purse2 Spend

	if (TRUE == m_bAddPurse3SpendTD)
	{
		RowAccount.AddToPurse3SpendToDate(m_dPurse3SpendTD);
		bReply = TRUE;
	}
	else if (TRUE == m_bClearPurse3SpendTD)
	{
		if (RowAccount.GetPurse3SpendToDate() != 0.0)
		{
			RowAccount.SetPurse3SpendToDate(0);
			bReply = TRUE;
		}
	}

	// Cash Spend

	if (TRUE == m_bClearCashSpendTD)
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

void CBatchAccountBalanceMemory::AddWarnings(CStringArray* pArray)
{
	// purse 1

	if ((TRUE == m_bAddPurse1Liability) && (m_dPurse1Liability != 0.0))
	{
		CString strMsg = "";

		if (m_dPurse1Liability < 0.0)
		{
			strMsg.Format("A value of %.2f will be SUBTRACTED from Purse1 Liability.", -(m_dPurse1Liability));
		}
		else
		{
			strMsg.Format("A value of %.2f will be ADDED to Purse1 Liability.", m_dPurse1Liability);
		}

		pArray->Add(strMsg);
	}
	else if (TRUE == m_bClearPurse1Liability)
	{
		pArray->Add("The Purse1 Liability value will be CLEARED.");
	}

	if ((m_bAddPurse1Credit == TRUE) && (m_dPurse1Credit != 0.0))
	{
		CString strMsg;
		if (m_dPurse1Credit < 0.0)
		{
			strMsg.Format("A value of %.2f will be SUBTRACTED from Purse1 Credit.", -(m_dPurse1Credit));
		}
		else
		{
			strMsg.Format("A value of %.2f will be ADDED to Purse1 Credit.", m_dPurse1Credit);
		}

		pArray->Add(strMsg);
	}
	else if (TRUE == m_bClearPurse1Credit)
	{
		pArray->Add("The Purse1 Credit value will be CLEARED.");
	}

	// purse 3

	if ((TRUE == m_bAddPurse3Liability) && (m_dPurse3Liability != 0.0))
	{
		CString strMsg = "";
		if (m_dPurse3Liability < 0.0)
		{
			strMsg.Format("A value of %.2f will be SUBTRACTED from Purse3 Liability.", -(m_dPurse3Liability));
		}
		else
		{
			strMsg.Format("A value of %.2f will be ADDED to Purse3 Liability.", m_dPurse3Liability);
		}

		pArray->Add(strMsg);
	}
	else if (TRUE == m_bClearPurse3Liability)
	{
		pArray->Add("The Purse3 Liability value will be CLEARED.");
	}

	if ((TRUE == m_bAddPurse3Credit) && (m_dPurse3Credit != 0.0))
	{
		CString strMsg;
		if (m_dPurse3Credit < 0.0)
		{
			strMsg.Format("A value of %.2f will be SUBTRACTED from Purse3 Credit.", -(m_dPurse3Credit));
		}
		else
		{
			strMsg.Format("A value of %.2f will be ADDED to Purse3 Credit.", m_dPurse3Credit);
		}

		pArray->Add(strMsg);
	}
	else if (TRUE == m_bClearPurse3Credit)
	{
		pArray->Add("The Purse3 Credit value will be CLEARED.");
	}

	// points

	if ((TRUE == m_bAddPoints) && (m_nPoints != 0))
	{
		CString strMsg = "";
		if (m_nPoints < 0)
		{
			strMsg.Format("%d Points will be SUBTRACTED from the Points value.", -(m_nPoints));
		}
		else
		{
			strMsg.Format("%d Points will be ADDED to the Points value.", m_nPoints);
		}

		pArray->Add(strMsg);
	}
	else if (TRUE == m_bClearPoints)
	{
		pArray->Add("The Points value will be CLEARED.");
	}

	// pointstd

	if ((TRUE == m_bAddPointsTD) && (m_nPointsTD != 0))
	{
		CString strMsg = "";
		if (m_nPointsTD < 0)
		{
			strMsg.Format("%d Points will be SUBTRACTED from the PointsTD value.", -(m_nPointsTD));
		}
		else
		{
			strMsg.Format("%d Points will be ADDED to the PointsTD value.", m_nPointsTD);
		}

		pArray->Add(strMsg);
	}
	else if (TRUE == m_bClearPointsTD)
	{
		pArray->Add("The PointsTD value will be CLEARED.");
	}

	// cash spend

	if (TRUE == m_bClearCashSpendTD)
	{
		pArray->Add("The Cash SpendTD value will be CLEARED.");
	}

	// Purse1 spend

	if ((TRUE == m_bAddPurse1SpendTD) && (m_dPurse1SpendTD != 0.0))
	{
		CString strMsg = "";

		if (m_dPurse1SpendTD < 0.0)
		{
			strMsg.Format("A value of %.2f will be SUBTRACTED from Purse1SpendTD.", -(m_dPurse1SpendTD));
		}
		else
		{
			strMsg.Format("A value of %.2f will be ADDED to Purse1SpendTD.", m_dPurse1SpendTD);
		}

		pArray->Add(strMsg);
	}
	else if (TRUE == m_bClearPurse1SpendTD)
	{
		pArray->Add("The Purse1SpendTD value will be CLEARED.");
	}
	// Purse2 spend

	if ((TRUE == m_bAddPurse2SpendTD) && (m_dPurse2SpendTD != 0.0))
	{
		CString strMsg = "";

		if (m_dPurse2SpendTD < 0.0)
		{
			strMsg.Format("A value of %.2f will be SUBTRACTED from Purse2SpendTD.", -(m_dPurse2SpendTD));
		}
		else
		{
			strMsg.Format("A value of %.2f will be ADDED to Purse2SpendTD.", m_dPurse2SpendTD);
		}

		pArray->Add(strMsg);
	}
	else if (TRUE == m_bClearPurse2SpendTD)
	{
		pArray->Add("The Purse2SpendTD value will be CLEARED.");
	}

	// Purse3 spend

	if ((TRUE == m_bAddPurse3SpendTD) && (m_dPurse3SpendTD != 0.0))
	{
		CString strMsg = "";
		if (m_dPurse3SpendTD < 0.0)
		{
			strMsg.Format("A value of %.2f will be SUBTRACTED from Purse3SpendTD.", -(m_dPurse3SpendTD));
		}
		else
		{
			strMsg.Format("A value of %.2f will be ADDED to Purse3SpendTD.", m_dPurse3SpendTD);
		}

		pArray->Add(strMsg);
	}
	else if (TRUE == m_bClearPurse3SpendTD)
	{
		pArray->Add("The Purse3SpendTD value will be CLEARED.");
	}
}

//$$******************************************************************
