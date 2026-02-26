/**********************************************************************/
#include "math.h"
/**********************************************************************/
#include "PropPagePromotionFlexValue.h"
/**********************************************************************/

CPropPagePromotionFlexValue::CPropPagePromotionFlexValue() : CSSPropertyPage(CPropPagePromotionFlexValue::IDD)
{
	//{{AFX_DATA_INIT(CPropPagePromotionFlexValue)
	//}}AFX_DATA_INIT

	m_IDFrom[0] = 0;
	m_IDFrom[1] = IDC_EDIT_FROM1;
	m_IDFrom[2] = IDC_EDIT_FROM2;
	m_IDFrom[3] = IDC_EDIT_FROM3;
	m_IDFrom[4] = IDC_EDIT_FROM4;

	m_IDTo[0] = IDC_EDIT_TO0;
	m_IDTo[1] = IDC_EDIT_TO1;
	m_IDTo[2] = IDC_EDIT_TO2;
	m_IDTo[3] = IDC_EDIT_TO3;
	m_IDTo[4] = 0;
	
	m_IDPercent[0] = IDC_EDIT_PERCENT0;
	m_IDPercent[1] = IDC_EDIT_PERCENT1;
	m_IDPercent[2] = IDC_EDIT_PERCENT2;
	m_IDPercent[3] = IDC_EDIT_PERCENT3;
	m_IDPercent[4] = IDC_EDIT_PERCENT4;

	m_bUpdatingTabs = FALSE;
}

/**********************************************************************/

CPropPagePromotionFlexValue::~CPropPagePromotionFlexValue()
{
}

/**********************************************************************/

void CPropPagePromotionFlexValue::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPagePromotionFlexValue)
	DDX_Control(pDX, IDC_EDIT_DESCRIPTION, m_editDescription);
	DDX_Control(pDX, IDC_STATIC_DISCOUNT, m_staticDiscount);
	DDX_Control(pDX, IDC_EDIT_FROM0, m_editFrom0);
	DDX_Control(pDX, IDC_EDIT_TO4, m_editTo4);

	for ( int n = 0; n < 4; n++ )
		DDX_Control( pDX, m_IDTo[n], m_editTo[n] );

	for ( int n = 1; n < 5; n++ )
	{
		DDX_Text(pDX, m_IDFrom[n], m_dFrom[n] );
		DDV_MinMaxDouble( pDX, m_dFrom[n], Promotions::FlexBoundValue.Min, Promotions::FlexBoundValue.Max);
	}

	for ( int n = 0; n < 5; n++ )
	{
		DDX_Text(pDX, m_IDPercent[n], m_dPercent[n] );
		DDV_MinMaxDouble( pDX, m_dPercent[n], Promotions::Percent.Min, Promotions::Percent.Max);
	}

	if ( pDX -> m_bSaveAndValidate == TRUE )
	{
		int nErrorType = 0;
		int nErrorPos = 0;
		bool bGotZeroAmount = FALSE;
		double dLastAmount = 0.0;

		for( int n = 1; ( n < 5 ) && ( 0 == nErrorType ); n++ )
		{
			pDX -> PrepareEditCtrl( m_IDFrom[n] );

			if ( m_dFrom[n] <= 0 )
			{
				bGotZeroAmount = TRUE;
			}
			else if ( TRUE == bGotZeroAmount )
			{
				nErrorType = 1;
				nErrorPos = n;
			}
			else if ( m_dFrom[n] <= dLastAmount )
			{
				nErrorType = 2;
				nErrorPos = n;
			}
			else
			{
				dLastAmount = m_dFrom[n];
			}
		}

		if ( nErrorType != 0 )
		{
			CString strError = "";
			CString strType = ( m_pPromoRecord -> GetPromoType() == 1 ) ? "Surcharge" : "Discount";

			if ( 1 == nErrorType )
				strError.Format( "%s threshold %d cannot be used because\nthe previous threshold is set to zero.", (const char*) strType, nErrorPos + 1 );
			else
				strError.Format( "%s threshold %d cannot be set to the same or\nlower value than the previous threshold.", (const char*) strType, nErrorPos + 1 );
				
			AfxMessageBox( strError );
			GetEditFrom( nErrorPos ) -> SetFocus();
			GetEditFrom( nErrorPos ) -> SetSel( 0, -1 );
			pDX -> Fail();
		}
	}

	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPagePromotionFlexValue, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPagePromotionFlexValue)
	//}}AFX_MSG_MAP
	ON_EN_KILLFOCUS( IDC_EDIT_FROM1, UpdateFromValue1)
	ON_EN_KILLFOCUS( IDC_EDIT_FROM2, UpdateFromValue2)
	ON_EN_KILLFOCUS( IDC_EDIT_FROM3, UpdateFromValue3)
	ON_EN_KILLFOCUS( IDC_EDIT_FROM4, UpdateFromValue4)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPagePromotionFlexValue::OnInitDialog() 
{
	GetRecordData();
	CPropertyPage::OnInitDialog();
	m_editDescription.LimitText( Promotions::Description.Max );

	m_editFrom0.SetWindowText( "0.00" );
	m_editFrom0.EnableWindow( FALSE );

	for ( int n = 0; n < 4; n++ )
		m_editTo[n].EnableWindow( FALSE );

	m_editTo4.SetWindowText( "   ----------" );
	m_editTo4.EnableWindow( FALSE );

	for ( int n = 1; n < 5; n++ )
		SubclassEdit( m_IDFrom[n], SS_NUM_DP, 10, "%.2f" );

	for ( int n = 0; n < 5; n++ )
		SubclassEdit( m_IDPercent[n], SS_NUM_SDP, 6, "%.2f" );

	return TRUE;  
}

