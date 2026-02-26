/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "..\CommonEcrStock\ListDataUnitDlg.h"
#include "StockOptionsIni.h"
/**********************************************************************/
#include "ListDataSystemCustomFieldSetDlg.h"
/**********************************************************************/

CSingleListContentHelpersSystemCustomFieldSet::CSingleListContentHelpersSystemCustomFieldSet() 
{
}

/**********************************************************************/

int CSingleListContentHelpersSystemCustomFieldSet::GetArraySize()
{
	return MAX_ITEM_CUSTOM_FIELDS;
}

/**********************************************************************/

bool CSingleListContentHelpersSystemCustomFieldSet::PrepareLine( int nIndex, CString& strLine )
{
	strLine = "";

	bool bResult = FALSE;
	if ( ( nIndex >= 0 ) && ( nIndex < GetArraySize() ) )
	{
		strLine = DataManagerNonDb.CustomFieldNames.GetName( nIndex + 1 );
	
		if ( strLine == "" )
			strLine.Format ( "Custom Field %d", nIndex + 1 );

		bResult = TRUE;
	}
	
	return bResult;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListDataSystemCustomFieldSetDlg::CListDataSystemCustomFieldSetDlg( CWnd* pParent) : CListDataDlg ( pParent )
{
	m_bMultiSelect = FALSE;
	m_pContentHelpers = new CSingleListContentHelpersSystemCustomFieldSet();
	m_pDisplayHelpers = new CSingleListDisplayHelpers( m_listData, *m_pContentHelpers );
}

/**********************************************************************/

CListDataSystemCustomFieldSetDlg::~CListDataSystemCustomFieldSetDlg()
{
	delete m_pDisplayHelpers;
	delete m_pContentHelpers;
}

/**********************************************************************/

void CListDataSystemCustomFieldSetDlg::SetTitle()
{
	SetWindowText ( "Setup System Custom Field Sets" );
}

/**********************************************************************/

void CListDataSystemCustomFieldSetDlg::SetButtonTexts()
{
	m_button1.SetWindowText ( "&Sets" );
	m_button2.ShowWindow ( SW_HIDE );
	m_button3.ShowWindow ( SW_HIDE );
	m_button4.ShowWindow ( SW_HIDE );
	m_button5.ShowWindow ( SW_HIDE );
	m_button6.ShowWindow ( SW_HIDE );
	m_buttonOK.ShowWindow ( SW_HIDE );
	m_buttonCancel.SetWindowText( "Close" );
}

/**********************************************************************/

void CListDataSystemCustomFieldSetDlg::SetColumns()
{
	m_listData.SubclassDlgItem ( IDC_LIST, this );
	m_listData.InsertColumn ( 0, "Name", LVCFMT_LEFT, 480 );
}

/**********************************************************************/

void CListDataSystemCustomFieldSetDlg::HandleButton1() 
{
	int nIndex = GetSafeSelection();
	if ( nIndex == -1 )	
		return;

	int nFieldNo = nIndex + 1;

	CDataManagerInfo info;
	if ( DataManagerNonDb.OpenSystemCustomFieldSet( nFieldNo, DB_READWRITE, info ) == FALSE )
	{
		Prompter.ShareError( info );
	}
	else
	{
		CListDataUnitDlg dlgSets( DataManagerNonDb.SystemCustomFieldSet[nFieldNo], "", this );
		dlgSets.SetTitleInfo( DataManagerNonDb.CustomFieldNames.GetName(nFieldNo) );

		if ( dlgSets.DoModal() == IDOK )
		{
			if ( DataManagerNonDb.WriteSystemCustomFieldSet( nFieldNo, info ) == FALSE )
				Prompter.WriteError( info );
			
			DataManagerNonDb.CloseSystemCustomFieldSet( nFieldNo, info );
		}
	}

	SelectLine( nIndex );
}

/**********************************************************************/
