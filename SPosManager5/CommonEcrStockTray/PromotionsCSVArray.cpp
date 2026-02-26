/**********************************************************************/
#include "AnalysisCategoryCSVArray.h"
#include "SPOSVersions.h"
#include "HexBitArray.h"
#include "NetworkCSVArray.h"
/**********************************************************************/
#include "PromotionsCSVArray.h"
/**********************************************************************/

CPromotionsCSVRecord::CPromotionsCSVRecord()
{
	ClearRecord();
	m_bServiceChargeVATFixBeforeEditing = FALSE;
}

/**********************************************************************/

void CPromotionsCSVRecord::ClearRecord()
{
	m_nPromoNo = 1;
	m_strDescription = "";
	m_nPromoType = 0;
	m_nFlexType = 0;
	m_dPercent = 0.0;
	m_dMarkdown = 0.0;
	m_dTrip = 0.0;
	m_bGratuity = FALSE;
	m_bSPOSApplyTax = TRUE;
	m_nUsage = 1;
	m_nDeptNoEpos = 0;
	m_bMyPromoOnPromo = FALSE;
	m_bPromoOnMyPromo = FALSE;
	m_nTimeplanMode = 0;
	m_nUseGate = 0;
	m_nFilterType = 1;
	m_nFilterOfferGroup = 0;
	m_strFilterAnalysis = "";
	m_bServiceChargeVATFix = FALSE;
	m_bDiscountAlwaysVATable = FALSE;
	m_bUpTo = FALSE;
	m_strTimeplanList = "";
	
	m_nReportAsPluType = 1;
	m_nDeptNoReport = 0;
	m_bReportAsZeroQty = FALSE;
	m_nSageNominalCode = 0;
	m_bReasons = FALSE;

	for ( int n = 0; n < 4; n++ )
	{
		m_nFlexBoundCovers[n] = 0;
		m_dFlexBoundValue[n] = 0.0;
	}

	for (int n = 0; n < 5; n++)
	{
		m_dFlexPercent[n] = 0.0;
	}
}

/**********************************************************************/

void CPromotionsCSVRecord::ConvertFromCSV(CCSV& csv)
{
	ClearRecord();

	switch (csv.GetInt(0))
	{
	case 1:
		V1to3ConvertFromCSV(csv, 1);
		break;

	case 2:
		V1to3ConvertFromCSV(csv, 2);
		break;

	case 3:	
		V1to3ConvertFromCSV(csv, 3);
		break;

	case 4:	
		V4to6ConvertFromCSV(csv, 4);
		break;

	case 5:	
		V4to6ConvertFromCSV(csv, 5);
		break;

	case 6:	
		V4to6ConvertFromCSV(csv, 6);
		break;

	case 7:	
		V7to11ConvertFromCSV(csv, 7);
		break;

	case 8:	
		V7to11ConvertFromCSV(csv, 8);
		break;

	case 9:	
		V7to11ConvertFromCSV(csv, 9);
		break;

	case 10:
		V7to11ConvertFromCSV(csv, 10);		
		break;

	case 11:
		V7to11ConvertFromCSV(csv, 11);
		break;
	}
}

/**********************************************************************/

void CPromotionsCSVRecord::V1to3ConvertFromCSV ( CCSV& csv, int nVer )
{
	SetPromoNo( csv.GetInt(1) );
	SetDescription( csv.GetString(2) );
	SetPromoType( csv.GetInt(3) );
	SetPercent( csv.GetDouble(4) );
	SetMarkdown( csv.GetDouble(5) );
	SetTrip( csv.GetDouble(6) );
	SetGratuityFlag( csv.GetBool(7) );
	SetSPOSApplyTaxFlag( csv.GetBool(8) );
	SetUsage( csv.GetInt(9) );
	SetDeptNoEpos( csv.GetInt(10) );
	SetLegacyAnalysis( csv.GetInt(11) );
	SetMyPromoOnPromoFlag( csv.GetBool(12) );
	SetPromoOnMyPromoFlag( csv.GetBool(13) );
	SetTimeplanMode( csv.GetInt(14) );
	SetUseGate( csv.GetInt(15) );
	
	int nOffset = 16;

	if ( nVer >= 3 )
	{
		SetFilterType( csv.GetInt( nOffset++ ) );
		SetFilterOfferGroup( csv.GetInt( nOffset++ ) );
	}

	if ( nVer >= 2 )
	{
		SetReportAsPluType( csv.GetBool( nOffset++) ? 0 : 1 );
		SetDeptNoReport( csv.GetInt(nOffset++) );
		SetReportAsZeroQtyFlag( csv.GetBool( nOffset++) );
	}
	
	SetSageNominalCode( csv.GetInt(nOffset++) );
}

