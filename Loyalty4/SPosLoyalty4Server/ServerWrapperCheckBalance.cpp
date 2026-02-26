//**********************************************************************
#include "..\SPosLoyalty4Shared\SQLTable_AuditPeriod\SQLRowSetAuditPeriod.h"
//**********************************************************************
#include "..\SPosLoyalty4Shared\AlertText_base.h"
#include "..\SPosLoyalty4Shared\ImageHandler.h"
#include "..\SPosLoyalty4Shared\LoyaltyHeaderHelpers.h"
#include "..\SPosLoyalty4Shared\LoyaltySubs.h"
#include "..\SPosLoyalty4Shared\MessageLogger.h"
#include "..\SPosLoyalty4Shared\ServerData.h"
#include "..\SPosLoyalty4Shared\SQLDefines.h"
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SPosLoyalty4Shared\SQLTable_Group\SQLRepositoryGroup.h"
#include "..\SPosLoyalty4Shared\SQLTable_OfferStamp\SQLRepositoryOfferStamp.h"
#include "..\SPosLoyalty4Shared\SQLTable_StampOfferInfo\SQLRepositoryStampOfferInfo.h"
//**********************************************************************
#include "SPosLoyalty4Server.h"
//**********************************************************************
#include "ServerWrapper.h"
//**********************************************************************

//**********************************************************************
//with RealTimePlus filename = a card number ( ie LID123456789.REQ ) 
//not the till number
//**********************************************************************

void CServerWrapper::RequestCheckBalance()
{
	CServerDataFolderSetIndex FolderSetIndex;

	int nMaxDb = min(PLU_DBNO_COUNT, System.GetMaxEcrmanDatabases());
	for (int nDbNo = 1; nDbNo <= nMaxDb; nDbNo++)
	{
		FolderSetIndex.m_nDbNo = nDbNo;

		for (int nSetNo = 1; nSetNo <= EPOSREQPATH_COUNT; nSetNo++)
		{
			FolderSetIndex.m_nSetNo = nSetNo;

			m_LoyReqThreadManager.CheckThread(FolderSetIndex);

			LONGLONG nThreadTime = m_LoyReqThreadManager.GetThreadTime();

			if (nThreadTime >= 3000)
			{
				if (Server.GetLogActionFlag() == TRUE)
				{
					CString strMsg = "";
					strMsg.Format("==> Folder Set %2.2d:%2.2d Slow REQ response (%I64d)", 
						FolderSetIndex.m_nDbNo,
						FolderSetIndex.m_nSetNo,
						nThreadTime / 1000);

					MessageLogger.LogSystemMessage(strMsg);
				}
			}

			if (m_LoyReqThreadManager.GetRequestLineCount() != 0)
			{
				Server.SetCurrentFolderSetIndex(FolderSetIndex);

				if (m_LoyReqThreadManager.IsReconRequest())
				{
					ProcessReconRequest();
				}
				else if ( m_LoyReqThreadManager.IsJSONRequest())
				{
					ProcessJSONRequest();
				}
				else if ((System.GetExternalAccountFlag() == FALSE) || (Server.GetCurrentExternalRequestPathFlag() == FALSE))
				{
					UpdatePluInfo(FolderSetIndex.m_nDbNo);
					UpdateStampOfferInfo();
					UpdateLoyaltySchemeInfo();
					RequestCheckBalanceInternal();
				}
				else
				{
					ProcessExternalCommand();
				}
			}

			m_LoyRspThreadManager.CheckThread(FolderSetIndex);
		}
	}

	for (int nDbNo = 1; nDbNo <= nMaxDb; nDbNo++)
	{
		FolderSetIndex.m_nDbNo = nDbNo;

		for (int nSetNo = 1; nSetNo <= EPOSREQPATH_COUNT; nSetNo++)
		{
			FolderSetIndex.m_nSetNo = nSetNo;

			m_LoyReqTidyUpThreadManager.CheckThread(FolderSetIndex);
		}
	}
}

//**********************************************************************

