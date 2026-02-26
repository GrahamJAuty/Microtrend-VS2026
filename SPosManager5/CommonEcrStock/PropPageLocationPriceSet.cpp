/**********************************************************************/
#include "PluCSVArray.h"
#include "PluDatabaseHeader.h"
#include "PriceSetItemManager.h"
#include "PropPageLocationPriceSet.h"
#include "SysInfo.h"
/**********************************************************************/

CPropPageLocationPriceSet::CPropPageLocationPriceSet() : CSSPropertyPage(CPropPageLocationPriceSet::IDD)
{
	//{{AFX_DATA_INIT(CPropPageLocationPriceSet)
	//}}AFX_DATA_INIT
	m_nLocIdx = 0;
	m_pLocationBuffer = NULL;
	m_pPriceSetHeaderManager = NULL;
}

/**********************************************************************/

CPropPageLocationPriceSet::~CPropPageLocationPriceSet()
{
}

/**********************************************************************/

void CPropPageLocationPriceSet::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageLocationPriceSet)
	DDX_Control(pDX, IDC_COMBO_PRICESET3, m_comboDbPriceSet3);
	DDX_Control(pDX, IDC_COMBO_PRICESET2, m_comboDbPriceSet2);
	DDX_Control(pDX, IDC_COMBO_PRICESET1, m_comboDbPriceSet1);
	DDX_Control(pDX, IDC_COMBO_KEYBOARD3, m_comboKeyboard3);
	DDX_Control(pDX, IDC_COMBO_KEYBOARD2, m_comboKeyboard2);
	DDX_Control(pDX, IDC_COMBO_KEYBOARD1, m_comboKeyboard1);
	DDX_Control(pDX, IDC_COMBO_LOCALPLU, m_comboLocalPlu);
	DDX_Control(pDX, IDC_COMBO_DEFPRICESET, m_comboDefPriceSet);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPageLocationPriceSet, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPageLocationPriceSet)
	ON_WM_MEASUREITEM()
	ON_CBN_SELCHANGE(IDC_COMBO_LOCALPLU, OnSelectPriceSetCount)
	ON_CBN_SELCHANGE(IDC_COMBO_PRICESET1, SetLocalPluEnables)
	ON_CBN_SELCHANGE(IDC_COMBO_PRICESET2, SetLocalPluEnables)
	ON_CBN_SELCHANGE(IDC_COMBO_PRICESET3, SetLocalPluEnables)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPageLocationPriceSet::OnInitDialog() 
{
	GetRecordData();
	CPropertyPage::OnInitDialog();
	SetLocalPluControls();
	SetLocalPluEnables();
	SelectPriceSetFields();
	return TRUE;  
}

/**********************************************************************/

void CPropPageLocationPriceSet::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
   if ( nIDCtl == IDC_LIST )
   {
	   m_listFields.AdjustRowHeight( lpMeasureItemStruct, 10 );
   }
}

/**********************************************************************/

BOOL CPropPageLocationPriceSet::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPageLocationPriceSet::OnSetActive() 
{
	GetRecordData();
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	SetLocalPluEnables(); //DISABLE PRICE SETS FOR COINLOADER
	return bResult;
}

/**********************************************************************/

void CPropPageLocationPriceSet::GetRecordData()
{
}

/**********************************************************************/

