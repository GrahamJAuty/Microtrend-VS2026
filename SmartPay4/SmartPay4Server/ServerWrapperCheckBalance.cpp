//**********************************************************************
#include "..\SmartPay4Shared\AlertText.h"
#include "..\SmartPay4Shared\AllergyText.h"
#include "..\SmartPay4Shared\ImageHandler.h"
#include "..\SmartPay4Shared\SmartPayLockManager.h"
#include "..\SmartPay4Shared\SQLAuditRecord_base.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLPurse2Handler.h"
#include "..\SmartPay4Shared\SystemData.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SmartPay4Shared\SQLTable_AuditPeriod\SQLRowSetAuditPeriod.h"
#include "..\SmartPay4Shared\SQLTable_BioRegister\SQLRepositoryBioRegister.h"
#include "..\SmartPay4Shared\SQLTable_EposTerminal\SQLRepositoryEposTerminal.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRowAccountFull.h"
#include "..\SmartPay4Shared\SQLTable_Group\SQLRepositoryGroup.h"
#include "..\SmartPay4Shared\SQLTable_PINNumber\SQLRepositoryPINNumber.h"
#include "..\SmartPay4Shared\SQLTable_PurchaseControlStamp\SQLRepositoryPurchaseControlStamp.h"
#include "..\SmartPay4Shared\SQLTable_PurchaseControlRule\SQLRepositoryPurchaseControlRule.h"
//**********************************************************************
#include "ServerSubs.h"
#include "ServerWrapper.h"
//**********************************************************************
static const char* szSTATUS_COMMAND = "CMD=STATUS";
//**********************************************************************

CPurse1AvailableSpendInfo::CPurse1AvailableSpendInfo()
{
	Reset();
}

//*****************************************************************

void CPurse1AvailableSpendInfo::Reset()
{
	m_dBalanceP1 = 0.0;
	m_dSpendLimitP1 = 0.0;
	m_dOverdraftP1 = 0.0;
	m_dAvailableSpendP1 = 0.0;
	m_dBalanceP2 = 0.0;
}

//*****************************************************************
//*****************************************************************
//*****************************************************************

CPurse3AvailableSpendInfo::CPurse3AvailableSpendInfo()
{
	Reset();
}

//*****************************************************************

void CPurse3AvailableSpendInfo::Reset()
{
	m_dBalanceP3 = 0.0;
	m_dSpendLimitP3 = 0.0;
	m_dOverdraftP3 = 0.0;
	m_dAvailableSpendP3 = 0.0;
}

//*****************************************************************
//*****************************************************************
//*****************************************************************

void CServerWrapper::RequestCheckBalance()
{
	for (int nThread = 1; nThread <= EPOSREQPATH_COUNT; nThread++)
	{
		m_LoyReqThreadManager.CheckThread(nThread);

		LONGLONG nThreadTime = m_LoyReqThreadManager.GetThreadTime();

		if (nThreadTime >= 3000)
		{
			CString strMsg = "";
			strMsg.Format("==> Folder Set %2.2d Slow REQ response (%I64d)", 
				nThread, 
				nThreadTime / 1000);

			LogAction2(strMsg);
		}

		if (m_LoyReqThreadManager.GetRequestLineCount() != 0)
		{
			Server.SetCurrentEposPathSet(nThread);

			if ((System.GetExternalAccountFlag() == TRUE) && (Server.GetCurrentExternalRequestPathFlag() == TRUE))
			{
				ProcessExternalCommand();
			}
			else
			{
				m_bIsKioskRequest = FALSE;
				m_bIsVendingRequest = FALSE;
				m_bIsEposRequest = FALSE;

				CString strFilename = m_LoyReqThreadManager.GetREQFilename();

				if (strFilename.Left(6) == "LOYBIO")
				{
					m_bIsKioskRequest = TRUE;
				}
				/*
				else if (strFilename.Left(6) == "LOYVND")
				{
					m_bIsVendingRequest = TRUE;
				}
				*/
				else
				{
					m_bIsEposRequest = TRUE;
				}

				m_dateToday.SetCurrentDate();
				m_timeToday.SetCurrentTime();

				if (strFilename.Left(5) == "RECON")
				{
					ProcessReconRequest();
				}
				else if (m_LoyReqThreadManager.IsJSONRequest())
				{
					ProcessJSONRequest();
				}
				else
				{
					UpdatePluInfo();
					//UpdateOperatorInfo();
					UpdateEposTerminalInfo();
					UpdatePurchaseControlInfo();
					ProcessRequest();
				}
			}
		}

		m_LoyRspThreadManager.CheckThread(nThread);
	}

	for (int nSetNo = 1; nSetNo <= EPOSREQPATH_COUNT; nSetNo++)
	{
		m_LoyReqTidyUpThreadManager.CheckThread(nSetNo);
	}
}

//**********************************************************************

