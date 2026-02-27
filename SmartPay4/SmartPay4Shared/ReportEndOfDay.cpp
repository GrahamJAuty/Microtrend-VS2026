//*****************************************************************************
#include "BackgroundOptions.h"
#include "ClosingBalanceBuilder.h"
#include "DateRangeHelpers.h"
#include "SmartPayLockManager.h"
#include "SQLTable_Account\SQLRepositoryAccount.h"
#include "SQLDefines.h"
#include "SQLSchema\SQLNamespaces.h"
#include "SQLTable_AuditPeriod\SQLRowSetAuditPeriod.h"
#include "SQLTable_ClosingBalance\SQLRecordSetClosingBalance.h"
#include "SQLTable_ClosingBalance\SQLRepositoryClosingBalance.h"
#include "SQLTable_Group\SQLRepositoryGroup.h"
//*****************************************************************************
#include "EODGroupSales.h"
#include "GroupSetText.h"
#include "PasswordData.h"
#include "PaymentAuditReport.h"
#include "SNoDatabase.h"
#include "TLogFile.h"
//*****************************************************************************
#ifndef SYSTEMTYPE_SERVER
//*****************************************************************************
#include "..\SmartPay4ManagerBgnd\WebPaymentExport.h"
//*****************************************************************************
#endif
//*****************************************************************************
#include "ReportEndOfDay.h"
//*****************************************************************************

void CReportEndOfDay::Init()
{
	m_strReportLabel = "R126";
	m_strTitle = "";
	m_bSingleEODRow = FALSE;
	m_nEODReportType = nEOD_VIEW;
	m_bZeroSkipDetail = FALSE;
	m_bZeroSkipVAT = FALSE;
	m_nReportTNoSetNo = 0;
	m_bPerformingEOD = FALSE;

	m_texts.Read();												// get EOD texts
	m_bDefaultZeroSkipDetail = m_texts.IsZeroSkip();
	m_bDefaultZeroSkipVAT = m_texts.IsZeroSkipVAT();

	CString strText = m_texts.GetText(nPURSE3TEXT);
	m_bShowPurse3Column = (strText == "") ? FALSE : TRUE;

	m_strEODSaveFilename = "";
	m_strEODAuditFilename = Filenames.GetEODReportPath("eod0005", "dat");
	m_strEODPaymentAuditFilename = Filenames.GetEODReportPath("eod0053", "dat");
	m_strEODInUseFilename = Filenames.GetReportInUseFilename(m_strReportLabel);

	Reset();
}

//********************************************************************

void CReportEndOfDay::Reset()
{
	m_nReportTNoSetNo = 0;

	m_nDayCounter = 0;								// used to count number of EOD consolidated
	m_strOpeningDate = "";
	m_strOpeningTime = "";
	m_dOpeningPurse1Liability = 0.0;
	m_dOpeningPurse1Credit = 0.0;
	m_dOpeningPurse2Balance = 0.0;
	m_dOpeningPurse3Liability = 0.0;
	m_dOpeningPurse3Credit = 0.0;

	m_strClosingDate = "";
	m_strClosingTime = "";
	m_dClosingPurse1Liability = 0.0;
	m_dClosingPurse1Credit = 0.0;
	m_dClosingPurse2Balance = 0.0;
	m_dClosingPurse3Liability = 0.0;
	m_dClosingPurse3Credit = 0.0;

	m_dDatabasePurse1Liability = 0.0;
	m_dDatabasePurse1Credit = 0.0;
	m_dDatabasePurse3Liability = 0.0;
	m_dDatabasePurse3Credit = 0.0;
}

//***********************************************************************
// create new end of day report ( ALSO CALLED BY BACKGROUND PROCESSOR )

//***********************************************************************
#ifndef SYSTEMTYPE_SERVER
//***********************************************************************

