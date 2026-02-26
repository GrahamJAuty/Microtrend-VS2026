/**********************************************************************/
#include "CustomPluField.h"
#include "LocationCSVArray.h"
#include "PluCSVArray.h"
#include "PluDatabaseHeader.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PriceSetCustomListReportHelpers.h"
/**********************************************************************/

CPriceSetCustomListLocInfo::CPriceSetCustomListLocInfo()
{
	m_nLocIdx = 0;
	m_nPluNo = 0;
	m_bGotPlu = FALSE;
	m_pPluCSVLine = NULL;
	m_pPluItems = NULL;

	for ( int m = 0; m < 10; m++ )
	{
		m_nModifierEnableColumn[m] = -1;
		m_nModifierEposTextFlagColumn[m] = -1;
		m_nModifierKeyTextFlagColumn[m] = -1;
		m_nModifierOrderGroupColumn[m] = -1;
		m_nModifierEposTextColumn[m] = -1;
		m_nModifierKeyTextColumn[m] = -1;
		m_nModifierMenuColumn[m] = -1;
		m_nModifierQualifierColumn[m] = -1;
		m_nModifierExtraColumn[m] = -1;
		m_nModifierTaxBandColumn[m] = -1;
		
		for (int p = 0; p < 10; p++)
		{
			m_nModifierPriceColumn[m][p] = -1;
		}
	}

	m_nReportCostColumn = -1;
	m_nPriorityColumn = -1;
}

/**********************************************************************/
/**********************************************************************/

bool CPriceSetCustomListLocInfo::GetDouble( int nCol, double& dValue )
{
	bool bResult = FALSE;

	if ( nCol != -1 )
	{
		dValue = m_pPluCSVLine -> GetDouble( nCol );
		bResult = TRUE;
	}
	
	return bResult;
}

/**********************************************************************/

bool CPriceSetCustomListLocInfo::GetInt( int nCol, int& nValue )
{
	bool bResult = FALSE;

	if ( nCol != -1 )
	{
		nValue = m_pPluCSVLine -> GetInt( nCol );
		bResult = TRUE;
	}
	
	return bResult;
}

/**********************************************************************/

bool CPriceSetCustomListLocInfo::GetBool( int nCol, bool& bValue )
{
	bool bResult = FALSE;

	if ( nCol != -1 )
	{
		bValue = m_pPluCSVLine -> GetBool( nCol );
		bResult = TRUE;
	}
	
	return bResult;
}

/**********************************************************************/

bool CPriceSetCustomListLocInfo::GetString( int nCol, CString& strValue )
{
	bool bResult = FALSE;

	if ( nCol != -1 )
	{
		strValue = m_pPluCSVLine -> GetString( nCol );
		bResult = TRUE;
	}
	
	return bResult;
}

/**********************************************************************/
/**********************************************************************/

bool CPriceSetCustomListLocInfo::GetModifierPrice( int nMod, int nPrice, double& dValue )
{
	return GetDouble( m_nModifierPriceColumn[nMod][nPrice], dValue );
}

/**********************************************************************/

bool CPriceSetCustomListLocInfo::GetModifierEnableFlag( int nMod, bool& bValue )
{
	return GetBool( m_nModifierEnableColumn[nMod], bValue );
}

/**********************************************************************/

bool CPriceSetCustomListLocInfo::GetModifierEposTextFlag( int nMod, bool& bValue )
{
	return GetBool( m_nModifierEposTextFlagColumn[nMod], bValue );
}

/**********************************************************************/

bool CPriceSetCustomListLocInfo::GetModifierEposText( int nMod, CString& strValue )
{
	return GetString( m_nModifierEposTextColumn[nMod], strValue );
}

/**********************************************************************/

bool CPriceSetCustomListLocInfo::GetModifierKeyTextFlag( int nMod, bool& bValue )
{
	return GetBool( m_nModifierKeyTextFlagColumn[nMod], bValue );
}

