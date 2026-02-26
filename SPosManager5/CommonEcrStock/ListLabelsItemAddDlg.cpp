/**********************************************************************/
#include "BarcodePluNoTable.h"
#include "BarcodeFilterDlg.h"
#include "..\CommonEcrStock\PluFilterArray.h"
#include "..\SPosEcrManager5\PluFilterDlgEcrman.h"
#include "..\SPosStockManager5\PluFilterDlgStockman.h"
#include "BarcodePluNoTable.h"
#include "SysInfo.h"
/**********************************************************************/
#include "ListLabelsItemAddDlg.h"
/**********************************************************************/

CListLabelsItemAddDlg::CListLabelsItemAddDlg( bool bProductLabel, CLabelListCSVRecord& LabelListRecord, CBarcodeFilterArray& BarcodeFilterArray, CPluFilterArray& PluFilterArray, CWnd* pParent )
	: CSSDialog(CListLabelsItemAddDlg::IDD, pParent), m_LabelListRecord( LabelListRecord ), m_BarcodeFilterArray ( BarcodeFilterArray ), m_PluFilterArray( PluFilterArray )
{
	//{{AFX_DATA_INIT(CListLabelsItemAddDlg)
	//}}AFX_DATA_INIT
	m_strModifierNotAllowed = "Not applicable for Plu barcodes";
	m_bProductLabel = bProductLabel;
	m_nBarcodeNo = 0;
	m_nPluNo = 0;
	m_nModifier = 0;
	m_nBarcodeQty = 1;
}

/**********************************************************************/

void CListLabelsItemAddDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CListLabelsItemAddDlg)
	DDX_Control(pDX, IDC_RADIO_BARCODE, m_radioBarcode);
	DDX_Control(pDX, IDC_BUTTON_BROWSE_BARCODE, m_buttonBrowseBarcode);
	DDX_Control(pDX, IDC_RADIO_PLU, m_radioPlu);
	DDX_Control(pDX, IDC_BUTTON_BROWSE_PLU, m_buttonBrowsePlu);
	DDX_Control(pDX, IDC_STATIC_DESCRIPTION, m_staticDescription);
	DDX_Control(pDX, IDC_EDIT_DESCRIPTION, m_editDescription);
	DDX_Control(pDX, IDC_STATIC_MODIFIER, m_staticModifier);
	DDX_Control(pDX, IDC_EDIT_MODIFIER, m_editModifier);
	DDX_Control(pDX, IDC_STATIC_QTY, m_staticQty);
	DDX_Control(pDX, IDOK, m_buttonOK);
	DDX_Control(pDX, IDCANCEL, m_buttonCancel);
	DDX_Text(pDX, IDC_EDIT_DESCRIPTION, m_strDescription);
	DDX_Text(pDX, IDC_EDIT_MODIFIER, m_strModifier);
	//}}AFX_DATA_MAP
	DDX_Text ( pDX, IDC_EDIT_QTY, m_nBarcodeQty );
	DDV_MinMaxInt(pDX, m_nBarcodeQty, 1, 99999);	
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CListLabelsItemAddDlg, CDialog)
	//{{AFX_MSG_MAP(CListLabelsItemAddDlg)
	ON_EN_KILLFOCUS(IDC_EDIT_BARCODE, OnKillFocusBarcode)
	ON_EN_KILLFOCUS(IDC_EDIT_PLU, OnKillFocusPlu)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_BARCODE, OnButtonBrowseBarcode)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_PLU, OnButtonBrowsePlu)
	ON_BN_CLICKED(IDC_RADIO_BARCODE, OnRadioBarcode)
	ON_BN_CLICKED(IDC_RADIO_PLU, OnRadioPlu)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/
CEdit* CListLabelsItemAddDlg::GetEditBarcodeNo() { return GetEdit( IDC_EDIT_BARCODE ); }
CEdit* CListLabelsItemAddDlg::GetEditPluNo() { return GetEdit( IDC_EDIT_PLU ); }
CEdit* CListLabelsItemAddDlg::GetEditQty() { return GetEdit( IDC_EDIT_QTY ); }
/**********************************************************************/

BOOL CListLabelsItemAddDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	SubclassEdit ( IDC_EDIT_BARCODE, SS_NUM, Super.MaxBarcodeLen(), "%s" );
	SubclassEdit ( IDC_EDIT_PLU, SS_NUM, Super.MaxPluLen(), "%s" );
	SubclassEdit ( IDC_EDIT_QTY, SS_NUM, 5, "%d" );

	int nMode = ( SysInfo.GetNewBarcodePluFlag() ? BARCODE_ITEM_PLU : BARCODE_ITEM_BARCODE );
	
	m_nNewItemMode = -1;
	SwitchMode( nMode );

	if ( BARCODE_ITEM_PLU == nMode )
		GetEditPluNo() -> SetFocus();
	else
		GetEditBarcodeNo() -> SetFocus();
		
	return FALSE;  
}

/**********************************************************************/