void CServerWrapper::RequestCheckBalanceInternal()
{
	if (Server.GetLogActionFlag() == TRUE)
	{
		CString strMsg;
		strMsg.Format("==> REQ: %s", 
			(const char*) m_LoyReqThreadManager.GetREQFilepath());

		MessageLogger.LogSystemMessage(strMsg);
	}

	CString strLine = m_LoyReqThreadManager.GetRequestLine(0);

	//HANDLE EMBEDDED QR CODE REQUEST
	bool bQRCodeEmbedError = FALSE;
	if (strLine.Left(3) == "QR=")
	{
		if (System.GetQRCodeEmbedFlag() == TRUE)
		{
			CString strID = strLine.Mid(3);
			CString strPrefix = System.GetQRCodeEmbedPrefix();

			if (strPrefix != "")
			{
				if ((strID.Left(strPrefix.GetLength()) != strPrefix) || (strPrefix.GetLength() > strID.GetLength()))
				{
					CCSV csvFirstLine;												// first line of response file
					csvFirstLine.Add(strID);										// send back cardno as received from EPOS
					csvFirstLine.Add(RSPFILE_ERROR_BAD_QRPREFIX);					// set valid flag to fail
					m_LoyReqThreadManager.AddResponseLine(csvFirstLine.GetLine());
					m_LoyReqThreadManager.AddResponseLine("ERROR - INCORRECT QR PREFIX");
					bQRCodeEmbedError = TRUE;
				}
			}

			if (FALSE == bQRCodeEmbedError)
			{
				strLine = strID.Mid(strPrefix.GetLength());

				if (IsStringNumeric(strLine) == FALSE)
				{
					CCSV csvFirstLine;												// first line of response file
					csvFirstLine.Add(strID);										// send back cardno as received from EPOS
					csvFirstLine.Add(RSPFILE_ERROR_BAD_QRFORMAT);					// set valid flag to fail
					m_LoyReqThreadManager.AddResponseLine(csvFirstLine.GetLine());
					m_LoyReqThreadManager.AddResponseLine("ERROR - BAD QR REQUEST");
					bQRCodeEmbedError = TRUE;
				}
			}
		}
	}

	m_dateToday.SetCurrentDate();							// set date & time
	m_timeToday.SetCurrentTime();

	if (FALSE == bQRCodeEmbedError)
	{
		if (::IsStringNumeric(strLine) == TRUE)
		{
			switch (System.GetInterfaceType())
			{
				//case nINTERFACE_MIFAREv3:
				//	nInterfaceType = nINTERFACE_SWIPEv1;
				//	break;

			case nINTERFACE_MIFAREv1:
			case nINTERFACE_MIFAREv2:
				if (System.GetPaxtonModeFlag() == TRUE)
				{
					if (strLine.GetLength() >= 9)
					{
						strLine = strLine.Right(8);
						strLine.TrimLeft('0');
					}
				}
				break;
			}

			int nInterfaceType = System.GetInterfaceType();
			if ((strLine.Left(4) == "1033") && (Server.GetSimpsInnsLookupFlag() == TRUE))
			{
				nInterfaceType = nINTERFACE_SIMPSINNS;
			}

			CAccountIdentifier AccountIdentifier(strLine, nInterfaceType);
			CreateBalanceReply1(AccountIdentifier, FALSE, TRUE);
		}
		else if (strLine.Left(3) == "QR=")
		{
			CCSV csvLine(strLine);

			if (csvLine.GetSize() == 1)
			{
				CString strBuf = csvLine.GetString(0);
				CAccountIdentifier AccountIdentifier(strBuf.Mid(3), nINTERFACE_QRINFO);
				CreateBalanceReply1(AccountIdentifier, FALSE, TRUE);
			}
			else
			{
				RegistrationError("", StringErrorCodeUnsupported(), StringErrorTextUnsupported());
			}
		}
		else if (System.GetInterfaceType() != nINTERFACE_MIFAREv3)
		{
			if (Server.GetSearchSnakesAndLaddersFlag() == TRUE)
			{
				CreateSearchReplySnakesAndLadders(strLine);
			}
			else
			{
				CreateSearchReplyAccountLookup(strLine);
			}
		}
		else
		{
			if (strLine.Left(3) == "MF=")
			{
				CCSV csvLine(strLine);

				if (System.GetPaxtonModeFlag() == TRUE)
				{
					CString strField = csvLine.GetString(0);

					if (strField.GetLength() >= 12)
					{
						CString strNum = strField.Right(8);
						strNum.TrimLeft('0');

						CString strPaxton = "MF=";
						strPaxton += strNum;;
						csvLine.SetAt(0, strPaxton);
						strLine = csvLine.GetLine();
					}
				}

				CString strBuf = csvLine.GetString(0);

				if (csvLine.GetSize() == 1) // see if MiFare number used instead of accountID
				{
					CAccountIdentifier AccountIdentifier(strBuf.Mid(3), nINTERFACE_MIFAREv2);
					CreateBalanceReply1(AccountIdentifier, FALSE);
				}
				else
				{
					CLoyaltyLockRequest LockRequest;
					LockRequest.m_bWantLockServerInner = TRUE;		
					
					CLoyaltyLockManager LockManager;
					int nLockResult = LockManager.AcquireLocks(LockRequest);		
					
					if (LOCKMANAGER_OK == nLockResult)
					{
						ProcessRegistration(strBuf.Mid(3), &csvLine);
					}
					else
					{
						CreatePausedReply(strBuf.Mid(3));
					}

					LockManager.LogError(nLockResult);
				}
			}
			else
			{
				CreateSearchReplyAccountLookup(strLine);
			}
		}
	}

	if (m_LoyReqThreadManager.GetResponseLineCount() > 2)
	{
		CServerDataFolderSetIndex FolderSetIndex = m_LoyReqThreadManager.GetThreadIndex();

		m_LoyRspThreadManager.QueueResponseHeader(FolderSetIndex);
		for (int n = 0; n < m_LoyReqThreadManager.GetResponseLineCount(); n++)
		{
			m_LoyRspThreadManager.QueueResponseLine(FolderSetIndex, m_LoyReqThreadManager.GetResponseLine(n));
		}
		m_LoyRspThreadManager.CheckThread(FolderSetIndex);

		if (Server.GetLogActionFlag() == TRUE)
		{
			CString strMsg = "";
			strMsg += "<== RSP: ";
			strMsg += m_LoyReqThreadManager.GetResponseLine(1);
			MessageLogger.LogSystemMessage(strMsg);
		}
	}
}