void CPropPageLocationPriceSet::SetLocalPluControls()
{
	for ( int nSet = 1; nSet <= MAX_PRICE_SETS; nSet++ )
	{
		CString strTemp;
		strTemp.Format ( "%d", nSet );
		m_comboLocalPlu.AddString ( strTemp );
	}

	int nPriceSets = m_pLocationBuffer -> GetPluPriceSetCount( 0 );
	if ( nPriceSets < 1 || nPriceSets > MAX_PRICE_SETS ) nPriceSets = 1;
	m_comboLocalPlu.SetCurSel ( nPriceSets - 1 );

	for ( int nSet = 1; nSet <= nPriceSets; nSet++ )
	{
		CString strTemp;
		strTemp.Format ( "%d", nSet );
		m_comboDefPriceSet.AddString ( strTemp );
	}

	int nPriceSet = m_pLocationBuffer -> GetDefaultPriceSet( 0 );
	if ( nPriceSet < 1 || nPriceSet > nPriceSets ) nPriceSet = 1;
	m_comboDefPriceSet.SetCurSel ( nPriceSet - 1 );
	m_comboDefPriceSet.EnableWindow ( nPriceSets > 1 );
		
	int nPos = m_pLocationBuffer -> GetPluPriceSetCount( 0 ) - 1;
	if ( nPos < 0 || nPos >= MAX_PRICE_SETS ) nPos = 0;
	m_comboLocalPlu.SetCurSel(nPos);

	m_listFields.SubclassDlgItem ( IDC_LIST, this );
	m_listFields.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_listFields.InsertColumn ( 0, "Fields", LVCFMT_LEFT, 250 );
	m_listFields.SetLockItemZeroFlag( TRUE );

	CCSV csvHeader( PluDatabaseHeader.GetPriceSetHeader() );
	for ( int n = 0; n < csvHeader.GetSize(); n++ )
	{
		CSSListTaggedSelectItem item;
		item.m_strText = csvHeader.GetString(n);
		item.m_nData = 0;
		item.SetTagState(0);
		m_listFields.AddItemToList( item );
	}

	//FILL PRICE SET COMBOS
	LoadPriceSetCombo( m_comboDbPriceSet1, m_pLocationBuffer -> GetDbPriceSet1(0) );
	LoadPriceSetCombo( m_comboDbPriceSet2, m_pLocationBuffer -> GetDbPriceSet2(0) );
	LoadPriceSetCombo( m_comboDbPriceSet3, m_pLocationBuffer -> GetDbPriceSet3(0) );

	//FILL KEYBOARD LINK COMBOS
	LoadKeyboardCombo( m_comboKeyboard1, m_pLocationBuffer -> GetPriceSetKeyboard1(0) );
	LoadKeyboardCombo( m_comboKeyboard2, m_pLocationBuffer -> GetPriceSetKeyboard2(0) );
	LoadKeyboardCombo( m_comboKeyboard3, m_pLocationBuffer -> GetPriceSetKeyboard3(0) );
}

/**********************************************************************/

void CPropPageLocationPriceSet::LoadPriceSetCombo( CSSComboBox& combo, int nType )
{
	combo.AddString( "Location specific" );
	
	for ( int n = 1; n <= 20; n++ )
	{
		CString strLine;
		strLine.Format ( "DB Price Set %d", n );
		combo.AddString( strLine );
	}

	combo.SetCurSel( nType );
}

/**********************************************************************/

void CPropPageLocationPriceSet::LoadKeyboardCombo( CSSComboBox& combo, int nKbd )
{
	combo.AddString( "None" );

	int nMaxKbd = EcrmanOptions.GetFeaturesKeyboardsPerLocation();

	for ( int n = 1; n <= nMaxKbd; n++ )
	{
		CString strLine;
		strLine.Format ( "Keyboard %d", n );
		combo.AddString( strLine );
	}

	if ( nKbd < 1 ) 
		nKbd = 0;
		
	if ( nKbd > nMaxKbd )
	{
		CString str;
		str.Format( "Keyboard %d", nKbd );
		combo.AddString( str );
		combo.SetCurSel( nMaxKbd + 1 );
	}
	else
		combo.SetCurSel( nKbd );
}

/**********************************************************************/

void CPropPageLocationPriceSet::SetLocalPluEnables()
{
	int nPriceSetCount = m_comboLocalPlu.GetCurSel();
	int nPriceSetType1 = m_comboDbPriceSet1.GetCurSel();
	int nPriceSetType2 = m_comboDbPriceSet2.GetCurSel();
	int nPriceSetType3 = m_comboDbPriceSet3.GetCurSel();

	m_comboLocalPlu.EnableWindow( TRUE );
	m_comboDefPriceSet.EnableWindow( nPriceSetCount > 0 );
	m_listFields.EnableWindow( TRUE );
	m_comboDbPriceSet1.EnableWindow( TRUE );
	m_comboDbPriceSet2.EnableWindow( nPriceSetCount > 0 );
	m_comboDbPriceSet3.EnableWindow( nPriceSetCount > 1 );
	m_comboKeyboard1.EnableWindow( 0 == nPriceSetType1 );
	m_comboKeyboard2.EnableWindow( ( nPriceSetCount > 0 ) && ( 0 == nPriceSetType2 ) );
	m_comboKeyboard3.EnableWindow( ( nPriceSetCount > 1 ) && ( 0 == nPriceSetType3 ) );

	if ( nPriceSetCount < 1 ) 
	{
		m_comboDbPriceSet2.SetCurSel(0);
		m_comboKeyboard2.SetCurSel(0);
	}

	if ( nPriceSetCount < 2 ) 
	{
		m_comboDbPriceSet3.SetCurSel(0);
		m_comboKeyboard3.SetCurSel(0);
	}

	if ( EcrmanOptions.GetFeaturesKeyboardFlag() == FALSE )
	{
		m_comboKeyboard1.EnableWindow( FALSE );
		m_comboKeyboard2.EnableWindow( FALSE );
		m_comboKeyboard3.EnableWindow( FALSE );
	}
}

