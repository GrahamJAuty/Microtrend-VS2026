/**********************************************************************/
#include "CustomField.h"
/**********************************************************************/
#include "..\CommonStockTray\StockCSVArray.h"
/**********************************************************************/

CStockCSVRecord::CStockCSVRecord()
{
	ClearRecord();
}

/**********************************************************************/

void CStockCSVRecord::ClearRecord()
{
	ClearSuppliers();

	m_strStockCode = "";
	m_strImportSheetCode = "";
	m_strDescription = "";
	m_nCategory = 1;

	for ( int nFieldNo = 1; nFieldNo <= MAX_ITEM_CUSTOM_FIELDS; nFieldNo++ )
		SetCustomFieldValueNo( nFieldNo, 0 );

	m_strStockUnit = "";
	m_strStockmateText = "";
	m_nSubUnits = 1;
	m_strSubUnit = "";
	m_nSUWeightType = 0;
	m_dSUWeight = 0.0;
	m_dWastePercent = 0.0;
	m_bDisableAutoOrder = FALSE;
	m_bDisablePluRange = FALSE;
	m_bAutoLabel = TRUE;
	m_bLocalSupplier = FALSE;
	m_nSupplierCount = 0;
}

/**********************************************************************/

void CStockCSVRecord::ConvertStockLevelToStockSub( double dLevel, __int64& nStock, __int64& nSub )
{
	nStock = ( __int64 ) dLevel;
	nSub = ( __int64 ) ( ( dLevel * m_nSubUnits ) - ( nStock * m_nSubUnits ) + 0.5 );
}

/**********************************************************************/

void CStockCSVRecord::ConvertStockLevelToStockSubForPluEdit( double dLevel, __int64& nStock, double& dRemainder )
{
	nStock = ( __int64 ) dLevel;
	dRemainder = dLevel - nStock;
	dRemainder *= m_nSubUnits;
}

/**********************************************************************/

void CStockCSVRecord::ConvertStockLevelToTotalSubUnits( double dLevel, __int64& nSubUnits )
{
	bool bNegative = FALSE;
	if ( dLevel < 0.0 )
	{
		dLevel = -dLevel;
		bNegative = TRUE;
	}

	__int64 nStock = ( __int64 ) dLevel;
	nSubUnits = ( __int64 ) ( ( dLevel * m_nSubUnits ) - ( nStock * m_nSubUnits ) + 0.5 );
	nSubUnits += ( nStock * m_nSubUnits );

	if ( bNegative )
		nSubUnits = -nSubUnits;
}

/**********************************************************************/

void CStockCSVRecord::ConvertTotalSubUnitsToStockLevel( __int64 nTotalSub, double& dLevel )
{
	__int64 nStock = nTotalSub / m_nSubUnits;
	__int64 nSpare = nTotalSub - ( nStock * m_nSubUnits );
	dLevel = nStock + ( ( double ) nSpare / m_nSubUnits );
}

/**********************************************************************/

void CStockCSVRecord::ClearSuppliers()
{
	for ( int nSuppIdx = 0; nSuppIdx < MAX_ITEM_SUPPLIERS; nSuppIdx++ )
		Suppliers[nSuppIdx].Clear();
}

/**********************************************************************/

void CStockCSVRecord::ClearSupplier( int nSuppIdx)
{
	if ( nSuppIdx >= 0 && nSuppIdx < MAX_ITEM_SUPPLIERS )
		Suppliers[nSuppIdx].Clear();
}

/**********************************************************************/

void CStockCSVRecord::MakePreferred( int nSuppIdx )
{
	if ( nSuppIdx > 0 && nSuppIdx < MAX_ITEM_SUPPLIERS )
	{
		CStockItemSupplier tempSupplier = Suppliers[0];
		Suppliers[0] = Suppliers [ nSuppIdx ];
		Suppliers[ nSuppIdx ] = tempSupplier;
	}
}

/**********************************************************************/

void CStockCSVRecord::TidySuppliers( int nUserMaxSuppliers )
{
	int nDestPos = 0;
	if ( nUserMaxSuppliers < 1 || nUserMaxSuppliers > MAX_ITEM_SUPPLIERS )
		nUserMaxSuppliers = MAX_ITEM_SUPPLIERS;

	for ( int nSuppIdx = 0; nSuppIdx < nUserMaxSuppliers; nSuppIdx++ )
	{
		if ( GetSuppNo(nSuppIdx) != 0 )
		{
			if ( nDestPos != nSuppIdx )
				Suppliers[nDestPos] = Suppliers[nSuppIdx];

			nDestPos++;
		}
	}

	for ( int nClear = nDestPos; nClear < MAX_ITEM_SUPPLIERS; nClear++ )
		ClearSupplier(nClear);

	m_nSupplierCount = nDestPos;
}

/**********************************************************************/

CStockCSVRecord::~CStockCSVRecord()
{	
}

/**********************************************************************/

void CStockCSVRecord::ConvertToCSV ( CCSV& csv )
{
	csv.Add ( m_strStockCode );
	csv.Add ( m_strDescription );
	csv.Add ( m_nCategory );
	csv.Add ( STOCKDB_VERSION );
	csv.Add ( m_strImportSheetCode );
	csv.Add ( GetCustomFieldValueNo(1) );
	csv.Add ( GetCustomFieldValueNo(2) );
	csv.Add ( GetCustomFieldValueNo(3) );
	csv.Add ( GetCustomFieldValueNo(4) );
	csv.Add ( GetCustomFieldValueNo(5) );
	csv.Add ( m_strStockUnit );
	csv.Add ( m_strStockmateText );
	csv.Add ( m_nSubUnits );
	csv.Add ( m_strSubUnit );
	csv.Add ( m_nSUWeightType );
	csv.Add ( m_dSUWeight, 5 );
	csv.Add ( m_dWastePercent, 2 );
	csv.Add ( m_bDisableAutoOrder );
	csv.Add ( m_bDisablePluRange );
	csv.Add ( m_bAutoLabel );
	csv.Add ( m_bLocalSupplier );
	
	int nSupplierCount = m_nSupplierCount;
	if (nSupplierCount > MAX_ITEM_SUPPLIERS)
	{
		nSupplierCount = MAX_ITEM_SUPPLIERS;
	}

	csv.Add ( nSupplierCount );

	for ( int nIndex = 0; nIndex < nSupplierCount; nIndex ++ )
	{
		csv.Add ( GetSuppNo ( nIndex ) );
		csv.Add ( GetCase ( nIndex ) );
		csv.Add ( GetDUItems( nIndex ) );
		csv.Add ( GetDUItemSize( nIndex ), 3 );
		csv.Add ( GetCost ( nIndex ), 5 );
		csv.Add ( GetSuppRefInternal ( nIndex ) );
		csv.Add ( GetWrapper( nIndex ) );
		csv.Add ( GetRetail( nIndex ), 3 );
	}
}

