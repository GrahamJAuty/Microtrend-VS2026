/****************************************************
Handles Database Import / Export filename & header lines
also database reporting

	UR01 - UR99		Custom Database reports
	R001			Hotlist
	R002			Anniverary
	R003			Low points usage
	R004			Low spend usage
	R005			Ranking points
	R006			Ranking spend
	R007			Active cards
	R008			Inactive cards
	R009			Starting Balances
	R010			Card Activity - Never used
	R011			PhotoID List

	R101			Audit report
	R102			Revaluation bonus
	R103			Spend Bonus
	R104			Purchse history
	R105			Purchase history ( single card )
	R106			Purchase ranking
	R106a			Purchase ranking ( accumulator PropSheet page1 defaults )
	R106b			Purchase ranking ( accumulator PropSheet page2 defaults )
	R107			Batch updates
	R108			batch Deletes
	R109			Cash Added
	R110			Redeem bonus
	R111			Batch Redeem
	R112			Points Audit
	R113			Points Added
	R114			Points Deducted
	R115			Card Sale
	R116			Daily Differ  CashTaken vs CardSales )
	R117			CSV import file
	R118			Group Period Sales
	R119			Purse1 Additions
	R120			Refund Audit
	R121			Visit Totaliser
	R122			Archive Search
	R123			Single Audit report

	R124			Cashless Liability Reconciliation
	R125			Cashless Posting

	R126			Family Member listing
	R127			Validation chcek on 'Plupts' Update File
	R128			Validation check on 'Stampoffer' Update File
	R129			Stamp Offer Report
	R130			Stamp Offer Report Single
	R131			Validation chcek on 'Department' Update File

	R132			Audit Transaction Summary
	R133			Audit Transaction Summary Single

	R134			Validation check on 'LoyaltyScheme' Update File

	R901			ImportExceptions
****************************************************/

//****************************************************
#include "..\SPosLoyalty4Shared\Consolidation.h"
#include "..\SPosLoyalty4Shared\LoyaltyHeaderHelpers.h"
#include "..\SPosLoyalty4Shared\LoyaltySubs.h"
#include "..\SPosLoyalty4Shared\ReportConsolidationArray.h"
#include "..\SPosLoyalty4Shared\SQLNamespaces.h"
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRepositoryAccount.h"
//****************************************************
#include "AlertText.h"
#include "DbExportHelpers.h"
#include "GroupComboHelpers.h"
#include "LoyaltyManager.h"
//****************************************************
#include "DbExportHandler.h"
//****************************************************
static const int nFILTERFIELDS		= 18;
static const int nCONDITIONFIELDS	= 7;
static const int nOUTPUTFIELDS		= 2;
//****************************************************
// old style in OrderBy paramater for convert to SortOn string parameter
// done when SortBy Surname added (18/07/2016)
//****************************************************
static const int	nORDERBYCARDNO			= 0;
static const int	nORDERBYCARDNAME		= 1;
static const int	nORDERBYINFO1			= 2;
static const int	nORDERBYINFO2			= 3;
static const int	nORDERBYINFO3			= 4;
static const int	nORDERBYINFO4			= 5;
static const int	nORDERBYADDRESS1		= 6;
static const int	nORDERBYADDRESS2		= 7;
static const int	nORDERBYADDRESS3		= 8;
static const int	nORDERBYADDRESS4		= 9;
static const int	nORDERBYADDRESS5		= 10;
static const int	nORDERBYEXTREF			= 11;
//****************************************************

CDbExportHandler::CDbExportHandler()
{
	m_strDefFilename = Filenames.GetExportHandlerFilename();
	m_strLabel = "";
	m_strDefaultConditions = "";
	Reset();
}

//****************************************************

