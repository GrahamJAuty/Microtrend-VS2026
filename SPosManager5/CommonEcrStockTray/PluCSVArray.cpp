/**********************************************************************/
#include "ImportDefines.h"
#include "ReportHelpers.h"
/**********************************************************************/
#include "PluCSVArray.h"
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "..\\SPosStockManager5\StockPluArray.h"
/**********************************************************************/
#endif
/**********************************************************************/

CEODPluImportInfo::CEODPluImportInfo()
{
	m_strImportLine = "";
	m_bImportIfExists = FALSE;
	m_nExistingMask = 0;
	m_nFieldMask = 0;
	m_bImportIfNew = FALSE;
	m_nImportModifierMode = 0;
	m_nResult = 0;
	m_nLastBasePluNo = -1;
	m_nNewPluDeptNo = 0;
	m_strNewPluText = "";
};

/**********************************************************************/

CPluCSVArray::CPluCSVArray() : CSharedCSVArray ( ',', '"', FALSE, TRUE, TRUE )
{
}

/**********************************************************************/

bool CPluCSVArray::FindPluByNumber ( __int64 nPluNo, int& nIndex )
{
	int nStart = 0;
	int nEnd = GetSize() - 1;

	while ( nStart <= nEnd )
	{
		nIndex = ( nStart + nEnd ) / 2;

		__int64 nArrayPluNo = GetPluNoInt ( nIndex );
		
		if ( nArrayPluNo < nPluNo )
			nStart = nIndex + 1;

		if ( nArrayPluNo > nPluNo )
			nEnd = nIndex - 1;

		if ( nArrayPluNo == nPluNo )
			return TRUE;
	}
		
	nIndex = nStart;
	return FALSE;
}

/**********************************************************************/

const char* CPluCSVArray::GetPluNoString ( int nIndex )
{
	CString strTemp;

	if ( ( strTemp = GetAt ( nIndex ) ) != "" )
	{
		CCSV csv ( strTemp, ',', '"', 1 );
		m_strPluNo = csv.GetString ( 0 );
		::AddLeading ( m_strPluNo, Super.MaxPluLen(), '0' );
		return m_strPluNo;
	}
	return "";
}

/**********************************************************************/

__int64 CPluCSVArray::GetPluNoInt ( int nIndex )
{
	__int64 nPluNo = 0;

	CString strTemp;
	if ( ( strTemp = GetAt ( nIndex ) ) != "" )
	{
		CCSV csv ( strTemp, ',', '"', 1 );
		nPluNo = _atoi64( csv.GetString(0) );
	}
	
	return nPluNo;
}

/**********************************************************************/

int CPluCSVArray::InsertRecord ( CPluCSVRecord& NewRecord )
{
	int nIndex;
	
	if ( FindPluByNumber ( NewRecord.GetPluNoInt(), nIndex ) == FALSE )
		InsertAt ( nIndex, NewRecord );
	
	return nIndex;
}
	
/**********************************************************************/

int CPluCSVArray::ImportLinePriceSet ( int& nIndex, CSSImportFile& import, const char* szImportLine, CString& strOldRecord )
{
	CCSV csvImport ( szImportLine );
	CString strPluNo = csvImport.GetString(0);

	if ( ::TestNumeric ( strPluNo, Super.MaxPluLen() ) == TRUE )
	{
		if ( FindPluByNumber ( _atoi64( strPluNo ), nIndex ) == TRUE )	// plu already exists
		{
			CPluCSVRecord NewRecord;
			strOldRecord = GetAt ( nIndex );
			GetAt ( nIndex, NewRecord );
			NewRecord.ImportLine ( import, csvImport );
			NewRecord.SetModifiedDateTimePlu();	// set current date and time
			SetAt ( nIndex, NewRecord );
			return IMPORT_LINE_MODIFIED;
		}
	}

	nIndex = -1;
	return IMPORT_LINE_IGNORED;
}

/**********************************************************************/