/**********************************************************************/

bool CPriceSetCustomListLocInfo::GetModifierKeyText( int nMod, CString& strValue )
{
	return GetString( m_nModifierKeyTextColumn[nMod], strValue );
}

/**********************************************************************/

bool CPriceSetCustomListLocInfo::GetModifierOrderGroup( int nMod, int& nValue )
{
	return GetInt( m_nModifierOrderGroupColumn[nMod], nValue );
}

/**********************************************************************/

bool CPriceSetCustomListLocInfo::GetModifierMenu( int nMod, int& nValue )
{
	return GetInt( m_nModifierMenuColumn[nMod], nValue );
}

/**********************************************************************/

bool CPriceSetCustomListLocInfo::GetModifierQualifier( int nMod, int& nValue )
{
	return GetInt( m_nModifierQualifierColumn[nMod], nValue );
}

/**********************************************************************/

bool CPriceSetCustomListLocInfo::GetModifierExtra( int nMod, int& nValue )
{
	return GetInt( m_nModifierExtraColumn[nMod], nValue );
}

/**********************************************************************/

bool CPriceSetCustomListLocInfo::GetModifierTaxBand(int nMod, CString& strValue)
{
	return GetString(m_nModifierTaxBandColumn[nMod], strValue);
}

/**********************************************************************/

bool CPriceSetCustomListLocInfo::GetReportCost( double& dValue )
{
	return GetDouble( m_nReportCostColumn, dValue );
}

/**********************************************************************/

bool CPriceSetCustomListLocInfo::GetPriority( int& nValue )
{
	return GetInt( m_nPriorityColumn, nValue );
}

/**********************************************************************/
/**********************************************************************/

bool CPriceSetCustomListLocInfo::FindPluNo( __int64 nPluNo, int& nIndex )
{
	int nStart = 0;
	int nEnd = m_pPluItems -> GetSize() - 1;

	while ( nStart <= nEnd )
	{
		nIndex = ( nStart + nEnd ) / 2;

		CString str = m_pPluItems -> GetAt( nIndex );
		CCSV csv( str, ',', '\"', 1 );

		__int64 nPluNoArray = csv.GetInt64(0);
		
		if ( nPluNoArray < nPluNo )
			nStart = nIndex + 1;

		else if ( nPluNoArray > nPluNo )
			nEnd = nIndex - 1;

		else
			return TRUE;
	}

	nIndex = nStart;
	return FALSE;
}

/**********************************************************************/
/**********************************************************************/

