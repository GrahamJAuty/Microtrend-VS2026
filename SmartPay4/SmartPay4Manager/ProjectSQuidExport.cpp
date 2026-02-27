//*******************************************************************
#include "..\SmartPay4Shared\MembershipDatabase.h"
#include "..\SmartPay4Shared\RepmanHandler.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SmartPay4Shared\SQLTable_WebPaymentOptions\SQLRepositoryWebPaymentOptions.h"
#include "..\SmartPay4Shared\SQLDb.h"
#include "..\SmartPay4Shared\WorkingDlg.h"
//*******************************************************************
#include "..\SmartPay4ManagerBgnd\Squid.hpp"
#include "..\SmartPay4ManagerBgnd\WebPaymentDefines.h"
//*******************************************************************
#include "CardPickerDlg.h"
#include "ReportCSVFile.h"
#include "WebPaymentReporter.h"
//*******************************************************************
#include "..\SmartPay4ManagerBgnd\sQuidExport.h"
//*******************************************************************

//**********************************************************************
//*** Link accounts to sQuid *******************************************
// on entrt ListFile conatins list of accounts to link
//**********************************************************************

void CSquidExport::LinkAccounts ( const char* szListFile )
{
	CString strTitle = "sQuid Link Accounts"; 
	if ( m_data.IsSet() == FALSE )
	{
		Prompter.Error( szINTERFACENOTSET, strTitle );
		return;
	}

	CSQLRepositoryWebPaymentOptions webOptions;
	webOptions.LoadRow(NULL);

	if ( webOptions.m_Row.GetPromptBeforeAccessFlag() == TRUE)
	{
		if ( Prompter.YesNo( szINTERNETACCESSNOW, strTitle ) == IDNO )
			return;
	}

	CFileRecycler::SendToRecycleBin ( m_strTempLinkedFile );
	CFileRecycler::SendToRecycleBin ( m_strTempExceptionFile );

	int nCount = LinkNewAccounts ( strTitle, szListFile );		// link any unlinked accounts

	CString strMsg;
	if ( nCount == 0 )	strMsg = "No accounts were found that required linking";
	else				strMsg.Format ( "Number of new account links generated = %d", nCount );

	if ( m_nFailures > 0 )
	{
		strMsg += szMSG_HAVEEXCEPTIONS;					// "\n\nHowever, exceptions were found.\n\nDo you wish to view the exceptions y/n";

		if ( Prompter.YesNo( strMsg, strTitle, TRUE ) == IDYES )
		{
			CCSVFileReport report ( "R221B", m_strTempExceptionFile, m_pParent );
			report.SetMode ( P_NOWAIT );
			report.Show();
		}

		strMsg = "";										// clear failure message
	}

	if ( nCount == 0 && m_nFailures == 0 )
		Prompter.Info( strMsg, strTitle );

	else if ( nCount > 0 )
	{
		if ( strMsg != "" )
			strMsg += "\n\n";

		strMsg += "Do you wish to view the list of newly linked accounts?";
	
		if ( Prompter.YesNo( strMsg, strTitle, TRUE ) == IDYES )
		{
			CCSVFileReport report ( "R221A", m_strTempLinkedFile, m_pParent );
			report.SetMode ( P_NOWAIT );
			report.Show();
		}
	}
}

//*********************************************************************

