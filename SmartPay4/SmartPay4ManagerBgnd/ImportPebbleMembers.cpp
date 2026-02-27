/**********************************************************************/
#include "..\SmartPay4Shared\RepmanHandler.h"
#include "..\SmartPay4Shared\SQLAuditRecord.h"
#include "..\SmartPay4Shared\SmartPayLockManager.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SmartPay4Shared\SQLTable_UserText\SQLRepositoryUserText.h"
#include "..\SmartPay4Shared\SQLTable_PebbleLinkTime\SQLRepositoryPebbleLinkTime.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLHelpers.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
/**********************************************************************/
#include "ImportPebbleMembers.h"
/**********************************************************************/

const char* szPB_BLANK = "    --";

/**********************************************************************/

CImportPebbleMembers::CImportPebbleMembers(CSQLRepositoryPebbleConfig* pConfig, CArray<CPebbleMemberBuffer, CPebbleMemberBuffer>& arrayMembers, CWnd* pParent) :
	m_arrayMembers(arrayMembers),
	m_pConfig(pConfig),
	m_pParent(pParent)
{
}

/**********************************************************************/

CImportPebbleMembers::~CImportPebbleMembers()
{
}

//***************************************************************


//*******************************************************************
// ParentPay import

bool CImportPebbleMembers::ImportMembers()
{
	ImportMembersInternal(TRUE);													
	return (m_strError == "");
}

//*******************************************************************
//*** process file for display or import*****************************
//*******************************************************************

int CImportPebbleMembers::ImportMembersInternal(bool bImportReqd)
{
	m_ImportStats.Reset();

	CString strTitle = "";
	if (TRUE == bImportReqd)
	{
		strTitle = "Import ";
	}
	else
	{
		strTitle = "Display ";
	}
	strTitle += m_strReportTitle;

	CWorkingDlg WorkingDlg(strTitle,FALSE);									//create import progress bar
	WorkingDlg.Create();

	for (int n = 0; n < m_arrayMembers.GetSize(); n++)
	{
		m_currentMember = m_arrayMembers.GetAt(n);

		++m_ImportStats.m_nRead;
		WorkingDlg.SetCaption2PercentDone((n * 100) / m_arrayMembers.GetSize());

		__int64 nCardNo = 0;
		m_ImportStats.m_nCurrentLineStatus = PEBBLE_IMPORTLINE_INACTIVE;
		m_ImportStats.m_strCurrentLineException = "";

		if (TRUE == m_currentMember.m_bIsActive)
		{
			CSQLRepositoryAccount RepoAccount;
			nCardNo = RepoAccount.LookupMemberID(m_currentMember.m_strMemberID, NULL).GetSQLResult();

			if (nCardNo != 0)
			{
				if (CheckPebbleUPN(nCardNo) == FALSE)
				{
					m_ImportStats.m_nCurrentLineStatus = PEBBLE_IMPORTLINE_UPNMISMATCH;
					++m_ImportStats.m_nUPNMismatch;
				}
				else
				{
					m_ImportStats.m_nCurrentLineStatus = PEBBLE_IMPORTLINE_ALREADYMATCHED;
					++m_ImportStats.m_nAlreadyMatched;
				}
			}
			else
			{
				if (MatchAccount(nCardNo) == FALSE)
				{
					if (PEBBLE_IMPORTLINE_NOTMATCHED == m_ImportStats.m_nCurrentLineStatus)
					{
						m_ImportStats.m_strCurrentLineException.Format("No account found for %s",
							(const char*)m_currentMember.m_strMemberID);
					}
				}
			}
		}
		else
		{
			m_ImportStats.m_nCurrentLineStatus = PEBBLE_IMPORTLINE_INACTIVE;
			++m_ImportStats.m_nPebbleInactive;
		}

		if (bImportReqd == FALSE)
		{
			ShowImportLine(nCardNo);
		}
		else
		{
			ImportLine(nCardNo);				// input line ( if no exception ) 
		}
	}

	return nREPORT_NOERROR;
}

//**********************************************************************
// search for unknown account using LocateBy database 

