/***********************************************************************/
#include <math.h>
/***********************************************************************/
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
/***********************************************************************/
#include "..\SmartPay4Shared\SQLAuditRecord_base.h"
#include "..\SmartPay4Shared\PaymentAuditRecord.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLPurse2Handler.h"
#include "..\SmartPay4Shared\SQLPurchaseControlBuffer.h"
/***********************************************************************/
#include "..\SmartPay4Shared\SQLTable_Account\SQLRowAccountFull.h"
#include "..\SmartPay4Shared\SQLTable_AuditPeriod\SQLRowSetAuditPeriod.h"
#include "..\SmartPay4Shared\SQLTable_EposTerminal\SQLRepositoryEposTerminal.h"
#include "..\SmartPay4Shared\SQLTable_Group\SQLRepositoryGroup.h"
#include "..\SmartPay4Shared\SQLTable_OperatorName\SQLRepositoryOperatorName.h"
#include "..\SmartPay4Shared\SQLTable_PeriodRefresh\SQLRepositoryPeriodRefresh.h"
#include "..\SmartPay4Shared\SQLTable_PluInfo\SQLRepositoryPluInfo.h"
#include "..\SmartPay4Shared\SQLTable_PurchaseControlRule\SQLRepositoryPurchaseControlRule.h"
/***********************************************************************/
#include "ServerSubs.h"
#include "SQLTranImportEposTerminal.h"
#include "ServerWrapper.h"
/***********************************************************************/
#include "EposSmartPayFile.h"
/***********************************************************************/

/*
$TRAN, 00001, 1, 1, 001000195, 250, 1, 1, 19 / 07 / 07, 11:22 : 04, 2
$PAYM, 20, Biometric, 250, n
$ITEM, 00001, 00000010, 1, 250

Add to Card transaction
$TRAN, 000003, 1, 1, 001007588, 1000, 2, 2, 29 / 04 / 08, 13:49 : 06, 0
$PAYM, 1, Cash, 1000
$ITEM, 000003, 90002000, 1, 1000

Refund
$TRAN, 000002, 1, 1, 001000076, -120, 1, 1, 17 / 05 / 10, 11 : 17 : 30, 0
$PAYM, 20, Cashless(account), -120
$ITEM, 000002, 00000040, 1, -120

Kiosk BioLinkAdd to Card transaction
$TRAN - BIO, 000003, 1, , 12324, 1000, , , 29 / 04 / 2101, 13 : 49

Vending transaction sales to Purse1
$TRAN - VND, 000001, 101, 901032546, 2.00, 16 / 06 / 2017, 10 : 38 : 08

// new format with descriptions on end of line
$TRAN, 4, 1, 1, 001342455, 500, 6, 2, 16 / 06 / 14, 15:25 : 33, 0
$PAYM, 1, Cash, 500, 0
$LDISC, 4, -55, D, 0, 'Discount (%)'
$ITEM, 4, 00002010, 5, 111, 'Boddies'


$TRAN, 000000, 1, 1, 001000025, 750, 9, 9, 16 / 10 / 14, 11:21 : 33, 0
$PAYM, 1, Cash, 750, 0
$ITEM, 000000, 00000340, 1, 50, 'BREAKFAST ITEM'
$ITEM, 000000, 00000190, 1, 100, 'BAC/SAUS IN ROLL'
$ITEM, 000000, 00000220, 1, 100, 'JUMBO SAUSAGE ROLL'
$ITEM, 000000, 00000340, 1, 50, 'BREAKFAST ITEM'
$ITEM, 000000, 00000190, 1, 100, 'BAC/SAUS IN ROLL'
$ITEM, 000000, 00000220, 1, 100, 'JUMBO SAUSAGE ROLL'
$ITEM, 000000, 00000340, 1, 50, 'BREAKFAST ITEM'
$ITEM, 000000, 00000190, 1, 100, 'BAC/SAUS IN ROLL'
$ITEM, 000000, 00000220, 1, 100, 'JUMBO SAUSAGE ROLL'

$TRAN, 1156406560, 1, 1, 001000026, 275, 4, 4, 16 / 10 / 14, 11:22 : 37, 0
$PAYM, 20, Cashless(account), 275, 6
$ITEM, 1156406560, 00001210, 1, 65, 'FUNTIME 200ml'
$ITEM, 1156406560, 00001200, 1, 50, 'CALYSPO CUP'
$ITEM, 1156406560, 00001170, 1, 80, 'FRUIT FIZZ'
$ITEM, 1156406560, 00001180, 1, 80, 'FRUIT COLA'

$TRAN, 000000, 1, 1, 001000027, 250, 3, 3, 16 / 10 / 14, 11:24 : 01, 0
$PAYM, 1, Cash, 250, 0
$ITEM, 000000, 00000340, 1, 50, 'BREAKFAST ITEM'
$ITEM, 000000, 00000190, 1, 100, 'BAC/SAUS IN ROLL'
$ITEM, 000000, 00000220, 1, 100, 'JUMBO SAUSAGE ROLL'

$TRAN, 1434713275, 1, 1, 001003351, -200, 1, 1, 21 / 10 / 14, 10:15 : 49, 0
$PAYM, 1, CASH, -200, 0
$ITEM, 1434713275, 00000150, 1, -200

// Purse3

$TRAN, 000003, 1, 1, 001000140, 80, 1, 1, 07 / 02 / 15, 07:43 : 36, 0
$PAYM, 20, Cashless(account), 80, 6, 3			... Price, Points, Purse3
$ITEM, 000003, 00001520, 1, 80, 'TWIX', 11, A		... Description, DeptNo, TaxCode

$TRAN, 000003, 1, 1, 001000141, 330, 2, 2, 07 / 02 / 15, 07 : 47 : 34, 0
$PAYM, 20, Cashless(account), 80, 6, 3
$PAYM, 20, Cashless(account), 250, 6, 1
$ITEM, 000003, 00000610, 1, 250, 'MAIN MEAL 1', 2, A
$ITEM, 000003, 00001520, 1, 80, 'TWIX', 11, A

// Vending
// $TRAN-VND,UserID,TNo,CCNo,Value,Value2,19/07/2017,11:22:04
*/

/**********************************************************************/
// logfilepath = "c:\xloylog\LOYnnn.$$1"	( renamed from .LOG )

CEposSmartPayFile::CEposSmartPayFile(CPendingExportPaymentData& PendingExportPaymentData, CPendingEposTerminalData& PendingEposTerminalData, CStringArray* pExternalLogData) : 
	m_PendingExportPaymentData(PendingExportPaymentData),
	m_PendingEposTerminalData(PendingEposTerminalData)
{
	m_strFilename = "";
	m_pExternalLogData = pExternalLogData;
	m_bExternalMode = TRUE;
	m_nExternalCCNo = 0;
	m_nTransactionSourceType = AUDIT_EXT;
	m_nFolderSet = 0;
	m_nREQFileType = REQFILETYPE_UNKNOWN;
	Init();
}

/**********************************************************************/

CEposSmartPayFile::CEposSmartPayFile(const char* szRenamedLogFile, int nFolderSet, CPendingExportPaymentData& PendingExportPaymentData, CPendingEposTerminalData& PendingEposTerminalData) :
	m_PendingExportPaymentData(PendingExportPaymentData),
	m_PendingEposTerminalData(PendingEposTerminalData)
{
	m_strFilename = szRenamedLogFile;
	m_pExternalLogData = NULL;
	m_bExternalMode = FALSE;
	m_nExternalCCNo = 0;
	m_nTransactionSourceType = AUDIT_POS;
	m_nFolderSet = nFolderSet;
	m_nREQFileType = REQFILETYPE_UNKNOWN;
	Init();
}

/**********************************************************************/

void CEposSmartPayFile::CEposSmartPayFile::Init()
{
	m_strTmpPaymentAuditFile = Filenames.GetTempPathFilename(::GetUniqueTempFilename("$02"));		// temp file

	m_strEposUserID = "";
	m_bValidData = FALSE;
	m_strError = "";
	m_bMultipleTransactions = FALSE;
	m_nTransactionLineCount = 0;

	m_nTransactionAppNo = APPNO_SALE;				// assume its a sale , can be a refund
	m_nTransactionCCNo = 0;

	m_dateTransaction.SetDate("");
	m_timeTransaction.SetTime("");
	m_nTillNo = 0;
	m_strServerID = "";
	m_strTransactionNo = "";						// transaction No ( receipt number )
	m_nPluPoints = 0;						// points from plu sale

	m_nTransactionItemCount = 0;						// number of items in the transaction
	m_dEposTransactionValue = 0;						// total transaction value
	m_dEposSpendPurse1Total = 0;
	m_dEposSpendPurse1Liability = 0;					// $PAYM spend value
	m_dEposSpendPurse1Credit = 0;						// $PAYM spend value
	m_dEposSpendPurse2 = 0;						// $PAYM spend value						
	m_dEposSpendPurse3Total = 0;
	m_dEposSpendPurse3Liability = 0;					// $PAYM spend value
	m_dEposSpendPurse3Credit = 0;						// $PAYM spend value
	m_dEposUnknownPursePaym20 = 0;						// $PAYM line old format no Purse indicator
	m_dEposUnknownPursePaym21 = 0;
	m_dCashSpend = 0;						// none purse spend ( Cash \ Credit \ Cheque etc )

	//m_strRestrictedP3List	= "";						// list of restricted departments

	m_dAddToPurse1Value = 0;						// Purse1 revaluation plu
	m_dAddToPurse3Value = 0;						// Purse3 revaluation plu
	m_nPointsAwarded = 0;						// gets printed on epos receipt

	m_nRevaluationType = 0;				// Server or group specfic
	m_dRevaluationTrip = 0;
	m_dRevaluationTrip2 = 0;
	m_nRevaluationPoints = 0;
	m_dRevaluationValue = 0;

	m_nRewardType = 0;				// Server or group specific
	m_dRewardSpendTrip = 0;
	m_nRewardPoints = 0;
	m_dRewardValue = 0;
	m_dRewardUpperSpendLimit = 0;
	m_bHaveRewardUpperSpendLimit = FALSE;
	m_dRewardMultiplierTrigger = 0;

	m_bOnDutyGroupShift = FALSE;

	SQLPurchaseControlBuffer.Reset();
	m_PurchaseControlCache.Reset();
}

