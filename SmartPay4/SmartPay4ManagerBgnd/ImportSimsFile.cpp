/*********************************************************************/
#include "..\SmartPay4Shared\Prompter.h"
#include "..\SmartPay4Shared\RepmanHandler.h"
#include "..\SmartPay4Shared\SQLAuditRecord.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLHelpers.h"
/*********************************************************************/
#include "ImportSimsFile.h"
/*********************************************************************/
#include "..\SmartPay4Shared\MembershipDatabase.h"
/*********************************************************************/

CImportSimsFile::~CImportSimsFile()
{
	if ( ( m_ImportFlag.GetImportFlag() == TRUE  ) && ( m_nAdded > 0 ) )								// see if new data has been imported
	{
		if ( m_pSims->m_nNewIntakeAccount == nSIMS_NEWINTAKE_NEXTFREE_INDEX )
		{
			m_pSims->m_strStartFromUserID = m_strStartFromUserID.TrimLeft ( '0' );	// save last free record used with leading 0's
			m_pSims->Save();
		}
	}
}

//*******************************************************************

void CImportSimsFile::Init()
{
	m_nRead = 0;
	m_nIgnored = 0;
	m_nUsed = 0;
	m_nAdded = 0;
	m_nSkipped = 0;
	m_strError = "";
	m_strImportFilename = "";
	m_strExceptionComment = "";
	
	m_strStartFromUserID	= m_pSims->m_strStartFromUserID;
	m_nImportMethod			= m_pSims->m_nImportMethod;							// can set be externally

	m_ImportFlag.SetImportFlag( FALSE );
}

/*********************************************************************/
//*** process file for display or import******************************
/*********************************************************************/

int CImportSimsFile::ProcessFile()
{
// set which header fields must exist 

	m_simsFile.SetUPNRequired  ( m_pSims->UPNRequired() );
	m_simsFile.SetPIDRequired  ( m_pSims->PIDRequired() );
	m_simsFile.SetSIMSRequired ( m_pSims->SIMSRequired() );

	if ( m_simsFile.Open ( m_strImportFilename ) == FALSE )
	{
		m_strError = m_simsFile.GetError();
		return nREPORT_INVALIDSOURCE;
	}

// scan import file

	CWorkingDlg WorkingDlg ( GetProgressTitle(), FALSE );
	WorkingDlg.Create();

	while ( m_simsFile.ReadLine() == TRUE )					// read next line
	{
		WorkingDlg.SetCaption2PercentDone ( m_simsFile.GetPercentPosition() );

		++m_nRead;
		m_strExceptionComment = "";							// clear any current exception

		CString strCardNo = "";
		CString strLabel = "";

		switch ( m_pSims->m_nLocateBy )						// locate account by Apno / UPN / Person_id
		{
		case nSIMS_LOCATEBY_ADNO_INDEX:		strLabel = m_simsFile.GetAdno();	break;
		case nSIMS_LOCATEBY_UPN_INDEX:		strLabel = m_simsFile.GetUPN();		break;
		case nSIMS_LOCATEBY_PID_INDEX:		strLabel = m_simsFile.GetPID();		break;
		}

		CSQLRepositoryAccount RepoAccount;
		__int64 nUserID = RepoAccount.LookupIndex(m_pSims->GetLocateByLabel(), strLabel, NULL).GetSQLResult();
		
		if (0 == nUserID)
		{
			if (m_pSims->m_bAddUnknowns == TRUE)					// not found - see if can add record
			{
				GetNewCardNo(strCardNo);							// get cardno for new record
			}
			else
			{
				m_strExceptionComment.Format("No account found for %s",
					(const char*)strLabel);
			}
		}
		else
		{
			strCardNo = FormatInt64Value(nUserID);
		}

		if ( m_ImportFlag.GetImportFlag() == TRUE )	
		{
			ImportLine ( strCardNo );	// input line ( cardno blank if exception raised ) 
		}
		else
		{
			ShowImportLine ( strCardNo );
		}
	}

	m_simsFile.Close();												// close sims import file

	return nREPORT_NOERROR;
}

/*********************************************************************/
// excecute SIMS import

bool CImportSimsFile::Import(const char* szFilename)
{
	m_ImportFlag.SetImportFlag(TRUE);

	bool bReply = FALSE;

	m_strImportFilename = szFilename;
	if (m_strImportFilename == "")
	{
		m_strImportFilename = m_pSims->m_strImportFilename;			// use default SIMS import filename if not set
	}

	if (ProcessFile() == nREPORT_NOERROR)							// import reqd					
	{
		if (m_pSims->m_bDeleteImportFile == TRUE)					// see if delete file on manual import reqd
		{
			CFileRecycler::SendToRecycleBin(m_strImportFilename);
		}
	}

	bReply = (m_strError == "") ? TRUE : FALSE;

	if (bReply == TRUE)
	{
		ShowStatistics();											// show import stats
	}

	return bReply;
}