//**********************************************************************

void CServerWrapper::CreateBalanceReply1(CAccountIdentifier& AccountIdentifier, bool bPrecheckedCardNo, bool bFromCheckBalance)
{
	CString strDatabaseCardNo = "";
	CString strErrorMsg = "";

	if (FALSE == bPrecheckedCardNo)
	{
		strErrorMsg = System.TranslateCardNo(AccountIdentifier, strDatabaseCardNo, bFromCheckBalance);
	}
	else
	{
		strDatabaseCardNo = AccountIdentifier.GetAccountID();
		strErrorMsg = "";
	}

	if (strErrorMsg != "")
	{
		CCSV csvFirstLine;													// first line of response file
		csvFirstLine.Add(AccountIdentifier.GetAccountID());				// send back cardno as received from EPOS
		csvFirstLine.Add(RSPFILE_ERROR_ACCOUNT_NUMBER);					// set valid flag to fail
		m_LoyReqThreadManager.AddResponseLine(csvFirstLine.GetLine());

		m_LoyReqThreadManager.AddResponseLine("ERROR - ACCOUNT NUMBER ERROR");
		m_LoyReqThreadManager.AddResponseLine(StringSeparatorLine());
		m_LoyReqThreadManager.AddResponseLine(strErrorMsg);
		m_LoyReqThreadManager.AddResponseLine(StringSeparatorLine());
	}
	else
	{
		CSQLRowAccountFull RowAccount;
		RowAccount.SetUserID(_atoi64(strDatabaseCardNo));
		CSQLRepositoryAccount repoAccount;
		__int64 nLoadAccountResult = repoAccount.SelectRow(RowAccount, NULL).GetSQLError();

		switch (nLoadAccountResult)
		{
		case SQLCRUD_ERR_NONE:
			CreateBalanceReplyInternal(1, RowAccount, bFromCheckBalance );
			break;

		case SQLCRUD_ERR_NOMATCH:
		{
			bool bAddUnknowns = TRUE;
			bAddUnknowns &= Server.CanAddUnknownCard(strDatabaseCardNo);
			bAddUnknowns &= (AccountIdentifier.GetAccountIDType() != nINTERFACE_SIMPSINNS);

			if (TRUE == bAddUnknowns)
			{
				CString strTest = strDatabaseCardNo;
				if (::TestNumeric(strTest) == FALSE)
				{
					bAddUnknowns = FALSE;
				}
				else
				{
					::TrimSpacesFromString(strTest, TRUE);
					if (strTest.GetLength() < Server.GetUnknownCardMinLength())
					{
						bAddUnknowns = FALSE;
					}
					else
					{
						__int64 nCardNo = _atoi64(strTest);
						if ((nCardNo < Server.GetUnknownCardMinID()) || (nCardNo > Server.GetUnknownCardMaxID()))
						{
							bAddUnknowns = FALSE;
						}
					}
				}
			}

			CCSV csvFirstLine;									// first line of response file
			if ( TRUE == bAddUnknowns )
			{
				csvFirstLine.Add(strDatabaseCardNo);			// send back cardno after translation
				csvFirstLine.Add("0");							// set valid flag to ok	
				csvFirstLine.Add("0");							// no points spent
				csvFirstLine.Add("0");							// no awarded
				
				CString strPendingName = Server.GetUnknownAccountPendingName();
				::TrimSpacesFromString(strPendingName, TRUE);
				
				if (strPendingName == "")
				{
					strPendingName = "New Loyalty Account";
				}
				
				csvFirstLine.Add(GetGCSafeTag("cardname", strPendingName));

				if (Server.GetUnknownCardGroupRSPFlag() == TRUE)
				{
					int nGroupNo = Server.GetUnknownCardGroupNo();
					int nSchemeNo = 0;

					CSQLRowGroup RowGroup;
					RowGroup.SetGroupNo(nGroupNo);

					CSQLRepositoryGroup repoGroup;
					if (repoGroup.SelectRow(RowGroup, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
					{
						nSchemeNo = RowGroup.GetSchemeNo();
					}

					if (nSchemeNo <= 0)
					{
						nSchemeNo = nGroupNo;
					}

					CString strText = "";
					strText.Format("group=%d", nSchemeNo);
					csvFirstLine.Add(strText);
				}

				CString strLine1 = Server.GetUnknownAccountLine1();
				CString strLine2 = Server.GetUnknownAccountLine2();
				CString strLine3 = Server.GetUnknownAccountLine3();
				::TrimSpacesFromString(strLine1, FALSE);
				::TrimSpacesFromString(strLine2, FALSE);
				::TrimSpacesFromString(strLine3, FALSE);

				if (strLine1 + strLine2 + strLine3 == "")
				{
					strLine1 = "UNKNOWN CARD NUMBER : Do you wish to create a new account ?";
				}

				CString strSpaces = "     ";
				m_LoyReqThreadManager.AddResponseLine(csvFirstLine.GetLine());
				m_LoyReqThreadManager.AddResponseLine("");
				m_LoyReqThreadManager.AddResponseLine(strSpaces + strLine1);
				m_LoyReqThreadManager.AddResponseLine("");
				m_LoyReqThreadManager.AddResponseLine(strSpaces + strLine2);
				m_LoyReqThreadManager.AddResponseLine("");
				m_LoyReqThreadManager.AddResponseLine(strSpaces + strLine3);
			}
			else
			{
				csvFirstLine.Add(strDatabaseCardNo);				// send back cardno after translation
				csvFirstLine.Add(RSPFILE_ERROR_ACCOUNT_UNKNOWN);	// set valid flag to fail
				m_LoyReqThreadManager.AddResponseLine(csvFirstLine.GetLine());
				m_LoyReqThreadManager.AddResponseLine("ERROR - UNKNOWN ACCOUNT");
				m_LoyReqThreadManager.AddResponseLine(StringSeparatorLine());
				m_LoyReqThreadManager.AddResponseLine("*** WARNING - ACCOUNT NOT IN DATABASE ****");
				m_LoyReqThreadManager.AddResponseLine(StringSeparatorLine());
			}

		}
		break;

		//ANY ERROR OTHER THAN NO MATCH
		default:
		{
			CCSV csvFirstLine;												// first line of response file
			csvFirstLine.Add(AccountIdentifier.GetAccountID());				// send back cardno as received from EPOS
			csvFirstLine.Add(RSPFILE_ERROR_OPEN_DATABASE);					// set valid flag to fail
			m_LoyReqThreadManager.AddResponseLine(csvFirstLine.GetLine());

			m_LoyReqThreadManager.AddResponseLine(StringSeparatorLine());

			CString strMsg = "";
			strMsg.Format("SQL Database Access Error (%I64d)", nLoadAccountResult);
			m_LoyReqThreadManager.AddResponseLine(strMsg);

			m_LoyReqThreadManager.AddResponseLine(StringSeparatorLine());
		}
		break;
		}
	}
}

//**********************************************************************

void CServerWrapper::CreateBalanceReply2(CSQLRowAccountFull& RowAccount, int nPointsAward, double dBonusCash)
{
	return CreateBalanceReplyInternal(2, RowAccount, FALSE, nPointsAward, dBonusCash);
}

//**********************************************************************

void CServerWrapper::CreateBalanceReplyInternal(int nType, CSQLRowAccountFull& RowAccount, bool bFromCheckBalance, int nPointsAward, double dBonusCash)
{
	CSQLRowGroup RowGroup;
	CReportConsolidationArray<CStampOfferRSPBuffer> arrayOfferStamps;

	CString strErrorMsg = "";
	int nRSPFileError = RSPFILE_ERROR_NONE;

	if (RowAccount.IsCardHotlisted() == TRUE)
	{
		strErrorMsg = "** WARNING - ACCOUNT HAS BEEN HOTLISTED **";
		nRSPFileError = RSPFILE_ERROR_ACCOUNT_HOTLISTED;
	}
	else if (RowAccount.GetInactive() == TRUE)
	{
		strErrorMsg = "** WARNING - ACCOUNT IS SET TO INACTIVE **";
		nRSPFileError = RSPFILE_ERROR_ACCOUNT_INACTIVE;
	}
	else
	{
		int nGroupNo = RowAccount.GetGroupNo();
		RowGroup.SetGroupNo(nGroupNo);

		CSQLRepositoryGroup repoGroup;
		m_bGroupExists = (repoGroup.SelectRow(RowGroup, NULL).GetSQLError() == SQLCRUD_ERR_NONE);

		if (FALSE == m_bGroupExists)
		{
			strErrorMsg.Format("** WARNING - UNKNOWN GROUP %d **", nGroupNo);
			nRSPFileError = RSPFILE_ERROR_ACCOUNT_GROUP;
		}
		else if (RowAccount.HasCardExpired() == TRUE)
		{
			strErrorMsg.Format("** WARNING - ACCOUNT EXPIRED %s **", (const char*)RowAccount.GetExpiryDate()); // "** WARNING - ACCOUNT EXPIRED 12/12/2007 **"
			nRSPFileError = RSPFILE_ERROR_ACCOUNT_EXPIRED;
		}
	}

	if (strErrorMsg != "")										// check for any errors
	{
		CCSV csvFirstLine;
		csvFirstLine.Add(RowAccount.GetUserIDString());						// send back cardno after translation
		csvFirstLine.Add(nRSPFileError);							// set valid flag to fail
		m_LoyReqThreadManager.AddResponseLine(csvFirstLine.GetLine()); // write first reply line

		m_LoyReqThreadManager.AddResponseLine(RowAccount.GetFullname());
		m_LoyReqThreadManager.AddResponseLine(StringSeparatorLine());
		m_LoyReqThreadManager.AddResponseLine(strErrorMsg);					// error message
		m_LoyReqThreadManager.AddResponseLine(StringSeparatorLine());
	}
	else
	{
		int nRedeemPointsType = Server.GetRedeemPointsType();
		int nRedeemPointsTrip = Server.GetRedeemPointsTrip();

		if (RowGroup.GetRedeemFlag() == TRUE)			// see if group specifc redeem reqd
		{
			if (RowGroup.GetRedeemDisable() == TRUE)		// yes - could be disabled
			{
				nRedeemPointsType = nREDEEM_STANDARD;			// set standard
				nRedeemPointsTrip = 0;							// set no trip
			}
			else
			{
				nRedeemPointsType = RowGroup.GetRedeemType();	// group specific redeem
				nRedeemPointsTrip = RowGroup.GetRedeemTrip();
			}
		}

		double dP1Balance = 0.0;
		double dP2Balance = 0.0;
		double dAvailableSpend = 0.0;
		CalculateAvailableSpend(RowAccount, RowGroup, dP1Balance, dP2Balance, dAvailableSpend);
		
		CCSV csvFirstLine(',', '\"', FALSE, FALSE, TRUE);

		csvFirstLine.Add(RowAccount.GetUserIDString());					// send back cardno after translation
		csvFirstLine.Add(RSPFILE_ERROR_NONE);							// set valid flag to ok
		csvFirstLine.Add(RowAccount.GetPoints());						// current points balance
		csvFirstLine.Add(dAvailableSpend * 100, 0);						// current purse & overdraft

		if (nType == 1)									// type = balance enquiry
		{
			CString strText;

			if (Server.GetPhotoIDFolderType() != 0)
			{
				CImageHandler image(Server.GetLoyaltyUNCSystemPath());
				CString strFilename = image.GetFirst(RowAccount.GetUserIDString());

				if (strFilename == "")
				{
					CImageHandler image(Server.GetLoyaltyUNCSystemPath());	// see if have default image
					strFilename = image.GetFirst("0");
				}

				if (strFilename != "")
				{
					strText = "image=" + strFilename;
					csvFirstLine.Add(strText);
				}
			}

			csvFirstLine.Add("0");							// no points spent
			csvFirstLine.Add("0");							// no awarded
			
			CString strCardName = RowAccount.GetFullname();
			::TrimSpacesFromString(strCardName, TRUE);
			
			if (strCardName == "")
			{
				strCardName = "Account ";
				strCardName += RowAccount.GetUserIDString();
			}
			
			csvFirstLine.Add(GetGCSafeTag("cardname", strCardName));

			{
				int nSchemeNo = RowAccount.GetSchemeNo();

				if (nSchemeNo <= 0)			
				{
					nSchemeNo = RowGroup.GetSchemeNo();	
				}

				if (nSchemeNo <= 0)
				{
					nSchemeNo = RowAccount.GetGroupNo();
				}

				strText.Format("group=%d", nSchemeNo);
				csvFirstLine.Add(strText);
			}

			{
				CString strText = "EXP=";

				CString strExpiry = RowAccount.GetExpiryDate();
				if (strExpiry == "")
				{
					strExpiry = "none";
				}

				strText += strExpiry;
				csvFirstLine.Add(strText);
			}

			if (RowGroup.GetStampOfferFlag() == FALSE)
			{
				csvFirstLine.Add("SO=0");
			}
			else
			{
				csvFirstLine.Add("SO=1");

				CSQLRepositoryOfferStamp repoStamp;
				repoStamp.GetStampCounts(RowAccount.GetUserID(), arrayOfferStamps);
				
				for (int n = 0; n < arrayOfferStamps.GetSize(); n++)
				{
					CStampOfferRSPBuffer item;
					arrayOfferStamps.GetAt(n, item);

					CSQLRowStampOfferInfo RowOffer;
					RowOffer.SetStampOfferID(item.m_nOfferID);

					CSQLRepositoryStampOfferInfo repoInfo;
					if (repoInfo.SelectRow(RowOffer, NULL).GetSQLError() != SQLCRUD_ERR_NONE)
					{
						arrayOfferStamps.RemoveAt(n--);
						continue;
					}

					if (RowOffer.GetEnableFlag() == FALSE)
					{
						arrayOfferStamps.RemoveAt(n--);
						continue;
					}

					item.m_strOfferName = RowOffer.GetDisplayDescription();
					item.m_nThreshold = RowOffer.GetThreshold();
					arrayOfferStamps.SetAt(n, item);

					if (item.m_nStampCount > 0)
					{
						CString strStamps = "";
						strStamps.Format("SO%d=%d",
							item.m_nOfferID,
							item.m_nStampCount);

						csvFirstLine.Add(strStamps);
					}
				}
			}
		}
		else if (nType == 2)								// type = balance on receipt reply
		{
			csvFirstLine.Add("0");							// no points spent
			csvFirstLine.Add(nPointsAward);					// points awarded
		}

		m_LoyReqThreadManager.AddResponseLine(csvFirstLine.GetLine());	// write first reply line

		m_dBonusCash = dBonusCash;								// used if <Bonus> field required on Send new balance on reply

		{
			int nInfoLines = 0;
			for (int n = 1; (n <= 8) && (nInfoLines < 8); n++)
			{
				CString strFieldType = Server.GetPosDisplayField(n);
				if (LoyaltyHeaderHelpers.GetNumericField(strFieldType) == LOYALTYFIELD_STAMPOFFER)
				{
					if (RowGroup.GetStampOfferFlag() == TRUE)
					{
						for (int x = 0; (x < arrayOfferStamps.GetSize()) && (nInfoLines < 8); x++)
						{
							CStampOfferRSPBuffer item;
							arrayOfferStamps.GetAt(x, item);

							CString strOffer = "";
							strOffer.Format("Offer: %s, %d/%d",
								(const char*)item.m_strOfferName,
								item.m_nStampCount,
								item.m_nThreshold);

							SaveLine(strOffer);
							nInfoLines++;
						}
					}
				}
				else
				{
					SaveLine(GetRecordData(RowAccount, Server.GetPosDisplayField(n), Server.GetPosDisplayText(n), dP1Balance, dP2Balance, dAvailableSpend));
					nInfoLines++;
				}
			}
		}
	}
}

//**********************************************************************

void CServerWrapper::CalculateAvailableSpend(CSQLRowAccountFull& RowAccount, CSQLRowGroup& RowGroup, double& dP1Balance, double& dP2Balance, double& dAvailableSpend)
{
	// what is available to spend
	double dAvailableSpendPurse1 = 0.0;
	double dAvailableSpendPurse2 = 0.0;

	dP1Balance = RowAccount.GetPurse1Balance();
	dP2Balance = RowAccount.GetPurse2Balance();

	// assume record values as default
	double dSpendLimit = RowAccount.GetMaxSpend();
	double dOverSpend = RowAccount.GetMaxOverdraft();

	if (m_bGroupExists == FALSE)
	{
		dAvailableSpendPurse1 = dP1Balance;
		dAvailableSpendPurse2 = dP2Balance;
	}
	else
	{
		switch (RowGroup.GetMaxSpendType())
		{
		case nSPENDTYPE_DAILY:
			// group limit
			dSpendLimit = RowGroup.GetMaxSpendPeriod1();
			dOverSpend = RowGroup.GetMaxOverdraftPeriod1();
			break;

		case nSPENDTYPE_RECORD:
			// see if record value set
			// no - use group as default
			if (dSpendLimit == 0.0)
			{
				dSpendLimit = RowGroup.GetMaxSpendPeriod1();
			}

			// see if record value set
			// no - use group as default
			if (dOverSpend == 0.0)
			{
				dOverSpend = RowGroup.GetMaxOverdraftPeriod1();
			}
			break;

		case nSPENDTYPE_PERIOD:
			switch (SQLRowSetAuditPeriod.GetPeriodIndex(m_timeToday.GetTime()))
			{
			case 0:
				dSpendLimit = RowGroup.GetMaxSpendPeriod1();
				dOverSpend = RowGroup.GetMaxOverdraftPeriod1();
				break;

			case 1:
				dSpendLimit = RowGroup.GetMaxSpendPeriod2();
				dOverSpend = RowGroup.GetMaxOverdraftPeriod2();
				break;

			case 2:
				dSpendLimit = RowGroup.GetMaxSpendPeriod3();
				dOverSpend = RowGroup.GetMaxOverdraftPeriod3();
				break;

			case 3:
				dSpendLimit = RowGroup.GetMaxSpendPeriod4();
				dOverSpend = RowGroup.GetMaxOverdraftPeriod4();
				break;

			case 4:
				dSpendLimit = RowGroup.GetMaxSpendPeriod5();
				dOverSpend = RowGroup.GetMaxOverdraftPeriod5();
				break;

			case 5:
				dSpendLimit = RowGroup.GetMaxSpendPeriod6();
				dOverSpend = RowGroup.GetMaxOverdraftPeriod6();
				break;
			}
			break;
		}

		dAvailableSpendPurse1 = 0.0;
		dAvailableSpendPurse2 = 0.0;
		
		if (RowGroup.IsPurseAllowedAtTime(2, m_timeToday) == TRUE)	// see if Purse 2 allowed
		{
			dP2Balance = LoyaltySubs.GetPurse2NextSpendBalance(RowAccount, RowGroup);
			dAvailableSpendPurse2 = dP2Balance;
		}
		else
		{
			dP2Balance = 0.0;
		}

		if ( RowGroup.IsPurseAllowedAtTime(1, m_timeToday) == TRUE)	// see if Purse1 allowed
		{
			dP1Balance = RowAccount.GetPurse1Balance();
			dAvailableSpendPurse1 = dP1Balance;
			
			if (Server.GetAllowOverSpendFlag() == TRUE)
			{
				dAvailableSpendPurse1 += dOverSpend;
			}
		}
		else
		{
			dP1Balance = 0.0;
		}
	}

	if (dAvailableSpendPurse1 < 0.0)
	{
		dAvailableSpendPurse1 = 0.0;
	}

	if (dAvailableSpendPurse2 < 0.0)
	{
		dAvailableSpendPurse2 = 0.0;
	}
	
	dAvailableSpend = dAvailableSpendPurse1 + dAvailableSpendPurse2;

	if (dSpendLimit > 0.0)						
	{
		double dDailyLimit = dSpendLimit;								// accounts daily limit
		dDailyLimit -= GetSpendToday(RowAccount, RowGroup);				// deduct what has been spend so far today
		if (dAvailableSpend > dDailyLimit)
		{
			dAvailableSpend = dDailyLimit;
		}
	}

	if (dAvailableSpend < 0.0)
	{
		dAvailableSpend = 0.0;
	}
}

//*******************************************************************
//*** Get current spend today / spend this period *******************
//*******************************************************************

double CServerWrapper::GetSpendToday( CSQLRowAccountFull& RowAccount, CSQLRowGroup& RowGroup )
{
	double dSpendTodayPurse1 = 0.0;
	double dSpendTodayPurse2 = 0.0;

	// Purse1
	{
		CSSDate dateLastUsed(RowAccount.GetPurse1LastSpendDate());	// see if purse1 used today
		if (dateLastUsed == m_dateToday)						// yes - has been used
		{
			if (RowGroup.GetMaxSpendType() == nSPENDTYPE_PERIOD)
			{
				if (SQLRowSetAuditPeriod.GetPeriodIndex(RowAccount.GetPurse1LastSpendTime()) == SQLRowSetAuditPeriod.GetPeriodIndex(m_timeToday.GetTime()))
				{
					dSpendTodayPurse1 = RowAccount.GetPurse1LastSpend();
				}
			}
			else
			{
				dSpendTodayPurse1 = RowAccount.GetPurse1LastSpend();
			}
		}
	}

	// purse2
	if (RowGroup.GetRefreshType() != nREFRESHTYPE_NORMAL)
	{
		CSSDate dateLastUsed(RowAccount.GetPurse2LastSpendDate());	// see if purse2 used today
		if (dateLastUsed == m_dateToday)						// has been used today
		{
			if (RowGroup.GetMaxSpendType() == nSPENDTYPE_PERIOD)
			{
				if (SQLRowSetAuditPeriod.GetPeriodIndex(RowAccount.GetPurse2LastSpendTime()) == SQLRowSetAuditPeriod.GetPeriodIndex(m_timeToday.GetTime()))
				{
					dSpendTodayPurse2 = RowAccount.GetPurse2LastSpend();
				}
			}
			else
			{
				dSpendTodayPurse2 = RowAccount.GetPurse2LastSpend();
			}
		}
	}

	return (dSpendTodayPurse1 + dSpendTodayPurse2);
}

//**********************************************************************

CString CServerWrapper::GetRecordData(CSQLRowAccountFull& RowAccount, const char* szField, const char* szText, double dBalanceP1, double dBalanceP2, double dAvailableSpend)
{
	CString strReply = "";

	switch (LoyaltyHeaderHelpers.GetNumericField(szField))
	{
	case LOYALTYFIELD_BLANK:
		strReply = "";
		break;

	case LOYALTYFIELD_SKIP_CHECK:
		strReply = " ";
		break;

	case LOYALTYFIELD_TEXT:
		strReply = szText;
		break;

	case LOYALTYFIELD_USERID:
		strReply = SetLine(RowAccount.GetUserIDString(), szText);
		break;

	case LOYALTYFIELD_FULLNAME:
		strReply = SetLine(RowAccount.GetFullname(), szText);
		break;

	case LOYALTYFIELD_SURNAME:
		strReply = SetLine(RowAccount.GetSurname(), szText);
		break;

	case LOYALTYFIELD_ADDRESS1:
		strReply = SetLine(RowAccount.GetAddress1(), szText);
		break;

	case LOYALTYFIELD_ADDRESS2:
		strReply = SetLine(RowAccount.GetAddress2(), szText);
		break;

	case LOYALTYFIELD_ADDRESS3:
		strReply = SetLine(RowAccount.GetAddress3(), szText);
		break;

	case LOYALTYFIELD_ADDRESS4:
		strReply = SetLine(RowAccount.GetAddress4(), szText);
		break;

	case LOYALTYFIELD_ADDRESS5:
		strReply = SetLine(RowAccount.GetAddress5(), szText);
		break;

	case LOYALTYFIELD_EXPIRYDATE:
		strReply = SetLine(RowAccount.GetExpiryDate(), szText);
		break;

	case LOYALTYFIELD_PHONE1:
		strReply = SetLine(RowAccount.GetPhone1(), szText);
		break;

	case LOYALTYFIELD_PHONE2:
		strReply = SetLine(RowAccount.GetPhone2(), szText);
		break;

	case LOYALTYFIELD_INFO1:
		strReply = SetLine(RowAccount.GetInfo1(), szText);
		break;

	case LOYALTYFIELD_INFO2:
		strReply = SetLine(RowAccount.GetInfo2(), szText);
		break;

	case LOYALTYFIELD_INFO3:
		strReply = SetLine(RowAccount.GetInfo3(), szText);
		break;

	case LOYALTYFIELD_INFO4:
		strReply = SetLine(RowAccount.GetInfo4(), szText);
		break;

	case LOYALTYFIELD_DOB:
		strReply = SetLine(RowAccount.GetDOB(), szText);
		break;

	case LOYALTYFIELD_POINTS:
		{
			CString strTmp = "";
			strTmp.Format("%d", RowAccount.GetPoints());
			strReply = SetLine(strTmp, szText);
		}
		break;

	case LOYALTYFIELD_POINTSTODATE:
		{
			CString strTmp = "";
			strTmp.Format("%d", RowAccount.GetPointsToDate());
			strReply = SetLine(strTmp, szText);
		}
		break;

	case LOYALTYFIELD_PURSE1_SPENDTODATE:
		{
			CString strTmp = "";
			strTmp.Format("%.2lf", RowAccount.GetPurse1SpendToDate());
			strReply = SetLine(strTmp, szText);
		}
		break;

	case LOYALTYFIELD_TOTAL_SPENDTODATE:
		{
			CString strTmp = "";
			strTmp.Format("%.2lf", RowAccount.GetTotalSpendToDate());
			strReply = SetLine(strTmp, szText);
		}
		break;

	case LOYALTYFIELD_NAME_BALANCE:
		{
			CString strData = "";
			strData.Format("%s (£%.2f)",
				(const char*)RowAccount.GetFullname(),
				dBalanceP1 + dBalanceP2);

			strReply = SetLine(strData, szText);
		}
		break;

	case LOYALTYFIELD_BALANCE:
		{
			CString strData = "";
			strData.Format("£%.2f",
				dBalanceP1 + dBalanceP2);

			strReply = SetLine(strData, szText);
		}
		break;

	case LOYALTYFIELD_PURSE1_BALANCE:
		{
			CString strData = "";
			strData.Format("£%.2f",
				dBalanceP1);

			strReply = SetLine(strData, szText);
		}
		break;

	case LOYALTYFIELD_PURSE2_BALANCE:
		{
			CString strData = "";
			strData.Format("£%.2f",
				dBalanceP2);

			strReply = SetLine(strData, szText);
		}
		break;

	case LOYALTYFIELD_AVAILABLESPEND:
	{
		CString strData = "";
		strData.Format("£%.2f",
			dAvailableSpend);

		strReply = SetLine(strData, szText);
	}
	break;

	case LOYALTYFIELD_BONUS_CHECK:
		if (m_dBonusCash != 0)
		{
			CString strText = szText;
			if (strText == "")
			{
				strText = "Bonus cash awarded ";
			}

			CString strData = "";
			strData.Format("%.2lf", m_dBonusCash);
			strReply = SetLine(strData, strText);
		}
		break;

	case LOYALTYFIELD_DOB_CHECK:
		if (RowAccount.IsBirthdayToday() == TRUE)
		{
			strReply = szText;
		}
		break;

	case LOYALTYFIELD_ALERT_CHECK:
		{
			CAlertText_base alertText;
			alertText.Open(Filenames.GetAlertTextFilename());

			CCSV csvIn(RowAccount.GetAlertCodes(), ':');

			CCSV csvOut;
			for (int n = 0; (n < csvIn.GetSize()) && (n < 9); n++)
			{
				int a = csvIn.GetInt(n);
				if ((a >= 1) && (a <= 9))
				{
					CAlertTextInfo info;
					alertText.GetInfo(a, info);

					if ((TRUE == info.m_bEPOS) && (info.m_strText != ""))
					{
						csvOut.Add(info.m_strText);
					}
				}
			}

			strReply = SetLine(csvOut.GetLine(), szText);
		}
		break;
	}
		
	return strReply;
}

//**********************************************************************

CString CServerWrapper::SetLine(const char* szData, const char* szText)
{
	CString strLine = "";
	CString strData = szData;

	if (strData != "")
	{
		CString strText = szText;

		if (strText == "")
		{
			strLine = strData;
		}
		else
		{
			strLine = strText;
			strLine += strData;
		}
	}

	return strLine;
}

//**********************************************************************

void CServerWrapper::SaveLine(CString strLine)
{
	if (strLine != "")
	{
		m_LoyReqThreadManager.AddResponseLine(strLine);
	}
}

//***************************************************************

