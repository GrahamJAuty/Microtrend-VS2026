//*****************************************************************************
#include "..\SmartPay4Shared\SmartPayLockManager.h"
#include "..\SmartPay4Shared\RepmanHandler.h"
#include "..\SmartPay4Shared\ReportEndOfDay.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
#include "..\SmartPay4Shared\SQLTable_ClosingBalance\SQLRepositoryClosingBalance.h"
#include "..\SmartPay4Shared\SQLTable_ClosingBalance\SQLRecordSetClosingBalance.h"
#include "..\SmartPay4Shared\TLogFile.h"
#include "..\SmartPay4Shared\WorkingDlg.h"
//*****************************************************************************
#include "EODListDlg.h"
//*****************************************************************************

CReportEndOfDay::CReportEndOfDay ( CWnd* pParent )
{
	m_pParent = pParent;
	Init();
}
	
//*****************************************************************************
// called from main menu ( NOT CALLED BY BACKGROUND PROCESSOR )

void CReportEndOfDay::Show()
{
	CEODListDlg dlg{};

	while (dlg.DoModal() != IDCANCEL)
	{
		m_nEODReportType = dlg.m_nEODType;
		m_strTitle = "EOD Report";

		switch (m_nEODReportType)
		{
		case nEOD_VIEW:
			m_strTitle = "Intermediate EOD Report";
			break;

		case nEOD_CONSOLIDATE:
			m_strTitle = "EOD Consolidation" + dlg.GetDateString();
			break;

		case nEOD_EXECUTE:
		case nEOD_REPRINT:
		case nEOD_REGENERATE_SINGLE:
		case nEOD_REGENERATE_ALL:
		default:
			break;
		}
		//
		int nErrorNo = nREPORT_NOERROR;
		bool bExitReqd = FALSE;

		CString strReportFile = Filenames.GetReportFilename(m_strReportLabel);
		CString strParamsFile = Filenames.GetReportParamsFilename(m_strReportLabel);

		CRepmanHandler repman(m_strTitle, m_pParent);

		if (m_nEODReportType == nEOD_REPRINT)							// see if a reprint
		{
			strReportFile = dlg.m_strReprintFilename;					// report file to reprint
			m_strEODSaveFilename = strReportFile;
		}
		else
		{
			CString strMsg = SetEODInProgress();
			if (strMsg == "")
			{
				CWaitCursor cursor;

				switch (m_nEODReportType)
				{
				case nEOD_REGENERATE_SINGLE:
					nErrorNo = RegenerateReport(dlg.GetReprintDate(0), strReportFile, TRUE);
					break;

				case nEOD_REGENERATE_ALL:
					{
						nErrorNo = 0;
						for (int n = 0; (n < dlg.GetReprintDateCount()) && (0 == nErrorNo); n++)
						{
							nErrorNo = RegenerateReport(dlg.GetReprintDate(n), strReportFile, FALSE);
						}
					}
					break;

				case nEOD_CONSOLIDATE:
					nErrorNo = ConsolidateReport(dlg.m_strKeyDateFrom, dlg.m_strKeyDateTo, strReportFile);
					break;

				case nEOD_EXECUTE:
					{
						CSmartPayLockRequest Request;
						Request.m_bWantLockBgnd = TRUE;
						Request.m_bWantLockServerInner = TRUE;
						
						CSmartPayLockManager LockManager;
						int nLockError = LockManager.AcquireLocks(Request, m_pParent);

						if (LOCKMANAGER_OK == nLockError)
						{
							if ((nErrorNo = EndOfDayReport(strReportFile, TRUE)) == nREPORT_NOERROR)
							{
								bExitReqd = TRUE;
							}
						}
						else
						{
							nErrorNo = nREPORT_PAUSEERROR;
						}
					}
					break;

				case nEOD_VIEW:
				case nEOD_REPRINT:
				default:
					nErrorNo = EndOfDayReport(strReportFile, FALSE);
					break;
				}

				ClearEODInProgress();
			}
			else
			{
				Prompter.Info("End of Day information is currently being used by another process!", "Function Temporarily Suspended");
				nErrorNo = nREPORT_ERRORDISPLAYED;
			}
		}

		// show report

		if (nErrorNo == nREPORT_NOERROR)
		{
			if (m_strEODSaveFilename != "")
			{
				repman.SetupSaveAs(m_texts.GetSaveAsType(), m_texts.GetSaveAsFilename(m_strEODSaveFilename));		// nSAVEAS_NONE / nSAVEAS_CSV / nSAVEAS_HTML
			}

			nErrorNo = repman.DisplayReport(strReportFile, strParamsFile, m_strReportLabel);
		}

		repman.DisplayError(nErrorNo);

		if (bExitReqd == TRUE)								// exit if just done EOD routine
		{
			break;
		}
	}
}