void CDbExportHandler::Reset()
{
	m_strTitle = "";					// some titles get created during editting
	m_strFileFilters = "";
	m_strDefExtension = "";

	m_strFilename = "";					// export file name OR report name descriptor
	m_strHeader = "";					// fields to export OR fields to include in report

	m_nExportFlag = nDBEXPORTFIELDS_REPORT;
	m_bSecondaryOutput = TRUE;
	m_bOrderByAllowed = TRUE;
	m_bGroupSummaryAllowed = TRUE;
	m_bGroupSortAllowed = TRUE;
	m_bCardListAllowed = TRUE;
	m_bUnknownAllowed = FALSE;
}

//****************************************************

bool CDbExportHandler::Read(const char* szLabel)
{
	m_strLabel = szLabel;
	Reset();

	SetTitles();										// set default text / titles / headers etc
	SetDefaultFilters();								// filters
	SetDefaultConditions();
	SetDefaultOutputMethod();

	CSharedStringArray Array;
	if (Array.Open(m_strDefFilename, DB_READONLY) == DB_ERR_NONE)
	{
		for (int nIndex = 0; nIndex < Array.GetSize(); nIndex++)
		{
			CCSV csv(Array.GetAt(nIndex));

			if (m_strLabel == csv.GetString(0))
			{
				m_strFilename = csv.GetString(1);		// filename 
				m_strHeader = csv.GetString(2);		// header
				CCSV csvBuf1(csv.GetString(3));		// report filters
				CCSV csvBuf2(csv.GetString(4));		// conditional
				CCSV csvBuf3(csv.GetString(5));		// output method

				// Filters
				if (csvBuf1.GetSize() <= nFILTERFIELDS)
				{
					for (int i = 0; i < csvBuf1.GetSize(); i++)
					{
						m_csvFilters.SetAt(i, csvBuf1.GetString(i));
					}
				}

				//Conditions
				if (csvBuf2.GetSize() == nCONDITIONFIELDS)
				{
					for (int i = 0; i < nCONDITIONFIELDS; i++)
					{
						m_csvConditions.SetAt(i, csvBuf2.GetString(i));
					}
				}

				// output method
				if (csvBuf3.GetSize() == nOUTPUTFIELDS)
				{
					for (int i = 0; i < nOUTPUTFIELDS; i++)
					{
						m_csvOutputMethod.SetAt(i, csvBuf3.GetString(i));
					}
				}

				return TRUE;
			}
		}
	}

	return FALSE;
}

//****************************************************
// get list of custom reports
// csv line = 'URnn,repname,URnn,repname etc..

void CDbExportHandler::GetCustomList(CCSV& csvLine)
{
	CSharedStringArray Array;
	if (Array.Open(m_strDefFilename, DB_READONLY) == DB_ERR_NONE)
	{
		for (int nIndex = 0; nIndex < Array.GetSize(); nIndex++)
		{
			CCSV csv(Array.GetAt(nIndex));

			CString strLabel = csv.GetString(0);

			if (strLabel.Left(2) == "UR")
			{
				csvLine.Add(strLabel);					// label
				csvLine.Add(csv.GetString(1));			// filename /report name
			}
		}
	}
}

//****************************************************

