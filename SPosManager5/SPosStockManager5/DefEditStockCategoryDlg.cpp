/**********************************************************************/
#include "DefEditStockCategoryDlg.h"
/**********************************************************************/

CDefEditStockCategoryDlg::CDefEditStockCategoryDlg( int nMode, int nIndex, CListDataDlgChangeFlag& WarnCancelAfterChange, CWnd* pParent)
	: CSSDialog(CDefEditStockCategoryDlg::IDD, pParent), m_WarnCancelAfterChange( WarnCancelAfterChange )
{
	m_pListDataDlg = NULL;

	switch( nMode )
	{
	case DEFEDIT_STOCKCAT_CATEGORY:
	case DEFEDIT_STOCKCAT_SYSTEM:
	case DEFEDIT_STOCKCAT_ALLOWANCE:
		m_nMode = nMode;
		break;

	default:
		m_nMode = DEFEDIT_STOCKCAT_CATEGORY;
		break;
	}

	m_nIndex = nIndex;
	m_nMaxIndex = ( DEFEDIT_STOCKCAT_ALLOWANCE == m_nMode ) ? MAX_ALLOWANCES : MAX_CATEGORY;
}

/**********************************************************************/

void CDefEditStockCategoryDlg::DoDataExchange(CDataExchange* pDX)
{
	int nMaxLen = 32;
	if ( nMaxLen < Dept::RepText.Max ) nMaxLen = Dept::RepText.Max;
	
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDefEditStockCategoryDlg)
	DDX_Control(pDX, IDC_CHECK_ENABLE, m_checkEnable);
	DDX_Control(pDX, IDC_EDIT_CATEGORY, m_editText);
	DDX_Control(pDX, IDC_STATIC_TAXBAND, m_staticTaxBand);
	DDX_Control(pDX, IDC_COMBO_TAXBAND, m_comboTaxBand);
	DDX_Control(pDX, IDC_DEFAULT_BUTTON, m_buttonDefault);
	DDX_Control(pDX, IDC_SPIN, m_buttonSpin);
	DDX_Control(pDX, IDOK, m_buttonOK);
	DDX_Control(pDX, IDCANCEL, m_buttonCancel);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CDefEditStockCategoryDlg, CDialog)
	//{{AFX_MSG_MAP(CDefEditCategoryDlg)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN, OnSpinButton)
	ON_BN_CLICKED(IDC_DEFAULT_BUTTON, OnDefaultButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CDefEditStockCategoryDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	int nMaxLen = 32;
	if ( nMaxLen < Dept::RepText.Max ) nMaxLen = Dept::RepText.Max;
	m_editText.LimitText( nMaxLen );

	if ( DEFEDIT_STOCKCAT_SYSTEM == m_nMode )
	{
		ShowAndEnableWindow( &m_staticTaxBand, FALSE );
		ShowAndEnableWindow( &m_comboTaxBand, FALSE );
		MoveControl( &m_checkEnable, 56, 26 );
		MoveControl( &m_buttonDefault, 94,45 );
		MoveControl( &m_buttonSpin, 7, 45 );
		MoveControl( &m_buttonOK, 239, 29 );
		MoveControl( &m_buttonCancel, 239, 45 );
		ResizeDialog( 296, 66 );
	}
	else
	{
		m_staticTaxBand.SetWindowText( EcrmanOptions.GetTaxBandString() );

		CArray<int, int> arrayStops;
		arrayStops.Add(5);
		arrayStops.Add(30);
		arrayStops.Add(130);
		m_comboTaxBand.SetTabStops( arrayStops );

		m_comboTaxBand.AddItem( ",None, 0.0000" );

		CString strLetter = "A";
		for ( int nTaxBand = 1; nTaxBand <= MAX_TAX_BANDS; nTaxBand++ )
		{
			CTaxRateInfo TaxRateInfo;
			DataManager.TaxRates.GetTaxRateInfo( nTaxBand, TaxRateInfo );

			strLetter.SetAt( 0, 'A' + nTaxBand - 1 );

			CString strRate;
			strRate.Format ( "%.4lf", TaxRateInfo.m_dTaxRate );
			
			CCSV csv;
			csv.Add( strLetter );
			csv.Add( TaxRateInfo.m_strReportText );
			csv.Add( strRate );
			m_comboTaxBand.AddItem( csv.GetLine() );
		}
	}

	GetData();
	return TRUE;  
}

