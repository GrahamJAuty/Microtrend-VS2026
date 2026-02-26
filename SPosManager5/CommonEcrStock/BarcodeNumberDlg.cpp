/**********************************************************************/
#include "BarcodeFilterDlg.h"
#include "BarcodePluNoTable.h"
#include "FilterTypes.h"
#include "..\SPosEcrManager5\PluFilterDlgEcrman.h"
#include "..\SPosStockManager5\PluFilterDlgStockman.h"
#include "BarcodePluNoTable.h"
#include "SysInfo.h"
/**********************************************************************/
#include "BarcodeNumberDlg.h"
/**********************************************************************/

CBarcodeNumberDlg::CBarcodeNumberDlg( int nMode, CBarcodeCSVRecord& BarcodeRecord, CWnd* pParent)
	: CSSDialog(CBarcodeNumberDlg::IDD, pParent), m_BarcodeRecord ( BarcodeRecord ), 
		m_PluFilterArray ( FILTERTYPE_BROWSE_PLU ), m_BarcodeFilterArray ( FILTERTYPE_DATABASE )
{
	//{{AFX_DATA_INIT(CBarcodeNumberDlg)
	m_strBarcodeNo = _T("");
	m_strPluNo = _T("");
	//}}AFX_DATA_INIT
	m_nMode = nMode;
	m_nCurrentPluNo = 0;
	m_nOriginalPluNo = 0;
	m_nOriginalModifier = 0;
}

/**********************************************************************/

void CBarcodeNumberDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBarcodeNumberDlg)
	DDX_Control(pDX, IDC_BUTTON_BROWSE_BARCODE, m_buttonBrowseBarcode);
	DDX_Control(pDX, IDC_CHECK_SHELF, m_checkPrintShelf);
	DDX_Control(pDX, IDC_CHECK_PRODUCT, m_checkPrintProduct);
	DDX_Control(pDX, IDC_EDIT_DESCRIPTION, m_editDescription);
	DDX_Control(pDX, IDC_BUTTON_BROWSE_PLU, m_buttonBrowsePlu);
	DDX_Control(pDX, IDC_STATIC_MODIFIER, m_staticModifier);
	DDX_Control(pDX, IDC_COMBO_MODIFIER, m_comboModifier);
	DDX_IntegerString(pDX, IDC_EDIT_BARCODENO, m_strBarcodeNo);
	DDV_IntegerString(pDX, m_strBarcodeNo, "1", Super.BarcodeEnd() );
	DDX_IntegerString(pDX, IDC_EDIT_PLUNO, m_strPluNo);
	DDV_IntegerString(pDX, m_strPluNo, "1", Super.PluEnd() );
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CBarcodeNumberDlg, CDialog)
	//{{AFX_MSG_MAP(CBarcodeNumberDlg)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_BARCODE, OnButtonBrowseBarcode)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_PLU, OnButtonBrowsePlu)
	ON_EN_KILLFOCUS(IDC_EDIT_BARCODENO, OnKillFocusBarcodeNo)
	ON_EN_KILLFOCUS(IDC_EDIT_PLUNO, OnKillFocusPluNo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/
CEdit* CBarcodeNumberDlg::GetEditPluNo() { return GetEdit( IDC_EDIT_PLUNO ); }
CEdit* CBarcodeNumberDlg::GetEditBarcodeNo() { return GetEdit( IDC_EDIT_BARCODENO ); }
/**********************************************************************/

BOOL CBarcodeNumberDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SubclassEdit ( IDC_EDIT_PLUNO, SS_NUM, Super.MaxPluLen(), "%s" );
	SubclassEdit ( IDC_EDIT_BARCODENO, SS_NUM, Super.MaxBarcodeLen(), "%s" );

	switch ( m_nMode )
	{
	case BARCODE_PLUDB_ADD:
		m_buttonBrowsePlu.ShowWindow( SW_HIDE );
		GetEditPluNo() -> EnableWindow( FALSE );
		SetWindowText ( "Link Barcode to Plu" );
		break;

	case BARCODE_PLUDB_EDIT:
		m_buttonBrowseBarcode.ShowWindow( SW_HIDE );
		m_buttonBrowsePlu.ShowWindow( SW_HIDE );
		GetEditPluNo() -> EnableWindow( FALSE );
		GetEditBarcodeNo() -> SetWindowText ( m_BarcodeRecord.GetBarcodeNoString() );
		GetEditBarcodeNo() -> EnableWindow( FALSE );
		SetWindowText ( "Change Barcode Settings" );
		break;

	case BARCODE_LIST_ADD:
		SetWindowText ( "Add Barcode" );
		m_buttonBrowseBarcode.ShowWindow( SW_HIDE );
		break;

	case BARCODE_LIST_EDIT:
	default:
		SetWindowText ( "Edit Barcode" );
		GetEditBarcodeNo() -> SetWindowText ( m_BarcodeRecord.GetBarcodeNoString() );
		GetEditBarcodeNo() -> EnableWindow( FALSE );
		m_buttonBrowseBarcode.ShowWindow( SW_HIDE );
		break;
	}

	CString strPluNo = m_BarcodeRecord.GetPluNoString();
	UpdatePluDetails( strPluNo );
	UpdateModifierCombo( strPluNo, TRUE );

	m_checkPrintShelf.SetCheck ( m_BarcodeRecord.GetPrintShelfFlag() );
	m_checkPrintProduct.SetCheck ( m_BarcodeRecord.GetPrintProductFlag() );

	if ( Sysset.IsBarmanSystem() == FALSE )
	{
		m_checkPrintShelf.ShowWindow( SW_HIDE );
		m_checkPrintProduct.ShowWindow( SW_HIDE );
	}

	if ( EcrmanOptions.AllowBarcodeModifierLinks() == FALSE )
	{
		m_staticModifier.ShowWindow( SW_HIDE );
		m_comboModifier.SetCurSel(0);
		m_comboModifier.EnableWindow( FALSE );
		m_comboModifier.ShowWindow( SW_HIDE );
	}

	return TRUE;  
}

