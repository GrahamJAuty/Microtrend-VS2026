/**********************************************************************
 Handles Database Import / Export filename & header lines
 also database reporting

	UR01 - UR99		Custom Database reports
	R001				** not used Alert Report
	R002			Birthday
	R003			Low points usage
	R004			Low spend usage
	R005			Ranking points
	R006			Ranking spend
	R007			Active cards
	R008			Inactive cards
	R009			Starting Balances
	R010			Card Activity - Never used
	R011			PhotoID List
	R012			Allergies Report

	R101			Audit report
	R102			Revaluation bonus
	R103			Spend Bonus
	R104			Purchase history ( purchase PropSheet page1 defaults )
	R104a			Purchase history ( purchase PropSheet page2 defaults )
	R105			Purchase history ( single card - purchase PropSheet page1 defaults )
	R105a			Purchase history ( single card - purchase PropSheet page2 defaults )
	R106			Purchase ranking
	R106a			Purchase ranking ( accumulator PropSheet page1 defaults )
	R106b			Purchase ranking ( accumulator PropSheet page2 defaults )
	R107			Batch updates
	R108			batch Deletes
	R109			Cash Added
	R110			Group Period Sales
	R111			Audit report ( single card )
	R112			Points Audit
	R113			Points Added Report
	R114			Points Deucted Report
	R115			Sales report
	R116			Purse1 Additions
	R117			Refund Audit
	R118			Kiosk Audit Additions
	R119			CSV import file
	R120			Card Archive report
	R121			Statements - show balances & purchases
	R122			Statement single account
	R123			Session Audit report ( from with edit record )
	R124			Cashless Liability Reconciliation
	R124A			Cashless posting when called via Cashless Liability
	R125			Cashless Posting
	R126			EndOfDay report
	R127			Leavers Audit
	R128			Weekly Spend report
	R129			Audit Balance Import
	R134			Purchase History XML Export
	R135			Epos Payment Audit
	R136			Purchase Breakdown report
	R137			Redeem bonus audit
	R138			Batch redeem
	R139			Purse credits
	R140			Group Shift
	R141			Group Shift (Single Card)
	R142			Purchase Control Audit

// following don't use any export or report handling just here for completeness.

	R203			Web Payment Exceptions
	R204			Web Payment imports
	R205			Web Payment Listing Service1
	R205A			Web Payment Listing Service2
	R206			Web Payment Export summary report
	R207			Web Payment Export detailed transactions Service1
	R207A			Web Payment Export detailed transactions Service2
	R208			Web Payment Export detailed balances
	R209			Web Payment Export payment echo log
	R210			Web Payment Export diner data echo log
	R211			View Raw SIMS importfile
	R212			View SIMS importfile
	R213			View SIMS Photo import file
	R214			View ParentPay diner importfile
	R215			View ParentPay account importfile
	R216			View CSV importfile
	R217			View ParentPay \ WisePay Opening Balances
	R218			View plupts.csv import file
	R219			View ParentPay Diner Match file
	R220			View Sales \ Topup transactions export file
	R221A			sQuid\ParentMail linked account list
	R221B			sQuid\ParentMail linked account exceptions
	R222			EOD No Sales list file
	R223			View Wonde importfile
	R224			View Wonde Photo import file
	R225			Web Payment Stop indicator data ( include htttp error message )
	R226			View purchasectrl.csv import file
	R227			View Pebble importfile
	R228			Pebble Balance Reconciliation
	R229			Operator Name importfile
	R230			Terminal Name importfile
	R231			Brom Com Sent Treansactions

	R801			Office Meal parameters

	R901			Import Exceptions
	R902			SurnameList Dlg
	R903			Find Dlg
	R999			Temp photo filename

//*********************************************************************/
#include "..\SmartPay4Shared\AllergyText.h"
#include "..\SmartPay4Shared\GlobalFunctions.h"
#include "..\SmartPay4Shared\SmartPayHeaderHelpers.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SmartPay4Shared\SQLTable_BioRegister\SQLRepositoryBioRegister.h"
//*********************************************************************/
#include "DbExportHelpers.h"
//*********************************************************************/
#include "DbExportHandler.h"
//*********************************************************************/

static const int nFILTERFIELDS		= 22;
static const int nCONDITIONFIELDS	= 17;
static const int nOUTPUTFIELDS		= 2;

//**********************************************************************
// old style in OrderBy paramater for convert to SortOn string parameter
// done when SortBy Surname added (11/07/2016)

static const int	nORDERBYUSERID		= 0;			// old int order by parameters
static const int	nORDERBYUSERNAME	= 1;			// replaced by SortOn labels 
static const int	nORDERBYMEMBERID	= 2;			// old 
static const int	nORDERBYEXTREF		= 3;			// old 
static const int	nORDERBYINFO1		= 4;			// old 
static const int	nORDERBYINFO2		= 5;			// old 
static const int	nORDERBYINFO3		= 6;			// old 
static const int	nORDERBYINFO4		= 7;			// old 
static const int	nORDERBYINFO5		= 8;			// old 
static const int	nORDERBYINFO6		= 9;			// old 
static const int	nORDERBYINFO7		= 10;			// old 
static const int	nORDERBYINFO8		= 11;			// old 