/**********************************************************************/

void CStockCSVRecord::ConvertToCSVSimpleExport(CCSV& csv, CSupplierCSVArray& SupplierArray, CPtrArray& CustomFieldArrays, int nSupplierCount)
{
	csv.Add(m_strStockCode);
	csv.Add(m_strDescription);
	csv.Add(m_nCategory);
	csv.Add(STOCKDB_VERSION);

	csv.Add(m_strImportSheetCode);
	ExportCustomFieldKey(csv, 1, CustomFieldArrays);
	ExportCustomFieldKey(csv, 2, CustomFieldArrays);
	ExportCustomFieldKey(csv, 3, CustomFieldArrays);
	ExportCustomFieldKey(csv, 4, CustomFieldArrays);
	ExportCustomFieldKey(csv, 5, CustomFieldArrays);

	csv.Add(m_strStockUnit);
	csv.Add(m_strStockmateText);
	csv.Add(m_nSubUnits);
	csv.Add(m_strSubUnit);
	csv.Add(m_nSUWeightType);
	csv.Add(m_dSUWeight, 5);
	csv.Add(m_dWastePercent, 2);
	csv.Add(m_bDisableAutoOrder);
	csv.Add(m_bDisablePluRange);
	csv.Add(m_bAutoLabel);
	csv.Add(m_bLocalSupplier);

	if (nSupplierCount < 0)
	{
		nSupplierCount = m_nSupplierCount;
	}

	if (nSupplierCount > MAX_ITEM_SUPPLIERS)
	{
		nSupplierCount = MAX_ITEM_SUPPLIERS;
	}

	csv.Add(nSupplierCount);

	for (int nIndex = 0; nIndex < nSupplierCount; nIndex++)
	{
		int nSuppIdx;
		if (SupplierArray.FindSupplierByNumber(GetSuppNo(nIndex), nSuppIdx))
		{
			csv.Add(SupplierArray.GetKey(nSuppIdx));
		}
		else
		{
			csv.Add("");
		}

		csv.Add(GetCase(nIndex));
		csv.Add(GetDUItems(nIndex));
		csv.Add(GetDUItemSize(nIndex), 3);
		csv.Add(GetCost(nIndex), 5);
		csv.Add(GetSuppRefInternal(nIndex));
		csv.Add(GetWrapper(nIndex));
		csv.Add(GetRetail(nIndex), 3);
	}
}

/**********************************************************************/

void CStockCSVRecord::ExportCustomFieldKey( CCSV& csv, int nFieldNo, CPtrArray& CustomFieldArrays )
{
	CString strKey = "N/A";

	if ( ( nFieldNo >= 1 ) && ( nFieldNo <= CustomFieldArrays.GetSize() ) )
	{
		CCustomFieldValueCSVArray* pArray = (CCustomFieldValueCSVArray*) CustomFieldArrays.GetAt( nFieldNo - 1 );

		int nValueNo = GetCustomFieldValueNo( nFieldNo );

		if (0 != nValueNo)
		{
			int nValueIdx;
			if (pArray->FindValueByNumber(nValueNo, nValueIdx) == TRUE)
			{
				strKey = pArray->GetKey(nValueIdx);
			}
		}
	}

	csv.Add( strKey );
}

/**********************************************************************/

void CStockCSVRecord::ConvertFromCSV(CCSV& csv)
{
	ClearRecord();

	SetStockCode(csv.GetString(0));
	SetDescription(csv.GetString(1));
	SetCategory(csv.GetInt(2));

	int nVersion = csv.GetInt(3);

	switch (nVersion)
	{
	case 1:
		V1ConvertFromCSV(csv);
		break;

	case 2:
		V2ConvertFromCSV(csv);
		break;

	case 3:
	case 4:
	case 5:
		V3to5ConvertFromCSV(csv);
		break;

	case 6:
	case 7:
		V6to7ConvertFromCSV(csv);
		break;

	case 8:
		V8to14ConvertFromCSV(csv, 8);
		break;

	case 9:
		V8to14ConvertFromCSV(csv, 9);
		break;

	case 10:
		V8to14ConvertFromCSV(csv, 10);
		break;

	case 11:
		V8to14ConvertFromCSV(csv, 11);
		break;

	case 12:
		V8to14ConvertFromCSV(csv, 12);
		break;

	case 13:
		V8to14ConvertFromCSV(csv, 13);
		break;

	case 14:
		V8to14ConvertFromCSV(csv, 14);
		break;
	}
}

/**********************************************************************/

void CStockCSVRecord::V1ConvertFromCSV ( CCSV& csv )
{	
	int n = 4;
	SetStockUnitName ( csv.GetString(n++) );
	SetSubUnits ( csv.GetInt(n++) );
	SetSubUnitName ( csv.GetString (n++) );
	SetWastePercent ( csv.GetDouble(n++) );
	SetDisableAutoOrderFlag ( csv.GetBool(n++) );
	SetDisablePluRangeFlag ( csv.GetBool(n++) );
	
	int nSupplierTableSize = csv.GetInt ( n++ );
	for ( int nIndex = 0; nIndex < nSupplierTableSize; nIndex++ )
	{
		if ( nIndex >= MAX_ITEM_SUPPLIERS )
			break;

		int nSuppNo = csv.GetInt ( n++ );
		if ( nSuppNo <= 0 )
			break;
			
		SetSuppNo ( nIndex, nSuppNo );
		SetCase ( nIndex, csv.GetString ( n++ ) );
		SetDUItems ( nIndex, csv.GetInt ( n++ ) );
		SetDUItemSize ( nIndex, 1.0 );
		SetCost ( nIndex, csv.GetDouble ( n++ ) );
		SetSuppRef ( nIndex, csv.GetString ( n++ ) );
		n++;	//SKIP OBSOLETE ON ORDER COUNT
		m_nSupplierCount++;
	}
}

/**********************************************************************/

