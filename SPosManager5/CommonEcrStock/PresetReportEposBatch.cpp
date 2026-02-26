/**********************************************************************/
#include "ReportTypes.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PresetReportEposBatch.h"
/**********************************************************************/

CPresetReportInfoEposBatch::CPresetReportInfoEposBatch()
{
	Reset();	
}

/**********************************************************************/

void CPresetReportInfoEposBatch::Reset()
{
	m_bReportByTerminal = FALSE; 
	m_bReportByLocation = FALSE; 
	m_bReportAllLocations = FALSE; 
	m_nActionType = 0;
	m_arrayReports.RemoveAll();
}

/**********************************************************************/

void CPresetReportInfoEposBatch::ToString( CString& str )
{
	int nJobCount = m_arrayReports.GetSize();

	CCSV csv;

	csv.Add( m_bReportByTerminal ); 
	csv.Add( m_bReportByLocation ); 
	csv.Add( m_bReportAllLocations ); 
	csv.Add( m_nActionType );
	csv.Add( nJobCount );
	
	for ( int nIndex = 0; nIndex < nJobCount; nIndex++ )
	{
		CSortedIntItem item;
		m_arrayReports.GetAt( nIndex, item );
		csv.Add( item.m_nItem );
	}

	str = csv.GetLine();
}

/**********************************************************************/

void CPresetReportInfoEposBatch::FromString(CString& str)
{
	Reset();
	CCSV csv(str);

	int nPos = 0;

	m_bReportByTerminal = csv.GetBool(nPos++);
	m_bReportByLocation = csv.GetBool(nPos++);
	m_bReportAllLocations = csv.GetBool(nPos++);

	SetActionType(csv.GetInt(nPos++));

	int nJobCount = csv.GetInt(nPos++);

	if (nJobCount > 200)
	{
		nJobCount = 200;
	}

	for (int nIndex = 0; nIndex < nJobCount; nIndex++)
	{
		SetReportFlag(csv.GetInt(nPos++), TRUE);
	}
}

/**********************************************************************/

void CPresetReportInfoEposBatch::ConvertFromCSV ( CCSV& csv, CPresetReportInfo& infoBase )
{
	Reset();
	infoBase.Reset();
	infoBase.SetName ( csv.GetString (0) );
	
	switch ( csv.GetInt(1) )
	{
	case 1:	V1To3ConvertFromCSV ( csv, infoBase, 1 );	break;
	case 2:	V1To3ConvertFromCSV ( csv, infoBase, 2 );	break;
	case 3:	V1To3ConvertFromCSV ( csv, infoBase, 3 );	break;
	}
}

/**********************************************************************/

void CPresetReportInfoEposBatch::V1To3ConvertFromCSV(CCSV& csv, CPresetReportInfo& infoBase, int nVer)
{
	int nPos = 2;

	if (nVer >= 2)
	{
		infoBase.SetEntityNo(csv.GetInt(nPos++));
	}

	m_bReportByTerminal = csv.GetBool(nPos++);
	m_bReportByLocation = csv.GetBool(nPos++);
	m_bReportAllLocations = csv.GetBool(nPos++);

	if (nVer >= 3)
	{
		SetActionType(csv.GetInt(nPos++));
	}

	int nJobCount = csv.GetInt(nPos++);

	if (nJobCount > 200)
	{
		nJobCount = 200;
	}

	for (int nIndex = 0; nIndex < nJobCount; nIndex++)
	{
		SetReportFlag(csv.GetInt(nPos++), TRUE);
	}
}

/**********************************************************************/

void CPresetReportInfoEposBatch::ConvertToCSV(CCSV& csv, CPresetReportInfo& infoBase)
{
	csv.Add(infoBase.GetName());
	csv.Add(PRESET_EPOS_BATCH_VERSION);
	csv.Add(infoBase.GetEntityNo());
	csv.Add(m_bReportByTerminal);
	csv.Add(m_bReportByLocation);
	csv.Add(m_bReportAllLocations);
	csv.Add(m_nActionType);
	csv.Add(m_arrayReports.GetSize());

	for (int n = 0; n < m_arrayReports.GetSize(); n++)
	{
		CSortedIntItem item;
		m_arrayReports.GetAt(n, item);
		csv.Add(item.m_nItem);
	}
}

