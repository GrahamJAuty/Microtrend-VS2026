// DbReportHandler.cpp: implementation of the CDbReportHandler class.
//
// Derived from CDbExportHandler -  ( CDbExportHandler see for list of report labels )
//
//////////////////////////////////////////////////////////////////////

//*******************************************************************
#include "..\SPosLoyalty4Shared\LoyaltyHeaderHelpers.h"
#include "..\SPosLoyalty4Shared\LoyaltySubs.h"
#include "..\SPosLoyalty4Shared\SQLNamespaces.h"
#include "..\SPosLoyalty4Shared\SQLTable_Group\SQLRepositoryGroup.h"
#include "..\SPosLoyalty4Shared\UserTexts.h"
//*******************************************************************
#include "GroupComboHelpers.h"
#include "LoyaltyManager.h"
//*******************************************************************
#include "DbReportHandler.h"
//*******************************************************************

CDbReportHandler::CDbReportHandler()
{
	m_Text.Read();
	ClearTotals();
}

//*******************************************************************

void CDbReportHandler::ClearTotals(int nIndex)
{
	if (nIndex == -1)
	{
		ClearTotals(nREPORT_GROUP);
		ClearTotals(nREPORT_TOTAL);
		return;
	}

	m_nTotalPoints[nIndex] = 0;
	m_nTotalPointsTD[nIndex] = 0;

	m_dTotalPurse1[nIndex] = 0.0;
	m_dTotalPurse2[nIndex] = 0.0;
	m_dTotalCashSpendTD[nIndex] = 0.0;;
	m_dTotalPurse1SpendTD[nIndex] = 0.0;
	m_dTotalPurse2SpendTD[nIndex] = 0.0;
	m_dTotalSpendTD[nIndex] = 0.0;
}

//*******************************************************************

bool CDbReportHandler::AddHotlistIndicator()
{
	if (GetHotlistedReqd() == FALSE)					// see if including Hotliste
	{
		return FALSE;									// no 
	}

	if ( m_strLabel == "R001" )	return FALSE;			// don't need on hotlist report
	if ( m_strLabel == "CSV" )	return FALSE;			// don't need on CSV export
	if ( m_strLabel == "DEL" )	return FALSE;			// don't need on CSV export
	if ( m_strLabel == "DB1" )	return FALSE;			// don't need on CSV export
	if ( m_strLabel == "DB2" )	return FALSE;			// don't need on CSV export
	if ( m_strLabel == "DB3" )	return FALSE;			// don't need on CSV export

	return TRUE;
}

//*******************************************************************

void CDbReportHandler::AddColumnCardNo ( CReportFile* pReport, bool bIndicateHotlist )
{
	pReport->AddColumn ( Account::UserID.Label, TA_LEFT, 162 );

	if (TRUE == bIndicateHotlist)
	{
		if (AddHotlistIndicator() == TRUE)
		{
			pReport->AddColumn("H", TA_CENTER, 63);
		}
	}
}

//*******************************************************************

void CDbReportHandler::AddColumnCardName ( CReportFile* pReport )
{
	pReport->AddColumn ( "Name", TA_LEFT, 357 );
}

//*******************************************************************
// always adds GRP column