void CServerWrapper::ProcessRequest()
{
	{
		CString strMsg = "";
		strMsg.Format("==> REQ: %s",
			(const char*) m_LoyReqThreadManager.GetREQFilepath());

		LogAction2(strMsg);
	}

	for (int n = 0; n < m_LoyReqThreadManager.GetRequestLineCount(); n++)
	{
		CCSV csvLine(m_LoyReqThreadManager.GetRequestLine(n));

		CString strBuf = csvLine.GetString(0);

		if (strBuf.Left(3) == "ID=")
		{
			CreateRequestReply(strBuf.Mid(3), &csvLine);
		}
		else if (strBuf.Left(4) == "PIN=")
		{
			if (System.GetAllowPINNumbersFlag() == TRUE)
			{
				ProcessPINRequest(strBuf.Mid(4), csvLine);
			}
			else
			{
				CreateNoIDErrorReply();
			}
		}
		else if (strBuf.Left(3) == "MF=" || strBuf.Left(4) == "MF2=")				// check for registraton request
		{
			int nOffset = (strBuf.Left(4) == "MF2=") ? 4 : 3;						// MF= use 'ExtRef') , MF2= use 'ExtRef2'
			int nMiFareDbNo = (nOffset == 4) ? 2 : 1;

			//REDIRECT MF2= TO PIN NUMBER LOOKUP IF ENABLED
			if ((4 == nOffset) && (System.GetAllowPINNumbersFlag() == TRUE))
			{
				ProcessPINRequest(strBuf.Mid(4), csvLine);
			}
			else if (csvLine.GetSize() == 1)											// see if MiFare number used instead of accountID
			{
				CString strUserID = System.TranslateMiFareNo(strBuf.Mid(nOffset), nMiFareDbNo);
				CreateRequestReply(strUserID, &csvLine);
			}
			else																	// no - do registration
			{
				CSmartPayLockRequest LockRequest;
				LockRequest.m_bWantLockServerInner = TRUE;

				CSmartPayLockManager LockManager;
				int nLockError = LockManager.AcquireLocks(LockRequest);
	
				if (LOCKMANAGER_OK == nLockError)									// see if server is pause
				{
					ProcessRegistration(strBuf.Mid(nOffset), nMiFareDbNo, &csvLine);
				}
				else
				{
					CreatePausedReply(strBuf.Mid(nOffset));	// can't do so send server pause reply
					break;
				}

				LockManager.LogError(nLockError);
			}
		}
		else if (strBuf == szSTATUS_COMMAND)
		{
			ProcessCommand(&csvLine);
		}
		else
		{
			// N1={surname}, N2=.. ( search for user )
			CreateSearchRequestReply(csvLine);
		}
	}

	if (m_LoyReqThreadManager.GetResponseLineCount() > 2)
	{
		int nThread = m_LoyReqThreadManager.GetCurrentThread();

		m_LoyRspThreadManager.QueueResponseHeader(nThread);
		for (int n = 0; n < m_LoyReqThreadManager.GetResponseLineCount(); n++)
		{
			m_LoyRspThreadManager.QueueResponseLine(nThread, m_LoyReqThreadManager.GetResponseLine(n));
		}
		m_LoyRspThreadManager.CheckThread(nThread);

		{
			CString strMsg = "";
			strMsg += "<== RSP: ";
			strMsg += m_LoyReqThreadManager.GetResponseLine(1);
			LogAction2(strMsg);
		}
	}	
}

//**********************************************************************