//*******************************************************************

bool CEposSmartPayFile::ScanSharpPos()
{
	CSSFile file;
	if ( file.Open ( m_strFilename, "rb" ) == FALSE )
	{
		m_strError.Format ( "Cannot open file %s", (const char*) m_strFilename );
		return FALSE;
	}

	CServerWrapper::UpdatePluInfo();
	//CServerWrapper::UpdateOperatorInfo();
	CServerWrapper::UpdateEposTerminalInfo();
	CServerWrapper::UpdatePurchaseControlInfo();

	m_nTransactionLineCount = 0;						// total number of lines read in log file 
	m_bOnDutyGroupShift = FALSE;

	CCSV csvLine{};
	CString strLabel = "";
	while ( file.Read ( csvLine ) == TRUE )
	{
		csvLine.Get ( 0, strLabel );					// get record type

		if (strLabel == "$TRANX")
		{
			ProcessTranLineExtra(&csvLine);
		}
		else if ( strLabel.Left(5) == "$TRAN" )				// $TRAN / $TRAN-BIO / $TRAN-VND
		{
			if ( m_nTransactionLineCount != 0 )			// 2nd trans line found
			{
				m_bMultipleTransactions = TRUE;			// exist - process it next time
				break;
			}

			if (ProcessTranLineSharpPos(&csvLine) == FALSE)	// see if valid trans line
			{
				break;
			}
		}
		else if (strLabel == "$PAYM")
		{
			ProcessPaymLine(&csvLine);
		}
		else if (strLabel == "$ITEM")
		{
			ProcessItemLine(&csvLine);
		}
		else if (strLabel == "$LDISC")
		{
			ProcessDiscountLine(&csvLine);		
		}
		else if (strLabel == "$SDISC")
		{
			ProcessDiscountLine(&csvLine);		
		}
		else if (strLabel == "$IDISC")
		{
			ProcessItemDiscountLine(&csvLine);
		}
		else if (strLabel == "$PCTRL_SALE")
		{
			ProcessPurchaseControlStampAwardLine(&csvLine);
		}

		++m_nTransactionLineCount;
	}

	file.Close();

	// no valid data found & no error message set
	if ( ( FALSE == m_bValidData )&& ( m_strError == "" ))			
	{
		m_strError = "Invalid data format";
	}

	return m_bValidData;
}

//*******************************************************************

bool CEposSmartPayFile::ScanExternal()
{
	CServerWrapper::UpdatePluInfo();
	//CServerWrapper::UpdateOperatorInfo();
	CServerWrapper::UpdateEposTerminalInfo();
	CServerWrapper::UpdatePurchaseControlInfo();

	CString strLabel = "";
	for (int n = 0; n < m_pExternalLogData->GetSize(); n++)
	{
		CCSV csvLine(m_pExternalLogData->GetAt(n));

		csvLine.Get(0, strLabel);

		if (strLabel == "$TRAN")
		{
			if (ProcessTranLineExternal(&csvLine) == FALSE)
			{
				break;
			}
		}
		else if (strLabel == "$PAYM")
		{
			ProcessPaymLine(&csvLine);
		}
		else if (strLabel == "$ITEM")
		{
			ProcessItemLine(&csvLine);
		}
		else if (strLabel == "$LDISC")
		{
			ProcessDiscountLine(&csvLine);
		}
		else if (strLabel == "$SDISC")
		{
			ProcessDiscountLine(&csvLine);
		}
	}

	if ((FALSE == m_bValidData) && (m_strError == ""))
	{
		m_strError = "Invalid data format";
	}

	return m_bValidData;
}

//*******************************************************************
// remove the processed transaction lines from start of a multiple log file 

bool CEposSmartPayFile::RemoveTransactionLines ( const char* szLogFilePath, const char* szSecondaryLogFile )
{
	return ::RemoveFileLines ( szLogFilePath, m_nTransactionLineCount, FALSE, szSecondaryLogFile );
}

//*******************************************************************
// $TRAN,1156406560,1,1,001000026,275,4,4,16/10/14,11:22:37,0
// $TRAN,000001,1,1,001000195,250,1,1,19/07/07,11:22:04,2
// $TRAN-BIO,UserID,TNo,,CCNo,Value,,,19/07/07,11:22:04			// old Kiosk formant ( no purse 1/3 )
// $TRAN-BIO,UserID,TNo,,CCNo,Value,PurseNo,,19/07/07,11:22:04	// New Kiosk BioLink single line

// $TRAN-VND,UserID,TNo,CCNo,Value,Value2,19/07/2017,11:22:04	// Vending single line ( different format)

bool CEposSmartPayFile::ProcessTranLineSharpPos(CCSV* pCsv)
{
	int nRecordType = nRECORDTYPE_EPOS;			// assume EPOS transaction
	m_nTransactionSourceType = AUDIT_POS;		// assume EPOS audit line

	CString strLabel = pCsv->GetString(0);
	m_strFullEposUserID = pCsv->GetString(1);		// get card number passed in log file ( as its used by New balance on receipt)
	m_nTillNo = pCsv->GetInt(2);			// till number / Kiosk / Vending machine

	// validate UserID & strip off leading 0's
	m_bValidData = Server.ValidateUserID(m_strFullEposUserID, m_strEposUserID);

	if (FALSE == m_bValidData)
	{
		m_strError.Format("#ERR,%s", Server.GetError());
		return FALSE;
	}

	// never implemented ********************************************************************
	//	if ( strLabel == "$TRAN-VND" )							// see if Vending Transaction line
	//	{														// yes - has different format to epos \ kiosk
	// ********************************************************
	//		nRecordType					= nRECORDTYPE_VENDING;
	//		m_nTransactionSourceType	= AUDIT_VENDING;
	//
	//		m_strTransactionNo		= pCsv->GetString(3);		// transaction number
	//		++m_nTransactionItemCount;							// inc number of transactions ( so sales get audited )
	//
	//		m_dEposTransactionValue = pCsv->GetDouble(4);		// card transaction value
	//		m_dEposSpendPurse1		= m_dEposTransactionValue;
	//		m_dCashSpend			= pCsv->GetDouble(5);		// cash \ credit card sale
	//
	//		m_dateTransaction.SetDate ( pCsv->GetString(6) );	// date last used dd/mm/yy
	//		m_timeTransaction.SetTime ( pCsv->GetString(7) );	// time last used (hh:mm:ss)
	//
	// ********************************************************
	//	}
	// never implemented ********************************************************************

	if (strLabel == "$TRAN-BIO") // see if Kiosk Transaction line
	{
		nRecordType = nRECORDTYPE_KIOSK;
		m_nTransactionSourceType = AUDIT_KIOSKLINK;

		//m_strServerID	= pCsv->GetString(3);			// Server Number ( not used )
		m_strTransactionNo = pCsv->GetString(4);		// CCNO

		m_dEposTransactionValue = pCsv->GetDouble(5) / 100;	// transaction value

		int nPurseNo = pCsv->GetInt(6);						// 1=purse1, 3=purse3

		m_dCashSpend = 0;						// all values added to a purse
		if (nPurseNo == 3)
		{
			m_dAddToPurse3Value = m_dEposTransactionValue;
		}
		else
		{
			m_dAddToPurse1Value = m_dEposTransactionValue;
		}

		m_dateTransaction.SetDate(pCsv->GetString(8));
		m_timeTransaction.SetTime(pCsv->GetString(9));	// time last used (hh:mm:ss)

		//		nPoints = pCsv->GetInt(10);							// points awarded at epos ( not used )
	}
	else	// SharpPos transaction
	{
		m_strServerID = pCsv->GetString(3);			// Server Number
		m_strTransactionNo = pCsv->GetString(4);			// transaction number 9digits

		m_dEposTransactionValue = pCsv->GetDouble(5) / 100;	// transaction value
		m_dCashSpend = m_dEposTransactionValue;	// assume all spend not via Purse1

		if (m_dEposTransactionValue < 0.0)
		{
			m_nTransactionAppNo = APPNO_REFUNDSALE;			// negative sale = refund
		}

		CString strBuf = pCsv->GetString(8);				// date last used dd/mm/yy
		if (strBuf.GetLength() == 8)
		{
			CString strTmp;
			strTmp = strBuf.Right(2);						// get year

			int nYear = atoi((const char*)strTmp);		// adjust to yyyy
			if (nYear > 96)
			{
				nYear += 1900;
			}
			else
			{
				nYear += 2000;
			}

			CString strDate = "";
			strDate.Format("%6.6s%4.4d", (const char*)strBuf, nYear);
			m_dateTransaction.SetDate(strDate);
		}
		else
		{
			m_dateTransaction.SetDate(strBuf);
		}

		m_timeTransaction.SetTime(pCsv->GetString(9));	// time last used (hh:mm:ss)

		//		nPoints = pCsv->GetInt(10);							// points awarded at epos ( not used )

		m_nREQFileType = pCsv->GetInt(11);
	}

	m_PendingExportPaymentData.m_PaymentDataPebble.SetDate(m_dateTransaction.GetDate());
	m_PendingExportPaymentData.m_PaymentDataPebble.SetTime(m_timeTransaction.GetTime());
	m_PendingExportPaymentData.m_PaymentDataChartwell.SetDate(m_dateTransaction.GetDate());
	m_PendingExportPaymentData.m_PaymentDataChartwell.SetTransactionID(pCsv->GetInt64(4));
	m_PendingExportPaymentData.m_PaymentDataChartwell.SetTerminalNo(m_nTillNo);

	//****
	// save TLID

	CSQLRepositoryEposTerminal RepoTerm;
	m_PendingEposTerminalData.m_bGotChange = TRUE;
	m_PendingEposTerminalData.m_RowEposTerminal.SetTerminalNo(m_nTillNo);
	RepoTerm.SelectRow(m_PendingEposTerminalData.m_RowEposTerminal, NULL);

	m_PendingEposTerminalData.m_RowEposTerminal.SetLastUsedDate(m_dateTransaction.GetCSVDate());
	m_PendingEposTerminalData.m_RowEposTerminal.SetLastUsedTime(m_timeTransaction.GetCSVTime());
	m_PendingEposTerminalData.m_RowEposTerminal.SetUserID(_atoi64(m_strFullEposUserID));
	m_PendingEposTerminalData.m_RowEposTerminal.SetCardValue(m_dEposTransactionValue);

	if (nRECORDTYPE_KIOSK == nRecordType)
	{
		m_PendingEposTerminalData.m_RowEposTerminal.UpdateTotalInDrawer(m_dEposTransactionValue);
	}

	m_PendingEposTerminalData.m_RowEposTerminal.IncrementCCNo();
	
	return TRUE;
}