void CListLabelsItemAddDlg::UpdateEditBoxes( int nMode )
{
	CString strModifier = "";
	CString strDescription = "";

	if ( BARCODE_ITEM_PLU == nMode )
	{
		m_nBarcodeNo = 0;
		m_nModifier = 0;
		strModifier = m_strModifierNotAllowed;
	}
	else
	{
		int nIndex;
		if ( DataManager.Barcode.FindBarcodeByNumber ( m_nBarcodeNo, nIndex ) == TRUE )
		{
			CBarcodeCSVRecord BarcodeRecord;
			DataManager.Barcode.GetAt ( nIndex, BarcodeRecord );
			m_nPluNo = BarcodeRecord.GetPluNoInt(); 
			m_nModifier = BarcodeRecord.GetModifier();
		}
		else
		{
			m_nPluNo = 0;
			m_nModifier = 0;
		}
	}
	
	int nPluIdx;
	if ( m_nPluNo != 0 )
	{
		if ( DataManager.Plu.FindPluByNumber( m_nPluNo, nPluIdx ) )
		{
			CPluCSVRecord PluRecord;
			DataManager.Plu.GetAt( nPluIdx, PluRecord );
			strDescription = PluRecord.GetReportText();
		}
		else
		{
			strDescription = "Unknown";
		}
	}

	if ( m_nModifier != 0 )
		strModifier = DataManager.Modifier.GetDisplayName( m_nModifier );
	
	CString strBarcodeNo = "";
	::FormatBarcodeNo( m_nBarcodeNo, strBarcodeNo );
	GetEditBarcodeNo() -> SetWindowText( strBarcodeNo );

	CString strPluNo = "";
	::FormatPluNo( m_nPluNo, strPluNo );
	GetEditPluNo() -> SetWindowText( strPluNo );

	m_editDescription.SetWindowText( strDescription );
	m_editModifier.SetWindowText( strModifier );
}

/**********************************************************************/
void CListLabelsItemAddDlg::OnRadioBarcode(){ SwitchMode( BARCODE_ITEM_BARCODE ); }
void CListLabelsItemAddDlg::OnRadioPlu(){ SwitchMode( BARCODE_ITEM_PLU ); }
/**********************************************************************/

void CListLabelsItemAddDlg::SwitchMode( int nMode )
{
	bool bBarcode = ( BARCODE_ITEM_BARCODE == nMode );
		
	m_radioBarcode.SetCheck( bBarcode );
	GetEditBarcodeNo() -> EnableWindow( bBarcode );
	m_buttonBrowseBarcode.EnableWindow( bBarcode );

	m_radioPlu.SetCheck( FALSE == bBarcode );
	GetEditPluNo() -> EnableWindow( FALSE == bBarcode );
	m_buttonBrowsePlu.EnableWindow( FALSE == bBarcode );

	if ( nMode != m_nNewItemMode )
	{
		m_nNewItemMode = nMode;
		m_nBarcodeNo = 0;
		m_nPluNo = 0;
		m_nModifier = 0;
		GetEditBarcodeNo() -> SetWindowText( "" );
		GetEditPluNo() -> SetWindowText( "" );

		CString strModifierWarn = "";
		if (FALSE == bBarcode)
		{
			strModifierWarn = m_strModifierNotAllowed;
		}

		m_editModifier.SetWindowText( strModifierWarn );
		m_editDescription.SetWindowText( "" );
		GetEditQty() -> SetWindowText( "1" );
	}
}

/**********************************************************************/

void CListLabelsItemAddDlg::OnKillFocusBarcode() 
{
	__int64 nOldBarcodeNo = m_nBarcodeNo;

	{
		CString strBarcodeNo;
		GetEditBarcodeNo() -> GetWindowText( strBarcodeNo );
		BarcodePluNoTable.ConvertBarcode( strBarcodeNo );
		m_nBarcodeNo = _atoi64( strBarcodeNo );
	}

	if ( m_nBarcodeNo != nOldBarcodeNo )
	{
		GetEditQty() -> SetWindowText( "1" );
		UpdateEditBoxes( BARCODE_ITEM_BARCODE );
	}

	GetEditQty() -> SetSel ( 0, -1 );
}

/**********************************************************************/

void CListLabelsItemAddDlg::OnKillFocusPlu() 
{
	__int64 nOldPluNo = m_nPluNo;

	{
		CString strPluNo;
		GetEditPluNo() -> GetWindowText( strPluNo );
		BarcodePluNoTable.ConvertPlu( strPluNo );
		m_nPluNo = _atoi64( strPluNo );
	}

	if ( m_nPluNo != nOldPluNo )
	{
		GetEditQty() -> SetWindowText( "1" );
		UpdateEditBoxes( BARCODE_ITEM_PLU );
	}

	GetEditQty() -> SetSel ( 0, -1 );
}

/**********************************************************************/

void CListLabelsItemAddDlg::OnButtonBrowseBarcode()
{
	CBarcodeFilterDlg dlgBrowse ( m_BarcodeFilterArray, this );

	if ( dlgBrowse.DoModal() != IDOK )
		return;

	int nBarcodeIdx = dlgBrowse.GetBarcodeIdx();
	if ( nBarcodeIdx < 0 )
		return;

	CBarcodeCSVRecord BarcodeRecord;
	DataManager.Barcode.GetAt( nBarcodeIdx, BarcodeRecord );
	GetEditBarcodeNo() -> SetWindowText( BarcodeRecord.GetBarcodeNoString() );
	OnKillFocusBarcode();
}