/**********************************************************************/

void CBarcodeNumberDlg::OnButtonBrowsePlu() 
{	
	int nDlgResult, nPluIdx;
	::ShowPluFilterDlg( m_PluFilterArray, this, nDlgResult, nPluIdx );

	if ( IDOK != nDlgResult )
		return;

	if ( nPluIdx < 0 )
		return;	

	CPluCSVRecord PluRecord;
	DataManager.Plu.GetAt( nPluIdx, PluRecord );
	CString strPluNo = PluRecord.GetPluNoString();
	UpdatePluDetails( strPluNo );
	UpdateModifierCombo( strPluNo, FALSE );
}

/**********************************************************************/

void CBarcodeNumberDlg::OnButtonBrowseBarcode() 
{
	CBarcodeFilterDlg dlgBrowse ( m_BarcodeFilterArray, this );

	if ( dlgBrowse.DoModal() != IDOK )
		return;

	int nBarcodeIdx = dlgBrowse.GetBarcodeIdx();
	if ( nBarcodeIdx < 0 )
		return;

	CBarcodeCSVRecord BarcodeRecord;
	DataManager.Barcode.GetAt( nBarcodeIdx, BarcodeRecord );
	GetEditBarcodeNo() ->SetWindowText( BarcodeRecord.GetBarcodeNoString() );
}

/**********************************************************************/

void CBarcodeNumberDlg::OnKillFocusPluNo() 
{
	CString strPluNo;
	GetEditPluNo() ->GetWindowText( strPluNo );
	strPluNo = BarcodePluNoTable.ConvertPlu( strPluNo );
	UpdatePluDetails( strPluNo );	
	UpdateModifierCombo( strPluNo, FALSE );
}

/**********************************************************************/

void CBarcodeNumberDlg::OnKillFocusBarcodeNo() 
{
	CString strBarcodeNo;
	GetEditBarcodeNo() ->GetWindowText( strBarcodeNo );
	strBarcodeNo = BarcodePluNoTable.ConvertBarcode( strBarcodeNo );
	GetEditBarcodeNo() ->SetWindowText( strBarcodeNo );
}

/**********************************************************************/

void CBarcodeNumberDlg::UpdatePluDetails( CString& strPluNo )
{	
	GetEditPluNo() ->SetWindowText( strPluNo );

	int nPluIdx;
	CPluCSVRecord PluRecord;
	if ( DataManager.Plu.FindPluByNumber( _atoi64 ( strPluNo ), nPluIdx ) )
	{
		DataManager.Plu.GetAt( nPluIdx, PluRecord );
		m_editDescription.SetWindowText ( PluRecord.GetReportText() );
	}
	else
		m_editDescription.SetWindowText ( "Unknown" );
}

/**********************************************************************/

