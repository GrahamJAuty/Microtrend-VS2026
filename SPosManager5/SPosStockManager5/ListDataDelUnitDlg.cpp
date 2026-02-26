/**********************************************************************/
#include "DefEditDelUnitDlg.h"
#include "SysInfo.h"
/**********************************************************************/
#include "ListDataDelUnitDlg.h"
/**********************************************************************/

CSingleListContentHelpersDelUnit::CSingleListContentHelpersDelUnit() 
{
}

/**********************************************************************/

int CSingleListContentHelpersDelUnit::GetArraySize()
{
	return DataManager.DelUnit.GetSize();
}

/**********************************************************************/

bool CSingleListContentHelpersDelUnit::PrepareLine( int nIndex, CString& strLine )
{
	strLine = "";

	bool bResult = FALSE;
	if ( ( nIndex >= 0 ) && ( nIndex < GetArraySize() ) )
	{
		strLine.Format ( "%s\t%d\t%.3f\t%.3f", 
			DataManager.DelUnit.GetText( nIndex ),
			DataManager.DelUnit.GetDUItems( nIndex ),
			DataManager.DelUnit.GetDUItemSize( nIndex ),
			DataManager.DelUnit.GetTotalSU( nIndex ) );

		bResult = TRUE;
	}

	return bResult;
}

/**********************************************************************/

void CSingleListContentHelpersDelUnit::DeleteDatabaseEntry( int nIndex )
{
	if ( ( nIndex >= 0 ) && ( nIndex < DataManager.DelUnit.GetSize() ) )
	{
		DataManager.DelUnit.RemoveAt( nIndex );
		m_bDeletionEffect = TRUE;
	}
}

/**********************************************************************/