int CPluCSVArray::ImportLine ( int nImportMethod, int& nIndex, CSSImportFile& import, const char* szImportLine, bool bWithStock )
{
	CCSV csvImport ( szImportLine );
	CString strPluNo = csvImport.GetString(0);

#ifdef STOCKMAN_SYSTEM
	if ((strPluNo == "") && (TRUE == bWithStock))
	{
		CString strStockCode = csvImport.GetString(1);
		
		int nPos = 0;
		if (DataManager.StockPlu.FindStockCode(strStockCode, nPos) == TRUE)
		{
			CStockPluRecord StockPluRecord;
			DataManager.StockPlu.GetAt ( nPos, StockPluRecord );
			
			if ((StockPluRecord.GetPluNoCount() == 1) && (StockPluRecord.GetRecipePluNoCount() == 0))
			{
				strPluNo.Format("%I64d", StockPluRecord.GetPluNo(0));
			}
		}
	}
#endif

	if ( ::TestNumeric ( strPluNo, Super.MaxPluLen() ) == TRUE )
	{
		if ( FindPluByNumber ( _atoi64( strPluNo ), nIndex ) == TRUE )	// plu already exists
		{
			if ( nImportMethod == IMPORT_METHOD_ALL || nImportMethod == IMPORT_METHOD_EXIST || nImportMethod == IMPORT_METHOD_PRICESET )
			{
				CPluCSVRecord NewRecord;
				GetAt ( nIndex, NewRecord );
				NewRecord.ImportLine ( import, csvImport, bWithStock );
				NewRecord.SetModifiedDateTimePlu();	// set current date and time
				SetAt ( nIndex, NewRecord );
				return IMPORT_LINE_MODIFIED;
			}
		}
		else			// pluno not found
		{
			if ( nImportMethod == IMPORT_METHOD_ALL || nImportMethod == IMPORT_METHOD_NEW )
			{
				CPluCSVRecord NewRecord;
				NewRecord.ImportLine ( import, csvImport, bWithStock );
				NewRecord.SetModifiedDateTimePlu();	// set current date and time
				InsertAt ( nIndex, NewRecord );
				return IMPORT_LINE_ADDED;
			}
		}
	}

	nIndex = -1;

	return IMPORT_LINE_IGNORED;
}

/**********************************************************************/

int CPluCSVArray::SaveRecord ( int nPluIdx, CPluCSVRecord& PluRecord )
{
	if ( nPluIdx < 0 || nPluIdx >= GetSize() )
		return 0;

	CPluCSVRecord OldRecord;
	GetAt ( nPluIdx, OldRecord );
	int nResult = OldRecord.GetChange ( PluRecord );

	switch ( nResult )
	{
	case 1:
		PluRecord.SetModifiedDateTimePlu();
		PluRecord.SetExport ( TRUE );
		SetAt ( nPluIdx, PluRecord );
		break;

	case 2:
		PluRecord.SetExport ( TRUE );
		SetAt ( nPluIdx, PluRecord );
		break;

	default:
		break;
	}	

	return nResult;
}

/**********************************************************************/