/**********************************************************************/

void CPromotionsCSVRecord::V4to6ConvertFromCSV ( CCSV& csv, int nVer )
{
	SetPromoNo( csv.GetInt(1) );
	SetDescription( csv.GetString(2) );
	SetPromoType( csv.GetInt(3) );

	int nOffset = 4;
	if (nVer >= 5)
	{
		SetFlexType(csv.GetInt(nOffset++));
	}

	SetPercent( csv.GetDouble(nOffset++) );
	SetMarkdown( csv.GetDouble(nOffset++) );
	SetTrip( csv.GetDouble(nOffset++) );
	SetGratuityFlag( csv.GetBool(nOffset++) );
	SetSPOSApplyTaxFlag( csv.GetBool(nOffset++) );
	SetUsage( csv.GetInt(nOffset++) );
	SetDeptNoEpos( csv.GetInt(nOffset++) );
	SetMyPromoOnPromoFlag( csv.GetBool(nOffset++) );
	SetPromoOnMyPromoFlag( csv.GetBool(nOffset++) );
	SetTimeplanMode( csv.GetInt(nOffset++) );
	SetUseGate( csv.GetInt(nOffset++) );
	SetFilterType( csv.GetInt(nOffset++) );
	SetFilterOfferGroup( csv.GetInt(nOffset++) );
	SetFilterAnalysis( csv.GetString(nOffset++) );

	SetReportAsPluType( csv.GetBool(nOffset++) ? 0 : 1 );
	SetDeptNoReport( csv.GetInt(nOffset++) );
	SetReportAsZeroQtyFlag( csv.GetBool(nOffset++) );
	SetSageNominalCode( csv.GetInt(nOffset++) );

	if (nVer >= 6)
	{
		SetReasonsFlag(csv.GetBool(nOffset++));
	}

	if ( nVer >= 5 )
	{
		for (int n = 1; n <= 4; n++)
		{
			SetFlexBoundCovers(n, csv.GetInt(nOffset++));
		}

		for (int n = 1; n <= 4; n++)
		{
			SetFlexBoundValue(n, csv.GetDouble(nOffset++));
		}

		for (int n = 0; n <= 4; n++)
		{
			SetFlexPercent(n, csv.GetDouble(nOffset++));
		}
	}
}

/**********************************************************************/

void CPromotionsCSVRecord::V7to11ConvertFromCSV ( CCSV& csv, int nVer )
{
	SetPromoNo( csv.GetInt(1) );
	SetDescription( csv.GetString(2) );
	SetPromoType( csv.GetInt(3) );
	SetFlexType( csv.GetInt(4) );
	SetPercent( csv.GetDouble(5) );
	SetMarkdown( csv.GetDouble(6) );
	SetTrip( csv.GetDouble(7) );
	SetGratuityFlag( csv.GetBool(8) );
	SetSPOSApplyTaxFlag( csv.GetBool(9) );
	SetUsage( csv.GetInt(10) );
	SetDeptNoEpos( csv.GetInt(11) );
	SetMyPromoOnPromoFlag( csv.GetBool(12) );
	SetPromoOnMyPromoFlag( csv.GetBool(13) );
	SetTimeplanMode( csv.GetInt(14) );
	SetUseGate( csv.GetInt(15) );
	SetFilterType( csv.GetInt(16) );
	SetFilterOfferGroup( csv.GetInt(17) );
	SetFilterAnalysis( csv.GetString(18) );
	SetReportAsPluType( csv.GetInt(19) );
	SetDeptNoReport( csv.GetInt(20) );
	SetReportAsZeroQtyFlag( csv.GetBool(21) );
	SetSageNominalCode( csv.GetInt(22) );
	SetReasonsFlag( csv.GetBool(23) );

	int nOffset = 24;

	if (nVer >= 8)
	{
		SetServiceChargeVATFixFlag(csv.GetBool(nOffset++));
	}

	if (nVer >= 9)
	{
		SetDiscountAlwaysVATableFlag(csv.GetBool(nOffset++));
	}

	if (nVer >= 10)
	{
		SetUpToFlag(csv.GetBool(nOffset++));
	}

	if ( nVer >= 11 )
	{
		SetTimeplanList(csv.GetString(nOffset++));
	}

	for (int n = 1; n <= 4; n++)
	{
		SetFlexBoundCovers(n, csv.GetInt(nOffset++));
	}

	for (int n = 1; n <= 4; n++)
	{
		SetFlexBoundValue(n, csv.GetDouble(nOffset++));
	}

	for (int n = 0; n <= 4; n++)
	{
		SetFlexPercent(n, csv.GetDouble(nOffset++));
	}
}

