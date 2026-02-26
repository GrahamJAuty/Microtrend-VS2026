/**********************************************************************/
#include "..\CommonEcrStock\DefEditDlg.h"
#include "ListDataPresetDlg.h"
#include "NodeTypes.h"
#include "SelectPresetActionDlg.h"
/**********************************************************************/
#include "PresetReportManager.h"
/**********************************************************************/

CPresetReportArrayWrapper::CPresetReportArrayWrapper( int nNodeType, CPresetReportArray* pArray ) : CDataManagerBase()
{ 
	m_nStatusArray = DATAMANAGER_CLOSED;
	m_nStatusBuffer = DATAMANAGER_CLOSED;
	m_nNodeType = nNodeType;
	m_pArray = pArray;
}	
/**********************************************************************/

CPresetReportArrayWrapper::~CPresetReportArrayWrapper()
{
}

/**********************************************************************/

bool CPresetReportArrayWrapper::Open( int nType, CDataManagerInfo& info )
{
	ResetInfo( info, m_nNodeType );
	bool bResult = OpenStringArray( (*m_pArray).m_arrayFileStrings, info, m_pArray -> GetFilename(), m_nStatusArray, nType, FALSE );
	m_pArray -> PrepareForEditing();
	return bResult;
}

/**********************************************************************/

bool CPresetReportArrayWrapper::Close( CDataManagerInfo& info )
{
	ResetInfo( info, m_nNodeType );
	return CloseStringArray( (*m_pArray).m_arrayFileStrings, info, m_nStatusArray );
}

/**********************************************************************/

bool CPresetReportArrayWrapper::Write( CDataManagerInfo& info )
{
	ResetInfo( info, m_nNodeType );
	m_pArray -> PrepareForWrite();
	bool bResult = WriteStringArray( (*m_pArray).m_arrayFileStrings, info, m_nStatusArray );
	m_pArray -> ClearStrings();
	return bResult;
}

/**********************************************************************/

bool CPresetReportArrayWrapper::OpenBuffer( CDataManagerInfo& info )
{
	ResetInfo( info, m_nNodeType );

	bool bResult = OpenStringArray( m_Buffer.m_arrayFileStrings, info, m_pArray -> GetFilename(), m_nStatusBuffer, DB_READWRITE, FALSE );

	if ( TRUE == bResult )
		m_pArray -> PrepareForEditing( m_Buffer );
	
	return bResult;
}

/**********************************************************************/

bool CPresetReportArrayWrapper::CloseBuffer( CDataManagerInfo& info )
{
	ResetInfo( info, m_nNodeType );
	m_pArray -> PrepareForEditing( m_Buffer );
	m_Buffer.ClearStrings();
	return CloseStringArray( m_Buffer.m_arrayFileStrings, info, m_nStatusBuffer );
}

/**********************************************************************/