int CSquidExport::LinkNewAccounts(const char* szTitle, const char* szListFile)
{
	int nLinkedCount = 0;
	int nRecordCount = 0;
	m_nFailures = 0;

	CWorkingDlg progress(szINTERNETMESSAGE);
	progress.Create();

	CSSFile fileList;
	fileList.Open(szListFile, "rb");
	CString strUserID;

	CSSSquid squid(&m_data);
	CString strLoginReplyFile = System.GetWebPaymentPath("LinkAccountReply.xml");

	while (fileList.ReadString(strUserID) == TRUE)
	{
		CSQLRowAccountFull RowAccount;
		RowAccount.SetUserIDString(strUserID);
		CSQLRepositoryAccount RepoAccount;
		RepoAccount.SelectRow(RowAccount,NULL);
	
		CString strMsg = "";
		strMsg.Format("Requests sent [ %d ]  :  Links received [ %d ]", nRecordCount, nLinkedCount);
		if (progress.SetCaption2Message(strMsg) == TRUE)
		{
			break;
		}

		++nRecordCount;
		m_strLinkError = "";

		CString strReference1 = m_data.GetSquidLinkReference1(RowAccount);	// see if have relevant link fields set in database
		CString strReference2 = m_data.GetSquidLinkReference2(RowAccount);

		if (strReference1 == "" && strReference2 == "")
		{
			m_strLinkError = "No link reference data";
		}
		else
		{
			if ((m_strLinkError = squid.RetrieveCardDetails(strLoginReplyFile, strReference1, strReference2)) == "")
			{
				++nLinkedCount;

				m_data.SetSquidCardNo(RowAccount, squid.GetSquidCardNo());		// save returned sQuid Account card No
				m_data.SetSquidCVV(RowAccount, squid.GetSquidCVV());		// save returned sQuid Account card CVV

				CString strMemberID = squid.GetSquidAccountNo();
				RowAccount.SetMemberID(strMemberID);					// MemberID = 1st sQuid purseAccountNumber
				CSQLRepositoryAccount RepoAccount;
				RepoAccount.UpdateRow(RowAccount,NULL);
				
				SaveLinkReference(RowAccount);								// log when link was done
			}

			if (m_strLinkError != "")
			{
				++m_nFailures;
			}

			SaveLinkNotification(RowAccount, m_strLinkError);
		}
	}

	fileList.Close();

	progress.DestroyWindow();

	if (m_data.m_bKeepXmlFiles == FALSE)
	{
		CFileRecycler::SendToRecycleBin(strLoginReplyFile);			// delete last xml login reply file
	}

	return nLinkedCount;
}

//*********************************************************************
//*********************************************************************
//*********************************************************************

void CSquidExport::DisassociateCards()
{
	CString strTitle = "Disassociate Cards"; 
	if ( m_data.IsSet() == FALSE )
	{
		Prompter.Error( szINTERFACENOTSET, strTitle );
		return;
	}

	CFileRecycler::SendToRecycleBin ( m_strTempLinkedFile );
	CFileRecycler::SendToRecycleBin ( m_strTempExceptionFile );

	int nCount = DisassociateCards ( strTitle );

	CString strMsg;
	if ( nCount == 0 )	strMsg = "No cards have been disassociated";
	else				strMsg.Format ( "Number of cards disassociated = %d", nCount );

	if ( m_nFailures > 0 || nCount == 0 )
	{
		if ( m_nFailures > 0 )
			strMsg += szMSG_HAVEEXCEPTIONS;					// "\n\nHowever, exceptions were found.\n\nDo you wish to view the exceptions y/n";

		if ( Prompter.YesNo( strMsg, strTitle, TRUE ) == IDYES )
		{
			CCSVFileReport report ( "R221B", m_strTempExceptionFile, m_pParent );
			report.Show();
		}
	}

	if ( nCount > 0 )
	{
		strMsg = "Do you wish to view the list of disassociated accounts?";
	
		if ( Prompter.YesNo( strMsg, strTitle, TRUE ) == IDYES )
		{
			CCSVFileReport report ( "R221A", m_strTempLinkedFile, m_pParent );
			report.Show();
		}
	}
}

//*********************************************************************

