/**********************************************************************/
#include "..\CommonEcrStock\DefEditDlg.h"
#include "MaxLengths.h"
#include "NodeTypes.h"
#include "PMSClosedRoomCSVArray.h"
/**********************************************************************/
#include "..\CommonEcrStock\ListDataUnitDlg.h"
/**********************************************************************/

CSingleListContentHelpersUnit::CSingleListContentHelpersUnit( CDefTextUnit& UnitText ) : m_UnitText( UnitText )
{
}

/**********************************************************************/

int CSingleListContentHelpersUnit::GetArraySize()
{
	return m_UnitText.GetSize();
}

/**********************************************************************/

bool CSingleListContentHelpersUnit::PrepareLine( int nIndex, CString& strLine )
{
	strLine = "";

	bool bResult = FALSE;
	if ( ( nIndex >= 0 ) && ( nIndex < GetArraySize() ) )
	{
		strLine = m_UnitText.GetText( nIndex );
		bResult = TRUE;
	}
	
	return bResult;
}

/**********************************************************************/

void CSingleListContentHelpersUnit::DeleteDatabaseEntry( int nIndex )
{
	if ( ( nIndex >= 0 ) && ( nIndex < m_UnitText.GetSize() ) )
	{
		m_UnitText.RemoveUnit ( nIndex );
		m_bDeletionEffect = TRUE;
	}
}

/**********************************************************************/

