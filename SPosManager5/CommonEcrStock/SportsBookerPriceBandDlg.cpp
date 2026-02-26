/**********************************************************************/
 
/**********************************************************************/
#include "DatabaseCSVArray.h"
//#include "DataManager.h"
#include "SportsBookerPriceBandCSVArray.h"
/**********************************************************************/
#include "SportsBookerPriceBandDlg.h"
/**********************************************************************/

CSportsBookerPriceBandDlg::CSportsBookerPriceBandDlg(CWnd* pParent)
	: CSSDialog(CSportsBookerPriceBandDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSportsBookerPriceBandDlg)
	//}}AFX_DATA_INIT
	m_staticBandID[0] = IDC_STATIC_BAND1;
	m_staticBandID[1] = IDC_STATIC_BAND2;
	m_staticBandID[2] = IDC_STATIC_BAND3;
	m_staticBandID[3] = IDC_STATIC_BAND4;
	m_staticBandID[4] = IDC_STATIC_BAND5;
	m_staticBandID[5] = IDC_STATIC_BAND6;
	m_staticBandID[6] = IDC_STATIC_BAND7;
	m_staticBandID[7] = IDC_STATIC_BAND8;
	m_staticBandID[8] = IDC_STATIC_BAND9;
	m_staticBandID[9] = IDC_STATIC_BAND10;
	
	m_comboBandID[0] = IDC_COMBO_BAND1;
	m_comboBandID[1] = IDC_COMBO_BAND2;
	m_comboBandID[2] = IDC_COMBO_BAND3;
	m_comboBandID[3] = IDC_COMBO_BAND4;
	m_comboBandID[4] = IDC_COMBO_BAND5;
	m_comboBandID[5] = IDC_COMBO_BAND6;
	m_comboBandID[6] = IDC_COMBO_BAND7;
	m_comboBandID[7] = IDC_COMBO_BAND8;
	m_comboBandID[8] = IDC_COMBO_BAND9;
	m_comboBandID[9] = IDC_COMBO_BAND10;
	
	m_editTextID[0] = IDC_EDIT_TEXT1;
	m_editTextID[1] = IDC_EDIT_TEXT2;
	m_editTextID[2] = IDC_EDIT_TEXT3;
	m_editTextID[3] = IDC_EDIT_TEXT4;
	m_editTextID[4] = IDC_EDIT_TEXT5;
	m_editTextID[5] = IDC_EDIT_TEXT6;
	m_editTextID[6] = IDC_EDIT_TEXT7;
	m_editTextID[7] = IDC_EDIT_TEXT8;
	m_editTextID[8] = IDC_EDIT_TEXT9;
	m_editTextID[9] = IDC_EDIT_TEXT10;
	m_nTabPage = 0;
}

/**********************************************************************/

void CSportsBookerPriceBandDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSportsBookerPriceBandDlg)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_TAB_BANDS, m_TabBands );

	for ( int i = 0; i < 10; i++ )
	{
		DDX_Control(pDX, m_staticBandID[i], m_staticBand[i] );
		DDX_Control(pDX, m_comboBandID[i], m_comboBand[i] );
		DDX_Control(pDX, m_editTextID[i], m_editText[i] );
	}
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSportsBookerPriceBandDlg, CDialog)
	//{{AFX_MSG_MAP(CSportsBookerPriceBandDlg)
	//}}AFX_MSG_MAP
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_BANDS, OnTcnSelchangeTabs)
	ON_CBN_SELCHANGE( IDC_COMBO_BAND1, OnSelectBand1)
	ON_CBN_SELCHANGE( IDC_COMBO_BAND2, OnSelectBand2)
	ON_CBN_SELCHANGE( IDC_COMBO_BAND3, OnSelectBand3)
	ON_CBN_SELCHANGE( IDC_COMBO_BAND4, OnSelectBand4)
	ON_CBN_SELCHANGE( IDC_COMBO_BAND5, OnSelectBand5)
	ON_CBN_SELCHANGE( IDC_COMBO_BAND6, OnSelectBand6)
	ON_CBN_SELCHANGE( IDC_COMBO_BAND7, OnSelectBand7)
	ON_CBN_SELCHANGE( IDC_COMBO_BAND8, OnSelectBand8)
	ON_CBN_SELCHANGE( IDC_COMBO_BAND9, OnSelectBand9)
	ON_CBN_SELCHANGE( IDC_COMBO_BAND10, OnSelectBand10)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CSportsBookerPriceBandDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();	

	CString strTitle = "Setup Sports Booker Price Bands ";
	strTitle += dbDatabase.GetDatabaseTitle( DataManager.GetActiveDbIdx(), TRUE ); 
	SetWindowText ( strTitle );

	for ( int i = 0; i < 10; i++ )
	{
		m_editText[i].LimitText( MAX_SBPRICETEXT_LEN );
		
		m_comboBand[i].AddString( "-----" );
		for ( int p = 0; p < 10; p++ )
		{
			CString strBand;
			strBand.Format( "%2.2d  %s", p + 1, DataManager.PriceText.GetReportText( p ) );
			m_comboBand[i].AddString( strBand );
		}
	}
	
	m_TabBands.InsertItem( 0, "Bands 1 - 10" );
	m_TabBands.InsertItem( 1, "Bands 11 - 20" );

	m_nTabPage = 0;
	LoadTabSettings();

	return TRUE;  
}

