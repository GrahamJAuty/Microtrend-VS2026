/**********************************************************************/
#include "DatabaseCSVArray.h"
/**********************************************************************/
#include "BarcodeDeleteDlg.h"
/**********************************************************************/

CBarcodeDeleteDlg::CBarcodeDeleteDlg(CArray<int,int>& arrayBarcodeIdx, CWnd* pParent)
	: CDialog(CBarcodeDeleteDlg::IDD, pParent), m_arrayBarcodeIdx ( arrayBarcodeIdx )
{
	//{{AFX_DATA_INIT(CBarcodeDeleteDlg)
	//}}AFX_DATA_INIT
}

/**********************************************************************/

void CBarcodeDeleteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBarcodeDeleteDlg)
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CBarcodeDeleteDlg, CDialog)
	//{{AFX_MSG_MAP(CBarcodeDeleteDlg)
	ON_WM_MEASUREITEM()
	ON_BN_CLICKED(IDC_BUTTON_ALL, OnButtonAll)
	ON_BN_CLICKED(IDC_BUTTON_NONE, OnButtonNone)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CBarcodeDeleteDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();	

	m_listBarcodes.SubclassDlgItem ( IDC_LIST, this );
	m_listBarcodes.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_listBarcodes.InsertColumn ( 0, "Barcodes", LVCFMT_LEFT, 210 );
	m_listBarcodes.SetLockItemZeroFlag( FALSE );

	CString strTitle;
	strTitle.Format( "Delete Unlinked Barcodes (%s)", dbDatabase.GetName( DataManager.GetActiveDbIdx() ) );
	SetWindowText( strTitle );

	AddList();
	return TRUE;  
}

/**********************************************************************/

void CBarcodeDeleteDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
   if ( nIDCtl == IDC_LIST )   
   {
	   m_listBarcodes.AdjustRowHeight( lpMeasureItemStruct, 10 ); 
   }
}

/**********************************************************************/

void CBarcodeDeleteDlg::OnButtonAll() 
{	
	m_listBarcodes.SelectAll();
}

/**********************************************************************/

void CBarcodeDeleteDlg::OnButtonNone() 
{	
	m_listBarcodes.SelectNone();
}

/**********************************************************************/

void CBarcodeDeleteDlg::AddList()
{
	for ( int n = 0; n < m_arrayBarcodeIdx.GetSize(); n++ )
	{
		int nBarcodeIdx = m_arrayBarcodeIdx.GetAt(n);

		CSSListTaggedSelectItem item;
		item.m_strText = DataManager.Barcode.GetBarcodeNoString( nBarcodeIdx );
		item.m_nData = 0;
		item.SetTagState(0);
		m_listBarcodes.AddItemToList( item );
	}
}

/**********************************************************************/

void CBarcodeDeleteDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		for ( int n = m_listBarcodes.GetItemCount() - 1; n >= 0; n-- )
		{
			if ( m_listBarcodes.GetItemTagState(n) != 1 )
			{
				m_arrayBarcodeIdx.RemoveAt(n);
			}
		}

		EndDialog(IDOK);
	}
}
/**********************************************************************/