/**********************************************************************/

void CPromotionsCSVRecord::ConvertToCSV(CCSV& csv)
{
	//MUST UPDATE GETPROMONO() IF ADDING NEW VERSION
	csv.Add(PROMOTIONS_VERSION);
	csv.Add(m_nPromoNo);
	csv.Add(m_strDescription);
	csv.Add(m_nPromoType);
	csv.Add(m_nFlexType);
	csv.Add(m_dPercent, 2);
	csv.Add(m_dMarkdown, 2);
	csv.Add(m_dTrip, 2);
	csv.Add(m_bGratuity);
	csv.Add(m_bSPOSApplyTax);
	csv.Add(m_nUsage);
	csv.Add(m_nDeptNoEpos);
	csv.Add(m_bMyPromoOnPromo);
	csv.Add(m_bPromoOnMyPromo);
	csv.Add(m_nTimeplanMode);
	csv.Add(m_nUseGate);
	csv.Add(m_nFilterType);
	csv.Add(m_nFilterOfferGroup);
	csv.Add(m_strFilterAnalysis);
	csv.Add(m_nReportAsPluType);
	csv.Add(m_nDeptNoReport);
	csv.Add(m_bReportAsZeroQty);
	csv.Add(m_nSageNominalCode);
	csv.Add(m_bReasons);
	csv.Add(m_bServiceChargeVATFix);
	csv.Add(m_bDiscountAlwaysVATable);
	csv.Add(m_bUpTo);
	csv.Add(m_strTimeplanList);

	for (int n = 0; n < 4; n++)
	{
		csv.Add(m_nFlexBoundCovers[n]);
	}

	for (int n = 0; n < 4; n++)
	{
		csv.Add(m_dFlexBoundValue[n], 2);
	}

	for (int n = 0; n < 5; n++)
	{
		csv.Add(m_dFlexPercent[n], 2);
	}
}

/**********************************************************************/

const char* CPromotionsCSVRecord::GetDisplayName()
{
	m_strDisplayName = m_strDescription;
	::TrimSpaces( m_strDisplayName, FALSE );
	
	if (m_strDisplayName == "")
	{
		m_strDisplayName.Format("Promo %d", m_nPromoNo);
	}

	return m_strDisplayName;
}

/**********************************************************************/

const char* CPromotionsCSVRecord::GetPromoTypeName()
{
	switch( m_nPromoType )
	{
	case PROMOTION_TYPE_DISCOUNT:
		m_strPromoTypeName = "Discount";
		break;

	case PROMOTION_TYPE_SURCHARGE:
		m_strPromoTypeName = "Surcharge";
		break;

	case PROMOTION_TYPE_MARKDOWN:		
		m_strPromoTypeName = "Markdown";		
		break;

	default:							
		m_strPromoTypeName = "Unknown";
		break;
	}
	return m_strPromoTypeName;
}

/**********************************************************************/

