/**********************************************************************/
 
/**********************************************************************/
//#include "DataManager.h"
/**********************************************************************/
#include "ListLabelsItemEditDlg.h"
/**********************************************************************/

CListLabelsItemEditDlg::CListLabelsItemEditDlg( CLabelListCSVRecord& LabelListRecord, CWnd* pParent )
	: CSSDialog(CListLabelsItemEditDlg::IDD, pParent), m_LabelListRecord( LabelListRecord )
{
	//{{AFX_DATA_INIT(CListLabelsItemEditDlg)
	//}}AFX_DATA_INIT
	m_nBarcodeQty = m_LabelListRecord.GetQtyToPrint();
}

/**********************************************************************/

void CListLabelsItemEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CListLabelsItemEditDlg)
	DDX_Control(pDX, IDC_STATIC_BARCODE, m_staticBarcode);
	DDX_Control(pDX, IDC_EDIT_BARCODE, m_editBarcode);
	DDX_Control(pDX, IDC_STATIC_PLU, m_staticPlu);
	DDX_Control(pDX, IDC_EDIT_PLU, m_editPlu);
	DDX_Control(pDX, IDC_STATIC_DESCRIPTION, m_staticDescription);
	DDX_Control(pDX, IDC_EDIT_DESCRIPTION, m_editDescription);
	DDX_Control(pDX, IDC_STATIC_MODIFIER, m_staticModifier);
	DDX_Control(pDX, IDC_EDIT_MODIFIER, m_editModifier);
	DDX_Control(pDX, IDC_STATIC_QTY, m_staticQty);
	DDX_Control(pDX, IDOK, m_buttonOK);
	DDX_Control(pDX, IDCANCEL, m_buttonCancel);
	//}}AFX_DATA_MAP
	DDX_Text ( pDX, IDC_EDIT_QTY, m_nBarcodeQty );
	DDV_MinMaxInt(pDX, m_nBarcodeQty, 1, 99999);	
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CListLabelsItemEditDlg, CDialog)
	//{{AFX_MSG_MAP(CListLabelsItemEditDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/
CEdit* CListLabelsItemEditDlg::GetEditQty() { return GetEdit( IDC_EDIT_QTY ); }
/**********************************************************************/

BOOL CListLabelsItemEditDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	SubclassEdit ( IDC_EDIT_QTY, SS_NUM, 5, "%d" );

	if ( m_LabelListRecord.GetDirectPluFlag() == TRUE )
	{
		ShowAndEnableWindow( &m_staticBarcode, FALSE );
		ShowAndEnableWindow( &m_editBarcode, FALSE );
		MoveControl( &m_staticPlu, 7, 14 );
		MoveControl( &m_editPlu, 70, 12 );
		MoveControl( &m_staticDescription, 7, 34 );
		MoveControl( &m_editDescription, 70, 32 );
		ShowAndEnableWindow( &m_staticModifier, FALSE );
		ShowAndEnableWindow( &m_editModifier, FALSE );
		MoveControl( &m_staticQty, 7, 64 );
		MoveControl( GetEditQty(), 70, 64 );
		MoveControl( &m_buttonOK, 266, 86 );
		MoveControl( &m_buttonCancel, 266, 102 );
		ResizeDialog( 323, 124 );
	}

	m_editBarcode.SetWindowText( "" );
	m_editPlu.SetWindowText( "" );
	m_editModifier.SetWindowText( "" );
	m_editDescription.SetWindowText( "" );

	__int64 nBarcodeNo = m_LabelListRecord.GetBarcodeNoInt();
	__int64 nPluNo = m_LabelListRecord.GetPluNoInt();
	int nModifier = m_LabelListRecord.GetModifier();

	int nPluIdx;
	if ( nPluNo != 0 )
	{
		if ( DataManager.Plu.FindPluByNumber( nPluNo, nPluIdx ) )
		{
			CPluCSVRecord PluRecord;
			DataManager.Plu.GetAt( nPluIdx, PluRecord );
			m_editDescription.SetWindowText( PluRecord.GetReportText() );
		}
		else
		{
			m_editDescription.SetWindowText(  "Unknown" );
		}

		if ( nModifier != 0 )
			m_editModifier.SetWindowText( DataManager.Modifier.GetDisplayName( nModifier ) );
	}

	CString strBarcodeNo;
	::FormatBarcodeNo( nBarcodeNo, strBarcodeNo );
	m_editBarcode.SetWindowText( strBarcodeNo );

	CString strPluNo;
	::FormatPluNo( nPluNo, strPluNo );
	m_editPlu.SetWindowText( strPluNo );
	
	return TRUE;  
}

/**********************************************************************/

void CListLabelsItemEditDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		m_LabelListRecord.SetQtyToPrint( m_nBarcodeQty );
		EndDialog( IDOK );
	}
}

/**********************************************************************/

