//******************************************************************************
#include "..\SPosLoyalty4Shared\SQLDefines.h"
#include "..\SPosLoyalty4Shared\SQLTable_Group\SQLRepositoryGroup.h"
//******************************************************************************
#include "GroupComboHelpers.h"
//******************************************************************************
#include "CalculateSpendBonus.h"
//******************************************************************************
// on entry atc filled out with partial record details so have a group number
// audit then is saved after call to this function

CCalculateSpendBonus::CCalculateSpendBonus ( double dSpend, CAuditRecord* pAtcRecord )
{
	m_dSpend = dSpend;
	m_pAtcRecord = pAtcRecord;
	m_dBonusCash = 0.0;
	m_nBonusPoints = 0;

	Server.Read ( Filenames.GetServerDataFilename() );
	CalculateBonus();
}

//******************************************************************************

CCalculateSpendBonus::~CCalculateSpendBonus()
{
}

//******************************************************************************
// calcualte bonus for cash added
	
void CCalculateSpendBonus::CalculateBonus()
{
	if (Server.GetRewardManualSpendFlag() == FALSE)
	{
		return;
	}
	
	// assume server bonus
	int nBonusType = Server.GetRewardType();
	double dBonusTrip = Server.GetRewardTrip();
	double dBonusValue = Server.GetRewardValue();
	int nBonusPoints = Server.GetRewardPoints();
	
	CSQLRowGroup RowGroup;
	RowGroup.SetGroupNo(m_pAtcRecord->GetGroupNo());

	CSQLRepositoryGroup repoGroup;
	if (repoGroup.SelectRow(RowGroup, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
	{
		if (RowGroup.GetRewardFlag() == TRUE)				// use group specific
		{
			if (RowGroup.GetRewardDisable() == TRUE)
			{
				return;
			}

			nBonusType = RowGroup.GetRewardType();
			dBonusTrip = RowGroup.GetRewardTrip();
			dBonusValue = RowGroup.GetRewardBonus();
			nBonusPoints = RowGroup.GetRewardPoints();
		}
	}

	if (dBonusTrip != 0.0)										// see if any bonus is required
	{
		switch (nBonusType)
		{
		case 0:
		{
			m_nBonusPoints = Server.CalculateRevaluationBonusPoints(m_dSpend, dBonusTrip, nBonusPoints);
			if (m_nBonusPoints != 0)
			{
				m_pAtcRecord->SetPoints(m_pAtcRecord->GetPoints() + m_nBonusPoints);
				m_pAtcRecord->SetPointsTD(m_pAtcRecord->GetPointsTD() + m_nBonusPoints);
				m_pAtcRecord->SetPointsTransaction(m_nBonusPoints);
				m_pAtcRecord->SetPurse1Transaction(0.0);
				m_pAtcRecord->SetPurse2Transaction(0.0);
				m_pAtcRecord->SetCashTransaction(m_dSpend);
				m_pAtcRecord->SetSourceType(AUDIT_PC);
				m_pAtcRecord->SetApplicationNo(APPNO_REWARDBONUSPOINTS);			// audit bonus points
			}
		}
		break;

		case 1:
		{
			m_dBonusCash = Server.CalculateRevaluationBonusValue(m_dSpend, dBonusTrip, dBonusValue);
			if (m_dBonusCash != 0)
			{
				m_pAtcRecord->SetPurse1(m_pAtcRecord->GetPurse1() + m_dBonusCash);
				m_pAtcRecord->SetPurse1Transaction(m_dBonusCash);
				m_pAtcRecord->SetPurse2Transaction(0.0);
				m_pAtcRecord->SetCashTransaction(m_dSpend);
				m_pAtcRecord->SetPointsTransaction(0);
				m_pAtcRecord->SetSourceType(AUDIT_PC);
				m_pAtcRecord->SetApplicationNo(APPNO_REWARDBONUSCASH);			// audit bonus cash
			}
		}
		break;
		}
	}
}

//****************************************************************