const char* CPromotionsCSVRecord::GetFlexTypeName()
{
	switch( m_nFlexType )
	{
	case 0:
		m_strFlexTypeName = "Fixed";
		break;

	case 1:
		m_strFlexTypeName = "By Value";
		break;

	case 2:
		m_strFlexTypeName = "By Covers";	
		break;

	default:
		m_strFlexTypeName = "Unknown";
		break;
	}
	return m_strFlexTypeName;
}

/**********************************************************************/

void CPromotionsCSVRecord::SetPromoNo( int n )
{
	Set( m_nPromoNo, n, Promotions::PromoNo );
}

/**********************************************************************/

void CPromotionsCSVRecord::SetDescription( const char* sz )
{
	CString str = sz;
	::TrimSpaces( str, FALSE );
	Set( m_strDescription, str, Promotions::Description );	
}

/**********************************************************************/

void CPromotionsCSVRecord::SetPromoType( int n )
{
	Set( m_nPromoType, n, Promotions::PromoType );
}

/**********************************************************************/

void CPromotionsCSVRecord::SetFlexType( int n )
{
	Set( m_nFlexType, n, Promotions::FlexType );
}

/**********************************************************************/

void CPromotionsCSVRecord::SetPercent( double d )
{
	Set( m_dPercent, d, Promotions::Percent );
}

/**********************************************************************/

void CPromotionsCSVRecord::SetMarkdown( double d )
{
	Set( m_dMarkdown, d, Promotions::Markdown );
}

/**********************************************************************/

void CPromotionsCSVRecord::SetTrip( double d )
{
	Set( m_dTrip, d, Promotions::Trip );
}

/**********************************************************************/

void CPromotionsCSVRecord::SetGratuityFlag( bool b )
{
	m_bGratuity = b;
}

/**********************************************************************/

void CPromotionsCSVRecord::SetSPOSApplyTaxFlag( bool b )
{
	m_bSPOSApplyTax = b;
}

/**********************************************************************/

void CPromotionsCSVRecord::SetUsage( int n )
{
	Set( m_nUsage, n, Promotions::Usage );
}

/**********************************************************************/

void CPromotionsCSVRecord::SetDeptNoEpos( int n )
{
	Set( m_nDeptNoEpos, n, Promotions::DeptNoEpos );
}

/**********************************************************************/

void CPromotionsCSVRecord::SetMyPromoOnPromoFlag( bool b )
{
	m_bMyPromoOnPromo = b;
}

/**********************************************************************/

void CPromotionsCSVRecord::SetPromoOnMyPromoFlag( bool b )
{
	m_bPromoOnMyPromo = b;
}

/**********************************************************************/

void CPromotionsCSVRecord::SetTimeplanMode( int n )
{
	Set( m_nTimeplanMode, n, Promotions::TimeplanMode );
}

/**********************************************************************/

void CPromotionsCSVRecord::SetUseGate( int n )
{
	Set( m_nUseGate, n, Promotions::UseGate );
}

/**********************************************************************/

void CPromotionsCSVRecord::SetFilterType( int n )
{
	Set( m_nFilterType, n, Promotions::FilterType );
}

/**********************************************************************/

void CPromotionsCSVRecord::SetFilterOfferGroup( int n )
{
	Set( m_nFilterOfferGroup, n, Promotions::FilterOfferGroup );
}

/**********************************************************************/

void CPromotionsCSVRecord::SetFilterAnalysis( const char* sz )
{
	Set( m_strFilterAnalysis, sz, Promotions::FilterAnalysis );
}

/**********************************************************************/

void CPromotionsCSVRecord::SetLegacyAnalysis( int n )
{
	int nFlags = 0;
	if ( ( n & 0x001 ) != 0 )	nFlags += 0x400;
	if ( ( n & 0x002 ) != 0 )	nFlags += 0x200;
	if ( ( n & 0x004 ) != 0 )	nFlags += 0x100;
	if ( ( n & 0x008 ) != 0 )	nFlags += 0x080;
	if ( ( n & 0x010 ) != 0 )	nFlags += 0x040;
	if ( ( n & 0x020 ) != 0 )	nFlags += 0x020;
	if ( ( n & 0x040 ) != 0 )	nFlags += 0x010;
	if ( ( n & 0x080 ) != 0 )	nFlags += 0x008;
	if ( ( n & 0x100 ) != 0 )	nFlags += 0x004;
	m_strFilterAnalysis.Format( "%X", nFlags );
}

