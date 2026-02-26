/**********************************************************************/
#include "OfferGroupHelpers.h"
/**********************************************************************/
#include "PropPageMixMatchBuckets.h"
/**********************************************************************/

CPropPageMixMatchBuckets::CPropPageMixMatchBuckets() : CSSPropertyPage(CPropPageMixMatchBuckets::IDD)
{
	//{{AFX_DATA_INIT(CPropPageMixMatchBuckets)
	m_strDescription = _T("");
	m_bEnable = FALSE;
	m_nBucketA = 0;
	m_nBucketB = 0;
	m_nBucketC = 0;
	m_nBucketD = 0;
	m_nBucketE = 0;
	m_dTrip = 0.0;
	//}}AFX_DATA_INIT
}

/**********************************************************************/

CPropPageMixMatchBuckets::~CPropPageMixMatchBuckets()
{
}

/**********************************************************************/

void CPropPageMixMatchBuckets::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageMixMatchBuckets)
	DDX_Control(pDX, IDC_COMBO_OFFERGROUP5, m_comboOfferGroup5);
	DDX_Control(pDX, IDC_COMBO_OFFERGROUP4, m_comboOfferGroup4);
	DDX_Control(pDX, IDC_COMBO_OFFERGROUP3, m_comboOfferGroup3);
	DDX_Control(pDX, IDC_COMBO_OFFERGROUP2, m_comboOfferGroup2);
	DDX_Control(pDX, IDC_COMBO_OFFERGROUP1, m_comboOfferGroup1);
	DDX_Check(pDX, IDC_CHECK_ENABLE, m_bEnable);
	DDX_Text(pDX, IDC_EDIT_BUCKETA, m_nBucketA);
	DDX_Text(pDX, IDC_EDIT_BUCKETB, m_nBucketB);
	DDX_Text(pDX, IDC_EDIT_BUCKETC, m_nBucketC);
	DDX_Text(pDX, IDC_EDIT_BUCKETD, m_nBucketD);
	DDX_Text(pDX, IDC_EDIT_BUCKETE, m_nBucketE);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_DESCRIPTION, m_strDescription);
	DDV_MaxChars(pDX, m_strDescription, MixMatch::Description.Max);
	DDX_Text(pDX, IDC_EDIT_TRIP, m_dTrip);
	DDV_MinMaxDouble(pDX, m_dTrip, MixMatch::MinSpend.Min, MixMatch::MinSpend.Max);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPageMixMatchBuckets, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPageMixMatchBuckets)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_GROUP1, OnButtonEditGroup1)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_GROUP2, OnButtonEditGroup2)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_GROUP3, OnButtonEditGroup3)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_GROUP4, OnButtonEditGroup4)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_GROUP5, OnButtonEditGroup5)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/
CEdit* CPropPageMixMatchBuckets::GetEditTrip() { return GetEdit( IDC_EDIT_TRIP ); }
CEdit* CPropPageMixMatchBuckets::GetEditBucketA() { return GetEdit( IDC_EDIT_BUCKETA ); }
CEdit* CPropPageMixMatchBuckets::GetEditBucketB() { return GetEdit( IDC_EDIT_BUCKETB ); }
CEdit* CPropPageMixMatchBuckets::GetEditBucketC() { return GetEdit( IDC_EDIT_BUCKETC ); }
CEdit* CPropPageMixMatchBuckets::GetEditBucketD() { return GetEdit( IDC_EDIT_BUCKETD ); }
CEdit* CPropPageMixMatchBuckets::GetEditBucketE() { return GetEdit( IDC_EDIT_BUCKETE ); }
/**********************************************************************/

BOOL CPropPageMixMatchBuckets::OnInitDialog() 
{
	GetRecordData();
	CPropertyPage::OnInitDialog();

	SubclassEdit ( IDC_EDIT_TRIP, SS_NUM_DP, 11, "%.2f" );
	SubclassEdit ( IDC_EDIT_BUCKETA, SS_NUM, 2, "%d" );
	SubclassEdit ( IDC_EDIT_BUCKETB, SS_NUM, 2, "%d" );
	SubclassEdit ( IDC_EDIT_BUCKETC, SS_NUM, 2, "%d" );
	SubclassEdit ( IDC_EDIT_BUCKETD, SS_NUM, 2, "%d" );
	SubclassEdit ( IDC_EDIT_BUCKETE, SS_NUM, 2, "%d" );

	SetRecordControls();
	return TRUE;  
}

/**********************************************************************/

BOOL CPropPageMixMatchBuckets::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPageMixMatchBuckets::OnSetActive() 
{
	GetRecordData();
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	SetRecordControls();
	return bResult;
}

/**********************************************************************/