void CBarcodeNumberDlg::UpdateModifierCombo( CString& strPluNo, bool bInitDialog )
{
	if (  EcrmanOptions.AllowBarcodeModifierLinks() == TRUE  )
	{
		int nPluIdx;
		__int64 nPluNo = _atoi64( strPluNo );

		if ( m_nCurrentPluNo != nPluNo )
		{
			m_nCurrentPluNo = nPluNo;

			m_comboModifier.ResetContent();
			m_comboModifier.AddString ( "None" );
			m_comboModifier.SetItemData ( 0, 0 );

			int nNewSel = 0;
		
			if ( TRUE == bInitDialog )
			{
				m_nOriginalPluNo = nPluNo;
				m_nOriginalModifier = m_BarcodeRecord.GetModifier();
			}

			if ( DataManager.Plu.FindPluByNumber( nPluNo, nPluIdx ) == TRUE )
			{
				CPluCSVRecord PluRecord;
				DataManager.Plu.GetAt( nPluIdx, PluRecord );

				for ( int nMod = 1; nMod <= PLU_MODIFIERS; nMod++ )
				{
					bool bShowModifier = FALSE;
					bShowModifier |= PluRecord.GetModifierEnable( nMod );
					bShowModifier |= ( ( nPluNo == m_nOriginalPluNo ) && ( nMod == m_nOriginalModifier ) );

					if ( TRUE == bShowModifier )
					{
						int nPos = m_comboModifier.AddString ( DataManager.Modifier.GetDisplayName( nMod ) );
						m_comboModifier.SetItemData( nPos, nMod );
					}
				}
			}

			int nSel = 0;
			if  ( TRUE == bInitDialog )
			{
				for ( int n = 1; n < m_comboModifier.GetCount(); n++ )
				{	
					if ( m_comboModifier.GetItemData(n) == m_nOriginalModifier )
					{
						nSel = n;
						break;
					}
				}
			}

			m_comboModifier.SetCurSel(nSel);
			m_comboModifier.EnableWindow( m_comboModifier.GetCount() >= 2 );
		}
	}
}

/**********************************************************************/

void CBarcodeNumberDlg::OnOK() 
{
	OnKillFocusBarcodeNo();

	int nModifier = 0;
	int nSel = m_comboModifier.GetCurSel();
	if ( ( EcrmanOptions.AllowBarcodeModifierLinks() == TRUE ) && ( nSel >= 0 ) && ( nSel < m_comboModifier.GetCount() ) )
		nModifier = m_comboModifier.GetItemData(nSel);

	if ( UpdateData ( TRUE ) == TRUE )
	{
		switch ( m_nMode )
		{
		case BARCODE_PLUDB_ADD:
			m_BarcodeRecord.SetBarcodeNo( _atoi64( m_strBarcodeNo ) );
			m_BarcodeRecord.SetPluNo( _atoi64 ( m_strPluNo ) );
			m_BarcodeRecord.SetModifier( nModifier );
			m_BarcodeRecord.SetPrintShelfFlag( m_checkPrintShelf.GetCheck() != 0 );
			m_BarcodeRecord.SetPrintProductFlag( m_checkPrintProduct.GetCheck() != 0 );
			EndDialog( IDOK );
			return;

		case BARCODE_PLUDB_EDIT:
			m_BarcodeRecord.SetModifier( nModifier );
			m_BarcodeRecord.SetPrintShelfFlag( m_checkPrintShelf.GetCheck() != 0 );
			m_BarcodeRecord.SetPrintProductFlag( m_checkPrintProduct.GetCheck() != 0 );
			EndDialog( IDOK );
			return;

		case BARCODE_LIST_ADD:
			{
				int nBarcodeIdx;
				if ( DataManager.Barcode.FindBarcodeByNumber( _atoi64 ( m_strBarcodeNo ), nBarcodeIdx ) )
				{
					Prompter.Error ( "The selected barcode already exists" );
					GetEditBarcodeNo() ->SetFocus();
					GetEditBarcodeNo() ->SetSel ( 0, -1 );
					return;
				}

				int nPluIdx;
				if ( DataManager.Plu.FindPluByNumber( _atoi64 ( m_strPluNo ), nPluIdx ) == FALSE )
				{
					Prompter.Error ( "The selected plu number does not exist" );
					GetEditPluNo() ->SetFocus();
					GetEditPluNo() ->SetSel ( 0, -1 );
					return;
				}

				m_BarcodeRecord.SetBarcodeNo( _atoi64 ( m_strBarcodeNo ) );
				m_BarcodeRecord.SetPluNo( _atoi64 (  m_strPluNo ) );
				m_BarcodeRecord.SetModifier( nModifier );
				m_BarcodeRecord.SetPrintShelfFlag( m_checkPrintShelf.GetCheck() != 0 );
				m_BarcodeRecord.SetPrintProductFlag( m_checkPrintProduct.GetCheck() != 0 );
	
				EndDialog( IDOK );
				return;
			}
			break;
	
		case BARCODE_LIST_EDIT:
			{
				int nPluIdx;
				if ( DataManager.Plu.FindPluByNumber( _atoi64 ( m_strPluNo ), nPluIdx ) == FALSE )
				{
					Prompter.Error ( "The selected plu number does not exist" );
					GetEditPluNo() ->SetFocus();
					GetEditPluNo() ->SetSel ( 0, -1 );
					return;
				}

				m_BarcodeRecord.SetPluNo( _atoi64( m_strPluNo ) );
				m_BarcodeRecord.SetModifier( nModifier );
				m_BarcodeRecord.SetPrintShelfFlag( m_checkPrintShelf.GetCheck() != 0 );
				m_BarcodeRecord.SetPrintProductFlag( m_checkPrintProduct.GetCheck() != 0 );
				
				EndDialog( IDOK );
				return;
			}
			break;
		}
	}
}

/**********************************************************************/