/**********************************************************************/

void CPropPageLocationPriceSet::SelectPriceSetFields()
{
	//PLU NO IS ALREADY SELECTED
	m_listFields.SetItemTagState( 0, 1 );
	
	CCSV csvIn ( PluDatabaseHeader.GetUpdatedHeader( m_pPriceSetHeaderManager -> GetNewHeader() ) );

	CReportConsolidationArray<CSortedStringItem> arrayTicked;
	for ( int n = 0; n < csvIn.GetSize(); n++ )
	{
		CSortedStringItem item;
		item.m_strItem = csvIn.GetString(n);
		arrayTicked.Consolidate( item );
	}

	for ( int n = 0; n < m_listFields.GetItemCount(); n++ )
	{
		CSSListTaggedSelectItem ListItem;
		m_listFields.GetListItem( n, ListItem );

		CSortedStringItem headerItem;
		headerItem.m_strItem = ListItem.m_strText;
		
		int nIdx;
		if ( arrayTicked.Find( headerItem, nIdx ) == TRUE )
		{
			m_listFields.SetItemTagState( n, 1 );
		}
	}
}

/**********************************************************************/

void CPropPageLocationPriceSet::OnSelectPriceSetCount() 
{
	int nOldDefPriceSet = m_comboDefPriceSet.GetCurSel() + 1;
	int nPriceSetCount = m_comboLocalPlu.GetCurSel() + 1;

	m_comboDefPriceSet.ResetContent();
	for ( int nSet = 1; nSet <= nPriceSetCount; nSet++ )
	{
		CString strTemp;
		strTemp.Format ( "%d", nSet );
		m_comboDefPriceSet.AddString ( strTemp );
	}

	if ( nOldDefPriceSet > nPriceSetCount ) nOldDefPriceSet = 1;
	m_comboDefPriceSet.SetCurSel ( nOldDefPriceSet - 1 );
	
	SetLocalPluEnables();
}

/**********************************************************************/

void CPropPageLocationPriceSet::SaveRecord()
{
	if ( UpdateData ( TRUE, TRUE ) == FALSE )
		return;

	m_pLocationBuffer -> SetPluPriceSetCount( 0, m_comboLocalPlu.GetCurSel() + 1 );
	m_pLocationBuffer -> SetDefaultPriceSet( 0, m_comboDefPriceSet.GetCurSel() + 1 );
	m_pLocationBuffer -> SetDbPriceSet1( 0, m_comboDbPriceSet1.GetCurSel() );
	m_pLocationBuffer -> SetDbPriceSet2( 0, m_comboDbPriceSet2.GetCurSel() );
	m_pLocationBuffer -> SetDbPriceSet3( 0, m_comboDbPriceSet3.GetCurSel() );
	
	int nMaxKbd = EcrmanOptions.GetFeaturesKeyboardsPerLocation();

	int nSel1 = m_comboKeyboard1.GetCurSel();
	if ( nSel1 <= nMaxKbd ) m_pLocationBuffer -> SetPriceSetKeyboard1( 0, nSel1 );
	
	int nSel2 = m_comboKeyboard2.GetCurSel();
	if ( nSel2 <= nMaxKbd ) m_pLocationBuffer -> SetPriceSetKeyboard2( 0, nSel2 );
	
	int nSel3 = m_comboKeyboard3.GetCurSel();
	if ( nSel3 <= nMaxKbd ) m_pLocationBuffer -> SetPriceSetKeyboard3( 0, nSel3 );
		
	CCSV csvOut;
	
	for ( int nIndex = 0; nIndex < m_listFields.GetItemCount(); nIndex++ )
	{
		CSSListTaggedSelectItem ListItem;
		m_listFields.GetListItem( nIndex, ListItem );
		
		if ( ListItem.GetTagState() == 1 )
		{
			csvOut.Add( ListItem.m_strText );
		}
	}
	
	m_pPriceSetHeaderManager -> SetNewHeader ( csvOut.GetLine() );
}

/**********************************************************************/