/******************************************************************************/

bool CEposSmartPayFile::ProcessTranLineExternal(CCSV* pCsv)
{
	int nRecordType = nRECORDTYPE_EPOS;			// assume EPOS transaction
	m_nTransactionSourceType = AUDIT_EXT;		// assume EPOS audit line

	CString strLabel = pCsv->GetString(0);
	m_strFullEposUserID = pCsv->GetString(1);		// get card number passed in log file ( as its used by New balance on receipt)
	
	m_bValidData = Server.ValidateUserID(m_strFullEposUserID, m_strEposUserID);

	if (FALSE == m_bValidData)
	{
		m_strError.Format("#ERR,%s", Server.GetError());
		return FALSE;
	}
	
	m_nTillNo = pCsv->GetInt(2);					// till number / Kiosk / Vending machine
	
	{
		m_strServerID = pCsv->GetString(3);			// Server Number
		//m_strTransactionNo = pCsv->GetString(4);	// transaction number 9digits
		m_strTransactionNo = "";

		m_dEposTransactionValue = pCsv->GetDouble(5) / 100;	// transaction value
		m_dCashSpend = m_dEposTransactionValue;	// assume all spend not via Purse1

		if (m_dEposTransactionValue < 0.0)
		{
			m_nTransactionAppNo = APPNO_REFUNDSALE;			// negative sale = refund
		}

		CString strBuf = pCsv->GetString(8);				// date last used dd/mm/yy
		if (strBuf.GetLength() == 8)
		{
			CString strTmp;
			strTmp = strBuf.Right(2);						// get year

			int nYear = atoi((const char*)strTmp);		// adjust to yyyy
			if (nYear > 96)
			{
				nYear += 1900;
			}
			else
			{
				nYear += 2000;
			}

			CString strDate = "";
			strDate.Format("%6.6s%4.4d", (const char*)strBuf, nYear);
			m_dateTransaction.SetDate(strDate);
		}
		else
		{
			m_dateTransaction.SetDate(strBuf);
		}

		m_timeTransaction.SetTime(pCsv->GetString(9));	// time last used (hh:mm:ss)
	}

	return TRUE;
}

/******************************************************************************/

void CEposSmartPayFile::ProcessTranLineExtra(CCSV* pCsv)
{
	for (int n = 1; n < pCsv->GetSize(); n++)
	{
		CString strTag = pCsv -> GetString(n);
		if (strTag == "ODGS=1")
		{
			m_bOnDutyGroupShift = TRUE;
			break;
		}
	}
}

/******************************************************************************/
// $PAYM,20,Cashless (account),80,6,3
// $PAYM,20,Cashless (account),250,6,1

// PaymentMethodType = 0(cash),1(cheque),2(eft),3(eftCNP),4(PostToRoom),5(PostToRoomManual),6(RewardPoints),7(currency),8(FromDeposit),9(eft(bill lodging)),99(other)

void CEposSmartPayFile::ProcessPaymLine(CCSV* pCsv)
{
	int nSPOSPaymentNo = pCsv->GetInt(1);				// payment number NOT type
	CString strPaymentName = pCsv->GetString(2);
	int nProcessPaymentNo = nSPOSPaymentNo;
	int nPaymentType = pCsv->GetInt(4);
	double dValue = pCsv->GetDouble(3) / 100;			// amount spent from purse (implied 2dps)

	int nSPOSPurseNo = -1;

	switch (nSPOSPaymentNo)
	{
		//SHOULD BE ACTUAL CANTEEN PAYMENT IN SPOS
	case 20:
		if (pCsv->IsEmpty(5) == FALSE)
		{
			nSPOSPurseNo = pCsv->GetInt(5);
		}
		nPaymentType = 6;
		break;

		//SHOULD BE CASH PAYMENT IN SPOS REDIRECTED TO CANTEEN
	case 21:
		nProcessPaymentNo = 20;
		nPaymentType = 6;
		break;
	}

	if (20 == nProcessPaymentNo)
	{
		if (nSPOSPurseNo != -1)
		{
			double dPurse1 = 0;
			double dPurse2 = 0;
			double dPurse3 = 0;

			switch (nSPOSPurseNo)
			{
			case 1:
				m_dEposSpendPurse1Total = dValue;
				dPurse1 = dValue;
				break;

			case 2:
				m_dEposSpendPurse2 = dValue;
				dPurse2 = dValue;
				break;

			case 3:
				m_dEposSpendPurse3Total = dValue;
				dPurse3 = dValue;
				break;
			}

			LogEposPaymentEntry(nSPOSPaymentNo, dPurse1, dPurse2, dPurse3);
		}
		else												// old format data
		{
			// need to work out which purse was used
			if (21 == nSPOSPaymentNo)
			{
				m_dEposUnknownPursePaym21 += dValue;		
			}
			else
			{
				m_dEposUnknownPursePaym20 += dValue;
			}
		}

		m_dCashSpend -= dValue;								// deduct card portion from total transaction value
	}
	else
	{
		LogEposPaymentEntry(nSPOSPaymentNo, dValue);			// All other transaction spend ( Cash \ Credit \ Cheque etc )
	}
	
	if ( (nPaymentType >= 0) && ( nPaymentType <= PebbleExportPayment::PaymentType1.Max ))
	{
		if (TRUE == m_PendingExportPaymentData.m_bPebble)
		{
			m_PendingExportPaymentData.m_PaymentDataPebble.ConsolidatePayment(nPaymentType, dValue);
		}
	}
}

//*******************************************************************
// $ITEM,00001,00000010,1,250
// $ITEM,0000000004,00002010,5,111,'Boddies'
// $ITEM,1156406560,00001210,1,65,'FUNTIME 200ml'
// $ITEM,1156406560,00001200,1,50,'CALYSPO CUP'
// $ITEM,1156406560,00001170,1,80,'FRUIT FIZZ'
// $ITEM,1156406560,00001180,1,80,'FRUIT COLA'

//$ITEM,000003,00001520,1,80,'TWIX',11,A		Description , DeptNo , Taxcode