/**********************************************************************/

void CListLabelsItemAddDlg::OnButtonBrowsePlu()
{
	int nDlgResult, nPluIdx;
	::ShowPluFilterDlg( m_PluFilterArray, this, nDlgResult, nPluIdx );
		
	if ( IDOK != nDlgResult )
		return;

	if ( nPluIdx < 0 )
		return;

	CPluCSVRecord PluRecord;
	DataManager.Plu.GetAt( nPluIdx, PluRecord );
	GetEditPluNo() -> SetWindowText( PluRecord.GetPluNoString() );
	OnKillFocusPlu();
}

/**********************************************************************/

void CListLabelsItemAddDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		if ( BARCODE_ITEM_PLU == m_nNewItemMode )
			OnKillFocusPlu();
		else
			OnKillFocusBarcode();

		{
			CString strBarcodeNo;
			GetEditBarcodeNo() -> GetWindowText( strBarcodeNo );
			m_nBarcodeNo = _atoi64( strBarcodeNo );

			CString strPluNo;
			GetEditPluNo() -> GetWindowText( strPluNo );
			m_nPluNo = _atoi64( strPluNo );
		}

		if ( BARCODE_ITEM_PLU == m_nNewItemMode )
		{
			m_nBarcodeNo = m_nPluNo;

			if ( 0 == m_nPluNo )
			{
				Prompter.Error ( "Please enter a Plu number." );
				GetEditPluNo() -> SetFocus();
				GetEditPluNo() -> SetSel( 0, -1 );
				return;
			}
	
			int nPluIdx;
			if ( DataManager.Plu.FindPluByNumber( m_nPluNo, nPluIdx ) == FALSE )
			{
				Prompter.Error ( "Please select a Plu number that\nexists in the current database." );
				GetEditPluNo() -> SetFocus();
				GetEditPluNo() -> SetSel( 0, -1 );
				return;
			}

			if ( SysInfo.IsModifiableSSPluNo( m_nPluNo ) == TRUE )
			{
				CString strPluNo;
				strPluNo.Format( "%I64d", m_nPluNo );
				
				CString strMsg;
				strMsg.Format( "%d digit Plu numbers are configured for use with modifiers\nand cannot be used directly as barcodes.",
					strPluNo.GetLength() );

				Prompter.ContactDealer( strMsg );
				GetEditPluNo() -> SetFocus();
				GetEditPluNo() -> SetSel( 0, -1 );
				return;
			}

			if ( TRUE == m_bProductLabel )
			{
				if ( CBarcodePluNoTable::IsValidEan ( m_nPluNo ) == FALSE )
				{
					Prompter.Error ( "Please select a valid EAN number." );
					GetEditPluNo() -> SetFocus();
					GetEditPluNo() -> SetSel( 0, -1 );
					return;
				}
			}
		}
		else
		{
			if ( 0 == m_nBarcodeNo )
			{
				Prompter.Error ( "Please enter a Barcode number." );
				GetEditBarcodeNo() -> SetFocus();
				GetEditBarcodeNo() -> SetSel( 0, -1 );
				return;
			}
	
			int nBarcodeIdx;
			if ( DataManager.Barcode.FindBarcodeByNumber ( m_nBarcodeNo, nBarcodeIdx ) == FALSE )
			{
				Prompter.Error ( "Please select a Barcode number that\nexists in the current database." );
				GetEditBarcodeNo() -> SetFocus();
				GetEditBarcodeNo() -> SetSel( 0, -1 );
				return;
			}

			if ( TRUE == m_bProductLabel )
			{
				if ( CBarcodePluNoTable::IsValidEan ( m_nBarcodeNo ) == FALSE )
				{
					Prompter.Error ( "Please select a valid EAN number." );
					GetEditBarcodeNo() -> SetFocus();
					GetEditBarcodeNo() -> SetSel( 0, -1 );
					return;
				}
			}

			int nPluIdx;
			if ( DataManager.Plu.FindPluByNumber( m_nPluNo, nPluIdx ) == FALSE )
			{
				Prompter.Error ( "Please select a Barcode number that is\nlinked to an existing plu." );
				GetEditBarcodeNo() -> SetFocus();
				GetEditBarcodeNo() -> SetSel( 0, -1 );
				return;
			}
		}

		SysInfo.SetNewBarcodePluFlag( BARCODE_ITEM_PLU == m_nNewItemMode );
		
		m_LabelListRecord.SetBarcodeNo( m_nBarcodeNo );
		m_LabelListRecord.SetPluNo( m_nPluNo );
		m_LabelListRecord.SetModifier( m_nModifier );
		m_LabelListRecord.SetDirectPluFlag( BARCODE_ITEM_PLU == m_nNewItemMode );
		m_LabelListRecord.SetQtyToPrint( m_nBarcodeQty );
		EndDialog( IDOK );
	}
}

/**********************************************************************/