void CPropPageMixMatchBuckets::GetRecordData()
{
	m_strDescription = m_pMixMatchRecord -> GetDisplayName();
	m_bEnable = m_pMixMatchRecord -> GetEnableFlag();
	m_dTrip = m_pMixMatchRecord -> GetMinSpend();
	m_nBucketA = m_pMixMatchRecord -> GetBucketCount1();
	m_nBucketB = m_pMixMatchRecord -> GetBucketCount2();
	m_nBucketC = m_pMixMatchRecord -> GetBucketCount3();
	m_nBucketD = m_pMixMatchRecord -> GetBucketCount4();
	m_nBucketE = m_pMixMatchRecord -> GetBucketCount5();
}

/**********************************************************************/

void CPropPageMixMatchBuckets::SetRecordControls()
{
	COfferGroupHelpers::FillOfferGroupCombo( m_comboOfferGroup1, m_pMixMatchRecord -> GetOfferGroup1() );
	COfferGroupHelpers::FillOfferGroupCombo( m_comboOfferGroup2, m_pMixMatchRecord -> GetOfferGroup2() );
	COfferGroupHelpers::FillOfferGroupCombo( m_comboOfferGroup3, m_pMixMatchRecord -> GetOfferGroup3() );
	COfferGroupHelpers::FillOfferGroupCombo( m_comboOfferGroup4, m_pMixMatchRecord -> GetOfferGroup4() );
	COfferGroupHelpers::FillOfferGroupCombo( m_comboOfferGroup5, m_pMixMatchRecord -> GetOfferGroup5() );
}

/**********************************************************************/

bool CPropPageMixMatchBuckets::UpdateRecord()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		SaveRecord();
		return TRUE;
	}
	return FALSE;
}

/**********************************************************************/

void CPropPageMixMatchBuckets::SaveRecord()
{
	m_pMixMatchRecord -> SetDescription( m_strDescription );
	m_pMixMatchRecord -> SetEnableFlag ( m_bEnable != 0 );
	m_pMixMatchRecord -> SetMinSpend( m_dTrip );
	m_pMixMatchRecord -> SetBucketCount1( m_nBucketA );
	m_pMixMatchRecord -> SetBucketCount2( m_nBucketB );
	m_pMixMatchRecord -> SetBucketCount3( m_nBucketC );
	m_pMixMatchRecord -> SetBucketCount4( m_nBucketD );
	m_pMixMatchRecord -> SetBucketCount5( m_nBucketE );
	m_pMixMatchRecord -> SetOfferGroup1( COfferGroupHelpers::GetOfferGroup( m_comboOfferGroup1 ) );
	m_pMixMatchRecord -> SetOfferGroup2( COfferGroupHelpers::GetOfferGroup( m_comboOfferGroup2 ) );
	m_pMixMatchRecord -> SetOfferGroup3( COfferGroupHelpers::GetOfferGroup( m_comboOfferGroup3 ) );
	m_pMixMatchRecord -> SetOfferGroup4( COfferGroupHelpers::GetOfferGroup( m_comboOfferGroup4 ) );
	m_pMixMatchRecord -> SetOfferGroup5( COfferGroupHelpers::GetOfferGroup( m_comboOfferGroup5 ) );
}

/**********************************************************************/

void CPropPageMixMatchBuckets::Refresh()
{
	GetRecordData();
	UpdateData ( FALSE );
	SetRecordControls();
}

/**********************************************************************/
void CPropPageMixMatchBuckets::OnButtonEditGroup1() { EditOfferGroup( m_comboOfferGroup1 );}
void CPropPageMixMatchBuckets::OnButtonEditGroup2() { EditOfferGroup( m_comboOfferGroup2 );}
void CPropPageMixMatchBuckets::OnButtonEditGroup3() { EditOfferGroup( m_comboOfferGroup3 );}
void CPropPageMixMatchBuckets::OnButtonEditGroup4() { EditOfferGroup( m_comboOfferGroup4 );}
void CPropPageMixMatchBuckets::OnButtonEditGroup5() { EditOfferGroup( m_comboOfferGroup5 );}
/**********************************************************************/

void CPropPageMixMatchBuckets::EditOfferGroup( CSSComboBox& combo ) 
{
	if ( COfferGroupHelpers::EditOfferGroupInternal( FALSE, combo, this ) == TRUE )
	{
		COfferGroupHelpers::RefillOfferGroupCombo( m_comboOfferGroup1 );
		COfferGroupHelpers::RefillOfferGroupCombo( m_comboOfferGroup2 );
		COfferGroupHelpers::RefillOfferGroupCombo( m_comboOfferGroup3 );
		COfferGroupHelpers::RefillOfferGroupCombo( m_comboOfferGroup4 );
		COfferGroupHelpers::RefillOfferGroupCombo( m_comboOfferGroup5 );
	}
}

/**********************************************************************/