bool CPresetReportArrayWrapper::WriteBuffer( CDataManagerInfo& info )
{
	ResetInfo( info, m_nNodeType );

	CPresetReportArray Temp;
	Temp.CopyStrings( m_Buffer );

	m_pArray -> PrepareForWrite();
	m_Buffer.CopyStrings( *m_pArray );

	bool bResult = WriteStringArray( m_Buffer.m_arrayFileStrings, info, m_nStatusBuffer );

	if ( bResult == FALSE )
		m_Buffer.CopyStrings( Temp );

	return bResult;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/

CPresetReportManager::CPresetReportManager() :
	WrapperEpos( NODE_PRESET_EPOS, &Epos ),
	WrapperEposBatch( NODE_PRESET_EPOS_BATCH, &EposBatch ),
	WrapperPluHist( NODE_PRESET_PLU_HIST, &PluHist ),
	WrapperCustHist( NODE_PRESET_CUST_HIST, &CustHist ),
	WrapperTimeAttend( NODE_PRESET_TIMEATTEND, &TimeAttend ),
	WrapperStockAudit( NODE_PRESET_STOCK_AUDIT, &StockAudit ),
	WrapperStockHist( NODE_PRESET_STOCK_HIST, &StockHist ),
	WrapperStockLevel( NODE_PRESET_STOCK_LEVEL, &StockLevel ),
	WrapperStockClosing( NODE_PRESET_STOCK_CLOSING, &StockClosing )
{
	StockLevel.SetStocktakeFlag( FALSE );
	StockClosing.SetStocktakeFlag( TRUE );
}

/**********************************************************************/
#else
/**********************************************************************/

CPresetReportManager::CPresetReportManager() :
	WrapperEpos( NODE_PRESET_EPOS, &Epos ),
	WrapperEposBatch( NODE_PRESET_EPOS_BATCH, &EposBatch ),
	WrapperPluHist( NODE_PRESET_PLU_HIST, &PluHist ),
	WrapperCustHist( NODE_PRESET_CUST_HIST, &CustHist ),
	WrapperTimeAttend( NODE_PRESET_TIMEATTEND, &TimeAttend ),
	WrapperEcrStock( NODE_PRESET_ECR_STOCK, &EcrStock )
{
}

/**********************************************************************/
#endif
/**********************************************************************/

CPresetReportArray* CPresetReportManager::GetPresetArray( int nMode )
{
	switch( nMode )
	{
	case NODE_PRESET_PLU_HIST:			return &PluHist;
	case NODE_PRESET_CUST_HIST:			return &CustHist;
	case NODE_PRESET_EPOS_BATCH:		return &EposBatch;
	case NODE_PRESET_TIMEATTEND:		return &TimeAttend;
	
#ifdef STOCKMAN_SYSTEM
	case NODE_PRESET_STOCK_LEVEL:		return &StockLevel;
	case NODE_PRESET_STOCK_CLOSING:		return &StockClosing;
	case NODE_PRESET_STOCK_AUDIT:		return &StockAudit;
	case NODE_PRESET_STOCK_HIST:		return &StockHist;
#else
	case NODE_PRESET_ECR_STOCK:			return &EcrStock;
#endif

	case NODE_PRESET_EPOS:				
	default:							return &Epos;
	}
}

/**********************************************************************/

CPresetReportArrayWrapper* CPresetReportManager::GetPresetWrapper( int nMode )
{
	switch( nMode )
	{
	case NODE_PRESET_PLU_HIST:			return &WrapperPluHist;
	case NODE_PRESET_CUST_HIST:			return &WrapperCustHist;
	case NODE_PRESET_EPOS_BATCH:		return &WrapperEposBatch;
	case NODE_PRESET_TIMEATTEND:		return &WrapperTimeAttend;
	
#ifdef STOCKMAN_SYSTEM
	case NODE_PRESET_STOCK_LEVEL:		return &WrapperStockLevel;
	case NODE_PRESET_STOCK_CLOSING:		return &WrapperStockClosing;
	case NODE_PRESET_STOCK_AUDIT:		return &WrapperStockAudit;
	case NODE_PRESET_STOCK_HIST:		return &WrapperStockHist;
#else
	case NODE_PRESET_ECR_STOCK:			return &WrapperEcrStock;
#endif

	case NODE_PRESET_EPOS:				
	default:							return &WrapperEpos;
	}
}

/**********************************************************************/

bool CPresetReportManager::Open( int nMode, int nType, CDataManagerInfo& info )
{
	return GetPresetWrapper( nMode ) -> Open( nType, info );
}

/**********************************************************************/

bool CPresetReportManager::Close( int nMode, CDataManagerInfo& info )
{
	return GetPresetWrapper( nMode ) -> Close( info );
}

/**********************************************************************/

bool CPresetReportManager::Write( int nMode, CDataManagerInfo& info )
{
	return GetPresetWrapper( nMode ) -> Write( info );
}

/**********************************************************************/

bool CPresetReportManager::OpenBuffer( int nMode, CDataManagerInfo& info )
{
	return GetPresetWrapper( nMode ) -> OpenBuffer( info );
}

/**********************************************************************/

bool CPresetReportManager::CloseBuffer( int nMode, CDataManagerInfo& info )
{
	return GetPresetWrapper( nMode ) -> CloseBuffer( info );
}

/**********************************************************************/

bool CPresetReportManager::WriteBuffer( int nMode, CDataManagerInfo& info )
{
	return GetPresetWrapper( nMode ) -> WriteBuffer( info );
}

/**********************************************************************/

void CPresetReportManager::PresetButton( int nMode, CSSComboBox& combo, CPresetReportInfo& Preset, CWnd* pParentWnd )
{
	CPresetReportArray* pPresetArray = GetPresetArray( nMode );	

	int nOldSelect = -1;
	{
		int nSel = combo.GetCurSel();
		if ( ( nSel >= 1 ) && ( nSel < combo.GetCount() ) )
			nOldSelect = combo.GetItemData( nSel );
	}

	int nAction = 0;
	int nOldEntityNo = 0;
	int nNewEntityNo = 1;
	
	if ( pPresetArray -> GetSize() != 0 )
	{
		CArray<int,int> arrayActions;
		
		arrayActions.Add(0);
		nNewEntityNo = pPresetArray -> GetNextFreeEntityNo();
			
		if ( nOldSelect != -1 ) 
		{
			CPresetReportInfo Preset;
			pPresetArray -> GetAt( nOldSelect, Preset );
			nOldEntityNo = Preset.GetEntityNo();
			arrayActions.Add(1);
		}
		
		arrayActions.Add(2);
			
		CSelectPresetActionDlg dlg( arrayActions, nOldEntityNo, nNewEntityNo, pParentWnd );
		if ( dlg.DoModal() != IDOK )
			return;

		nAction = dlg.GetAction();
	}

	CDataManagerInfo infoOpen;
	CDataManagerInfo infoWrite;
	bool bOpenError = FALSE;
	bool bWriteError = FALSE;

	if ( OpenBuffer( nMode, infoOpen ) == FALSE )
		bOpenError = TRUE;
	else
	{		
		switch ( nAction )
		{
		case 0:
			{
				CString strCreate;
				strCreate.Format( "Create Preset #%d", nNewEntityNo );

				CDefEditDlg dlg( strCreate, "", 50, "Name", pParentWnd );
				
				if ( dlg.DoModal() == IDOK )
				{
					Preset.SetName( dlg.m_strName );
					Preset.SetEntityNo( pPresetArray -> GetNextFreeEntityNo() );
					pPresetArray -> Add( Preset );
					pPresetArray -> SetPasswordAccess();

					bWriteError = ( WriteBuffer( nMode, infoWrite ) == FALSE );
					
					if ( TRUE == bWriteError )
						FillPresetCombo( nMode, combo, nOldSelect );
					else
						FillPresetCombo( nMode, combo, pPresetArray -> GetSize() - 1 );
				}
			}
			break;

		case 1:
			{	
				CPresetReportInfo OldPreset;
				pPresetArray -> GetAt( nOldSelect, OldPreset );
				Preset.SetEntityNo( OldPreset.GetEntityNo() );
				Preset.SetName( OldPreset.GetName() );

				CString strModify;
				strModify.Format( "Modify Preset #%d", nOldEntityNo );
				
				CDefEditDlg dlg( strModify, Preset.GetName(), 50, "Name", pParentWnd );
				if ( dlg.DoModal() == IDOK )
				{
					Preset.SetName( dlg.m_strName );
					pPresetArray -> SetAt( nOldSelect, Preset );
					pPresetArray -> SetPasswordAccess();
					bWriteError = ( WriteBuffer( nMode, infoWrite ) == FALSE );
					FillPresetCombo( nMode, combo, nOldSelect );
				}
			}
			break;

		case 2:
			{
				CListDataPresetDlg dlg ( nMode, nOldSelect, *pPresetArray, pParentWnd );
		
				if ( dlg.DoModal() == IDOK )
				{
					pPresetArray -> SetPasswordAccess();
					bWriteError = ( WriteBuffer( nMode, infoWrite ) == FALSE );
					
					if ( TRUE == bWriteError )
						FillPresetCombo( nMode, combo, nOldSelect );
					else
						FillPresetCombo( nMode, combo, dlg.GetListSelection() );
				}
			}
			break;
		}

		CloseBuffer( nMode, infoOpen );
	}	

	if ( TRUE == bOpenError )
		Prompter.ShareError( infoOpen );

	if ( TRUE == bWriteError )
		Prompter.WriteError( infoWrite );
}

/**********************************************************************/

void CPresetReportManager::FillPresetCombo( int nMode, CSSComboBox& combo, int nPresetIdxSel )
{
	CPresetReportArray* pPresetArray = GetPresetArray( nMode );	

	combo.ResetContent();

	int nSel = 0;
	combo.AddString( "None" );
	for ( int nPresetIdx = 0; nPresetIdx < pPresetArray -> GetSize(); nPresetIdx++ )
	{
		CPresetReportInfo Preset;
		pPresetArray -> GetAt( nPresetIdx, Preset );

		if ( Preset.GetPasswordEnableFlag() == TRUE )
		{
			int nPos = combo.AddString( Preset.GetListName() );
			combo.SetItemData( nPos, nPresetIdx );

			if ( nPresetIdx == nPresetIdxSel )
				nSel = nPos;
		}
	}

	combo.SetCurSel(nSel);
}

/**********************************************************************/

int CPresetReportManager::SelectFromCombo( int nMode, CSSComboBox& combo, int nPresetNo )
{
	CPresetReportArray* pPresetArray = GetPresetArray( nMode );	

	int nError = 1;
	for ( int nPresetIdx = 0; nPresetIdx < pPresetArray -> GetSize(); nPresetIdx++ )
	{
		CPresetReportInfo Preset;
		pPresetArray -> GetAt( nPresetIdx, Preset );

		if ( Preset.GetEntityNo() == nPresetNo )
		{
			nError = 2;
			for ( int nComboSel = 1; nComboSel < combo.GetCount(); nComboSel++ )
			{
				if ( combo.GetItemData(nComboSel) == nPresetIdx )
				{
					nError = 0;
					combo.SetCurSel(nComboSel);
					break;
				}
			}
			break;
		}
	}

	return nError;
}

/**********************************************************************/