void CStockCSVRecord::V2ConvertFromCSV ( CCSV& csv )
{
	int n = 4;
	SetStockUnitName ( csv.GetString(n++) );
	SetSubUnits ( csv.GetInt(n++) );
	SetSubUnitName ( csv.GetString (n++) );
	SetWastePercent ( csv.GetDouble(n++) );
	SetDisableAutoOrderFlag ( csv.GetBool(n++) );
	SetDisablePluRangeFlag ( csv.GetBool(n++) );
	
	int nSupplierTableSize = csv.GetInt ( n++ );
	for ( int nIndex = 0; nIndex < nSupplierTableSize; nIndex++ )
	{
		if ( nIndex >= MAX_ITEM_SUPPLIERS )
			break;

		int nSuppNo = csv.GetInt ( n++ );
		if ( nSuppNo <= 0 )
			break;
			
		SetSuppNo ( nIndex, nSuppNo );
		SetCase ( nIndex, csv.GetString ( n++ ) );
		SetDUItems ( nIndex, csv.GetInt ( n++ ) );
		SetDUItemSize ( nIndex, csv.GetDouble ( n++ ) );
		SetCost ( nIndex, csv.GetDouble ( n++ ) );
		SetSuppRef ( nIndex, csv.GetString ( n++ ) );
		m_nSupplierCount++;
	}
}

/**********************************************************************/

void CStockCSVRecord::V3to5ConvertFromCSV ( CCSV& csv )
{
	int n = 4;
	SetStockUnitName ( csv.GetString(n++) );
	SetSubUnits ( csv.GetInt(n++) );
	SetSubUnitName ( csv.GetString (n++) );
	SetWastePercent ( csv.GetDouble(n++) );
	SetDisableAutoOrderFlag ( csv.GetBool(n++) );
	SetDisablePluRangeFlag ( csv.GetBool(n++) );
	SetAutoLabelFlag( csv.GetBool(n++) );
	SetLocalSupplierFlag( csv.GetBool(n++) );
	
	int nSupplierTableSize = csv.GetInt ( n++ );
	for ( int nIndex = 0; nIndex < nSupplierTableSize; nIndex++ )
	{
		if ( nIndex >= MAX_ITEM_SUPPLIERS )
			break;

		int nSuppNo = csv.GetInt ( n++ );
		if ( nSuppNo <= 0 )
			break;
			
		SetSuppNo ( nIndex, nSuppNo );
		SetCase ( nIndex, csv.GetString ( n++ ) );
		SetDUItems ( nIndex, csv.GetInt ( n++ ) );
		SetDUItemSize ( nIndex, csv.GetDouble ( n++ ) );
		SetCost ( nIndex, csv.GetDouble ( n++ ) );
		SetSuppRef ( nIndex, csv.GetString ( n++ ) );
		m_nSupplierCount++;
	}
}

/**********************************************************************/

void CStockCSVRecord::V6to7ConvertFromCSV ( CCSV& csv )
{	
	int n = 4;
	SetStockUnitName ( csv.GetString(n++) );
	SetSubUnits ( csv.GetInt(n++) );
	SetSubUnitName ( csv.GetString (n++) );
	SetSUWeightType( csv.GetInt(n++) );
	SetSUWeight( csv.GetDouble(n++) );
	SetWastePercent ( csv.GetDouble(n++) );
	SetDisableAutoOrderFlag ( csv.GetBool(n++) );
	SetDisablePluRangeFlag ( csv.GetBool(n++) );
	SetAutoLabelFlag( csv.GetBool(n++) );
	SetLocalSupplierFlag( csv.GetBool(n++) );
	
	int nSupplierTableSize = csv.GetInt ( n++ );
	for ( int nIndex = 0; nIndex < nSupplierTableSize; nIndex++ )
	{
		if ( nIndex >= MAX_ITEM_SUPPLIERS )
			break;

		int nSuppNo = csv.GetInt ( n++ );
		if ( nSuppNo <= 0 )
			break;
			
		SetSuppNo ( nIndex, nSuppNo );
		SetCase ( nIndex, csv.GetString ( n++ ) );
		SetDUItems ( nIndex, csv.GetInt ( n++ ) );
		SetDUItemSize ( nIndex, csv.GetDouble ( n++ ) );
		SetCost ( nIndex, csv.GetDouble ( n++ ) );
		SetSuppRef ( nIndex, csv.GetString ( n++ ) );
		m_nSupplierCount++;
	}
}

/**********************************************************************/

void CStockCSVRecord::V8to14ConvertFromCSV ( CCSV& csv, int nVer )
{	
	int n = 4;

	if (nVer >= 12)
	{
		SetImportSheetCode(csv.GetString(n++));
	}

	if ( nVer >= 10 )
	{
		SetCustomFieldValueNo( 1, csv.GetInt(n++) );
		SetCustomFieldValueNo( 2, csv.GetInt(n++) );
		SetCustomFieldValueNo( 3, csv.GetInt(n++) );
		SetCustomFieldValueNo( 4, csv.GetInt(n++) );
		SetCustomFieldValueNo( 5, csv.GetInt(n++) );
	}

	SetStockUnitName ( csv.GetString(n++) );

	if (nVer >= 14)
	{
		SetStockmateText(csv.GetString(n++));
	}

	SetSubUnits ( csv.GetInt(n++) );
	SetSubUnitName ( csv.GetString (n++) );
	SetSUWeightType( csv.GetInt(n++) );
	SetSUWeight( csv.GetDouble(n++) );
	SetWastePercent ( csv.GetDouble(n++) );
	SetDisableAutoOrderFlag ( csv.GetBool(n++) );
	SetDisablePluRangeFlag ( csv.GetBool(n++) );
	SetAutoLabelFlag( csv.GetBool(n++) );
	SetLocalSupplierFlag( csv.GetBool(n++) );
	
	int nSupplierTableSize = csv.GetInt ( n++ );
	for ( int nIndex = 0; nIndex < nSupplierTableSize; nIndex++ )
	{
		if (nIndex >= MAX_ITEM_SUPPLIERS)
		{
			break;
		}

		int nSuppNo = csv.GetInt ( n++ );
		if (nSuppNo <= 0)
		{
			break;
		}
			
		SetSuppNo ( nIndex, nSuppNo );
		SetCase ( nIndex, csv.GetString ( n++ ) );
		SetDUItems ( nIndex, csv.GetInt ( n++ ) );
		SetDUItemSize ( nIndex, csv.GetDouble ( n++ ) );
		SetCost ( nIndex, csv.GetDouble ( n++ ) );
		SetSuppRef ( nIndex, csv.GetString ( n++ ) );
		SetWrapper( nIndex, csv.GetString( n++ ) );

		if (nVer >= 9)
		{
			SetRetail(nIndex, csv.GetDouble(n++));
		}

		m_nSupplierCount++;
	}
}