/**********************************************************************/

void CPromotionsCSVRecord::SetReportAsPluType( int n )
{
	Set( m_nReportAsPluType, n, Promotions::ReportAsPluType );
}

/**********************************************************************/

void CPromotionsCSVRecord::SetDeptNoReport( int n )
{
	Set( m_nDeptNoReport, n, Promotions::DeptNoReport );
}

/**********************************************************************/

void CPromotionsCSVRecord::SetReportAsZeroQtyFlag( bool b )
{
	m_bReportAsZeroQty = b;
}

/**********************************************************************/

void CPromotionsCSVRecord::SetSageNominalCode( int n )
{
	Set( m_nSageNominalCode, n, Promotions::SageNominalCode );
}

/**********************************************************************/

void CPromotionsCSVRecord::SetReasonsFlag( bool b )
{
	m_bReasons = b;
}

/**********************************************************************/

void CPromotionsCSVRecord::SetFlexBoundCovers( int nBand, int n )
{
	if ( ( nBand >= 1 ) && ( nBand <= 4 ) )
		Set( m_nFlexBoundCovers[nBand-1], n, Promotions::FlexBoundCovers );
}

/**********************************************************************/

void CPromotionsCSVRecord::SetFlexBoundValue( int nBand, double d )
{
	if ( ( nBand >= 1 ) && ( nBand <= 4 ) )
		Set( m_dFlexBoundValue[nBand-1], d, Promotions::FlexBoundValue );
}

/**********************************************************************/

void CPromotionsCSVRecord::SetFlexPercent( int nBand, double d )
{
	if ( ( nBand >= 0 ) && ( nBand <= 4 ) )
		m_dFlexPercent[nBand] = d;
}

/**********************************************************************/

void CPromotionsCSVRecord::SetTimeplanList(const char* sz)
{
	CString str = sz;
	::TrimSpaces(str, FALSE);
	Set(m_strTimeplanList, str, Promotions::TimeplanList);
}

/**********************************************************************/

bool CPromotionsCSVRecord::GetLegacyAnalysis(int& nFlags)
{
	nFlags = 0;

	CString strFilter = m_strFilterAnalysis;
	if (strFilter.GetLength() > 3)
	{
		return FALSE;
	}

	if (::TestNumericHex(strFilter) == FALSE)
	{
		return FALSE;
	}

	::AddTrailing(strFilter, 3, '0');

	int nTemp = ::HexToInt(strFilter);

	if ((nTemp & 3) != 0)
	{
		return FALSE;
	}

	if ((nTemp & 0x400) != 0) nFlags += 0x001;
	if ((nTemp & 0x200) != 0) nFlags += 0x002;
	if ((nTemp & 0x100) != 0) nFlags += 0x004;
	if ((nTemp & 0x080) != 0) nFlags += 0x008;
	if ((nTemp & 0x040) != 0) nFlags += 0x010;
	if ((nTemp & 0x020) != 0) nFlags += 0x020;
	if ((nTemp & 0x010) != 0) nFlags += 0x040;
	if ((nTemp & 0x008) != 0) nFlags += 0x080;
	if ((nTemp & 0x004) != 0) nFlags += 0x100;

	return TRUE;
}

/**********************************************************************/

const char* CPromotionsCSVRecord::GetAnalysisList(bool bForDownload)
{
	m_strAnalysisList = "";

	CHexBitArray HexBitArray(100);
	HexBitArray.TranslateFromHexString(m_strFilterAnalysis);

	for (int n = 1; n <= MAX_ACAT_NO; n++)
	{
		if (HexBitArray.GetBit(n) == TRUE)
		{
			if ((FALSE == bForDownload) && (m_strAnalysisList != ""))
			{
				m_strAnalysisList += ",";
			}

			CString str = "";
			str.Format("%d", n);
			m_strAnalysisList += str;

			if (TRUE == bForDownload)
			{
				m_strAnalysisList += ";";
			}
		}
	}

	return m_strAnalysisList;
}