//**********************************************************************

CDatabaseReportInfo::CDatabaseReportInfo()
{
	m_strLabel = "";
	m_strName = "";
	m_strHeader = "";
	m_strFilter = "";
	m_strCondition = "";
	m_strOutputMethod = "";
}

//**********************************************************************
//**********************************************************************
//**********************************************************************

CDbExportHandler::CDbExportHandler()
{
	m_strDefFilename = Filenames.GetExportHandlerFilename();
	m_strLabel = "";
	m_strDefaultConditions = "";

	Reset();
}

//**********************************************************************

CDbExportHandler::~CDbExportHandler()
{
}

//**********************************************************************

void CDbExportHandler::Reset()
{
	m_strTitle = "";					// some titles get created during editing
	m_strFileFilters = "";
	m_strDefExtension = "";

	m_strFilename = "";					// export file name OR report name descriptor
	m_strHeader = "";					// fields to export OR fields to include in report

	m_nExportFlag = nDBEXPORTFIELDS_REPORT;
	m_bSecondaryOutput = TRUE;
	m_bSortOnAllowed = TRUE;
	m_bTotaliseAllowed = FALSE;
	m_bGroupSummaryAllowed = TRUE;
	m_bGroupSortAllowed = TRUE;
	m_bCardListAllowed = TRUE;
	m_bUnknownAllowed = FALSE;
}

//**********************************************************************

bool CDbExportHandler::MergeCustomReports(CArray<CDatabaseReportInfo, CDatabaseReportInfo>& arrayCustom)
{
	bool bReply = FALSE;

	CSharedStringArray arrayReports;
	if (arrayReports.Open(m_strDefFilename, DB_READWRITE) == DB_ERR_NONE)
	{
		for (int n = arrayReports.GetSize() - 1; n >= 0; n--)
		{
			CCSV csv(arrayReports.GetAt(n));
			CString strLabel = csv.GetString(0);

			if ((strLabel.GetLength() == 4) && (strLabel.Left(2) == "UR") && (::TestNumeric(strLabel.Right(2)) == TRUE))
			{
				arrayReports.RemoveAt(n);
			}
		}

		for (int n = 0; n < arrayCustom.GetSize(); n++)
		{
			CCSV csv;
			csv.Add(arrayCustom[n].m_strLabel);
			csv.Add(arrayCustom[n].m_strName);
			csv.Add(arrayCustom[n].m_strHeader);
			csv.Add(arrayCustom[n].m_strFilter);
			csv.Add(arrayCustom[n].m_strCondition);
			csv.Add(arrayCustom[n].m_strOutputMethod);
			arrayReports.Add(csv.GetLine());
		}

		bReply = arrayReports.Close();
	}

	return bReply;
}

//**********************************************************************

bool CDbExportHandler::Read ( const char *szLabel )
{
	m_strLabel = szLabel;
	Reset();

	SetTitles();										// set default text / titles / headers etc
	SetDefaultFilters();								// filters
	SetDefaultConditions();
	SetDefaultOutputMethod();

	CSharedStringArray Array;
	if ( Array.Open ( m_strDefFilename, DB_READONLY ) == DB_ERR_NONE )
	{
		for ( int nIndex = 0; nIndex < Array.GetSize(); nIndex++ )
		{
			CCSV csv ( Array.GetAt( nIndex ) );

			if ( m_strLabel == csv.GetString(0) )
			{
				m_strFilename = csv.GetString(1);		// filename 
				m_strHeader   = csv.GetString(2);		// header
				CCSV csvBuf1 ( csv.GetString(3) );		// report filters
				CCSV csvBuf2 ( csv.GetString(4) );		// conditional
				CCSV csvBuf3 ( csv.GetString(5) );		// additional output file

// Filters
				if ( csvBuf1.GetSize() <= nFILTERFIELDS )
				{
					for (int i = 0; i < csvBuf1.GetSize(); i++)
					{
						m_csvFilters.SetAt(i, csvBuf1.GetString(i));
					}
				}

//Conditions
				if ( csvBuf2.GetSize() <= nCONDITIONFIELDS )
				{
					for ( int i = 0 ; i < csvBuf2.GetSize() ; i++ )
						m_csvConditions.SetAt ( i, csvBuf2.GetString(i) );
				}

// output method
				if ( csvBuf3.GetSize() <= nOUTPUTFIELDS )
				{
					for ( int i = 0 ; i < csvBuf3.GetSize() ; i++ )
						m_csvOutputMethod.SetAt ( i, csvBuf3.GetString(i) );
				}

				return TRUE;
			}
		}
	}

	return FALSE;
}

/**********************************************************************/