bool CDbExportHandler::GetNextCustomLabel(CString& strNextLabel)
{
	CSharedStringArray Array;
	if (Array.Open(m_strDefFilename, DB_READONLY) == DB_ERR_NONE)
	{
		for (int nCount = 1; nCount < 100; nCount++)
		{
			strNextLabel.Format("UR%2.2d", nCount);

			int nIndex;
			for (nIndex = 0; nIndex < Array.GetSize(); nIndex++)
			{
				CCSV csv(Array.GetAt(nIndex));

				if (strNextLabel == csv.GetString(0))		// label found
				{
					break;
				}
			}

			if (nIndex >= Array.GetSize())				// see if reached end without finding label
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}

//****************************************************

bool CDbExportHandler::IsCustomReport()
{
	return (m_strLabel.Left(2) == "UR") ? TRUE : FALSE;
}

//****************************************************

bool CDbExportHandler::Save ( const char* szLabel )
{
	bool bReply = FALSE;

	m_strLabel = szLabel;
	CSharedStringArray Array;
	if ( Array.Open ( m_strDefFilename, DB_READWRITE ) == DB_ERR_NONE )
	{
		for ( int nIndex = 0; nIndex < Array.GetSize(); nIndex++ )
		{
			CCSV csv ( Array.GetAt( nIndex ) );
			if ( m_strLabel == csv.GetString(0) )
			{
				Array.RemoveAt ( nIndex );				// delete old line
				break;
			}
		}

		CCSV csv;
		csv.Add ( m_strLabel );
		csv.Add ( m_strFilename );
		csv.Add ( m_strHeader );
		csv.Add ( m_csvFilters.GetLine() );
		csv.Add ( m_csvConditions.GetLine() );
		csv.Add ( m_csvOutputMethod.GetLine() );

		if (Array.GetSize() == 0)
		{
			Array.Add(csv.GetLine());
		}
		else
		{
			Array.InsertAt(0, csv.GetLine());
		}

		bReply = Array.Close();
	}

	return bReply;
}

//****************************************************

bool CDbExportHandler::Delete(const char* szLabel)
{
	bool bReply = FALSE;

	CSharedStringArray Array;
	if (Array.Open(m_strDefFilename, DB_READWRITE) == DB_ERR_NONE)
	{
		CString strLabel = szLabel;
		for (int nIndex = 0; nIndex < Array.GetSize(); nIndex++)
		{
			CCSV csv(Array.GetAt(nIndex));
			if (strLabel == csv.GetString(0))
			{
				Array.RemoveAt(nIndex);
				m_strLabel = "";
				break;
			}
		}

		bReply = Array.Close();
	}

	return bReply;
}

//****************************************************
// Paremeter changed from an int to a string

CString CDbExportHandler::GetSortOnLabel()
{
	// "UserID", "Info1" ... 
	CString strLabel = m_csvFilters.GetString(10);		
	
	// old orderby parameter was an int
	if (::IsStringNumeric(strLabel) == TRUE)				
	{
		switch (atoi(strLabel))							
		{
		case nORDERBYCARDNAME:	
			strLabel = Account::Fullname.Label;		
			break;

		case nORDERBYINFO1:		
			strLabel = Account::Info1.Label;		
			break;

		case nORDERBYINFO2:		
			strLabel = Account::Info2.Label;		
			break;

		case nORDERBYINFO3:		
			strLabel = Account::Info3.Label;		
			break;

		case nORDERBYINFO4:		
			strLabel = Account::Info4.Label;		
			break;

		case nORDERBYADDRESS1:	
			strLabel = Account::Address1.Label;		
			break;

		case nORDERBYADDRESS2:	
			strLabel = Account::Address2.Label;		
			break;

		case nORDERBYADDRESS3:	
			strLabel = Account::Address3.Label;		
			break;

		case nORDERBYADDRESS4:	
			strLabel = Account::Address4.Label;		
			break;

		case nORDERBYADDRESS5:	
			strLabel = Account::Address5.Label;		
			break;

		case nORDERBYEXTREF:	
			strLabel = Account::ExternalRef.Label;	
			break;

		case nORDERBYCARDNO:
		default:
			strLabel = Account::UserID.Label;	
			break;
		}
	}

	return strLabel;
}

//****************************************************

int CDbExportHandler::GetSortOn()
{
	int nReply = nSORTONCARDNO;

	switch (LoyaltyHeaderHelpers.GetNumericField(GetSortOnLabel()))
	{
	case LOYALTYFIELD_USERID:
		nReply = nSORTONCARDNO;
		break;

	case LOYALTYFIELD_FULLNAME:
		nReply = nSORTONCARDNAME;
		break;

	case LOYALTYFIELD_SURNAME:
		nReply = nSORTONSURNAME;
		break;

	case LOYALTYFIELD_EXTERNALREF:
		nReply = nSORTONEXTREF;
		break;

	case LOYALTYFIELD_INFO1:
		nReply = nSORTONINFO1;
		break;

	case LOYALTYFIELD_INFO2:
		nReply = nSORTONINFO2;
		break;

	case LOYALTYFIELD_INFO3:
		nReply = nSORTONINFO3;
		break;

	case LOYALTYFIELD_INFO4:
		nReply = nSORTONINFO4;
		break;

	case LOYALTYFIELD_ADDRESS1:
		nReply = nSORTONADDRESS1;
		break;

	case LOYALTYFIELD_ADDRESS2:
		nReply = nSORTONADDRESS2;
		break;

	case LOYALTYFIELD_ADDRESS3:
		nReply = nSORTONADDRESS3;
		break;

	case LOYALTYFIELD_ADDRESS4:
		nReply = nSORTONADDRESS4;
		break;

	case LOYALTYFIELD_ADDRESS5:
		nReply = nSORTONADDRESS5;
		break;
	}
	
	return nReply;
}

//****************************************************

void CDbExportHandler::SetTitles()
{
	// export formats

	if (m_strLabel == "CSV")
	{
		m_strTitle = "CSV File Export";
		m_strFileFilters = "CSV Files (*.csv)|*.csv";
		m_strDefExtension = "csv";
		m_nExportFlag = nDBEXPORTFIELDS_DATA;				// export all fields to a file
		m_bSecondaryOutput = FALSE;
	}

	//	else if ( m_strLabel == "TXT" )
	//	{
	//		m_strTitle			= "MailMerge File Export";
	//		m_strFileFilters	= "MailMerge Files (*.txt)|*.txt";
	//		m_strDefExtension	= "txt";
	//		m_nExportFlag		= nDBEXPORTFIELDS_DATA;				// export all fields to a file
	//	}

	//	else if ( m_strLabel == "DAT" )
	//	{
	//		m_strTitle			= "Data File Export";
	//		m_strFileFilters	= "Data Files (*.dat)|*.dat";
	//		m_strDefExtension	= "dat";
	//		m_nExportFlag		= nDBEXPORTFIELDS_DATA;				// export all fields to a file
	//	}

	else if (m_strLabel.Left(2) == "DB")					// DB1, DB2, DB3
	{
		m_strTitle.Format("Automatic CSV File Export%d", atoi(m_strLabel.Mid(2)));
		m_strFileFilters = "CSV Files (*.csv)|*.csv";
		m_strDefExtension = "csv";
		m_nExportFlag = nDBEXPORTFIELDS_DATA;				// export all fields to a file
		m_bSecondaryOutput = FALSE;
	}

	else if (m_strLabel == "DEL")
	{
		m_strTitle = "Delete Database Record Export";
		m_strFileFilters = "Data Files (*.dat)|*.dat|CSV Files (*.csv)|*.csv";
		m_strDefExtension = "dat";
		m_nExportFlag = nDBEXPORTFIELDS_DATA;				// export all fields to a file
		m_bSecondaryOutput = FALSE;
	}

	// report formats

	else if (m_strLabel.Left(2) == "UR")
	{
		m_strFilename.Format("Database Report %s", 
			(const char*) m_strLabel.Mid(2));	// default report name

		m_strTitle = m_strFilename;				// title for export prop page
		m_nExportFlag = nDBEXPORTFIELDS_DBASE;
		m_bGroupSummaryAllowed = FALSE;
	}

	else if (m_strLabel == "R001")
	{
		m_strTitle = "Hotlisted Cards";
		m_nExportFlag = nDBEXPORTFIELDS_REPORT;
	}

	else if (m_strLabel == "R002")
	{
		m_strTitle = "Anniversary Report";
		m_nExportFlag = nDBEXPORTFIELDS_REPORT;
		m_bOrderByAllowed = FALSE;
		m_bGroupSortAllowed = FALSE;
		m_bGroupSummaryAllowed = FALSE;
	}

	else if (m_strLabel == "R003")
	{
		m_strTitle = "Low Points Usage";
		m_nExportFlag = nDBEXPORTFIELDS_REPORT;
		m_bGroupSummaryAllowed = FALSE;
		m_bUnknownAllowed = TRUE;
	}

	else if (m_strLabel == "R004")
	{
		m_strTitle = "Low Spend Usage";
		m_nExportFlag = nDBEXPORTFIELDS_REPORT;
		m_bGroupSummaryAllowed = FALSE;
		m_bUnknownAllowed = TRUE;
	}

	else if (m_strLabel == "R005")
	{
		m_strTitle = "Points Ranking";
		m_nExportFlag = nDBEXPORTFIELDS_REPORT;
		m_bOrderByAllowed = FALSE;
		m_bGroupSummaryAllowed = FALSE;
		m_bGroupSortAllowed = FALSE;
		m_bUnknownAllowed = TRUE;
	}

	else if (m_strLabel == "R006")
	{
		m_strTitle = "Spend Ranking";
		m_nExportFlag = nDBEXPORTFIELDS_REPORT;
		m_bOrderByAllowed = FALSE;
		m_bGroupSummaryAllowed = FALSE;
		m_bGroupSortAllowed = FALSE;
		m_bUnknownAllowed = TRUE;
	}

	else if (m_strLabel == "R007")
	{
		m_strTitle = "Active Cards";
		m_nExportFlag = nDBEXPORTFIELDS_REPORT;
		m_bGroupSummaryAllowed = FALSE;
	}

	else if (m_strLabel == "R008")
	{
		m_strTitle = "Inactive Cards";
		m_nExportFlag = nDBEXPORTFIELDS_REPORT;
		m_bGroupSummaryAllowed = FALSE;
	}

	else if (m_strLabel == "R009")
	{
		m_strTitle = "Estimated Starting Balances";
		m_nExportFlag = nDBEXPORTFIELDS_REPORT;
		m_bGroupSummaryAllowed = FALSE;
		m_bGroupSortAllowed = FALSE;
		m_bUnknownAllowed = TRUE;
		m_bOrderByAllowed = FALSE;
	}
	else if (m_strLabel == "R010")
	{
		m_strTitle = "Never used cards";
		m_nExportFlag = nDBEXPORTFIELDS_REPORT;
		m_bGroupSummaryAllowed = FALSE;
	}
	else if (m_strLabel == "R106")
	{
		m_strTitle = "";				// Purchase Ranking
		m_nExportFlag = nDBEXPORTFIELDS_REPORT;
		m_bGroupSummaryAllowed = FALSE;
		m_bGroupSortAllowed = FALSE;
		m_bOrderByAllowed = FALSE;
	}

	else if (m_strLabel == "R011")
	{
		m_strTitle = "Photo ID Listing";
		m_nExportFlag = nDBEXPORTFIELDS_NONE;
		m_bGroupSummaryAllowed = FALSE;
		m_bSecondaryOutput = FALSE;
	}
	/*
	//	else if ( m_strLabel == "R003" )
	//	{
	//		m_strTitle				= "Low Threshold Balances";
	//		m_nExportFlag			= nDBEXPORTFIELDS_REPORT;
	//		m_bConditionalOutput	= FALSE;				// fields & conditions forced - set by report
	//	}
	//	else if ( m_strLabel == "R004" )
	//	{
	//		m_strTitle				= "Negative Card Balances";
	//		m_nExportFlag			= nDBEXPORTFIELDS_REPORT;
	//		m_bConditionalOutput	= FALSE;
	//	}
	//	else if ( m_strLabel == "R005" )
	//	{
	//		m_strTitle				= "Pending Card Balances";
	//		m_nExportFlag			= nDBEXPORTFIELDS_REPORT;
	//		m_bConditionalOutput	= FALSE;
	//	}
	//	else if ( m_strLabel == "R009" )
	//	{
	//		m_strTitle				= "Never credited cards";
	//		m_nExportFlag			= nDBEXPORTFIELDS_REPORT;
	//		m_bConditionalOutput	= FALSE;
	//		m_bGroupSummaryAllowed	= FALSE;
	//	}
	//	else if ( m_strLabel == "R012" )
	//	{
	//		m_strTitle				= "Ecr Pending Balance Summary";
	//		m_nExportFlag			= nDBEXPORTFIELDS_REPORT;
	//		m_bConditionalOutput	= FALSE;
	//	}
	//	else if ( m_strLabel == "R013" )
	//	{
	//		m_strTitle				= "Card Balances (inc.Ecr Pending)";
	//		m_nExportFlag			= nDBEXPORTFIELDS_REPORT;
	//		m_bConditionalOutput	= FALSE;
	//	}
	//	else if ( m_strLabel == "R019" )
	//	{
	//		m_strTitle				= "Hotlisted Cards";
	//		m_nExportFlag			= nDBEXPORTFIELDS_REPORT;
	//		m_bConditionalOutput	= FALSE;
	//		m_bCardListAllowed		= FALSE;
	//	}
	//	else if ( m_strLabel == "R020" )
	//	{
	//		m_strTitle				= "Speeder Attendance";
	//		m_nExportFlag			= nDBEXPORTFIELDS_REPORT;
	//		m_bConditionalOutput	= FALSE;
	//		m_bUnknownAllowed		= TRUE;
	//	}
	//	else if ( m_strLabel == "R021" )
	//	{
	//		m_strTitle				= "Speeder Absence";
	//		m_nExportFlag			= nDBEXPORTFIELDS_REPORT;
	//		m_bConditionalOutput	= FALSE;
	//		m_bUnknownAllowed		= TRUE;
	//	}
	*/

	if (m_nExportFlag == nDBEXPORTFIELDS_DATA)
	{
		m_strHeader = "CardNo,GroupNo,FullName";			// default header line
	}
}

//****************************************************

void CDbExportHandler::SetDefaultFilters()
{
	bool bHotlisted = (m_strLabel == "R001") ? TRUE : FALSE;	// Hotlisted cards

	m_csvFilters.RemoveAll();						// make sure empty before adding

	SetCardRangeIndex(0);
	SetStartCardNo(System.GetLowCardNo());
	SetEndCardNo(System.GetHighCardNo());
	SetGroupNo(0);
	SetSortByGroup(FALSE);
	SetGroupSummary(FALSE);
	SetSortOn(Account::UserID.Label);
	SetValidReqd(!bHotlisted);
	SetHotlistedReqd(bHotlisted);
	SetExpiredReqd(FALSE);
	SetInactiveReqd(FALSE);
	SetUnknownReqd(FALSE);
	SetSkipBlanks(FALSE);
	SetGenderOption(0);
	SetGenderCode(0);
	SetAlertFilter("");
}

//****************************************************
// called by CLEAR function on dbexport handler prop page

void CDbExportHandler::SetDefaultConditions()
{
	CString strField1 = szPURSE1;
	CString strField2 = szPURSE2;

	CCSV csv;										// create default conditions
	m_csvConditions.RemoveAll();					// make sure empty before adding

	m_csvConditions.Add(strField1);
	csv.Add(strField1);

	m_csvConditions.Add(szDONTCARE);
	csv.Add(szDONTCARE);

	m_csvConditions.Add(0.0, 2);
	csv.Add(0.0, 2);

	m_csvConditions.Add(szOR);
	csv.Add(szOR);

	m_csvConditions.Add(strField2);
	csv.Add(strField2);

	m_csvConditions.Add(szDONTCARE);
	csv.Add(szDONTCARE);

	m_csvConditions.Add(0.0, 2);
	csv.Add(0.0, 2);

	m_strDefaultConditions = csv.GetLine();			// stop default conditions from beong saved.
}

//****************************************************

void CDbExportHandler::SetDefaultOutputMethod()
{
	m_csvOutputMethod.RemoveAll();					// make sure empty before adding
	SetOutputMethod(0);								// report
	SetOutputFile("");								// no ccsv file
}

//****************************************************

bool CDbExportHandler::NonSQLFilter(CSQLRowAccountFull& RowAccount, bool bExist)
{
	if (bExist == FALSE)									// record not in database
	{
		return GetUnknownReqd();
	}

	if (IsCardInRange(RowAccount.GetUserID()) == FALSE)
	{
		return FALSE;
	}

	bool bOK = FALSE;
	bool bIsHotlisted = RowAccount.IsCardHotlisted();
	bool bIsExpired = RowAccount.HasCardExpired();
	bool bIsInactive = RowAccount.GetInactive();

	if (GetValidReqd() == TRUE && ((bIsHotlisted || bIsExpired || bIsInactive) == FALSE))
	{
		bOK = TRUE;
	}
	else if (GetHotlistedReqd() && bIsHotlisted)
	{
		bOK = TRUE;
	}
	else if (GetExpiredReqd() && bIsExpired)
	{
		bOK = TRUE;
	}
	else if (GetInactiveReqd() && bIsInactive )
	{
		bOK = TRUE;
	}

	if (bOK == TRUE)
	{
		bOK = CAlertText::ValidateRecord(RowAccount,GetAlertFilter());
	}

	return bOK;
}

//****************************************************

CString CDbExportHandler::GetSQLOrderBy()
{
	CString strOrderBy = "";

	if (GetSortByGroup() == TRUE)
	{
		strOrderBy = Account::GroupNo.Label;
		strOrderBy += ",";
	}

	int nSortOn = GetSortOn();
	switch (nSortOn)
	{
	case nSORTONCARDNAME:
		strOrderBy += Account::Fullname.Label;
		break;

	case nSORTONSURNAME:
		strOrderBy += Account::Surname.Label;
		break;

	case nSORTONEXTREF:
		strOrderBy += Account::ExternalRef.Label;
		break;

	case nSORTONINFO1:
		strOrderBy += Account::Info1.Label;
		break;

	case nSORTONINFO2:
		strOrderBy += Account::Info2.Label;
		break;

	case nSORTONINFO3:
		strOrderBy += Account::Info3.Label;
		break;

	case nSORTONINFO4:
		strOrderBy += Account::Info4.Label;
		break;

	case nSORTONADDRESS1:
		strOrderBy += Account::Address1.Label;
		break;

	case nSORTONADDRESS2:
		strOrderBy += Account::Address2.Label;
		break;

	case nSORTONADDRESS3:
		strOrderBy += Account::Address3.Label;
		break;

	case nSORTONADDRESS4:
		strOrderBy += Account::Address4.Label;
		break;

	case nSORTONADDRESS5:
		strOrderBy += Account::Address5.Label;
		break;

	case nSORTONCARDNO:
	default:
		nSortOn = nSORTONCARDNO;
		strOrderBy += Account::UserID.Label;
		break;
	}

	if (nSortOn != nSORTONCARDNO)
	{
		strOrderBy += ",";
		strOrderBy += Account::UserID.Label;
	}

	return strOrderBy;
}

//****************************************************

bool CDbExportHandler::IsValidOrderByField(CSQLRowAccountFull& RowAccount)
{
	if (GetSkipBlanks() == FALSE)
	{
		return TRUE;
	}

	CString strOrderOn = "";

	switch (GetSortOn())
	{
	case nSORTONCARDNO:	
		strOrderOn = SimpleFormatInt64Value(RowAccount.GetUserID());	
		break;

	case nSORTONCARDNAME:	
		strOrderOn = RowAccount.GetFullname();	
		break;

	case nSORTONSURNAME:	
		strOrderOn = RowAccount.GetSurname();	
		break;

	case nSORTONINFO1:		
		strOrderOn = RowAccount.GetInfo1();	
		break;

	case nSORTONINFO2:		
		strOrderOn = RowAccount.GetInfo2();	
		break;

	case nSORTONINFO3:		
		strOrderOn = RowAccount.GetInfo3();	
		break;

	case nSORTONINFO4:		
		strOrderOn = RowAccount.GetInfo4();	
		break;

	case nSORTONADDRESS1:	
		strOrderOn = RowAccount.GetAddress1();
		break;

	case nSORTONADDRESS2:	
		strOrderOn = RowAccount.GetAddress2();
		break;

	case nSORTONADDRESS3:	
		strOrderOn = RowAccount.GetAddress3();
		break;

	case nSORTONADDRESS4:	
		strOrderOn = RowAccount.GetAddress4();
		break;

	case nSORTONADDRESS5:	
		strOrderOn = RowAccount.GetAddress5();
		break;

	case nSORTONEXTREF:		
		strOrderOn = RowAccount.GetExternalRef();
		break;
	}

	return (strOrderOn == "") ? FALSE : TRUE;
}

//****************************************************

CString CDbExportHandler::GetSQLConditionalField(int n)
{
	CString strField = (2 == n) ? GetField2() : GetField1();
	return DbExportHelpers.GetExportConditionField(strField);
}

//*******************************************************************

void CDbExportHandler::GetSortItem(CSQLRowAccountFull& AccountRow, CSortedDbReportItem& item, bool bGotRow)
{
	item.m_nUserID = AccountRow.GetUserID();
	item.m_nGroupNo = AccountRow.GetGroupNo();

	if (FALSE == bGotRow)
	{
		CSQLRepositoryAccount repoAccount;
		repoAccount.SelectRow(AccountRow,NULL);
	}

	if (FALSE == GetSortByGroup())
	{
		item.m_nGroupNo = 0;
	}

	switch (GetSortOn())
	{
	case nSORTONCARDNAME:	item.m_strSortField = AccountRow.GetFullname();		break;
	case nSORTONSURNAME:	item.m_strSortField = AccountRow.GetSurname();		break;
	case nSORTONEXTREF:		item.m_strSortField = AccountRow.GetExternalRef();	break;
	case nSORTONINFO1:		item.m_strSortField = AccountRow.GetInfo1();		break;
	case nSORTONINFO2:		item.m_strSortField = AccountRow.GetInfo2();		break;
	case nSORTONINFO3:		item.m_strSortField = AccountRow.GetInfo3();		break;
	case nSORTONINFO4:		item.m_strSortField = AccountRow.GetInfo4();		break;
	case nSORTONADDRESS1:	item.m_strSortField = AccountRow.GetAddress1();		break;
	case nSORTONADDRESS2:	item.m_strSortField = AccountRow.GetAddress2();		break;
	case nSORTONADDRESS3:	item.m_strSortField = AccountRow.GetAddress3();		break;
	case nSORTONADDRESS4:	item.m_strSortField = AccountRow.GetAddress4();		break;
	case nSORTONADDRESS5:	item.m_strSortField = AccountRow.GetAddress5();		break;
	default:				item.m_strSortField = "";							break;
	}
}

//**********************************************************************

bool CDbExportHandler::IsCardInRange(__int64 nCardNo)
{
	bool bResult = FALSE;

	if (GetCardRangeIndex() == 1)
	{
		bResult = m_cardPicker.IsValid(SimpleFormatInt64Value(nCardNo), Filenames.GetReportCardListFilename(m_strLabel));
	}
	else
	{
		bResult = ((nCardNo >= GetStartCardNoInt64()) && (nCardNo <= GetEndCardNoInt64()));
	}

	return bResult;
}

//****************************************************