void CDbReportHandler::AddOrderByColumnText(CReportFile* pReport)
{
	bool bAddCardNo = TRUE;

	switch (GetSortOn())
	{
	case nSORTONCARDNO:		
		AddColumnCardNo(pReport, TRUE);
		AddColumnCardName(pReport);
		bAddCardNo = FALSE;							
		break;

	case nSORTONCARDNAME:	
		AddColumnCardName(pReport);								
		break;

	case nSORTONSURNAME:	
		pReport->AddColumn(Account::Surname.Label, TA_LEFT, 190);	
		break;

	case nSORTONINFO1:		
		pReport->AddColumn(m_Text.GetText(LOYALTYFIELD_INFO1, ""), TA_LEFT, 190);	
		break;

	case nSORTONINFO2:	
		pReport->AddColumn(m_Text.GetText(LOYALTYFIELD_INFO2, ""), TA_LEFT, 190);	
		break;

	case nSORTONINFO3:	
		pReport->AddColumn(m_Text.GetText(LOYALTYFIELD_INFO3, ""), TA_LEFT, 190);	
		break;

	case nSORTONINFO4:	
		pReport->AddColumn(m_Text.GetText(LOYALTYFIELD_INFO4, ""), TA_LEFT, 190);	
		break;

	case nSORTONADDRESS1:	
		pReport->AddColumn(m_Text.GetText(LOYALTYFIELD_ADDRESS1, ""), TA_LEFT, 190);
		break;

	case nSORTONADDRESS2:	
		pReport->AddColumn(m_Text.GetText(LOYALTYFIELD_ADDRESS2, ""), TA_LEFT, 190);
		break;

	case nSORTONADDRESS3:
		pReport->AddColumn(m_Text.GetText(LOYALTYFIELD_ADDRESS3, ""), TA_LEFT, 190);
		break;

	case nSORTONADDRESS4:	
		pReport->AddColumn(m_Text.GetText(LOYALTYFIELD_ADDRESS4, ""), TA_LEFT, 190);
		break;

	case nSORTONADDRESS5:	
		pReport->AddColumn(m_Text.GetText(LOYALTYFIELD_ADDRESS5, ""), TA_LEFT, 190);
		break;

	case nSORTONEXTREF:		
		pReport->AddColumn(m_Text.GetText(LOYALTYFIELD_EXTERNALREF, ""), TA_LEFT, 190);	
		break;
	}

	if (bAddCardNo == TRUE)
	{
		AddColumnCardNo(pReport, TRUE);
	}

	pReport->AddColumn("Grp", TA_RIGHT, 93);
}

//**********************************************************************

void CDbReportHandler::AddCardNo ( CCSV* pCsv, CSQLRowAccountFull& RowAccount, bool bExists, bool bIndicateHotlist )
{
	CString strCardNo = System.FormatCardNo(RowAccount.GetUserIDString());
	
	if (bExists == FALSE)
	{
		strCardNo += " *";
	}

	pCsv->Add ( strCardNo );

	if ( TRUE == bIndicateHotlist )
	{
		if ( AddHotlistIndicator() == TRUE )
		{
			bool bHotlisted = FALSE;
			if (TRUE == bExists)
			{
				bHotlisted = RowAccount.IsCardHotlisted();
			}

			if (bHotlisted == TRUE)
			{
				pCsv->Add("X");
			}
			else
			{
				pCsv->Add("");
			}
		}
	}
}

//*******************************************************************
// used when record not in database

void CDbReportHandler::AddOrderByData ( CCSV* pCsv, CString strCardName, __int64 nUserID )
{	
	if (strCardName == "")
	{
		strCardName = "unknown";
	}
	
	CSQLRowAccountFull RowAccount;
	RowAccount.SetUserID(nUserID);

	switch ( GetSortOn() )
	{
	case nSORTONCARDNO:		
		AddCardNo ( pCsv, RowAccount, FALSE, TRUE );
		pCsv->Add ( strCardName );
		break;

	case nSORTONCARDNAME:	
		pCsv->Add ( strCardName );
		AddCardNo ( pCsv, RowAccount, FALSE, TRUE );
		break;
	
	default:
		pCsv->Add ("");
		AddCardNo ( pCsv, RowAccount, FALSE, TRUE );
		break;
	}
	
	// default Group
	pCsv->Add(1);											
}

//*******************************************************************