void CDbExportHandler::GetCustomListArray(CArray<CDatabaseReportInfo, CDatabaseReportInfo>& arrayInfo)
{
	arrayInfo.RemoveAll();

	CSharedStringArray Array;
	if (Array.Open(m_strDefFilename, DB_READONLY) == DB_ERR_NONE)
	{
		for (int nIndex = 0; nIndex < Array.GetSize(); nIndex++)
		{
			CCSV csv(Array.GetAt(nIndex));

			CString strLabel = csv.GetString(0);

			if (strLabel.Left(2) == "UR")
			{
				CDatabaseReportInfo info;
				info.m_strLabel = strLabel;
				info.m_strName = csv.GetString(1);
				info.m_strHeader = csv.GetString(2);
				info.m_strFilter = csv.GetString(3);
				info.m_strCondition = csv.GetString(4);
				info.m_strOutputMethod = csv.GetString(5);
				arrayInfo.Add(info);
			}
		}
	}

	if (arrayInfo.GetSize() == 0)
	{
		CDatabaseReportInfo info;
		info.m_strLabel = "UR01";
		MakeDefaultReportInfo(info);
		arrayInfo.Add(info);
	}
}

/**********************************************************************/

void CDbExportHandler::CopyFromReportInfo(CDatabaseReportInfo& info)
{
	m_strLabel = info.m_strLabel;

	Reset();
	SetTitles();

	m_strFilename = info.m_strName;
	m_strHeader = info.m_strHeader;
	m_csvFilters.ParseLine(info.m_strFilter);
	m_csvConditions.ParseLine(info.m_strCondition);
	m_csvOutputMethod.ParseLine(info.m_strOutputMethod);
}

/**********************************************************************/

void CDbExportHandler::CopyToReportInfo(CDatabaseReportInfo& info)
{
	info.m_strLabel = m_strLabel;
	info.m_strName = m_strFilename;
	info.m_strHeader = m_strHeader;
	info.m_strFilter = m_csvFilters.GetLine();
	info.m_strCondition = m_csvConditions.GetLine();
	info.m_strOutputMethod = m_csvOutputMethod.GetLine();
}

/**********************************************************************/

void CDbExportHandler::MakeDefaultReportInfo(CDatabaseReportInfo& info)
{
	m_strLabel = info.m_strLabel;
	
	Reset();

	SetTitles();										// set default text / titles / headers etc
	SetDefaultFilters();								// filters
	SetDefaultConditions();
	SetDefaultOutputMethod();

	info.m_strName = m_strFilename;
	info.m_strHeader = m_strHeader;
	info.m_strFilter = m_csvFilters.GetLine();
	info.m_strCondition = m_csvConditions.GetLine();
	info.m_strOutputMethod = m_csvOutputMethod.GetLine();
}

/**********************************************************************/

bool CDbExportHandler::IsCustomReport()
{
	return ( m_strLabel.Left(2) == "UR" ) ? TRUE : FALSE;
}

/**********************************************************************/

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

/**********************************************************************/

bool CDbExportHandler::Delete ( const char* szLabel )
{
	bool bReply = FALSE;

	CSharedStringArray Array;
	if ( Array.Open ( m_strDefFilename, DB_READWRITE ) == DB_ERR_NONE )
	{
		CString strLabel = szLabel;
		for ( int nIndex = 0; nIndex < Array.GetSize(); nIndex++ )
		{
			CCSV csv ( Array.GetAt( nIndex ) );
			if ( strLabel == csv.GetString(0) )
			{
				Array.RemoveAt ( nIndex );
				m_strLabel = "";
				break;
			}
		}
		bReply = Array.Close();
	}

	return bReply;
}

//*****************************************************************
// Paremeter changed from an int to a string

CString CDbExportHandler::GetSortOnLabel()
{
	CString strLabel = m_csvFilters.GetString(9);				// "CardNo", "Info1" ... 

	if ( ::IsStringNumeric ( strLabel ) == TRUE )				// old orderby parameter was an int
	{
		switch ( atoi ( strLabel ) )							// convert in to a string label
		{
		case nORDERBYUSERNAME:	strLabel = Account::Username.Label;	break;
		case nORDERBYMEMBERID:	strLabel = Account::MemberID.Label;	break;
		case nORDERBYEXTREF:	strLabel = Account::ExternalRef1.Label;		break;
		case nORDERBYINFO1:		strLabel = Account::Info1.Label;		break;
		case nORDERBYINFO2:		strLabel = Account::Info2.Label;		break;
		case nORDERBYINFO3:		strLabel = Account::Info3.Label;		break;
		case nORDERBYINFO4:		strLabel = Account::Info4.Label;		break;
		case nORDERBYINFO5:		strLabel = Account::Info5.Label;		break;
		case nORDERBYINFO6:		strLabel = Account::Info6.Label;		break;
		case nORDERBYINFO7:		strLabel = Account::Info7.Label;		break;
		case nORDERBYINFO8:		strLabel = Account::Info8.Label;		break;

		case nORDERBYUSERID:
		default:				strLabel = Account::UserID.Label;	break;
		}
	}

	return strLabel;
}

//*****************************************************************

