//******************************************************************************
#include "..\SmartPay4Shared\MembershipDatabase.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
//******************************************************************************
#include "..\SmartPay4ManagerBgnd\ParentMail.hpp"
#include "..\SmartPay4ManagerBgnd\WebPaymentDefines.h"
//******************************************************************************
#include "ReportCSVFile.h"
//******************************************************************************
#include "..\SmartPay4ManagerBgnd\ParentMailExport.h"
//******************************************************************************

//**********************************************************************
//*** Link accounts to ParentMail **************************************
//**********************************************************************

int CParentMailExport::LinkNewAccounts(const char* szListFile, CWorkingDlg* pWorking)
{
	m_nFailedCounter = 0;
	int nTotalCount = 0;
	int nLinkedCount = 0;

	CSSFile fileList;
	fileList.Open(szListFile, "rb");
	CString strUserID;

	CSSParentMail pm(&m_data);
	CString strLoginReplyFile = System.GetWebPaymentPath("LinkNewAccountReply.xml");

	int nCount = 0;
	while (fileList.ReadString(strUserID) == TRUE)
	{
		CSQLRowAccountFull RowAccount;
		RowAccount.SetUserIDString(strUserID);
		CSQLRepositoryAccount RepoAccount;
		RepoAccount.SelectRow(RowAccount,NULL);
		
		CString strMsg = "";
		strMsg.Format("Requests sent [ %d ]  :  Links received [ %d ]", nTotalCount, nLinkedCount);

		if (pWorking->SetCaption2Message(strMsg) == TRUE)
		{
			break;
		}

		CString strType = "S";							// assume staff
		CString strFormName = "";							// staff matched only on Forename & Surname
		CString strAdditionalData = "";
		CString strXMLDoB = "";

		if (m_data.IsStaffGroup(RowAccount.GetGroupNo()) == FALSE)	// if not staff - then pupil
		{
			strType = "P";								// set pupil type
			strFormName = RowAccount.GetInfo(m_data.m_nFormNameRefField);
			strAdditionalData = RowAccount.GetInfo(m_data.m_nAdditionalRefField);

			CSSDate date(RowAccount.GetDOB());
			if (date.IsSet() == TRUE)
			{
				strXMLDoB = ::FormatXmlDate(&date);				// yyyy-mm-dd
			}
		}

		++nTotalCount;

		m_strError = pm.LinkNewAccount(strLoginReplyFile, RowAccount.GetForename(), RowAccount.GetSurname(), strXMLDoB, RowAccount.GetUserIDString(), strAdditionalData, strFormName, strType);
		if (m_strError == "")
		{
			CString strException = "";
			RowAccount.SetMemberID(pm.GetAccountID());			// set MemberID if any
			RepoAccount.UpdateRow(RowAccount,NULL);
		
			if (pm.GetAccountID() == "")
			{
				strException = "No match found";
			}
			else
			{
				AuditOpeningBalance(RowAccount);
				++nLinkedCount;
			}
			SaveLinkNotification(RowAccount, strException);
		}
		else
		{
			SaveLinkNotification(RowAccount, m_strError);
		}
	}

	fileList.Close();

	if (m_data.m_bKeepXmlLogin == FALSE)
	{
		CFileRecycler::SendToRecycleBin(strLoginReplyFile);
	}

	m_nFailedCounter = nTotalCount - nLinkedCount;

	return nLinkedCount;
}

//*******************************************************************
//*** Called from main menu******************************************
// On entry - ListFile contains list of unlinked accounts numbers
//*******************************************************************

void CParentMailExport::LinkAccounts ( const char* szListFile )
{
	CString strTitle = "ParentMail +Pay Link Accounts"; 
	if ( m_data.IsSet() == FALSE )
	{
		Prompter.Error( szINTERFACENOTSET, strTitle );
		return;
	}

	if ( m_webOptions.m_Row.GetPromptBeforeAccessFlag() == TRUE)
	{
		if ( Prompter.YesNo( szINTERNETACCESSNOW, strTitle ) == IDNO )
			return;
	}

	CWorkingDlg progress ( strTitle );
	progress.Create();
	progress.SetCaption1 ( szINTERNETMESSAGE  );

	CFileRecycler::SendToRecycleBin ( m_strTempLinkedFile );
	CFileRecycler::SendToRecycleBin ( m_strTempExceptionFile );

	SendUpdates ( FALSE, FALSE );									//remove any updates not yet linked to an account ( no eo

	int nCount1 = RequestLinkedAccounts ( &progress );				// see if any manual links have been done
	int nCount2 = LinkNewAccounts ( szListFile, &progress );		// try & link any unlinked accounts
	
	progress.DestroyWindow();

	CString strMsg = "";
	int nCount = nCount1 + nCount2;

	if (nCount == 0)
	{
		strMsg = "No new links were received or accounts found that required linking.";
	}
	else	
	{
		CString strMsg1;
		if ( nCount1 != 0 )
		{
			CString strMsg1 = "";
			strMsg1.Format ( "Number of manually linked accounts received = %d.", nCount1 );
			strMsg += strMsg1;
		}
		
		if (nCount2 != 0)
		{
			if (nCount1 != 0)
			{
				strMsg += "\n";
			}

			strMsg1.Format("Number of new account link requests processed = %d.", nCount2);
			strMsg += strMsg1;
		}

		SendUpdates ( TRUE, FALSE );								// send updates to web site with starting balances (not EndOfDay )
	}

	if ( m_nFailedCounter > 0 )
	{
		strMsg += szMSG_HAVEEXCEPTIONS;								// "\n\nHowever, exceptions were found.\n\nDo you wish to view the exceptions y/n";

		if ( Prompter.YesNo( strMsg, strTitle, TRUE ) == IDYES )
		{
			CCSVFileReport report ( "R221B", m_strTempExceptionFile, m_pParent );
			report.SetMode ( P_NOWAIT );
			report.Show();
		}
		strMsg = "";												// clear failure message
	}

	if (nCount == 0 && m_nFailedCounter == 0)
	{
		Prompter.Info(strMsg, strTitle);
	}
	else if ( nCount > 0 )
	{
		if (strMsg != "")
		{
			strMsg += "\n\n";
		}
		strMsg += "Do you wish to view the list of newly linked accounts?";
	
		if ( Prompter.YesNo( strMsg, strTitle, TRUE ) == IDYES )
		{
			CCSVFileReport report ( "R221A", m_strTempLinkedFile, m_pParent );
			report.SetMode ( P_NOWAIT );
			report.Show();
		}
	}
}

//**********************************************************************