void CServerWrapper::ProcessPINRequest(CString strPIN, CCSV& csvLine)
{
	CString strUserID = "";
	
	if (CSQLRowPINNumber::IsValidPINNumber(strPIN) == TRUE)
	{
		CSQLRowPINNumber RowPIN;
		RowPIN.SetPINNumber(strPIN);
	
		CSQLRepositoryPINNumber RepoPIN;
		if (RepoPIN.SelectRow(RowPIN, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
		{
			strUserID = RowPIN.GetUserID();
		}
	}

	if (strUserID != "")
	{
		CSQLRepositoryPINNumber RepoPIN;
		RepoPIN.UpdatePINNumberUsage(strPIN, NULL);
		CreateRequestReply(strUserID, &csvLine);
	}
	else
	{
		CreateNoIDErrorReply();
	}
}

//**********************************************************************

void CServerWrapper::CreateNoIDErrorReply()
{
	CCSV csvReply;
	csvReply.Add("ID=");
	csvReply.Add(StringErrorCodeUnknownID());
	csvReply.Add(StringErrorTextUnknownID());
	m_LoyReqThreadManager.AddResponseLine(csvReply.GetLine());
}

//**********************************************************************
// szEposUserID maybe either a MiFare number of a UserID
// NOTE: pCsvLine maybe NULL

void CServerWrapper::CreateRequestReply(CString strEposUserID, CCSV* pCsvLine )
{
	CCSV csvReply;
	{
		//FIRST LINE OF REPLY BEGINS WITH USERID
		CString strBuf = "";
		strBuf += "ID=";
		strBuf += strEposUserID;
		csvReply.Add(strBuf);
	}
	if (strEposUserID == Server.GetOfflineSpendID())				// szSPENDLIMIT_USERID
	{
		CString strMsg = "";
		strMsg.Format("AS=%.0f", Server.GetOfflineSpendLimit() * 100);	// Default Daily Spend Limit
		csvReply.Add(strMsg);
	}
	else
	{
		CString strAccountID = strEposUserID;

		// user ID translate may return blank
		bool bExists = FALSE;
		__int64 nLoadAccountError = SQLCRUD_ERR_NOMATCH;

		if (strAccountID != "")
		{
			CSQLRowAccountFull RowAccount;
			RowAccount.SetUserID(_atoi64(strAccountID));
			CSQLRepositoryAccount RepoAccount;
			nLoadAccountError = RepoAccount.SelectRow(RowAccount, NULL).GetSQLError();

			bool bGotODGS = FALSE;

			if ( SQLCRUD_ERR_NONE == nLoadAccountError)
			{
				bExists = TRUE;

				if (pCsvLine != NULL)									// check have a csv line to scan
				{
					bool bGotBI = FALSE;
				
					for (int i = 1; i < pCsvLine->GetSize(); i++)
					{
						CString strBuf = pCsvLine->GetString(i);
						
						if (FALSE == bGotBI)
						{
							if (strBuf.Left(4) == "BI=0")					
							{
								LogBioUnRegister(RowAccount);
								bGotBI = TRUE;
							}
							else if (strBuf.Left(4) == "BI=1")					// v1.06b 04/12/2015
							{
								LogBioUse(RowAccount);
								bGotBI = TRUE;
							}
							else if (strBuf.Left(4) == "BI=2")
							{
								LogBioRegister(RowAccount);
								bGotBI = TRUE;
							}
						}

						if (FALSE == bGotODGS)
						{
							if (strBuf.Left(6) == "ODGS=1")
							{
								bGotODGS = TRUE;
							}
						}

						if ((TRUE == bGotBI) && (TRUE == bGotODGS))
						{
							break;
						}
					}
				}

				SaveRequestDetails(RowAccount, &csvReply, bGotODGS);					// record found
			}
		}

		if ( (SQLCRUD_ERR_NONE != nLoadAccountError) && (SQLCRUD_ERR_NOMATCH != nLoadAccountError) )
		{
			csvReply.Add(StringErrorCodeDbAccess());

			CString strMsg = "";
			strMsg.Format("EM=SQL Database Access Error (%I64d)", nLoadAccountError);
			csvReply.Add(strMsg);
		}
		else if (FALSE == bExists)
		{
			if (CanAddNewRecord(strAccountID) == TRUE)	// see if new record should be added
			{
				CSmartPayLockRequest LockRequest;
				LockRequest.m_bWantLockServerInner = TRUE;	

				CSmartPayLockManager LockManager;
				int nLockError = LockManager.AcquireLocks(LockRequest);	
				
				if (LOCKMANAGER_OK == nLockError)							// see if server is paused
				{
					AddNewRecord(strAccountID, pCsvLine, &csvReply);		// if paused means database\audit in use
				}
				else
				{
					csvReply.RemoveAll();								// clear previous reply & send server busy message
					CreatePausedReplyCSVLine(strEposUserID, &csvReply);		// ID=nnnn,ER=107,EM=Server busy
				}

				LockManager.LogError(nLockError);
			}
			else													// yes - add new database record
			{
				csvReply.Add(StringErrorCodeUnknownID());				// no

				CString strMsg = StringErrorTextUnknownID();
				if (strAccountID != "" && (m_bIsKioskRequest == TRUE || m_bIsVendingRequest))		// pass user id
				{
					strMsg.Format("%s ( %s )", 
						(const char*) StringErrorTextUnknownID(), 
						(const char*)strAccountID);
				}
				csvReply.Add(strMsg);
			}
		}
	}

	m_LoyReqThreadManager.AddResponseLine(csvReply.GetLine());
}

//**********************************************************************
// save database details - ID already saved as first field in csv line

void CServerWrapper::SaveRequestDetails(CSQLRowAccountFull& AccountRow, CCSV* pCsvReply, bool bOnDutyGroupShift)
{
	CReportConsolidationArray<CPurchaseControlRSPBuffer> arrayPurchaseControlStamps;

	int nUserType = 1;
	int nGroupPriceLevel = 1;
	bool bVatShift = FALSE;
	//double dRestrictedP3	= 0;								// restricted Purse 3 allowance

	int nDayNo = m_dateToday.GetDayOfWeek();				// 1=Sun, 2=Mon, 0=not set
	int nGroupNo = AccountRow.GetDailyGroupNo(nDayNo - 1);		// get alternate daily group for current day
	if (nGroupNo == 0)										// not set
	{
		nGroupNo = AccountRow.GetGroupNo();							// use default group 
	}
//
	if ((3 == nGroupNo) && (TRUE == bOnDutyGroupShift))
	{
		nGroupNo = 4;
	}

	pCsvReply->Add(StringVersionReq());							// .REQ version number

	CString strMsg = "";
	strMsg.Format("USID=%I64d", AccountRow.GetUserID());
	pCsvReply->Add(strMsg);

	strMsg.Format("MFID=%s", (const char*)AccountRow.GetExternalRef1());
	pCsvReply->Add(strMsg);

	strMsg.Format("MF2ID=%s", (const char*)AccountRow.GetExternalRef2());
	pCsvReply->Add(strMsg);

	CPurse1AvailableSpendInfo ASInfoP1;
	CPurse3AvailableSpendInfo ASInfoP3;

	CSQLRowGroupFull RowGroup;
	RowGroup.SetGroupNo(nGroupNo);

	CSQLRepositoryGroup RepoGroup;
	if ( RepoGroup.SelectRow(RowGroup,NULL).GetSQLError() == SQLCRUD_ERR_NONE)
	{
		nUserType = RowGroup.GetUserType();					// 1=pupil, 2=staff 
		nGroupPriceLevel = RowGroup.GetPriceLevel();		// price level
		bVatShift = (RowGroup.GetVatShiftFlag() != 0);		// Vat Shift

		CalculateBalanceP1(AccountRow, RowGroup, ASInfoP1);
		CalculateBalanceP3(AccountRow, RowGroup, ASInfoP3);			// get current purse3 & overdraft

		strMsg.Format("GR=%d", nGroupNo);
		pCsvReply->Add(strMsg);
	}
	else
	{
		pCsvReply->Add(StringErrorCodeUnknownGroup());

		strMsg.Format("%s ( %d )", 
			(const char*) StringErrorTextUnknownGroup(), 
			nGroupNo);

		pCsvReply->Add(strMsg);
	}

	// user names **********************************************

	pCsvReply->Add(GetGCSafeTag("N1",AccountRow.GetUsername()));

	{
		CString strName = "";
		switch (Server.GetForenameSearchFieldType())
		{
		case 2:
			strName = AccountRow.GetInfo3();
			break;

		case 3:
			strName = AccountRow.GetInfo4();
			break;

		case 0:
		case 1:
		default:
			strName = AccountRow.GetForename();
			break;
		}

		strMsg = GetGCSafeTag("N2", strName);

		if (strMsg.GetLength() > 3)
		{
			pCsvReply->Add(strMsg);
		}
	}

	strMsg = GetGCSafeTag("N3", AccountRow.GetSurname());
	if (strMsg.GetLength() > 3)
	{
		pCsvReply->Add(strMsg);
	}

	// Purse 1 ************************************************

	strMsg.Format("P1=%.0f", ((ASInfoP1.m_dBalanceP1 + ASInfoP1.m_dBalanceP2) * 100));			// Purse1 + Purse2
	pCsvReply->Add(strMsg);

	strMsg.Format("P2=%.0f", ASInfoP1.m_dBalanceP2 * 100);				// Purse2 only
	pCsvReply->Add(strMsg);

	strMsg.Format("AS=%.0f", (ASInfoP1.m_dAvailableSpendP1 + ASInfoP1.m_dBalanceP2) * 100);	// Available Spend
	pCsvReply->Add(strMsg);

	{
		CString strSL = "SL=";

		if (ASInfoP1.m_dSpendLimitP1 > 0.0)								// see if there is a max daily spend limit set
		{
			ASInfoP1.m_dSpendLimitP1 -= SpendTodayP1(AccountRow);				// deduct was has been spend so far today

			CString strLimit = "";
			strLimit.Format("%.0f", ASInfoP1.m_dSpendLimitP1 * 100);
			strSL += strLimit;
		}
		else
		{
			strSL += "0";
		}

		pCsvReply->Add(strSL);
	}

	strMsg.Format("OD1=%.0f", ASInfoP1.m_dOverdraftP1 * 100);		// Purse1 Overdraft limit
	pCsvReply->Add(strMsg);


	//Purse 3 ************************************************

	if (m_bIsEposRequest == TRUE || m_bIsKioskRequest == TRUE)
	{
		strMsg.Format("P3=%.0f", ASInfoP3.m_dBalanceP3 * 100);			// Purse3
		pCsvReply->Add(strMsg);

		strMsg.Format("AS3=%.0f", (ASInfoP3.m_dAvailableSpendP3) * 100);	// Available Spend
		pCsvReply->Add(strMsg);

		if (ASInfoP3.m_dSpendLimitP3 > 0.0)									// see if there is a max daily spend limit set
		{
			ASInfoP3.m_dSpendLimitP3 -= SpendTodayP3(AccountRow);			// deduct was has been spend so far today

			strMsg.Format("SL3=%.0f", ASInfoP3.m_dSpendLimitP3 * 100);		// Daily Spend Limit
			pCsvReply->Add(strMsg);
		}

		strMsg.Format("OD3=%.0f", ASInfoP3.m_dOverdraftP3 * 100);			// Purse3 Overdraft limit
		pCsvReply->Add(strMsg);
	}

	// Points & Last Used *********************************************

	if (m_bIsEposRequest == TRUE || m_bIsKioskRequest == TRUE)
	{
		strMsg.Format("PB=%d", AccountRow.GetPoints());
		pCsvReply->Add(strMsg);

		strMsg.Format("PD=%d", AccountRow.GetPointsToDate());				// PointsTD
		pCsvReply->Add(strMsg);

		CSSDate date;
		CSSTime time;

		if (AccountRow.GetLastUsed(date, time) == TRUE)
		{
			strMsg.Format("LUD=%s", date.GetDate());				// last used date
			pCsvReply->Add(strMsg);

			strMsg.Format("LUT=%s", time.GetTime());				// last used date
			pCsvReply->Add(strMsg);
		}
	}

	// other EPOS fields ***********************************************

	if (m_bIsEposRequest == TRUE)
	{
		strMsg.Format("TY=%d", nUserType);						// account type
		pCsvReply->Add(strMsg);

		if (nGroupPriceLevel != 0)
		{
			strMsg.Format("PL=%d", nGroupPriceLevel);						// price level
			pCsvReply->Add(strMsg);
		}

		strMsg = (bVatShift == TRUE) ? "VS=1" : "VS=0";			// VAT Shift
		pCsvReply->Add(strMsg);

		// allergies 

		int nAllergyCodes = AccountRow.GetAllergyCodes();					// Allergies
		if (nAllergyCodes != 0)
		{
			CAllergyText allergy;
			allergy.Read();
			allergy.SetAllergyCodes(nAllergyCodes);

			for (int nCode = 1; nCode <= nMAX_ALLERGIES; nCode++)
			{
				if (allergy.HaveAllergy(nCode) == TRUE)
				{
					strMsg.Format("AG=%d", nCode);
					pCsvReply->Add(strMsg);
				}
			}
		}

		CString strBuf = "";

		// dietary tet
		{
			strBuf = AccountRow.GetDietary();
			if (strBuf != "")
			{
				pCsvReply->Add(GetGCSafeTag("AL", strBuf));
			}
		}

		// Inhibited Departments *****************************

		strBuf = RowGroup.GetDeptInhibitCash();
		if (strBuf != "")
		{
			strMsg = "DIP0=" + strBuf;								// Cash inhiited departments
			pCsvReply->Add(strMsg);
		}

		strBuf = RowGroup.GetDeptInhibitPurse1();
		if (strBuf != "")
		{
			strMsg = "DIP1=" + strBuf;								// Purse1 inhiited departments
			pCsvReply->Add(strMsg);
		}

		strBuf = RowGroup.GetDeptInhibitPurse2();
		if (strBuf != "")
		{
			strMsg = "DIP2=" + strBuf;								// Purse2 inhiited departments
			pCsvReply->Add(strMsg);
		}

		strBuf = RowGroup.GetDeptInhibitPurse3();
		if (strBuf != "")
		{
			strMsg = "DIP3=" + strBuf;								// Purse3 inhiited departments
			pCsvReply->Add(strMsg);
		}

		// Restricted P3 Departments ******************************

				/*
				strBuf = m_dbGroup.GetDeptRestrictedP3List();
				if ( strBuf != "" )
				{
					strMsg = "DRP3=" + strBuf;								// Purse3 restricted departments
					pCsvReply->Add ( strMsg );

					strMsg.Format ( "DRV3=%.0f", dRestrictedP3 * 100 );		// Purse3 restricted allowance left
					pCsvReply->Add ( strMsg );
				}
				*/
	}

	// Photo image ********************************************

	if (m_bIsEposRequest == TRUE || m_bIsKioskRequest == TRUE)
	{
		if (Server.GetPhotoIDFolderType() != 0)					// photo ID
		{
			CImageHandler image(TRUE);
			CString strFilename = image.GetImageFilepath(AccountRow.GetUserIDString());
			if (strFilename != "")
			{
				strMsg = "IM=" + strFilename;
				pCsvReply->Add(strMsg);
			}
		}
	}

	// purchase control **************************

	if (TRUE == m_bIsEposRequest)
	{
		if (RowGroup.GetPurchaseControlFlag() == FALSE)
		{
			pCsvReply->Add("SO=0");
		}
		else
		{
			pCsvReply->Add("SO=1");

			CSQLRepositoryPurchaseControlStamp RepoStamp;
			CSQLRepositoryPurchaseControlRule RepoRule;

			RepoStamp.GetStampCounts(AccountRow.GetUserID(), arrayPurchaseControlStamps);
			
			for (int n = 0; n < arrayPurchaseControlStamps.GetSize(); n++)
			{
				CPurchaseControlRSPBuffer item;
				arrayPurchaseControlStamps.GetAt(n, item);

				CSQLRowPurchaseControlRule RowRule;
				RowRule.SetRuleID(item.m_nRuleID);
				if (RepoRule.SelectRow(RowRule, NULL).GetSQLError() != SQLCRUD_ERR_NONE)
				{
					arrayPurchaseControlStamps.RemoveAt(n--);
					continue;
				}

				if (RowRule.GetEnableFlag() == FALSE)
				{
					arrayPurchaseControlStamps.RemoveAt(n--);
					continue;
				}

				item.m_strRuleName = RowRule.GetDisplayDescription();
				item.m_nThreshold = RowRule.GetThreshold();
				arrayPurchaseControlStamps.SetAt(n, item);

				if (item.m_nStampCount > 0)
				{
					CString strStamps = "";
					strStamps.Format("SO%d=%d",
						item.m_nRuleID,
						item.m_nStampCount);

					pCsvReply->Add(strStamps);
				}
			}
		}
	}

	// Group Shift by Date Range **************************

	if (TRUE == m_bIsEposRequest)
	{
		if (AccountRow.GetGroupShiftStatus() == 1)
		{
			{
				CString strTag = "";
				strTag += "GSFT_SD=";
				strTag += AccountRow.GetGroupShiftStartDate();
				pCsvReply->Add(strTag);
			}

			{
				CString strTag = "";
				strTag += "GSFT_ED=";
				strTag += AccountRow.GetGroupShiftEndDate();
				pCsvReply->Add(strTag);
			}

			{
				CString strTag = "";
				strTag.Format( "GSFT_UG=%d", AccountRow.GetGroupShiftFutureGroup() );
				pCsvReply->Add(strTag);
			}

			{
				CString strName = "";

				CSQLRowGroupFull RowUsualGroup;
				RowUsualGroup.SetGroupNo(AccountRow.GetGroupShiftFutureGroup());
				
				if (RepoGroup.SelectRow(RowUsualGroup, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
				{
					strName = RowUsualGroup.GetGroupName();
				}

				pCsvReply->Add(GetGCSafeTag("GSFT_UGN", strName));	
			}

			pCsvReply->Add(GetGCSafeTag("GSFT_SGN", RowGroup.GetGroupName()));
		}
	}

	// account error \ warnning messages **************************
	bool bKioskBlock = FALSE;
	if (TRUE == m_bIsKioskRequest)
	{
		if ( RowGroup.GetAllowRevalKioskFlag() == FALSE )
		{
			pCsvReply->Add(StringErrorCodeKioskBlock());
			pCsvReply->Add(StringErrorTextKioskBlock());
			bKioskBlock = TRUE;									
		}
		else
		{
			CString strAccountID = AccountRow.GetUserIDString();

			// account is being edited
			if (HaveAccountAccess(strAccountID) == FALSE)			
			{
				pCsvReply->Add(StringErrorCodeInUse());
				pCsvReply->Add(StringErrorTextInUse2());
				bKioskBlock = TRUE;									
			}
		}
	}

	if (FALSE == bKioskBlock)
	{
		if (AccountRow.GetInactive() == TRUE)
		{
			pCsvReply->Add(StringErrorCodeInactive());
			pCsvReply->Add(StringErrorTextInactive());
		}
		else if (AccountRow.HasUserExpired() == TRUE)
		{
			pCsvReply->Add(StringErrorCodeExpired());

			strMsg.Format("%s %s",
				(const char*)StringErrorTextExpired(),
				(const char*)AccountRow.GetExpiryDate());

			pCsvReply->Add(strMsg);
		}
		else if (AccountRow.IsAlertSet() == TRUE)
		{
			pCsvReply->Add(StringErrorCodeAlert());

			CAlertText alert;
			alert.Open(Filenames.GetAlertFilename());			// readonly

			CString strText = alert.GetText(AccountRow.GetAlertCode());
			if (strText == "")
			{
				strText.Format("%d", AccountRow.GetAlertCode());
			}

			strMsg.Format("%s ( %s )",
				(const char*)StringErrorTextAlert(),
				(const char*)strText);

			pCsvReply->Add(strMsg);
		}
	}

	//BIO CONSENT
	{
		CString strBioConsent = "BC=";

		int nBioConsentTick = System.GetBioConsentTickNo();
		if ((nBioConsentTick < 1) || (nBioConsentTick > 4) || (System.GetAssumeBioConsentFlag() == TRUE))
		{
			strBioConsent += "1";
		}
		else
		{
			strBioConsent += (AccountRow.GetTick(nBioConsentTick)) ? "1" : "0";
		}

		pCsvReply->Add(strBioConsent);
	}

	//DUTY SHIFT PROMT
	if (System.GetAllowGroupShiftOnDutyFlag() == TRUE)
	{
		if (AccountRow.GetGroupNo() == 3)
		{
			if ( RowGroup.GetGroupNo() == 3 )
			{
				if (RowGroup.GetUserType() == 3)
				{
					CSQLRowGroupFull RowGroupCheck;
					RowGroupCheck.SetGroupNo(4);

					if (RepoGroup.SelectRow(RowGroupCheck,NULL).GetSQLError() == SQLCRUD_ERR_NONE)
					{
						if (RowGroupCheck.GetUserType() == 4)
						{
							pCsvReply->Add("ODP=1");
						}
					}
				}
			}
		}
	}
}

//**********************************************************************
// szCommand ' CMD=STATUS,Tno,StatusNo,CCNo
//
// Status ( 0=off,1=on,2=Refresh,3=Inservice,4=OutOfService,5=EmptyCash )
//
// reply "OK,0" / "DISABLED,1"

void CServerWrapper::ProcessCommand(CCSV* pCsv)
{
	//	CString strCommand	= pCsv->GetString(0);
	int nTerminalNo = pCsv->GetInt(1);				// kiosk number
	int nStatus = pCsv->GetInt(2);				// reqd status
	int nCCNo = pCsv->GetInt(3);				// current kiosk ccno
	double dGT = pCsv->GetDouble(4);			// current GT value

	CSQLRowEposTerminal RowEP;
	RowEP.SetTerminalNo(nTerminalNo);
	RowEP.SetTerminalType(nRECORDTYPE_KIOSK);

	CSQLRepositoryEposTerminal RepoTerm;
	RepoTerm.SelectRow(RowEP, NULL);	

	RowEP.SetLastUsedDateAndTimeToNow();

	switch (nStatus)
	{
	//kiosk terminated
	case 0:
		RowEP.SetTerminalOnFlag(FALSE);
		break;

	//kiosk started
	case 1:
		RowEP.SetTerminalOnFlag(TRUE);
		RowEP.SetUserID(0);
		RowEP.SetCardValue(0);
		break;

	case 2:
		RowEP.SetTerminalOnFlag(TRUE);
		break;

	case 3:
		RowEP.SetInServiceFlag(TRUE);
		break;

	case 4:
		RowEP.SetInServiceFlag(FALSE);
		break;

	case 5:
		RowEP.SetCashRemoved(nCCNo, dGT, Filenames.GetCashFilename(nTerminalNo));
		break;
	}

	RepoTerm.UpsertRow(RowEP, NULL);	
	
	m_LoyReqThreadManager.AddResponseLine((RowEP.GetDisableFlag() == TRUE) ? "DISABLED,1" : "OK,0");
}

//**********************************************************************
//*** get P1 account balance & spend limts & overdraft ********************
//**********************************************************************

void CServerWrapper::CalculateBalanceP1( CSQLRowAccountFull& RowAccount, CSQLRowGroupFull& RowGroup, CPurse1AvailableSpendInfo& ASInfo)
{
	ASInfo.Reset();

	{
		bool bDaily, bGroup;
		CServerSubs::GetMaxSpendFlags(RowAccount, RowGroup, bDaily, bGroup);

		if (TRUE == bDaily)
		{
			if (TRUE == bGroup)
			{
				ASInfo.m_dSpendLimitP1 = RowGroup.GetMaxSpendPurse1Period1();		// group limit
				ASInfo.m_dOverdraftP1 = RowGroup.GetMaxOverdraftPurse1Period1();
			}
			else
			{
				ASInfo.m_dSpendLimitP1 = RowAccount.GetMaxSpendPurse1Period1();
				ASInfo.m_dOverdraftP1 = RowAccount.GetMaxOverdraftPurse1Period1();

				if ((ASInfo.m_dSpendLimitP1 == 0.0) && (ASInfo.m_dOverdraftP1 == 0.0))	// neither set
				{
					if (RowGroup.GetMaxSpendType() == nGRPSPENDTYPE_DAILY_RECORD1)
					{
						ASInfo.m_dSpendLimitP1 = RowGroup.GetMaxSpendPurse1Period1();	// use group as default
						ASInfo.m_dOverdraftP1 = RowGroup.GetMaxOverdraftPurse1Period1();
					}
				}
			}
		}
		else
		{
			if (TRUE == bGroup)
			{
				switch (SQLRowSetAuditPeriod.GetPeriodIndex(m_timeToday.GetTime()))
				{
				case 0:	
					ASInfo.m_dSpendLimitP1 = RowGroup.GetMaxSpendPurse1Period1();
					ASInfo.m_dOverdraftP1 = RowGroup.GetMaxOverdraftPurse1Period1();
					break;

				case 1:	
					ASInfo.m_dSpendLimitP1 = RowGroup.GetMaxSpendPurse1Period2();
					ASInfo.m_dOverdraftP1 = RowGroup.GetMaxOverdraftPurse1Period2();
					break;

				case 2:	
					ASInfo.m_dSpendLimitP1 = RowGroup.GetMaxSpendPurse1Period3();
					ASInfo.m_dOverdraftP1 = RowGroup.GetMaxOverdraftPurse1Period3();
					break;

				case 3:	
					ASInfo.m_dSpendLimitP1 = RowGroup.GetMaxSpendPurse1Period4();
					ASInfo.m_dOverdraftP1 = RowGroup.GetMaxOverdraftPurse1Period4();
					break;

				case 4:	
					ASInfo.m_dSpendLimitP1 = RowGroup.GetMaxSpendPurse1Period5();
					ASInfo.m_dOverdraftP1 = RowGroup.GetMaxOverdraftPurse1Period5();
					break;

				case 5:	
					ASInfo.m_dSpendLimitP1 = RowGroup.GetMaxSpendPurse1Period6();
					ASInfo.m_dOverdraftP1 = RowGroup.GetMaxOverdraftPurse1Period6();
					break;
				}
			}
			else
			{
				switch (SQLRowSetAuditPeriod.GetPeriodIndex(m_timeToday.GetTime()))
				{
				case 0:	
					ASInfo.m_dSpendLimitP1 = RowAccount.GetMaxSpendPurse1Period1();
					ASInfo.m_dOverdraftP1 = RowAccount.GetMaxOverdraftPurse1Period1();
					break;

				case 1:	
					ASInfo.m_dSpendLimitP1 = RowAccount.GetMaxSpendPurse1Period2();
					ASInfo.m_dOverdraftP1 = RowAccount.GetMaxOverdraftPurse1Period2();
					break;

				case 2:	
					ASInfo.m_dSpendLimitP1 = RowAccount.GetMaxSpendPurse1Period3();
					ASInfo.m_dOverdraftP1 = RowAccount.GetMaxOverdraftPurse1Period3();
					break;

				case 3:	
					ASInfo.m_dSpendLimitP1 = RowAccount.GetMaxSpendPurse1Period4();
					ASInfo.m_dOverdraftP1 = RowAccount.GetMaxOverdraftPurse1Period4();
					break;

				case 4:	
					ASInfo.m_dSpendLimitP1 = RowAccount.GetMaxSpendPurse1Period5();
					ASInfo.m_dOverdraftP1 = RowAccount.GetMaxOverdraftPurse1Period5();
					break;

				case 5:	
					ASInfo.m_dSpendLimitP1 = RowAccount.GetMaxSpendPurse1Period6();
					ASInfo.m_dOverdraftP1 = RowAccount.GetMaxOverdraftPurse1Period6();
					break;
				}
			}
		}
	}

	if (Server.GetAllowOverdraftFlag() == FALSE)
	{
		ASInfo.m_dOverdraftP1 = 0.0;
	}

	// calculate available spend on Purse1 \ Purse2

	CSQLPurse2Handler purse2(RowAccount, RowGroup);
	ASInfo.m_dBalanceP2 = purse2.GetBalance(FALSE);										// Get purse2 no save refresh if refresh was required 

	ASInfo.m_dBalanceP1 = RowAccount.GetPurse1Liability() + RowAccount.GetPurse1Credit();
	ASInfo.m_dAvailableSpendP1 = 0.0;

	if (CServerSubs::IsPurseAllowed(1, RowAccount, RowGroup, m_dateToday, m_timeToday) == TRUE)	// see if purse1 allowed
	{
		ASInfo.m_dAvailableSpendP1 = ASInfo.m_dBalanceP1 + ASInfo.m_dOverdraftP1;
	}

	if (ASInfo.m_dAvailableSpendP1 < 0.0)
	{
		ASInfo.m_dAvailableSpendP1 = 0.0;
	}
}

//*******************************************************************
// calculate what has been spent so far today

double CServerWrapper::SpendTodayP1(CSQLRowAccountFull& RowAccount)
{
	CSQLRowGroupFull RowGroup;
	RowGroup.SetGroupNo(RowAccount.GetGroupNo());
	
	CSQLRepositoryGroup RepoGroup;
	RepoGroup.SelectRow(RowGroup,NULL);

	double dSpendTodayPurse1 = 0;
	double dSpendTodayPurse2 = 0;

	// Purse1

	CSSDate dateLastUsed(RowAccount.GetPurse1LastSpendDate());	// see if purse1 used today
	if (dateLastUsed == m_dateToday)							// has been used today
	{
		switch (RowGroup.GetMaxSpendType())
		{
		case nGRPSPENDTYPE_PERIOD_LOCKED:
		case nGRPSPENDTYPE_PERIOD_RECORD1:
		case nGRPSPENDTYPE_PERIOD_RECORD2:
			if (SQLRowSetAuditPeriod.GetPeriodIndex(RowAccount.GetPurse1LastSpendTime()) ==
				SQLRowSetAuditPeriod.GetPeriodIndex(m_timeToday.GetTime()))
			{
				dSpendTodayPurse1 = RowAccount.GetPurse1LastSpend();
			}
			break;

		default:
			dSpendTodayPurse1 = RowAccount.GetPurse1LastSpend();
			break;
		}
	}

	// purse2

	if (RowGroup.GetRefreshType() != nREFRESHTYPE_DISABLED)
	{
		CSSDate dateLastUsed(RowAccount.GetPurse2LastSpendDate());	// see if purse2 used today
		if (dateLastUsed == m_dateToday)							// has been used today
		{
			switch (RowGroup.GetMaxSpendType())
			{
			case nGRPSPENDTYPE_PERIOD_LOCKED:
			case nGRPSPENDTYPE_PERIOD_RECORD1:
			case nGRPSPENDTYPE_PERIOD_RECORD2:
				if (SQLRowSetAuditPeriod.GetPeriodIndex(RowAccount.GetPurse2LastSpendTime()) ==
					SQLRowSetAuditPeriod.GetPeriodIndex(m_timeToday.GetTime()))
				{
					dSpendTodayPurse2 = RowAccount.GetPurse2LastSpend();
				}
				break;

			default:
				dSpendTodayPurse2 = RowAccount.GetPurse2LastSpend();
				break;
			}
		}
	}

	return (dSpendTodayPurse1 + dSpendTodayPurse2);
}

//**********************************************************************
//*** get P3 account balance & spend limts & overdraft ********************
//**********************************************************************

void CServerWrapper::CalculateBalanceP3(CSQLRowAccountFull& RowAccount, CSQLRowGroupFull& RowGroup, CPurse3AvailableSpendInfo& ASInfo)
{
	ASInfo.Reset();

	{
		bool bDaily, bGroup;
		CServerSubs::GetMaxSpendFlags(RowAccount, RowGroup, bDaily, bGroup);

		if (TRUE == bDaily)
		{
			if (TRUE == bGroup)
			{
				ASInfo.m_dSpendLimitP3 = RowGroup.GetMaxSpendPurse3Period1();		// group limit
				ASInfo.m_dOverdraftP3 = RowGroup.GetMaxOverdraftPurse3Period1();
			}
			else
			{
				ASInfo.m_dSpendLimitP3 = RowAccount.GetMaxSpendPurse3Period1();
				ASInfo.m_dOverdraftP3 = RowAccount.GetMaxOverdraftPurse3Period1();;

				if ((ASInfo.m_dSpendLimitP3 == 0.0) && (ASInfo.m_dOverdraftP3 == 0.0))	// neither set
				{
					if (RowGroup.GetMaxSpendType() == nGRPSPENDTYPE_DAILY_RECORD1)
					{
						ASInfo.m_dSpendLimitP3 = RowGroup.GetMaxSpendPurse3Period1();	// use group as default
						ASInfo.m_dOverdraftP3 = RowGroup.GetMaxOverdraftPurse3Period1();
					}
				}
			}
		}
		else
		{
			if (TRUE == bGroup)
			{
				switch (SQLRowSetAuditPeriod.GetPeriodIndex(m_timeToday.GetTime()))
				{
				case 0:
					ASInfo.m_dSpendLimitP3 = RowGroup.GetMaxSpendPurse3Period1();
					ASInfo.m_dOverdraftP3 = RowGroup.GetMaxOverdraftPurse3Period1();
					break;

				case 1:
					ASInfo.m_dSpendLimitP3 = RowGroup.GetMaxSpendPurse3Period2();
					ASInfo.m_dOverdraftP3 = RowGroup.GetMaxOverdraftPurse3Period2();
					break;

				case 2:
					ASInfo.m_dSpendLimitP3 = RowGroup.GetMaxSpendPurse3Period3();
					ASInfo.m_dOverdraftP3 = RowGroup.GetMaxOverdraftPurse3Period3();
					break;

				case 3:
					ASInfo.m_dSpendLimitP3 = RowGroup.GetMaxSpendPurse3Period4();
					ASInfo.m_dOverdraftP3 = RowGroup.GetMaxOverdraftPurse3Period4();
					break;

				case 4:
					ASInfo.m_dSpendLimitP3 = RowGroup.GetMaxSpendPurse3Period5();
					ASInfo.m_dOverdraftP3 = RowGroup.GetMaxOverdraftPurse3Period5();
					break;

				case 5:
					ASInfo.m_dSpendLimitP3 = RowGroup.GetMaxSpendPurse3Period6();
					ASInfo.m_dOverdraftP3 = RowGroup.GetMaxOverdraftPurse3Period6();
					break;
				}
			}
			else
			{
				switch (SQLRowSetAuditPeriod.GetPeriodIndex(m_timeToday.GetTime()))
				{
				case 0:	
					ASInfo.m_dSpendLimitP3 = RowAccount.GetMaxSpendPurse3Period1();
					ASInfo.m_dOverdraftP3 = RowAccount.GetMaxOverdraftPurse3Period1();
					break;

				case 1:	
					ASInfo.m_dSpendLimitP3 = RowAccount.GetMaxSpendPurse3Period2();
					ASInfo.m_dOverdraftP3 = RowAccount.GetMaxOverdraftPurse3Period2();
					break;

				case 2:	
					ASInfo.m_dSpendLimitP3 = RowAccount.GetMaxSpendPurse3Period3();
					ASInfo.m_dOverdraftP3 = RowAccount.GetMaxOverdraftPurse3Period3();
					break;

				case 3:	
					ASInfo.m_dSpendLimitP3 = RowAccount.GetMaxSpendPurse3Period4();
					ASInfo.m_dOverdraftP3 = RowAccount.GetMaxOverdraftPurse3Period4();
					break;

				case 4:	
					ASInfo.m_dSpendLimitP3 = RowAccount.GetMaxSpendPurse3Period5();
					ASInfo.m_dOverdraftP3 = RowAccount.GetMaxOverdraftPurse3Period5();
					break;

				case 5:	
					ASInfo.m_dSpendLimitP3 = RowAccount.GetMaxSpendPurse3Period6();
					ASInfo.m_dOverdraftP3 = RowAccount.GetMaxOverdraftPurse3Period6();
					break;
				}
			}
		}
	}

	if (Server.GetAllowOverdraftFlag() == FALSE)
	{
		ASInfo.m_dOverdraftP3 = 0;
	}

	// calculate available spend

	ASInfo.m_dBalanceP3 = RowAccount.GetPurse3Liability() + RowAccount.GetPurse3Credit();
	ASInfo.m_dAvailableSpendP3 = 0.0;

	if (CServerSubs::IsPurseAllowed(3, RowAccount, RowGroup, m_dateToday, m_timeToday) == TRUE)	// see if purse3 allowed
	{
		ASInfo.m_dAvailableSpendP3 = ASInfo.m_dBalanceP3 + ASInfo.m_dOverdraftP3;
	}

	if (ASInfo.m_dAvailableSpendP3 < 0.0)
	{
		ASInfo.m_dAvailableSpendP3 = 0.0;
	}
}

//*******************************************************************
// calculate what has been spent so far today

double CServerWrapper::SpendTodayP3(CSQLRowAccountFull& RowAccount)
{
	CSQLRowGroupFull RowGroup;
	RowGroup.SetGroupNo(RowAccount.GetGroupNo());

	CSQLRepositoryGroup RepoGroup;
	RepoGroup.SelectRow(RowGroup,NULL);

	double dSpendToday = 0;

	// Purse1

	CSSDate dateLastUsed(RowAccount.GetPurse3LastSpendDate());	// see if purse3 used today
	if (dateLastUsed == m_dateToday)						// has been used today
	{
		switch (RowGroup.GetMaxSpendType())
		{
		case nGRPSPENDTYPE_PERIOD_LOCKED:
		case nGRPSPENDTYPE_PERIOD_RECORD1:
		case nGRPSPENDTYPE_PERIOD_RECORD2:
			if (SQLRowSetAuditPeriod.GetPeriodIndex(RowAccount.GetPurse3LastSpendTime()) ==
				SQLRowSetAuditPeriod.GetPeriodIndex(m_timeToday.GetTime()))
			{
				dSpendToday = RowAccount.GetPurse3LastSpend();
			}
			break;

		default:
			dSpendToday = RowAccount.GetPurse3LastSpend();
			break;
		}
	}

	return dSpendToday;
}

//**********************************************************************
// regiser fact that account has a biometric entry in SharpPos system

void CServerWrapper::LogBioRegister(CSQLRowAccountFull& AccountRow)
{
	CSQLRowBioRegister RowBioRegister;
	RowBioRegister.SetUserID(AccountRow.GetUserID());
	
	CSQLRepositoryBioRegister RepoBio;
	if (RepoBio.SelectRow(RowBioRegister, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
	{
		RowBioRegister.SetRegDate();
		RowBioRegister.SetRegTime();
		RowBioRegister.SetUnRegFlag(FALSE);
		RepoBio.UpdateRow(RowBioRegister, NULL);
	}
	else
	{
		RowBioRegister.SetUsername(AccountRow.GetUsername());
		RowBioRegister.SetRegDate();
		RowBioRegister.SetRegTime();
		RowBioRegister.SetUnRegFlag(FALSE);
		RepoBio.InsertRow(RowBioRegister, NULL);
	}

	CSQLAuditRecord_base AuditRecord;
	AuditRecord.Set(&AccountRow, AUDIT_SERVER);
	AuditRecord.SetApplicationNo(APPNO_BIOREG);
	AuditRecord.SaveLineToFile();
}

//**********************************************************************

void CServerWrapper::LogBioUse(CSQLRowAccountFull& AccountRow)
{
	CSQLRowBioRegister RowBioRegister;
	RowBioRegister.SetUserID(AccountRow.GetUserID());
	
	CSQLRepositoryBioRegister RepoBio;
	if (RepoBio.SelectRow(RowBioRegister,NULL).GetSQLError() == SQLCRUD_ERR_NONE)
	{
		CString strDate = RowBioRegister.GetFirstDate();
		if (strDate == "")
		{
			RowBioRegister.SetFirstDate();
			RowBioRegister.SetFirstTime();
			RowBioRegister.SetUnRegFlag(FALSE);
			RepoBio.UpdateRow(RowBioRegister, NULL);
		}
		else
		{
			RowBioRegister.SetLastDate();
			RowBioRegister.SetLastTime();
			RowBioRegister.SetUnRegFlag(FALSE);
			RepoBio.UpdateRow(RowBioRegister, NULL);
		}
	}
	else
	{
		RowBioRegister.SetUsername(AccountRow.GetUsername());
		RowBioRegister.SetFirstDate();
		RowBioRegister.SetFirstTime();
		RowBioRegister.SetUnRegFlag(FALSE);
		RepoBio.InsertRow(RowBioRegister,NULL);
	}
}

//*******************************************************************

void CServerWrapper::LogBioUnRegister(CSQLRowAccountFull& AccountRow)
{
	CSQLRowBioRegister RowBioRegister;
	RowBioRegister.SetUserID(AccountRow.GetUserID());
	
	CSQLRepositoryBioRegister RepoBio;
	if (RepoBio.SelectRow(RowBioRegister, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
	{
		RowBioRegister.SetUnRegDate();
		RowBioRegister.SetUnRegTime();
		RowBioRegister.SetUnRegFlag(TRUE);
		RepoBio.UpdateRow(RowBioRegister, NULL);
	}
	else
	{
		RowBioRegister.SetUsername(AccountRow.GetUsername());
		RowBioRegister.SetUnRegDate();
		RowBioRegister.SetUnRegTime();
		RowBioRegister.SetUnRegFlag(TRUE);
		RepoBio.InsertRow(RowBioRegister, NULL);
	}

	CSQLAuditRecord_base AuditRecord;
	AuditRecord.Set(&AccountRow, AUDIT_SERVER);
	AuditRecord.SetApplicationNo(APPNO_BIOUNREG);
	AuditRecord.SaveLineToFile();
}

//**********************************************************************