int CDbExportHandler::GetSortOn()
{
	int nReply = nSORTONUSERID;

	switch (SmartPayHeaderHelpers.GetNumericField(GetSortOnLabel()))
	{
	case SMARTPAYFIELD_USERID:
		nReply = nSORTONUSERID;
		break;

	case SMARTPAYFIELD_USERNAME:
		nReply = nSORTONUSERNAME;
		break;

	case SMARTPAYFIELD_SURNAME:
		nReply = nSORTONSURNAME;
		break;

	case SMARTPAYFIELD_MEMBERID:
		nReply = nSORTONMEMBERID;
		break;

	case SMARTPAYFIELD_EXTERNALREF1:
		nReply = nSORTONEXTREF1;
		break;

	case SMARTPAYFIELD_EXTERNALREF2:
		nReply = nSORTONEXTREF2;
		break;

	case SMARTPAYFIELD_INFO1:
		nReply = nSORTONINFO1;
		break;

	case SMARTPAYFIELD_INFO2:
		nReply = nSORTONINFO2;
		break;

	case SMARTPAYFIELD_INFO3:
		nReply = nSORTONINFO3;
		break;

	case SMARTPAYFIELD_INFO4:
		nReply = nSORTONINFO4;
		break;

	case SMARTPAYFIELD_INFO5:
		nReply = nSORTONINFO5;
		break;

	case SMARTPAYFIELD_INFO6:
		nReply = nSORTONINFO6;
		break;

	case SMARTPAYFIELD_INFO7:
		nReply = nSORTONINFO7;
		break;

	case SMARTPAYFIELD_INFO8:
		nReply = nSORTONINFO8;
		break;
	}

	return nReply;
}

//******************************************************************

void CDbExportHandler::SetTitles()
{
// export formats

	if ( m_strLabel == "CSV" )
	{
		m_strTitle			= "CSV File Export";
		m_strFileFilters	= "CSV Files (*.csv)|*.csv";
		m_strDefExtension	= "csv";
		m_nExportFlag		= nDBEXPORTFIELDS_DATA;				// export all fields to a file
		m_bSecondaryOutput	= FALSE;
	}

//	else if ( m_strLabel == "TXT" )
//	{
//		m_strTitle			= "MailMerge File Export";
//		m_strFileFilters	= "MailMerge Files (*.txt)|*.txt";
//		m_strDefExtension	= "txt";
//		m_nExportFlag		= nDBEXPORTFIELDS_DATA;				// export all fields to a file
//	}
//
//	else if ( m_strLabel == "DAT" )
//	{
//		m_strTitle			= "Data File Export";
//		m_strFileFilters	= "Data Files (*.dat)|*.dat";
//		m_strDefExtension	= "dat";
//		m_nExportFlag		= nDBEXPORTFIELDS_DATA;				// export all fields to a file
//	}

	else if ( m_strLabel.Left(2) == "DB" )						// DB1, DB2, DB3
	{
		m_strTitle.Format ( "Automatic CSV File Export%d", atoi(m_strLabel.Mid(2) ) );
		m_strFileFilters	= "CSV Files (*.csv)|*.csv";
		m_strDefExtension	= "csv";
		m_nExportFlag		= nDBEXPORTFIELDS_DATA;				// export all fields to a file
		m_bSecondaryOutput	= FALSE;
	}

	else if ( m_strLabel == "DEL" )
	{
		m_strTitle = "Deleted Database Records Export";
		m_strFileFilters	= "Data Files (*.dat)|*.dat|CSV Files (*.csv)|*.csv";
		m_strDefExtension	= "dat";
		m_nExportFlag		= nDBEXPORTFIELDS_DATA;				// export all fields to a file
		m_bSecondaryOutput	= FALSE;
	}

	else if ( m_strLabel == "DEL2" )							// DEL2 used for EPOS
	{
		m_strFilename = "C:\\VPro\\Data\\BioDeletions.txt";		// default report name
		m_strTitle = "EPOS Delete Account Notification";
		m_strFileFilters	= "Data Files (*.txt)|*.txt";
		m_strDefExtension	= "txt";
		m_nExportFlag		= nDBEXPORTFIELDS_DATA;				// export all fields to a file
		m_bSecondaryOutput	= FALSE;
	}

	else if ( m_strLabel == "EXP" )								// Export Sales & Topups
	{
		m_strTitle = "Export Sales \\ Topup Transactions";
		m_nExportFlag		= nDBEXPORTFIELDS_REPORT;
		m_bSecondaryOutput	= FALSE;

		CCSV csv;
		csv.Add ( Account::UserID.Label );				// add account number
		csv.Add ( Account::MemberID.Label );
		csv.Add ( Account::Username.Label );
		csv.Add ( Account::GroupNo.Label );

		m_strHeader = csv.GetLine();
	}

// report formats

	else if ( m_strLabel.Left(2) == "UR" )
	{
		m_strFilename.Format ( "Database Report %s", (const char*) m_strLabel.Mid(2) );	// default report name
		m_strTitle				= m_strFilename;				// title for export prop page
		m_nExportFlag			= nDBEXPORTFIELDS_DBASE;
		m_bGroupSummaryAllowed	= FALSE;
		m_bTotaliseAllowed		= TRUE;
	}

	else if ( m_strLabel == "R002" )
	{
		m_strTitle				= "Birthday Report";
		m_nExportFlag			= nDBEXPORTFIELDS_REPORT;
		m_bSortOnAllowed		= FALSE;
		m_bGroupSortAllowed		= FALSE;
		m_bGroupSummaryAllowed	= FALSE;
	}
	
	else if ( m_strLabel == "R003" )
	{
		m_strTitle				= "Low Points Usage";
		m_nExportFlag			= nDBEXPORTFIELDS_REPORT;
		m_bGroupSummaryAllowed	= FALSE;
		m_bTotaliseAllowed		= TRUE;
		m_bUnknownAllowed		= TRUE;
	}

	else if ( m_strLabel == "R004" )	
	{
		m_strTitle				= "Low Spend Usage";
		m_nExportFlag			= nDBEXPORTFIELDS_REPORT;
		m_bGroupSummaryAllowed	= FALSE;
		m_bTotaliseAllowed		= TRUE;
		m_bUnknownAllowed		= TRUE;
	}

	else if ( m_strLabel == "R005" )
	{
		m_strTitle				= "Points Ranking";
		m_nExportFlag			= nDBEXPORTFIELDS_REPORT;
		m_bSortOnAllowed		= FALSE;
		m_bGroupSummaryAllowed	= FALSE;
		m_bGroupSortAllowed		= FALSE;
		m_bUnknownAllowed		= TRUE;
	}

	else if ( m_strLabel == "R006" )
	{
		m_strTitle				= "Spend Ranking";
		m_nExportFlag			= nDBEXPORTFIELDS_REPORT;
		m_bSortOnAllowed		= FALSE;
		m_bGroupSummaryAllowed	= FALSE;
		m_bGroupSortAllowed		= FALSE;
		m_bUnknownAllowed		= TRUE;
	}

	else if ( m_strLabel == "R007" )
	{
		m_strTitle				= "Active Users";
		m_nExportFlag			= nDBEXPORTFIELDS_REPORT;
		m_bGroupSummaryAllowed	= FALSE;
		m_bTotaliseAllowed		= TRUE;
	}

	else if ( m_strLabel == "R008" )
	{
		m_strTitle				= "Inactive Users";
		m_nExportFlag			= nDBEXPORTFIELDS_REPORT;
		m_bGroupSummaryAllowed	= FALSE;
		m_bTotaliseAllowed		= TRUE;
	}

	else if ( m_strLabel == "R009" )
	{
		m_strTitle				= "Estimated Starting Balances";
		m_nExportFlag			= nDBEXPORTFIELDS_REPORT;
		m_bGroupSummaryAllowed	= FALSE;
		m_bGroupSortAllowed		= FALSE;
		m_bUnknownAllowed		= TRUE;
		m_bSortOnAllowed		= FALSE;			//11/07/2016 - v1.12x 
	}
	else if ( m_strLabel == "R106" )
	{
		m_strTitle				= "";				// Purchase Ranking
		m_nExportFlag			= nDBEXPORTFIELDS_REPORT;
		m_bGroupSummaryAllowed	= FALSE;
		m_bGroupSortAllowed		= FALSE;
		m_bSortOnAllowed		= FALSE;
	}
	else if ( m_strLabel == "R010" )
	{
		m_strTitle				= "Never used";
		m_nExportFlag			= nDBEXPORTFIELDS_REPORT;
		m_bTotaliseAllowed		= TRUE;
	}
	else if ( m_strLabel == "R011" )
	{
		m_strTitle				= "Photo ID Listing";
		m_nExportFlag			= nDBEXPORTFIELDS_NONE;
		m_bSecondaryOutput	= FALSE;
	}


	if ( m_nExportFlag == nDBEXPORTFIELDS_DATA )
	{
		CCSV csv;
		csv.Add ( Account::UserID.Label );				// add account number

		if ( m_strLabel != "DEL2" )						// not Epos delete file
		{
			csv.Add ( Account::GroupNo.Label );
			csv.Add ( Account::Username.Label );
			csv.Add ( Account::DeletedDate.Label );
			csv.Add ( Account::DeletedTime.Label );
		}

		m_strHeader = csv.GetLine();
	}
}