bool CImportPebbleMembers::MatchAccount(__int64& nCardNo)
{
	nCardNo = 0;

	bool bReply = FALSE;
	
	//MATCH BY UPN
	{
		CString strField = m_pConfig->GetLocateByLabelUPN();
		CString strMatch = m_currentMember.m_strUPN;

		if ((strField != "") && (strMatch != ""))
		{
			CSQLRepositoryAccount RepoAccount;
			nCardNo = RepoAccount.LookupIndex(strField, strMatch, NULL).GetSQLResult();
		}

		if (nCardNo != 0)
		{
			++m_ImportStats.m_nMatchedByUPN;
			m_ImportStats.m_nCurrentLineStatus = PEBBLE_IMPORTLINE_MATCHEDBYUPN;
		}
	}

	//MATCH BY MisID
	if (nCardNo == 0)
	{
		CString strField = m_pConfig->GetLocateByLabelMisID();
		CString strMatch = m_currentMember.m_strMisID;

		if ((strField != "") && (strMatch != ""))
		{
			CSQLRepositoryAccount RepoAccount;
			nCardNo = RepoAccount.LookupIndex(strField, strMatch, NULL).GetSQLResult();
		}
		
		if (nCardNo != 0)
		{
			++m_ImportStats.m_nMatchedByMisID;
			m_ImportStats.m_nCurrentLineStatus = PEBBLE_IMPORTLINE_MATCHEDBYMIS;
		}
	}

	if (nCardNo != 0)
	{
		//IF WE JUST MATCHED BY MIS, CHECK UPN IF WE HAVE ONE
		if (PEBBLE_IMPORTLINE_MATCHEDBYMIS == m_ImportStats.m_nCurrentLineStatus)
		{
			if ( CheckPebbleUPN(nCardNo) == FALSE )
			{
				--m_ImportStats.m_nMatchedByMisID;
				++m_ImportStats.m_nUPNMismatch;
				m_ImportStats.m_nCurrentLineStatus = PEBBLE_IMPORTLINE_UPNMISMATCH;
			}
		}

		bReply = TRUE;
	}
	else
	{
		++m_ImportStats.m_nNotMatched;
		m_ImportStats.m_nCurrentLineStatus = PEBBLE_IMPORTLINE_NOTMATCHED;
	}

	return bReply;
}

//******************************************************************

void CImportPebbleMembers::AddColumnText()
{
	CSQLRepositoryUserText RepoText;

	m_Report.AddColumn ( RepoText.GetMemberIDText(), TA_LEFT, 300 );
	m_Report.AddColumn ( "Account", TA_LEFT, 500 );
	m_Report.AddColumn ( "Forename (Pb)", TA_LEFT, 300 );
	m_Report.AddColumn ( "Surname (Pb)", TA_LEFT, 300 );
	m_Report.AddColumn ( "DOB (Pb)", TA_LEFT, 300 );
	
	int nField = 0;
	if ((nField = m_pConfig->m_Row.GetMISInfoIndex()) > 0)
	{
		m_Report.AddColumn(RepoText.GetInfoText(nField), TA_LEFT, 300);
	}

	if ((nField = m_pConfig->m_Row.GetUPNInfoIndex()) > 0)
	{
		m_Report.AddColumn(RepoText.GetInfoText(nField), TA_LEFT, 300);
	}

	m_Report.AddColumn ( "", TA_LEFT, 600 );
}

//********************************************************************

bool CImportPebbleMembers::ShowImportLine(__int64 nCardNo)
{
	switch (m_ImportStats.m_nCurrentLineStatus)
	{
	case PEBBLE_IMPORTLINE_ALREADYMATCHED:
	case PEBBLE_IMPORTLINE_MATCHEDBYUPN:
	case PEBBLE_IMPORTLINE_MATCHEDBYMIS:
		{
			CSQLRepositoryAccount RepoAccount;

			//THIS SHOULD NEVER HAPPEN, BUT IF IT DOES, TREAT AS NOT MATCHED
			if (RepoAccount.RowExists(nCardNo, NULL).GetSQLResult() == 0)
			{
				++m_ImportStats.m_nNotMatched;

				switch (m_ImportStats.m_nCurrentLineStatus)
				{
				case PEBBLE_IMPORTLINE_ALREADYMATCHED:
					--m_ImportStats.m_nAlreadyMatched;
					break;

				case PEBBLE_IMPORTLINE_MATCHEDBYUPN:
					--m_ImportStats.m_nMatchedByUPN;
					break;

				case PEBBLE_IMPORTLINE_MATCHEDBYMIS:
					--m_ImportStats.m_nMatchedByMisID;
					break;
				}

				m_ImportStats.m_nCurrentLineStatus = PEBBLE_IMPORTLINE_NOTMATCHED;
			}
		}
	}

	CString strCardNo = "";

	if (nCardNo != 0)
	{
		strCardNo = System.FormatCardNo(nCardNo, TRUE);

		switch (m_ImportStats.m_nCurrentLineStatus)
		{
		case PEBBLE_IMPORTLINE_MATCHEDBYUPN:
			strCardNo.Insert(0, "(UPN) ");
			break;

		case PEBBLE_IMPORTLINE_MATCHEDBYMIS:
			strCardNo.Insert(0, "(MIS) ");
			break;

		case PEBBLE_IMPORTLINE_NOTMATCHED:
			strCardNo.Insert(0, "x ");
			break;
		}
	}
	else
	{
		strCardNo = szPB_BLANK;
	}

	CCSV csv('\t');
	csv.Add(m_currentMember.m_strMemberID);
	csv.Add(strCardNo);
	csv.Add(m_currentMember.m_strFirstName);
	csv.Add(m_currentMember.m_strLastName);
	csv.Add(m_currentMember.m_strDOB);

	if (m_pConfig->m_Row.GetMISInfoIndex() > 0)
	{
		csv.Add(m_currentMember.m_strMisID);
	}

	if (m_pConfig->m_Row.GetUPNInfoIndex() > 0)
	{
		csv.Add(m_currentMember.m_strUPN);
	}

	if (PEBBLE_IMPORTLINE_INACTIVE == m_ImportStats.m_nCurrentLineStatus)
	{
		csv.Add("Inactive - skipped");
	}
	else
	{
		csv.Add(m_ImportStats.m_strCurrentLineException);
	}

	return (m_Report.WriteLine(csv));
}