void CPluCSVArray::ImportEODPluLine( CEODPluImportInfo& ImportInfo ) 
{
	CPluCSVRecord PluRecord;

	ImportInfo.m_nResult = IMPORT_LINE_IGNORED;

	CCSV csvPlu( ImportInfo.m_strImportLine );

	int nSize = csvPlu.GetSize();

	switch( nSize )
	{
	case 27:
	case 28:
	case 33:
	case 34:
		break;

	default:
		ImportInfo.m_nResult = IMPORT_LINE_INVALID;
		return;
	}

	bool b10Prices = ( ( 33 == nSize ) || ( 34 == nSize ) );
	bool bFreeText = ( ( 28 == nSize ) || ( 34 == nSize ) );

	__int64 nEPOSPluNo = csvPlu.GetInt64(0);

	CSQLPluNoInfo infoPluNo;
	infoPluNo.m_nEposPluNo = csvPlu.GetInt64(0);
	::ProcessPluNo( infoPluNo );

	if ( FALSE == infoPluNo.m_bValid )
	{
		ImportInfo.m_nResult = IMPORT_LINE_INVALID;
		return;
	}

	int nPluIdx;
	bool bPluExists = FindPluByNumber( infoPluNo.m_nBasePluNo, nPluIdx );

	if ( TRUE == bPluExists )
	{
		GetAt( nPluIdx, PluRecord );
	}

	if ( 0 != infoPluNo.m_nModifier )
	{
		{
			bool bCanImport = FALSE;
			switch( ImportInfo.m_nImportModifierMode )
			{
			case 1:		
				bCanImport = bPluExists;	
				break;

			case 2:		
				bCanImport = TRUE;			
				break;
			
			case 0:		
			default:
				bCanImport = FALSE;	
				break;
			}

			if ( FALSE == bCanImport )
			{
				return;
			}
		}

		if ( FALSE == bPluExists )
		{
			PluRecord.SetPluNo( infoPluNo.m_nBasePluNo );
			PluRecord.SetEposText( csvPlu.GetString(1) );
		}

		PluRecord.SetModifierEnable		( infoPluNo.m_nModifier, TRUE );
		PluRecord.SetModifierEposText	( infoPluNo.m_nModifier, TRUE, csvPlu.GetString(1) );
		PluRecord.SetModifierPrice		( infoPluNo.m_nModifier, 0, ( (double) csvPlu.GetInt(2) ) / 100.0 );
		PluRecord.SetModifierPrice		( infoPluNo.m_nModifier, 1, ( (double) csvPlu.GetInt(3) ) / 100.0 );
		PluRecord.SetModifierPrice		( infoPluNo.m_nModifier, 2, ( (double) csvPlu.GetInt(4) ) / 100.0 );
		PluRecord.SetModifierPrice		( infoPluNo.m_nModifier, 3, ( (double) csvPlu.GetInt(5) ) / 100.0 );

		int nOffset = 6;
		if ( TRUE == b10Prices )
		{
			PluRecord.SetModifierPrice	( infoPluNo.m_nModifier, 4, ( (double) csvPlu.GetInt(nOffset++) ) / 100.0 );
			PluRecord.SetModifierPrice	( infoPluNo.m_nModifier, 5, ( (double) csvPlu.GetInt(nOffset++) ) / 100.0 );
			PluRecord.SetModifierPrice	( infoPluNo.m_nModifier, 6, ( (double) csvPlu.GetInt(nOffset++) ) / 100.0 );
			PluRecord.SetModifierPrice	( infoPluNo.m_nModifier, 7, ( (double) csvPlu.GetInt(nOffset++) ) / 100.0 );
			PluRecord.SetModifierPrice	( infoPluNo.m_nModifier, 8, ( (double) csvPlu.GetInt(nOffset++) ) / 100.0 );
			PluRecord.SetModifierPrice	( infoPluNo.m_nModifier, 9, ( (double) csvPlu.GetInt(nOffset++) ) / 100.0 );
		}

		{
			int nDeptNo = csvPlu.GetInt(nOffset++);
				
			if ( FALSE == bPluExists )
			{
				PluRecord.SetBaseDeptNo( nDeptNo );
			}

			if ( nDeptNo != PluRecord.GetBaseDeptNo() )
				PluRecord.SetModifierDeptNoRaw( infoPluNo.m_nModifier, nDeptNo );
			else
				PluRecord.SetModifierDeptNoRaw( infoPluNo.m_nModifier, -1 );
		}

		{
			CString strTaxBand = csvPlu.GetString(nOffset++);

			if ( strTaxBand != PluRecord.GetBaseTaxBandString() )
				PluRecord.SetModifierTaxBandStringRaw( infoPluNo.m_nModifier, strTaxBand );
			else
				PluRecord.SetModifierTaxBandStringRaw( infoPluNo.m_nModifier, "*" );
		}

		{
			int nPluType = csvPlu.GetInt(nOffset++);

			switch( nPluType )
			{
			case 0:	//GENERAL
			case 4: //QUALIFIER
				break;

			default:
				nPluType = -1;
				break;
			}

			if (nPluType != PluRecord.GetMicrotrendPluType())
			{
				PluRecord.SetModifierMicrotrendPluType(infoPluNo.m_nModifier, nPluType);
			}
			else
			{
				PluRecord.SetModifierMicrotrendPluType(infoPluNo.m_nModifier, -1);
			}
		}

		{
			int nCat = csvPlu.GetInt(nOffset++);

			if ( ( FALSE == bPluExists ) || ( ( ImportInfo.m_nFieldMask & 1 ) == 0 ) )
			{
				
				if ( nCat != PluRecord.GetBaseAnalysisCategory() )
					PluRecord.SetModifierAnalysisCategoryRaw( infoPluNo.m_nModifier, csvPlu.GetInt(nOffset) );
				else
					PluRecord.SetModifierAnalysisCategoryRaw( infoPluNo.m_nModifier, -1 );
			}
		}
		
		{
			int nMenu = csvPlu.GetInt(nOffset++);

			if ( nMenu != PluRecord.GetFirstMenu() )
				PluRecord.SetModifierFirstMenu( infoPluNo.m_nModifier, nMenu );
			else
				PluRecord.SetModifierFirstMenu( infoPluNo.m_nModifier, -1 );
		}
		
		{
			int nQualifier = csvPlu.GetInt(nOffset++);

			if ( nQualifier != PluRecord.GetFirstQualifier() )
				PluRecord.SetModifierFirstQualifier( infoPluNo.m_nModifier, nQualifier );
			else
				PluRecord.SetModifierFirstQualifier( infoPluNo.m_nModifier, -1 );
		}

		{
			int nExtra = csvPlu.GetInt(nOffset++);

			if ( nExtra != PluRecord.GetFirstExtra() )
				PluRecord.SetModifierFirstExtra( infoPluNo.m_nModifier, nExtra );
			else
				PluRecord.SetModifierFirstExtra( infoPluNo.m_nModifier, -1 );
		}
	}
	else
	{
		if ( ( TRUE == bPluExists ) && ( FALSE == ImportInfo.m_bImportIfExists ) )
		{
			return;
		}

		if ( ( FALSE == bPluExists ) && ( FALSE == ImportInfo.m_bImportIfNew ) )
		{
			return;
		}

		PluRecord.SetPluNo( infoPluNo.m_nBasePluNo );
		PluRecord.SetEposText( csvPlu.GetString(1) );
		PluRecord.SetPrice( 0, ( (double) csvPlu.GetInt(2) ) / 100.0 );
		PluRecord.SetPrice( 1, ( (double) csvPlu.GetInt(3) ) / 100.0 );
		PluRecord.SetPrice( 2, ( (double) csvPlu.GetInt(4) ) / 100.0 );
		PluRecord.SetPrice( 3, ( (double) csvPlu.GetInt(5) ) / 100.0 );

		int nOffset = 6;
		if ( TRUE == b10Prices )
		{
			PluRecord.SetPrice( 4, ( (double) csvPlu.GetInt(nOffset++) ) / 100.0 );
			PluRecord.SetPrice( 5, ( (double) csvPlu.GetInt(nOffset++) ) / 100.0 );
			PluRecord.SetPrice( 6, ( (double) csvPlu.GetInt(nOffset++) ) / 100.0 );
			PluRecord.SetPrice( 7, ( (double) csvPlu.GetInt(nOffset++) ) / 100.0 );
			PluRecord.SetPrice( 8, ( (double) csvPlu.GetInt(nOffset++) ) / 100.0 );
			PluRecord.SetPrice( 9, ( (double) csvPlu.GetInt(nOffset++) ) / 100.0 );
		}

		PluRecord.SetBaseDeptNo( csvPlu.GetInt(nOffset++) );
		PluRecord.SetBaseTaxBandString( csvPlu.GetString(nOffset++) );
		PluRecord.SetMicrotrendPluType( csvPlu.GetInt(nOffset++) );
		
		if ( ( FALSE == bPluExists ) || ( ( ImportInfo.m_nFieldMask & 1 ) == 0 ) )
		{
			PluRecord.SetBaseAnalysisCategory( csvPlu.GetInt(nOffset) );
		}
		
		nOffset++;		
		PluRecord.SetFirstMenu( csvPlu.GetInt(nOffset++) );
		PluRecord.SetFirstQualifier( csvPlu.GetInt(nOffset++) );
		PluRecord.SetFirstExtra( csvPlu.GetInt(nOffset++) );
		PluRecord.SetOrderPrinterFlag( 0, csvPlu.GetBool(nOffset++) );
		PluRecord.SetOrderPrinterFlag( 1, csvPlu.GetBool(nOffset++) );
		PluRecord.SetOrderPrinterFlag( 2, csvPlu.GetBool(nOffset++) );
		PluRecord.SetOrderPrinterFlag( 3, csvPlu.GetBool(nOffset++) );
		PluRecord.SetOrderPrinterFlag( 4, csvPlu.GetBool(nOffset++) );
		PluRecord.SetOrderPrinterFlag( 5, csvPlu.GetBool(nOffset++) );
		PluRecord.SetOrderPrinterFlag( 6, csvPlu.GetBool(nOffset++) );
		PluRecord.SetOrderPrinterFlag( 7, csvPlu.GetBool(nOffset++) );
		PluRecord.SetOrderPrinterFlag( 8, csvPlu.GetBool(nOffset++) );
		PluRecord.SetDiscountableFlag( csvPlu.GetBool(nOffset++) );
		PluRecord.SetPriority( csvPlu.GetInt(nOffset++) );
		PluRecord.SetPriceType( csvPlu.GetInt(nOffset++) );
	
		nOffset += 2;

		if ( TRUE == bFreeText )
		{
			PluRecord.SetFreeTextFlag( csvPlu.GetBool(nOffset++) );
		}
		
		PluRecord.SetExport( TRUE );
	}	
	
	if ( FALSE == bPluExists )
	{
		PluRecord.SetModifiedDateTimePlu();
		InsertAt( nPluIdx, PluRecord );
		ImportInfo.m_nResult = IMPORT_LINE_ADDED;
		ImportInfo.m_nLastBasePluNo = PluRecord.GetPluNoInt();
	}
	else
	{
		int nType = PluRecord.GetMicrotrendPluType();
		if ( ( ImportInfo.m_nExistingMask & ( 1 << nType ) ) != 0 )
		{
			PluRecord.SetModifiedDateTimePlu();
			SetAt( nPluIdx, PluRecord );
			ImportInfo.m_nNewPluDeptNo = PluRecord.GetBaseDeptNo();
			ImportInfo.m_nResult = IMPORT_LINE_MODIFIED;
			ImportInfo.m_nLastBasePluNo = PluRecord.GetPluNoInt();
			ImportInfo.m_strNewPluText = ReportHelpers.GetReportText( PluRecord );
		}
	}
}

/**********************************************************************/

bool CPluCSVArray::CopyDefaultPlu( int nDeptNo, CPluCSVRecord& Target )
{
	int nDeptIdx;
	if ( DataManager.Department.FindDeptByNumber( nDeptNo, nDeptIdx ) == FALSE )
		return FALSE;

	CDepartmentCSVRecord Dept;
	DataManager.Department.GetAt( nDeptIdx, Dept );

	CSQLPluNoInfo infoPluNo;
	infoPluNo.m_nEposPluNo = Dept.GetSpecimenLearnPlu();
	::ProcessPluNo( infoPluNo );

	int nPluIdx;
	if ( FindPluByNumber( infoPluNo.m_nBasePluNo, nPluIdx ) == FALSE )
		return FALSE;

	__int64 nPluNo = Target.GetPluNoInt();
	GetAt( nPluIdx, Target );
	Target.SetPluNo( nPluNo );
	Target.SetBaseDeptNo( nDeptNo );
	
	return TRUE;
}

/**********************************************************************/