//*******************************************************************

void CDbExportHandler::SetDefaultFilters()
{
	SetCardRangeIndex(0);
	SetStartCardNo(System.GetLowCardNo());
	SetEndCardNo(System.GetHighCardNo());
	SetGroupNo(0);
	SetSortByGroup(FALSE);
	SetGroupSummary(FALSE);
	SetSortOn(Account::UserID.Label);
	SetValidReqd(TRUE);
	SetExpiredReqd(FALSE);
	SetInactiveReqd(FALSE);
	SetAlertReqd(FALSE);
	SetUnknownReqd(FALSE);
	SetGroupShiftNoneReqd(TRUE);
	SetGroupShiftCurrentReqd(TRUE);
	SetGroupShiftFutureReqd(TRUE);
	SetSkipBlanks(FALSE);
	SetTick1(nFILTER_IGNORE);
	SetTick2(nFILTER_IGNORE);
	SetTick3(nFILTER_IGNORE);
	SetTick4(nFILTER_IGNORE);
	SetUnissuedReqd(FALSE);
	SetTotalise(FALSE);
}

//*******************************************************************
// called by CLEAR function on dbexport handler prop page

void CDbExportHandler::SetDefaultConditions()
{
	SetField1(szPURSE1);
	SetCondition1(szDONTCARE);
	SetValue1(0.0);
	SetAndOr(szOR);
	SetField2(szPURSE1);
	SetCondition2(szDONTCARE);
	SetValue2(0.0);
	SetCheckAllergies(FALSE);
	SetAllergyCodes(0);
	SetCheckAlertCode(FALSE);
	SetAlertCode(0);
	SetCondition3(szDONTCARE);						// gender
	SetInfoFilterIndex(0);								// 0=dont care lse info1 - info8
	SetInfoFilterText("");
	SetBioRegisterCheck(0);								// 0=no check, 1=include if found, 2=exclude if found
	SetDietaryCheck(FALSE);
	SetDietaryText("");

	m_strDefaultConditions = m_csvConditions.GetLine();		// stop default conditions from being saved.
}

