/**********************************************************************/
#include "PosTrayTask.h"
/**********************************************************************/
#include "PosTrayExportFieldNames.h"
/**********************************************************************/

CPosTrayExportFieldName::CPosTrayExportFieldName()
{
	Reset();
}

/**********************************************************************/

void CPosTrayExportFieldName::Reset()
{
	m_nFieldType = 0;
	m_strDisplayName1 = "";
	m_strDisplayName2 = "";
	m_strHeaderNameDefault = "";
	m_strHeaderNameUser = "";
}

/**********************************************************************/

void CPosTrayExportFieldName::Add (CPosTrayExportFieldName& source )
{
}

/**********************************************************************/

int CPosTrayExportFieldName::Compare( CPosTrayExportFieldName& source, int nHint )
{
	if ( m_nFieldType != source.m_nFieldType )
		return ( ( m_nFieldType > source.m_nFieldType ) ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/

const char* CPosTrayExportFieldName::GetSingleColumnName()
{
	m_strSingleColumnName = m_strDisplayName1;
	
	if ( m_strDisplayName2 != "" )
	{
		m_strSingleColumnName += " (";
		m_strSingleColumnName += m_strDisplayName2;
		m_strSingleColumnName += ")";
	}

	return m_strSingleColumnName;
}

/**********************************************************************/

const char* CPosTrayExportFieldName::GetHeaderNameForExport()
{
	m_strHeaderNameForExport = m_strHeaderNameUser;
	::TrimSpaces( m_strHeaderNameForExport, FALSE );
	
	if ( m_strHeaderNameForExport == "" )
		m_strHeaderNameForExport = m_strHeaderNameDefault;
	
	return m_strHeaderNameForExport;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CPosTrayExportFieldNames::CPosTrayExportFieldNames(void)
{
	m_bFixedTaxNames = FALSE;
	AddField( POSTRAY_EXPORTFIELD_DATE_CAL,				"Calendar Date",				"",						"CalendarDate" );
	AddField( POSTRAY_EXPORTFIELD_DATE_BUS,				"Business Date",				"",						"BusinessDate" );
	AddField( POSTRAY_EXPORTFIELD_DATE_WEEK,			"Week Start Date",				"",						"WeekStartDate" );
	AddField( POSTRAY_EXPORTFIELD_YEARNO,				"Year",							"",						"Year" );
	AddField( POSTRAY_EXPORTFIELD_MONTHNO,				"Month Number",					"",						"MonthNo" );
	AddField( POSTRAY_EXPORTFIELD_WEEKNO,				"Week Number",					"",						"WeekNo" );
	AddField( POSTRAY_EXPORTFIELD_WEEKDAYNO,			"Weekday Number",				"",						"WeekdayNo" );
	AddField( POSTRAY_EXPORTFIELD_TIME_TRAN,			"Transaction Time",				"",						"TranTime" );
	AddField( POSTRAY_EXPORTFIELD_TIME_SALE,			"Sale Time",					"",						"SaleTime" );
	AddField( POSTRAY_EXPORTFIELD_TIME_VOID,			"Void Time",					"",						"VoidTime" );
	AddField( POSTRAY_EXPORTFIELD_TIME_DISCOUNT,		"Discount Time",				"",						"DiscountTime" );
	AddField( POSTRAY_EXPORTFIELD_MONTHNAME,			"Month Name",					"",						"MonthName" );
	AddField( POSTRAY_EXPORTFIELD_WEEKDAYNAME,			"Weekday Name",					"",						"WeekdayName" );
	AddField( POSTRAY_EXPORTFIELD_DBNO,					"Database Number",				"",						"DatabaseNo" );
	AddField( POSTRAY_EXPORTFIELD_LOCNO,				"Location Number",				"",						"LocationNo" );
	AddField( POSTRAY_EXPORTFIELD_RPNO,					"Reportpoint Number",			"",						"ReportpointNo" );
	AddField( POSTRAY_EXPORTFIELD_TRMNO,				"Terminal Number",				"",						"TerminalNo" );
	AddField( POSTRAY_EXPORTFIELD_TBLGRPNO,				"Table Group Number",			"",						"TableGroupNo" );
	AddField( POSTRAY_EXPORTFIELD_TBLNO,				"Table Number",					"",						"TableNo" );
	AddField( POSTRAY_EXPORTFIELD_DBNAME,				"Database Name",				"",						"DatabaseName" );
	AddField( POSTRAY_EXPORTFIELD_LOCNAME,				"Location Name",				"",						"LocationName" );
	AddField( POSTRAY_EXPORTFIELD_RPNAME,				"Reportpoint Name",				"",						"ReportpointName" );
	AddField( POSTRAY_EXPORTFIELD_TBLGRPNAME,			"Table Group Name",				"",						"TableGroupName" );
	AddField( POSTRAY_EXPORTFIELD_TBLNAME,				"Table Name",					"",						"TableName" );
	AddField( POSTRAY_EXPORTFIELD_PLUNO,				"Plu Number",					"",						"PluNo" );
	AddField( POSTRAY_EXPORTFIELD_MODNO,				"Modifier Number",				"",						"ModifierNo" );
	AddField( POSTRAY_EXPORTFIELD_REPTEXT_BASE,			"Report Text",					"Base Item",			"RepTextPlu" );
	AddField( POSTRAY_EXPORTFIELD_REPTEXT_MOD,			"Report Text",					"Modifier",				"RepTextModifier" );
	AddField( POSTRAY_EXPORTFIELD_MODNAME,				"Modifier Name",				"",						"ModifierName" );
	AddField( POSTRAY_EXPORTFIELD_SUPPREF,				"Supplier Reference",			"",						"SupplierReference" );
	AddField( POSTRAY_EXPORTFIELD_SORTCODE,				"Sort Code",					"",						"SortCode" );
	AddField( POSTRAY_EXPORTFIELD_STOCKCODE,			"Stock Code",					"",						"StockCode" );
	AddField( POSTRAY_EXPORTFIELD_DPTNO_DBASE,			"Department Number",			"Database",				"DeptNoDBase" );
	AddField( POSTRAY_EXPORTFIELD_DPTNO_SALE,			"Department Number",			"Sale",					"DeptNoSale" );
	AddField( POSTRAY_EXPORTFIELD_DPTNAME_DBASE,		"Department Name",				"Database",				"DeptNameDBase" );
	AddField( POSTRAY_EXPORTFIELD_DPTNAME_SALE,			"Department Name",				"Sale",					"DeptNameSale" );
	AddField( POSTRAY_EXPORTFIELD_GRPNO_DBASE,			"Group Number",					"Database",				"GroupNoDBase" );
	AddField( POSTRAY_EXPORTFIELD_GRPNO_SALE,			"Group Number",					"Sale",					"GroupNoSale" );
	AddField( POSTRAY_EXPORTFIELD_GRPNAME_DBASE,		"Group Name",					"Database",				"GroupNameDbase" );
	AddField( POSTRAY_EXPORTFIELD_GRPNAME_SALE,			"Group Name",					"Sale",					"GroupNameSale" );
	AddField( POSTRAY_EXPORTFIELD_ACATNO_DBASE,			"Category Number",				"Database",				"ACatNoDBase" );
	AddField( POSTRAY_EXPORTFIELD_ACATNO_SALE,			"Category Number",				"Sale",					"ACatNoSale" );
	AddField( POSTRAY_EXPORTFIELD_ACATNAME_DBASE,		"Category Name",				"Database",				"ACatNameDBase" );
	AddField( POSTRAY_EXPORTFIELD_ACATNAME_SALE,		"Category Name",				"Sale",					"ACatNameSale" );
	AddField( POSTRAY_EXPORTFIELD_SERVER_TRAN,			"Server Number",				"Transaction",			"ServerNoTran" );
	AddField( POSTRAY_EXPORTFIELD_SERVER_ITEM,			"Server Number",				"Item",					"ServerNoItem" );
	AddField( POSTRAY_EXPORTFIELD_SERVER_AUTH,			"Server Number",				"Authorisation",		"ServerNoAuth" );
	AddField( POSTRAY_EXPORTFIELD_SRVNAME_TRAN,			"Server Name",					"Transaction",			"ServerNameTran" );
	AddField( POSTRAY_EXPORTFIELD_SRVNAME_ITEM,			"Server Name",					"Item",					"ServerNameItem" );
	AddField( POSTRAY_EXPORTFIELD_SRVNAME_AUTH,			"Server Name",					"Authorisation",		"ServerNameAuth" );
	AddField( POSTRAY_EXPORTFIELD_TAXNO,				"Tax Band",						"",						"TaxBand" );
	AddField( POSTRAY_EXPORTFIELD_TAXNAME,				"Tax Band Name",				"",						"TaxBandName" );
	AddField( POSTRAY_EXPORTFIELD_PBANDNO,				"Price Band",					"",						"PriceBand" );
	AddField( POSTRAY_EXPORTFIELD_PBANDNAME,			"Price Band Name",				"",						"PriceBandName" );
	AddField( POSTRAY_EXPORTFIELD_QTY_BASE,				"Base Plu Quantity",			"",						"BasePluQty" );
	AddField( POSTRAY_EXPORTFIELD_QTY_MOD,				"Modifier Quantity",			"",						"ModifierPluQty" );
	AddField( POSTRAY_EXPORTFIELD_VALUE_FULL,			"Value Before Discount",		"",						"ValueBeforeDiscount" );
	AddField( POSTRAY_EXPORTFIELD_DISCOUNTPREMIUM,		"Discount Minus Premium",		"",						"NetDiscountAmount" );
	AddField( POSTRAY_EXPORTFIELD_VALUE_DISC,			"Value After Discount",			"",						"ValueAfterDiscount" );
	AddField( POSTRAY_EXPORTFIELD_TAX,					"",								"",						"" );
	AddField( POSTRAY_EXPORTFIELD_VALUE_NET,			"",								"",						"" );
	AddField( POSTRAY_EXPORTFIELD_COST,					"Cost Price",					"",						"CostPrice" );
	AddField( POSTRAY_EXPORTFIELD_WASTE_QTY_BASE,		"Base Wastage Quantity",		"",						"BaseWastageQty" );
	AddField( POSTRAY_EXPORTFIELD_WASTE_QTY_MOD,		"Modifier Wastage Quantity",	"",						"ModifierWastageQty" );
	AddField( POSTRAY_EXPORTFIELD_WASTE_COST,			"Wastage Cost",					"",						"WastageCost" );
	AddField( POSTRAY_EXPORTFIELD_VALUE_VOID,			"Value",						"",						"Value" );
	AddField( POSTRAY_EXPORTFIELD_LINE_COUNT,			"Line Count",					"",						"LineCount" );
	AddField( POSTRAY_EXPORTFIELD_QTY_COVERS,			"Covers Quantity",				"",						"CoversQty" );
	AddField( POSTRAY_EXPORTFIELD_DISCOUNTONLY,			"Discount Amount",				"",						"DiscountAmount");
	AddField( POSTRAY_EXPORTFIELD_PREMIUMONLY,			"Premium Amount",				"",						"PremiumAmount");	
	AddField( POSTRAY_EXPORTFIELD_QTY_PAY,				"Payment Quantity",				"",						"PaymentQty" );
	AddField( POSTRAY_EXPORTFIELD_VALUE_TENDER,			"Tender",						"",						"Tender" );
	AddField( POSTRAY_EXPORTFIELD_VALUE_AMOUNT,			"Amount",						"",						"Amount" );
	AddField( POSTRAY_EXPORTFIELD_VALUE_GRATUITY,		"Gratuity",						"",						"Gratuity" );
	AddField( POSTRAY_EXPORTFIELD_VALUE_CASHBACK,		"Cashback",						"",						"Cashback" );
	AddField( POSTRAY_EXPORTFIELD_VALUE_SURPLUS,		"Surplus",						"",						"Surplus" );
	AddField( POSTRAY_EXPORTFIELD_PAYNO,				"Payment Number",				"",						"PaymentNo" );
	AddField( POSTRAY_EXPORTFIELD_PAYNAME,				"Payment Name",					"",						"PaymentName" );
	AddField( POSTRAY_EXPORTFIELD_VOID_SEQNO,			"Sequence Number",				"",						"SequenceNo" );
	AddField( POSTRAY_EXPORTFIELD_VOID_TABLENO,			"Table Number",					"",						"TableNo" );
	AddField( POSTRAY_EXPORTFIELD_VOID_LINE_TYPE,		"Line Type",					"Sale / Discount",		"LineType" );
	AddField( POSTRAY_EXPORTFIELD_VOID_BEFOREAFTER,		"Before/After",					"",						"Before/After" );
	AddField( POSTRAY_EXPORTFIELD_VOID_REASON,			"Reason Text",					"",						"ReasonText" );
	AddField( POSTRAY_EXPORTFIELD_DISCOUNT_LINE_TYPE,	"Line Type",					"Discount / Premium",	"LineType" );
	AddField( POSTRAY_EXPORTFIELD_DISCOUNT_MODE,		"Discount Mode",				"Item / Subtotal",		"DiscountMode" );
	AddField( POSTRAY_EXPORTFIELD_DISCOUNT_USAGE,		"Discount Usage",				"",						"DiscountUsage" );
	AddField( POSTRAY_EXPORTFIELD_DISCOUNT_TYPE,		"Discount Type",				"Discount / Markdown",	"DiscountType" );
	AddField( POSTRAY_EXPORTFIELD_DISCOUNT_RATE,		"Discount Rate",				"",						"DiscountRate" );
	AddField( POSTRAY_EXPORTFIELD_PROMONO,				"Promo Number",					"",						"PromoNo" );
	AddField( POSTRAY_EXPORTFIELD_PROMONAME,			"Promo Name",					"",						"PromoName" );
	AddField( POSTRAY_EXPORTFIELD_TRAN_SEQNO,			"Sequence Number",				"",						"SequenceNo" );
	AddField( POSTRAY_EXPORTFIELD_MIXTYPENO,			"Mix Match Type Number",		"",						"MixMatchTypeNo" );
	AddField( POSTRAY_EXPORTFIELD_MIXMATCHNO,			"Mix Match Offer Number",		"",						"MixMatchOfferNo" );
	AddField( POSTRAY_EXPORTFIELD_MIXTYPENAME,			"Mix Match Type Name",			"",						"MixMatchTypeName" );
	AddField( POSTRAY_EXPORTFIELD_MIXMATCHNAME,			"Mix Match Offer Name",			"",						"MixMatchOfferName" );
	AddField( POSTRAY_EXPORTFIELD_DUMMY,				"Dummy",						"",						"Dummy" );
}

/**********************************************************************/

void CPosTrayExportFieldNames::AddField( int nFieldType, const char* szDisplayName1, const char* szDisplayName2, const char* szHeaderName )
{
	CPosTrayExportFieldName Field;
	Field.m_nFieldType = nFieldType;
	Field.m_strDisplayName1 = szDisplayName1;
	Field.m_strDisplayName2 = szDisplayName2;
	Field.m_strHeaderNameDefault = szHeaderName;
	Field.m_strHeaderNameUser = "";
	m_arrayNames.DirectAdd( Field );
}

/**********************************************************************/

void CPosTrayExportFieldNames::FixTaxNames()
{
	if ( FALSE == m_bFixedTaxNames )
	{
		for ( int n = 0; n < m_arrayNames.GetSize(); n++ )
		{
			CPosTrayExportFieldName Field;
			m_arrayNames.GetAt( n, Field );
			
			switch( Field.m_nFieldType )
			{
			case POSTRAY_EXPORTFIELD_TAX:
				Field.m_strDisplayName1 = EcrmanOptions.GetTaxString( "%T Amount" );
				Field.m_strHeaderNameDefault = EcrmanOptions.GetTaxString( "%TAmount" );
				m_arrayNames.SetAt( n, Field );
				break;

			case POSTRAY_EXPORTFIELD_VALUE_NET:
				Field.m_strDisplayName1 = EcrmanOptions.GetTaxString( "Value After %T" );
				Field.m_strHeaderNameDefault = EcrmanOptions.GetTaxString( "ValueAfter%T" );
				m_arrayNames.SetAt( n, Field );
				break;

			case POSTRAY_EXPORTFIELD_TAXNO:
				Field.m_strDisplayName1 = EcrmanOptions.GetTaxString( "%T Band" );
				Field.m_strHeaderNameDefault = EcrmanOptions.GetTaxString( "%TBand" );
				m_arrayNames.SetAt( n, Field );
				break;

			case POSTRAY_EXPORTFIELD_TAXNAME:
				Field.m_strDisplayName1 = EcrmanOptions.GetTaxString( "%T Band Name" );
				Field.m_strHeaderNameDefault = EcrmanOptions.GetTaxString( "%TBandName" );
				m_arrayNames.SetAt( n, Field );
				break;
			}
		}
		m_bFixedTaxNames = TRUE;
	}
}

/**********************************************************************/

int CPosTrayExportFieldNames::Open( const char* szFilename, int nMode )
{
	RemoveAt( 0, GetSize() );
	
	int nReply = CSharedStringArray::Open ( szFilename, nMode );
	
	if ( nReply != DB_ERR_SHARE )
	{
		for ( int n = 0; n < m_arrayNames.GetSize(); n++ )
		{
			CPosTrayExportFieldName Name;
			m_arrayNames.GetAt( n, Name );
			Name.m_strHeaderNameUser = "";
			m_arrayNames.SetAt( n, Name );
		}

		for ( int n = 0; n < GetSize(); n++ )
		{
			CCSV csv( GetAt(n) );

			if ( csv.GetInt(0) == 1 )
			{
				CPosTrayExportFieldName Name;
				Name.m_nFieldType = csv.GetInt(1);

				int nPos;
				if ( m_arrayNames.Find( Name, nPos ) == TRUE )
				{
					m_arrayNames.GetAt( nPos, Name );

					CString strText = csv.GetString(2);
					if ( strText.GetLength() <= POSTRAY_MAXLENGTH_FIELD_NAME )
					{
						if ( strText != Name.m_strHeaderNameDefault )
						{
							Name.m_strHeaderNameUser = strText;
							m_arrayNames.SetAt( nPos, Name );
						}
					}
				}
			}
		}
	}

	RemoveAt( 0, GetSize() );
	return nReply;
}

/**********************************************************************/

void CPosTrayExportFieldNames::PrepareForWrite()
{
	RemoveAt( 0, GetSize() );

	for ( int n = 0; n < m_arrayNames.GetSize(); n++ )
	{
		CPosTrayExportFieldName Name;
		m_arrayNames.GetAt( n, Name );

		if ( Name.m_strHeaderNameUser != "" )
		{
			if ( Name.m_strHeaderNameUser != Name.m_strHeaderNameDefault )
			{
				CCSV csv;
				csv.Add( 1 );
				csv.Add( Name.m_nFieldType );
				csv.Add( Name.m_strHeaderNameUser );
				Add( csv.GetLine() );
			}
		}
	}
}

/**********************************************************************/

void CPosTrayExportFieldNames::GetNameInfo( CPosTrayExportFieldName& Name )
{
	int nPos;
	if ( m_arrayNames.Find( Name, nPos ) == TRUE )
		m_arrayNames.GetAt( nPos, Name );
	else
	{
		Name.m_strDisplayName1 = "Unknown";
		Name.m_strDisplayName2 = "";
		Name.m_strHeaderNameDefault = "Unknown";
		Name.m_strHeaderNameUser = "";
	}
}

/**********************************************************************/

void CPosTrayExportFieldNames::SetNameInfo( CPosTrayExportFieldName& Name )
{
	int nPos;
	if ( m_arrayNames.Find( Name, nPos ) == TRUE )
		m_arrayNames.SetAt( nPos, Name );
}

/**********************************************************************/