/**********************************************************************/
//	Get header
/**********************************************************************/

CString CStockCSVRecord::GetHeader ( int nSupplierCount )
{
	CString strHeader;
	CString strBuf;

	CCSV csv;
	csv.Add ( Stock::StockCode.Label );
	csv.Add ( Stock::Description.Label );
	csv.Add ( Stock::Category.Label );
	csv.Add ( "" );				//Protected version field

	csv.Add ( Stock::ImportSheetCode.Label );
	CString strCustomField = CustomFieldValue::Key.Label;
	csv.Add ( strCustomField + "1" );
	csv.Add ( strCustomField + "2" );
	csv.Add ( strCustomField + "3" );
	csv.Add ( strCustomField + "4" );
	csv.Add ( strCustomField + "5" );

	csv.Add ( Stock::StockUnit.Label );
	csv.Add ( Stock::StockmateText.Label);
	csv.Add ( Stock::SUYield.Label );
	csv.Add ( Stock::SubUnit.Label );
	csv.Add ( Stock::SUWeightType.Label );
	csv.Add ( Stock::SUWeight.Label );
	csv.Add ( Stock::WastagePercent.Label );
	csv.Add ( Stock::DisableOrder.Label );
	csv.Add ( Stock::DisableRange.Label );
	csv.Add ( Stock::AutoLabel.Label );
	csv.Add ( "" );				//Protected local supplier field
	csv.Add ( "" );				//Protected supplier count field

	CString strSuppKey = Supplier::SupplierKey.Label;
	CString strDelUnit = Stock::DeliveryUnit.Label;
	CString strDUItems = Stock::DUItems.Label;
	CString strDUItemSize = Stock::DUItemSize.Label;
	CString strCost = Stock::DUCost.Label;
	CString strSuppRef = Stock::SupplierRef.Label;
	CString strWrapper = Stock::Wrapper.Label;
	CString strRetail = Stock::DURetail.Label;

	for ( int nSuppIdx = 0; nSuppIdx < nSupplierCount; nSuppIdx++ )
	{
		strBuf.Format ( "%1.1d", nSuppIdx + 1 );

		csv.Add ( strSuppKey + strBuf );
		csv.Add ( strDelUnit + strBuf );
		csv.Add ( strDUItems + strBuf );
		csv.Add ( strDUItemSize + strBuf );
		csv.Add ( strCost + strBuf );
		csv.Add ( strSuppRef + strBuf );
		csv.Add ( strWrapper + strBuf );
		csv.Add ( strRetail + strBuf );
	}

	strHeader = csv.GetLine();
	return strHeader;
}

/**********************************************************************/
//	Import Unit
/**********************************************************************/

CString CStockCSVRecord::StripSpaces ( const char* szText )
{
	CString strText = szText;
	::TrimSpaces ( strText, FALSE );
	return strText;
}

/**********************************************************************/

void CStockCSVRecord::ImportLine ( CSSImportFile& import, CCSV& csv, int nSupplierCount, CSupplierCSVArray& SupplierArray, CPtrArray& CustomFieldArrays )
{
	m_nSupplierCount = nSupplierCount;
	
	int n = 0;
	if ( import.IsRequired(n) == TRUE ) Import ( StripSpaces ( csv.GetString(n) ), m_strStockCode, Stock::StockCode ); n++;
	if ( import.IsRequired(n) == TRUE ) Import ( StripSpaces ( csv.GetString(n) ), m_strDescription, Stock::Description ); n++;
	if ( import.IsRequired(n) == TRUE ) Import ( csv.GetString(n), m_nCategory, Stock::Category ); n++;
	n++;		//protected version number

	if ( import.IsRequired(n) == TRUE ) Import ( csv.GetString(n), m_strImportSheetCode, Stock::ImportSheetCode ); n++;
	ImportCustomFieldKey( import, n, csv, 1, CustomFieldArrays );	n++;
	ImportCustomFieldKey( import, n, csv, 2, CustomFieldArrays );	n++;
	ImportCustomFieldKey( import, n, csv, 3, CustomFieldArrays );	n++;
	ImportCustomFieldKey( import, n, csv, 4, CustomFieldArrays );	n++;
	ImportCustomFieldKey( import, n, csv, 5, CustomFieldArrays );	n++;

	if ( import.IsRequired(n) == TRUE ) Import ( StripSpaces ( csv.GetString(n) ), m_strStockUnit, Stock::StockUnit ); n++;
	if ( import.IsRequired(n) == TRUE ) Import ( StripSpaces ( csv.GetString(n) ), m_strStockmateText, Stock::StockmateText); n++;
	if ( import.IsRequired(n) == TRUE ) Import ( csv.GetString(n), m_nSubUnits, Stock::SUYield ); n++;
	if ( import.IsRequired(n) == TRUE ) Import ( StripSpaces ( csv.GetString (n) ), m_strSubUnit, Stock::SubUnit ); n++;
	if ( import.IsRequired(n) == TRUE ) SetSUWeightType ( csv.GetInt(n) ); n++;
	if ( import.IsRequired(n) == TRUE ) Import ( csv.GetString(n), m_dSUWeight, Stock::SUWeight ); n++;
	if ( import.IsRequired(n) == TRUE ) Import ( csv.GetString(n), m_dWastePercent, Stock::WastagePercent ); n++;
	if ( import.IsRequired(n) == TRUE ) Import ( csv.GetString(n), m_bDisableAutoOrder ); n++;
	if ( import.IsRequired(n) == TRUE ) Import ( csv.GetString(n), m_bDisablePluRange ); n++;
	if ( import.IsRequired(n) == TRUE ) Import ( csv.GetString(n), m_bAutoLabel ); n++;
	n++;		//protected local supplier flag
	n++;		//protected supplier count flag

	if (m_bLocalSupplier == TRUE)
	{
		return;
	}

	for ( int nSuppIdx = 0; nSuppIdx < nSupplierCount; nSuppIdx++ )
	{
		int nSuppNo = GetSuppNo ( nSuppIdx );
		int nLocalSuppNo = nSuppNo;
		CString strCase = GetCase ( nSuppIdx );
		int nDUItems = GetDUItems ( nSuppIdx );
		double dDUItemSize = GetDUItemSize( nSuppIdx );
		double dCost = GetCost ( nSuppIdx );
		CString strSuppRef = GetSuppRefInternal ( nSuppIdx );
		CString strWrapper = GetWrapper ( nSuppIdx );
		double dRetail = GetRetail( nSuppIdx );

		if ( import.IsRequired(n) )
		{
			CString strSupplierKey;	
			Import ( csv.GetString(n), strSupplierKey, Supplier::SupplierKey );
			::TrimSpaces ( strSupplierKey, FALSE );

			if ( strSupplierKey != "" )
			{	
				int nSuppIdx;
				if (SupplierArray.FindSupplierByKey(strSupplierKey, nSuppIdx) == TRUE)
				{
					nLocalSuppNo = SupplierArray.GetSuppNo(nSuppIdx);
				}
				else
				{
					nLocalSuppNo = SupplierArray.FindFreeNumber();
					if (nLocalSuppNo == 0)
					{
						nLocalSuppNo = nSuppNo;
					}
					else
					{
						CSupplierCSVRecord Supplier;
						Supplier.SetKey ( strSupplierKey );
						Supplier.SetSuppNo ( nLocalSuppNo );
						Supplier.SetName ( strSupplierKey );
						SupplierArray.AddSupplier( Supplier );
					}
				}
			}
		}
		n++;
			
		if ( import.IsRequired(n) == TRUE ) Import ( StripSpaces ( csv.GetString(n) ), strCase, Stock::DeliveryUnit ); n++;
		if ( import.IsRequired(n) == TRUE ) Import ( csv.GetString(n), nDUItems, Stock::DUItems ); n++;
		if ( import.IsRequired(n) == TRUE ) Import ( csv.GetString(n), dDUItemSize, Stock::DUItemSize ); n++;
		if ( import.IsRequired(n) == TRUE ) Import ( csv.GetString(n), dCost, Stock::DUCost ); n++;
		if ( import.IsRequired(n) == TRUE ) Import ( StripSpaces ( csv.GetString(n) ), strSuppRef, Stock::SupplierRef ); n++;
		if ( import.IsRequired(n) == TRUE ) Import ( StripSpaces ( csv.GetString(n) ), strWrapper, Stock::Wrapper ); n++;
		if ( import.IsRequired(n) == TRUE ) Import ( csv.GetString(n), dRetail, Stock::DURetail ); n++;

		SetSuppNo ( nSuppIdx, nLocalSuppNo );
		SetCase ( nSuppIdx, strCase );
		SetDUItems ( nSuppIdx, nDUItems );
		SetDUItemSize ( nSuppIdx, dDUItemSize );
		SetCost ( nSuppIdx, dCost );
		SetSuppRef ( nSuppIdx, strSuppRef );
		SetWrapper ( nSuppIdx, strWrapper );
		SetRetail( nSuppIdx, dRetail );
	}
}