/**********************************************************************/

void CDefEditStockCategoryDlg::OnSpinButton(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	
	if ( pNMUpDown -> iDelta > 0 )
		SpinPrevious();
	else
		SpinNext();
	
	*pResult = 0;
}

/**********************************************************************/

void CDefEditStockCategoryDlg::OnDefaultButton() 
{
	SpinNext();
}

/**********************************************************************/

void CDefEditStockCategoryDlg::SpinPrevious()
{
	if ( m_nIndex > 1 )
	{
		SaveData();
		m_nIndex--;
		GetData();

		if ( m_pListDataDlg != NULL )
			m_pListDataDlg -> SelectLine ( m_nIndex - 1 );
	}
}

/**********************************************************************/

void CDefEditStockCategoryDlg::SpinNext()
{
	if ( m_nIndex < m_nMaxIndex )
	{
		SaveData();
		m_nIndex++;
		GetData();

		if ( m_pListDataDlg != NULL )
			m_pListDataDlg -> SelectLine ( m_nIndex - 1 );
	}
}

/**********************************************************************/

void CDefEditStockCategoryDlg::GetData()
{
	CString strTitle;

	switch( m_nMode )
	{
	case DEFEDIT_STOCKCAT_SYSTEM:
		m_checkEnable.SetCheck ( DataManagerNonDb.SystemCategory.GetReportEnable ( m_nIndex ) );
		m_editText.SetWindowText ( DataManagerNonDb.SystemCategory.GetText ( m_nIndex ) );
		strTitle.Format ( "C%2.2d %s", m_nIndex, DataManagerNonDb.SystemCategory.GetDefaultText ( m_nIndex ) );
		break;

	case DEFEDIT_STOCKCAT_ALLOWANCE:
		m_comboTaxBand.SetCurSel ( DataManager.Allowance.GetTaxBand ( m_nIndex ) );
		m_checkEnable.SetCheck ( DataManager.Allowance.GetReportEnable ( m_nIndex ) );
		m_editText.SetWindowText ( DataManager.Allowance.GetText ( m_nIndex ) );
		strTitle = DataManager.Allowance.GetDefaultText ( m_nIndex );
		break;

	case DEFEDIT_STOCKCAT_CATEGORY:
	default:
		m_comboTaxBand.SetCurSel ( DataManager.Category.GetTaxBand ( m_nIndex ) );
		m_checkEnable.SetCheck ( DataManager.Category.GetReportEnable ( m_nIndex ) );
		m_editText.SetWindowText ( DataManager.Category.GetText ( m_nIndex ) );
		strTitle.Format ( "C%2.2d %s", m_nIndex, DataManager.Category.GetDefaultText ( m_nIndex ) );
		break;
	}

	SetWindowText ( strTitle );
	m_editText.SetSel ( 0, -1 );
}

/**********************************************************************/

void CDefEditStockCategoryDlg::SaveData()
{
	m_WarnCancelAfterChange.SetFlag();

	int nTaxBand = m_comboTaxBand.GetCurSel();
	bool bEnable = ( m_checkEnable.GetCheck() != 0 );

	CString strText;
	m_editText.GetWindowText( strText );

	switch( m_nMode )
	{
	case DEFEDIT_STOCKCAT_SYSTEM:
		DataManagerNonDb.SystemCategory.SetCategory ( m_nIndex, strText, 0, bEnable );
		break;

	case DEFEDIT_STOCKCAT_ALLOWANCE:
		DataManager.Allowance.SetAllowance ( m_nIndex, strText, nTaxBand, bEnable );
		break;

	case DEFEDIT_STOCKCAT_CATEGORY:
	default:
		DataManager.Category.SetCategory ( m_nIndex, strText, nTaxBand, bEnable );
		break;
	}
}

/**********************************************************************/

void CDefEditStockCategoryDlg::OnOK() 
{
	SaveData();
	EndDialog ( IDOK );
}

/**********************************************************************/