//*****************************************************************
// Regenerate a report ( not in background )

int CReportEndOfDay::RegenerateReport(CString strDate, CString strReportFile, bool bSingle)
{
	Reset();

	CString strEODAuditFile = CheckEODAuditFile();					// Get previous saved EOD auditfile ( create one if not found )
	if (strEODAuditFile == "")
	{
		return nREPORT_PAUSEERROR;
	}

	int nErrorNo = nREPORT_NOERROR;
	CSmartPayAuditFile auditFile(strEODAuditFile);
	if ((nErrorNo = auditFile.ValidateFile()) != nREPORT_NOERROR)
	{
		return nErrorNo;
	}

	CSQLRowClosingBalance RowClosing;
	RowClosing.SetKeyDate(strDate);

	CSQLRepositoryClosingBalance Repo;
	if (Repo.SelectRow(RowClosing, NULL).GetSQLError() != SQLCRUD_ERR_NONE)
	{
		return nREPORT_NOSOURCE;
	}

	CSSDate dateClosing(RowClosing.GetDate());					// get balance date
	m_strClosingDate = dateClosing.GetDate();

	CSSTime timeClosing(RowClosing.GetTime());					// get balance time
	m_strClosingTime = timeClosing.GetTime();

	int nReply = nREPORT_NOERROR;									// assume no error

	CString strDateKeyClosing = RowClosing.GetKeyDate();

	bool bHaveAdjust = RowClosing.GetHaveAdjustFlag();

	if (TRUE == bHaveAdjust)
	{
		m_dDatabasePurse1Liability = RowClosing.GetPurse1LiabilityDatabase();
		m_dDatabasePurse1Credit = RowClosing.GetPurse1CreditDatabase();
		m_dDatabasePurse3Liability = RowClosing.GetPurse3LiabilityDatabase();
		m_dDatabasePurse3Credit = RowClosing.GetPurse3CreditDatabase();
	}
	else
	{
		m_dDatabasePurse1Liability = 0.0;
		m_dDatabasePurse1Credit = 0.0;
		m_dDatabasePurse3Liability = 0.0;
		m_dDatabasePurse3Credit = 0.0;
	}

	CSQLRowClosingBalance RowOpening;
	RowOpening.SetKeyDate(strDateKeyClosing);

	if (Repo.SelectPreviousRow(RowOpening, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
	{
		CString strDateKeyOpening = RowOpening.GetKeyDate();

		m_dOpeningPurse1Liability = RowOpening.GetPurse1LiabilityDatabase();
		m_dOpeningPurse2Balance = 0;
		m_dOpeningPurse3Liability = RowOpening.GetPurse3LiabilityDatabase();
		m_dOpeningPurse1Credit = RowOpening.GetPurse1CreditDatabase();
		m_dOpeningPurse3Credit = RowOpening.GetPurse3CreditDatabase();
		m_strOpeningDate = RowOpening.GetDate();
		m_strOpeningTime = RowOpening.GetTime();

		CTime datetimeOpening;
		{
			CSSDate date(m_strOpeningDate);
			CSSTime time(m_strOpeningTime);
			datetimeOpening = CTime(date.GetYear(), date.GetMonth(), date.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
		}

		CTime datetimeClosing;
		{
			CSSDate date(m_strClosingDate);
			CSSTime time(m_strClosingTime);
			datetimeClosing = CTime(date.GetYear(), date.GetMonth(), date.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
		}

		// check for cash account history file

		CString strTempCashHistoryFilename = "";

		if (::FileExists(Filenames.GetPurchaseHistoryFilename("0")) == TRUE)	// see if have cash account history file "000000.dat"
		{
			CTLogFile tlog("0");														// cash account history file - will merge in any outstanding
			strTempCashHistoryFilename = Filenames.GetTempCashHistoryFilename();		// temp cash account = "xxxxxx.dat"

// opening date less 15 minutes

			//CSSDate date ( m_strOpeningDate );
			//CSSTime time ( m_strOpeningTime );

			//CTime t1 ( date.GetYear(), date.GetMonth(), date.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond() );
			CTime t2;
			t2 = datetimeOpening - CTimeSpan(0, 0, 15, 0);							// 15 minutes before last end of day

			CSSDate dateStart;
			dateStart.SetDate(t2);

			CSSTime timeStart;
			timeStart.SetTime(t2);

			// closing date plus 15 minutes

						//CTime t3 ( dateClosing.GetYear(), dateClosing.GetMonth(), dateClosing.GetDay(), timeClosing.GetHour(), timeClosing.GetMinute(), timeClosing.GetSecond() );
			t2 = datetimeClosing + CTimeSpan(0, 0, 15, 0);							// 15 minutes after end of day

			CSSDate dateEnd;
			dateEnd.SetDate(t2);

			CSSTime timeEnd;
			timeEnd.SetTime(t2);

			if (tlog.Extract(strTempCashHistoryFilename, dateStart.GetDate(), timeStart.GetTime(), dateEnd.GetDate(), timeEnd.GetTime()) == -1)	// create temp history file for cash account
			{
				nReply = nREPORT_NOSOURCE;												// no previous date to start searhing from
			}
		}

		if (nReply == nREPORT_NOERROR)												// see if ok to carry on
		{
			if (auditFile.Open() == TRUE)												// reopen source file
			{
				if (m_Report.Open(strReportFile) == TRUE)
				{
					CString strTitle = "";
					strTitle.Format("Regenerating EOD report for %2.2d/%2.2d/%4.4d",
						dateClosing.GetDay(),
						dateClosing.GetMonth(),
						dateClosing.GetYear());

					CWorkingDlg WorkingDlg(strTitle, TRUE);
					WorkingDlg.Create();

					CEODCashlessConsolidator EODCashlessConsolidator(FALSE);
					EODCashlessConsolidator.SetNoSalesFilename(strDateKeyOpening);			// key - opening balance date yyyymmdd

					int nRecordsChecked = 0;
					if ((nReply = EODCashlessConsolidator.ConsolidateFromAudit(&auditFile, datetimeOpening, datetimeClosing, FALSE, TRUE, &WorkingDlg, nRecordsChecked)) == nREPORT_NOERROR)	// calc whats been added / subtracted in the period
					{
						m_nDayCounter = 1;										// number of EOD dates being consolidated
						CreateReport(&EODCashlessConsolidator,bHaveAdjust);

						RowClosing.SetPurse1LiabilityAudit(m_dClosingPurse1Liability);
						RowClosing.SetPurse1CreditAudit(m_dClosingPurse1Credit);
						RowClosing.SetPurse2BalanceAudit(m_dClosingPurse2Balance);
						RowClosing.SetPurse3LiabilityAudit(m_dClosingPurse3Liability);
						RowClosing.SetPurse3CreditAudit(m_dClosingPurse3Credit);
						RowClosing.SetHaveAdjustFlag(bHaveAdjust);

						if (TRUE == bHaveAdjust)
						{
							RowClosing.SetPurse1LiabilityAdjust(m_dDatabasePurse1Liability - m_dClosingPurse1Liability);
							RowClosing.SetPurse1CreditAdjust(m_dDatabasePurse1Credit - m_dClosingPurse1Credit);
							RowClosing.SetPurse3LiabilityAdjust(m_dDatabasePurse3Liability - m_dClosingPurse3Liability);
							RowClosing.SetPurse3CreditAdjust(m_dDatabasePurse3Credit - m_dClosingPurse3Credit);
						}
						else
						{
							RowClosing.SetPurse1LiabilityAdjust(0.0);
							RowClosing.SetPurse1CreditAdjust(0.0);
							RowClosing.SetPurse3LiabilityAdjust(0.0);
							RowClosing.SetPurse3CreditAdjust(0.0);
						}

						Repo.UpsertRow(RowClosing, NULL);

						AddAdditionalInfo();									// add group period sales \ Epos payments

						AddTerminalGrouping(&EODCashlessConsolidator);						// add secondary Terminal Grouping sets data
					}
				
					m_Report.Close();

					if (nReply == nREPORT_NOERROR)
					{
						CString strFilename = Filenames.GetEODReportPath(strDateKeyClosing);		// key - yyyymmdd

						int nYesNo = IDYES;
						if ( (::FileExists(strFilename) == TRUE) && ( TRUE == bSingle ) )
						{
							CString strMsg = "";
							strMsg.Format("As an End of Day report already exists for %s!\n\nDo you wish to replace it with this new re-generated report?", 
								(const char*) CreateDisplayDateFromSortDate(strDate));

							nYesNo = Prompter.YesNo(strMsg, m_strTitle);
						}

						if (nYesNo == IDYES)
						{
							m_strEODSaveFilename = strFilename;
							::CopyFile(strReportFile, strFilename, FALSE);	// make a copy of copy to EOD folder
						}
					}
				}
				else
				{
					nReply = nREPORT_CREATEFAIL;
				}

				auditFile.Close();											// close audit file if still open
			}
			else
			{
				nReply = nREPORT_OPENERROR;
			}
		}

		CFileRecycler::SendToRecycleBin(strTempCashHistoryFilename);					// delete temp file if found
	}

	return nReply;
}

//*****************************************************************
// consolidate reports - not called from background

int CReportEndOfDay::ConsolidateReport(CString strKeyDateFrom, CString strKeyDateTo, CString strReportFile)
{
	Reset();

	CString strEODAuditFile = CheckEODAuditFile();					// Get previous saved EOD auditfile ( create one if not found )
	if (strEODAuditFile == "")
	{
		return nREPORT_PAUSEERROR;
	}

	int nErrorNo = nREPORT_NOERROR;
	CSmartPayAuditFile auditFile(strEODAuditFile);
	if ((nErrorNo = auditFile.ValidateFile()) != nREPORT_NOERROR)
	{
		return nErrorNo;
	}

	CSQLRowClosingBalance RowFirst;
	CSQLRepositoryClosingBalance Repo;
	if (Repo.SelectFirstRow(RowFirst, NULL).GetSQLError() != SQLCRUD_ERR_NONE)
	{
		return nREPORT_NOSOURCE;
	}

	// see if have start date range in closing database
	CSQLRowClosingBalance RowStart;
	RowStart.SetKeyDate(strKeyDateFrom);
	if (Repo.SelectRow(RowStart, NULL).GetSQLError() != SQLCRUD_ERR_NONE)
	{
		if (Repo.SelectNextRow(RowStart, NULL).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			return DisplayNoDate(strKeyDateFrom,strKeyDateTo);
		}
	}

	CSQLRowClosingBalance RowOpening = RowStart;
	if (Repo.SelectPreviousRow(RowOpening, NULL).GetSQLError() != SQLCRUD_ERR_NONE)
	{
		return DisplayNoDate(strKeyDateFrom,strKeyDateTo);
	}

	m_strOpeningDate = RowOpening.GetDate();									// as starting balances date & values
	m_strOpeningTime = RowOpening.GetTime();									//
	
	// see if have end date range in closing database
	CSQLRowClosingBalance RowEnd;
	RowEnd.SetKeyDate(strKeyDateTo);
	
	if (Repo.SelectRow(RowEnd, NULL).GetSQLError() != SQLCRUD_ERR_NONE)
	{
		if (Repo.SelectPreviousRow(RowEnd, NULL).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			return DisplayNoDate(strKeyDateFrom,strKeyDateTo);
		}
	}
	
	m_strClosingDate = RowEnd.GetDate();						// end of range date & time
	m_strClosingTime = RowEnd.GetTime();

	if (RowEnd.GetKeyDate() < RowStart.GetKeyDate())
	{
		return DisplayNoDate(strKeyDateFrom, strKeyDateTo);
	}
	
	CWorkingDlg WorkingDlg(szMSG_CREATINGREPORT, TRUE);
	WorkingDlg.Create();

	// check for cash account history file & create temp file of cash sales for consolidation date range

	int nReply = nREPORT_NOERROR;

	CString strTempCashHistoryFilename = "";

	if (::FileExists(Filenames.GetPurchaseHistoryFilename("0")) == TRUE)	// see if have cash account history file "000000.dat"
	{
		CTLogFile tlog("0");														// cash account history file - will merge in any outstanding
		strTempCashHistoryFilename = Filenames.GetTempCashHistoryFilename();		// temp cash account = "xxxxxx.dat"

// opening date -15 minutes

		CSSDate date(m_strOpeningDate);
		CSSTime time(m_strOpeningTime);

		CTime t1(date.GetYear(), date.GetMonth(), date.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());

		// 15 minutes before last end of day
		CTime t2 = t1 - CTimeSpan(0, 0, 15, 0);

		CSSDate dateStart;	
		dateStart.SetDate(t2);
		CSSTime timeStart;	
		timeStart.SetTime(t2);

		// closing date +15 minutes

		date.SetDate(m_strClosingDate);
		time.SetTime(m_strClosingTime);

		CTime t3(date.GetYear(), date.GetMonth(), date.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
		t2 = t3 + CTimeSpan(0, 0, 15, 0);											// 15 minutes after end of day

		CSSDate dateEnd;	
		dateEnd.SetDate(t2);
		CSSTime timeEnd;
		timeEnd.SetTime(t2);

		if (tlog.Extract(strTempCashHistoryFilename, dateStart.GetDate(), timeStart.GetTime(), dateEnd.GetDate(), timeEnd.GetTime()) == -1)	// create temp history file for cash account
		{
			nReply = nREPORT_NOSOURCE;												// no previous date to start searhing from
		}
	}
	//

	CEODCashlessConsolidator EODCashlessConsolidator(FALSE);
	EODCashlessConsolidator.SetNoSalesFilename("consol");								// file to hold consoliadet no sales

	bool bHaveDatabaseBalance = FALSE;

	if (nReply == nREPORT_NOERROR)										// see if ok to carry on
	{
		if (auditFile.Open() == TRUE)										// reopen source file
		{
			bool bStarted = FALSE;
			
			CSQLRecordSetClosingBalance RecordSetClosing(NULL, 
				RSParams_ClosingBalance_DetailByRange{
				RowOpening.GetKeyDate(),
				RowEnd.GetKeyDate() });

			CSQLRowClosingBalance RowBuffer;
			CSQLRowClosingBalance RowCurrent;
			CSQLRowClosingBalance RowPrevious;

			int nRecordsChecked = 0;

			bool bFirstPass = TRUE;
			while( RecordSetClosing.StepSelectAll(RowBuffer) == TRUE )
			{ 
				RowPrevious = RowCurrent;
				RowCurrent = RowBuffer;

				if (TRUE == bFirstPass)
				{
					bFirstPass = FALSE;
					continue;
				}
			
				CTime datetimeOpening;
				CTime datetimeClosing;

				{
					CSSDate date(RowCurrent.GetDate());
					CSSTime time(RowCurrent.GetTime());
					datetimeClosing = CTime(date.GetYear(), date.GetMonth(), date.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
				}

				{
					CSSDate date(RowPrevious.GetDate());
					CSSTime time(RowPrevious.GetTime());
					datetimeOpening = CTime(date.GetYear(), date.GetMonth(), date.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
				}

				bool bFirstEOD = (RowPrevious.GetKeyDate() == RowFirst.GetKeyDate());

				if (bStarted == FALSE)											// see if first time through
				{
					m_dOpeningPurse1Liability = RowPrevious.GetPurse1LiabilityDatabase();				//
					m_dOpeningPurse2Balance = 0;									//
					m_dOpeningPurse3Liability = RowPrevious.GetPurse3LiabilityDatabase();				// used as first line of report
					m_dOpeningPurse1Credit = RowPrevious.GetPurse1CreditDatabase();
					m_dOpeningPurse3Credit = RowPrevious.GetPurse3CreditDatabase();
					m_strOpeningDate = RowPrevious.GetDate();					// as starting balances date & values
					m_strOpeningTime = RowPrevious.GetTime();					//
					bStarted = TRUE;
				}

				bHaveDatabaseBalance = RowCurrent.GetHaveAdjustFlag();
				if (TRUE == bHaveDatabaseBalance)
				{
					m_dDatabasePurse1Liability = RowCurrent.GetPurse1LiabilityDatabase();
					m_dDatabasePurse1Credit = RowCurrent.GetPurse1CreditDatabase();
					m_dDatabasePurse3Liability = RowCurrent.GetPurse3LiabilityDatabase();
					m_dDatabasePurse3Credit = RowCurrent.GetPurse3CreditDatabase();
				}
				else
				{
					m_dDatabasePurse1Liability = 0.0;
					m_dDatabasePurse1Credit = 0.0;
					m_dDatabasePurse3Liability = 0.0;
					m_dDatabasePurse3Credit = 0.0;
				}

				CString strDate = RowPrevious.GetDate();

				CString strMsg = "";
				strMsg.Format("Creating EOD Consolidation : Checking %s", 
					(const char*) strDate);

				WorkingDlg.SetCaption1(strMsg);
				
				if ((nReply = EODCashlessConsolidator.ConsolidateFromAudit( &auditFile, datetimeOpening, datetimeClosing, bFirstEOD, TRUE, &WorkingDlg, nRecordsChecked)) != nREPORT_NOERROR)	// calc whats been added / subtracted in the period
				{
					break;
				}

				++m_nDayCounter;												// inc number of EOD dates being consolidated

				EODCashlessConsolidator.ResetCardCounter();
			}

			auditFile.Close();													// close audit file
		}
		else
		{
			nReply = nREPORT_OPENERROR;
		}
	}

	if (nReply == nREPORT_NOERROR)
	{
		if (m_Report.Open(strReportFile) == TRUE)
		{
			CreateReport(&EODCashlessConsolidator,bHaveDatabaseBalance);						// complie consolidate report

			AddAdditionalInfo();										// add period group sales & Epos payments

			AddTerminalGrouping(&EODCashlessConsolidator);				// add secondary Terminal Grouping sets data

			m_Report.Close();
		}
		else
		{
			nReply = nREPORT_CREATEFAIL;
		}
	}

	CFileRecycler::SendToRecycleBin(strTempCashHistoryFilename);						// delete temp file if found
	
	return nReply;
}

//*****************************************************************************

int CReportEndOfDay::DisplayNoDate ( CString strDateFrom, CString strDateTo )
{
	strDateFrom = CreateDisplayDateFromSortDate(strDateFrom);
	strDateTo = CreateDisplayDateFromSortDate(strDateTo);

	CString strMsg = "";
	strMsg += "No End of Day Balance entry found for ";
	strMsg += strDateFrom;

	if ((strDateTo != "") && (strDateTo != strDateFrom))
	{
		strMsg += " - ";
		strMsg += strDateTo;
	}

	Prompter.Error( strMsg, "EOD Report" );

	return nREPORT_ERRORDISPLAYED;								// already displayed error message
}

//*****************************************************************************

CString CReportEndOfDay::CreateDisplayDateFromSortDate(CString strSortDate)
{
	CString strDisplayDate = strSortDate;
	
	if ((strDisplayDate.GetLength() == 8) && (TestNumeric(strDisplayDate) == TRUE))
	{
		CString strTempDate = strDisplayDate;
		strDisplayDate.Format("%s/%s/%s", 
			(const char*) strTempDate.Right(2), 
			(const char*) strTempDate.Mid(4, 2),
			(const char*) strTempDate.Left(4));
	}

	return strDisplayDate;
}

//*****************************************************************************