/**********************************************************************/

void CStockCSVRecord::ImportCustomFieldKey( CSSImportFile& import, int n, CCSV& csv, int nFieldNo, CPtrArray& CustomFieldArrays )
{
	//NOTHING TO DO IF IMPORT NOT REQUIRED
	if (import.IsRequired(n) == FALSE)
	{
		return;
	}

	//NOTHING TO DO IF FIELD NUMBER OUT OF RANGE
	if ((nFieldNo < 1) || (nFieldNo > CustomFieldArrays.GetSize()))
	{
		return;
	}

	CString strKey;	
	Import ( csv.GetString(n), strKey, CustomFieldValue::Key );
	::TrimSpaces ( strKey, FALSE );

	//NOTHING TO DO IF IMPORT KEY IS BLANK
	if (strKey == "")
	{
		return;
	}

	//RESET CUSTOM FIELD VALUE TO ZERO IF IMPORT FIELD IS N/A
	if ( strKey == "N/A" )
	{
		SetCustomFieldValueNo( nFieldNo, 0 );
		return;
	}
		
	CCustomFieldValueCSVArray* pArray = (CCustomFieldValueCSVArray*) CustomFieldArrays.GetAt( nFieldNo - 1 );

	//SET CUSTOM FIELD TO APPROPRIATE LOCAL NUMBER IF KEY IS FOUND
	int nValueIdx;
	if ( pArray -> FindValueByKey ( strKey, nValueIdx ) == TRUE )
	{
		SetCustomFieldValueNo( nFieldNo, pArray -> GetNumber ( nValueIdx ) );
		return;
	}

	//CREATE NEW KEY WITH NEXT AVAILABLE LOCAL NUMBER IF KEY NOT FOUND
	int nNewValueNo = pArray -> FindFreeNumber();
	if ( nNewValueNo != 0 )
	{
		CCustomFieldValueCSVRecord Value;
		Value.SetKey ( strKey );
		Value.SetNumber ( nNewValueNo );
		Value.SetName ( strKey );
		pArray -> AddValue( Value );
		
		SetCustomFieldValueNo( nFieldNo, nNewValueNo );
	}
}
	
/**********************************************************************/
//	Stock Code
/**********************************************************************/

void CStockCSVRecord::SetStockCode ( const char* szCode )
{
	Set ( m_strStockCode, StripSpaces ( szCode ), Stock::StockCode );
}

/**********************************************************************/
//	Description
/**********************************************************************/

void CStockCSVRecord::SetDescription ( const char* szDescription )
{
	CString strDescription = szDescription;
	::TrimSpaces ( strDescription, FALSE );

	if (strDescription == "")
	{
		strDescription = m_strStockCode;
	}

	Set ( m_strDescription, strDescription, Stock::Description );
}

/**********************************************************************/
//	Category
/**********************************************************************/

void CStockCSVRecord::SetCategory ( int nCategory )
{
	Set ( m_nCategory, nCategory, Stock::Category );
}

/**********************************************************************/
//	Stock Sheet Code
/**********************************************************************/

void CStockCSVRecord::SetImportSheetCode ( const char* szCode )
{
	Set ( m_strImportSheetCode, StripSpaces ( szCode ), Stock::ImportSheetCode );
}

/**********************************************************************/
//	Custom field
/**********************************************************************/

void CStockCSVRecord::SetCustomFieldValueNo(int nFieldNo, int nNo)
{
	if ((nFieldNo >= 1) && (nFieldNo <= MAX_ITEM_CUSTOM_FIELDS))
	{
		Set(m_nCustomFieldValueNo[nFieldNo - 1], nNo, Stock::CustomField);
	}
}