int CSquidExport::DisassociateCards(const char* szTitle)
{
	CWorkingDlg progress(szMSG_SCANNINGDATABASE);
	progress.Create();

	bool bCancel = FALSE;
	
	CArray<CSQLRowAccountList, CSQLRowAccountList> arrayAccountList;
	{
		CSQLRowAccountList dummy;
		arrayAccountList.Add(dummy);
	}
	
	int nCount = 0;
	CSQLRecordSetAccountFull RecordSet(NULL, RSParams_AccountFull_NoParams{}, FALSE);

	CSQLRowAccountFull RowAccount;
	while (RecordSet.StepSelectAll(RowAccount) == TRUE)
	{
		if (progress.SetCaption2RecordsChecked(++nCount) == TRUE)
		{
			bCancel = TRUE;
			break;
		}

		CString strSquidCardNo = m_data.GetSquidCardNo(RowAccount);		// see if squid card number already exists
		if (strSquidCardNo != "")
		{
			bool bIsInactive = RowAccount.GetInactive();
			bool bIsExpired = RowAccount.HasUserExpired();

			if (bIsInactive || bIsExpired)
			{
				CSQLRowAccountList RowList;
				RowList.SetUserID(RowAccount.GetUserID());
				RowList.SetGroupNo(RowAccount.GetGroupNo());
				RowList.SetUsername(RowAccount.GetUsername());
				RowList.SetForename(RowAccount.GetForename());
				RowList.SetExpiryDate(RowAccount.GetExpiryDate());
				RowList.SetInactive(RowAccount.GetInactive());
				RowList.SetAlertCode(RowAccount.GetAlertCode());
				RowList.SetMemberID(RowAccount.GetMemberID());
				arrayAccountList.Add(RowList);
			}
		}
	}

	progress.DestroyWindow();

	if (( TRUE == bCancel ) || ( arrayAccountList.GetSize() < 2 ) )
	{
		return 0;
	}
	
	m_nFailures = 0;
	int nRecordCount = 0;
	int nLinkedCount = 0;
	CString strReportList = m_data.GetDisassociateFilename();

	CCardPickerDlg dlg(szTitle, strReportList, arrayAccountList, m_pParent);
	if (dlg.DoModal() == IDOK)
	{
		CSQLRepositoryWebPaymentOptions webOptions;
		webOptions.LoadRow(NULL);

		int nReply = IDYES;
		if (webOptions.m_Row.GetPromptBeforeAccessFlag() == TRUE)
		{
			nReply = Prompter.YesNo(szINTERNETACCESSNOW, szTitle);
		}

		if (nReply == IDYES)
		{
			CWorkingDlg WorkingDlg(szINTERNETMESSAGE,FALSE);
			WorkingDlg.Create();
			WorkingDlg.SetCaption2Message("Disassociating cards");

			CString strLoginReplyFile = System.GetWebPaymentPath("DisassociateReply.xml");

			CSSSquid squid(&m_data);

			CSSFile fileList;
			bool bReply = fileList.Open(strReportList, "rb");

			CString strUserID;
			while (fileList.ReadString(strUserID) == TRUE)
			{
				CSQLRepositoryAccount RepoAccount;
				if (RepoAccount.RowExists(_atoi64(strUserID), NULL).GetSQLResult() != 0)
				{
					++nRecordCount;
					m_strLinkError = squid.DisassociateCard(strLoginReplyFile, m_data.GetSquidCardNo(RowAccount));

					if (m_strLinkError == "")
					{
						++nLinkedCount;
					}
					else
					{
						++m_nFailures;
					}
				}

				SaveLinkNotification(RowAccount, m_strLinkError);

				CString strMsg = "";
				strMsg.Format("Requests send [%d] : Responses [%d]", nRecordCount, nLinkedCount);
				WorkingDlg.SetCaption2(strMsg);
			}
			fileList.Close();

			progress.DestroyWindow();

			if (m_data.m_bKeepXmlFiles == FALSE)
			{
				CFileRecycler::SendToRecycleBin(strLoginReplyFile);			// delete last xml login reply file
			}

			CFileRecycler::SendToRecycleBin(strReportList);
		}
	}

	return nLinkedCount;
}

//**********************************************************************