/**********************************************************************/

int CPromotionsCSVRecord::GetFlexBoundCovers(int nBand)
{
	if ((nBand >= 1) && (nBand <= 4))
	{
		return m_nFlexBoundCovers[nBand - 1];
	}
	else
	{
		return 0;
	}
}

/**********************************************************************/

double CPromotionsCSVRecord::GetFlexBoundValue(int nBand)
{
	if ((nBand >= 1) && (nBand <= 4))
	{
		return m_dFlexBoundValue[nBand - 1];
	}
	else
	{
		return 0.0;
	}
}

/**********************************************************************/

double CPromotionsCSVRecord::GetFlexPercent(int nBand)
{
	if ((nBand >= 0) && (nBand <= 4))
	{
		return m_dFlexPercent[nBand];
	}
	else
	{
		return 0.0;
	}
}

/**********************************************************************/

void CPromotionsCSVRecord::ValidateFlexSettings()
{
	bool bEnableValueType = TRUE;
	bEnableValueType &= (m_nPromoType != 2);
	bEnableValueType &= ((2 == m_nUsage) || (6 == m_nUsage));

	if (FALSE == bEnableValueType)
	{
		m_nFlexType = 0;
	}
}

/**********************************************************************/

void CPromotionsCSVRecord::ValidateUpToFlag()
{
	if (TRUE == m_bUpTo)
	{
		if (GetPromoType() != 2)
		{
			SetUpToFlag(FALSE);
		}
		else
		{
			SetUsage(6);
		}
	}
}

/**********************************************************************/

void CPromotionsCSVRecord::CheckMinimumSPOSVersion( CMinimumSPOSVersion& version, int& nLegacyCats )
{
	version.m_nMinVer = SPOS_V4;
	version.m_strReason = "";
	version.m_bInvalidData = FALSE;

	switch( m_nFlexType )
	{
	case 0:
		break;

	case 1:
		version.m_nMinVer = SPOS_V4_2161;
		version.m_strReason = "Flexible Promo by Value";
		return;

	case 2:
		version.m_nMinVer = SPOS_V4_2161;
		version.m_strReason = "Flexible Promo by Covers";
		return;
			
	default:
		version.m_bInvalidData = TRUE;
		version.m_strReason = "Invalid Flexible Promo type";
		return;
	}

	nLegacyCats = 0;		
	if ( GetLegacyAnalysis( nLegacyCats ) == FALSE )
	{
		version.m_nMinVer = SPOS_V4_2133;
		version.m_strReason = "Uses Analysis Categories 10 - 99";
		return;
	}

	switch( m_nUsage )
	{
	case 1:
	case 2:
	case 3:
		break;

	case 6:
		version.m_nMinVer = SPOS_V4_1392;
		version.m_strReason = "Deferred Subtotal Usage";
		return;
			
	default:
		version.m_bInvalidData = TRUE;
		version.m_strReason = "Invalid usage type";
		return;
	}
}

/**********************************************************************/

void CPromotionsCSVRecord::SetServiceChargeVATFixFields()
{
	if (TRUE == m_bServiceChargeVATFix)
	{
		m_bSPOSApplyTax = TRUE;
		m_nPromoType = 1;
		m_nUsage = 2;
		m_nReportAsPluType = 0;
	}
}

/**********************************************************************/