//***********************************************************

void CImportPebbleMembers::AddStatistics()
{
	CString strLine = "";
	m_Report.WriteLine ( "" );

	strLine.Format ( "%d\t<..>Lines read", m_ImportStats.m_nRead );
	m_Report.WriteLine ( strLine );

	strLine.Format ( "%d\t<..>Accounts already matched",m_ImportStats.m_nAlreadyMatched );
	m_Report.WriteLine ( strLine );

	{
		CString strLocBy = "Accounts newly matched by ";

		if (m_ImportStats.m_nMatchedByUPN > 0)
		{
			strLine.Format("%d\t<..>%s'%s' (UPN)",
				m_ImportStats.m_nMatchedByUPN,
				(const char*)strLocBy,
				(const char*)m_pConfig->GetLocateByText(nPEBBLE_LOCATEBY_UPN));

			m_Report.WriteLine(strLine);
		}

		if (m_ImportStats.m_nMatchedByMisID > 0)
		{
			strLine.Format("%d\t<..>%s'%s' (MIS)",
				m_ImportStats.m_nMatchedByMisID,
				(const char*)strLocBy,
				(const char*)m_pConfig->GetLocateByText(nPEBBLE_LOCATEBY_MIS));

			m_Report.WriteLine(strLine);
		}
	}

	if ( m_ImportStats.m_nPebbleInactive > 0 )
	{
		strLine.Format ( "%d\t<..>Inactive Pebble accounts", m_ImportStats.m_nPebbleInactive);
		m_Report.WriteLine ( strLine );
	}

	if ( m_ImportStats.m_nUPNMismatch > 0 )
	{
		strLine.Format ( "%d\t<..>Accounts not matched - UPN mismatch", m_ImportStats.m_nUPNMismatch);
		m_Report.WriteLine ( strLine );
	}

	if ( m_ImportStats.m_nNotMatched > 0 )
	{
		strLine.Format ( "%d\t<..>Accounts not matched", m_ImportStats.m_nNotMatched);
		m_Report.WriteLine ( strLine );
	}
}

//**********************************************************************

