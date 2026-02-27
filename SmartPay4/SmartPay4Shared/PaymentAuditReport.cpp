//**********************************************************************
#include "..\SmartPay4Shared\RecSort.h"
#include "..\SmartPay4Shared\RepmanHandler.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
//**********************************************************************
#include "PaymentAuditReport.h"
//**********************************************************************

void CPaymentAuditReport::Reset ( int n )
{
	m_PaymentSale[n].Reset();
}

//**********************************************************************

void CPaymentAuditReport::ResetPaymentGroups()
{
	m_GroupSale.RemoveAll();

	for (int i = 0; i < m_eposPaymentGroups.GetRecordCount(); i++)
	{
		m_eposPaymentGroups.MoveTo(i);

		CEPosPaymentGroupSale GroupSale;
		GroupSale.m_nGroupNo = m_eposPaymentGroups.GetNo();
		GroupSale.m_strGroupText = m_eposPaymentGroups.GetText();
		m_GroupSale.Consolidate(GroupSale);
	}
}

//********************************************************************
// called by EOD reporting to compile epos payments between given dates
// similar to ReportByDate summary report

bool CPaymentAuditReport::EODPayments(const char* szDateFrom, const char* szTimeFrom, const char* szDateTo, const char* szTimeTo)
{
	int nCount = 0;
	bool bStopAtEOD = TRUE;

	CSSDate dateFrom(szDateFrom);
	CSSTime timeFrom(szTimeFrom);

	CSSDate	dateTo(szDateTo);
	CSSTime timeTo(szTimeTo);

	if ((bStopAtEOD = dateTo.IsSet()) == FALSE)
	{
		dateTo.SetCurrentDate();
	}

	if (timeTo.IsSet() == FALSE)
	{
		timeTo.SetCurrentTime();
	}

	SetSummaryRequiredFlag(TRUE);
	Reset(nTOTAL_REPORT);
	ResetPaymentGroups();

	CEposPaymentAuditFile auditFile(m_strAuditFilename);
	auditFile.Open();

	auditFile.JumpIn(dateFrom.GetDate());

	bool bStarted = FALSE;
	CPaymentAuditRecord atc;
	while (auditFile.ReadLine(&atc) == TRUE)
	{
		CSSDate dateAudit(atc.GetDateFiled());
		CSSTime timeAudit(atc.GetTimeFiled());

		if (dateAudit < dateFrom)						// see if started yet
		{
			continue;
		}

		if (dateAudit == dateFrom)					// start date match
		{
			if (timeAudit < timeFrom)					// see if time started yet
			{
				continue;
			}

			if (timeAudit == timeFrom)				// time matches - should be able to find EOD marker
			{
				if (atc.IsEODEntry() == TRUE)			// see if EOD marker
				{
					bStarted = TRUE;					// set EOD start marker found
					continue;
				}
				else
				{
					if (bStarted == FALSE)			// not stated yet no marker as yet
					{
						continue;
					}
				}
			}
		}

		if (dateAudit > dateTo)						// see if gone passed end date
		{
			break;										// yes exit
		}

		if (dateAudit == dateTo)
		{
			if (atc.IsEODEntry() == TRUE)				// found EOD start marker
			{
				break;
			}
		}

		SaveLine(&atc);								// pretend to do a summary line to compile figures
		++nCount;
	}

	return (nCount > 0);
}

//**********************************************************************
// Also called when compiling EOD payment entries

bool CPaymentAuditReport::SaveLine(CPaymentAuditRecord* pAtc)
{
	int nPaymentType = pAtc->GetPaymentType();
	int nPaymentGroup = 1;
	
	CEPosPaymentGroupSale ThisPayment;

	switch( nPaymentType )
	{ 
	case 20:
	case 21:
		ThisPayment.m_dPurse1 = pAtc->GetPaymentValue1();
		ThisPayment.m_dPurse2 = pAtc->GetPaymentValue2();
		ThisPayment.m_dPurse3 = pAtc->GetPaymentValue3();
		ThisPayment.m_dOther = 0.0;
		break;

	default:
		ThisPayment.m_dPurse1 = 0.0;
		ThisPayment.m_dPurse2 = 0.0;
		ThisPayment.m_dPurse3 = 0.0;
		ThisPayment.m_dOther = pAtc->GetPaymentValue1();
		break;
	}

	CString strPaymentText = "";
	if (m_eposPayments.FindRecord(nPaymentType) == TRUE)
	{
		strPaymentText = m_eposPayments.GetText();
		ThisPayment.m_nGroupNo = m_eposPayments.GetGroup();
	}
	else
	{
		ThisPayment.m_nGroupNo = 0;
	}
	
	if (m_eposPaymentGroups.FindRecord(ThisPayment.m_nGroupNo) == FALSE)
	{
		ThisPayment.m_strGroupText = "Unknown";
	}

	CCSV csv('\t');

	csv.Add(pAtc->GetDateFiled());
	csv.Add(pAtc->GetTimeFiled());

	if (pAtc->IsEODEntry() == TRUE)
	{
		csv.Add("");							// UserID
		csv.Add("EOD");						// Username
	}
	else
	{
		CString strUserID = System.FormatCardNo(pAtc->GetUserID());
		csv.Add(strUserID);

		CSQLRowAccountFull RowAccount;
		RowAccount.SetUserIDString(strUserID);

		CSQLRepositoryAccount RepoAccount;
		if (RepoAccount.SelectRow(RowAccount, NULL).GetSQLError() == SQLCRUD_ERR_NONE )
		{
			csv.Add(RowAccount.GetUsername());
			strUserID.TrimLeft('0');

			if (strUserID != "")
			{
				csv.Add(RowAccount.GetGroupNo());
			}
			else
			{
				csv.Add("-");
			}
		}
		else
		{
			csv.Add("unknown");
			csv.Add("-");
		}

		csv.Add(pAtc->GetDateLastUsed());				// date
		csv.Add(pAtc->GetTimeLastUsed());				// time

		if (pAtc->GetExternalMode() == FALSE)
		{
			csv.Add(pAtc->GetOperatorID());
			csv.Add(pAtc->GetTerminalNo());
		}
		else
		{
			csv.Add("");
			csv.Add("Ext");
		}

		csv.Add(pAtc->GetTransactionNo());
		csv.Add(nPaymentType);
		csv.Add(strPaymentText);

		switch (nPaymentType)
		{
		case 20:
		case 21:
			csv.Add(ThisPayment.m_dPurse1, 2);			// purse1
			csv.Add(ThisPayment.m_dPurse2, 2);			// purse2

			if (System.GetEnablePurse3Flag() == TRUE)
			{
				csv.Add(ThisPayment.m_dPurse3, 2);
			}

			csv.Add("--");								// no other
			
			break;

		default:
			csv.Add("--");								// no purse1
			csv.Add("--");								// no purse2

			if (System.GetEnablePurse3Flag() == TRUE)
			{
				csv.Add("--");							// no purse3
			}

			csv.Add(ThisPayment.m_dOther, 2);							// other 
		
			break;
		}

		m_PaymentSale[nTOTAL_CARD].Add(ThisPayment);
		m_PaymentSale[nTOTAL_REPORT].Add(ThisPayment);
		m_GroupSale.Consolidate(ThisPayment);
	}

	return CheckSaveLine(csv);
}

//**********************************************************************