void CEposSmartPayFile::ProcessItemLine(CCSV* pCsv)
{
	CString strPluNo = pCsv->GetString(2);					// pluno sold
	double dLineQty = pCsv->GetDouble(3);				// quantity sold
	int nPerItemVal = pCsv->GetInt(4);
	double dLineVal = pCsv->GetDouble(4);				// value of 1 item sold

	dLineVal = (dLineVal * dLineQty) / 100.0;					//v1.04b 16/06/2015

	strPluNo.TrimLeft('0');										// remove leading 0's

	if (strPluNo == Server.GetPluNo())				// see if its Purse1 revaluation open plu
	{
		m_dAddToPurse1Value += dLineVal;
		m_dCashSpend -= dLineVal;									// deduct added to card value from total transaction value
	}
	else if (strPluNo == Server.GetPluNo2())			// see if its Purse3 revaluation open plu
	{
		m_dAddToPurse3Value += dLineVal;
		m_dCashSpend -= dLineVal;									// deduct added to card value from total transaction value
	}
	else
	{
		CSQLRepositoryPluInfo RepoPlu;
		m_SQLRowPluInfo.SetPluNo(_atoi64(strPluNo));
		bool bFoundPlu = (RepoPlu.SelectRow(m_SQLRowPluInfo, NULL).GetSQLError() == SQLCRUD_ERR_NONE);

		if (dLineVal < 0.0)										// if value -ve make Qty -ve
		{
			dLineQty = -(dLineQty);
		}

		CString strText = pCsv->GetString(5);				// see if text passed

		int nDeptNo = 0;
		CString strTaxCode = "";

		if (FALSE == m_bExternalMode)
		{
			nDeptNo = pCsv->GetInt(6);					// DeptNo
			strTaxCode = pCsv->GetString(7);			// TaxCode A-L
		}
		else if (TRUE == bFoundPlu)
		{
			nDeptNo = m_SQLRowPluInfo.GetDeptNo();
			strTaxCode = m_SQLRowPluInfo.GetTaxCode();
		}

		if (strText != "")
		{
			strText.TrimLeft('\x027');						// remove leading '
			strText.TrimRight('\x027');						// remove trailing '
		}

		int nItemType = 0;
		CString strItemNo = "";

		if (nDeptNo == 0)										// see id DeptNo passed
		{
			strItemNo = strPluNo;								// no pluno prefix
			nItemType = nTLOGTYPE_PLU;							// no DeptNo prefix
		}
		else
		{
			strItemNo.Format("%4.4d%s", nDeptNo, (const char*)strPluNo);	// prefix PluNo with DeptNo
			nItemType = nTLOGTYPE_PLU2;							// Prefix PluNo with DeptNo ( DDDDnnn.... )
		}

		int nPoints = 0;										// assume no points
		if (TRUE == bFoundPlu)
		{
			if (strText == "")								// if text not passed - use plu database text
			{
				strText = m_SQLRowPluInfo.GetDescription();
			}

			nPoints = m_SQLRowPluInfo.GetPoints() * (int)dLineQty;	// calc points for plu qty
			m_nPluPoints += nPoints;								// add to total transaction points
		}
		else
		{
			if (strText == "")
			{
				strText.Format("Plu %s", (const char*)strPluNo);
			}
		}

		++m_nTransactionItemCount;
		m_TLogRecord.SaveItemData(nItemType, strItemNo, strText, dLineQty, nPerItemVal, dLineVal, nPoints, strTaxCode);
	}
}

//*******************************************************************
// $LDISC,0600101537,-231,D,2000

void CEposSmartPayFile::ProcessDiscountLine ( CCSV* pCsv )
{
	double dValue = pCsv->GetDouble(2) / 100;
	m_TLogRecord.SaveItemData ( nTLOGTYPE_DISC_SUBT, "", "Discount", 1, 0, dValue, 0, "" );
}

//*******************************************************************

void CEposSmartPayFile::ProcessItemDiscountLine(CCSV* pCsv)
{
	CString strPluNo = pCsv->GetString(2);
	::TrimSpacesFromString(strPluNo, TRUE);
	double dValue = pCsv->GetDouble(3) / 100;

	CString strDescription = pCsv->GetString(6);
	::TrimSpacesFromString(strDescription, FALSE);

	if (strDescription == "")
	{
		strDescription = "Item Discount";	
	}

	m_TLogRecord.SaveItemData(nTLOGTYPE_DISC_ITEM, strPluNo, strDescription, 1, 0, dValue, 0, "");
}

//*******************************************************************