//*****************************************************************

void CDbExportHandler::SetDefaultOutputMethod()
{
	m_csvOutputMethod.RemoveAll();					// make sure empty before adding
	SetOutputMethod(0);								// report
	SetOutputFile("");								// no ccsv file
}

/**********************************************************************/

bool CDbExportHandler::NonSQLFilter(CSQLRowAccountFull& AccountRow, bool bExist)
{
	// record not in database
	if (bExist == FALSE)
	{
		return GetUnknownReqd();
	}

	if (IsCardInRange(AccountRow.GetUserIDString()) == FALSE)
	{
		return FALSE;
	}

	int nGroupReqd = GetGroupNo();
	if (nGroupReqd > 0 && (AccountRow.GetGroupNo() != nGroupReqd))	// not single group
	{
		return FALSE;
	}

	{
		bool bOK = FALSE;
		bool bHasAlert = AccountRow.IsAlertSet();
		bool bIsExpired = AccountRow.HasUserExpired();
		bool bIsInactive = AccountRow.GetInactive();

		bool bIsUnissued = FALSE;

		if (System.GetInterfaceType() == nINTERFACE_MIFAREv2)					//27/03/2014 - v1.06b
		{
			bIsUnissued = (AccountRow.GetExternalRef1() == "") ? TRUE : FALSE;
		}

		if (GetValidReqd() == TRUE && (bHasAlert == FALSE && bIsInactive == FALSE && bIsExpired == FALSE && bIsUnissued == FALSE))
		{
			bOK = TRUE;
		}
		else if (GetUnissuedReqd() == TRUE && bIsUnissued == TRUE)
		{
			bOK = TRUE;
		}
		else if (GetInactiveReqd() == TRUE && bIsInactive == TRUE)
		{
			bOK = TRUE;
		}
		else if (GetAlertReqd() == TRUE && bHasAlert == TRUE)
		{
			bOK = TRUE;
		}
		else if (GetExpiredReqd() == TRUE && bIsExpired == TRUE)
		{
			bOK = TRUE;
		}

		if (FALSE == bOK)
		{
			return FALSE;
		}
	}

	{
		bool bOK = FALSE;

		switch (AccountRow.GetGroupShiftStatus())
		{
		case 1:
			bOK = (GetGroupShiftCurrentReqd());
			break;

		case 2:
			bOK = (GetGroupShiftFutureReqd());
			break;

		case 0:
		default:
			bOK = (GetGroupShiftNoneReqd());
			break;
		}

		if (FALSE == bOK)
		{
			return FALSE;
		}
	}

	switch (System.GetInterfaceType())
	{
	case nINTERFACE_BIOMETRIC:
	case nINTERFACE_DUAL:
	{
		int nConnectError = 0;

		switch (GetBioRegisterCheck())
		{
		case 1:
			if ( 0 == nConnectError )
			{		
				CSQLRepositoryBioRegister Repo;
				if ( Repo.RowExists(AccountRow.GetUserID(), NULL).GetSQLResult() == 0 )	
				{
					return FALSE;
				}
			}
			break;

		case 2:
			if (0 == nConnectError)
			{
				CSQLRepositoryBioRegister Repo;
				if (Repo.RowExists(AccountRow.GetUserID(), NULL).GetSQLResult() != 0)
				{
					return FALSE;
				}
			}
			break;
		}
	}
	break;
	}

	int nIndex = GetInfoFilterIndex();
	if (nIndex > 0)										// 0=don' tcare, info1-8
	{
		CString strFilterText = ::RemoveSpaces(GetInfoFilterText());
		if (strFilterText != "")
		{
			CString strInfoText = ::RemoveSpaces(AccountRow.GetInfo(nIndex));
			if (strFilterText.CompareNoCase(strInfoText) != 0)
			{
				return FALSE;
			}

		}
	}

	if (GetDietaryCheck() == TRUE)
	{
		CString strDietaryText = ::RemoveSpaces(AccountRow.GetDietary());
		if (strDietaryText == "")
		{
			return FALSE;
		}

		CString strFilterText = ::RemoveSpaces(GetDietaryText());
		strFilterText.MakeUpper();

		strDietaryText.MakeUpper();
		if (strDietaryText.Find(strFilterText) == -1)
		{
			return FALSE;
		}
	}

	if (GetCheckAllergies() == TRUE)
	{
		CAllergyText allergy;
		allergy.Read();
		allergy.SetAllergyCodes(GetAllergyCodes());

		if (allergy.MatchAllergies(AccountRow.GetAllergyCodes()) == 0)	// see how many allergies match
		{
			return FALSE;
		}
	}

	if (GetCheckAlertCode() == TRUE)
	{
		if (AccountRow.GetAlertCode() != GetAlertCode())
		{
			return FALSE;
		}
	}

	return TRUE;
}