void CDbReportHandler::AddOrderByData ( CCSV* pCsv, CSQLRowAccountFull& RowAccount )
{
	bool bAddCardNo = TRUE;

	switch ( GetSortOn() )
	{
	case nSORTONCARDNO:		
		AddCardNo ( pCsv, RowAccount, TRUE, TRUE);			// card no + hotlist
		pCsv->Add (RowAccount.GetFullname() );
		bAddCardNo = FALSE;
		break;

	case nSORTONCARDNAME:	
		pCsv->Add (RowAccount.GetFullname() );		
		break;

	case nSORTONSURNAME:	
		pCsv->Add (RowAccount.GetSurname() );		
		break;

	case nSORTONINFO1:		
		pCsv->Add (RowAccount.GetInfo1() );	
		break;

	case nSORTONINFO2:		
		pCsv->Add (RowAccount.GetInfo2() );	
		break;

	case nSORTONINFO3:		
		pCsv->Add (RowAccount.GetInfo3() );	
		break;

	case nSORTONINFO4:		
		pCsv->Add (RowAccount.GetInfo4() );	
		break;

	case nSORTONADDRESS1:
		pCsv->Add (RowAccount.GetAddress1() );	
		break;

	case nSORTONADDRESS2:
		pCsv->Add (RowAccount.GetAddress2() );
		break;

	case nSORTONADDRESS3:	
		pCsv->Add (RowAccount.GetAddress3() );
		break;

	case nSORTONADDRESS4:	
		pCsv->Add (RowAccount.GetAddress4() );			
		break;

	case nSORTONADDRESS5:	
		pCsv->Add (RowAccount.GetAddress5() );			
		break;

	case nSORTONEXTREF:		
		pCsv->Add (RowAccount.GetExternalRef() );		
		break;
	}

	if (bAddCardNo == TRUE)
	{
		AddCardNo(pCsv, RowAccount, TRUE, TRUE);
	}

	pCsv->Add (RowAccount.GetGroupNo() );
}

//*******************************************************************

void CDbReportHandler::AddOrderByTotalText(const char* szText, CCSV* pCsv, CCSV* pCsvTabLine, int nGroupCount)
{
	pCsv->Add(szText);							// name
	pCsvTabLine->Add("");

	pCsv->Add("");								// card no
	pCsvTabLine->Add("");

	if (AddHotlistIndicator() == TRUE)
	{
		pCsv->Add("");							// H column
		pCsvTabLine->Add("");
	}

	if (nGroupCount != 0)
	{
		pCsv->Add(nGroupCount);						// group total
		pCsvTabLine->Add("<LI>");
	}
	else
	{
		pCsv->Add("");						
		pCsvTabLine->Add("");
	}
}

//*******************************************************************
// szText can be additiona text to include in report title before database name

CString CDbReportHandler::GetReportTitle ( const char* szText )
{
	CString strMsg = "";
	CString strReportTitle = "";

	if (m_strLabel.Left(2) == "UR")
	{
		strMsg = m_strFilename;		//"Custom Database Report" text;
	}
	else
	{
		strMsg = m_strTitle;		// standard report title;
	}

	strReportTitle.Format ( "%s%s", 
		(const char*) strMsg,  
		szText );

	return strReportTitle;
}

//*******************************************************************
// get filename to hold report --p16\temp01\Rnnn.rep

CString CDbReportHandler::GetReportFilename()
{
	CString strFilename	= Filenames.GetReportFilename ( m_strLabel );
	return strFilename;
}

//*******************************************************************

// get filename to hold report parameters ..p16\cardnnn\reports\Rnnn.prm

CString CDbReportHandler::GetParamsFilename()
{
	CString strFilename	= Filenames.GetReportParamsFilename ( m_strLabel );
	return strFilename;
}

//*******************************************************************

// create report key depending on filters used

CString CDbReportHandler::GetReportKey()
{
	CString strReportKey = "";
	strReportKey.Format ( "%s%s", 
		(const char*) m_strLabel, 
		(const char*) m_csvFilters.GetLine() );		//* use SortOn flag as report key

	return strReportKey;
}

//*******************************************************************
//*******************************************************************
//*******************************************************************
// add additional fields