//******************************************************************
// get CardNo for new record,
// on entry exception string is blank, on exit exception string set if no cardno set

void CImportSimsFile::GetNewCardNo( CString& strCardNo )
{
	strCardNo = "";													// clear number

	if ( m_pSims->m_nNewIntakeAccount == nSIMS_NEWINTAKE_NEXTFREE_INDEX )
	{
		CSQLRepositoryAccount RepoAccount;
		__int64 nCardNo = RepoAccount.GetNextFreeUserID(_atoi64(m_strStartFromUserID), _atoi64(System.GetHighCardNo()), NULL).GetSQLResult();
			
		strCardNo = FormatInt64Value(nCardNo);

		if (0 == nCardNo)
		{
			m_strExceptionComment = "No more accounts";
		}
		else
		{
			m_strStartFromUserID = CSQLHelpers::IncrementUserID(strCardNo, System.GetHighCardNo());		// next new start from position
		}
	}
	else
	{
		CString strCardID;

		if (m_pSims->m_nNewIntakeAccount == nSIMS_NEWINTAKE_ADNO_INDEX)
		{
			strCardID = m_simsFile.GetAdno();	// create new account using Adno
		}
		else
		{
			strCardID = m_simsFile.GetPID();	// create new account using Person-id
		}

		if (System.IsValidCardNo(strCardID) == FALSE)
		{
			m_strExceptionComment.Format("Invalid %s reference %s", m_pSims->GetNewIntakeAddedByText(), strCardID);
		}
		else
		{
			CSQLRowAccountFull RowAccount;
			RowAccount.SetUserID(_atoi64(strCardID));

			CSQLRepositoryAccount RepoAccount;
			if (RepoAccount.SelectRow(RowAccount, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
			{
				if (MatchRecord(RowAccount) == FALSE)						// see if fields match
				{
					m_strExceptionComment.Format("Account %s already exists", System.FormatCardNo(strCardID));
				}
			}
		}

		if (m_strExceptionComment == "")							// check for any errors
		{
			strCardNo = strCardID;
		}
	}
}

//**********************************************************************

void CImportSimsFile::ImportLine ( const char* szCardNo ) 
{
	CString strCardNo = szCardNo;

	if (strCardNo == "")
	{
		++m_nIgnored;
	}
	else
	{
		CSQLRepositoryAccount RepoAccount;

		bool bOk = TRUE;	
		bool bExists = (RepoAccount.RowExists(_atoi64(strCardNo), NULL).GetSQLResult() != 0);
		
		if ( (FALSE == bExists) && ( nIMPORT_METHOD_EXISTONLY == m_nImportMethod ) )	// see if must exist
		{
			bOk = FALSE;
		}
		else if ( ( TRUE == bExists ) && ( nIMPORT_METHOD_NEWONLY == m_nImportMethod ) )		// only new recod allowed
		{
			bOk = FALSE;
		}

		if ( TRUE == bOk )
		{
			++m_nUsed;

			if ( bExists == FALSE )
			{
				++m_nAdded;
				CSQLRowAccountFull RowAccount;
				RowAccount.SetUserIDString(strCardNo);
				RepoAccount.InsertRow(RowAccount, NULL);

				SetRecord(RowAccount);											// update new record
				RowAccount.CopyGroupMaxSpendSettings();
				RepoAccount.UpdateRow(RowAccount, NULL);
				
				CSQLAuditRecord atc;
				atc.Set( &RowAccount );
				atc.SetApplicationNo ( APPNO_INSERTRECORDSIMS );
				atc.SaveNewLine();
			}
			else
			{
				CSQLRowAccountFull RowAccount;
				RowAccount.SetUserIDString(strCardNo);
				RepoAccount.SelectRow(RowAccount, NULL);
				
				int nOldGroupNo = RowAccount.GetGroupNo();

				SetRecord(RowAccount);											// update existing record

				if (nOldGroupNo != RowAccount.GetGroupNo())
				{
					RowAccount.CopyGroupMaxSpendSettings();
				}

				RepoAccount.UpdateRow(RowAccount, NULL);
			}
		}
		else
		{
			++m_nSkipped;
		}
	}
}
	
/**********************************************************************/

void CImportSimsFile::SetRecord(CSQLRowAccountFull& RowAccount)
{
	int nField;
//
	if ( m_simsFile.HavePID() == TRUE )
	{
		if ( ( nField = m_pSims->m_nPIDUserTextNo ) > 0 )					// 0=not defined, 1=Member, 2-9=Info1-8 
		{
			if (nField == 1)
			{
				RowAccount.SetMemberID(m_simsFile.GetPID());
			}
			else
			{
				RowAccount.SetInfo(nField - 1, m_simsFile.GetPID());	// info1 - info8
			}
		}
	}

//

	if ( m_simsFile.HaveUPN() == TRUE )
	{
		if ((nField = m_pSims->m_nUPNUserTextNo) > 0)					// 0=not defined, 1=Member, 2-9=Info1-8  
		{
			if (nField == 1)
			{
				RowAccount.SetMemberID(m_simsFile.GetUPN());
			}
			else
			{
				RowAccount.SetInfo(nField - 1, m_simsFile.GetUPN());	// info1 - info8
			}
		}
	}

//
	if ( m_simsFile.HaveSimsNo() == TRUE )
	{
		if ((nField = m_pSims->m_nSIMSUserTextNo) > 0)					// 0=not defined, 1=Member, 2-9=Info1-8  
		{
			if (nField == 1)
			{
				RowAccount.SetMemberID(m_simsFile.GetAdno());
			}
			else
			{
				RowAccount.SetInfo(nField - 1, m_simsFile.GetAdno());	// info1 - info8
			}
		}
	}

//
	if (m_simsFile.HaveYear() == TRUE)
	{
		if ((nField = m_pSims->m_nYearContactNo) > 0)					// 0=not defined or Info1-8
		{
			RowAccount.SetInfo(nField, m_simsFile.GetYear());
		}
	}

//
	if ( m_simsFile.HaveReg() == TRUE )
	{
		if ((nField = m_pSims->m_nRegContactNo) > 0)					// 0=not defined or Info1-8
		{
			RowAccount.SetInfo(nField, m_simsFile.GetReg());
		}
	}

//
	if ( m_simsFile.HaveEmail() == TRUE )
	{
		if ( m_pSims->m_bSaveEmail == TRUE )
		{
			if ((nField = m_pSims->m_nEmailContactNo) == 0)
			{
				RowAccount.SetEmail(m_simsFile.GetEmail());
			}
			else
			{
				RowAccount.SetInfo(nField, m_simsFile.GetEmail());
			}
		}
	}

//
	if ( m_simsFile.HaveDOB() == TRUE )
	{
		switch ( m_pSims->m_nDobNo )								// 0-not defined, 1=Dob1
		{
		case 1:		RowAccount.SetDOB ( m_simsFile.GetDOB() );	break;
		}
	}

//

	if ( m_simsFile.HaveCardName()	== TRUE )	RowAccount.SetUsername (  m_simsFile.GetCardName() );	//29/07/2016 - v1.12i 
	if ( m_simsFile.HaveForename()	== TRUE )	RowAccount.SetForename ( m_simsFile.GetForename() );
	if ( m_simsFile.HaveSurname()	== TRUE )	RowAccount.SetSurname ( m_simsFile.GetSurname() );
	if ( m_simsFile.HaveGender()	== TRUE )	RowAccount.SetGender ( m_simsFile.GetGender() );

	if ( m_simsFile.HaveFSM() == TRUE )									// see if FSM field supplied
	{
		if (m_simsFile.IsFSM() == TRUE)								// see is FSM set
		{
			if (m_pSims->m_nFSMGroupNo != 0)
			{
				RowAccount.SetGroupNo(m_pSims->m_nFSMGroupNo);	// yes - set to default FSM group
			}
		}
		else															// FSM not set
		{
			if (RowAccount.GetGroupNo() == m_pSims->m_nFSMGroupNo)	// see if is was previoulsy FSM group
			{
				if (m_pSims->m_nFSMRevertGroupNo != 0)						// is not FSM see if is
				{
					RowAccount.SetGroupNo(m_pSims->m_nFSMRevertGroupNo);	// see to default not FSM group
				}
			}
		}
	}
}

//**********************************************************************
// check sims data again record data to see if same record 

bool CImportSimsFile::MatchRecord(CSQLRowAccountFull& RowAccount)
{
	int nField;
	CString strCardData;

	if ( m_simsFile.HavePID() == TRUE )
	{
		if ( ( nField = m_pSims->m_nPIDUserTextNo ) > 0 )					// 0 = not defined, 1=MemberID, 2-9= Info1-8 
		{
			if (nField == 1)
			{
				strCardData = RowAccount.GetMemberID();
			}
			else
			{
				strCardData = RowAccount.GetInfo(nField - 1);	// info1 - info8
			}
			if ( strCardData != "" )
			{
				CString strSimsData = m_simsFile.GetPID();

				if (strCardData != strSimsData)
				{
					return FALSE;
				}
			}
		}
	}

	if ( m_simsFile.HaveUPN() == TRUE )
	{
		if ( ( nField = m_pSims->m_nUPNUserTextNo ) > 0 )					// 0 = not defined, 1=MemberID, 2-9= Info1-8 
		{
			if (nField == 1)
			{
				strCardData = RowAccount.GetMemberID();
			}
			else
			{
				strCardData = RowAccount.GetInfo(nField - 1);	// info1 - info8
			}

			if ( strCardData != "" )
			{
				CString strSimsData = m_simsFile.GetUPN();

				if (strCardData != strSimsData)
				{
					return FALSE;
				}
			}
		}
	}
	
	return TRUE;
}

/**********************************************************************/
