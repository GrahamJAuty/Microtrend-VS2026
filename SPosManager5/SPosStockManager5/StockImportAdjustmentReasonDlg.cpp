/**********************************************************************/
#include "..\CommonEcrStock\ListDataUnitDlg.h"
#include "MaxLengths.h"
/**********************************************************************/
#include "StockImportAdjustmentReasonDlg.h"
/**********************************************************************/

CStockImportAdjustmentReasonDlg::CStockImportAdjustmentReasonDlg( CString& strLastAdjustment, CWnd* pParent)
	: CDialog(CStockImportAdjustmentReasonDlg::IDD, pParent), m_strLastAdjustment( strLastAdjustment )
{
	//{{AFX_DATA_INIT(CStockImportAdjustmentReasonDlg)
	//}}AFX_DATA_INIT
}

/**********************************************************************/

void CStockImportAdjustmentReasonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStockImportAdjustmentReasonDlg)
	DDX_Control(pDX, IDC_BUTTON_SET, m_buttonSet);
	DDX_Control(pDX, IDC_COMBO_REASON, m_comboReason);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_COMBO_REASON, m_strLastAdjustment);
	DDV_MaxChars(pDX, m_strLastAdjustment, MAX_LENGTH_GENERAL_REASON);
}
/**********************************************************************/

BEGIN_MESSAGE_MAP(CStockImportAdjustmentReasonDlg, CDialog)
	//{{AFX_MSG_MAP(CStockImportAdjustmentReasonDlg)
	ON_BN_CLICKED(IDC_BUTTON_SET, OnButtonSet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CStockImportAdjustmentReasonDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if ( DealerFlags.GetImportBarcodeFlag() == TRUE )
		SetWindowText( "Import Adjustments by Barcode Number" );
	else
		SetWindowText( "Import Adjustments by Plu Number" );

	for ( int nIndex = 0; nIndex < DataManagerNonDb.AdjustText.GetSize(); nIndex++ )
		m_comboReason.AddString ( DataManagerNonDb.AdjustText.GetText( nIndex ) );

	m_comboReason.SetWindowText( m_strLastAdjustment );
	return TRUE;  
}

/**********************************************************************/

bool CStockImportAdjustmentReasonDlg::CheckReason()
{
	::TrimSpaces( m_strLastAdjustment, FALSE );
	if ( m_strLastAdjustment == "" )
	{
		Prompter.Error ( "You must enter a reason for the imported adjustments." );
		return FALSE;
	}

	return TRUE;
}

/**********************************************************************/

void CStockImportAdjustmentReasonDlg::OnButtonSet() 
{
	CDataManagerInfo info;
	if ( DataManagerNonDb.OpenAdjustTextBuffer( info ) == FALSE )
		Prompter.ShareError( info );
	else
	{
		CString strText;
		m_comboReason.GetWindowText( strText );

		CListDataUnitDlg dlg ( DataManagerNonDb.AdjustText, strText, this );
		if ( dlg.DoModal() == IDOK )
		{
			if ( DataManagerNonDb.WriteAdjustTextBuffer( info ) == FALSE )
				Prompter.WriteError( info );
			else
			{
				m_comboReason.ResetContent();
				for ( int nIndex = 0; nIndex < DataManagerNonDb.AdjustText.GetSize(); nIndex++ )
					m_comboReason.AddString ( DataManagerNonDb.AdjustText.GetText( nIndex ) );
		
				m_comboReason.SetWindowText( dlg.GetSelectedText() );
			}
		}

		DataManagerNonDb.CloseAdjustTextBuffer( info );
	}
}

/**********************************************************************/

void CStockImportAdjustmentReasonDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == FALSE )
		return;

	if ( CheckReason() == FALSE )
		return;
		
	EndDialog ( IDOK );
}

/**********************************************************************/