void CDbReportHandler::CreateReportHeader ( CReportFile* pReport )
{
	CUserTexts texts;
	texts.Read();

	CArray<int, int> arrayHeader;	
	LoyaltyHeaderHelpers.CreateNumericHeader(m_strHeader, arrayHeader);
	
	for ( int i = 0 ; i < arrayHeader.GetSize() ; i++ )
	{
		switch (arrayHeader.GetAt(i))
		{
		case LOYALTYFIELD_USERID:
			AddColumnCardNo(pReport, FALSE);						
			break;

		case LOYALTYFIELD_EXTERNALREF:
			pReport->AddColumn(texts.GetText(LOYALTYFIELD_EXTERNALREF, ""), TA_LEFT, 357);
			break;

		case LOYALTYFIELD_QRINFO:
			pReport->AddColumn(Account::QRInfo.Label, TA_LEFT, 357);
			break;

		case LOYALTYFIELD_FULLNAME:
			pReport->AddColumn("Name", TA_LEFT, 357);
			break;

		case LOYALTYFIELD_FORENAME:
			pReport->AddColumn(Account::Forename.Label, TA_LEFT, 357);
			break;

		case LOYALTYFIELD_SURNAME:
			pReport->AddColumn(Account::Surname.Label, TA_LEFT, 357);
			break;

		case LOYALTYFIELD_GROUPNO:
			pReport->AddColumn("Grp", TA_RIGHT, 93);
			break;

		case LOYALTYFIELD_SCHEME:
			pReport->AddColumn("Schm", TA_RIGHT, 150);
			break;

		case LOYALTYFIELD_GENDER:
			pReport->AddColumn("Sx", TA_RIGHT, 90);
			break;

		case LOYALTYFIELD_POINTS:
			pReport->AddColumn("Points", TA_RIGHT, 180);
			break;

		case LOYALTYFIELD_POINTSTODATE:
			pReport->AddColumn("PointsTD", TA_RIGHT, 189);
			break;

		case LOYALTYFIELD_PURSE1_BALANCE:
			pReport->AddColumn("Purse1", TA_RIGHT, 195);
			break;

		case LOYALTYFIELD_PURSE1_LASTSPENDDATE:
			pReport->AddColumn("P1.Date", TA_LEFT, 250);
			break;

		case LOYALTYFIELD_PURSE1_LASTSPENDTIME:
			pReport->AddColumn("P1.Time", TA_LEFT, 250);
			break;

		case LOYALTYFIELD_PURSE1_LASTSPEND:
			pReport->AddColumn("P1.Spend", TA_RIGHT, 250);
			break;

		case LOYALTYFIELD_PURSE1_SPENDTODATE:
			pReport->AddColumn("P1.STD", TA_RIGHT, 250);
			break;

		case LOYALTYFIELD_PURSE2_BALANCE:
			pReport->AddColumn("Purse2", TA_RIGHT, 195);
			break;

		case LOYALTYFIELD_PURSE2_LASTSPENDDATE:
			pReport->AddColumn("P2.Date", TA_LEFT, 250);
			break;

		case LOYALTYFIELD_PURSE2_LASTSPENDTIME:
			pReport->AddColumn("P2.Time", TA_LEFT, 250);
			break;

		case LOYALTYFIELD_PURSE2_LASTSPEND:
			pReport->AddColumn("P2.Spend", TA_RIGHT, 250);
			break;

		case LOYALTYFIELD_PURSE2_REFRESHEDDATE:
			pReport->AddColumn("P2.RFDate", TA_LEFT, 250);
			break;

		case LOYALTYFIELD_PURSE2_REFRESHEDTIME:
			pReport->AddColumn("P2.RFTime", TA_LEFT, 250);
			break;

		case LOYALTYFIELD_PURSE2_SPENDTODATE:
			pReport->AddColumn("P2.STD", TA_RIGHT, 250);
			break;

		case LOYALTYFIELD_CASH_LASTSPENDDATE:
			pReport->AddColumn("Cash.Date", TA_LEFT, 250);
			break;

		case LOYALTYFIELD_CASH_LASTSPENDTIME:
			pReport->AddColumn("Cash.Time", TA_LEFT, 250);
			break;

		case LOYALTYFIELD_CASH_LASTSPEND:
			pReport->AddColumn("Cash.Spend", TA_RIGHT, 250);
			break;

		case LOYALTYFIELD_CASH_SPENDTODATE:
			pReport->AddColumn("Cash.STD", TA_RIGHT, 250);
			break;

		case LOYALTYFIELD_MAXSPEND:
			pReport->AddColumn("MaxSpend", TA_RIGHT, 220);
			break;

		case LOYALTYFIELD_MAXOVERDRAFT:
			pReport->AddColumn("OverSpend", TA_RIGHT, 220);
			break;

		case LOYALTYFIELD_TOTAL_SPENDTODATE:
			pReport->AddColumn("Total.STD", TA_RIGHT, 220);
			break;

		case LOYALTYFIELD_EXPIRYDATE:
			pReport->AddColumn("Expiry", TA_LEFT, 219);
			break;

		case LOYALTYFIELD_HOTLISTDATE:
			pReport->AddColumn("H.Date", TA_LEFT, 219);
			break;

		case LOYALTYFIELD_HOTLISTTIME:
			pReport->AddColumn("H.Time", TA_LEFT, 156);
			break;

		case LOYALTYFIELD_HOTLISTCODE:
			pReport->AddColumn("H.Code", TA_LEFT, 219);
			break;

		case LOYALTYFIELD_PREVUSERID:
			pReport->AddColumn("Prev.No", TA_LEFT, 162);
			break;

		case LOYALTYFIELD_NEXTUSERID:
			pReport->AddColumn("Next.No", TA_LEFT, 162);
			break;

		case LOYALTYFIELD_ADDRESS1:
			pReport->AddColumn(texts.GetText(LOYALTYFIELD_ADDRESS1, Account::Address1.Label), TA_LEFT, 300);
			break;

		case LOYALTYFIELD_ADDRESS2:
			pReport->AddColumn(texts.GetText(LOYALTYFIELD_ADDRESS2, Account::Address2.Label), TA_LEFT, 300);
			break;

		case LOYALTYFIELD_ADDRESS3:
			pReport->AddColumn(texts.GetText(LOYALTYFIELD_ADDRESS3, Account::Address3.Label), TA_LEFT, 300);
			break;

		case LOYALTYFIELD_ADDRESS4:
			pReport->AddColumn(texts.GetText(LOYALTYFIELD_ADDRESS4, Account::Address4.Label), TA_LEFT, 300);
			break;

		case LOYALTYFIELD_ADDRESS5:
			pReport->AddColumn(texts.GetText(LOYALTYFIELD_ADDRESS5, Account::Address5.Label), TA_LEFT, 300);
			break;

		case LOYALTYFIELD_INFO1:
			pReport->AddColumn(texts.GetText(LOYALTYFIELD_INFO1, Account::Info1.Label), TA_LEFT, 300);
			break;

		case LOYALTYFIELD_INFO2:
			pReport->AddColumn(texts.GetText(LOYALTYFIELD_INFO2, Account::Info2.Label), TA_LEFT, 300);
			break;

		case LOYALTYFIELD_INFO3:
			pReport->AddColumn(texts.GetText(LOYALTYFIELD_INFO3, Account::Info3.Label), TA_LEFT, 300);
			break;

		case LOYALTYFIELD_INFO4:
			pReport->AddColumn(texts.GetText(LOYALTYFIELD_INFO4, Account::Info4.Label), TA_LEFT, 300);
			break;

		case LOYALTYFIELD_PHONE1:
			pReport->AddColumn(texts.GetText(LOYALTYFIELD_PHONE1, Account::Phone1.Label), TA_LEFT, 300);
			break;

		case LOYALTYFIELD_PHONE2:
			pReport->AddColumn(texts.GetText(LOYALTYFIELD_PHONE2, Account::Phone2.Label), TA_LEFT, 300);
			break;

		case LOYALTYFIELD_DOB:
			pReport->AddColumn(texts.GetText(LOYALTYFIELD_DOB, ""), TA_LEFT, 220);
			break;

		case LOYALTYFIELD_DOB_DAY:
			pReport->AddColumn("Day", TA_LEFT, 120);
			break;

		case LOYALTYFIELD_DOB_NEXTAGE:
			pReport->AddColumn ( "NxtAge", TA_RIGHT, 150 );
			break;

		case LOYALTYFIELD_ALERTCODES:
			pReport->AddColumn("A", TA_CENTER, 69);
			break;

		case LOYALTYFIELD_INACTIVE:
			pReport->AddColumn("In", TA_CENTER, 69);
			break;
		}			
	}
}

