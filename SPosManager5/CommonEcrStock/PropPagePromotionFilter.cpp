/**********************************************************************/
#include "HexBitArray.h"
#include "OfferGroupHelpers.h"
/**********************************************************************/
#include "PropPagePromotionFilter.h"
/**********************************************************************/

CPropPagePromotionFilter::CPropPagePromotionFilter() : CSSPropertyPage(CPropPagePromotionFilter::IDD), m_HexBitArray(100)
{
	//{{AFX_DATA_INIT(CPropPagePromotionFilter)
	//}}AFX_DATA_INIT
	m_bFiltered = FALSE;
	m_bUpdatingTabs = FALSE;
}

/**********************************************************************/

CPropPagePromotionFilter::~CPropPagePromotionFilter()
{
}

/**********************************************************************/

void CPropPagePromotionFilter::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPagePromotionFilter)
	DDX_Control(pDX, IDC_EDIT_DESCRIPTION, m_editDescription);
	DDX_Control(pDX, IDC_COMBO_FILTERTYPE, m_comboFilterType);
	DDX_Control(pDX, IDC_STATIC_FILTER, m_staticFilter);
	DDX_Control(pDX, IDC_COMBO_FILTERGROUP, m_comboFilterGroup);
	DDX_Control(pDX, IDC_BUTTON_FILTERGROUP, m_buttonFilterGroup);
	DDX_Control(pDX, IDC_BUTTON_ALL, m_buttonAll);
	DDX_Control(pDX, IDC_BUTTON_NONE, m_buttonNone);
	DDX_Control(pDX, IDC_BUTTON_FILTER, m_buttonFilter);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPagePromotionFilter, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPagePromotionFilter)
	ON_WM_MEASUREITEM()
	ON_CBN_SELCHANGE(IDC_COMBO_FILTERTYPE, OnSelectFilterType)
	ON_BN_CLICKED(IDC_BUTTON_FILTERGROUP, OnButtonFilterGroup)
	ON_BN_CLICKED(IDC_BUTTON_ALL, OnButtonAll)
	ON_BN_CLICKED(IDC_BUTTON_NONE, OnButtonNone)
	ON_BN_CLICKED(IDC_BUTTON_FILTER, OnButtonFilter)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPagePromotionFilter::OnInitDialog() 
{
	GetRecordData();
	CPropertyPage::OnInitDialog();

	m_editDescription.LimitText( Promotions::Description.Max );

	m_comboFilterType.AddString( "None" );
	m_comboFilterType.AddString( "Analysis" );
	m_comboFilterType.AddString( "Offer Group" );

	COfferGroupHelpers::FillOfferGroupCombo( m_comboFilterGroup, m_pPromoRecord -> GetFilterOfferGroup() );

	m_listCategory.SubclassDlgItem ( IDC_LIST_ANALYSIS, this );
	m_listCategory.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_listCategory.InsertColumn ( 0, "", LVCFMT_LEFT, 220 );
	m_listCategory.SetLockItemZeroFlag( FALSE );

	return TRUE;  
}

/**********************************************************************/

void CPropPagePromotionFilter::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
   if ( nIDCtl == IDC_LIST_ANALYSIS )
   {
	   m_listCategory.AdjustRowHeight( lpMeasureItemStruct, 10 );
   }
}

/**********************************************************************/

void CPropPagePromotionFilter::FillAnalysisList()
{
	m_listCategory.ClearList();

	for ( int nCat = 1; nCat <= 99; nCat++ )
	{
		bool bSelected = m_HexBitArray.GetBit( nCat );

		if ( ( TRUE == bSelected ) || ( FALSE == m_bFiltered ) )
		{
			CSSListTaggedSelectItem item;
			item.m_strText = DataManager.AnalysisCategory.GetDisplayText(nCat);
			item.m_nData = nCat;
			item.SetTagState( bSelected ? 1 : 0);
			m_listCategory.AddItemToList( item );
		}
	}
}

/**********************************************************************/

BOOL CPropPagePromotionFilter::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPagePromotionFilter::OnSetActive() 
{
	GetRecordData();
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	SetRecordControls();
	OnSelectFilterType();
	return bResult;
}

/**********************************************************************/

void CPropPagePromotionFilter::GetRecordData()
{
	CString strFilter = m_pPromoRecord -> GetFilterAnalysis();
	m_HexBitArray.TranslateFromHexString( strFilter );
}

/**********************************************************************/