void CEposSmartPayFile::ProcessPurchaseControlStampAwardLine(CCSV* pCsv)
{
	__int64 nPluNo = pCsv->GetInt64(1);
	CString strPluDescription = pCsv->GetString(2);
	int nQty = pCsv->GetInt(3);
	int nRuleID = pCsv->GetInt(4);
	int nStampCount = pCsv->GetInt(5);

	if (nQty <= 0)
	{
		return;
	}

	if (strPluDescription != "")
	{
		strPluDescription.TrimLeft('\x027');					// remove leading '
		strPluDescription.TrimRight('\x027');					// remove trailing '
	}

	if (strPluDescription == "")
	{
		CSQLRowPluInfo RowPluInfo;
		RowPluInfo.SetPluNo(nPluNo);

		CSQLRepositoryPluInfo RepoPlu;
		if (RepoPlu.SelectRow(RowPluInfo, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
		{
			strPluDescription = RowPluInfo.GetDescription();
		}
	}

	if ((nRuleID < PurchaseControlRule::RuleID.Min) || (nRuleID > PurchaseControlRule::RuleID.Max))
	{
		return;
	}

	CSQLRowPurchaseControlRule RowSO;
	if (m_PurchaseControlCache.GetPurchaseControlRow(nRuleID, RowSO) == FALSE)
	{
		return;
	}

	if (RowSO.GetEnableFlag() == FALSE)
	{
		return;

	}

	CSQLPurchaseControlPendingInfo info;
	info.m_nControlID = nRuleID;
	info.m_strControlName = RowSO.GetDisplayDescription();
	info.m_nExpiryDays = RowSO.GetExpiryDays();
	info.m_nLineStamps = nStampCount;
	info.m_nPluNo = nPluNo;
	info.m_strPluDescription = strPluDescription;
	info.m_nPluQty = nQty;

	SQLPurchaseControlBuffer.BufferAward(info);
}

//*******************************************************************

void CEposSmartPayFile::UpdateRecordFromLogData( CSQLRowAccountFull& RowAccount, int& nGroupNo )
{
	m_atc.ClearPendingLines();

	m_nTransactionCCNo = 0;
	m_nPointsAwarded = 0;

	bool bIsRedeemDisabled = Server.GetRedeemDisableAutoFlag();

	CRedeemPoints pointsRedeemer;										// points redeem

	m_atc.Set(&RowAccount);													// set audit record to database record
	m_atc.SetTerminalNo(m_nTillNo);
	m_atc.SetOperatorID(m_strServerID);
	m_atc.SetAuditLineExtraText(m_strTransactionNo);
	m_atc.SetFolderSet(m_nFolderSet);
	m_atc.SetREQFileType(m_nREQFileType);

	//*******************************************************************

	nGroupNo = RowAccount.GetGroupNo();
	{
		// get alternate daily group for transaction day
		int nDayGroupNo = RowAccount.GetDailyGroupNo(m_dateTransaction.GetDayOfWeek() - 1);
		
		if (nDayGroupNo != 0)
		{
			nGroupNo = nDayGroupNo;
			m_atc.SetAlternateGroupNo(nGroupNo);
		}
	}

	if (TRUE == m_bOnDutyGroupShift)
	{
		nGroupNo = 4;
		m_atc.SetAlternateGroupNo(4);
	}

	CSQLRowGroupFull RowGroup;
	RowGroup.SetGroupNo(nGroupNo);

	CSQLRepositoryGroup RepoGroup;
	bool bGotGroup = (RepoGroup.SelectRow(RowGroup, NULL).GetSQLError() == SQLCRUD_ERR_NONE);

	GetRevalueSettings(RowGroup, bGotGroup);
	GetRewardSettings(RowGroup, bGotGroup);
	GetRedeemSettings(RowGroup, bGotGroup, bIsRedeemDisabled, pointsRedeemer);

	m_bHaveRewardUpperSpendLimit = (m_dRewardUpperSpendLimit == 0) ? FALSE : TRUE;

	if (m_dateTransaction.IsSet() == TRUE)
	{
		m_atc.SetDateLastUsed(m_dateTransaction.GetDate());
		m_atc.SetTimeLastUsed(m_timeTransaction.GetTime());
	}

	ProcessPurse2Refresh(RowAccount, RowGroup);
	ProcessPurse1Revaluation(RowAccount);
	ProcessPurse3Revaluation(RowAccount);
	ProcessUnknownPurse(RowAccount, RowGroup, m_dEposUnknownPursePaym20, 20);
	ProcessUnknownPurse(RowAccount, RowGroup, m_dEposUnknownPursePaym21, 21);

	if ((m_nTransactionItemCount > 0) || (TRUE == m_bExternalMode))
	{
		ProcessPurse1Spend(RowAccount, RowGroup);
		ProcessPurse2Spend(RowAccount, RowGroup);
		ProcessPurse3Spend(RowAccount, RowGroup);

		RowAccount.AddToCashSpendToDate(m_dCashSpend);			// add to cash spend to date
		RowAccount.UpdatePoints(m_nPluPoints);					// add plu points to points & pointsTD ( if any )

		m_atc.SetTransactionPoints(m_nPluPoints);
		m_atc.SetTransactionCash(-(m_dCashSpend));
		m_atc.SetTransactionPurse1Liability(-(m_dEposSpendPurse1Liability));
		m_atc.SetTransactionPurse1Credit(-(m_dEposSpendPurse1Credit));
		m_atc.SetTransactionPurse2Balance(-(m_dEposSpendPurse2));
		m_atc.SetTransactionPurse3Liability(-(m_dEposSpendPurse3Liability));
		m_atc.SetTransactionPurse3Credit(-(m_dEposSpendPurse3Credit));
		m_atc.SetSourceType(m_nTransactionSourceType);	// EPOS \ VENDING \ kiosk
		m_atc.SetApplicationNo(m_nTransactionAppNo);		// card sale or refund
		SaveAudit(RowAccount);

		m_nTransactionCCNo = m_atc.GetCCNoFull();				// get the CCNo used to save epos sale for use by purchase history
		m_nPointsAwarded += m_nPluPoints;

		ProcessRewardSpendBonus(RowAccount);
	}

	// check for auto points redeem
	if (bIsRedeemDisabled == FALSE)
	{
		AutoRedeemPoints(RowAccount, &pointsRedeemer);
	}
}

//*******************************************************************

void CEposSmartPayFile::GetRevalueSettings(CSQLRowGroupFull& RowGroup, bool bGotGroup)
{
	bool bUseSystemRevaluation = TRUE;

	if (RowGroup.GetActiveRevalueFlag() == TRUE)				// revaluation bonus
	{
		bUseSystemRevaluation = FALSE;

		if (RowGroup.GetRevalueDisable() == TRUE)			// see if disabled for this group
		{
			m_dRevaluationTrip = 0;
			m_dRevaluationTrip2 = 0;
		}
		else
		{
			m_dRevaluationTrip = RowGroup.GetRevalueTrip1();
			m_dRevaluationTrip2 = RowGroup.GetRevalueTrip2();
			m_nRevaluationType = RowGroup.GetRevalueType();
			m_nRevaluationPoints = RowGroup.GetRevaluePoints();
			m_dRevaluationValue = RowGroup.GetRevalueBonus();
		}
	}

	if ( TRUE == bUseSystemRevaluation)
	{
		m_dRevaluationTrip = Server.GetActiveRevaluationTrip();
		m_dRevaluationTrip2 = Server.GetActiveRevaluationTrip2();
		m_nRevaluationType = Server.GetRevaluationType();
		m_nRevaluationPoints = Server.GetRevaluationPoints();
		m_dRevaluationValue = Server.GetRevaluationValue();
	}
}

//*******************************************************************

void CEposSmartPayFile::GetRewardSettings(CSQLRowGroupFull& RowGroup, bool bGotGroup)
{
	bool bUseSystemReward = TRUE;

	if (TRUE == bGotGroup)
	{
		if (RowGroup.GetActiveRewardFlag() == TRUE)				// reward points bonus
		{
			bUseSystemReward = FALSE;

			if (RowGroup.GetRewardDisable() == TRUE)			// see if disabled for this group
			{
				m_dRewardSpendTrip = 0;
			}
			else
			{
				m_dRewardSpendTrip = RowGroup.GetRewardTrip();
				m_nRewardType = RowGroup.GetRewardType();
				m_nRewardPoints = RowGroup.GetRewardPoints();
				m_dRewardValue = RowGroup.GetRewardBonus();
				m_dRewardUpperSpendLimit = RowGroup.GetRewardLimit();

				double dMinimumSpend = RowGroup.GetRewardTrigger();;
				if (dMinimumSpend != 0)
				{
					m_dRewardMultiplierTrigger = dMinimumSpend;
				}
			}
		}
	}

	if (TRUE == bUseSystemReward)
	{
		m_nRewardType = Server.GetRewardType();
		m_dRewardSpendTrip = Server.GetActiveRewardTrip();
		m_nRewardPoints = Server.GetRewardPoints();
		m_dRewardValue = Server.GetRewardValue();
		m_dRewardUpperSpendLimit = Server.GetRewardUpperSpendLimit();
	}

	m_bHaveRewardUpperSpendLimit = (m_dRewardUpperSpendLimit == 0) ? FALSE : TRUE;
}

//*******************************************************************

void CEposSmartPayFile::GetRedeemSettings(CSQLRowGroupFull& RowGroup, bool bGotGroup, bool& bIsRedeemDisabled, CRedeemPoints& pointsRedeemer )
{
	bool bUseSystemRedeem = TRUE;

	if (TRUE == bGotGroup)
	{
		if (RowGroup.GetActiveRedeemFlag() == TRUE)							// group specifc redeem points
		{
			bUseSystemRedeem = FALSE;
			if (bIsRedeemDisabled == FALSE)									// check not disabled system wide
			{
				bIsRedeemDisabled = RowGroup.GetRedeemDisable();	// individual group could be disabled
			}

			pointsRedeemer.m_nTrip = RowGroup.GetRedeemTrip();
			pointsRedeemer.m_dValue = RowGroup.GetRedeemValue();
			pointsRedeemer.m_nPurse = RowGroup.GetRedeemPurse();
		}
	}

	if (bUseSystemRedeem == TRUE)
	{
		pointsRedeemer.m_nTrip = Server.GetActiveRedeemPointsTrip();
		pointsRedeemer.m_dValue = Server.GetRedeemValue();
		pointsRedeemer.m_nPurse = Server.GetRedeemPurse();
	}

}

//*******************************************************************

void CEposSmartPayFile::ProcessPurse2Refresh(CSQLRowAccountFull& RowAccount, CSQLRowGroupFull& RowGroup)
{
	if (CServerSubs::IsPurseAllowed(2, RowAccount, RowGroup, m_dateTransaction, m_timeTransaction) == TRUE)		// see if Purse2 was allowed at time of transaction
	{
		CString strDate = m_dateTransaction.GetDate();
		CString strTime = m_timeTransaction.GetTime();

		CSQLPurse2Handler purse2(RowAccount, RowGroup);
		double dPurse2 = purse2.GetBalance(TRUE, strDate, strTime);	// get purse2 balance ***** NOTE will refresh & update RefreshDatabase if refresh required *****
		if (purse2.IsRefreshReqd() == TRUE)							// see if purse2 was refreshed / Refresh Database will have been updated
		{
			CString strEODDate = purse2.GetEODRefreshDate();

			if (strEODDate == "")
			{
				RowAccount.RevaluePurse2(dPurse2, strDate, strTime);				// Set purse2 in database record

				m_atc.SetTransactionPurse2Balance(purse2.GetRefreshValue());		// set refresh value
				m_atc.SetCurrentPurse2Balance(RowAccount.GetPurse2Balance());		// set new purse2 balance	
				m_atc.SetSourceType(AUDIT_SERVER);
				m_atc.SetApplicationNo(APPNO_REFRESH);
				SaveAudit(RowAccount);
			}
			else
			{
				RowAccount.RevaluePurse2(dPurse2, strEODDate, "00:00");				// Set purse2 in database record

				m_atc.SetTransactionPurse2Balance(purse2.GetRefreshValue());		// set refresh value
				m_atc.SetCurrentPurse2Balance(RowAccount.GetPurse2Balance());			// set new purse2 balance	
				m_atc.SetSourceType(AUDIT_SERVER);
				m_atc.SetApplicationNo(APPNO_REFRESH);
				SaveAudit(RowAccount);
			}
		}
	}
}

//*******************************************************************

void CEposSmartPayFile::ProcessPurse1Revaluation(CSQLRowAccountFull& RowAccount)
{
	if (m_dAddToPurse1Value != 0.0)						// see if any cash was added
	{
		RowAccount.AddToPurse1Liability(m_dAddToPurse1Value);			// add any AddtoCard value

		m_atc.SetTransactionPurse1Liability(m_dAddToPurse1Value);
		m_atc.SetTransactionPurse1Credit(0.0);
		m_atc.SetTransactionPurse2Balance(0.0);
		m_atc.SetTransactionPurse3Liability(0.0);
		m_atc.SetTransactionPurse3Credit(0.0);
		m_atc.SetTransactionPoints(0);
		m_atc.SetTransactionCash(0.0);
		m_atc.SetSourceType(m_nTransactionSourceType);
		m_atc.SetApplicationNo(APPNO_ADDCASH);			// audit cash added
		SaveAudit(RowAccount);

		m_nTransactionCCNo = m_atc.GetCCNoFull();

		if (m_dRevaluationTrip != 0.0)					// see if any revaluation bonus is required
		{
			if (m_nRevaluationType == 0)						// bonus points
			{
				int nBonusPoints = CalculateRevaluationBonusPoints(m_dAddToPurse1Value, m_dRevaluationTrip, m_nRevaluationPoints);
				if (nBonusPoints != 0)
				{
					RowAccount.UpdatePoints(nBonusPoints);

					m_atc.SetTransactionPoints(nBonusPoints);		// bonus points
					m_atc.SetTransactionPurse1Liability(m_dAddToPurse1Value);	// cash taken 
					m_atc.SetTransactionPurse1Credit(0.0);
					m_atc.SetTransactionPurse2Balance(0.0);
					m_atc.SetTransactionPurse3Liability(0.0);
					m_atc.SetTransactionPurse3Credit(0.0);
					m_atc.SetTransactionCash(0.0);
					m_atc.SetSourceType(AUDIT_SERVER);
					m_atc.SetApplicationNo(APPNO_REVALUEBONUSPOINTS);	// audit bonus points
					SaveAudit(RowAccount, Server.GetRevaluationComment());
					m_nPointsAwarded += nBonusPoints;
				}
			}
			else													// bonus cash
			{
				double dBonusValue = CalculateRevaluationBonusValue(m_dAddToPurse1Value, m_dRevaluationTrip, m_dRevaluationValue);
				if (dBonusValue != 0.0)
				{
					RowAccount.AddToPurse1Liability(dBonusValue);

					m_atc.SetTransactionPurse1Liability(m_dAddToPurse1Value);	// cash taken
					m_atc.SetTransactionPurse1Credit(0.0);
					m_atc.SetTransactionPurse2Balance(dBonusValue);			// bonus cash added to Purse1
					m_atc.SetTransactionPurse3Liability(0.0);
					m_atc.SetTransactionPurse3Credit(0.0);
					m_atc.SetTransactionPoints(0);
					m_atc.SetTransactionCash(0.0);
					m_atc.SetSourceType(AUDIT_SERVER);
					m_atc.SetApplicationNo(APPNO_REVALUEBONUSCASH);	// audit bonus cash
					SaveAudit(RowAccount,Server.GetRevaluationComment());
				}
			}
		}
	}
}

//*******************************************************************

void CEposSmartPayFile::ProcessPurse3Revaluation( CSQLRowAccountFull& RowAccount )
{
	if (m_dAddToPurse3Value != 0.0)						// see if any cash was added
	{
		RowAccount.AddToPurse3Liability(m_dAddToPurse3Value);			// add any AddtoCard value

		m_atc.SetTransactionPurse1Liability(0.0);
		m_atc.SetTransactionPurse1Credit(0.0);
		m_atc.SetTransactionPurse2Balance(0.0);
		m_atc.SetTransactionPurse3Liability(m_dAddToPurse3Value);
		m_atc.SetTransactionPurse3Credit(0.0);
		m_atc.SetTransactionPoints(0);
		m_atc.SetTransactionCash(0.0);
		m_atc.SetSourceType(m_nTransactionSourceType);	// pos \ kiosk
		m_atc.SetApplicationNo(APPNO_ADDCASH);			// audit cash added
		SaveAudit(RowAccount);

		if (m_dRevaluationTrip2 != 0.0)					// see if any revaluation bonus is required
		{
			if (m_nRevaluationType == 0)					// bonus points
			{
				int nBonusPoints = CalculateRevaluationBonusPoints(m_dAddToPurse3Value, m_dRevaluationTrip2, m_nRevaluationPoints);
				if (nBonusPoints != 0)
				{
					RowAccount.UpdatePoints(nBonusPoints);

					m_atc.SetTransactionPoints(nBonusPoints);			// bonus points
					m_atc.SetTransactionPurse1Liability(0.0);
					m_atc.SetTransactionPurse1Credit(0.0);
					m_atc.SetTransactionPurse2Balance(0.0);
					m_atc.SetTransactionPurse3Liability(m_dAddToPurse3Value);		// cash taken
					m_atc.SetTransactionPurse2Balance(0.0);
					m_atc.SetTransactionCash(0.0);
					m_atc.SetSourceType(AUDIT_SERVER);
					m_atc.SetApplicationNo(APPNO_REVALUEBONUSPOINTS);	// audit bonus points
					SaveAudit(RowAccount,Server.GetRevaluationComment());
					m_nPointsAwarded += nBonusPoints;
				}
			}
			else													// bonus cash
			{
				double dBonusValue = CalculateRevaluationBonusValue(m_dAddToPurse3Value, m_dRevaluationTrip2, m_dRevaluationValue);
				if (dBonusValue != 0.0)
				{
					RowAccount.AddToPurse3Liability(dBonusValue);

					m_atc.SetTransactionPurse1Liability(0);
					m_atc.SetTransactionPurse1Credit(0);
					m_atc.SetTransactionPurse2Balance(dBonusValue);			// bonus cash added to Purse3
					m_atc.SetTransactionPurse3Liability(m_dAddToPurse3Value);	// cash taken
					m_atc.SetTransactionPurse3Credit(0.0);
					m_atc.SetTransactionPoints(0);
					m_atc.SetTransactionCash(0.0);
					m_atc.SetSourceType(AUDIT_SERVER);
					m_atc.SetApplicationNo(APPNO_REVALUEBONUSCASH);	// audit bonus cash
					SaveAudit(RowAccount,Server.GetRevaluationComment());
				}
			}
		}
	}
}

//*******************************************************************

void CEposSmartPayFile::ProcessUnknownPurse(CSQLRowAccountFull& RowAccount, CSQLRowGroupFull& RowGroup, double dUnknownPurseAmount, int nPaymentNo)
{
	if (dUnknownPurseAmount != 0)						// if set old data format was used
	{
		bool bPurse2Allowed = CServerSubs::IsPurseAllowed(2, RowAccount, RowGroup, m_dateTransaction, m_timeTransaction);	// see if Purse2 allowed

		double dPurse2 = (bPurse2Allowed == TRUE) ? RowAccount.GetPurse2Balance() : 0;		// purse2 value that can be used

		if (dPurse2 == 0)	// no purse2
		{
			m_dEposSpendPurse1Total = dUnknownPurseAmount;			// all spend from purse1
		}
		else if (dPurse2 >= dUnknownPurseAmount)
		{
			m_dEposSpendPurse2 = dUnknownPurseAmount;			// all spend from purse2
		}
		else
		{
			m_dEposSpendPurse2 = dPurse2;
			m_dEposSpendPurse1Total = dUnknownPurseAmount - dPurse2;	// spend split between Purse 1 & 2
		}

		LogEposPaymentEntry(nPaymentNo, m_dEposSpendPurse1Total, m_dEposSpendPurse2);
	}
}

//*******************************************************************

void CEposSmartPayFile::ProcessPurse1Spend(CSQLRowAccountFull& RowAccount, CSQLRowGroupFull& RowGroup)
{
	if (m_dEposSpendPurse1Total != 0)
	{
		RowAccount.AllocatePurse1Spend(m_dEposSpendPurse1Total, m_dEposSpendPurse1Liability, m_dEposSpendPurse1Credit);

		bool bClearLastSpend = FALSE;
		CSSDate dateLastUsed(RowAccount.GetPurse1LastSpendDate());	// see if purse1 used today

		if (dateLastUsed != m_dateTransaction)
		{
			bClearLastSpend = TRUE;
		}
		else
		{
			switch (RowGroup.GetMaxSpendType())
			{
			case nGRPSPENDTYPE_PERIOD_LOCKED:
			case nGRPSPENDTYPE_PERIOD_RECORD1:
			case nGRPSPENDTYPE_PERIOD_RECORD2:
				if (SQLRowSetAuditPeriod.GetPeriodIndex(RowAccount.GetPurse1LastSpendTime()) !=
					SQLRowSetAuditPeriod.GetPeriodIndex(m_timeTransaction.GetTime()))
				{
					bClearLastSpend = TRUE;
				}
				break;
			}
		}

		RowAccount.SubtractFromPurse1Liability(m_dEposSpendPurse1Liability);
		RowAccount.SubtractFromPurse1Credit(m_dEposSpendPurse1Credit);
		RowAccount.SetPurse1LastSpendDate(m_dateTransaction.GetDate());
		RowAccount.SetPurse1LastSpendTime(m_timeTransaction.GetTime());

		if (bClearLastSpend == TRUE)
		{
			RowAccount.SetPurse1LastSpend(m_dEposSpendPurse1Total);
		}
		else
		{
			RowAccount.AddToPurse1LastSpend(m_dEposSpendPurse1Total);
		}
		RowAccount.AddToPurse1SpendToDate(m_dEposSpendPurse1Total);
	}
}

//*******************************************************************

void CEposSmartPayFile::ProcessPurse2Spend(CSQLRowAccountFull& RowAccount, CSQLRowGroupFull& RowGroup)
{
	if (m_dEposSpendPurse2 != 0)
	{
		bool bClearLastSpend = FALSE;
		CSSDate dateLastUsed(RowAccount.GetPurse2LastSpendDate());		// see if purse2 used today

		int nPeriodIndex = SQLRowSetAuditPeriod.GetPeriodIndex(m_timeTransaction.GetTime());

		if (dateLastUsed != m_dateTransaction)
		{
			bClearLastSpend = TRUE;
		}
		else
		{
			switch (RowGroup.GetMaxSpendType())
			{
			case nGRPSPENDTYPE_PERIOD_LOCKED:
			case nGRPSPENDTYPE_PERIOD_RECORD1:
			case nGRPSPENDTYPE_PERIOD_RECORD2:
				if (SQLRowSetAuditPeriod.GetPeriodIndex(RowAccount.GetPurse2LastSpendTime()) != nPeriodIndex)
				{
					bClearLastSpend = TRUE;
				}
				break;
			}
		}

		RowAccount.SubtractFromPurse2Balance(m_dEposSpendPurse2);
		RowAccount.SetPurse2LastSpendDate(m_dateTransaction.GetDate());
		RowAccount.SetPurse2LastSpendTime(m_timeTransaction.GetTime());

		if (bClearLastSpend == TRUE)
		{
			RowAccount.SetPurse2LastSpend(m_dEposSpendPurse2);
		}
		else
		{
			RowAccount.AddToPurse2LastSpend(m_dEposSpendPurse2);
		}

		RowAccount.AddToPurse2SpendToDate(m_dEposSpendPurse2);

		CSQLRowPeriodRefresh RowRefresh;
		RowRefresh.SetUserID(RowAccount.GetUserID());
		CSQLRepositoryPeriodRefresh RepoPeriod;
		if (RepoPeriod.SelectRow(RowRefresh, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
		{
			double dBalance = RowRefresh.GetBalance(nPeriodIndex);					// get current balance in refresh database
			RowRefresh.SetBalance(nPeriodIndex, (dBalance - m_dEposSpendPurse2));	// deduct pourse2 spend
			RepoPeriod.UpdateRow(RowRefresh, NULL);
		}
	}
}

//*******************************************************************

void CEposSmartPayFile::ProcessPurse3Spend(CSQLRowAccountFull& RowAccount, CSQLRowGroupFull& RowGroup)
{
	if (m_dEposSpendPurse3Total != 0)
	{
		RowAccount.AllocatePurse3Spend(m_dEposSpendPurse3Total, m_dEposSpendPurse3Liability, m_dEposSpendPurse3Credit);

		bool bClearLastSpend = FALSE;
		CSSDate dateLastUsed(RowAccount.GetPurse3LastSpendDate());	// see if purse3 used today

		if (dateLastUsed != m_dateTransaction)
		{
			bClearLastSpend = TRUE;
		}
		else
		{
			switch (RowGroup.GetMaxSpendType())
			{
			case nGRPSPENDTYPE_PERIOD_LOCKED:
			case nGRPSPENDTYPE_PERIOD_RECORD1:
			case nGRPSPENDTYPE_PERIOD_RECORD2:
				if (SQLRowSetAuditPeriod.GetPeriodIndex(RowAccount.GetPurse3LastSpendTime()) !=
					SQLRowSetAuditPeriod.GetPeriodIndex(m_timeTransaction.GetTime()))
				{
					bClearLastSpend = TRUE;
				}
				break;
			}
		}

		RowAccount.SubtractFromPurse3Liability(m_dEposSpendPurse3Liability);
		RowAccount.SubtractFromPurse3Credit(m_dEposSpendPurse3Credit);
		RowAccount.SetPurse3LastSpendDate(m_dateTransaction.GetDate());
		RowAccount.SetPurse3LastSpendTime(m_timeTransaction.GetTime());

		if (bClearLastSpend == TRUE)
		{
			RowAccount.SetPurse3LastSpend(m_dEposSpendPurse3Total);
		}
		else
		{
			RowAccount.AddToPurse3LastSpend(m_dEposSpendPurse3Total);
		}

		RowAccount.AddToPurse3SpendToDate(m_dEposSpendPurse3Total);
	}
}

//*******************************************************************

void CEposSmartPayFile::AutoRedeemPoints (CSQLRowAccountFull& RowAccount, CRedeemPoints* pPointsRedeemer )
{
	double dValueAdded;
	int nPointsRedeemed = pPointsRedeemer->Redeem ( RowAccount.GetPoints(), dValueAdded );	

	if ( nPointsRedeemed > 0 )
	{
		RowAccount.AddToPoints ( -nPointsRedeemed );
		m_atc.SetTransactionPoints ( -nPointsRedeemed );

		if ( pPointsRedeemer->m_nPurse == 3 )
		{
			RowAccount.AddToPurse3Liability( dValueAdded );						// add to Purse3
			m_atc.SetTransactionPurse1Liability( 0.0 );
			m_atc.SetTransactionPurse1Credit(0.0);
			m_atc.SetTransactionPurse2Balance ( 0.0 );
			m_atc.SetTransactionPurse3Liability( dValueAdded );
			m_atc.SetTransactionPurse3Credit(0.0);
			m_atc.SetTransactionCash( 0.0 );
		}
		else if ( pPointsRedeemer->m_nPurse == 2 ) 
		{
			RowAccount.AddToPurse2Balance ( dValueAdded );						// add to Purse2
			m_atc.SetTransactionPurse1Liability ( 0.0 );
			m_atc.SetTransactionPurse1Credit(0.0);
			m_atc.SetTransactionPurse2Balance( dValueAdded );
			m_atc.SetTransactionPurse3Liability( 0.0 );
			m_atc.SetTransactionPurse3Credit(0.0);
			m_atc.SetTransactionCash( 0.0 );
		}
		else
		{
			RowAccount.AddToPurse1Liability ( dValueAdded );						// add to Purse1
			m_atc.SetTransactionPurse1Liability ( dValueAdded );
			m_atc.SetTransactionPurse1Credit(0.0);
			m_atc.SetTransactionPurse2Balance( 0.0 );
			m_atc.SetTransactionPurse3Liability ( 0.0 );
			m_atc.SetTransactionPurse3Credit(0.0);
			m_atc.SetTransactionCash( 0.0 );
		}

		m_atc.SetSourceType (AUDIT_SERVER);
		m_atc.SetApplicationNo ( APPNO_REDEEM );					// points redemption
		SaveAudit (RowAccount,Server.GetRedeemComment() );
	}
}

//*******************************************************************

void CEposSmartPayFile::SaveAudit(CSQLRowAccountFull& RowAccount, const char* szComment)
{
	m_atc.SetCurrentPoints(RowAccount.GetPoints());
	m_atc.SetCurrentPurse1Liability(RowAccount.GetPurse1Liability());
	m_atc.SetCurrentPurse1Credit(RowAccount.GetPurse1Credit());
	m_atc.SetCurrentPurse2Balance(RowAccount.GetPurse2Balance());
	m_atc.SetCurrentPurse3Liability(RowAccount.GetPurse3Liability());
	m_atc.SetCurrentPurse3Credit(RowAccount.GetPurse3Credit());
	m_atc.SetToDatePoints(RowAccount.GetPointsToDate());
	m_atc.SetToDateCashSpend(RowAccount.GetCashSpendToDate());
	m_atc.SetToDatePurse1Spend(RowAccount.GetPurse1SpendToDate());
	m_atc.SetToDatePurse2Spend(RowAccount.GetPurse2SpendToDate());
	m_atc.SetToDatePurse3Spend(RowAccount.GetPurse3SpendToDate());
	m_atc.SetLineComment(szComment);

	m_atc.BufferNewLine();

	if ((TRUE == m_bExternalMode) && (0 == m_nExternalCCNo))
	{
		m_nExternalCCNo = m_atc.GetCCNoFull();
	}
}

//******************************************************************

int CEposSmartPayFile::GetBonusMultiplier ( double dSpendValue )
{
	int nFactor = 1;
	CSSTime timeStart;
	CSSTime timeEnd;

	if (m_timeTransaction.IsSet() == TRUE)		// get day of transaction
	{
		switch (m_dateTransaction.GetDayOfWeek())	// 1=sunday 2=monday..7=saturday, 0-not set
		{
		case 0:
			break;

		case 1:
			timeStart = Server.GetSunStart();
			timeEnd = Server.GetSunEnd();
			nFactor = Server.GetRewardMultiplierFactorSun();
			break;

		case 2:
			timeStart = Server.GetMonStart();
			timeEnd = Server.GetMonEnd();
			nFactor = Server.GetRewardMultiplierFactorMon();
			break;

		case 3:
			timeStart = Server.GetTueStart();
			timeEnd = Server.GetTueEnd();
			nFactor = Server.GetRewardMultiplierFactorTue();
			break;

		case 4:
			timeStart = Server.GetWedStart();
			timeEnd = Server.GetWedEnd();
			nFactor = Server.GetRewardMultiplierFactorWed();
			break;

		case 5:
			timeStart = Server.GetThuStart();
			timeEnd = Server.GetThuEnd();
			nFactor = Server.GetRewardMultiplierFactorThu();
			break;

		case 6:
			timeStart = Server.GetFriStart();
			timeEnd = Server.GetFriEnd();
			nFactor = Server.GetRewardMultiplierFactorFri();
			break;

		case 7:
			timeStart = Server.GetSatStart();
			timeEnd = Server.GetSatEnd();
			nFactor = Server.GetRewardMultiplierFactorSat();
			break;
		}
	}

	if (nFactor == 1)								// see if bonus multiplier set or date / time not set
	{
		return 1;									// not set
	}

	if (m_timeTransaction >= timeStart && m_timeTransaction <= timeEnd)	// see if transaction time is in range 
	{
		if (dSpendValue >= m_dRewardMultiplierTrigger)				// see if spend triggers multifier
		{
			return nFactor;												// use server multiplier factor
		}
	}
	return 1;
}

//******************************************************************
// Save payment lines to temp payment audit file

void CEposSmartPayFile::LogEposPaymentEntry(int nType, double dValue1, double dValue2, double dValue3)
{
	CPaymentAuditRecord paymentRecord;

	paymentRecord.SetDateLastUsed(m_dateTransaction.GetDate());
	paymentRecord.SetTimeLastUsed(m_timeTransaction.GetTime());
	paymentRecord.SetUserID(m_strEposUserID);
	paymentRecord.SetOperatorID(m_strServerID);
	paymentRecord.SetFolderSet(m_nFolderSet);
	paymentRecord.SetTerminalNo(m_nTillNo);
	paymentRecord.SetTransactionNo(m_strTransactionNo);
	paymentRecord.SetPaymentType(nType);
	paymentRecord.SetPaymentValue1(dValue1);
	paymentRecord.SetPaymentValue2(dValue2);
	paymentRecord.SetPaymentValue3(dValue3);
	paymentRecord.SetExternalMode(m_bExternalMode);
	paymentRecord.SaveEntry(m_strTmpPaymentAuditFile);

	if (TRUE == m_PendingExportPaymentData.m_bChartwell)
	{
		switch (nType)
		{
		case 20:
		case 21:
			m_PendingExportPaymentData.m_PaymentDataChartwell.ConsolidatePayment(dValue1, dValue2, dValue3);
			break;
		}
	}
}

//******************************************************************
// Save temp payment audit lines to main payment auditt file

void CEposSmartPayFile::SaveEposPaymentAudit()
{
	if (::FileExists(m_strTmpPaymentAuditFile) == TRUE)
	{
		if (::AppendDataFile(m_strTmpPaymentAuditFile, Filenames.GetPaymentAuditFilename()) == TRUE)
		{
			CFileRecycler::SendToRecycleBin(m_strTmpPaymentAuditFile);
		}
	}
}

//*******************************************************************

int CEposSmartPayFile::CalculateRevaluationBonusPoints( double dValueAdded, double dTripValue, int nPointsPerTrip )
{
	int nPoints = 0;

	int nFactor = (int)(dValueAdded / dTripValue);
	nPoints = nPointsPerTrip * nFactor;

	return nPoints;
}

//*******************************************************************

double CEposSmartPayFile::CalculateRevaluationBonusValue(double dValueAdded, double dTripValue, double dValuePerTrip)
{
	double dValue = 0.0;

	int nFactor = (int)(dValueAdded / dTripValue);		// round it
	dValue = dValuePerTrip * (double)nFactor;

	return dValue;
}

//*******************************************************************

void CEposSmartPayFile::ProcessRewardSpendBonus(CSQLRowAccountFull& RowAccount)
{
	if (m_dRewardSpendTrip > 0.0)								// see if Reward bonus setup
	{
		double dSpendValue = 0;
		if (Server.GetRewardIncludePurse1SpendFlag() == TRUE)	dSpendValue += m_dEposSpendPurse1Total;
		if (Server.GetRewardIncludePurse2SpendFlag() == TRUE)	dSpendValue += m_dEposSpendPurse2;
		if (Server.GetRewardIncludePurse3SpendFlag() == TRUE)	dSpendValue += m_dEposSpendPurse3Total;
		if (Server.GetRewardIncludeCashSpendFlag() == TRUE)		dSpendValue += m_dCashSpend;

		if (m_bHaveRewardUpperSpendLimit == TRUE)				// see if need to check single transaction upper limit
		{
			if (dSpendValue >= m_dRewardUpperSpendLimit)			// check for max spend per transaction
			{
				dSpendValue = m_dRewardUpperSpendLimit;
			}
		}

		int nFactor = (int)(dSpendValue / m_dRewardSpendTrip);
		int nMultiplier = GetBonusMultiplier(dSpendValue);

		if (m_nRewardType == 0)									// bonus points
		{
			int nBonusPoints = (m_nRewardPoints * nFactor) * nMultiplier;
			if (nBonusPoints != 0)
			{
				RowAccount.UpdatePoints(nBonusPoints);

				m_atc.SetTransactionPurse1Liability(dSpendValue);			// spend that generated the bonus cash
				m_atc.SetTransactionPurse1Credit(0.0);
				m_atc.SetTransactionPurse2Balance(0);
				m_atc.SetTransactionPurse3Liability(0.0);
				m_atc.SetTransactionPurse3Credit(0.0);
				m_atc.SetTransactionPoints(nBonusPoints);
				m_atc.SetTransactionCash(0.0);
				m_atc.SetSourceType(AUDIT_SERVER);

				int nAppNo = (m_nTransactionAppNo == APPNO_REFUNDSALE) ? APPNO_REFUNDREWARDBONUSPOINTS : APPNO_REWARDBONUSPOINTS;

				m_atc.SetApplicationNo(nAppNo);	// audit bonus points
				SaveAudit(RowAccount,Server.GetRewardComment());
			}
		}
		else															// bonus cash
		{
			if ((m_dRewardValue != 0.0) && (nFactor != 0) && (nMultiplier != 0))
			{
				double dBonusValue = m_dRewardValue;
				dBonusValue *= nFactor;
				dBonusValue *= nMultiplier;

				RowAccount.AddToPurse1Liability(dBonusValue);

				m_atc.SetTransactionPurse1Liability(dSpendValue);	// spend that generated the bonus cash
				m_atc.SetTransactionPurse1Credit(0.0);
				m_atc.SetTransactionPurse2Balance(dBonusValue);		// cash added to Purse1
				m_atc.SetTransactionPurse3Liability(0.0);
				m_atc.SetTransactionPurse3Credit(0.0);
				m_atc.SetTransactionPoints(0);
				m_atc.SetTransactionCash(0.0);
				m_atc.SetSourceType(AUDIT_SERVER);

				int nAppNo = (m_nTransactionAppNo == APPNO_REFUNDSALE) ? APPNO_REFUNDREWARDBONUSCASH : APPNO_REWARDBONUSCASH;

				m_atc.SetApplicationNo(nAppNo);						// audit bonus cash
				SaveAudit(RowAccount,Server.GetRewardComment());
			}
		}
	}
}

//*******************************************************************

void CEposSmartPayFile::ProcessPurchaseHistory(CSQLRowAccountFull& RowAccount)
{
	if ((m_nTransactionItemCount > 0) || (TRUE == m_bExternalMode))
	{
		if (Server.GetActiveEnablePurchaseHistoryFlag() == TRUE)	// see if purchase history required
		{
			if (m_TLogRecord.GetItemCounter() > 0)			// see if have anything
			{
				m_TLogRecord.SetDate(m_dateTransaction.GetDate());
				m_TLogRecord.SetTime(m_timeTransaction.GetTime());
				m_TLogRecord.SetFolderSet(m_nFolderSet);
				m_TLogRecord.SetTerminalNo(m_nTillNo);
				m_TLogRecord.SetCCNo(m_nTransactionCCNo);		// CCNo used when epos sale saved in audit file
				m_TLogRecord.SetReceiptNo(m_strTransactionNo);
				m_TLogRecord.AddToPurse1Spend(m_dEposSpendPurse1Total);
				m_TLogRecord.AddToPurse2Spend(m_dEposSpendPurse2);
				m_TLogRecord.AddToPurse3Spend(m_dEposSpendPurse3Total);
				m_TLogRecord.SetPurse1(RowAccount.GetPurse1Balance());
				m_TLogRecord.SetPurse2(RowAccount.GetPurse2Balance());
				m_TLogRecord.SetPurse3(RowAccount.GetPurse3Balance());
				SavePurchaseDetails(RowAccount.GetUserIDString());
			}
		}
	}
}

//*******************************************************************

void CEposSmartPayFile::SavePurchaseDetails(const char* szUserID)
{
	bool bAllowHistory = TRUE;

	if (Server.GetDisablePurchaseHistoryAccountZeroFlag() == TRUE)
	{
		CString strTest = szUserID;
		TrimSpacesFromString(strTest, TRUE);
		bAllowHistory = (strTest != "");
	}

	if (TRUE == bAllowHistory)
	{
		CSSFile file;
		if (file.Open(Filenames.GetPurchaseHistoryUpdateFilename(szUserID), "ab") == TRUE)
		{
			m_TLogRecord.WriteLine(&file);				// add to user id's purchase history 
			file.Close();
		}
	}
}

//*******************************************************************

void CEposSmartPayFile::WritePendingAuditLines()
{
	m_atc.WritePendingLines();
}

//*******************************************************************