/**********************************************************************/

void CSportsBookerPriceBandDlg::GetTabItemCount( int& nStartIdx, int& nItemsOnPage )
{
	switch( m_nTabPage )
	{
	case 1:
		nStartIdx = 10;
		nItemsOnPage = 10;
		break;

	case 0:
	default:
		nStartIdx = 0;
		nItemsOnPage = 10;
		break;
	}
}

/**********************************************************************/

void CSportsBookerPriceBandDlg::OnTcnSelchangeTabs(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;

	SaveTabChanges();

	m_nTabPage = m_TabBands.GetCurSel();
	if ( ( m_nTabPage < 0 ) || ( m_nTabPage > 1 ) )
		m_nTabPage = 0;

	LoadTabSettings();
}

/**********************************************************************/

void CSportsBookerPriceBandDlg::LoadTabSettings()
{
	int nStartIdx, nItemsOnPage;
	GetTabItemCount( nStartIdx, nItemsOnPage );

	for ( int nRow = 0; nRow < nItemsOnPage; nRow++ )
	{
		CString str;
		str.Format( "Band %d", nStartIdx + nRow + 1 );
		m_staticBand[ nRow ].SetWindowText( str );

		CSportsBookerPriceBandCSVRecord Record;
		DataManager.SBPriceText.GetRecord( nStartIdx + nRow, Record );

		int nSharpPosBand = Record.GetSharpPosBand();
		if ( ( nSharpPosBand < 0 ) || ( nSharpPosBand > 10 ) )
			nSharpPosBand = 0;

		m_comboBand[nRow].SetCurSel( nSharpPosBand );
		UpdateTextField( nRow, Record );
	}
}

/**********************************************************************/
void CSportsBookerPriceBandDlg::OnSelectBand1()	{ SelectSharpPosBand(0); }
void CSportsBookerPriceBandDlg::OnSelectBand2()	{ SelectSharpPosBand(1); }
void CSportsBookerPriceBandDlg::OnSelectBand3()	{ SelectSharpPosBand(2); }
void CSportsBookerPriceBandDlg::OnSelectBand4()	{ SelectSharpPosBand(3); }
void CSportsBookerPriceBandDlg::OnSelectBand5()	{ SelectSharpPosBand(4); }
void CSportsBookerPriceBandDlg::OnSelectBand6()	{ SelectSharpPosBand(5); }
void CSportsBookerPriceBandDlg::OnSelectBand7()	{ SelectSharpPosBand(6); }
void CSportsBookerPriceBandDlg::OnSelectBand8()	{ SelectSharpPosBand(7); }
void CSportsBookerPriceBandDlg::OnSelectBand9()	{ SelectSharpPosBand(8); }
void CSportsBookerPriceBandDlg::OnSelectBand10(){ SelectSharpPosBand(9); }
/**********************************************************************/

void CSportsBookerPriceBandDlg::SelectSharpPosBand( int nRow )
{
	int nStartIdx, nItemsOnPage;
	GetTabItemCount( nStartIdx, nItemsOnPage );

	CSportsBookerPriceBandCSVRecord Record;
	DataManager.SBPriceText.GetRecord( nStartIdx + nRow, Record );

	UpdateTextField( nRow, Record );
}

/**********************************************************************/

void CSportsBookerPriceBandDlg::UpdateTextField( int nRow, CSportsBookerPriceBandCSVRecord Record )
{
	if ( ( nRow >= 0 ) && ( nRow <= 10 ) )
	{
		int nSel = m_comboBand[nRow].GetCurSel();

		if ( 0 == nSel )
		{
			m_editText[nRow].SetWindowText( Record.GetReportText() );
			m_editText[nRow].SetReadOnly( FALSE );
		}
		else
		{
			m_editText[nRow].SetWindowText( DataManager.PriceText.GetReportText( nSel - 1 ) );
			m_editText[nRow].SetReadOnly( TRUE );
		}
	}
}

/**********************************************************************/

void CSportsBookerPriceBandDlg::SaveTabChanges()
{
	int nStartIdx, nItemsOnPage;
	GetTabItemCount( nStartIdx, nItemsOnPage );

	for ( int nRow = 0; nRow < nItemsOnPage; nRow++ )
	{
		int nSel = m_comboBand[ nRow ].GetCurSel();
		if ( ( nSel < 0 ) || ( nSel > 10 ) )
			nSel = 0;

		CSportsBookerPriceBandCSVRecord Record;
		DataManager.SBPriceText.GetRecord( nStartIdx + nRow, Record );

		Record.SetSharpPosBand( nSel );	

		if ( 0 == nSel )
		{	
			CString strText;
			m_editText[ nRow ].GetWindowText( strText );
			Record.SetReportText( strText );
		}

		DataManager.SBPriceText.SetRecord( nStartIdx + nRow, Record );
	}
}

/**********************************************************************/

void CSportsBookerPriceBandDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		SaveTabChanges();
		EndDialog ( IDOK );
	}
}

/**********************************************************************/
