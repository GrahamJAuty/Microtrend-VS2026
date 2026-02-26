/**********************************************************************/
#include "..\CommonEcrStock\DefEditDlg.h"
#include "NodeTypes.h"
/**********************************************************************/
#include "ListDataPresetDlg.h"
/**********************************************************************/

CSingleListContentHelpersPreset::CSingleListContentHelpersPreset( int nNodeType, CPresetReportArray& arrayPresets ) : m_arrayPresets( arrayPresets )
{
	m_nNodeType = nNodeType;
}

/**********************************************************************/

int CSingleListContentHelpersPreset::GetArraySize()
{
	return m_arrayPresets.GetSize();
}

/**********************************************************************/

bool CSingleListContentHelpersPreset::PrepareLine( int nIndex, CString& strLine )
{
	strLine = "";

	bool bResult = FALSE;
	if ( ( nIndex >= 0 ) && ( nIndex < GetArraySize() ) )
	{
		int nEntityNo;
		CString strName;
		m_arrayPresets.GetEntityNoAndListName( nIndex, nEntityNo, strName );

		strLine.Format( "%3.3d\t%s",
			nEntityNo,
			(const char*) strName );

		bResult = TRUE;
	}
	
	return bResult;
}

/**********************************************************************/

void CSingleListContentHelpersPreset::DeleteDatabaseEntry( int nIndex )
{
	if ( ( nIndex >= 0 ) && ( nIndex < m_arrayPresets.GetSize() ) )
	{
		m_arrayPresets.RemoveAt ( nIndex );
		m_bDeletionEffect = TRUE;
	}
}

/**********************************************************************/

void CSingleListContentHelpersPreset::SetColumns( CSSListCtrlVirtual& list, CWnd* pParent )
{
	list.SubclassDlgItem ( IDC_LIST, pParent );
	list.InsertColumn ( 0, "Num", LVCFMT_LEFT, 40 );
	list.InsertColumn ( 1, "Preset Name", LVCFMT_LEFT, 440 );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListDataPresetDlg::CListDataPresetDlg( int nNodeType, int nInitialSel, CPresetReportArray& arrayPresets, CWnd* pParent) : CListDataDlg ( pParent ), m_arrayPresets( arrayPresets )
{
	m_nNodeType = nNodeType;
	m_nInitialSel = nInitialSel;
	
	m_pContentHelpers = new CSingleListContentHelpersPreset( nNodeType, arrayPresets );
	m_pDisplayHelpers = new CSingleListDisplayHelpers( m_listData, *m_pContentHelpers );

	m_pDisplayHelpers -> SetTopSingleText( "preset report" );
	m_pDisplayHelpers -> SetTopPluralText( "preset reports" );
	m_pDisplayHelpers -> SetBottomSingleText( "preset report" );
	m_pDisplayHelpers -> SetBottomPluralText( "preset reports" );
}

/**********************************************************************/

CListDataPresetDlg::~CListDataPresetDlg()
{
	delete m_pDisplayHelpers;
	delete m_pContentHelpers;
}

/**********************************************************************/

void CListDataPresetDlg::SetTitle()
{
	switch( m_nNodeType )
	{
	case NODE_PRESET_PLU_HIST:
		SetWindowText ( "Setup Plu Sales History Presets" );		
		break;

	case NODE_PRESET_CUST_HIST:
		SetWindowText ( "Setup Customer Transaction Presets" );		
		break;

	case NODE_PRESET_STOCK_HIST:
		SetWindowText ( "Setup Stock Sales History Presets" );		
		break;

	case NODE_PRESET_STOCK_LEVEL:
		SetWindowText ( "Setup Stock Report Presets" );		
		break;

	case NODE_PRESET_STOCK_CLOSING:
		SetWindowText ( "Setup Closing Stock Report Presets" );		
		break;

	case NODE_PRESET_STOCK_AUDIT:
		SetWindowText ( "Setup Stock Audit Presets" );		
		break;

	case NODE_PRESET_EPOS: 
		SetWindowText ( "Setup EPOS Report Presets" );		
		break;

	case NODE_PRESET_ECR_STOCK: 
		SetWindowText ( "Setup Stock Level Report Presets" );		
		break;

	case NODE_PRESET_TIMEATTEND: 
		SetWindowText ( "Setup Time && Attendance Report Presets" );		
		break;
	}
}

/**********************************************************************/

void CListDataPresetDlg::SetButtonTexts()
{
	m_button1.SetWindowText ( "&Edit Name" );
	m_button2.SetWindowText ( "&Delete" );
	m_button3.ShowWindow( SW_HIDE );
	m_button4.SetWindowText ( "Move &Up" );
	m_button5.SetWindowText ( "Move D&own" );
	m_button6.ShowWindow( SW_HIDE );

	MoveControl( &m_button1, 368, 7 );
	MoveControl( &m_button2, 368, 47 );
	MoveControl( &m_button4, 368, 147 );
	MoveControl( &m_button5, 368, 187 );
}

/**********************************************************************/

void CListDataPresetDlg::SetColumns()
{
	m_pContentHelpers -> SetColumns( m_listData, this );
}

/**********************************************************************/

void CListDataPresetDlg::PrepareList()
{
	CListDataDlg::PrepareList();
	SelectLine( m_nInitialSel );
}

/**********************************************************************/

void CListDataPresetDlg::HandleButton1() 
{
	int nSelection = GetSafeSelection();
	if ( nSelection == -1 )
		return;

	CDefEditDlg dlg( "Edit Preset Name", m_arrayPresets.GetName( nSelection ), 50, "Name", this );
	
	if ( dlg.DoModal() == IDOK )
	{
		m_arrayPresets.SetName( nSelection, dlg.m_strName );
		SelectLine( nSelection );
		m_WarnCancelAfterChange.SetFlag();
	}
}
		
/**********************************************************************/
	
void CListDataPresetDlg::HandleButton2() 
{
	m_pDisplayHelpers -> HandleDelete( this );
	m_WarnCancelAfterChange.SetFlag( m_pContentHelpers -> GetDeletionEffectFlag() );
}

/**********************************************************************/

void CListDataPresetDlg::HandleButton4()  
{
	int nSelection = GetSafeSelection ( 1, GetArraySize() - 1 );
	if ( nSelection != -1 )
	{
		m_arrayPresets.SwapItems( nSelection - 1 );
		SelectLine ( nSelection - 1 );
		m_WarnCancelAfterChange.SetFlag();
	}
}

/**********************************************************************/

void CListDataPresetDlg::HandleButton5() 
{
	int nSelection = GetSafeSelection ( 0, GetArraySize() - 2 );
	if ( nSelection != -1 ) 
	{
		m_arrayPresets.SwapItems( nSelection );
		SelectLine ( nSelection + 1 );
		m_WarnCancelAfterChange.SetFlag();
	}
}

/**********************************************************************/