//*******************************************************************

void CDbReportHandler::AppendDbFieldsToCSVLine(CSQLRowAccountFull& RowAccount, CCSV* pCsv, bool bExists)
{
	CArray<int, int> arrayHeader;
	LoyaltyHeaderHelpers.CreateNumericHeader(m_strHeader, arrayHeader);
	CreateReportLine(arrayHeader, pCsv, RowAccount, bExists);
}

//*******************************************************************

void CDbReportHandler::CreateReportLine ( CArray<int,int>& arrayHeader, CCSV* pCsv, CSQLRowAccountFull& RowAccount, bool bExists )
{
	if (FALSE == bExists)									// no database record
	{
		return;
	}

	for ( int i = 0 ; i < arrayHeader.GetSize() ; i++ )
	{
		switch (arrayHeader.GetAt(i))
		{
		// add card no & hotlist flag if required
		case LOYALTYFIELD_USERID:
			AddCardNo(pCsv, RowAccount, TRUE, FALSE);									
			break;

		case LOYALTYFIELD_EXTERNALREF:	
	 		pCsv->Add (RowAccount.GetExternalRef() );
			break;

		case LOYALTYFIELD_QRINFO:
			pCsv->Add(RowAccount.GetQRInfo());
			break;

		case LOYALTYFIELD_FULLNAME:
	 		pCsv->Add (RowAccount.GetFullname() );
			break;

		case LOYALTYFIELD_FORENAME:
	 		pCsv->Add (RowAccount.GetForename() );
			break;

		case LOYALTYFIELD_SURNAME:
	 		pCsv->Add (RowAccount.GetSurname() );
			break;

		case LOYALTYFIELD_GROUPNO:
			pCsv->Add (RowAccount.GetGroupNo() );
			break;

		case LOYALTYFIELD_SCHEME:
			pCsv->Add(RowAccount.GetSchemeNo());
			break;

		case LOYALTYFIELD_GENDER:
			pCsv->Add (RowAccount.GetGender() );
			break;

		case LOYALTYFIELD_POINTS:
			{
				int nPoints = RowAccount.GetPoints();
				pCsv->Add ( nPoints );
				m_nTotalPoints[nREPORT_GROUP] += nPoints;
				m_nTotalPoints[nREPORT_TOTAL] += nPoints;
			}
			break;

		case LOYALTYFIELD_POINTSTODATE:
			{
				int nPoints = RowAccount.GetPointsToDate();
				pCsv->Add ( nPoints );
				m_nTotalPointsTD[nREPORT_GROUP] += nPoints;
				m_nTotalPointsTD[nREPORT_TOTAL] += nPoints;
			}
			break;

		case LOYALTYFIELD_MAXSPEND:
			pCsv->Add(RowAccount.GetMaxSpend(), System.GetDPValue());
			break;

		case LOYALTYFIELD_MAXOVERDRAFT:
			pCsv->Add(RowAccount.GetMaxOverdraft(), System.GetDPValue());
			break;

		case LOYALTYFIELD_EXPIRYDATE:
			pCsv->Add(RowAccount.GetExpiryDate());
			break;

		case LOYALTYFIELD_PURSE1_BALANCE:
			{
				double dPurse = RowAccount.GetPurse1Balance();
				pCsv->Add(dPurse, System.GetDPValue());
				m_dTotalPurse1[nREPORT_GROUP] += dPurse;
				m_dTotalPurse1[nREPORT_TOTAL] += dPurse;
			}
			break;

		case LOYALTYFIELD_PURSE1_LASTSPENDDATE:
			pCsv->Add(RowAccount.GetPurse1LastSpendDate());
			break;

		case LOYALTYFIELD_PURSE1_LASTSPENDTIME:
			pCsv->Add(RowAccount.GetPurse1LastSpendTime());
			break;

		case LOYALTYFIELD_PURSE1_LASTSPEND:
			pCsv->Add(RowAccount.GetPurse1LastSpend(), System.GetDPValue());
			break;

		case LOYALTYFIELD_PURSE1_SPENDTODATE:
			{
				double dSpendTD = RowAccount.GetPurse1SpendToDate();
				pCsv->Add(dSpendTD, System.GetDPValue());
				m_dTotalPurse1SpendTD[nREPORT_GROUP] += dSpendTD;
				m_dTotalPurse1SpendTD[nREPORT_TOTAL] += dSpendTD;
			}
			break;

		case LOYALTYFIELD_PURSE2_BALANCE:
			{
				CSQLRowGroup RowGroup;
				RowGroup.SetGroupNo(RowAccount.GetGroupNo());
				
				CSQLRepositoryGroup repoGroup;
				repoGroup.SelectRow(RowGroup, NULL);
				
				double dPurse = LoyaltySubs.GetPurse2NextSpendBalance(RowAccount, RowGroup);
				pCsv->Add(dPurse, System.GetDPValue());
				m_dTotalPurse2[nREPORT_GROUP] += dPurse;
				m_dTotalPurse2[nREPORT_TOTAL] += dPurse;
			}
			break;

		case LOYALTYFIELD_PURSE2_LASTSPENDDATE:
			pCsv->Add(RowAccount.GetPurse2LastSpendDate());
			break;

		case LOYALTYFIELD_PURSE2_LASTSPENDTIME:
			pCsv->Add(RowAccount.GetPurse2LastSpendTime());
			break;

		case LOYALTYFIELD_PURSE2_LASTSPEND:
			pCsv->Add(RowAccount.GetPurse2LastSpend(), System.GetDPValue());
			break;

		case LOYALTYFIELD_PURSE2_SPENDTODATE:
			{
				double dSpendTD = RowAccount.GetPurse2SpendToDate();
				pCsv->Add(dSpendTD, System.GetDPValue());
				m_dTotalPurse2SpendTD[nREPORT_GROUP] += dSpendTD;
				m_dTotalPurse2SpendTD[nREPORT_TOTAL] += dSpendTD;
			}
			break;

		case LOYALTYFIELD_PURSE2_REFRESHEDDATE:
			pCsv->Add(RowAccount.GetPurse2RefreshedDate());
			break;

		case LOYALTYFIELD_PURSE2_REFRESHEDTIME:
			pCsv->Add(RowAccount.GetPurse2RefreshedTime());
			break;

		case LOYALTYFIELD_CASH_LASTSPENDDATE:
			pCsv->Add(RowAccount.GetCashLastSpendDate());
			break;

		case LOYALTYFIELD_CASH_LASTSPENDTIME:
			pCsv->Add(RowAccount.GetCashLastSpendTime());
			break;

		case LOYALTYFIELD_CASH_LASTSPEND:
			pCsv->Add(RowAccount.GetCashLastSpend(), System.GetDPValue());
			break;

		case LOYALTYFIELD_CASH_SPENDTODATE:
			{
				double dSpendTD = RowAccount.GetCashSpendToDate();
				pCsv->Add(dSpendTD, System.GetDPValue());
				m_dTotalCashSpendTD[nREPORT_GROUP] += dSpendTD;
				m_dTotalCashSpendTD[nREPORT_TOTAL] += dSpendTD;
			}
			break;

		case LOYALTYFIELD_TOTAL_SPENDTODATE:
			{
				double dSpendTD = RowAccount.GetTotalSpendToDate();
				pCsv->Add(dSpendTD, System.GetDPValue());
				m_dTotalSpendTD[nREPORT_GROUP] += dSpendTD;
				m_dTotalSpendTD[nREPORT_TOTAL] += dSpendTD;
			}
			break;

		case LOYALTYFIELD_ADDRESS1:
			pCsv->Add(RowAccount.GetAddress1());
			break;

		case LOYALTYFIELD_ADDRESS2:
			pCsv->Add(RowAccount.GetAddress2());
			break;

		case LOYALTYFIELD_ADDRESS3:
			pCsv->Add(RowAccount.GetAddress3());
			break;

		case LOYALTYFIELD_ADDRESS4:
			pCsv->Add(RowAccount.GetAddress4());
			break;

		case LOYALTYFIELD_ADDRESS5:
			pCsv->Add(RowAccount.GetAddress5());
			break;

		case LOYALTYFIELD_PHONE1:
			pCsv->Add(RowAccount.GetPhone1());
			break;

		case LOYALTYFIELD_PHONE2:
			pCsv->Add(RowAccount.GetPhone2());
			break;

		case LOYALTYFIELD_INFO1:
			pCsv->Add(RowAccount.GetInfo1());
			break;

		case LOYALTYFIELD_INFO2:
			pCsv->Add(RowAccount.GetInfo2());
			break;

		case LOYALTYFIELD_INFO3:
			pCsv->Add(RowAccount.GetInfo3());
			break;

		case LOYALTYFIELD_INFO4:
			pCsv->Add(RowAccount.GetInfo4());
			break;

		case LOYALTYFIELD_DOB:
			pCsv->Add(RowAccount.GetDOB());
			break;

		case LOYALTYFIELD_DOB_NEXTAGE:
			pCsv->Add(RowAccount.GetAgeNextBirthday());
			break;

		case LOYALTYFIELD_DOB_DAY:
			pCsv->Add(RowAccount.GetDOBDayOfWeek());
			break;

		case LOYALTYFIELD_HOTLISTDATE:
			pCsv->Add(RowAccount.GetHotlistDate());
			break;

		case LOYALTYFIELD_HOTLISTTIME:
			pCsv->Add(RowAccount.GetHotlistTime());
			break;

		case LOYALTYFIELD_HOTLISTCODE:
			pCsv->Add(RowAccount.GetHotlistCode());
			break;

		case LOYALTYFIELD_PREVUSERID:
			{
				if (RowAccount.HavePreviousUserID() == TRUE)
				{
					pCsv->Add(RowAccount.GetPreviousUserID());
				}
				else
				{
					pCsv->Add("");
				}
			}
			break;

		case LOYALTYFIELD_NEXTUSERID:
			{
				if (RowAccount.HaveNextUserID() == TRUE)
				{
					pCsv->Add(RowAccount.GetNextUserID());
				}
				else
				{
					pCsv->Add("");
				}
			}
			break;

		case LOYALTYFIELD_ALERTCODES:
			pCsv->Add(RowAccount.GetAlertCodes());
			break;

		case LOYALTYFIELD_INACTIVE:
			{
				if (RowAccount.GetInactive() == TRUE)
				{
					pCsv->Add("Y");
				}
				else
				{
					pCsv->Add("-");
				}
			}
			break;

		case LOYALTYFIELD_DELDATE:
			pCsv->Add(CSSDate::GetCurrentDate().GetDate());
			break;

		case LOYALTYFIELD_DELTIME:
			pCsv->Add(CSSTime::GetCurrentTime().GetTime());
			break;
		}
	}
}