void CPropPagePromotionFilter::UpdateHexBitArray()
{
	m_HexBitArray.Clear();
	for ( int n = 0; n < m_listCategory.GetItemCount(); n++ )
	{
		CSSListTaggedSelectItem item;
		m_listCategory.GetListItem( n, item );
			
		if ( item.GetTagState() == 1 )
		{
			int nBit = item.m_nData;
			m_HexBitArray.SetBit( nBit, TRUE );
		}
	}
}

/**********************************************************************/

void CPropPagePromotionFilter::OnSelectFilterType() 
{
	int nType = m_comboFilterType.GetCurSel();
	
	switch( nType )
	{
	case 1:
		m_staticFilter.SetWindowText( "Filter analysis" );
		m_staticFilter.ShowWindow( SW_SHOW );
		break;

	case 2:
		m_staticFilter.SetWindowText( "Filter offer group" );
		m_staticFilter.ShowWindow( SW_SHOW );
		break;

	default:
		m_staticFilter.ShowWindow( SW_HIDE );
		break;
	}
	
	ShowAndEnableWindow( &m_listCategory, ( 1 == nType ) );
	ShowAndEnableWindow( &m_buttonAll, ( 1 == nType ) );
	ShowAndEnableWindow( &m_buttonNone, ( 1 == nType ) );
	ShowAndEnableWindow( &m_buttonFilter, ( 1 == nType ) );
	ShowAndEnableWindow( &m_comboFilterGroup, ( 2 == nType ) );
	ShowAndEnableWindow( &m_buttonFilterGroup, ( 2 == nType ) );
}

/**********************************************************************/

void CPropPagePromotionFilter::OnButtonFilterGroup()
{
	if (COfferGroupHelpers::EditOfferGroupInternal(FALSE, m_comboFilterGroup, this) == TRUE)
	{
		COfferGroupHelpers::RefillOfferGroupCombo(m_comboFilterGroup);
	}
}

/**********************************************************************/

void CPropPagePromotionFilter::OnButtonAll()
{
	m_HexBitArray.Clear();
	
	for ( int n = 1; n <= MAX_ACAT_NO; n++ )
		m_HexBitArray.SetBit( n, TRUE );

	FillAnalysisList();
}

/**********************************************************************/

void CPropPagePromotionFilter::OnButtonNone()
{
	m_HexBitArray.Clear();
	FillAnalysisList();
}

/**********************************************************************/

void CPropPagePromotionFilter::OnButtonFilter()
{
	if ( TRUE == m_bFiltered )
	{
		m_bFiltered = FALSE;
		m_buttonFilter.SetWindowText( "Show\nSelected" );
	}
	else
	{
		m_bFiltered = TRUE;
		m_buttonFilter.SetWindowText( "Show\nAll" );
	}

	UpdateHexBitArray();
	FillAnalysisList();
}

/**********************************************************************/

void CPropPagePromotionFilter::SetRecordControls()
{
	m_editDescription.SetWindowText( m_pPromoRecord -> GetDisplayName() );

	int nType = m_pPromoRecord -> GetFilterType();
	if ( ( nType >= 0 ) && ( nType <= 2 ) )
		m_comboFilterType.SetCurSel( nType );
	else
		m_comboFilterType.SetCurSel( 0 );

	COfferGroupHelpers::FillOfferGroupCombo( m_comboFilterGroup, m_pPromoRecord -> GetFilterOfferGroup() );
	FillAnalysisList();
	
	OnSelectFilterType();

	m_editDescription.SetSel ( 0, -1 );
	m_editDescription.SetFocus();
}

/**********************************************************************/

bool CPropPagePromotionFilter::UpdateRecord()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		SaveRecord();
		return TRUE;
	}
	return FALSE;
}

/**********************************************************************/

void CPropPagePromotionFilter::SaveRecord()
{
	if ( FALSE == m_bUpdatingTabs )
	{
		CString strDescription;
		m_editDescription.GetWindowText( strDescription );
		
		m_pPromoRecord -> SetDescription( strDescription );
		
		m_pPromoRecord -> SetFilterType( m_comboFilterType.GetCurSel() );
		m_pPromoRecord -> SetFilterOfferGroup( COfferGroupHelpers::GetOfferGroup( m_comboFilterGroup ) );

		UpdateHexBitArray();

		CString strFilter;
		m_HexBitArray.TranslateToHexString( strFilter );
		m_pPromoRecord ->SetFilterAnalysis( strFilter );
	}
}

/**********************************************************************/

void CPropPagePromotionFilter::Refresh()
{
	GetRecordData();
	UpdateData ( FALSE );
	SetRecordControls();
}

/**********************************************************************/