/**********************************************************************/
#include "..\CommonEcrStock\DefEditDlg.h"
#include "NodeTypes.h"
#include "..\CommonEcrStock\NewRecordNumberDlg.h"
#include "EcrmanSupplierCSVArray.h"
/**********************************************************************/
#include "ListDataEcrmanSupplierDlg.h"
/**********************************************************************/

CSingleListContentHelpersEcrmanSupplier::CSingleListContentHelpersEcrmanSupplier()
{
}

/**********************************************************************/

int CSingleListContentHelpersEcrmanSupplier::GetArraySize()
{
	return DataManagerNonDb.EcrmanSupplier.GetSize();
}

/**********************************************************************/

bool CSingleListContentHelpersEcrmanSupplier::PrepareLine( int nIndex, CString& strLine )
{
	strLine = "";

	bool bResult = FALSE;
	if ( ( nIndex >= 0 ) && ( nIndex < GetArraySize() ) )
	{
		CEcrmanSupplierCSVRecord Supplier;
		DataManagerNonDb.EcrmanSupplier.GetAt( nIndex, Supplier );

		strLine.Format ( "%d\t%s", 
			Supplier.GetSuppNo(),
			Supplier.GetSuppName() );

		bResult = TRUE;
	}
	
	return bResult;
}

/**********************************************************************/

void CSingleListContentHelpersEcrmanSupplier::DeleteDatabaseEntry( int nIndex )
{
	if ( ( nIndex >= 0 ) && ( nIndex < DataManagerNonDb.EcrmanSupplier.GetSize() ) )
	{
		DataManagerNonDb.EcrmanSupplier.RemoveAt ( nIndex );
		m_bDeletionEffect = TRUE;
	}
}

/**********************************************************************/

void CSingleListContentHelpersEcrmanSupplier::SetColumns( CSSListCtrlVirtual& list, CWnd* pParent )
{
	list.SubclassDlgItem ( IDC_LIST, pParent );
	list.InsertColumn ( 0, "No", LVCFMT_LEFT, 50 );
	list.InsertColumn ( 1, "Name", LVCFMT_LEFT, 430 );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListDataEcrmanSupplierDlg::CListDataEcrmanSupplierDlg( int nInitialSuppNo, CWnd* pParent) : CListDataDlg ( pParent )
{
	m_nInitialSuppNo = nInitialSuppNo;

	m_pContentHelpers = new CSingleListContentHelpersEcrmanSupplier();
	m_pDisplayHelpers = new CSingleListDisplayHelpers( m_listData, *m_pContentHelpers );

	m_pDisplayHelpers -> SetTopSingleText( "supplier" );
	m_pDisplayHelpers -> SetTopPluralText( "suppliers" );
	m_pDisplayHelpers -> SetBottomSingleText( "supplier" );
	m_pDisplayHelpers -> SetBottomPluralText( "suppliers" );
}

/**********************************************************************/

CListDataEcrmanSupplierDlg::~CListDataEcrmanSupplierDlg()
{
	delete m_pDisplayHelpers;
	delete m_pContentHelpers;
}

/**********************************************************************/

void CListDataEcrmanSupplierDlg::SetTitle()
{
	SetWindowText ( "Setup Suppliers" );
}

/**********************************************************************/

void CListDataEcrmanSupplierDlg::SetButtonTexts()
{
	m_button1.SetWindowText ( "&Add" );
	m_button2.SetWindowText ( "&Edit" );
	m_button3.SetWindowText ( "&Delete" );
	m_button4.ShowWindow ( SW_HIDE );
	m_button5.ShowWindow ( SW_HIDE );
	m_button6.ShowWindow ( SW_HIDE );

	MoveControl( &m_button1, 368, 7 );
	MoveControl( &m_button2, 368, 47 );
	MoveControl( &m_button3, 368, 87 );
}

/**********************************************************************/

void CListDataEcrmanSupplierDlg::SetColumns()
{
	m_pContentHelpers -> SetColumns( m_listData, this );
}

/**********************************************************************/

void CListDataEcrmanSupplierDlg::PrepareList()
{
	CListDataDlg::PrepareList();

	int nSuppIdx;
	if ( DataManagerNonDb.EcrmanSupplier.FindSupplierByNumber( m_nInitialSuppNo, nSuppIdx ) == TRUE )
		SelectLine( nSuppIdx );
}

/**********************************************************************/

void CListDataEcrmanSupplierDlg::HandleButton1() 
{
	int nSuppNo = DataManagerNonDb.EcrmanSupplier.GetNextFreeNumber();

	if ( -1 == nSuppNo )
	{
		Prompter.Error ( "All possible supplier numbers are already in use." );
		return;
	}

	int nSuppIdx;

	while( TRUE )
	{
		CNewRecordNumberDlg dlg ( NODE_ECR_SUPPLIER, this );
		dlg.m_nNumber = nSuppNo;

		if ( dlg.DoModal() != IDOK )
			return;

		nSuppNo = dlg.m_nNumber;
		if ( DataManagerNonDb.EcrmanSupplier.FindSupplierByNumber( nSuppNo, nSuppIdx ) == FALSE )
			break;

		CString strMsg;
		strMsg.Format ( "Supplier %d is already in use.", nSuppNo );
		Prompter.Error( strMsg );
	}

	CEcrmanSupplierCSVRecord Supplier;
	Supplier.SetSuppNo( nSuppNo );
	Supplier.SetSuppName( "" );

	DataManagerNonDb.EcrmanSupplier.InsertAt( nSuppIdx, Supplier );
	SelectLine( nSuppIdx );

	m_listData.SetItemCountEx( GetArraySize() );
	SelectLine( nSuppIdx );
	
	m_WarnCancelAfterChange.SetFlag();

	HandleButton2();
}

/**********************************************************************/

void CListDataEcrmanSupplierDlg::HandleButton2() 
{
	int nIndex = GetSafeSelection();
	if ( nIndex == -1 ) 
		return;

	CChangeEcrmanSupplierNameDlg dlg ( nIndex, this );
		
	if ( dlg.DoModal() == IDOK )
	{
		CEcrmanSupplierCSVRecord Supplier;
		DataManagerNonDb.EcrmanSupplier.GetAt( nIndex, Supplier );
		Supplier.SetSuppName( dlg.m_strName );
		DataManagerNonDb.EcrmanSupplier.SetAt( nIndex, Supplier );
		SelectLine ( nIndex );

		m_WarnCancelAfterChange.SetFlag();
	}
}

/**********************************************************************/

void CListDataEcrmanSupplierDlg::HandleButton3() 
{
	m_pDisplayHelpers -> HandleDelete( this );
	m_WarnCancelAfterChange.SetFlag( m_pContentHelpers -> GetDeletionEffectFlag() );
}

/**********************************************************************/