/**********************************************************************/

bool CPresetReportInfoEposBatch::GetReportFlag( int nReportType )
{
	CSortedIntItem item;
	item.m_nItem = nReportType;

	int nPos;
	return ( m_arrayReports.Find( item, nPos ) );
}

/**********************************************************************/
				
void CPresetReportInfoEposBatch::SetReportFlag( int nReportType, bool bFlag )
{
	CSortedIntItem item;
	item.m_nItem = nReportType;

	int nPos;
	bool bExists = ( m_arrayReports.Find( item, nPos ) );

	if (TRUE == bFlag)
	{
		if (FALSE == bExists)
		{
			m_arrayReports.InsertAt(nPos, item);
		}
	}
	else
	{
		if (TRUE == bExists)
		{
			m_arrayReports.RemoveAt(nPos);
		}
	}
}

/**********************************************************************/

void CPresetReportInfoEposBatch::SetActionType( int n )
{
	if ((n >= 0) && (n <= 2))
	{
		m_nActionType = n;
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CPresetReportArrayEposBatch::CPresetReportArrayEposBatch(void) : CPresetReportArray()
{
}

/**********************************************************************/

CPresetReportArrayEposBatch::~CPresetReportArrayEposBatch(void)
{
}

/**********************************************************************/

const char* CPresetReportArrayEposBatch::GetFilename()
{
	CFilenameUpdater FnUp ( SysFiles::PresetRepEposBatch );
	m_strFilename = FnUp.GetFilenameToUse();
	return m_strFilename;
}

/**********************************************************************/

void CPresetReportArrayEposBatch::PrepareForEditingInternal()
{
	m_arrayPresets.RemoveAll();

	for ( int n = 0; n < m_arrayFileStrings.GetSize(); n++ )
	{
		CCSV csv( m_arrayFileStrings.GetAt(n) );

		CPresetReportInfo infoBase;
		CPresetReportInfoEposBatch infoPreset;
		infoPreset.ConvertFromCSV( csv, infoBase );
		
		CString strPreset;
		infoPreset.ToString( strPreset );
		infoBase.SetOtherFields( strPreset );
		m_arrayPresets.Add( infoBase );
	}

	m_arrayFileStrings.RemoveAt(0, m_arrayFileStrings.GetSize() );

	if ( GetSize() == 0 )
	{
		bool bModifier = ( SysInfo.GetMaxBasePluLen() != 0 );

		CArray<int,int> arrayReports;
		
		arrayReports.Add( REPORT_TYPE_PLUSALES );
		arrayReports.Add( REPORT_TYPE_PLUSALES_DEPT_ITEM );
		arrayReports.Add( REPORT_TYPE_PLUSALES_TAX_ITEM );
		arrayReports.Add( REPORT_TYPE_PLUSALES_TRANSERVER );
		arrayReports.Add( REPORT_TYPE_COVERS_PLU_TRANSERVER );
		arrayReports.Add( REPORT_TYPE_PLUSALES_CUSTOMER );
		
		if (TRUE == bModifier)
		{
			arrayReports.Add(REPORT_TYPE_PLUSALES_MODIFIER);
		}

		arrayReports.Add( REPORT_TYPE_PLUSALES_DEPT_SUMMARY );
		arrayReports.Add( REPORT_TYPE_TAX_SUMMARY_TAX );
		arrayReports.Add( REPORT_TYPE_TAX_SUMMARY_PRICE );
		arrayReports.Add( REPORT_TYPE_HOURLY );
		arrayReports.Add( REPORT_TYPE_HOURLY_GROUP );
		arrayReports.Add( REPORT_TYPE_HOURLY_DEPT );
		arrayReports.Add( REPORT_TYPE_DAILY );
		arrayReports.Add( REPORT_TYPE_DAILY_GROUP );
		arrayReports.Add( REPORT_TYPE_DAILY_DEPT );
		arrayReports.Add( REPORT_TYPE_TRANSACTION_NORMAL );
		CreateDefaultBatch( "Sales", arrayReports );

		arrayReports.RemoveAll();
		arrayReports.Add( REPORT_TYPE_COVERS_PLU_TRANSERVER );
		arrayReports.Add( REPORT_TYPE_COVERS_GROUP );
		arrayReports.Add( REPORT_TYPE_COVERS_DEPT );
		arrayReports.Add( REPORT_TYPE_COVERS_HOURLY );
		arrayReports.Add( REPORT_TYPE_COVERS_HOURLY_GROUP );
		arrayReports.Add( REPORT_TYPE_COVERS_HOURLY_DEPT );
		arrayReports.Add( REPORT_TYPE_COVERS_DAILY );
		arrayReports.Add( REPORT_TYPE_COVERS_DAILY_GROUP );
		arrayReports.Add( REPORT_TYPE_COVERS_DAILY_DEPT );
		arrayReports.Add( REPORT_TYPE_COVERS_MONTHLY );
		arrayReports.Add( REPORT_TYPE_COVERS_PRICELEVEL );
		arrayReports.Add( REPORT_TYPE_COVERS_PAYMENT );
		arrayReports.Add( REPORT_TYPE_MIXMATCH_ITEM_COVERS );
		arrayReports.Add( REPORT_TYPE_MIXMATCH_DEPT_COVERS );
		arrayReports.Add( REPORT_TYPE_MIXMATCH_GROUP_COVERS );
		arrayReports.Add( REPORT_TYPE_TRANSACTION_NORMAL );
		CreateDefaultBatch( "Covers", arrayReports );

		arrayReports.RemoveAll();
		arrayReports.Add( REPORT_TYPE_PAYMENT_SUMMARY );
		arrayReports.Add( REPORT_TYPE_PAYMENT_SERVER );
		arrayReports.Add( REPORT_TYPE_PAYMENT_GROUP_SUMMARY );
		arrayReports.Add( REPORT_TYPE_PAYMENT_ESTIMATE_GROUP );
		arrayReports.Add( REPORT_TYPE_PAYMENT_ESTIMATE_DEPT );
		arrayReports.Add( REPORT_TYPE_PAYMENT_ESTIMATE_ITEM );
		arrayReports.Add( REPORT_TYPE_PAYMENT_ESTIMATE_TAX );
		arrayReports.Add( REPORT_TYPE_PAYMENT_DETAIL );
		arrayReports.Add( REPORT_TYPE_TRANSACTION_NORMAL );
		CreateDefaultBatch( "Payments", arrayReports );

		arrayReports.RemoveAll();
		arrayReports.Add( REPORT_TYPE_GENERAL_GROUP );
		arrayReports.Add( REPORT_TYPE_GENERAL_DEPT );
		arrayReports.Add( REPORT_TYPE_GENERAL_GROUP_QTY );
		arrayReports.Add( REPORT_TYPE_GENERAL_DEPT_QTY );
		arrayReports.Add( REPORT_TYPE_WEEKLY_GROUP );
		arrayReports.Add( REPORT_TYPE_WEEKLY_DEPT );
		arrayReports.Add( REPORT_TYPE_WEEKLY_GROUP_QTY );
		arrayReports.Add( REPORT_TYPE_WEEKLY_DEPT_QTY );
		arrayReports.Add( REPORT_TYPE_TRANSACTION_NORMAL );
		CreateDefaultBatch( "General", arrayReports );

		arrayReports.RemoveAll();
		arrayReports.Add( REPORT_TYPE_PLUSALES_TRANSERVER );
		arrayReports.Add( REPORT_TYPE_TRANSERVER_SUMMARY );
		arrayReports.Add( REPORT_TYPE_COVERS_PLU_TRANSERVER );
		arrayReports.Add( REPORT_TYPE_PAYMENT_SERVER );
		arrayReports.Add( REPORT_TYPE_TRANSERVER_VOID );
		arrayReports.Add( REPORT_TYPE_TRANSERVER_VOID_REASON );
		arrayReports.Add( REPORT_TYPE_TRANSERVER_REFUND );
		arrayReports.Add( REPORT_TYPE_TRANSERVER_REFUND_REASON );
		arrayReports.Add( REPORT_TYPE_TRANSERVER_DISCOUNT );
		arrayReports.Add( REPORT_TYPE_TRANSERVER_DISCOUNT_REASON );
		arrayReports.Add( REPORT_TYPE_NOSALE );
		arrayReports.Add( REPORT_TYPE_AGEVERIFY );
		arrayReports.Add( REPORT_TYPE_TRANSACTION_NORMAL );
		CreateDefaultBatch( "Servers", arrayReports );

		arrayReports.RemoveAll();
		arrayReports.Add( REPORT_TYPE_DEPOSIT_PURCHASED );
		arrayReports.Add( REPORT_TYPE_DEPOSIT_REDEEMED );
		arrayReports.Add( REPORT_TYPE_DEPOSIT_UNREDEEMED );
		arrayReports.Add( REPORT_TYPE_DEPOSIT_BOOKING_PURCHASE );
		arrayReports.Add( REPORT_TYPE_DEPOSIT_BOOKING_EVENT );
		arrayReports.Add( REPORT_TYPE_TRANSACTION_NORMAL );
		CreateDefaultBatch( "Deposits", arrayReports );

		arrayReports.RemoveAll();
		arrayReports.Add( REPORT_TYPE_PLUSALES_CUSTOMER );
		arrayReports.Add( REPORT_TYPE_CUSTOMER_RA );
		arrayReports.Add( REPORT_TYPE_CUSTOMER_SPEND );
		arrayReports.Add( REPORT_TYPE_CUSTOMER_ACTIVITY );
		arrayReports.Add( REPORT_TYPE_CUSTOMER_BALANCE );
		arrayReports.Add( REPORT_TYPE_TRANSACTION_NORMAL );
		CreateDefaultBatch( "Customers", arrayReports );

		arrayReports.RemoveAll();
		arrayReports.Add( REPORT_TYPE_PROMOTION_SUMMARY );
		arrayReports.Add( REPORT_TYPE_MIXMATCH_GROUP );
		arrayReports.Add( REPORT_TYPE_MIXMATCH_DEPT );
		arrayReports.Add( REPORT_TYPE_MIXMATCH_ITEM );
		arrayReports.Add( REPORT_TYPE_MIXMATCH_ITEM_COVERS );
		arrayReports.Add( REPORT_TYPE_MIXMATCH_DEPT_COVERS );
		arrayReports.Add( REPORT_TYPE_MIXMATCH_GROUP_COVERS );
		arrayReports.Add( REPORT_TYPE_CHEAPESTONEFREE_SUMMARY );
		arrayReports.Add( REPORT_TYPE_CHEAPESTONEFREE_DETAIL );
		arrayReports.Add( REPORT_TYPE_TRANSACTION_NORMAL );
		arrayReports.Add( REPORT_TYPE_TRANSERVER_DISCOUNT );
		arrayReports.Add( REPORT_TYPE_TRANSERVER_DISCOUNT_REASON );
		CreateDefaultBatch( "Discounts", arrayReports );
	}

}

/**********************************************************************/

void CPresetReportArrayEposBatch::PrepareForWrite()
{
	m_arrayFileStrings.RemoveAt(0, m_arrayFileStrings.GetSize() );
	
	for ( int n = 0; n < m_arrayPresets.GetSize(); n++ )
	{
		CPresetReportInfo infoBase = m_arrayPresets.GetAt(n);
		
		CPresetReportInfoEposBatch infoPreset;
		CString strFields = infoBase.GetOtherFields();
		infoPreset.FromString( strFields  );

		CCSV csv( ',', '"', TRUE, TRUE, TRUE );
		infoPreset.ConvertToCSV( csv, infoBase );
		m_arrayFileStrings.Add( csv.GetLine() );
	}
}

/**********************************************************************/

void CPresetReportArrayEposBatch::CreateDefaultBatch(const char* szBatchName, CArray<int, int>& arrayReports)
{
	CPresetReportInfo Base;
	CPresetReportInfoEposBatch Preset;

	Base.SetName(szBatchName);
	Base.SetEntityNo(GetNextFreeEntityNo());
	Preset.SetReportAllLocationsFlag(FALSE);
	Preset.SetReportByTerminalFlag(FALSE);
	Preset.SetReportByLocationFlag(FALSE);
	Preset.SetActionType(1);

	for (int n = 0; n < arrayReports.GetSize(); n++)
	{
		Preset.SetReportFlag(arrayReports.GetAt(n), TRUE);
	}

	CString str;
	Preset.ToString(str);
	Base.SetOtherFields(str);

	Add(Base);
}

/**********************************************************************/