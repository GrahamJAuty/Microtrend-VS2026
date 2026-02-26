/**********************************************************************/
 
/**********************************************************************/
#include "BarcodePluNoTable.h"
#include "BarcodeTableDefaultsDlg.h"
#include "BarcodeTableEditDlg.h"
 
/**********************************************************************/
#include "BarcodeTableListDlg.h"
/**********************************************************************/

CBarcodeTableListDlg::CBarcodeTableListDlg(CWnd* pParent)
	: CDialog(CBarcodeTableListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBarcodeTableListDlg)
	//}}AFX_DATA_INIT
	m_bIsModified = FALSE;
}

/**********************************************************************/

void CBarcodeTableListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBarcodeTableListDlg)
	DDX_Control(pDX, IDOK, m_buttonOK);
	DDX_Control(pDX, IDCANCEL, m_buttonCancel);
	DDX_Control(pDX, IDC_BUTTON2, m_button2);
	DDX_Control(pDX, IDC_BUTTON1, m_button1);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CBarcodeTableListDlg, CDialog)
	//{{AFX_MSG_MAP(CBarcodeTableListDlg)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_NOTIFY(NM_KILLFOCUS, IDC_LIST, OnKillFocusList)
	ON_NOTIFY(NM_SETFOCUS, IDC_LIST, OnSetFocusList)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDoubleClickList)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST, OnGetDispInfoList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

int CBarcodeTableListDlg::GetArraySize()
{
	return BarcodePluNoTable.GetTableSize();
}

/**********************************************************************/

BOOL CBarcodeTableListDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();	
	
	SetWindowText ( "Barcode & Plu Number Entry Details" );

	m_button1.SetWindowText ( "&Edit" );
	m_button2.SetWindowText ( "De&fault" );

	m_listData.SubclassDlgItem ( IDC_LIST, this );
	m_listData.InsertColumn ( 0, "Digits", LVCFMT_RIGHT, 80 );
	m_listData.InsertColumn ( 1, "Barcode Entry", LVCFMT_LEFT, 150 );
	m_listData.InsertColumn ( 2, "Plu Entry", LVCFMT_LEFT, 150 );
	
	m_listData.SetItemCountEx ( GetArraySize() );
	m_listData.SetCurSel(0);

	return TRUE;  
}

/**********************************************************************/

void CBarcodeTableListDlg::PrepareList()
{
	m_listData.SetItemCountEx ( GetArraySize() );
	m_listData.SetCurSel(0);
}

/**********************************************************************/

bool CBarcodeTableListDlg::PrepareLine ( int nIndex )
{
	if ( nIndex < 0 || nIndex >= BarcodePluNoTable.GetTableSize() )
		return FALSE;

	CCSV csv( '\t' );
	csv.Add( nIndex + 1 );
	csv.Add( BarcodePluNoTable.GetBarcodeFormat( nIndex + 1 ) );

	if ( nIndex < Super.MaxPluLen() - 1 )
		csv.Add( BarcodePluNoTable.GetPluFormat( nIndex + 1 ) );

	m_strDisplayLine = csv.GetLine();
	return TRUE;
}

/**********************************************************************/

void CBarcodeTableListDlg::SelectLine ( int nIndex )
{
	int nSize = m_listData.GetItemCount();

	if ( ( nIndex >= 0 ) && ( nIndex < nSize ) )
		m_listData.SetCurSel ( nIndex );
	else 
		m_listData.SetCurSel ( nSize - 1 );
	
	m_listData.Invalidate();
}

/**********************************************************************/

void CBarcodeTableListDlg::OnButton1()
{
	int nIndex = GetSafeSelection();
	if ( nIndex == -1 )
		return;

	CBarcodePluNoTableInfo info;
	info.m_strBarcodeFormat = BarcodePluNoTable.GetBarcodeFormat( nIndex + 1 );
	info.m_strPluFormat = BarcodePluNoTable.GetPluFormat( nIndex + 1 );
	
	CBarcodeTableEditDlg dlg ( nIndex + 1, info, this );
		
	if ( dlg.DoModal() == IDOK )
	{
		BarcodePluNoTable.SetBarcodeFormat ( nIndex + 1, info.m_strBarcodeFormat );
		BarcodePluNoTable.SetPluFormat ( nIndex + 1, info.m_strPluFormat );
		SelectLine ( nIndex );
	}
}

/**********************************************************************/

void CBarcodeTableListDlg::OnButton2()
{
	CBarcodeTableDefaultsDlg dlg( this );
	if ( dlg.DoModal() == IDOK )
	{
		BarcodePluNoTable.RestoreDefaults( dlg.m_nBarcodeAction, dlg.m_nPluAction );
		SelectLine ( GetSafeSelection() );
	}
}

/**********************************************************************/

int CBarcodeTableListDlg::GetSafeSelection()
{
	return GetSafeSelection ( 0, GetArraySize() - 1 );
}

/**********************************************************************/

int CBarcodeTableListDlg::GetSafeSelection( int nMin, int nMax )
{
	int nSel = m_listData.GetCurSel();
	if ( ( nSel < nMin ) || ( nSel > nMax ) || ( nSel >= m_listData.GetItemCount() ) )
		nSel = -1;
	
	return nSel;
}

/**********************************************************************/

void CBarcodeTableListDlg::OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if (( pDispInfo->item.mask & LVIF_TEXT ) != 0 )
	{
		int nIndex = pDispInfo->item.iItem;
		PrepareLine( nIndex );
		CCSV csv ( m_strDisplayLine, '\t' );
		lstrcpy ( pDispInfo->item.pszText, csv.GetString ( pDispInfo->item.iSubItem ) );
	}
	*pResult = 0;
}

/**********************************************************************/

void CBarcodeTableListDlg::OnSetFocusList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	SetDefID ( GetDefaultButtonID() );
	*pResult = 0;
}

/**********************************************************************/

void CBarcodeTableListDlg::OnKillFocusList(NMHDR* pNMHDR, LRESULT* pResult) 
{	
	SetDefID ( IDOK );
	*pResult = 0;
}

/**********************************************************************/

void CBarcodeTableListDlg::OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{	
	OnButton1();
	*pResult = 0;
}

/**********************************************************************/

void CBarcodeTableListDlg::OnOK() 
{
	BarcodePluNoTable.Write();
	EndDialog ( IDOK );
}

/**********************************************************************/

void CBarcodeTableListDlg::OnCancel() 
{
	BarcodePluNoTable.Read();
	EndDialog ( IDCANCEL );
}

/**********************************************************************/