void CImportPebbleMembers::ImportLine(__int64 nCardNo)
{
	switch (m_ImportStats.m_nCurrentLineStatus)
	{
	case PEBBLE_IMPORTLINE_MATCHEDBYMIS:
	case PEBBLE_IMPORTLINE_MATCHEDBYUPN:
		{
			CSQLRowAccountFull RowAccount;
			RowAccount.SetUserID(nCardNo);

			CSQLRepositoryAccount RepoAccount;
			if (RepoAccount.SelectRow(RowAccount, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
			{
				RowAccount.SetMemberID(m_currentMember.m_strMemberID);
				RepoAccount.UpdateRow(RowAccount, NULL);

				CString strDateTime = GetSimpleDateTimeNowString();

				CSQLRowPebbleLinkTime RowLinkTime;
				RowLinkTime.SetDate(strDateTime.Left(8));
				RowLinkTime.SetTime(strDateTime.Right(6));
				RowLinkTime.SetUserID(RowAccount.GetUserID());
				RowLinkTime.SetUsername(RowAccount.GetUsername());
				RowLinkTime.SetForename(RowAccount.GetForename());
				RowLinkTime.SetSurname(RowAccount.GetSurname());
				RowLinkTime.SetGroupNo(RowAccount.GetGroupNo());
				RowLinkTime.SetPebbleFirstName(m_currentMember.m_strFirstName);
				RowLinkTime.SetPebbleLastName(m_currentMember.m_strLastName);
				RowLinkTime.SetMatchType(PEBBLE_IMPORTLINE_MATCHEDBYUPN == m_ImportStats.m_nCurrentLineStatus ? 1 : 0);
				RowLinkTime.SetMIS(m_currentMember.m_strMisID);
				RowLinkTime.SetUPN(m_currentMember.m_strUPN);

#ifdef SYSTEMTYPE_BACKGROUND
				RowLinkTime.SetProcessType(1);
#else
				RowLinkTime.SetProcessType(0);
#endif

				CSQLRepositoryPebbleLinkTime RepoLinkTime;
				RepoLinkTime.InsertRow(RowLinkTime, NULL);

				++m_ImportStats.m_nNewLinks;
			}
			else
			{
				switch (m_ImportStats.m_nCurrentLineStatus)
				{
				case PEBBLE_IMPORTLINE_MATCHEDBYUPN:
					--m_ImportStats.m_nMatchedByUPN;
					break;

				case PEBBLE_IMPORTLINE_MATCHEDBYMIS:
					--m_ImportStats.m_nMatchedByMisID;
					break;
				}

				++m_ImportStats.m_nNotMatched;
				m_ImportStats.m_nCurrentLineStatus = PEBBLE_IMPORTLINE_NOTMATCHED;
			}
		}
	}
}
	
	
//**********************************************************************
// check pebble data against record data to see if same record 

bool CImportPebbleMembers::CheckPebbleUPN(__int64 nCardNo)
{
	if (m_currentMember.m_strUPN == "")
	{
		return TRUE;
	}

	int nField = m_pConfig->m_Row.GetUPNInfoIndex();
	if (nField <= 0)
	{
		return TRUE;
	}

	CSQLRowAccountFull RowAccount;
	RowAccount.SetUserID(nCardNo);

	CSQLRepositoryAccount RepoAccount;
	if (RepoAccount.SelectRow(RowAccount, NULL).GetSQLError() != SQLCRUD_ERR_NONE)
	{
		return TRUE;
	}

	CString strAccountUPN = RowAccount.GetInfo(nField);
	if ((strAccountUPN == "") || (strAccountUPN == m_currentMember.m_strUPN))
	{
		return TRUE;
	}

	m_ImportStats.m_strCurrentLineException.Format("Account UPN mismatch (%s)",
		(const char*)strAccountUPN);

	return FALSE;
}

//**********************************************************************
#ifdef SYSTEMTYPE_MANAGER
//**********************************************************************

bool CImportPebbleMembers::ShowImports(bool bFirstTime)
{
	CString strReportLabel = "R227";
	CString strReportFile = Filenames.GetReportFilename(strReportLabel);
	CString strParamsFile = Filenames.GetReportParamsFilename(strReportLabel);

	CRepmanHandler repman(m_strReportTitle, m_pParent);

	int nErrorNo = 0;
	if (m_Report.Open(strReportFile) == TRUE)				// open report output file
	{
		m_Report.SetStyle1(m_strReportTitle);
		AddColumnText();

		nErrorNo = ImportMembersInternal(FALSE);						// no import reqd					
	
		AddStatistics();

		m_Report.Close();										// close report file
	}
	else
	{
		nErrorNo = nREPORT_CREATEFAIL;
	}

	if (m_strError == "")
	{
		if (nErrorNo == nREPORT_NOERROR)
		{
			if (m_ImportStats.GotExceptions() && (TRUE == bFirstTime))
			{
				repman.DisplayMessage(szMSG_EXCEPTIONSDETECTED, MB_ICONEXCLAMATION);		// "Exceptions were detected with the import data!\n\nPlease see end of report for details.";
			}

			nErrorNo = repman.DisplayReport(strReportFile, strParamsFile, strReportLabel);
		}

		repman.DisplayError(nErrorNo);
		return TRUE;
	}
	else
	{
		repman.DisplayMessage(m_strError, MB_ICONEXCLAMATION);
	}

	return FALSE;
}

//**********************************************************************

void CImportPebbleMembers::ShowStatistics(CImportStatsDlgConfig& ImportStatsConfig)
{
	ImportStatsConfig.m_nLinesRead = m_ImportStats.m_nRead;
	ImportStatsConfig.m_nLinesUsed = m_ImportStats.m_nMatchedByMisID + m_ImportStats.m_nMatchedByUPN;
	ImportStatsConfig.m_nLinesIgnored = ImportStatsConfig.m_nLinesRead - ImportStatsConfig.m_nLinesUsed;
	ImportStatsConfig.m_nLinesReadPos = 0;
	ImportStatsConfig.m_nLinesIgnoredPos = 1;
	ImportStatsConfig.m_nLinesUsedPos = 2;
	ImportStatsConfig.m_nLinesInvalidPos = -1;
	ImportStatsConfig.m_nRecordsAddedPos = -1;
	ImportStatsConfig.m_nRecordsDeletedPos = -1;
	ImportStatsConfig.m_nLastLinePos = 3;
	ImportStatsConfig.m_strCaption = "Link Pebble Members";

	CImportStatsDlg dlg(ImportStatsConfig);
	dlg.DoModal();
}

//**********************************************************************
#endif
//**********************************************************************
