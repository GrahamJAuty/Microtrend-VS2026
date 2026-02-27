//**********************************************************************
#include "..\SmartPay4Shared\AuditReportTexts.h"
//**********************************************************************
#include "TLogFile.h"
#include "TLogDatabase.h"
//**********************************************************************
#include "SQLAuditRecord.h"
//**********************************************************************

CSQLAuditRecord::CSQLAuditRecord() : CSQLAuditRecord_base(), m_dSales{}
{
	m_dNotFoundSales = 0.0;
	m_dSalesUnknown = 0.0;
}

//**********************************************************************

CString CSQLAuditRecord::ExtractPurchaseText(CTLogDatabase* pDatabase, bool bNewDatabase)
{
	CString strText = "Ecr sale";

	if (pDatabase != NULL)
	{
		if (TRUE == bNewDatabase)
		{
			pDatabase->Open(GetHistoryFilename());
		}

		if (pDatabase->FindSalesText(GetDateLastUsed(), GetTimeLastUsed(), GetCCNoFull()) == TRUE)
		{
			strText = pDatabase->GetSalesText();			// extract purchase text
		}
	}

	return strText;
}

//**********************************************************************

CString CSQLAuditRecord::GetHistoryFilename()
{
	CString strUserID = GetUserID();
	return GetHistoryFilenameStatic(strUserID);
}

//**********************************************************************

CString CSQLAuditRecord::GetHistoryFilenameStatic( CString strUserID )
{
	CString strFilename = "";

	if ( strUserID == "" || strUserID == System.GetBlankCardNo() )			// see if report is for the cash account "000000.dat"
	{
		CString strTmp = Filenames.GetTempCashHistoryFilename();			// see if have a temp CASH tlog in use instead of actual cash history file
		if (::FileExists(strTmp) == TRUE)
		{
			// use temp file in favour of real history file ( outstanding updates already merged in 
			strFilename = strTmp;
		}	
	}

	if ( strFilename == "" )												// no file bame been set, so use the audit userid
	{
		CTLogFile tlogFile ( strUserID );									// will merge in any outstanding updates
		strFilename = Filenames.GetPurchaseHistoryFilename ( strUserID );
	}

	return strFilename;
}

//**********************************************************************
// extract sales by taxcode from Tlog purchase record
// any not found purchase records added to given filename

bool CSQLAuditRecord::ExtractSalesTaxes (  const char* szFilename )
{
	bool bReply = FALSE;

	m_dNotFoundSales = 0;
	for (int i = 0; i < nMAX_TAXCODES; i++)
	{
		m_dSales[i] = 0;
	}

	CTLogDatabase dbTlogDatabase;
	if ( dbTlogDatabase.Open ( GetHistoryFilename() ) == DB_ERR_NONE )
	{
		if ( ( bReply = dbTlogDatabase.FindSalesTaxes ( GetDateLastUsed(), GetTimeLastUsed(), GetCCNoFull() ) ) == TRUE )
		{
			for (int i = 0; i < nMAX_TAXCODES; i++)
			{
				m_dSales[i] = dbTlogDatabase.m_dSales[i];
			}

			m_dSalesUnknown = dbTlogDatabase.m_dSalesUnknown;			// sales with no VAT code ( eg:- Discount ) 
		}
//*********
		else
		{
			CString strFilename = szFilename;
			if ( strFilename != "" )
			{
				bool bSaveHeaderReqd = FALSE;
				if (::GetFileLineCount(strFilename) == 0)
				{
					bSaveHeaderReqd = TRUE;
				}

				CSSFile file;
				if (file.Open(strFilename, "ab") == TRUE)
				{
					if (bSaveHeaderReqd == TRUE)
					{
						CCSV csvHeader;
						csvHeader.Add("Date");
						csvHeader.Add("Time");
						csvHeader.Add("Source");
						csvHeader.Add("CCNo");
						csvHeader.Add("UserID");
						csvHeader.Add("Name");
						csvHeader.Add("Purse1");
						csvHeader.Add("Purse2");
						csvHeader.Add("Purse3");
						csvHeader.Add("Cash");
						file.Write(csvHeader);
					}

					CCSV csv;
					csv.Add(m_dateLastUsed.GetDate());			// date
					csv.Add(m_timeLastUsed.GetTime());			// time
					csv.Add(CAuditReportTexts::GetReportSourceText(m_nSourceType, m_nApplicationNo, m_nTerminalNo, m_strAuditLineExtraText));
					csv.Add(GetCCNoFull());
					csv.Add(System.FormatCardNo(m_strUserID));	// user  no
					csv.Add(m_strUserName);						// user name
					csv.Add(m_dTransactionPurse1Liability + m_dTransactionPurse1Credit, 2);			// purse added
					csv.Add(m_dTransactionPurse2Balance, 2);			// transaction total
					csv.Add(m_dTransactionPurse3Liability + m_dTransactionPurse3Credit, 2);			// transaction total
					csv.Add(m_dTransactionCash, 2);				// cash spend
					file.Write(csv);
					file.Close();

					m_dNotFoundSales = -(m_dTransactionPurse1Liability + m_dTransactionPurse1Credit + m_dTransactionPurse2Balance + m_dTransactionPurse3Liability + m_dTransactionPurse3Credit + m_dTransactionCash);	// -ve value = sale 
				}
			}
		}
	}

	return bReply;
}

//**********************************************************************

bool CSQLAuditRecord::IsDeleteAppNo()
{
	switch (m_nApplicationNo)
	{
	case APPNO_DELETE:
	case APPNO_BATCHDELETE:
	case APPNO_IMPORTDELETE:
	case APPNO_DELETELEAVER:
	case APPNO_BATCHLEAVER:
	case APPNO_IMPORTLEAVER:
		return TRUE;

	default:
		return FALSE;
	}
}

//**********************************************************************

bool CSQLAuditRecord::IsWebPaymentAppNo()
{
	bool bResult = FALSE;

	switch (m_nApplicationNo)
	{
	case APPNO_PARENTPAY:
	case APPNO_WISEPAY:
	case APPNO_TUCASI:
	case APPNO_TUCASIv2:
	case APPNO_SCHOOLCOMMS:
	case APPNO_SQUID:
	case APPNO_PARENTMAIL:
	case APPNO_BROMCOM:
	case APPNO_PEBBLE_TOPUP:
	case APPNO_PEBBLE_TOPUP_REFUND:
	case APPNO_PEBBLE_ORDER:
	case APPNO_PEBBLE_ORDER_REFUND:
		bResult = TRUE;
		break;
	}

	return bResult;
}

//**********************************************************************

bool CSQLAuditRecord::IsEODEntry()
{
	return ( m_nSourceType == AUDIT_EODBALANCE ) ? TRUE : FALSE;
}

//**********************************************************************