void CPromotionsCSVRecord::GetTimeplanMap(CReportConsolidationArray<CSortedIntByInt>& TimeplanMap)
{
	DataManager.TimePlanTemplateEntry.GetTimeplanMap(m_strTimeplanList, TimeplanMap);
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CPromotionsCSVArray::CPromotionsCSVArray() 
{
}

/**********************************************************************/

CPromotionsCSVArray::~CPromotionsCSVArray() 
{
}

/**********************************************************************/

int CPromotionsCSVArray::GetPromoNoByIndex( int nPromoIdx )
{
	int nPromoNo = 0;

	CString strTemp = "";
	if ( ( strTemp = GetAt ( nPromoIdx ) ) != "" )
	{
		CCSV csv ( strTemp, ',', '"', 2 );
		
		switch( csv.GetInt(0) )
		{
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
		case 10:
		case 11:
			nPromoNo = csv.GetInt(1);
			break;
		}
	}
	
	return nPromoNo;
}

/**********************************************************************/

bool CPromotionsCSVArray::FindTableByNumber(int nTableNo, int& nTableIdx)
{
	int nStart = 0;
	int nEnd = GetSize() - 1;

	while (nStart <= nEnd)
	{
		nTableIdx = (nStart + nEnd) / 2;

		CPromotionsCSVRecord Promo;
		GetAt(nTableIdx, Promo);
		int nArrayNo = Promo.GetPromoNo();

		if (nTableNo == nArrayNo)
		{
			return TRUE;
		}

		if (nTableNo < nArrayNo)
		{
			nEnd = nTableIdx - 1;
		}
		else
		{
			nStart = nTableIdx + 1;
		}
	}

	nTableIdx = nStart;
	return FALSE;
}

/**********************************************************************/

int CPromotionsCSVArray::FindFirstFreeNumber()
{
	int nResult = 0;

	CPromotionsCSVRecord Promo;
	for (int n = 0; n < GetSize(); n++)
	{
		GetAt(n, Promo);
		if (Promo.GetPromoNo() > n + 1)
		{
			nResult = n + 1;
			break;
		}
	}

	if (nResult == 0)
	{
		nResult = GetSize() + 1;
	}

	if (nResult < Promotions::PromoNo.Min || nResult > Promotions::PromoNo.Max)
	{
		nResult = 0;
	}

	return nResult;
}

/**********************************************************************/

int CPromotionsCSVArray::Open(const char* szFilename, int nMode)
{
	RemoveAt(0, GetSize());

	int nReply = CSharedCSVArray::Open(szFilename, nMode);

	if (nReply != DB_ERR_SHARE)
	{
		if (nMode == DB_READONLY)	// if file does not exist use default values
		{
			nReply = DB_ERR_NONE;
		}
	}

	int nLastPromoNo = 0;
	int nValidRecords = 0;

	CPromotionsCSVRecord Promo;
	for (; nValidRecords < GetSize(); nValidRecords++)
	{
		GetAt(nValidRecords, Promo);

		int nPromoNo = Promo.GetPromoNo();
		if (nPromoNo <= nLastPromoNo || nPromoNo > Promotions::PromoNo.Max)
		{
			break;
		}

		nLastPromoNo = nPromoNo;
	}

	while (GetSize() > nValidRecords)
	{
		RemoveAt(GetSize() - 1);
	}

	return nReply;
}

/**********************************************************************/

void CPromotionsCSVArray::GetComboTexts( int nPromoNo, CString& strNum, CString& strName )
{
	strNum.Format( "P%2.2d", nPromoNo );

	int nPromoIdx;
	if ( DataManager.Promotion.FindTableByNumber( nPromoNo, nPromoIdx ) == TRUE )
	{
		CPromotionsCSVRecord Promo;
		DataManager.Promotion.GetAt( nPromoIdx, Promo );
		strName = Promo.GetDisplayName();		
	}
	else
	{
		strName = "Unknown";
	}
}

/**********************************************************************/

const char* CPromotionsCSVArray::GetTabbedComboText( int nPromoNo )
{
	CString strNum, strName;
	GetComboTexts( nPromoNo, strNum, strName );

	CCSV csv;
	csv.Add( strNum );
	csv.Add( strName );
	m_strTabbedComboText = csv.GetLine();

	return m_strTabbedComboText;
}

/**********************************************************************/

const char* CPromotionsCSVArray::GetSimpleListText( int nPromoNo )
{
	CString strNum, strName;
	GetComboTexts( nPromoNo, strNum, strName );

	m_strSimpleListText = "";
	m_strSimpleListText += strNum;
	m_strSimpleListText += "  ";
	m_strSimpleListText += strName;

	return m_strSimpleListText;
}

/**********************************************************************/