void CSingleListContentHelpersDelUnit::SetColumns( CSSListCtrlVirtual& list, CWnd* pParent )
{
	list.SubclassDlgItem ( IDC_LIST, pParent );
	list.InsertColumn ( 0, "Text", LVCFMT_LEFT, 220 );
	list.InsertColumn ( 1, "Items", LVCFMT_RIGHT, 60 );
	list.InsertColumn ( 2, "SU per Item", LVCFMT_RIGHT, 100 );
	list.InsertColumn ( 3, "Total SU", LVCFMT_RIGHT, 100 );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListDataDelUnitDlg::CListDataDelUnitDlg( const char* szText, int nDUItems, double dDUItemSize, CWnd* pParent) : CListDataDlg ( pParent )
{
	m_strInitialText = szText;
	::TrimSpaces( m_strInitialText, FALSE );
	m_nInitialDUItems = nDUItems;
	m_dInitialDUItemSize = dDUItemSize;

	m_pContentHelpers = new CSingleListContentHelpersDelUnit();
	m_pDisplayHelpers = new CSingleListDisplayHelpers( m_listData, *m_pContentHelpers );

	m_pDisplayHelpers -> SetTopSingleText( "delivery unit" );
	m_pDisplayHelpers -> SetTopPluralText( "delivery units" );
	m_pDisplayHelpers -> SetBottomSingleText( "unit" );
	m_pDisplayHelpers -> SetBottomPluralText( "units" );
}

/**********************************************************************/

CListDataDelUnitDlg::~CListDataDelUnitDlg()
{
	delete m_pDisplayHelpers;
	delete m_pContentHelpers;
}

/**********************************************************************/

void CListDataDelUnitDlg::SetTitle()
{
	SetWindowText ( "Setup Stock Delivery Unit Texts" );
}

/**********************************************************************/

void CListDataDelUnitDlg::SetButtonTexts()
{
	m_button1.SetWindowText ( "&Add" );
	m_button2.SetWindowText ( "&Edit" );
	m_button3.SetWindowText ( "&Delete" );
	m_button4.SetWindowText ( "Move &Up" );
	m_button5.SetWindowText ( "Move D&own" );
	m_button6.ShowWindow ( SW_HIDE );

	MoveControl( &m_button1, 368, 7 );
	MoveControl( &m_button2, 368, 47 );
	MoveControl( &m_button3, 368, 87 );
	MoveControl( &m_button4, 368, 147 );
	MoveControl( &m_button5, 368, 187 );
}

/**********************************************************************/

void CListDataDelUnitDlg::SetColumns()
{
	m_pContentHelpers -> SetColumns( m_listData, this );
}

/**********************************************************************/

void CListDataDelUnitDlg::PrepareList()
{
	CListDataDlg::PrepareList();

	if ( m_strInitialText != "" )
	{
		CDefTextDelUnit tempArray;
		tempArray.AddDelUnit( m_strInitialText, m_nInitialDUItems, m_dInitialDUItemSize );
		CString strTest = tempArray.GetComboText(0);;
		strTest.MakeUpper();
	
		for ( int n = 0; n < GetArraySize(); n++ )
		{
			CString strUnitText = DataManager.DelUnit.GetComboText(n);
			strUnitText.MakeUpper();

			if ( strUnitText == strTest )
			{
				SelectLine(n);
				return;
			}
		}

		DataManager.DelUnit.Add( tempArray.GetAt(0) );
		m_listData.SetItemCountEx( GetArraySize() );
		SelectLine( GetArraySize() - 1 );
	}
}

/**********************************************************************/

void CListDataDelUnitDlg::HandleButton1() 
{
	CDefEditDelUnitDlg dlg ( "Add Delivery Unit", this );

	dlg.m_strDeliveryUnit = "";
	dlg.m_nDUItems = 1;
	dlg.m_dDUItemSize = 1.0;

	if ( dlg.DoModal() == IDOK )
	{
		DataManager.DelUnit.AddDelUnit ( dlg.m_strDeliveryUnit, dlg.m_nDUItems, dlg.m_dDUItemSize );
		m_listData.SetItemCountEx ( GetArraySize() );
		SelectLine ( DataManager.DelUnit.GetSize() - 1 );

		m_WarnCancelAfterChange.SetFlag();
	}
}

/**********************************************************************/

void CListDataDelUnitDlg::HandleButton2() 
{
	int nIndex = GetSafeSelection();
	if ( nIndex == -1 ) 
		return;

	CDefEditDelUnitDlg dlg ( "Edit Delivery Unit", this );
	dlg.m_strDeliveryUnit = DataManager.DelUnit.GetText ( nIndex );
	dlg.m_nDUItems = DataManager.DelUnit.GetDUItems( nIndex );
	dlg.m_dDUItemSize = DataManager.DelUnit.GetDUItemSize( nIndex );
		
	if ( dlg.DoModal() == IDOK )
	{
		DataManager.DelUnit.SetDelUnit ( nIndex, dlg.m_strDeliveryUnit, dlg.m_nDUItems, dlg.m_dDUItemSize );
		SelectLine ( nIndex );

		m_WarnCancelAfterChange.SetFlag();
	}
}

/**********************************************************************/

void CListDataDelUnitDlg::HandleButton3() 
{
	m_pDisplayHelpers -> HandleDelete( this );
	m_WarnCancelAfterChange.SetFlag( m_pContentHelpers -> GetDeletionEffectFlag() );
}

/**********************************************************************/

void CListDataDelUnitDlg::HandleButton4()
{
	int nIndex = GetSafeSelection();
	if ( nIndex <= 0 ) 
		return;

	DataManager.DelUnit.SwapWithNext( nIndex - 1 );
	SelectLine( nIndex - 1 );
	
	m_WarnCancelAfterChange.SetFlag();
}

/**********************************************************************/

void CListDataDelUnitDlg::HandleButton5()
{
	int nIndex = GetSafeSelection();
	if ( nIndex >= GetArraySize() - 1 ) 
		return;

	DataManager.DelUnit.SwapWithNext( nIndex );
	SelectLine( nIndex + 1 );

	m_WarnCancelAfterChange.SetFlag();
}

/**********************************************************************/