//*******************************************************************

CString CDbExportHandler::MakeSortLabel ( const char* szText )
{
	CString strText = szText;
	CString strLabel = strText.Left(nMAX_SORTLABELENGTH);			// first 15 characters of text
	::AddLeading ( strLabel, nMAX_SORTLABELENGTH, ' ' );			// pad out to nMAX_SORTLABELENGTH
	return strLabel;
}

//**********************************************************************
// if UserID passed go get record otherwise use current record

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
	case nSORTONMEMBERID:
		strOrderBy += Account::MemberID.Label;
		break;

	case nSORTONUSERNAME:
		strOrderBy += Account::Username.Label;
		break;

	case nSORTONSURNAME:
		strOrderBy += Account::Surname.Label;
		break;

	case nSORTONEXTREF1:
		strOrderBy += Account::ExternalRef1.Label;
		break;

	case nSORTONEXTREF2:
		strOrderBy += Account::ExternalRef2.Label;
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

	case nSORTONINFO5:
		strOrderBy += Account::Info5.Label;
		break;

	case nSORTONINFO6:
		strOrderBy += Account::Info6.Label;
		break;

	case nSORTONINFO7:
		strOrderBy += Account::Info7.Label;
		break;

	case nSORTONINFO8:
		strOrderBy += Account::Info8.Label;
		break;

	case nSORTONUSERID:
	default:
		nSortOn = nSORTONUSERID;
		strOrderBy += Account::UserID.Label;
		break;
	}

	if (nSortOn != nSORTONUSERID)
	{
		strOrderBy += ",";
		strOrderBy += Account::UserID.Label;
	}

	return strOrderBy;
}

//**********************************************************************

CString CDbExportHandler::GetSortFieldText( CSQLRowAccountFull& AccountRow)
{
	CString strResult = "";

	switch (GetSortOn())
	{
	case nSORTONMEMBERID:
		strResult = AccountRow.GetMemberID();
		break;

	case nSORTONUSERNAME:
		strResult = AccountRow.GetUsername();
		break;

	case nSORTONSURNAME:
		strResult = AccountRow.GetSurname();
		break;

	case nSORTONEXTREF1:
		strResult = AccountRow.GetExternalRef1();
		break;

	case nSORTONEXTREF2:
		strResult = AccountRow.GetExternalRef2();
		break;

	case nSORTONINFO1:
		strResult = AccountRow.GetInfo1();
		break;

	case nSORTONINFO2:
		strResult = AccountRow.GetInfo2();
		break;

	case nSORTONINFO3:
		strResult = AccountRow.GetInfo3();
		break;

	case nSORTONINFO4:
		strResult = AccountRow.GetInfo4();
		break;

	case nSORTONINFO5:
		strResult = AccountRow.GetInfo5();
		break;

	case nSORTONINFO6:
		strResult = AccountRow.GetInfo6();
		break;

	case nSORTONINFO7:
		strResult = AccountRow.GetInfo7();
		break;

	case nSORTONINFO8:
		strResult = AccountRow.GetInfo8();
		break;
	}

	return strResult;
}

//**********************************************************************

void CDbExportHandler::GetSortItem(CSQLRowAccountFull& AccountRow, CSortedDbReportItem& item, bool bGotRow )
{
	item.m_nUserID = AccountRow.GetUserID();
	item.m_nGroupNo = AccountRow.GetGroupNo();

	if (FALSE == bGotRow)
	{
		CSQLRepositoryAccount RepoAccount;
		RepoAccount.SelectRow(AccountRow,NULL);
	}

	if (FALSE == GetSortByGroup())
	{
		item.m_nGroupNo = 0;
	}

	switch (GetSortOn())
	{
	case nSORTONMEMBERID:	item.m_strSortField = AccountRow.GetMemberID();		break;
	case nSORTONUSERNAME:	item.m_strSortField = AccountRow.GetUsername();		break;
	case nSORTONSURNAME:	item.m_strSortField = AccountRow.GetSurname();		break;
	case nSORTONEXTREF1:	item.m_strSortField = AccountRow.GetExternalRef1();	break;
	case nSORTONEXTREF2:	item.m_strSortField = AccountRow.GetExternalRef2();	break;
	case nSORTONINFO1:		item.m_strSortField = AccountRow.GetInfo1();		break;
	case nSORTONINFO2:		item.m_strSortField = AccountRow.GetInfo2();		break;
	case nSORTONINFO3:		item.m_strSortField = AccountRow.GetInfo3();		break;
	case nSORTONINFO4:		item.m_strSortField = AccountRow.GetInfo4();		break;
	case nSORTONINFO5:		item.m_strSortField = AccountRow.GetInfo5();		break;
	case nSORTONINFO6:		item.m_strSortField = AccountRow.GetInfo6();		break;
	case nSORTONINFO7:		item.m_strSortField = AccountRow.GetInfo7();		break;
	case nSORTONINFO8:		item.m_strSortField = AccountRow.GetInfo8();		break;
	default:				item.m_strSortField = "";
	}
}

