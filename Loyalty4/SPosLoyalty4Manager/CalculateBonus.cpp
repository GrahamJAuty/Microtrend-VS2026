//******************************************************************************
#include "..\SPosLoyalty4Shared\SQLDefines.h"
#include "..\SPosLoyalty4Shared\SQLTable_Group\SQLRepositoryGroup.h"
#include "..\SPosLoyalty4Shared\SQLTable_GroupTopupBonus\SQLRepositoryGroupTopupBonus.h"
//******************************************************************************
#include "GroupComboHelpers.h"
//******************************************************************************
#include "CalculateBonus.h"
//******************************************************************************
// on entry atc filled out with partial record details so have a group number
// audit then is saved after call to this function

CCalculateBonus::CCalculateBonus ( bool bPurse2, double dCashAdded, CAuditRecord* pAtcRecord )
{
	m_dCashAdded = dCashAdded;
	m_pAtcRecord = pAtcRecord;
	m_dBonusCash = 0.0;
	m_nBonusPoints = 0;
	m_bPurse2 = bPurse2;

	Server.Read ( Filenames.GetServerDataFilename() );
	CalculateBonus();
}

//******************************************************************************

CCalculateBonus::~CCalculateBonus()
{
}

//******************************************************************************
// calcualte bonus for cash added
	
void CCalculateBonus::CalculateBonus()
{
	int nTopupNo = 0;
	int nPurseNo = 0;

	if (TRUE == m_bPurse2)
	{
		nTopupNo = Server.GetTopupPCBonusTypePurse2();
		nPurseNo = 2;
	}
	else
	{
		nTopupNo = Server.GetTopupPCBonusTypePurse1();
		nPurseNo = 1;
	}

	if ((nTopupNo < 1) || (nTopupNo > REVALUE_PLUNO_PERPURSE_COUNT))
	{
		return;
	}

	// assume server bonus
	int nRevaluationType = Server.GetTopupBonusType(nPurseNo, nTopupNo);
	double dRevaluationTrip = Server.GetTopupBonusTrip(nPurseNo, nTopupNo);
	double dRevaluationValue = Server.GetTopupBonusValue(nPurseNo, nTopupNo);
	int nRevaluationPoints = Server.GetTopupBonusPoints(nPurseNo, nTopupNo);
	int nRevaluationPercent = Server.GetTopupBonusPercent(nPurseNo, nTopupNo);

	CSQLRowGroup RowGroup;
	RowGroup.SetGroupNo(m_pAtcRecord->GetGroupNo());

	CSQLRepositoryGroup repoGroup;
	CSQLRepositoryGroupTopupBonus repoBonus;

	if (repoGroup.SelectRow(RowGroup, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
	{
		CSQLRowGroupTopupBonus RowGroupTopupBonus;
		RowGroupTopupBonus.SetGroupNo(m_pAtcRecord->GetGroupNo());
		RowGroupTopupBonus.SetTopupNo(nTopupNo);
		RowGroupTopupBonus.SetPurseNo(nPurseNo);

		if (repoBonus.SelectRow(RowGroupTopupBonus, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
		{
			if (RowGroupTopupBonus.GetBonusFlag() == TRUE)
			{
				nRevaluationType = RowGroupTopupBonus.GetBonusType();
				dRevaluationTrip = RowGroupTopupBonus.GetBonusTrip();
				dRevaluationValue = RowGroupTopupBonus.GetBonusValue();
				nRevaluationPoints = RowGroupTopupBonus.GetBonusPoints();
				nRevaluationPercent = RowGroupTopupBonus.GetBonusPercent();
			}
		}
	}

	if (dRevaluationTrip != 0.0)										// see if any revaluation bonus is required
	{
		switch (nRevaluationType)
		{
		case 0:
		{
			m_nBonusPoints = Server.CalculateRevaluationBonusPoints(m_dCashAdded, dRevaluationTrip, nRevaluationPoints);
			if (m_nBonusPoints != 0)
			{
				m_pAtcRecord->SetPoints(m_pAtcRecord->GetPoints() + m_nBonusPoints);
				m_pAtcRecord->SetPointsTD(m_pAtcRecord->GetPointsTD() + m_nBonusPoints);
				m_pAtcRecord->SetPointsTransaction(m_nBonusPoints);
				m_pAtcRecord->SetPurse1Transaction(0.0);
				m_pAtcRecord->SetPurse2Transaction(0.0);
				m_pAtcRecord->SetCashTransaction(m_dCashAdded);
				m_pAtcRecord->SetSourceType(AUDIT_PC);
				m_pAtcRecord->SetApplicationNo(APPNO_REVALUEBONUSPOINTS);			// audit bonus points
			}
		}
		break;

		case 1:
		{
			m_dBonusCash = Server.CalculateRevaluationBonusValue(m_dCashAdded, dRevaluationTrip, dRevaluationValue);
			if (m_dBonusCash != 0)
			{
				if (FALSE == m_bPurse2)
				{
					m_pAtcRecord->SetPurse1(m_pAtcRecord->GetPurse1() + m_dBonusCash);
					m_pAtcRecord->SetPurse1Transaction(m_dBonusCash);
					m_pAtcRecord->SetPurse2Transaction(0.0);
				}
				else
				{
					m_pAtcRecord->SetPurse2(m_pAtcRecord->GetPurse2() + m_dBonusCash);
					m_pAtcRecord->SetPurse2Transaction(m_dBonusCash);
					m_pAtcRecord->SetPurse1Transaction(0.0);
				}

				m_pAtcRecord->SetCashTransaction(m_dCashAdded);
				m_pAtcRecord->SetPointsTransaction(0);
				m_pAtcRecord->SetSourceType(AUDIT_PC);
				m_pAtcRecord->SetApplicationNo(APPNO_REVALUEBONUSCASH);			// audit bonus cash
			}
		}
		break;

		case 2:
		{
			m_dBonusCash = Server.CalculateRevaluationBonusPercent(m_dCashAdded, dRevaluationTrip, nRevaluationPercent);
			if (m_dBonusCash != 0)
			{
				if (FALSE == m_bPurse2)
				{
					m_pAtcRecord->SetPurse1(m_pAtcRecord->GetPurse1() + m_dBonusCash);
					m_pAtcRecord->SetPurse1Transaction(m_dBonusCash);
					m_pAtcRecord->SetPurse2Transaction(0.0);
				}
				else
				{
					m_pAtcRecord->SetPurse2(m_pAtcRecord->GetPurse2() + m_dBonusCash);
					m_pAtcRecord->SetPurse2Transaction(m_dBonusCash);
					m_pAtcRecord->SetPurse1Transaction(0.0);
				}
				
				m_pAtcRecord->SetCashTransaction(m_dCashAdded);
				m_pAtcRecord->SetPointsTransaction(0);
				m_pAtcRecord->SetSourceType(AUDIT_PC);
				m_pAtcRecord->SetApplicationNo(APPNO_REVALUEBONUSCASH);			// audit bonus cash
			}
		}
		break;
		}
	}
}

//****************************************************************