/**********************************************************************/
CEdit* CPropPagePromotionFlexValue::GetEditFrom(int n){ return GetEdit( m_IDFrom[n] ); }
CEdit* CPropPagePromotionFlexValue::GetEditPercent(int n){ return GetEdit( m_IDPercent[n] ); }
/**********************************************************************/

BOOL CPropPagePromotionFlexValue::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPagePromotionFlexValue::OnSetActive() 
{
	GetRecordData();
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	SetRecordControls();
	return bResult;
}

/**********************************************************************/

void CPropPagePromotionFlexValue::GetRecordData()
{
	for ( int n = 1; n < 5; n++ )
		m_dFrom[n] = m_pPromoRecord -> GetFlexBoundValue(n);

	for ( int n = 0; n < 5; n++ )
		m_dPercent[n] = m_pPromoRecord -> GetFlexPercent(n);
}

/**********************************************************************/

void CPropPagePromotionFlexValue::SetRecordControls()
{
	m_editDescription.SetWindowText( m_pPromoRecord -> GetDisplayName() );

	if ( m_pPromoRecord -> GetPromoType() == 1 )
		m_staticDiscount.SetWindowText( "Surcharge" );
	else
		m_staticDiscount.SetWindowText( "Discount" );

	for( int n = 1; n <= 4; n++ )
		UpdateFromValue(n);

	m_editDescription.SetSel ( 0, -1 );
	m_editDescription.SetFocus();
}

/**********************************************************************/

bool CPropPagePromotionFlexValue::UpdateRecord()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		SaveRecord();
		return TRUE;
	}
	return FALSE;
}

/**********************************************************************/

void CPropPagePromotionFlexValue::SaveRecord()
{
	if ( FALSE == m_bUpdatingTabs )
	{
		CString strDescription;
		m_editDescription.GetWindowText( strDescription );
		m_pPromoRecord -> SetDescription( strDescription );

		for ( int n = 1; n < 5; n++ )
			m_pPromoRecord -> SetFlexBoundValue(n, m_dFrom[n]);

		for ( int n = 0; n < 5; n++ )
			m_pPromoRecord -> SetFlexPercent(n, m_dPercent[n]);
	}
}

/**********************************************************************/

void CPropPagePromotionFlexValue::Refresh()
{
	GetRecordData();
	UpdateData ( FALSE );
	SetRecordControls();
}

/**********************************************************************/
void CPropPagePromotionFlexValue::UpdateFromValue1(){ UpdateFromValue(1); }
void CPropPagePromotionFlexValue::UpdateFromValue2(){ UpdateFromValue(2); }
void CPropPagePromotionFlexValue::UpdateFromValue3(){ UpdateFromValue(3); }
void CPropPagePromotionFlexValue::UpdateFromValue4(){ UpdateFromValue(4); }
/**********************************************************************/

void CPropPagePromotionFlexValue::UpdateFromValue( int n )
{
	if ( ( n >= 1 ) && ( n <= 4 ) )
	{
		CString strNum;
		GetEditFrom(n) -> GetWindowText( strNum );
		double dNum = atof( strNum ) * 100.0;
		int nNum = ( int( floor ( dNum ) ) ) - 1;

		if ( nNum >= 0 )
		{
			dNum = ( (double) nNum ) / 100.0;
			strNum.Format( "%.2f", dNum );
			m_editTo[ n- 1 ].SetWindowText( strNum );
		}
		else
		{
			m_editTo[ n- 1 ].SetWindowText( "   ----------" );
		}
	}
}

/**********************************************************************/