void CSingleListContentHelpersUnit::SetColumns( CSSListCtrlVirtual& list, CWnd* pParent )
{
	list.SubclassDlgItem ( IDC_LIST, pParent );
	list.InsertColumn ( 0, "Text", LVCFMT_LEFT, 480 );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListDataUnitDlg::CListDataUnitDlg( CDefTextUnit& UnitText, const char* szUnitText, CWnd* pParent) : CListDataDlg ( pParent ), m_UnitText ( UnitText )
{
	m_nType = m_UnitText.GetType();
	m_strInitialUnit = szUnitText;
	m_strTitleInfo = "";
	::TrimSpaces( m_strInitialUnit, FALSE );

	switch( m_nType )
	{
	case NODE_ADJUSTTEXT:
		m_nMaxLength = MAX_LENGTH_GENERAL_REASON;
		break;

#ifdef COMPILE_PMS

	case NODE_PMSCLOSURE:
		m_nMaxLength = PMSClosure::Reason.Max;
		break;

#endif

	default:
		m_nMaxLength = MAX_LENGTH_UNIT_NAME;
		break;
	}

	m_pContentHelpers = new CSingleListContentHelpersUnit( UnitText );
	m_pDisplayHelpers = new CSingleListDisplayHelpers( m_listData, *m_pContentHelpers );

	switch( m_nType )
	{
	case NODE_CUSTOM_FIELD_SET:
		m_pDisplayHelpers -> SetTopSingleText( "unit text" );
		m_pDisplayHelpers -> SetTopPluralText( "unit texts" );
		m_pDisplayHelpers -> SetBottomSingleText( "text" );
		m_pDisplayHelpers -> SetBottomPluralText( "texts" );
		break;

	default:
		m_pDisplayHelpers -> SetTopSingleText( "set name" );
		m_pDisplayHelpers -> SetTopPluralText( "set names" );
		m_pDisplayHelpers -> SetBottomSingleText( "set name" );
		m_pDisplayHelpers -> SetBottomPluralText( "set names" );
		break;
	}
}

/**********************************************************************/

CListDataUnitDlg::~CListDataUnitDlg()
{
	delete m_pDisplayHelpers;
	delete m_pContentHelpers;
}

/**********************************************************************/

void CListDataUnitDlg::SetTitle()
{
	CString strTitle = "";

	switch ( m_nType )
	{
	case NODE_STOCKUNIT:		strTitle = "Setup Stock Unit Texts";		break;
	case NODE_SUBUNIT:			strTitle = "Setup Sub Unit Texts";			break;
	case NODE_ADJUSTTEXT:		strTitle = "Setup Adjustment Texts";		break;
	case NODE_PMSCLOSURE:		strTitle = "Setup Room Closure Texts";		break;
	case NODE_CUSTOM_FIELD_SET:	strTitle = "Setup System Custom Field Sets";break;
	case NODE_PLUUNIT:			
	default:					strTitle = "Setup Unit Price Texts";		break;
	}

	if ( m_strTitleInfo != "" )
	{
		 strTitle += " (";
		 strTitle += m_strTitleInfo;
		 strTitle += ")";
	}

	SetWindowText( strTitle );
}

/**********************************************************************/

void CListDataUnitDlg::SetButtonTexts()
{
	m_button1.SetWindowText ( "&Add" );
	m_button2.SetWindowText ( "&Edit" );
	m_button3.SetWindowText ( "&Delete" );
	m_button4.SetWindowText ( "Move &Up" );
	m_button5.SetWindowText ( "Move D&own" );
	m_button6.SetWindowText ( "De&fault" );
	
	switch( m_nType )
	{
	case NODE_ADJUSTTEXT:
	case NODE_CUSTOM_FIELD_SET:
		ShowAndEnableWindow( &m_button6, FALSE );
		break;
	}

	MoveControl( &m_button1, 368, 7 );
	MoveControl( &m_button2, 368, 42 );
	MoveControl( &m_button3, 368, 77 );
	MoveControl( &m_button4, 368, 122 );
	MoveControl( &m_button5, 368, 157 );
	MoveControl( &m_button6, 368, 202 );
}

/**********************************************************************/

void CListDataUnitDlg::SetColumns()
{
	m_pContentHelpers -> SetColumns( m_listData, this );
}

/**********************************************************************/

void CListDataUnitDlg::PrepareList()
{
	CListDataDlg::PrepareList();

	CString strTest = m_strInitialUnit;
	strTest.MakeUpper();
	
	for ( int n = 0; n < GetArraySize(); n++ )
	{
		CString strUnitText = m_UnitText.GetText(n);
		strUnitText.MakeUpper();

		if ( strUnitText == strTest )
		{
			SelectLine(n);
			return;
		}
	}

	if ( m_strInitialUnit != "" )
	{
		m_UnitText.AddUnit( m_strInitialUnit );
		m_listData.SetItemCountEx( GetArraySize() );
		SelectLine( GetArraySize() - 1 );
	}
}

/**********************************************************************/

void CListDataUnitDlg::HandleButton1() 
{
	CString strTitle;
		
	switch ( m_nType )
	{
	case NODE_STOCKUNIT:		strTitle = "Add Stock Unit Text"	;	break;
	case NODE_SUBUNIT:			strTitle = "Add Sub Unit Text";			break;
	case NODE_ADJUSTTEXT:		strTitle = "Add Adjustment Text";		break;
	case NODE_PMSCLOSURE:		strTitle = "Add Room Closure Text";		break;
	case NODE_CUSTOM_FIELD_SET:	strTitle = "Add Custom Field Set";		break;
	case NODE_PLUUNIT:		
	default:					strTitle = "Add Unit Price Text";		break;
	}

	CDefEditDlg dlg ( strTitle, "", m_nMaxLength, "", this );
	if ( dlg.DoModal() != IDOK )
		return;
	
	::TrimSpaces ( dlg.m_strName, FALSE );
			
	if ( dlg.m_strName != "" )
	{
		m_UnitText.AddUnit ( dlg.m_strName );
		m_listData.SetItemCountEx ( GetArraySize() );
		SelectLine ( m_UnitText.GetSize() - 1 );
		
		m_WarnCancelAfterChange.SetFlag();
	}
}

/**********************************************************************/

void CListDataUnitDlg::HandleButton2() 
{
	int nSelection = GetSafeSelection();	
	if ( nSelection == -1 )
		return;
	
	CString strTitle;
		
	switch ( m_nType )
	{
	case NODE_STOCKUNIT:		strTitle = "Edit Stock Unit Text";		break;
	case NODE_SUBUNIT:			strTitle = "Edit Sub Unit Text";		break;
	case NODE_ADJUSTTEXT:		strTitle = "Edit Adjustment Text";		break;
	case NODE_PMSCLOSURE:		strTitle = "Edit Room Closure Text";	break;
	case NODE_CUSTOM_FIELD_SET:	strTitle = "Edit Custom Field Set";		break;
	case NODE_PLUUNIT:		
	default:					strTitle = "Edit Unit Price Text";		break;
	}

	CString strOldName = m_UnitText.GetText ( nSelection );
	CDefEditDlg dlg ( strTitle, strOldName, m_nMaxLength, "", this );
	if ( dlg.DoModal() != IDOK )
		return;

	::TrimSpaces ( dlg.m_strName, FALSE );

	if ( dlg.m_strName != "" )
	{
		m_UnitText.SetText ( nSelection, dlg.m_strName );
		SelectLine ( nSelection );

		m_WarnCancelAfterChange.SetFlag();
	}
}

/**********************************************************************/

void CListDataUnitDlg::HandleButton3() 
{
	m_pDisplayHelpers -> HandleDelete( this );
	m_WarnCancelAfterChange.SetFlag( m_pContentHelpers -> GetDeletionEffectFlag() );
}

/**********************************************************************/

void CListDataUnitDlg::HandleButton4()
{
	int nIndex = GetSafeSelection();
	if ( nIndex <= 0 ) 
		return;

	m_UnitText.SwapWithNext( nIndex - 1 );
	SelectLine( nIndex - 1 );

	m_WarnCancelAfterChange.SetFlag();
}

/**********************************************************************/

void CListDataUnitDlg::HandleButton5()
{
	int nIndex = GetSafeSelection();
	if ( nIndex >= GetArraySize() - 1 ) 
		return;

	m_UnitText.SwapWithNext( nIndex );
	SelectLine( nIndex + 1 );

	m_WarnCancelAfterChange.SetFlag();
}

/**********************************************************************/

void CListDataUnitDlg::HandleButton6() 
{	
	if ( Prompter.YesNo ( "Are you sure you wish to restore the default texts" ) == IDYES )
	{
		m_UnitText.SetDefaults();
		m_listData.SetItemCountEx ( GetArraySize() );
		SelectLine(0);

		m_WarnCancelAfterChange.SetFlag();
	}
}

/**********************************************************************/

const char* CListDataUnitDlg::GetSelectedText()
{
	if ( m_nListSelection >= 0 && m_nListSelection < m_UnitText.GetSize() )
		m_strSelectedText = m_UnitText.GetText ( m_nListSelection );
	else
		m_strSelectedText = "";

	return m_strSelectedText;
}

/**********************************************************************/