/**********************************************************************/
//	StockUnit
/**********************************************************************/

void CStockCSVRecord::SetStockUnitName ( const char* szName )
{
	Set ( m_strStockUnit, StripSpaces ( szName ), Stock::StockUnit );
}

/**********************************************************************/
//	StockUnit
/**********************************************************************/

void CStockCSVRecord::SetStockmateText(const char* szText)
{
	Set(m_strStockmateText, StripSpaces(szText), Stock::StockmateText);
}

/**********************************************************************/
//	SubUnit
/**********************************************************************/

void CStockCSVRecord::SetSubUnitName ( const char* szName )
{
	Set ( m_strSubUnit, StripSpaces ( szName ), Stock::StockUnit );
}

/**********************************************************************/
//	SU Weight
/**********************************************************************/

void CStockCSVRecord::SetSUWeightType ( int nType )
{	
	switch ( nType )
	{
	case QTYWGHT_GALLON_GALLON: 
	case QTYWGHT_GALLON_PINT:
	case QTYWGHT_GALLON_FLOUNCE:
	case QTYWGHT_GALLON_LITRE:
	case QTYWGHT_GALLON_CLITRE:
	case QTYWGHT_GALLON_MLITRE:
	case QTYWGHT_PINT_PINT:
	case QTYWGHT_PINT_FLOUNCE:
	case QTYWGHT_PINT_LITRE:
	case QTYWGHT_PINT_CLITRE:
	case QTYWGHT_PINT_MLITRE:
	case QTYWGHT_FLOUNCE_FLOUNCE:
	case QTYWGHT_LITRE_PINT:
	case QTYWGHT_LITRE_FLOUNCE:
	case QTYWGHT_LITRE_LITRE:
	case QTYWGHT_LITRE_CLITRE:
	case QTYWGHT_LITRE_MLITRE:
	case QTYWGHT_CLITRE_CLITRE:
	case QTYWGHT_CLITRE_MLITRE:
	case QTYWGHT_MLITRE_MLITRE:
	case QTYWGHT_KILO_KILO:
	case QTYWGHT_KILO_GRAM:
	case QTYWGHT_GRAM_GRAM:
		m_nSUWeightType = nType;
		break;
	}
}

/**********************************************************************/

void CStockCSVRecord::SetSUWeightType ( int nUnitType, int nSaleType )
{
	SetSUWeightType( ( nUnitType * 100 ) + nSaleType );
}

/**********************************************************************/

void CStockCSVRecord::SetSUWeight ( double dWeight )
{
	Set ( m_dSUWeight, dWeight, Stock::SUWeight );
}

/**********************************************************************/

double CStockCSVRecord::GetSUWeight( bool bEnabled )
{
	if ( bEnabled == TRUE )
		return m_dSUWeight;
	else
		return 0.0;
}

/**********************************************************************/
//	Wastage
/**********************************************************************/

void CStockCSVRecord::SetWastePercent ( double dValue )
{
	Set ( m_dWastePercent, dValue, Stock::WastagePercent );
}

/**********************************************************************/
//	SU Yield
/**********************************************************************/

void CStockCSVRecord::SetSubUnits ( int nQty )
{
	Set ( m_nSubUnits, nQty, Stock::SUYield );
}

int CStockCSVRecord::GetSubUnits( bool bEnabled )
{
	if ( bEnabled )
		return m_nSubUnits;
	else
		return 1;
}

/**********************************************************************/
//	Flags
/**********************************************************************/

void CStockCSVRecord::SetDisableAutoOrderFlag ( bool bFlag )
{
	m_bDisableAutoOrder = bFlag;
}

void CStockCSVRecord::SetDisablePluRangeFlag ( bool bFlag )
{
	m_bDisablePluRange = bFlag;
}

void CStockCSVRecord::SetAutoLabelFlag( bool bFlag )
{
	m_bAutoLabel = bFlag;
}

void CStockCSVRecord::SetLocalSupplierFlag( bool bFlag )
{
	m_bLocalSupplier = bFlag;
}

/**********************************************************************/
//	Custom field
/**********************************************************************/

int CStockCSVRecord::GetCustomFieldValueNo( int nFieldNo )
{
	if ( ( nFieldNo >= 1 ) && ( nFieldNo <= MAX_ITEM_CUSTOM_FIELDS ) )
		return m_nCustomFieldValueNo[nFieldNo - 1];
	else
		return 0;
}

/**********************************************************************/
//	Unit Cost
/**********************************************************************/

double CStockCSVRecord::GetUnitCost()
{
	return GetUnitCost ( 0 );
}

/**********************************************************************/

double CStockCSVRecord::GetUnitCost ( int nIndex )
{
	double dUnitCost = 0.0;

	if ( nIndex >= 0 && nIndex < MAX_ITEM_SUPPLIERS )
	{	
		double dCost = GetCost( nIndex );
		double dYield = GetYield ( nIndex );

		if ( dYield != 0 ) 
			dUnitCost = ( dCost / dYield );
		else
			dUnitCost = 0.0;
	}
	return dUnitCost;
}

/**********************************************************************/
//Unit Retail
/**********************************************************************/

double CStockCSVRecord::GetUnitRetail()
{
	return GetUnitRetail ( 0 );
}

/**********************************************************************/

double CStockCSVRecord::GetUnitRetail ( int nIndex )
{
	double dUnitRetail = 0.0;

	if ( nIndex >= 0 && nIndex < MAX_ITEM_SUPPLIERS )
	{	
		double dRetail = GetRetail( nIndex );
		double dYield = GetYield ( nIndex );

		if ( dYield != 0 ) 
			dUnitRetail = ( dRetail / dYield );
		else
			dUnitRetail = 0.0;
	}
	return dUnitRetail;
}

/**********************************************************************/

int CStockCSVRecord::GetSuppNo( int nIndex )
{
	if ( nIndex >= 0 && nIndex < MAX_ITEM_SUPPLIERS )
		return Suppliers[nIndex].m_nSuppNo;

	return 0;
}

/**********************************************************************/

const char* CStockCSVRecord::GetCase( int nIndex )
{
	if ( nIndex >= 0 && nIndex < MAX_ITEM_SUPPLIERS )
		return Suppliers[nIndex].m_strCase;
		
	return "";
}

/**********************************************************************/