void CPriceSetCustomListLocInfo::UpdateLocPluCache( __int64 nPluNo )
{
	if ( m_nPluNo != nPluNo )
	{
		m_nPluNo = nPluNo;

		int nItemIndex;
		if ( FindPluNo( nPluNo, nItemIndex ) == TRUE )
		{
			m_bGotPlu = TRUE;
			m_pPluCSVLine -> ParseLine( m_pPluItems -> GetAt(nItemIndex) ); 
		}
		else
		{
			m_bGotPlu = FALSE;
		}
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CPriceSetCustomListReportHelpers::CPriceSetCustomListReportHelpers( int nPriceSet )
{
	m_nPriceSet = nPriceSet;
	m_strFieldKey = "";

	CReportFile ReportFile;
	m_strPriceSetColour = ReportFile.GetColourText( "", 0xFF0000, 0xFFFFFF );
}

/**********************************************************************/

CPriceSetCustomListReportHelpers::~CPriceSetCustomListReportHelpers()
{
	for ( int n = 0; n < m_arrayLocInfo.GetSize(); n++ )
	{
		CCSV* pCSV = m_arrayLocInfo[n].m_pPluCSVLine;

		if ( pCSV != NULL )
		{
			pCSV -> RemoveAll();
			delete ( pCSV );
		}

		CStringArray* pArray = m_arrayLocInfo[n].m_pPluItems;

		if ( pArray != NULL )
		{
			pArray -> RemoveAll();
			delete ( pArray );
		}
	}
}

/**********************************************************************/

void CPriceSetCustomListReportHelpers::SetFieldList( const char* szFields )
{
	m_strFieldKey = szFields;

	m_arrayFields.RemoveAll();

	CCSV csvFields( m_strFieldKey );
			
	for ( int n = 0; n < csvFields.GetSize(); n += 2 )
	{
		CPriceSetCustomListFieldInfo info;
		info.m_nFieldType = csvFields.GetInt(n);
		info.m_bPriceSet = ( csvFields.GetInt(n+1) == 2 );
		m_arrayFields.Add(info);
	}
}

/**********************************************************************/

void CPriceSetCustomListReportHelpers::GetFieldInfo( int n, CPriceSetCustomListFieldInfo& info )
{
	if ( ( n >= 0 ) && ( n < m_arrayFields.GetSize() ) )
		info = m_arrayFields.GetAt( n );
}

/**********************************************************************/

int CPriceSetCustomListReportHelpers::GetLocIdx( int n )
{
	if ( ( n >= 0 ) && ( n < m_arrayLocInfo.GetSize() ) )
		return m_arrayLocInfo[n].m_nLocIdx;
	else
		return 0;
}

/**********************************************************************/

int CPriceSetCustomListReportHelpers::FindColumn( CString& strLabel, CReportConsolidationArray<CSortedIntByString>& arrayColumns )
{
	int nResult = -1;

	CSortedIntByString item;
	item.m_strItem = strLabel;
	item.m_strItem.MakeUpper();
	
	int nPos;
	if ( arrayColumns.Find( item, nPos ) == TRUE )
	{
		arrayColumns.GetAt( nPos, item );
		nResult = item.m_nItem;
	}

	return nResult;
}

/**********************************************************************/

void CPriceSetCustomListReportHelpers::AddLocation( int nLocIdx )
{
	CPriceSetCustomListLocInfo infoLoc;
	infoLoc.m_nLocIdx = nLocIdx;
	infoLoc.m_pPluItems = new CStringArray;
	infoLoc.m_pPluCSVLine = new CCSV( "" );; 

	CString strHeader = "";
	{
		CString strFieldFilename = dbLocation.GetFilePathDataLocalPluField( nLocIdx, m_nPriceSet );
		
		CSSFile filePriceSet;
		if ( filePriceSet.Open( strFieldFilename, "rb" ) == TRUE )
		{
			CString strBuffer;
			if ( filePriceSet.ReadString( strBuffer ) == TRUE )
			{
				strHeader = strBuffer; 
				strHeader = PluDatabaseHeader.GetUpdatedHeader( strHeader );

				while ( filePriceSet.ReadString( strBuffer ) == TRUE )
					infoLoc.m_pPluItems -> Add( strBuffer );
			}
		}
	}
			
	{
		CCSV csvHeader( strHeader );
		CReportConsolidationArray<CSortedIntByString> arrayColumns;
		for ( int c = 0; c < csvHeader.GetSize(); c++ )
		{
			CString strHeaderField = csvHeader.GetString(c);
			strHeaderField.MakeUpper();

			CSortedIntByString item;
			item.m_strItem = strHeaderField;
			item.m_nItem = c;
			arrayColumns.Consolidate( item );
		}

		CString strLabel = "";
				
		for ( int m = 0; m <= 9; m++ )
		{
			if ( m != 0 )
			{
				CPluCSVRecord::GetModifierLabelEnable( m, FALSE, strLabel );
				infoLoc.m_nModifierEnableColumn[m] = FindColumn( strLabel, arrayColumns );

				CPluCSVRecord::GetModifierLabelEposTextFlag( m, FALSE, strLabel );
				infoLoc.m_nModifierEposTextFlagColumn[m] = FindColumn( strLabel, arrayColumns );

				CPluCSVRecord::GetModifierLabelKeyTextFlag( m, FALSE, strLabel );
				infoLoc.m_nModifierKeyTextFlagColumn[m] = FindColumn( strLabel, arrayColumns );

				CPluCSVRecord::GetModifierLabelEposText( m, FALSE, strLabel );
				infoLoc.m_nModifierEposTextColumn[m] = FindColumn( strLabel, arrayColumns );

				CPluCSVRecord::GetModifierLabelKeyText( m, FALSE, strLabel );
				infoLoc.m_nModifierKeyTextColumn[m] = FindColumn( strLabel, arrayColumns );

				CPluCSVRecord::GetModifierLabelOrderPrinterGroup( m, strLabel );
				infoLoc.m_nModifierOrderGroupColumn[m] = FindColumn( strLabel, arrayColumns );

				CPluCSVRecord::GetModifierLabelFirstMenu( m, strLabel );
				infoLoc.m_nModifierMenuColumn[m] = FindColumn( strLabel, arrayColumns );

				CPluCSVRecord::GetModifierLabelFirstQualifier( m, strLabel );
				infoLoc.m_nModifierQualifierColumn[m] = FindColumn( strLabel, arrayColumns );

				CPluCSVRecord::GetModifierLabelFirstExtra( m, strLabel );
				infoLoc.m_nModifierExtraColumn[m] = FindColumn( strLabel, arrayColumns );

				CPluCSVRecord::GetModifierLabelTaxBand(m, FALSE, strLabel);
				infoLoc.m_nModifierTaxBandColumn[m] = FindColumn(strLabel, arrayColumns);

				for ( int p = 1; p <= 10; p++ )
				{
					CPluCSVRecord::GetModifierLabelPrice( m, p, FALSE, strLabel );
					infoLoc.m_nModifierPriceColumn[m][p-1] = FindColumn( strLabel, arrayColumns );
				}
			}
			else
			{
				strLabel = Plu::EposText.Label;
				infoLoc.m_nModifierEposTextColumn[0] = FindColumn( strLabel, arrayColumns );

				strLabel = Plu::KeyText.Label;
				infoLoc.m_nModifierKeyTextColumn[0] = FindColumn( strLabel, arrayColumns );

				strLabel = Plu::OrderPrinterGroup.Label;
				infoLoc.m_nModifierOrderGroupColumn[0] = FindColumn( strLabel, arrayColumns );

				strLabel = Plu::FirstMenu.Label;
				infoLoc.m_nModifierMenuColumn[0] = FindColumn( strLabel, arrayColumns );

				strLabel = Plu::FirstQualifier.Label;
				infoLoc.m_nModifierQualifierColumn[0] = FindColumn( strLabel, arrayColumns );

				strLabel = Plu::FirstExtra.Label;
				infoLoc.m_nModifierExtraColumn[0] = FindColumn( strLabel, arrayColumns );

				strLabel = Plu::TaxBand.Label;
				infoLoc.m_nModifierTaxBandColumn[0] = FindColumn(strLabel, arrayColumns);

				for ( int p = 1; p <= 10; p++ )
				{
					CPluCSVRecord::GetPriceLabel( p, strLabel );
					infoLoc.m_nModifierPriceColumn[0][p-1] = FindColumn( strLabel, arrayColumns );
				}
			}
		}

		strLabel = Plu::ReportCost.Label;
		infoLoc.m_nReportCostColumn = FindColumn( strLabel, arrayColumns );

		strLabel = Plu::Priority.Label;
		infoLoc.m_nPriorityColumn = FindColumn( strLabel, arrayColumns );

		m_arrayLocInfo.Add( infoLoc );
	}
}

/**********************************************************************/

void CPriceSetCustomListReportHelpers::AppendPriceSetInt( CCSVReportLine& csv, CPluCSVRecord& PluRecord, int nType, int nMod, bool bPriceSet )
{
	int nMasterMod = 0;
	int nMasterBase = 0;
	
	if ( CustomPluList::Menu.Type == nType )		
	{
		nMasterMod = PluRecord.GetModifierFirstMenu(nMod);
		nMasterBase = PluRecord.GetFirstMenu();
	}
	else if ( CustomPluList::Qualifier.Type == nType )	
	{
		nMasterMod = PluRecord.GetModifierFirstQualifier(nMod);
		nMasterBase = PluRecord.GetFirstQualifier();
	}
	else if ( CustomPluList::Extra.Type == nType )		
	{
		nMasterMod = PluRecord.GetModifierFirstExtra(nMod);
		nMasterBase = PluRecord.GetFirstExtra();
	}
	else if ( CustomPluList::Priority.Type == nType )	
	{
		nMasterMod = PluRecord.GetPriority();
		nMasterBase = nMasterMod;
	}

	if ( ( nMasterMod != -1 ) || ( nMod == 0 ) )
	{
		csv.AppendInt( nMasterMod );
	}
	else
	{
		CString str;
		str.Format( "[%d]", nMasterBase );
		csv.AppendString( str );
	}

	if ( TRUE == bPriceSet )
	{
		for ( m_nLocInfoIdx = 0; m_nLocInfoIdx < m_arrayLocInfo.GetSize(); m_nLocInfoIdx++ )
		{
			UpdateLocPluCache( PluRecord.GetPluNoInt() );

			int nPriceSetMod = nMasterMod;
			int nPriceSetBase = nMasterBase;

			bool bOverride = FALSE;	
			if ( GotPlu() == TRUE )
			{
				if ( CustomPluList::Menu.Type == nType )			
				{
					bOverride = GetModifierMenu( nMod, nPriceSetMod );
					GetModifierMenu( 0, nPriceSetBase );
				}
				else if ( CustomPluList::Qualifier.Type == nType )	
				{
					bOverride = GetModifierQualifier( nMod, nPriceSetMod );
					GetModifierQualifier( 0, nPriceSetBase );
				}
				else if ( CustomPluList::Extra.Type == nType )		
				{
					bOverride = GetModifierExtra( nMod, nPriceSetMod );
					GetModifierExtra( 0, nPriceSetBase );
				}
				else if ( CustomPluList::Priority.Type == nType )	
				{
					bOverride = GetPriority( nPriceSetMod );
					nPriceSetBase = nPriceSetMod;
				}
			}

			CString strValue;
			if ( ( nPriceSetMod != -1 ) || ( nMod == 0 ) )
			{
				strValue.Format( "%d", nPriceSetMod );
			}
			else
			{
				strValue.Format( "[%d]", nPriceSetBase );
			}

			if ( FALSE == bOverride )
			{
				csv.AppendString( strValue );
			}
			else
			{
				csv.AppendString( m_strPriceSetColour + strValue );
			}
		}
	}
}

/**********************************************************************/

void CPriceSetCustomListReportHelpers::AppendPriceSetDouble( CCSVReportLine& csv, CPluCSVRecord& PluRecord, int nType, bool bPriceSet )
{
	double dMaster = 0.0;
	bool bAvailable = TRUE;
	if ( CustomPluList::ReportCost.Type == nType )			
	{
		dMaster = PluRecord.GetReportCost();
		bAvailable = PluRecord.GetReportCostEnable();
	}
	
	if ( TRUE == bAvailable )
		csv.AppendVal( dMaster );
	else
		csv.AppendString( "--" );

	if ( TRUE == bPriceSet )
	{
		for ( m_nLocInfoIdx = 0; m_nLocInfoIdx < m_arrayLocInfo.GetSize(); m_nLocInfoIdx++ )
		{
			if ( FALSE == bAvailable )
				csv.Add( "--" );
			else
			{
				UpdateLocPluCache( PluRecord.GetPluNoInt() );

				double dValue = dMaster;

				bool bOverride = FALSE;	
				if ( GotPlu() == TRUE )
				{
					if ( CustomPluList::ReportCost.Type == nType )		bOverride = GetReportCost( dValue ); 
				}

				if ( FALSE == bOverride )
					csv.AppendVal( dValue );
				else
				{
					CString strValue;
					strValue.Format( "%.*f", SysInfo.GetDPValue(), dValue );
					csv.AppendString( m_strPriceSetColour + strValue );
				}
			}
		}
	}
}

/**********************************************************************/

void CPriceSetCustomListReportHelpers::AppendPriceSetPrices( CCSVReportLine& csv, CPluCSVRecord& PluRecord, int nMod, int nPrice, bool bPriceSet )
{
	double dMaster = PluRecord.GetModifierPrice( nMod, nPrice );
	csv.AppendVal( dMaster );

	if ( TRUE == bPriceSet )
	{
		for ( m_nLocInfoIdx = 0; m_nLocInfoIdx < m_arrayLocInfo.GetSize(); m_nLocInfoIdx++ )
		{
			UpdateLocPluCache( PluRecord.GetPluNoInt() );

			bool bEnableMod = TRUE;
			bool bOverride = FALSE;
			double dValue = dMaster;
			
			if ( GotPlu() == TRUE )
			{
				if ( nMod != 0 )
					if ( GetModifierEnableFlag(nMod,bEnableMod) == TRUE )
						if ( FALSE == bEnableMod )
							bOverride = TRUE;
		
				if ( TRUE == bEnableMod )
					if ( GetModifierPrice( nMod, nPrice, dValue ) == TRUE )
						bOverride = TRUE;
			}

			if ( FALSE == bOverride )
				csv.AppendVal( dValue );
			else if ( FALSE == bEnableMod )
				csv.AppendString( m_strPriceSetColour + "---" );
			else
			{
				CString strValue;
				strValue.Format( "%.*f", SysInfo.GetDPValue(), dValue );
				csv.AppendString( m_strPriceSetColour + strValue );
			}
		}
	}
}

/**********************************************************************/

void CPriceSetCustomListReportHelpers::AppendPriceSetEposTexts( CCSVReportLine& csv, CPluCSVRecord& PluRecord, int nMod, bool bPriceSet )
{
	CString strMaster = PluRecord.GetModifierEposText( nMod ); 
	csv.AppendString( strMaster );

	if ( TRUE == bPriceSet )
	{
		for ( m_nLocInfoIdx = 0; m_nLocInfoIdx < m_arrayLocInfo.GetSize(); m_nLocInfoIdx++ )
		{
			bool bEnableMod = TRUE;
			bool bOverride = FALSE;
			__int64 nPluNo = PluRecord.GetPluNoInt();

			UpdateLocPluCache( nPluNo );

			CPluCSVRecord PluRecordLoc;
			PluRecordLoc.SetPluNo( nPluNo );
			PluRecordLoc.SetEposText( PluRecord.GetEposText() );

			if ( nMod != 0 )
			{
				if ( GotPlu() == TRUE )
				{
					if ( GetModifierEnableFlag( nMod, bEnableMod ) == TRUE )
						if ( FALSE == bEnableMod )
							bOverride = TRUE;

					if ( TRUE == bEnableMod )
					{
						bool bFlag =		PluRecord.GetModifierEposTextFlag( nMod );
						CString strText =	PluRecord.GetModifierEposText( nMod );

						if ( GetModifierEposTextFlag( nMod, bFlag ) == TRUE )
							bOverride = TRUE;
							
						if ( TRUE == bFlag )
							if ( GetModifierEposText( nMod, strText ) == TRUE )
								bOverride = TRUE;
						
						PluRecordLoc.SetModifierEnable( nMod, TRUE );
						PluRecordLoc.SetModifierEposText( nMod, bFlag, strText );
					}
				}
			}

			if ( TRUE == bEnableMod )
			{
				CString strText = "";
				if ( GotPlu() == TRUE )
				{
					if ( GetModifierEposText( 0, strText ) == TRUE )
					{
						PluRecordLoc.SetEposText( strText );
						bOverride = TRUE;
					}
				}
			}

			if ( FALSE == bOverride )
				csv.AppendString( strMaster );
			else if ( FALSE == bEnableMod )
				csv.AppendString( m_strPriceSetColour + "---" );
			else
				csv.AppendString( m_strPriceSetColour + PluRecordLoc.GetModifierEposText( nMod ) );
		}
	}
}

/**********************************************************************/

void CPriceSetCustomListReportHelpers::AppendPriceSetKeyTexts( CCSVReportLine& csv, CPluCSVRecord& PluRecord, int nMod, bool bPriceSet )
{
	CString strMaster = PluRecord.GetModifierKeyText( nMod ); 
	csv.AppendString( strMaster );

	if ( TRUE == bPriceSet )
	{
		for ( m_nLocInfoIdx = 0; m_nLocInfoIdx < m_arrayLocInfo.GetSize(); m_nLocInfoIdx++ )
		{
			bool bEnableMod = TRUE;
			bool bOverride = FALSE;
			__int64 nPluNo = PluRecord.GetPluNoInt();

			UpdateLocPluCache( nPluNo );

			CPluCSVRecord PluRecordLoc;
			PluRecordLoc.SetPluNo( nPluNo );
			PluRecordLoc.SetKeyText( PluRecord.GetKeyText() );

			if ( nMod != 0 )
			{
				if ( GotPlu() == TRUE )
				{
					if ( GetModifierEnableFlag( nMod, bEnableMod ) == TRUE )
						if ( FALSE == bEnableMod )
							bOverride = TRUE;

					if ( TRUE == bEnableMod )
					{
						bool bFlag =		PluRecord.GetModifierKeyTextFlag( nMod );
						CString strText =	PluRecord.GetModifierKeyText( nMod );

						if ( GetModifierKeyTextFlag( nMod, bFlag ) == TRUE )
							bOverride = TRUE;
							
						if ( TRUE == bFlag )
							if ( GetModifierKeyText( nMod, strText ) == TRUE )
								bOverride = TRUE;
						
						PluRecordLoc.SetModifierEnable( nMod, TRUE );
						PluRecordLoc.SetModifierKeyText( nMod, bFlag, strText );
					}
				}
			}

			if ( TRUE == bEnableMod )
			{
				CString strText = "";
				if ( GotPlu() == TRUE )
				{
					if ( GetModifierKeyText( 0, strText ) == TRUE )
					{
						PluRecordLoc.SetKeyText( strText );
						bOverride = TRUE;
					}
				}
			}

			if ( FALSE == bOverride )
				csv.AppendString( strMaster );
			else if ( FALSE == bEnableMod )
				csv.AppendString( m_strPriceSetColour + "---" );
			else
				csv.AppendString( m_strPriceSetColour + PluRecordLoc.GetModifierKeyText( nMod ) );
		}
	}
}

/**********************************************************************/

void CPriceSetCustomListReportHelpers::AppendPriceSetOrderGroups( CCSVReportLine& csv, CPluCSVRecord& PluRecord, int nMod, bool bPriceSet )
{
	CString strMaster = "None";
				
	int nGroup = PluRecord.GetModifierOrderPrinterGroup(nMod);
	if ( ( nGroup >= 1 ) && ( nGroup <= DataManagerNonDb.OPGroupSystem.GetOPGroupCount() ) )
	{
		COPGroupMiniInfo infoGroup;
		DataManagerNonDb.OPGroupSystem.GetOPGroup( nGroup - 1, infoGroup );
		strMaster = infoGroup.m_strName;
	}
	else if ( ( 0 == nGroup ) && ( nMod != 0 ) )
	{
		strMaster = "As Base Plu";
	}

	csv.AppendString( strMaster );

	if ( TRUE == bPriceSet )
	{
		for ( m_nLocInfoIdx = 0; m_nLocInfoIdx < m_arrayLocInfo.GetSize(); m_nLocInfoIdx++ )
		{
			bool bOverride = FALSE;
			CString strValue = strMaster;

			UpdateLocPluCache( PluRecord.GetPluNoInt() );
				
			if ( GotPlu() == TRUE )
			{
				int nValue = 0;
				if ( GetModifierOrderGroup( nMod, nValue ) == TRUE )
				{
					bOverride = TRUE;
					strValue = "None";

					if ( ( nValue >= 1 ) && ( nValue <= DataManagerNonDb.OPGroupSystem.GetOPGroupCount() ) )
					{
						COPGroupMiniInfo infoGroup;
						DataManagerNonDb.OPGroupSystem.GetOPGroup( nValue - 1, infoGroup );
						strValue = infoGroup.m_strName;
					}
					else if ( ( 0 == nGroup ) && ( nMod != 0 ) )
					{
						strValue = "As Base Plu";
					}
				}
			}
			
			if ( FALSE == bOverride )
				csv.AppendString( strMaster );
			else
				csv.AppendString( m_strPriceSetColour + strValue );
		}
	}
}

/**********************************************************************/

void CPriceSetCustomListReportHelpers::AppendPriceSetTaxBands(CCSVReportLine& csv, CPluCSVRecord& PluRecord, int nMod, bool bPriceSet)
{
	CString strMaster = PluRecord.GetModifierTaxBandStringRaw(nMod);

	if (strMaster == "*")
	{
		strMaster.Format("[%s]", PluRecord.GetBaseTaxBandString());
	}

	csv.AppendString(strMaster);

	if (TRUE == bPriceSet)
	{
		for (m_nLocInfoIdx = 0; m_nLocInfoIdx < m_arrayLocInfo.GetSize(); m_nLocInfoIdx++)
		{
			bool bEnableMod = TRUE;
			bool bOverride = FALSE;
			__int64 nPluNo = PluRecord.GetPluNoInt();

			UpdateLocPluCache(nPluNo);

			CPluCSVRecord PluRecordLoc;
			PluRecordLoc.SetPluNo(nPluNo);
			PluRecordLoc.SetBaseTaxBandString(PluRecord.GetBaseTaxBandString());

			if (nMod != 0)
			{
				if (GotPlu() == TRUE)
				{
					if (GetModifierEnableFlag(nMod, bEnableMod) == TRUE)
					{
						if (FALSE == bEnableMod)
						{
							bOverride = TRUE;
						}
					}

					if (TRUE == bEnableMod)
					{
						CString strTaxBand = PluRecord.GetModifierTaxBandStringRaw(nMod);

						if (GetModifierTaxBand(nMod, strTaxBand) == TRUE)
						{
							bOverride = TRUE;
						}

						PluRecordLoc.SetModifierEnable(nMod, TRUE);
						PluRecordLoc.SetModifierTaxBandStringRaw(nMod, strTaxBand);
					}
				}
			}

			if (TRUE == bEnableMod)
			{
				CString strTaxBand = "";
				if (GotPlu() == TRUE)
				{
					if (GetModifierTaxBand(0, strTaxBand) == TRUE)
					{
						PluRecordLoc.SetBaseTaxBandString(strTaxBand);
						bOverride = TRUE;
					}
				}
			}

			if (FALSE == bOverride)
			{
				csv.AppendString(strMaster);
			}
			else if (FALSE == bEnableMod)
			{
				csv.AppendString(m_strPriceSetColour + "---");
			}
			else
			{
				CString strTaxBand = PluRecordLoc.GetModifierTaxBandStringRaw(nMod);

				if (strTaxBand == "*")
				{
					strTaxBand.Format("[%s]", PluRecordLoc.GetBaseTaxBandString());
				}

				csv.AppendString(m_strPriceSetColour + strTaxBand );
			}
		}
	}
}

/**********************************************************************/