//**********************************************************************

CString CDbExportHandler::CreateSortLabel(CSQLRowAccountFull& RowAccount)
{
	CString strSortLabel;

	switch (GetSortOn())
	{
	case nSORTONMEMBERID:	
		strSortLabel = MakeSortLabel(RowAccount.GetMemberID());
		break;

	case nSORTONUSERNAME:	
		strSortLabel = MakeSortLabel(RowAccount.GetUsername());	
		break;

	case nSORTONSURNAME:	
		strSortLabel = MakeSortLabel(RowAccount.GetSurname());	
		break;

	case nSORTONEXTREF1:		
		strSortLabel = MakeSortLabel(RowAccount.GetExternalRef1());	
		break;

	case nSORTONEXTREF2:	
		strSortLabel = MakeSortLabel(RowAccount.GetExternalRef2());	
		break;

	case nSORTONINFO1:	
		strSortLabel = MakeSortLabel(RowAccount.GetInfo1()); 
		break;

	case nSORTONINFO2:		
		strSortLabel = MakeSortLabel(RowAccount.GetInfo2()); 
		break;

	case nSORTONINFO3:	
		strSortLabel = MakeSortLabel(RowAccount.GetInfo3()); 
		break;

	case nSORTONINFO4:	
		strSortLabel = MakeSortLabel(RowAccount.GetInfo4()); 
		break;

	case nSORTONINFO5:		
		strSortLabel = MakeSortLabel(RowAccount.GetInfo5());
		break;

	case nSORTONINFO6:		
		strSortLabel = MakeSortLabel(RowAccount.GetInfo6());
		break;

	case nSORTONINFO7:		
		strSortLabel = MakeSortLabel(RowAccount.GetInfo7()); 
		break;

	case nSORTONINFO8:		
		strSortLabel = MakeSortLabel(RowAccount.GetInfo8());
		break;

	case nSORTONUSERID:
	default:
		strSortLabel = System.FormatCardNo(RowAccount.GetUserIDString());
		break;
	}

	strSortLabel.MakeUpper();

	if (GetSortByGroup() == TRUE)
	{
		strSortLabel = ::FormatGroupNo(RowAccount.GetGroupNo()) + strSortLabel;
	}

	if (GetSortOn() != nSORTONUSERID)
	{
		strSortLabel = strSortLabel + System.FormatCardNo(RowAccount.GetUserIDString());	// add card UserID to end of sort label {GroupNo}{SortText}{UserID}
	}

	return strSortLabel;
}
	

//******************************************************************

bool CDbExportHandler::IsCardInRange ( const char* szCardNo )
{
	if (GetCardRangeIndex() == 1)
	{
		return m_cardPicker.IsValid(szCardNo, Filenames.GetReportCardListFilename(m_strLabel));
	}

	__int64 nCardNo	= _atoi64 ( szCardNo );
	__int64 nCard1	= GetStartCardNoInt();
	__int64 nCard2	= GetEndCardNoInt();

	if (nCardNo < nCard1 || nCardNo > nCard2)
	{
		return FALSE;
	}

	return TRUE;
}

//*******************************************************************

CString CDbExportHandler::GetSQLConditionalField(int n)
{
	CString strField = (2 == n) ? GetField2() : GetField1();
	return DbExportHelpers.GetExportConditionField(strField);
}

//*******************************************************************

double CDbExportHandler::GetValueByFieldName(CString strField, CSQLRowAccountFull& RowAccount)
{
	if (strField == szPURSE1_BALANCE)			return RowAccount.GetPurse1Balance();
	else if (strField == szPURSE2_BALANCE)		return RowAccount.GetPurse2Balance();
	else if (strField == szPURSE3_BALANCE)		return RowAccount.GetPurse3Balance();
	else if (strField == szPURSE1_CREDIT)		return RowAccount.GetPurse1Credit();
	else if (strField == szPURSE1_LIABILITY)	return RowAccount.GetPurse1Liability();
	else if (strField == szPURSE3_CREDIT)		return RowAccount.GetPurse3Credit();
	else if (strField == szPURSE3_LIABILITY)	return RowAccount.GetPurse3Liability();
	else if (strField == szPOINTS)				return RowAccount.GetPoints();
	else if (strField == szCASHSPENDTD)			return RowAccount.GetCashSpendToDate();
	else if (strField == szPURSE1SPENDTD)		return RowAccount.GetPurse1SpendToDate();
	else if (strField == szPURSE2SPENDTD)		return RowAccount.GetPurse2SpendToDate();
	else if (strField == szPURSE3SPENDTD)		return RowAccount.GetPurse3SpendToDate();
	else if (strField == szTOTALSPENDTD)		return RowAccount.GetTotalSpendToDate();
	else if (strField == szPOINTSTD)			return RowAccount.GetPointsToDate();
	else return 0.0;
}

//*******************************************************************