//*******************************************************************

bool CDbReportHandler::CreateTotalLine ( CArray<int,int>& arrayHeader, CCSV* pCsv, CCSV* pCsvTabLine, int nIndex )
{
	bool bHaveAnyTotals = FALSE;

	for ( int i = 0 ; i < arrayHeader.GetSize() ; i++ )
	{
		bool bHaveTotal = FALSE;

		switch (arrayHeader.GetAt(i))
		{
		case LOYALTYFIELD_POINTS:
			pCsv->Add(m_nTotalPoints[nIndex]);
			bHaveTotal = TRUE;
			break;

		case LOYALTYFIELD_POINTSTODATE:
			pCsv->Add(m_nTotalPointsTD[nIndex]);
			bHaveTotal = TRUE;
			break;

		case LOYALTYFIELD_PURSE1_BALANCE:
			pCsv->Add(m_dTotalPurse1[nIndex], System.GetDPValue());
			bHaveTotal = TRUE;
			break;

		case LOYALTYFIELD_PURSE2_BALANCE:
			pCsv->Add(m_dTotalPurse2[nIndex], System.GetDPValue());
			bHaveTotal = TRUE;
			break;

		case LOYALTYFIELD_PURSE1_SPENDTODATE:
			pCsv->Add(m_dTotalPurse1SpendTD[nIndex], System.GetDPValue());
			bHaveTotal = TRUE;
			break;

		case LOYALTYFIELD_PURSE2_SPENDTODATE:
			pCsv->Add(m_dTotalPurse2SpendTD[nIndex], System.GetDPValue());
			bHaveTotal = TRUE;
			break;

		case LOYALTYFIELD_CASH_SPENDTODATE:
			pCsv->Add(m_dTotalCashSpendTD[nIndex], System.GetDPValue());
			bHaveTotal = TRUE;
			break;

		case LOYALTYFIELD_TOTAL_SPENDTODATE:
			pCsv->Add ( m_dTotalSpendTD[nIndex], System.GetDPValue() );
			bHaveTotal = TRUE;
			break;
		}
		
		if (bHaveTotal == TRUE)
		{
			pCsvTabLine->Add("<LI>");
			bHaveAnyTotals = TRUE;
		}
		else
		{
			pCsv->Add ( "" );
			pCsvTabLine->Add ("");
		}
	}

	return bHaveAnyTotals;
}

//*******************************************************************