int CReportEndOfDay::EndOfDayReport(const char* szReportFile, bool bPerformEOD)
{
	Reset();

	int nErrorNo = nREPORT_NOERROR;
	CSmartPayAuditFile auditFile(Filenames.GetAuditFilename());
	if ((nErrorNo = auditFile.ValidateFile()) != nREPORT_NOERROR)
	{
		return nErrorNo;
	}
	
	if (auditFile.Open() == FALSE)							// reopen audit file ( can be closed by destructor )
	{
		return nREPORT_OPENERROR;
	}

	// check balance database

	int nReply = nREPORT_NOERROR;								// assume  no error

	CSQLRepositoryClosingBalance RepoClosing;
	CSQLResultInfo infoResult = RepoClosing.CountRows(NULL);

	if (infoResult.GetSQLError() != SQLCRUD_ERR_NONE)
	{
		return nREPORT_OPENERROR;
	}

	if (infoResult.GetSQLResult() == 0)						// no database entries - so make an opening balance for today
	{
		COleDateTime timeNow = COleDateTime::GetCurrentTime();

		COleDateTime timeBalance = COleDateTime(
			timeNow.GetYear(),
			timeNow.GetMonth(),
			timeNow.GetDay(),
			0, 0, 0);

		CDateRangeHelpers DateRangeHelpers;
		DateRangeHelpers.AddDaysToTime(timeBalance, -1);

		CClosingBalanceBuilder BalanceBuilder;
		if (BalanceBuilder.CalculateBalanceForDate( timeBalance, &auditFile) == FALSE)
		{
			nReply = nREPORT_INVALIDDATA;
		}
	}

	// start report

	CTime datetimeOpening;

	CSQLRowClosingBalance LastClosingBalanceRow;

	if (RepoClosing.SelectLastRow(LastClosingBalanceRow,NULL).GetSQLError() != SQLCRUD_ERR_NONE)
	{
		return nREPORT_OPENERROR;
	}

	{
		CSQLResultInfo infoResult2 = RepoClosing.CountRows(NULL);

		if ( (infoResult2.GetSQLError() != SQLCRUD_ERR_NONE) || ( infoResult2.GetSQLResult() < 1 ) )
		{
			return nREPORT_OPENERROR;
		}
		else
		{
			m_bSingleEODRow = (infoResult2.GetSQLResult() == 1);
		}
	}

	if (nReply == nREPORT_NOERROR)
	{
		m_dOpeningPurse1Liability = LastClosingBalanceRow.GetPurse1LiabilityDatabase();
		m_dOpeningPurse1Credit = LastClosingBalanceRow.GetPurse1CreditDatabase();
		m_dOpeningPurse2Balance = 0.0;
		m_dOpeningPurse3Liability = LastClosingBalanceRow.GetPurse3LiabilityDatabase();
		m_dOpeningPurse3Credit = LastClosingBalanceRow.GetPurse3CreditDatabase();
		m_strOpeningDate = LastClosingBalanceRow.GetDate();				// date last balance entry
		m_strOpeningTime = LastClosingBalanceRow.GetTime();				// time last balance entry
		m_strClosingDate = "";									// gets set when report printed
		m_strClosingTime = "";

		{
			CSSDate date(m_strOpeningDate);
			CSSTime time(m_strOpeningTime);

			if ((date.IsSet() == FALSE) || (time.IsSet() == FALSE))
			{
#ifdef SYSTEMTYPE_BACKGROUND
				m_strBackgroundError = "Invalid previous EOD date";
#endif
				nReply = nREPORT_INVALIDDATA;
			}
			else
			{
				datetimeOpening = CTime(date.GetYear(), date.GetMonth(), date.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
			}
		}
	}

	if (nReply == nREPORT_NOERROR)
	{
		bool bOk = TRUE;

		// check cash account history file

		CString strTempCashHistoryFilename = "";

		if (::FileExists(Filenames.GetPurchaseHistoryFilename("0")) == TRUE)	// see if have cash account history file "000000.dat"
		{
			CTLogFile tlog("0");														// cash account history file - will merge in any outstanding
			strTempCashHistoryFilename = Filenames.GetTempCashHistoryFilename();		// temp cash account = "xxxxxx.dat"

			CTime t2;
			t2 = datetimeOpening - CTimeSpan(0, 0, 15, 0);											// 15 minutes before last end of day

			CSSDate dateStart;	
			dateStart.SetDate(t2);
			
			CSSTime timeStart;
			timeStart.SetTime(t2);

			if (tlog.Extract(strTempCashHistoryFilename, dateStart.GetDate(), timeStart.GetTime()) == -1)	// create temp history file for cash account
			{
				nReply = nREPORT_NOSOURCE;												// no previous date to start searhing from
			}
		}

		m_dDatabasePurse1Liability = 0.0;
		m_dDatabasePurse1Credit = 0.0;
		double dDatabasePurse2Balance = 0.0;
		m_dDatabasePurse3Liability = 0.0;
		m_dDatabasePurse3Credit = 0.0;

		{
			COleDateTime timeNow = COleDateTime::GetCurrentTime();

			CString strBalanceFolder = Filenames.GetFilename("EOD\\BALANCES", "");
			::MakeSubdirectory(strBalanceFolder);

			CString strBalanceFilename = "";
			strBalanceFilename.Format("%s\\EODBAL_%4.4d%2.2d%2.2d_%2.2d%2.2d.dat",
				(const char*)strBalanceFolder,
				timeNow.GetYear(),
				timeNow.GetMonth(),
				timeNow.GetDay(),
				timeNow.GetHour(),
				timeNow.GetMinute());

			CSSFile fileBalances;
			fileBalances.Open(strBalanceFilename, "wb");

			{
				CCSV csvHeader;
				csvHeader.Add(Account::UserID.Label);
				csvHeader.Add(Account::Purse1Liability.Label);
				csvHeader.Add(Account::Purse1Credit.Label);
				csvHeader.Add(Account::Purse2Balance.Label);
				csvHeader.Add(Account::Purse3Liability.Label);
				csvHeader.Add(Account::Purse3Credit.Label);
				fileBalances.WriteLine(csvHeader.GetLine());
			}

			CWorkingDlg WorkingDlg("Checking Database Balances", FALSE);
			WorkingDlg.Create();

			CSQLRowAccountFull RowAccount;
			CSQLRecordSetAccountFull RecordSet(NULL, RSParams_AccountFull_NoParams{}, FALSE);
			
			int nRecordCount = 0;
			while (RecordSet.StepSelectAll(RowAccount) == TRUE)
			{
				WorkingDlg.SetCaption2RecordsChecked(nRecordCount++, FALSE);
				m_dDatabasePurse1Liability += RowAccount.GetPurse1Liability();
				m_dDatabasePurse1Credit += RowAccount.GetPurse1Credit();
				dDatabasePurse2Balance += RowAccount.GetPurse2Balance();
				m_dDatabasePurse3Liability += RowAccount.GetPurse3Liability();
				m_dDatabasePurse3Credit += RowAccount.GetPurse3Credit();

				{
					CCSV csvLine;
					csvLine.Add(RowAccount.GetUserID());
					csvLine.Add(RowAccount.GetPurse1Liability(), 2);
					csvLine.Add(RowAccount.GetPurse1Credit(), 2);
					csvLine.Add(RowAccount.GetPurse2Balance(), 2);
					csvLine.Add(RowAccount.GetPurse3Liability(), 2);
					csvLine.Add(RowAccount.GetPurse3Credit(), 2);
					fileBalances.WriteLine(csvLine.GetLine());
				}
			}
		}

		if (nReply == nREPORT_NOERROR)												// see if ok to carry on
		{
			if (m_Report.Open(szReportFile) == TRUE)
			{
				CEODCashlessConsolidator EODCashlessConsolidator(FALSE);										// set not secondary report
				EODCashlessConsolidator.SetNoSalesFilename(LastClosingBalanceRow.GetKeyDate());			// key - opening balance date yyyymmdd

				if (CModuleGlobals::IsBackgroundModule() == TRUE)
				{
					int nRecordsChecked = 0;
					CWorkingDlg WorkingDlg("dummy",FALSE);
					nReply = EODCashlessConsolidator.ConsolidateFromAudit(&auditFile, datetimeOpening, datetimeOpening, m_bSingleEODRow, FALSE, &WorkingDlg,nRecordsChecked);	// show no progress
				}
				else
				{
					int nRecordsChecked = 0;
					CWorkingDlg WorkingDlg("Generating EOD report", TRUE);
					WorkingDlg.Create();
					nReply = EODCashlessConsolidator.ConsolidateFromAudit(&auditFile, datetimeOpening, datetimeOpening, m_bSingleEODRow, FALSE, &WorkingDlg, nRecordsChecked);	// show progress bar
				}

				if (nReply == nREPORT_NOERROR)
				{
					m_nDayCounter = 1;											// number of EOD dates being consolidated
					CreateReport(&EODCashlessConsolidator,TRUE);

					if (bPerformEOD == TRUE)
					{
						{
							COleDateTime timeNow = COleDateTime::GetCurrentTime();
							LastClosingBalanceRow.SetKeyDate(LastClosingBalanceRow.MakeKeyDate(timeNow));
							LastClosingBalanceRow.SetDate(LastClosingBalanceRow.MakeDbDate(timeNow));
							LastClosingBalanceRow.SetTime(LastClosingBalanceRow.MakeDbTime(timeNow));
							LastClosingBalanceRow.SetPurse1LiabilityAudit(m_dClosingPurse1Liability);
							LastClosingBalanceRow.SetPurse1CreditAudit(m_dClosingPurse1Credit);
							LastClosingBalanceRow.SetPurse2BalanceAudit(m_dClosingPurse2Balance);
							LastClosingBalanceRow.SetPurse3LiabilityAudit(m_dClosingPurse3Liability);
							LastClosingBalanceRow.SetPurse3CreditAudit(m_dClosingPurse3Credit);
							LastClosingBalanceRow.SetHaveAdjustFlag(TRUE);
							LastClosingBalanceRow.SetPurse1LiabilityAdjust(m_dDatabasePurse1Liability - m_dClosingPurse1Liability);
							LastClosingBalanceRow.SetPurse1CreditAdjust(m_dDatabasePurse1Credit - m_dClosingPurse1Credit);
							LastClosingBalanceRow.SetPurse3LiabilityAdjust(m_dDatabasePurse3Liability - m_dClosingPurse3Liability);
							LastClosingBalanceRow.SetPurse3CreditAdjust(m_dDatabasePurse3Credit - m_dClosingPurse3Credit);

							RepoClosing.UpsertRow(LastClosingBalanceRow,NULL);
						}

						CSQLAuditRecord atc;
						atc.Set(AUDIT_EODBALANCE, APPNO_BALANCE);

						if (CModuleGlobals::IsBackgroundModule() == TRUE)
						{
							atc.SetOperatorID("BP");							// set operator to Background Processor action
						}
						else
						{
							atc.SetOperatorID(Passwords.GetPasswordInUse());
						}

						atc.SetCurrentPurse1Liability(m_dDatabasePurse1Liability);
						atc.SetCurrentPurse1Credit(m_dDatabasePurse1Credit);
						atc.SetCurrentPurse2Balance(dDatabasePurse2Balance);
						atc.SetCurrentPurse3Liability(m_dDatabasePurse3Liability);
						atc.SetCurrentPurse3Credit(m_dDatabasePurse3Credit);

						atc.SaveNewLine();									// append line to main audit file of current balances

						OtherEODFunctions(&atc);							// do any other End of Day functions
					}

					AddAdditionalInfo();									// Group sales \ Epos payments

					nReply = AddTerminalGrouping(&EODCashlessConsolidator); // add secondary Terminal Grouping sets data
				}

				m_Report.Close();

				if (nReply == nREPORT_NOERROR)
				{
					if (m_nEODReportType != nEOD_VIEW)					// proper EOD - so save report in EOD folder
					{
						m_strEODSaveFilename = Filenames.GetEODReportPath(LastClosingBalanceRow.GetKeyDate());	// key - yyyymmdd
						::CopyFile(szReportFile, m_strEODSaveFilename, FALSE);							// copy to EOD folder
					}

					if (nEOD_EXECUTE == m_nEODReportType)
					{
						CSSFile fileEODDate;
						if (fileEODDate.Open(Filenames.GetEODRefreshDateFilename(), "wb") == TRUE)
						{
							fileEODDate.WriteLine(LastClosingBalanceRow.GetKeyDate());
						}
					}
				}
			}
			else
			{
				nReply = nREPORT_CREATEFAIL;
			}
		}

		CFileRecycler::SendToRecycleBin(strTempCashHistoryFilename);					// delete temp cash history file if found
	}

	auditFile.Close();														// close audit file if still open
	
	return nReply;
}

//*****************************************************************

void CReportEndOfDay::OtherEODFunctions(CSQLAuditRecord* pAtcRecord)
{
	if (System.GetEnableExportSalesFlag() == TRUE)						// see if export sales \ topups required
	{
		CBackgroundOptions options;									// get background options
		options.Read();

		if (options.m_nExportSalesMinutes == 0)					// task being done via background processor
		{
			CWebPaymentExport web(m_pParent);
			web.SpecialSalesExport();
		}
	}

	MakeEODAuditFile();												// copy current audit file (eod0005.dat) to EOD folder for consolidation reporting

	// payment audit ***********************************

	CPaymentAuditRecord paymentAudit;
	paymentAudit.SetOperatorID(pAtcRecord->GetOperatorID());
	paymentAudit.SetTerminalNo(pAtcRecord->GetCCNoFull());
	paymentAudit.SaveEODEntry();

	::CopyFile(Filenames.GetPaymentAuditFilename(), m_strEODPaymentAuditFilename, FALSE);	// copy current audit file to EOD folder
}

//*************************************************************************
#endif
//*************************************************************************

int CReportEndOfDay::AddTerminalGrouping ( CEODCashlessConsolidator* pEODCashlessConsolidator )
{
	int nReply = nREPORT_NOERROR;

	for ( m_nReportTNoSetNo = 1 ; m_nReportTNoSetNo <= 4 ; m_nReportTNoSetNo++ )
	{
		CString strFilename = pEODCashlessConsolidator->GetSecondaryFilename ( m_nReportTNoSetNo );
		if ( ::FileExists ( strFilename ) == TRUE )
		{
			CSmartPayAuditFile auditTmp ( strFilename );
			if ( auditTmp.ValidateFile() == nREPORT_NOERROR )
			{
				if ( auditTmp.Open() == TRUE )
				{
					CEODCashlessConsolidator ConsolidatorTmp ( TRUE );

					if ( CModuleGlobals::IsBackgroundModule() == TRUE )
					{
						CWorkingDlg progress( "dummy", TRUE );
						nReply = ConsolidatorTmp.ConsolidateFromAudit2 ( &auditTmp, &progress );			// show no progress	
					}
					else
					{
						CString strMsg = "";
						strMsg.Format ( "Collating %s EOD Terminal Breakdown", 
							(const char*) GetTerminalSetName(m_nReportTNoSetNo) );

						CWorkingDlg WorkingDlg ( strMsg, TRUE );
						WorkingDlg.Create();

						ConsolidatorTmp.SetNoSalesFilename ( "tmp" );						// temp file to hold no sales purchase adjustment entries if found
						nReply = ConsolidatorTmp.ConsolidateFromAudit2 ( &auditTmp, &WorkingDlg );	// show progress bar
					}

					CreateReport ( &ConsolidatorTmp, FALSE );

					AddPeriodSales ( strFilename, TRUE );								// add period sales - use all of audit file ( no EOD markers )
				}
			}
		}
	}
	return nReply;
}

//*****************************************************************
// Add Period Sales & EPOS Payments to report file

void CReportEndOfDay::AddAdditionalInfo()
{
	CString strAuditFile = Filenames.GetAuditFilename();
	CString strPaymentFile = Filenames.GetPaymentAuditFilename();

	switch (m_nEODReportType)
	{
	case nEOD_REGENERATE_SINGLE:
	case nEOD_REGENERATE_ALL:
	case nEOD_CONSOLIDATE:
		strAuditFile = m_strEODAuditFilename;					// use EOD\eod0005.dat file
		strPaymentFile = m_strEODPaymentAuditFilename;
		break;
	}

	AddPeriodSales(strAuditFile, FALSE);						// don't use full file ( use date range )		
	AddEposPayments(strPaymentFile);							// Epos payments
}

//*****************************************************************************
//*** values added / subtracted from cards in period **************************
//*****************************************************************************

void CReportEndOfDay::CreateReport(CEODCashlessConsolidator* pEODCashlessConsolidator, bool bHaveDatabaseBalance)
{
	bool bIsTerminalSetReport = FALSE;

	m_dClosingPurse1Liability = m_dOpeningPurse1Liability;
	m_dClosingPurse1Credit = m_dOpeningPurse1Credit;
	m_dClosingPurse2Balance = m_dOpeningPurse2Balance;
	m_dClosingPurse3Liability = m_dOpeningPurse3Liability;
	m_dClosingPurse3Credit = m_dOpeningPurse3Credit;

	pEODCashlessConsolidator->CalculateTotals();								// calc purse totals & possible count

	m_dClosingPurse1Liability += pEODCashlessConsolidator->m_CashlessTotals.m_dAdded[CASHLESS_TOTALTYPE_PURSE1LIABILITY];
	m_dClosingPurse1Credit += pEODCashlessConsolidator->m_CashlessTotals.m_dAdded[CASHLESS_TOTALTYPE_PURSE1CREDIT];
	m_dClosingPurse2Balance += pEODCashlessConsolidator->m_CashlessTotals.m_dAdded[CASHLESS_TOTALTYPE_PURSE2];
	m_dClosingPurse3Liability += pEODCashlessConsolidator->m_CashlessTotals.m_dAdded[CASHLESS_TOTALTYPE_PURSE3LIABILITY];
	m_dClosingPurse3Credit += pEODCashlessConsolidator->m_CashlessTotals.m_dAdded[CASHLESS_TOTALTYPE_PURSE3CREDIT];

	m_dClosingPurse1Liability -= pEODCashlessConsolidator->m_CashlessTotals.m_dSubtracted[CASHLESS_TOTALTYPE_PURSE1LIABILITY];
	m_dClosingPurse1Credit -= pEODCashlessConsolidator->m_CashlessTotals.m_dSubtracted[CASHLESS_TOTALTYPE_PURSE1CREDIT];
	m_dClosingPurse2Balance -= pEODCashlessConsolidator->m_CashlessTotals.m_dSubtracted[CASHLESS_TOTALTYPE_PURSE2];
	m_dClosingPurse3Liability -= pEODCashlessConsolidator->m_CashlessTotals.m_dSubtracted[CASHLESS_TOTALTYPE_PURSE3LIABILITY];
	m_dClosingPurse3Credit -= pEODCashlessConsolidator->m_CashlessTotals.m_dSubtracted[CASHLESS_TOTALTYPE_PURSE3CREDIT];

	CString strText = "";
	CString strText2 = "";
	CString strText3 = "";
	CString strLine = "";

	if (m_nReportTNoSetNo == 0)
	{
		AddColumnText();

		strText.Format("<..>Opening Balance ( %s @ %s )",
			(const char*)m_strOpeningDate,
			(const char*)m_strOpeningTime);

		CreateLinePurse1And3(strText, "", "", m_dOpeningPurse1Liability + m_dOpeningPurse1Credit, m_dOpeningPurse3Liability + m_dOpeningPurse3Credit);

		if ( System.GetCreditOptionsFlag() == TRUE)
		{
			CreateLinePurse1And3("<..>Opening Credit", "", "", m_dOpeningPurse1Credit, m_dOpeningPurse3Credit);
			ShowLines(m_bShowPurse3Column ? 3 : 2);
			CreateLinePurse1And3("<..>Opening Liability", "", "", m_dOpeningPurse1Liability, m_dOpeningPurse3Liability);
		}
	}
	else
	{
		bIsTerminalSetReport = TRUE;

		m_Report.WriteLine("<NEWPAGE>");

		strLine.Format("<..>EOD %s Breakdown",
			(const char*)GetTerminalSetName(m_nReportTNoSetNo));

		m_Report.WriteLine(strLine);
	}

	if (System.GetCreditOptionsFlag() == TRUE)
	{
		CreateLiabilityAdditionsSection(pEODCashlessConsolidator, bIsTerminalSetReport);
		CreateCreditAdditionsSection(pEODCashlessConsolidator, bIsTerminalSetReport);
		CreateLiabilityDeductionsSection(pEODCashlessConsolidator, bIsTerminalSetReport);
		CreateCreditDeductionsSection(pEODCashlessConsolidator, bIsTerminalSetReport);
	}
	else
	{
		CreateLiabilityAdditionsSection(pEODCashlessConsolidator, bIsTerminalSetReport);
		CreateLiabilityDeductionsSection(pEODCashlessConsolidator, bIsTerminalSetReport);
	}

	if (FALSE == bIsTerminalSetReport)
	{
		CreateClosingBalanceSection(bHaveDatabaseBalance);
	}

	CreateCashSalesSection(pEODCashlessConsolidator);

	if (m_texts.ExcludeSalesBreakdown() == FALSE)
	{
		CreateSalesBreakdownSection(pEODCashlessConsolidator, bIsTerminalSetReport);
	}
}

//*************************************************************************

void CReportEndOfDay::CreateLiabilityAdditionsSection(CEODCashlessConsolidator* pEODCashlessConsolidator, bool bIsTerminalSetReport )
{
	if (System.GetCreditOptionsFlag() == TRUE)
	{
		ShowColumnHeadings("Liability Additions", "", FALSE, TRUE);
	}
	else
	{
		ShowColumnHeadings("Additions", "", FALSE, TRUE);
	}

	int nCount = 0;

	if (TRUE == bIsTerminalSetReport)
	{
		m_bZeroSkipDetail = FALSE;
	}
	else
	{
		m_bZeroSkipDetail = m_bDefaultZeroSkipDetail;
	}

	if (CreateLinePosting("", m_texts.GetText(nPOSTOPUP), "", pEODCashlessConsolidator, EODENTRY_ECRTOPUP, FALSE, FALSE ) == TRUE )
	{
		++nCount;
	}

	if (bIsTerminalSetReport == FALSE)
	{
		if (CreateLinePosting("", m_texts.GetText(nPOSTOPUPREFUND), "", pEODCashlessConsolidator, EODENTRY_ECRTOPUP, FALSE, TRUE ) == TRUE )
		{
			++nCount;
		}

		if (CreateLinePosting("", m_texts.GetText(nOFFICETOPUP), "", pEODCashlessConsolidator, EODENTRY_PCTOPUP, FALSE, FALSE ) == TRUE )
		{
			++nCount;
		}

		if (CreateLinePosting("", m_texts.GetText(nOFFICETOPUPREFUND), "", pEODCashlessConsolidator, EODENTRY_PCTOPUP, FALSE, TRUE ) == TRUE )
		{
			++nCount;
		}

		if (CreateLinePosting("", m_texts.GetText(nIMPORTTOPUP), "", pEODCashlessConsolidator, EODENTRY_IMPORTTOPUP, FALSE, FALSE) == TRUE)
		{
			++nCount;
		}

		if (CreateLinePosting("", m_texts.GetText(nIMPORTTOPUPREFUND), "", pEODCashlessConsolidator, EODENTRY_IMPORTTOPUP, FALSE, TRUE ) == TRUE )
		{
			++nCount;
		}

		if (CreateLinePosting("", m_texts.GetText(nOFFICEADJUSTMENT), "", pEODCashlessConsolidator, EODENTRY_ADJUST, FALSE, FALSE ) == TRUE )
		{
			++nCount;
		}
	}

	m_bZeroSkipDetail = m_bDefaultZeroSkipDetail;

	if (CreateLinePosting("", m_texts.GetText(nKIOSKTOPUP), "", pEODCashlessConsolidator, EODENTRY_KIOSKADDED, FALSE, FALSE ) == TRUE )
	{
		++nCount;
	}

	if (bIsTerminalSetReport == FALSE)
	{
		if (System.GetWebPaymentType() != nWEBPAYMENT_NONE)
		{
			if (CreateLinePosting("", m_texts.GetText(nWEBPAYMENTTOPUP), "", pEODCashlessConsolidator, EODENTRY_WEBPAYMENT, FALSE, FALSE, FALSE ) == TRUE )
			{
				++nCount;
			}

			if (CreateLinePosting("", m_texts.GetText(nWEBPAYMENTREFUND), "", pEODCashlessConsolidator, EODENTRY_WEBPAYMENT, FALSE, TRUE, FALSE) == TRUE)
			{
				++nCount;
			}
		}

		if (System.GetWebPaymentType() == nWEBPAYMENT_PEBBLE)
		{
			if (CreateLinePosting("", m_texts.GetText(nPEBBLEORDER), "", pEODCashlessConsolidator, EODENTRY_PEBBLEORDER, FALSE, TRUE, FALSE) == TRUE)
			{
				++nCount;
			}

			if (CreateLinePosting("", m_texts.GetText(nPEBBLEORDERREFUND), "", pEODCashlessConsolidator, EODENTRY_PEBBLEORDER, FALSE, FALSE, FALSE) == TRUE)
			{
				++nCount;
			}
		}

		// bonus cash added

		if (CreateLinePosting("", "Redeemed Points Cash", "", pEODCashlessConsolidator, EODENTRY_ADDEDBONUS, FALSE, FALSE ) == TRUE )
		{
			++nCount;
		}

		if (CreateLine1Value("Spend Bonus Cash", "", pEODCashlessConsolidator-> m_EODEntries[EODENTRY_SPENDBONUS].m_dAddPurse1Liability, TRUE) == TRUE)				// is a topup
		{
			++nCount;
		}

		if (CreateLine1Value("Spend Bonus Refund", "", pEODCashlessConsolidator->m_EODEntries[EODENTRY_SPENDBONUS].m_dSubPurse1Liability, TRUE) == TRUE)	// refund a topup
		{
			++nCount;
		}
	}

	if (nCount > 0)
	{
		ShowLines(m_bShowPurse3Column ? 3 : 2);
	}

	m_bZeroSkipDetail = FALSE;

	if (System.GetCreditOptionsFlag() == TRUE)
	{
		CreateLine3Value("", "<..>Total Liability Additions", "",
			pEODCashlessConsolidator->m_CashlessTotals.m_dAdded[CASHLESS_TOTALTYPE_PURSE1LIABILITY],
			pEODCashlessConsolidator->m_CashlessTotals.m_dAdded[CASHLESS_TOTALTYPE_PURSE2],
			pEODCashlessConsolidator->m_CashlessTotals.m_dAdded[CASHLESS_TOTALTYPE_PURSE3LIABILITY],
			FALSE);
	}
	else
	{
		CreateLine3Value("", "<..>Total Additions", "",
			pEODCashlessConsolidator->m_CashlessTotals.m_dAdded[CASHLESS_TOTALTYPE_PURSE1LIABILITY] + pEODCashlessConsolidator->m_CashlessTotals.m_dAdded[CASHLESS_TOTALTYPE_PURSE1CREDIT],
			pEODCashlessConsolidator->m_CashlessTotals.m_dAdded[CASHLESS_TOTALTYPE_PURSE2],
			pEODCashlessConsolidator->m_CashlessTotals.m_dAdded[CASHLESS_TOTALTYPE_PURSE3LIABILITY] + pEODCashlessConsolidator->m_CashlessTotals.m_dAdded[CASHLESS_TOTALTYPE_PURSE3CREDIT],
			FALSE);
	}
}

//*************************************************************************

void CReportEndOfDay::CreateCreditAdditionsSection(CEODCashlessConsolidator* pEODCashlessConsolidator, bool bIsTerminalSetReport)
{
	ShowColumnHeadings("Credit Additions", "", TRUE, TRUE);

	int nCount = 0;
	m_bZeroSkipDetail = (bIsTerminalSetReport == TRUE) ? FALSE : m_bDefaultZeroSkipDetail;

	if (bIsTerminalSetReport == FALSE)
	{
		if (CreateLinePosting("", m_texts.GetText(nIMPORTTOPUP), "", pEODCashlessConsolidator, EODENTRY_IMPORTTOPUP, TRUE, FALSE ) == TRUE )
		{
			++nCount;
		}

		if (CreateLinePosting("", m_texts.GetText(nIMPORTTOPUPREFUND), "", pEODCashlessConsolidator, EODENTRY_IMPORTTOPUP, TRUE, TRUE ) == TRUE)
		{
			++nCount;
		}
	
		if (CreateLinePosting("", m_texts.GetText(nCREDITADJUSTPCUP), "", pEODCashlessConsolidator, EODENTRY_CREDIT, TRUE, FALSE ) == TRUE )
		{
			++nCount;
		}

		if (CreateLinePosting("", m_texts.GetText(nCREDITADJUSTPCDOWN), "", pEODCashlessConsolidator, EODENTRY_CREDIT, TRUE, TRUE ) == TRUE)
		{
			++nCount;
		}

		if (CreateLinePosting("", m_texts.GetText(nOFFICEADJUSTMENT), "", pEODCashlessConsolidator, EODENTRY_ADJUST, TRUE, FALSE ) == TRUE)
		{
			++nCount;
		}

		if (System.GetWebPaymentType() == nWEBPAYMENT_PEBBLE)
		{
			if (CreateLinePosting("", m_texts.GetText(nPEBBLEORDER), "", pEODCashlessConsolidator, EODENTRY_PEBBLEORDER, TRUE, FALSE, FALSE) == TRUE)
			{
				++nCount;
			}

			if (CreateLinePosting("", m_texts.GetText(nPEBBLEORDERREFUND), "", pEODCashlessConsolidator, EODENTRY_PEBBLEORDER, TRUE, TRUE, FALSE) == TRUE)
			{
				++nCount;
			}
		}
	}

	m_bZeroSkipDetail = m_bDefaultZeroSkipDetail;

	if (nCount > 0)
	{
		ShowLines(m_bShowPurse3Column ? 3 : 2);
	}

	m_bZeroSkipDetail = FALSE;
	CreateLinePurse1And3("", "<..>Total Credit Additions", "",
		pEODCashlessConsolidator->m_CashlessTotals.m_dAdded[CASHLESS_TOTALTYPE_PURSE1CREDIT],
		pEODCashlessConsolidator->m_CashlessTotals.m_dAdded[CASHLESS_TOTALTYPE_PURSE3CREDIT]);
}

//*************************************************************************

void CReportEndOfDay::CreateLiabilityDeductionsSection(CEODCashlessConsolidator* pEODCashlessConsolidator, bool bIsTerminalSetReport)
{
	if (System.GetCreditOptionsFlag() == TRUE)
	{
		ShowColumnHeadings("Liability Deductions", "", FALSE, TRUE);
	}
	else
	{
		ShowColumnHeadings("Deductions", "", FALSE, TRUE);
	}

	int nCount = 0;
	m_bZeroSkipDetail = FALSE;

	CGroupSetText groupSet;
	groupSet.Open(Filenames.GetGroupSetFilename());		// readonly

	for (int nGroupSet = 0; nGroupSet <= 9; nGroupSet++)		// 0=unknown, else 1-9
	{
		CEODGroupSale* pEODGroupSale = &(pEODCashlessConsolidator->m_GroupSet[nGroupSet]);

		CString strName = groupSet.GetText(nGroupSet);
		int nHeadCount = pEODGroupSale->m_nHeadCount;

		if (nHeadCount > 0 && strName == "")							// see if have count but no text
		{
			strName.Format("GroupSet %d", nGroupSet);
		}

		if (nHeadCount > 0 || strName != "")
		{
			CString strText3 = "";
			int nPossibleCount = pEODGroupSale->m_nPossibleCount;	// possible accounts that could use it 
			nPossibleCount *= m_nDayCounter;							// multiple by number of days being consolidated

			CString strBuf = "";
			strBuf.Format(" ( %d )", nPossibleCount);	// add possible count to group set
			strName += strBuf;

			strText3.Format("%d", nHeadCount);

			if (nPossibleCount > 0)								// calc % of accounts have were used
			{
				double dValue1 = (double)nHeadCount;
				double dValue2 = (double)nPossibleCount;
				double dPercentage = (dValue1 / dValue2) * 100;

				strBuf.Format(" : %.1lf%%", dPercentage);
				strText3 += strBuf;									// Add % take up to head count
			}

			if (System.GetCreditOptionsFlag() == TRUE)
			{
				if (CreateLine4Value("", strName, strText3,
					pEODGroupSale->m_dPurse1Liability,
					pEODGroupSale->m_dPurse2,
					pEODGroupSale->m_dPurse3Liability,
					pEODGroupSale->m_dCash) == TRUE)
				{
					++nCount;
				}
			}
			else
			{
				if (CreateLine4Value("", strName, strText3,
					pEODGroupSale->m_dPurse1Liability + pEODGroupSale->m_dPurse1Credit,
					pEODGroupSale->m_dPurse2,
					pEODGroupSale->m_dPurse3Liability + pEODGroupSale->m_dPurse3Credit,
					pEODGroupSale->m_dCash) == TRUE)
				{
					++nCount;
				}
			}
		}
	}

	if (nCount > 0)												// see if any group set sales lines shown
	{
		m_Report.WriteLine("");
	}

	m_bZeroSkipDetail = m_bDefaultZeroSkipDetail;

	if (CreateLinePosting("", m_texts.GetText(nPOSSALEREFUND), "", pEODCashlessConsolidator, EODENTRY_ECRSALE, FALSE, TRUE, TRUE, TRUE ) == TRUE )
	{
		++nCount;
	}

	if (bIsTerminalSetReport == FALSE)
	{
		if (CreateLinePosting("", m_texts.GetText(nOFFICESALEADJUSTMENT), "", pEODCashlessConsolidator, EODENTRY_ADJUST, FALSE, TRUE) == TRUE)
		{
			++nCount;
		}

		if (CreateLinePosting("", m_texts.GetText(nOFFICEDELETION), "", pEODCashlessConsolidator, EODENTRY_DELETION, FALSE, TRUE ) == TRUE )
		{
			++nCount;
		}

		if (CreateLinePosting("", m_texts.GetText(nOFFICEMEAL), "", pEODCashlessConsolidator, EODENTRY_MEAL, FALSE, TRUE, TRUE, TRUE ) == TRUE )
		{
			++nCount;
		}

		if (CreateLinePosting("", "Vending", "", pEODCashlessConsolidator, EODENTRY_VENDING, FALSE, TRUE, TRUE, TRUE ) == TRUE )
		{
			++nCount;
		}
	}

	if (nCount > 0)
	{
		ShowLines(m_bShowPurse3Column ? 4 : 3);
	}

	m_bZeroSkipDetail = FALSE;

	if (System.GetCreditOptionsFlag() == TRUE)
	{
		CreateLine4Value("", "<..>Total Liability Deductions", "",
			pEODCashlessConsolidator->m_CashlessTotals.m_dSubtracted[CASHLESS_TOTALTYPE_PURSE1LIABILITY],
			pEODCashlessConsolidator->m_CashlessTotals.m_dSubtracted[CASHLESS_TOTALTYPE_PURSE2],
			pEODCashlessConsolidator->m_CashlessTotals.m_dSubtracted[CASHLESS_TOTALTYPE_PURSE3LIABILITY],
			pEODCashlessConsolidator->m_CashlessTotals.m_dSubtracted[CASHLESS_TOTALTYPE_CASH]);
	}
	else
	{
		CreateLine4Value("", "<..>Total Deductions", "",
			pEODCashlessConsolidator->m_CashlessTotals.m_dSubtracted[CASHLESS_TOTALTYPE_PURSE1LIABILITY] + pEODCashlessConsolidator->m_CashlessTotals.m_dSubtracted[CASHLESS_TOTALTYPE_PURSE1CREDIT],
			pEODCashlessConsolidator->m_CashlessTotals.m_dSubtracted[CASHLESS_TOTALTYPE_PURSE2],
			pEODCashlessConsolidator->m_CashlessTotals.m_dSubtracted[CASHLESS_TOTALTYPE_PURSE3LIABILITY] + pEODCashlessConsolidator->m_CashlessTotals.m_dSubtracted[CASHLESS_TOTALTYPE_PURSE3CREDIT],
			pEODCashlessConsolidator->m_CashlessTotals.m_dSubtracted[CASHLESS_TOTALTYPE_CASH]);
	}
}

//*************************************************************************

void CReportEndOfDay::CreateCreditDeductionsSection(CEODCashlessConsolidator* pEODCashlessConsolidator, bool bIsTerminalSetReport)
{
	ShowColumnHeadings("Credit Deductions", "", TRUE, TRUE );

	int nCount = 0;
	m_bZeroSkipDetail = FALSE;

	CGroupSetText groupSet;
	groupSet.Open(Filenames.GetGroupSetFilename());		// readonly

	for (int nGroupSet = 0; nGroupSet <= 9; nGroupSet++)		// 0=unknown, else 1-9
	{
		CEODGroupSale* pEODGroupSale = &(pEODCashlessConsolidator->m_GroupSet[nGroupSet]);

		CString strName = groupSet.GetText(nGroupSet);
		int nHeadCount = pEODGroupSale -> m_nHeadCount;

		if (nHeadCount > 0 && strName == "")							// see if have count but no text
		{
			strName.Format("GroupSet %d", nGroupSet);
		}

		if (nHeadCount > 0 || strName != "")
		{
			CString strText3 = "";
			int nPossibleCount = pEODGroupSale -> m_nPossibleCount;	// possible accounts that could use it
			nPossibleCount *= m_nDayCounter;										// multiple by number of days being consolidated

			CString strBuf = "";
			strBuf.Format(" ( %d )", nPossibleCount);	// add possible count to group set
			strName += strBuf;

			strText3.Format("%d", nHeadCount);

			if (nPossibleCount > 0)								// calc % of accounts have were used
			{
				double dValue1 = (double)nHeadCount;
				double dValue2 = (double)nPossibleCount;
				double dPercentage = (dValue1 / dValue2) * 100;

				strBuf.Format(" : %.1lf%%", dPercentage);
				strText3 += strBuf;									// Add % take up to head count
			}

			if (CreateLinePurse1And3("", strName, strText3,
				pEODGroupSale->m_dPurse1Credit,
				pEODGroupSale->m_dPurse3Credit) == TRUE)
			{
				++nCount;
			}
		}
	}

	if (nCount > 0)												// see if any group set sales lines shown
	{
		m_Report.WriteLine("");
	}

	m_bZeroSkipDetail = m_bDefaultZeroSkipDetail;

	if (bIsTerminalSetReport == FALSE)
	{
		if (CreateLinePosting("", m_texts.GetText(nOFFICESALEADJUSTMENT), "", pEODCashlessConsolidator, EODENTRY_ADJUST, TRUE, TRUE) == TRUE)
		{
			++nCount;
		}
	
		if (CreateLinePosting("", m_texts.GetText(nOFFICEDELETION), "", pEODCashlessConsolidator, EODENTRY_DELETION, TRUE, TRUE) == TRUE)
		{
			++nCount;
		}

		if (CreateLinePosting("", m_texts.GetText(nOFFICEMEAL), "", pEODCashlessConsolidator, EODENTRY_MEAL, TRUE, TRUE) == TRUE)
		{
			++nCount;
		}

		if (CreateLinePosting("", "Vending", "", pEODCashlessConsolidator, EODENTRY_VENDING, TRUE, TRUE ) == TRUE)
		{
			++nCount;
		}
	}

	if (nCount > 0)
	{
		ShowLines(m_bShowPurse3Column ? 3 : 2);
	}

	m_bZeroSkipDetail = FALSE;
	CreateLinePurse1And3("", "<..>Total Credit Deductions", "",
		pEODCashlessConsolidator->m_CashlessTotals.m_dSubtracted[CASHLESS_TOTALTYPE_PURSE1CREDIT],
		pEODCashlessConsolidator->m_CashlessTotals.m_dSubtracted[CASHLESS_TOTALTYPE_PURSE3CREDIT] );
}

//*************************************************************************

void CReportEndOfDay::CreateClosingBalanceSection(bool bHaveDatabaseBalance)
{
	CString strDate = m_strClosingDate;
	CString strTime = m_strClosingTime;

	if (strDate == "" && strTime == "")
	{
		CSSDate date;
		date.SetCurrentDate();
		CSSTime time;
		time.SetCurrentTime();

		strDate = date.GetDate();
		strTime = time.GetTime();
	}

	CString strPrefix = "";
	strPrefix += "<..>";
	strPrefix += (nEOD_VIEW == m_nEODReportType) ? "Intermediate" : "Closing";

	CString strAudit = " Audit";

	CString strDateSuffix = "";
	strDateSuffix.Format(" Balance ( %s @ %s )",
		(const char*)strDate,
		(const char*)strTime);

	m_bZeroSkipDetail = FALSE;

	bool bShowAdjust = FALSE;
	if (TRUE == bHaveDatabaseBalance)
	{
		if (System.GetCreditOptionsFlag() == TRUE)
		{
			bShowAdjust |= (CompareDoubles(m_dClosingPurse1Liability, m_dDatabasePurse1Liability, 3) != 0);
			bShowAdjust |= (CompareDoubles(m_dClosingPurse1Credit, m_dDatabasePurse1Credit, 3) != 0);
			bShowAdjust |= (CompareDoubles(m_dClosingPurse1Liability, m_dDatabasePurse1Liability, 3) != 0);
			bShowAdjust |= (CompareDoubles(m_dClosingPurse1Credit, m_dDatabasePurse1Credit, 3) != 0);
		}
		else
		{
			bShowAdjust |= (CompareDoubles(m_dClosingPurse1Liability + m_dClosingPurse1Credit, m_dDatabasePurse1Liability + m_dDatabasePurse1Credit, 3) != 0);
			bShowAdjust |= (CompareDoubles(m_dClosingPurse3Liability + m_dClosingPurse3Credit, m_dDatabasePurse3Liability + m_dDatabasePurse3Credit, 3) != 0);
		}
	}

	if (TRUE == bShowAdjust)
	{
		ShowColumnHeadings("Balance Adjustments", "", TRUE, TRUE);

		CreateLinePurse1And3("", "Balance", "",
			(m_dDatabasePurse1Liability - m_dClosingPurse1Liability) + (m_dDatabasePurse1Credit - m_dClosingPurse1Credit),
			(m_dDatabasePurse3Liability - m_dClosingPurse3Liability) + (m_dDatabasePurse3Credit - m_dClosingPurse3Credit));

		if (System.GetCreditOptionsFlag() == TRUE)
		{
			CreateLinePurse1And3("", "Credit", "", m_dDatabasePurse1Credit - m_dClosingPurse1Credit, m_dDatabasePurse3Credit - m_dClosingPurse3Credit);
			ShowLines(m_bShowPurse3Column ? 3 : 2);
			CreateLinePurse1And3("", "Liability", "", m_dDatabasePurse1Liability - m_dClosingPurse1Liability, m_dDatabasePurse3Liability - m_dClosingPurse3Liability);
		}

		m_Report.WriteLine("");
		m_Report.WriteLine("");

		CreateLinePurse1And3(strPrefix + strDateSuffix, "", "", m_dDatabasePurse1Liability + m_dDatabasePurse1Credit, m_dDatabasePurse3Liability + m_dDatabasePurse3Credit);

		if (System.GetCreditOptionsFlag() == TRUE)
		{
			CreateLinePurse1And3(strPrefix + " Credit", "", "", m_dDatabasePurse1Credit, m_dDatabasePurse3Credit);
			ShowLines(m_bShowPurse3Column ? 3 : 2);
			CreateLinePurse1And3(strPrefix + " Liability", "", "", m_dDatabasePurse1Liability, m_dDatabasePurse3Liability);
		}
	}
	else
	{
		m_Report.WriteLine("");
		m_Report.WriteLine("");

		CreateLinePurse1And3(strPrefix + strDateSuffix, "", "", m_dClosingPurse1Liability + m_dClosingPurse1Credit, m_dClosingPurse3Liability + m_dClosingPurse3Credit);

		if (System.GetCreditOptionsFlag() == TRUE)
		{
			CreateLinePurse1And3(strPrefix + " Credit", "", "", m_dClosingPurse1Credit, m_dClosingPurse3Credit);
			ShowLines(m_bShowPurse3Column ? 3 : 2);
			CreateLinePurse1And3(strPrefix + " Liability", "", "", m_dClosingPurse1Liability, m_dClosingPurse3Liability);
		}
	}
}

//*************************************************************************

void CReportEndOfDay::CreateCashSalesSection(CEODCashlessConsolidator* pPosting)
{
	double dTotalSales = 0;

	if (	( pPosting -> m_EODEntries[EODENTRY_ECRSALE].m_dAddPurse0 != 0.0 ) || 
			( pPosting -> m_EODEntries[EODENTRY_ECRSALE].m_dSubPurse0 != 0.0 ) || 
			( pPosting -> m_EODEntries[EODENTRY_ECRSALE].m_dAddCash != 0.0 ) || 
			( pPosting -> m_EODEntries[EODENTRY_ECRSALE].m_dSubCash != 0.0) )
	{
		m_Report.WriteLine("");
		m_Report.WriteLine("");

		CString strLine = "";
		strLine.Format("\t%s Sales",
			(const char*) m_texts.GetText(nCASHTEXT));

		m_Report.WriteLine(strLine);
		m_Report.WriteLine("<LI>");

		int nCount = 0;
		m_bZeroSkipDetail = m_bDefaultZeroSkipDetail;

		if (CreateLine1Value("<..>Account", "", pPosting -> m_EODEntries[EODENTRY_ECRSALE].m_dAddPurse0 ) == TRUE)
		{
			++nCount;
			dTotalSales += pPosting->m_EODEntries[EODENTRY_ECRSALE].m_dAddPurse0;
		}

		if (CreateLine1Value("<..>Account Refund", "", pPosting->m_EODEntries[EODENTRY_ECRSALE].m_dSubPurse0) == TRUE)
		{
			++nCount;
			dTotalSales += pPosting->m_EODEntries[EODENTRY_ECRSALE].m_dSubPurse0;	// as refunds are negative ADD it NOT subtract it
		}

		if (CreateLine1Value("<..>Non Account", "", pPosting->m_EODEntries[EODENTRY_ECRSALE].m_dAddCash) == TRUE)
		{
			++nCount;
			dTotalSales += pPosting->m_EODEntries[EODENTRY_ECRSALE].m_dAddCash;
		}

		if (CreateLine1Value("<..>Non Account Refund", "", pPosting->m_EODEntries[EODENTRY_ECRSALE].m_dSubCash) == TRUE)
		{
			++nCount;
			dTotalSales += pPosting->m_EODEntries[EODENTRY_ECRSALE].m_dSubCash;		// as cash refund is negative ADD it NOT subtract it
		}

		if (nCount > 0)
		{
			m_Report.WriteLine("\t\t\t<LI>");
		}

		m_bZeroSkipDetail = FALSE;
		strLine.Format("\tTotal %s Sales",
			(const char*) m_texts.GetText(nCASHTEXT));

		CreateLine1Value(strLine, "", dTotalSales);
	}
}

//*************************************************************************

bool CReportEndOfDay::ShowTaxLine(double dSalesValue)
{
	bool bResult = TRUE;

	if ( TRUE == m_bZeroSkipVAT)
	{
		if ( ::CompareDoubles(dSalesValue, 0.0, 3 ) == 0 )
		{
			bResult = FALSE;
		}
	}

	return bResult;
}

//*************************************************************************

void CReportEndOfDay::CreateSalesBreakdownSection(CEODCashlessConsolidator* pPosting, bool bIsTerminalSetReport)
{
	m_Report.WriteLine("");
	m_Report.WriteLine("");

	m_Report.WriteLine("\t<..>Sales Breakdown\t\tTotal\tExVAT\tVAT");
	m_Report.WriteLine("<LI>");

	CTaxCodes taxCodes;
	taxCodes.Read();

	double dTotalSales = 0;
	double dTotalExVat = 0;
	double dTotalVat = 0;

	int nCount = 0;
	m_bZeroSkipVAT = (bIsTerminalSetReport == TRUE) ? FALSE : m_bDefaultZeroSkipVAT;
	m_bZeroSkipDetail = FALSE;

	double dSalesValue = 0.0;
	for (int i = 0; i < nMAX_TAXCODES; i++)
	{
		dSalesValue = pPosting->m_dSales[i];
		CString strTaxText = taxCodes.GetTaxText(i);
		::TrimSpacesFromString(strTaxText);			// lose leading\trailing spaces

		if ( ShowTaxLine( dSalesValue) == TRUE)
		{
			CString strText2 = "";
			strText2.Format("<..>TAX %c ( %s )", i + 'A',
				(const char*)taxCodes.GetTaxText(i));

			CString strText3 = "";
			strText3.Format("%.2lf%%", taxCodes.GetTaxRate(i));

			double dVat = taxCodes.CalcVatContent(dSalesValue, i);
			double dExVat = dSalesValue - dVat;
			if (CreateLine3Value("", strText2, strText3, dSalesValue, dExVat, dVat, TRUE) == TRUE)
			{
				++nCount;
			}

			dTotalSales += dSalesValue;
			dTotalExVat += dExVat;
			dTotalVat += dVat;
		}
	}

	dSalesValue = pPosting->m_dMealSales;
	if (ShowTaxLine(dSalesValue) == TRUE)
	{
		dTotalSales += dSalesValue;
		dTotalExVat += dSalesValue;
		//		dTotalVat   += 0;						// unknown has no VAT Code

		if (CreateLine3Value("", m_texts.GetText(nOFFICEMEAL), "", dSalesValue, dSalesValue, 0, TRUE) == TRUE)
		{
			++nCount;
		}
	}

	dSalesValue = pPosting->m_dVendingSales;
	if (ShowTaxLine(dSalesValue) == TRUE)
	{
		dTotalSales += dSalesValue;
		dTotalExVat += dSalesValue;
		//		dTotalVat   += 0;						// unknown has no VAT Code

		if (CreateLine3Value("", "Vending", "", dSalesValue, dSalesValue, 0, TRUE) == TRUE)
		{
			++nCount;
		}
	}

	dSalesValue = pPosting->m_dSalesUnknown;
	if (ShowTaxLine(dSalesValue) == TRUE)
	{
		dTotalSales += dSalesValue;
		dTotalExVat += dSalesValue;
		//		dTotalVat   += 0;						// unknown has no VAT Code ( eg Discount )

		if (CreateLine3Value("", "Adjustment (no TAX)", "", dSalesValue, dSalesValue, 0, TRUE) == TRUE)
		{
			++nCount;
		}
	}

	dSalesValue = pPosting->m_dNotFoundSales;
	if (ShowTaxLine(dSalesValue) == TRUE)
	{
		dTotalSales += dSalesValue;
		dTotalExVat += dSalesValue;
		//		dTotalVat   += 0;						// No purchase history line so don't know what VAT would be

		if (CreateLine3Value("", "Adjustment (Sale)", "", dSalesValue, dSalesValue, 0, TRUE) == TRUE)
		{
			++nCount;
		}
	}

	if (nCount > 0)
	{
		ShowLines(3);
	}

	CreateLine3Value("", "\tTotal Sales", "", dTotalSales, dTotalExVat, dTotalVat, TRUE);
}

//*************************************************************************
// Extract sales from either full audit File or temp TerminalGroupAudit file

void CReportEndOfDay::AddPeriodSales ( const char* szAuditFile, bool bUseFullFile )
{
	if (m_texts.ExcludePeriodSales() == TRUE)
	{
		return;
	}

	CString strText2 = "";
	CString strText3 = "";

	CEODGroupSales PeriodSales ( szAuditFile );
	
	int nError;
	if (bUseFullFile == TRUE)
	{
		nError = PeriodSales.Extract();			// use all of temp file ( no eod markers )
	}
	else
	{
		nError = PeriodSales.Extract(m_strOpeningDate, m_strOpeningTime, m_strClosingDate, m_strClosingTime);
	}

	if ( nError == nREPORT_NOERROR )
	{
		m_bZeroSkipDetail = m_bDefaultZeroSkipDetail;
		int nPeriodCount = 0;
		int nColumns = m_bShowPurse3Column ? 4 : 3;			// have cash column
			
		m_Report.WriteLine ( "" );
		ShowColumnHeadings ( "Period Group Sales", "UserCount", FALSE, FALSE );

		EODSale Totals[100]{};
		for (int i = 0; i <= 99; i++)
		{
			Totals[i].nCardCount = 0;
			Totals[i].dPurse1 = 0;
			Totals[i].dPurse2 = 0;
			Totals[i].dPurse3 = 0;
			Totals[i].dOther = 0;
		}

		for (int nPeriodIndex = 0; nPeriodIndex < nMAXPERIODS; nPeriodIndex++)
		{
			bool bHaveSales = FALSE;
			for (int nGroupNo = 1; nGroupNo <= 99; nGroupNo++)
			{
				if (PeriodSales.Sales[nPeriodIndex][nGroupNo].nCardCount > 0)
				{
					bHaveSales = TRUE;
					break;
				}
			}

			if (bHaveSales == TRUE)
			{
				EODSale PeriodTotal{};
				PeriodTotal.nCardCount = 0;
				PeriodTotal.dPurse1 = 0;
				PeriodTotal.dPurse2 = 0;
				PeriodTotal.dPurse3 = 0;
				PeriodTotal.dOther = 0;

				for (int nGroupNo = 1; nGroupNo <= 99; nGroupNo++)
				{
					CSQLRowGroupFull RowGroup;
					RowGroup.SetGroupNo(nGroupNo);
					
					CSQLRepositoryGroup RepoGroup;
					if (RepoGroup.SelectRow(RowGroup, NULL).GetSQLError() == SQLCRUD_ERR_NONE )
					{
						int nCardCount = PeriodSales.Sales[nPeriodIndex][nGroupNo].nCardCount;
						double dPurse1 = PeriodSales.Sales[nPeriodIndex][nGroupNo].dPurse1;
						double dPurse2 = PeriodSales.Sales[nPeriodIndex][nGroupNo].dPurse2;
						double dPurse3 = PeriodSales.Sales[nPeriodIndex][nGroupNo].dPurse3;
						double dOther = PeriodSales.Sales[nPeriodIndex][nGroupNo].dOther;

						strText2.Format("<..>%s", 
							(const char*) RowGroup.GetGroupName());

						strText3.Format("%d", nCardCount);
						CreateLine4Value("", strText2, strText3, dPurse1, dPurse2, dPurse3, dOther);

						PeriodTotal.nCardCount += nCardCount;
						PeriodTotal.dPurse1 += dPurse1;
						PeriodTotal.dPurse2 += dPurse2;
						PeriodTotal.dPurse3 += dPurse3;
						PeriodTotal.dOther += dOther;

						Totals[nGroupNo].nCardCount += nCardCount;
						Totals[nGroupNo].dPurse1 += dPurse1;
						Totals[nGroupNo].dPurse2 += dPurse2;
						Totals[nGroupNo].dPurse3 += dPurse3;
						Totals[nGroupNo].dOther += dOther;

						Totals[0].nCardCount += nCardCount;
						Totals[0].dPurse1 += dPurse1;
						Totals[0].dPurse2 += dPurse2;
						Totals[0].dPurse3 += dPurse3;
						Totals[0].dOther += dOther;
					}
				}

				ShowLines(nColumns, TRUE);						// count column totals reqd

				strText2.Format("<..>%s Period", 
					(const char*) SQLRowSetAuditPeriod.GetPeriodName(nPeriodIndex));

				strText3.Format("%d", PeriodTotal.nCardCount);

				CreateLine4Value("", strText2, strText3, PeriodTotal.dPurse1, PeriodTotal.dPurse2, PeriodTotal.dPurse3, PeriodTotal.dOther);

				m_Report.WriteLine("");
				++nPeriodCount;
			}
		}

		if ( nPeriodCount > 1 )
		{
			for ( int nGroupNo = 1 ; nGroupNo <= 99 ; nGroupNo++ )
			{
				CSQLRowGroupFull RowGroup;
				RowGroup.SetGroupNo(nGroupNo);

				CSQLRepositoryGroup RepoGroup;
				if ( RepoGroup.SelectRow( RowGroup, NULL).GetSQLError() == SQLCRUD_ERR_NONE )
				{
					strText2.Format ( "<..>%s Total",
						(const char*) RowGroup.GetGroupName() );

					strText3.Format ( "%d", Totals[nGroupNo].nCardCount );

					CreateLine4Value( "", strText2, strText3, Totals[nGroupNo].dPurse1, Totals[nGroupNo].dPurse2, Totals[nGroupNo].dPurse3, Totals[nGroupNo].dOther );
				}
			}

			ShowLines ( nColumns, TRUE );							// count column totals reqd
			strText2.Format ( "<..>Total Group Sales" );
			strText3.Format ( "%d", Totals[0].nCardCount );

			CreateLine4Value( "", strText2, strText3, Totals[0].dPurse1, Totals[0].dPurse2, Totals[0].dPurse3, Totals[0].dOther );
		}
	}
}

//**************************************************************************************

void CReportEndOfDay::AddEposPayments ( const char* szAuditFile )
{
	if (m_texts.ExcludeEposPayments() == TRUE)
	{
		return;
	}

	CPaymentAuditReport eposPayment(szAuditFile, m_pParent);
	if (eposPayment.EODPayments(m_strOpeningDate, m_strOpeningTime, m_strClosingDate, m_strClosingTime) == TRUE)
	{
		m_Report.WriteLine("");
		ShowColumnHeadings("Epos Payment Summary", "", FALSE, FALSE);

		CEPosPaymentGroupSale Totals{};

		for (int i = 0; i < eposPayment.m_GroupSale.GetSize(); i++)
		{
			CEPosPaymentGroupSale GroupSale;
			eposPayment.m_GroupSale.GetAt(i, GroupSale);

			CString strText = "";
			strText.Format("<..>(G%2.2d) %s",
				GroupSale.m_nGroupNo,
				(const char*) GroupSale.m_strGroupText);

			CreateLine4Value("", strText, "", GroupSale.m_dPurse1, GroupSale.m_dPurse2, GroupSale.m_dPurse3, GroupSale.m_dOther);

			Totals.Add(GroupSale);
		}

		int nColumns = (m_bShowPurse3Column == TRUE) ? 4 : 3;				// have cash column
		ShowLines(nColumns);

		CreateLine4Value("", "Total Epos Payments", "", Totals.m_dPurse1, Totals.m_dPurse2, Totals.m_dPurse3, Totals.m_dOther);
	}
}

//**********************************************************************

void CReportEndOfDay::AddColumnText()
{
	m_Report.SetStyle1 ( m_strTitle );

	m_Report.AddColumn ( "", TA_LEFT, 100 );
	m_Report.AddColumn ( "", TA_LEFT, 400 );
	m_Report.AddColumn ( "", TA_RIGHT, 230 );
	m_Report.AddColumn ( m_texts.GetText(nPURSE1TEXT), TA_RIGHT, 250 );
	m_Report.AddColumn ( m_texts.GetText(nPURSE2TEXT), TA_RIGHT, 250 );
	if ( m_bShowPurse3Column == TRUE ) 	m_Report.AddColumn ( m_texts.GetText(nPURSE3TEXT), TA_RIGHT, 250 );
	m_Report.AddColumn ( "", TA_RIGHT, 250 );			// cash
}

//*****************************************************************************

void CReportEndOfDay::ShowLines ( int nColumns, bool bExtraReqd )
{
	CCSV csv ( '\t' );

	csv.Add ( "" );
	csv.Add ( "" );

	if (bExtraReqd == TRUE)
	{
		csv.Add("<LI>");
	}
	else
	{
		csv.Add("");
	}

	for (int n = 0; n < nColumns; n++)
	{
		csv.Add("<LI>");
	}

	m_Report.WriteLine ( csv );
}

//*****************************************************************************

void CReportEndOfDay::ShowColumnHeadings(const char* szText1, const char* szText2, bool bNoPurse2Text, bool bNoCashText)
{
	CString strBuf = "";

	CString strLine = "";

	strLine.Format("\t<..>%s\t%s\t%s\t",
		szText1,
		szText2,
		(const char*)m_texts.GetText(nPURSE1TEXT));

	if (FALSE == bNoPurse2Text)
	{
		strLine += m_texts.GetText(nPURSE2TEXT);
	}

	if (m_bShowPurse3Column == TRUE)
	{
		strBuf.Format("\t%s",
			(const char*)m_texts.GetText(nPURSE3TEXT));

		strLine += strBuf;
	}

	if (FALSE == bNoCashText)
	{
		strBuf.Format("\t%s",
			(const char*)m_texts.GetText(nCASHTEXT));

		strLine += strBuf;
	}

	m_Report.WriteLine("");
	m_Report.WriteLine("");
	m_Report.WriteLine(strLine);
	m_Report.WriteLine("<LI>");
}

//*****************************************************************************

bool CReportEndOfDay::CreateLineInternal(const char* szText1, const char* szText2, const char* szText3, double dValue1, double dValue2, double dValue3, double dValue4, bool bZeroSkip, bool bNoPurse2, bool bNoPurse3, bool bUseValue2, bool bUseValue3, bool bUseValue4)
{
	if (m_bZeroSkipDetail == TRUE)
	{
		if ( ( dValue1 == 0.0 ) && ( dValue2 == 0.0 ) && (dValue3 == 0.0) && (dValue4 == 0.0) )
		{
			return FALSE;
		}
	}

	CCSV csv('\t');

	csv.Add(szText1);
	csv.Add(szText2);
	csv.Add(szText3);
	csv.Add(dValue1, System.GetDPValue());

	if (TRUE == bUseValue2)
	{
		if (FALSE == bNoPurse2)
		{
			csv.Add(dValue2, System.GetDPValue());
		}
		else
		{
			csv.Add(szNODATA);
		}
	}

	if (TRUE == bUseValue3)
	{
		if (FALSE == bNoPurse3)
		{
			csv.Add(dValue3, System.GetDPValue());
		}
		else
		{
			csv.Add(szNODATA);
		}
	}

	if (TRUE == bUseValue4)
	{
		csv.Add(dValue4, System.GetDPValue());
	}

	m_Report.WriteLine(csv);
	return TRUE;
}

//*****************************************************************************

bool CReportEndOfDay::CreateLinePosting(const char* szText1, const char* szText2, const char* szText3, CEODCashlessConsolidator* pEODCashlessConsolidator, int nActionType, bool bCredit, bool bNegative, bool bShowColumn3, bool bShowColumn4)
{
	double dPurse1Value = 0.0;
	double dPurse2Value = 0.0;
	double dPurse3Value = 0.0;
	double dPurse0Value = 0.0;

	if ((nActionType >= 0) && (nActionType < EODENTRY_TYPE_COUNT))
	{
		if (FALSE == bNegative)
		{
			if (FALSE == bCredit)
			{
				dPurse0Value = pEODCashlessConsolidator->m_EODEntries[nActionType].m_dAddPurse0;
				dPurse1Value = pEODCashlessConsolidator->m_EODEntries[nActionType].m_dAddPurse1Liability;
				dPurse2Value = pEODCashlessConsolidator->m_EODEntries[nActionType].m_dAddPurse2;
				dPurse3Value = pEODCashlessConsolidator->m_EODEntries[nActionType].m_dAddPurse3Liability;
			
				if (System.GetCreditOptionsFlag() == FALSE)
				{
					dPurse1Value += pEODCashlessConsolidator->m_EODEntries[nActionType].m_dAddPurse1Credit;
					dPurse3Value += pEODCashlessConsolidator->m_EODEntries[nActionType].m_dAddPurse3Credit;
				}
			}
			else
			{
				dPurse1Value = pEODCashlessConsolidator->m_EODEntries[nActionType].m_dAddPurse1Credit;
				dPurse2Value = 0.0;
				dPurse3Value = pEODCashlessConsolidator->m_EODEntries[nActionType].m_dAddPurse3Credit;
			}
		}
		else
		{
			if (FALSE == bCredit)
			{
				dPurse1Value = pEODCashlessConsolidator->m_EODEntries[nActionType].m_dSubPurse1Liability;
				dPurse2Value = pEODCashlessConsolidator->m_EODEntries[nActionType].m_dSubPurse2;
				dPurse3Value = pEODCashlessConsolidator->m_EODEntries[nActionType].m_dSubPurse3Liability;
			
				if (System.GetCreditOptionsFlag() == FALSE)
				{
					dPurse1Value += pEODCashlessConsolidator->m_EODEntries[nActionType].m_dSubPurse1Credit;
					dPurse3Value += pEODCashlessConsolidator->m_EODEntries[nActionType].m_dSubPurse3Credit;
				}
			}
			else
			{
				dPurse1Value = pEODCashlessConsolidator->m_EODEntries[nActionType].m_dSubPurse1Credit;
				dPurse2Value = 0.0;
				dPurse3Value = pEODCashlessConsolidator->m_EODEntries[nActionType].m_dSubPurse3Credit;
			}
		}
	}

	return CreateLineInternal(
		szText1,
		szText2,
		szText3,
		dPurse1Value,
		dPurse2Value,
		dPurse3Value,
		dPurse0Value,
		m_bZeroSkipDetail,
		bCredit,									//PURSE2 NOT RELEVANT FLAG
		FALSE,										//PURSE3 NOT RELEVANT FLAG
		TRUE,										//SHOW VALUE 2 COLUMN
		bShowColumn3 && m_bShowPurse3Column,		//SHOW VALUE 3 COLUMN
		bShowColumn4);								//SHOW VALUE 4 COLUMN
}

//*****************************************************************************

bool CReportEndOfDay::CreateLinePurse1And3(const char* szText1, const char* szText2, const char* szText3, double dPurse1Value, double dPurse3Value)
{
	return CreateLineInternal(
		szText1,
		szText2,
		szText3,
		dPurse1Value,
		0.0,
		dPurse3Value,
		0.0,
		m_bZeroSkipDetail,
		TRUE,										//PURSE2 NOT RELEVANT FLAG
		FALSE,										//PURSE3 NOT RELEVANT FLAG
		TRUE,										//SHOW VALUE 2 COLUMN
		m_bShowPurse3Column,						//SHOW VALUE 3 COLUMN
		FALSE);										//SHOW VALUE 4 COLUMN
}

//*****************************************************************************

bool CReportEndOfDay::CreateLine1Value ( const char* szText2, const char* szText3, double dPurse1Value, bool bIsTopup )
{
	return CreateLineInternal(
		"",
		szText2,
		szText3,
		dPurse1Value,
		0.0,
		0.0,
		0.0,
		m_bZeroSkipDetail,
		TRUE,										//PURSE2 NOT RELEVANT FLAG
		TRUE,										//PURSE3 NOT RELEVANT FLAG
		bIsTopup,									//SHOW VALUE 2 COLUMN
		bIsTopup && m_bShowPurse3Column,			//SHOW VALUE 3 COLUMN
		FALSE);										//SHOW VALUE 4 COLUMN
}

//*****************************************************************************

bool CReportEndOfDay::CreateLine3Value(const char* szText1, const char* szText2, const char* szText3, double dPurse1Value, double dPurse2Value, double dPurse3Value, bool bForceValue3)
{
	return CreateLineInternal(
		szText1,
		szText2,
		szText3,
		dPurse1Value,
		dPurse2Value,
		dPurse3Value,
		0.0,
		m_bZeroSkipDetail,
		FALSE,										//PURSE2 NOT RELEVANT FLAG
		FALSE,										//PURSE3 NOT RELEVANT FLAG
		TRUE,										//SHOW VALUE 2 COLUMN
		m_bShowPurse3Column || bForceValue3,		//SHOW VALUE 3 COLUMN
		FALSE);										//SHOW VALUE 4 COLUMN
}

//*****************************************************************************

bool CReportEndOfDay::CreateLine4Value(const char* szText1, const char* szText2, const char* szText3, double dPurse1Value, double dPurse2Value, double dPurse3Value, double dCashValue)
{
	return CreateLineInternal(
		szText1,
		szText2,
		szText3,
		dPurse1Value,
		dPurse2Value,
		dPurse3Value,
		dCashValue,
		m_bZeroSkipDetail,
		FALSE,										//PURSE2 NOT RELEVANT FLAG
		FALSE,										//PURSE3 NOT RELEVANT FLAG
		TRUE,										//SHOW VALUE 2 COLUMN
		m_bShowPurse3Column,						//SHOW VALUE 3 COLUMN
		TRUE);										//SHOW VALUE 4 COLUMN
}

//*************************************************************************

CString CReportEndOfDay::CheckEODAuditFile()
{
	CString strFilename = m_strEODAuditFilename;
	if ( ::FileExists ( strFilename ) == FALSE )					// see has previous saved EOD auditfile
	{
		CSmartPayLockRequest LockRequest;
		LockRequest.m_bWantLockBgnd = TRUE;
		LockRequest.m_bWantLockServerInner = TRUE;

		CSmartPayLockManager LockManager;
		int nLockError = LockManager.AcquireLocks(LockRequest);

		if ( LOCKMANAGER_OK == nLockError )							// no - need to create it so pause server \ background
		{
			MakeEODAuditFile();										// copy current audit  fiel to EOD folder for consolidation repporting
		}
		else
		{
			strFilename = "";										// pause error
		}
	}
	return strFilename;
}

//*************************************************************************

void CReportEndOfDay::MakeEODAuditFile()
{
	::CopyFile ( Filenames.GetAuditFilename(), m_strEODAuditFilename, FALSE );	// copy current audit file to EOD folder
}

//*************************************************************************

CString CReportEndOfDay::GetTerminalSetName ( int nSetNo )
{
	CString strName = "";

	CSNoDatabase db;
	if ( db.OpenForSharing() == DB_ERR_NONE )
	{
		if (db.FindRecord(nSetNo) == TRUE)
		{
			strName = db.GetName();
		}

		db.Close();
	}

	if (strName == "")
	{
		strName.Format("Set %d", nSetNo);
	}

	return strName;
}

//*************************************************************************

CString CReportEndOfDay::SetEODInProgress()
{
	CString strError = "";

	int nRetries = 1;
	if ((m_bPerformingEOD = m_fileEODInUse.Open(m_strEODInUseFilename, "wb", _SH_DENYRW, nRetries)) == FALSE)
	{
		strError = "EOD in progress";
	}

	return strError;
}

//*************************************************************************

void CReportEndOfDay::ClearEODInProgress()
{
	if ( m_bPerformingEOD == TRUE )
	{
		m_fileEODInUse.Close();
		CFileRecycler::SendToRecycleBin ( m_strEODInUseFilename );
		m_bPerformingEOD = FALSE;
	}
}

//*************************************************************************

void CReportEndOfDay::GetEODDateInfo(CArray<CEODListInfo, CEODListInfo>& arrayEODListInfo, int& nTotalRows, int nOffset, int nMaxCount)
{
	arrayEODListInfo.RemoveAll();
	nTotalRows = 0;

	CSQLRecordSetClosingBalance RecordSet(NULL, RSParams_ClosingBalance_NormalNoParams{});
	RecordSet.m_strSort = ClosingBalance::KeyDate.Label;

	bool bFirstRow = TRUE;
	CSQLRowClosingBalance RowClosing;
	while (RecordSet.StepSelectAll(RowClosing) == TRUE)
	{
		if (FALSE == bFirstRow)
		{
			if ( nTotalRows >= nOffset )
			{
				if ( ( nMaxCount < 0 ) || ( arrayEODListInfo.GetSize() < nMaxCount ) )
				{
					CEODListInfo ListInfo;
					ListInfo.m_strKeyDate = RowClosing.GetKeyDate();
					ListInfo.m_strDisplayDate = RowClosing.GetDate();
					ListInfo.m_strDisplayTime = RowClosing.GetTime();
					ListInfo.m_dPurse1 = RowClosing.GetPurse1LiabilityDatabase() + RowClosing.GetPurse1CreditDatabase();
					ListInfo.m_dPurse3 = RowClosing.GetPurse3LiabilityDatabase() + RowClosing.GetPurse3CreditDatabase();
					ListInfo.m_bArchived = RowClosing.IsArchived();
					arrayEODListInfo.Add(ListInfo);
				}
			}
			
			++nTotalRows;
		}

		bFirstRow = FALSE;
	}
}

//*************************************************************************
