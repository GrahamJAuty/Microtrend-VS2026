/******************************************************************************/
#include "..\SmartPay4Shared\RepmanDefines.h"
#include "..\SmartPay4Shared\RecSort.h"
#include "..\SmartPay4Shared\SmartPayAuditFile.h"
#include "..\SmartPay4Shared\SQLTable_AuditPeriod\SQLRowSetAuditPeriod.h"
/******************************************************************************/
#include "EODGroupSales.h"
/******************************************************************************/

CEODGroupSales::CEODGroupSales ( const char* szAuditFile )
{
	m_strAuditFilename = szAuditFile;
	SQLRowSetAuditPeriod.LoadPeriods();
}

/******************************************************************************/

void CEODGroupSales::Reset()
{
	for (int nPeriodIndex = 0; nPeriodIndex < nMAXPERIODS; nPeriodIndex++)
	{
		for (int nGroupNo = 0; nGroupNo <= 99; nGroupNo++)
		{
			Sales[nPeriodIndex][nGroupNo].nCardCount = 0;
			//Sales[nPeriodIndex][nGroupNo].nTransCount	= 0;
			Sales[nPeriodIndex][nGroupNo].dPurse1 = 0.0;
			Sales[nPeriodIndex][nGroupNo].dPurse2 = 0.0;
			Sales[nPeriodIndex][nGroupNo].dPurse3 = 0.0;
			Sales[nPeriodIndex][nGroupNo].dOther = 0.0;
		}
	}
}

//************************************************************************************
// If date range not set - its a temp audit file - use all lines it has no EOD markers!!

int CEODGroupSales::Extract(const char* szDateFrom, const char* szTimeFrom, const char* szDateTo, const char* szTimeTo)
{
	CSSDate dateFrom;	
	dateFrom.SetDate(szDateFrom);

	CSSTime timeFrom;
	timeFrom.SetTime(szTimeFrom);

	CSSDate dateTo;		
	dateTo.SetDate(szDateTo);

	CSSTime timeTo;		
	timeTo.SetTime(szTimeFrom);

	bool bFullFile = FALSE;
	if (dateFrom.IsSet() == FALSE)
	{
		bFullFile = TRUE;									// dont look for first EOD marker - use all file
	}
	else
	{
		if (dateTo.IsSet() == FALSE)
		{
			dateTo.SetCurrentDate();
		}

		if (timeTo.IsSet() == FALSE)
		{
			timeTo.SetCurrentTime();
		}
	}

	bool bStarted = FALSE;
	int nReply = nREPORT_NOERROR;

	CSmartPayAuditFile auditFile(m_strAuditFilename);
	if (auditFile.Open() == TRUE)							// try and open audit file
	{
		Reset();
		CRecordSorter sorter;

		if (bFullFile == FALSE)							// find nearest date if not using full file
		{
			auditFile.JumpIn(dateFrom.GetDate());
		}

		CSQLAuditRecord atc;
		while (TRUE)
		{
			int nFilePos = auditFile.GetFilePos();			// start of next line

			if (auditFile.ReadLine(&atc) == FALSE)
			{
				break;
			}

			if (bFullFile == FALSE)
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
						else									// not an EOD marker
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
					if (atc.IsEODEntry() == TRUE)				// found next EOD start marker
					{
						break;
					}
				}
			}

			switch (atc.GetSourceType())
			{
			case AUDIT_POS:
			case AUDIT_EXT:
				if (atc.GetApplicationNo() == APPNO_SALE || atc.GetApplicationNo() == APPNO_REFUNDSALE)
				{
					CString strLabel = "";
					strLabel.Format("%d%2.2d%s", 
						SQLRowSetAuditPeriod.GetPeriodIndex(atc.GetTimeFiled()),
						atc.GetGroupNo(), 
						(const char*) System.FormatCardNo(atc.GetUserID()));

					sorter.Add(strLabel, nFilePos);
				}
				break;
			}
		}

		if (sorter.GetSize() == 0)
		{
			nReply = nREPORT_NOSOURCE;								// no source data found
		}
		else
		{
			CString strLastCardNo = "x";

			for (int nIndex = 0; nIndex < sorter.GetSize(); nIndex++)
			{
				auditFile.SetFilePos(sorter.GetRecordNo(nIndex));
				if (auditFile.ReadLine(&atc) == TRUE)
				{
					CString strLabel = sorter.GetLabel(nIndex);			// sorted label = PeriodIndex + GroupNo + UserID

					int nPeriodIndex = atoi(strLabel.Left(1));	// Period Index
					int nGroupNo = atc.GetGroupNo();				// GroupNo
					CString strCardNo = atc.GetUserID();				// UserID

					if (strLastCardNo != strCardNo)
					{
						++Sales[nPeriodIndex][nGroupNo].nCardCount;
						strLastCardNo = strCardNo;
					}

					//					++Sales[nPeriodIndex][nGroupNo].nTransCount;

					double dValue = -(atc.GetTransactionPurse1Total());				//* - ( -ve ) == positive )
					Sales[nPeriodIndex][nGroupNo].dPurse1 += dValue;

					dValue = -(atc.GetTransactionPurse2Balance());
					Sales[nPeriodIndex][nGroupNo].dPurse2 += dValue;

					dValue = -(atc.GetTransactionPurse3Total());
					Sales[nPeriodIndex][nGroupNo].dPurse3 += dValue;

					dValue = -(atc.GetTransactionCash());
					Sales[nPeriodIndex][nGroupNo].dOther += dValue;
				}
			}
		}

		auditFile.Close();
	}
	return nReply;
}

//*****************************************************************************