int CStockCSVRecord::GetDUItems( int nIndex )
{
	if ( nIndex >= 0 && nIndex < MAX_ITEM_SUPPLIERS )
		return Suppliers[nIndex].m_nDUItems;

	return 1;
}

/**********************************************************************/

double CStockCSVRecord::GetDUItemSize( int nIndex )
{
	if ( nIndex >= 0 && nIndex < MAX_ITEM_SUPPLIERS )
		return Suppliers[nIndex].m_dDUItemSize;

	return 1.0;
}

/**********************************************************************/

double CStockCSVRecord::GetCost( int nIndex )
{
	if ( nIndex >= 0 && nIndex < MAX_ITEM_SUPPLIERS )
		return Suppliers[nIndex].m_dCost;

	return 0.0;
}

/**********************************************************************/

const char* CStockCSVRecord::GetSuppRefInternal( int nIndex )
{
	if ( nIndex >= 0 && nIndex < MAX_ITEM_SUPPLIERS )
		return Suppliers[nIndex].m_strSuppRef;

	return "";
}

/**********************************************************************/

const char* CStockCSVRecord::GetSuppRefExternal( int nIndex, bool bUseStockCode )
{
	if ( FALSE == bUseStockCode )
	{
		if ( nIndex >= 0 && nIndex < MAX_ITEM_SUPPLIERS )
			return Suppliers[nIndex].m_strSuppRef;
		else
			return "";
	}
	else
		return m_strStockCode;
}

/**********************************************************************/

const char* CStockCSVRecord::GetWrapper( int nIndex )
{
	if ( nIndex >= 0 && nIndex < MAX_ITEM_SUPPLIERS )
		return Suppliers[nIndex].m_strWrapper;

	return "";
}

/**********************************************************************/

double CStockCSVRecord::GetRetail( int nIndex )
{
	if ( nIndex >= 0 && nIndex < MAX_ITEM_SUPPLIERS )
		return Suppliers[nIndex].m_dRetail;

	return 0.0;
}

/**********************************************************************/

void CStockCSVRecord::SetSuppNo ( int nIndex, int nSuppNo )
{
	if ( nIndex >= 0 && nIndex < MAX_ITEM_SUPPLIERS )
		Set ( Suppliers[nIndex].m_nSuppNo, nSuppNo, Stock::SupplierNo );
}

/**********************************************************************/

void CStockCSVRecord::SetCase ( int nIndex, const char* szCase )
{
	if ( nIndex >= 0 && nIndex < MAX_ITEM_SUPPLIERS )
		Set ( Suppliers[nIndex].m_strCase, StripSpaces ( szCase ), Stock::DeliveryUnit );
}

/**********************************************************************/

void CStockCSVRecord::SetDUItems ( int nIndex, int nItems )
{
	if ( nIndex >= 0 && nIndex < MAX_ITEM_SUPPLIERS )
		Set ( Suppliers[nIndex].m_nDUItems, nItems, Stock::DUItems );
}

/**********************************************************************/

void CStockCSVRecord::SetDUItemSize ( int nIndex, double dSize )
{
	if ( nIndex >= 0 && nIndex < MAX_ITEM_SUPPLIERS )
		Set ( Suppliers[nIndex].m_dDUItemSize, dSize, Stock::DUItemSize );
}

/**********************************************************************/

void CStockCSVRecord::SetCost ( int nIndex, double dCost )
{
	if ( nIndex >= 0 && nIndex < MAX_ITEM_SUPPLIERS )
		Set ( Suppliers[nIndex].m_dCost, dCost, Stock::DUCost );
}

/**********************************************************************/

void CStockCSVRecord::SetSuppRef ( int nIndex, const char* szSuppRef )
{
	if ( nIndex >= 0 && nIndex < MAX_ITEM_SUPPLIERS )
		Set ( Suppliers[nIndex].m_strSuppRef, StripSpaces ( szSuppRef ), Stock::SupplierRef );
}

/**********************************************************************/

void CStockCSVRecord::SetWrapper ( int nIndex, const char* szWrapper )
{
	if ( nIndex >= 0 && nIndex < MAX_ITEM_SUPPLIERS )
		Set ( Suppliers[nIndex].m_strWrapper, StripSpaces ( szWrapper ), Stock::Wrapper );
}

/**********************************************************************/

void CStockCSVRecord::SetRetail ( int nIndex, double dRetail )
{
	if ( nIndex >= 0 && nIndex < MAX_ITEM_SUPPLIERS )
		Set ( Suppliers[nIndex].m_dRetail, dRetail, Stock::DURetail );
}

/**********************************************************************/

int CStockCSVRecord::PurgeSupplier ( int nSuppNo, int nUserMaxSuppliers )
{	
	int nResult = SUPPLIER_PURGE_NOCHANGE;

	for ( int nSuppIdx = 0; nSuppIdx < MAX_ITEM_SUPPLIERS; nSuppIdx++ )
	{
		if ( GetSuppNo ( nSuppIdx ) == nSuppNo )
		{
			if ( nResult == SUPPLIER_PURGE_NOCHANGE )
				nResult = SUPPLIER_PURGE_CHANGESTOCK;

			ClearSupplier( nSuppIdx );
			
			if ( nSuppIdx == 0 )
				nResult = SUPPLIER_PURGE_CHANGEPLU;
		}
	}

	if ( nResult != SUPPLIER_PURGE_NOCHANGE )
		TidySuppliers( nUserMaxSuppliers );

	return nResult;	
}

/**********************************************************************/

double CStockCSVRecord::GetQtyToWeightFactor()
{
	double dMultiplier = 1.0;

	switch( GetUnitWeightType() )
	{
	case WEIGHTTYPE_GALLON:
		switch( GetSaleWeightType() )
		{
		case WEIGHTTYPE_GALLON:		dMultiplier = 1.0;		break;
		case WEIGHTTYPE_PINT:		dMultiplier = 8.0;		break;
		case WEIGHTTYPE_FLOUNCE:		dMultiplier = 160.0;	break;
		case WEIGHTTYPE_LITRE:		dMultiplier = 4.54609;	break;
		case WEIGHTTYPE_CLITRE:		dMultiplier = 454.609;	break;
		case WEIGHTTYPE_MLITRE:		dMultiplier = 4546.09;	break;
		}
		break;

	case WEIGHTTYPE_PINT:
		switch( GetSaleWeightType() )
		{
		case WEIGHTTYPE_PINT:		dMultiplier = 1.0;			break;
		case WEIGHTTYPE_FLOUNCE:		dMultiplier = 20.0;			break;
		case WEIGHTTYPE_LITRE:		dMultiplier = 0.56826125;	break;
		case WEIGHTTYPE_CLITRE:		dMultiplier = 56.826125;		break;
		case WEIGHTTYPE_MLITRE:		dMultiplier = 568.26125;		break;
		}
		break;

	case WEIGHTTYPE_FLOUNCE:
		switch ( GetSaleWeightType() )
		{
		case WEIGHTTYPE_FLOUNCE:	dMultiplier = 1.0;		break;
		}
		break;

	case WEIGHTTYPE_LITRE:
		switch( GetSaleWeightType() )
		{
		case WEIGHTTYPE_LITRE:		dMultiplier = 1.0;		break;
		case WEIGHTTYPE_CLITRE:		dMultiplier = 100.0;	break;
		case WEIGHTTYPE_MLITRE:		dMultiplier = 1000.0;	break;
		case WEIGHTTYPE_PINT:		dMultiplier = 1.75975;	break;
		case WEIGHTTYPE_FLOUNCE:		dMultiplier = 35.195;	break;
		}
		break;

	case WEIGHTTYPE_CLITRE:
		switch( GetSaleWeightType() )
		{
		case WEIGHTTYPE_CLITRE:		dMultiplier = 1.0;		break;
		case WEIGHTTYPE_MLITRE:		dMultiplier = 10.0;		break;
		}
		break;

	case WEIGHTTYPE_MLITRE:
		switch( GetSaleWeightType() )
		{
		case WEIGHTTYPE_MLITRE:		dMultiplier = 1.0;		break;
		}
		break;

	case WEIGHTTYPE_KILO:
		switch( GetSaleWeightType() )
		{
		case WEIGHTTYPE_KILO:		dMultiplier = 1.0;		break;
		case WEIGHTTYPE_GRAM:		dMultiplier = 1000.0;	break;
		}
		break;

	case WEIGHTTYPE_GRAM:
		switch( GetSaleWeightType() )
		{
		case WEIGHTTYPE_GRAM:		dMultiplier = 1.0;		break;
		}
		break;
	}

	return m_dSUWeight * dMultiplier;
}

/**********************************************************************/

void CStockCSVRecord::GetValidSaleWeights( int nUnitWeight, CWordArray& SaleWeights )
{	
	SaleWeights.RemoveAll();

	switch( nUnitWeight )
	{
	case WEIGHTTYPE_GALLON:
		SaleWeights.Add ( WEIGHTTYPE_GALLON );
		SaleWeights.Add ( WEIGHTTYPE_PINT );
		SaleWeights.Add ( WEIGHTTYPE_FLOUNCE );
		SaleWeights.Add ( WEIGHTTYPE_LITRE );
		SaleWeights.Add ( WEIGHTTYPE_CLITRE );
		SaleWeights.Add ( WEIGHTTYPE_MLITRE );
		break;
	
	case WEIGHTTYPE_PINT:
		SaleWeights.Add ( WEIGHTTYPE_PINT );
		SaleWeights.Add ( WEIGHTTYPE_FLOUNCE );
		SaleWeights.Add ( WEIGHTTYPE_LITRE );
		SaleWeights.Add ( WEIGHTTYPE_CLITRE );
		SaleWeights.Add ( WEIGHTTYPE_MLITRE );
		break;

	case WEIGHTTYPE_FLOUNCE:
		SaleWeights.Add ( WEIGHTTYPE_FLOUNCE );
		break;
	
	case WEIGHTTYPE_LITRE:
		SaleWeights.Add ( WEIGHTTYPE_LITRE );
		SaleWeights.Add ( WEIGHTTYPE_CLITRE );
		SaleWeights.Add ( WEIGHTTYPE_MLITRE );
		SaleWeights.Add ( WEIGHTTYPE_PINT );
		SaleWeights.Add ( WEIGHTTYPE_FLOUNCE );
		break;

	case WEIGHTTYPE_CLITRE:
		SaleWeights.Add ( WEIGHTTYPE_CLITRE );
		SaleWeights.Add ( WEIGHTTYPE_MLITRE );
		break;

	case WEIGHTTYPE_MLITRE:
		SaleWeights.Add ( WEIGHTTYPE_MLITRE );
		break;

	case WEIGHTTYPE_KILO:
		SaleWeights.Add ( WEIGHTTYPE_KILO );
		SaleWeights.Add ( WEIGHTTYPE_GRAM );
		break;

	case WEIGHTTYPE_GRAM:
		SaleWeights.Add ( WEIGHTTYPE_GRAM );
		break;
	}
}
	
/**********************************************************************/
	
void CStockCSVRecord::GetUnitAndSaleWeights( int nWeightType, int& nUnitWeight, int& nSaleWeight )
{
	switch ( nWeightType )
	{
	case QTYWGHT_GALLON_GALLON:
	case QTYWGHT_GALLON_PINT:
	case QTYWGHT_GALLON_FLOUNCE:
	case QTYWGHT_GALLON_LITRE:
	case QTYWGHT_GALLON_CLITRE:
	case QTYWGHT_GALLON_MLITRE:
	case QTYWGHT_PINT_PINT:
	case QTYWGHT_PINT_FLOUNCE:
	case QTYWGHT_PINT_LITRE:
	case QTYWGHT_PINT_CLITRE:
	case QTYWGHT_PINT_MLITRE:
	case QTYWGHT_FLOUNCE_FLOUNCE:
	case QTYWGHT_LITRE_PINT:
	case QTYWGHT_LITRE_FLOUNCE:
	case QTYWGHT_LITRE_LITRE:
	case QTYWGHT_LITRE_CLITRE:
	case QTYWGHT_LITRE_MLITRE:
	case QTYWGHT_CLITRE_CLITRE:
	case QTYWGHT_CLITRE_MLITRE:
	case QTYWGHT_MLITRE_MLITRE:
	case QTYWGHT_KILO_KILO:
	case QTYWGHT_KILO_GRAM:
	case QTYWGHT_GRAM_GRAM:
		nUnitWeight = nWeightType / 100;
		nSaleWeight = nWeightType % 100;

	default:
		nUnitWeight = WEIGHTTYPE_GALLON;
		nSaleWeight = WEIGHTTYPE_GALLON;
		break;
	}
}
	
/**********************************************************************/

	
	
