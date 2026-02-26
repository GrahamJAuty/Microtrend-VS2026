/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include "PMSOptions.h"
/**********************************************************************/
#include "PropPagePMSOptionsPayment.h"
/**********************************************************************/

CPropPagePMSOptionsPayment::CPropPagePMSOptionsPayment() : CSSPropertyPage(CPropPagePMSOptionsPayment::IDD)
{
	//{{AFX_DATA_INIT(CPropPagePMSOptionsPayment)
	//}}AFX_DATA_INIT
}

/**********************************************************************/

CPropPagePMSOptionsPayment::~CPropPagePMSOptionsPayment()
{
}

/**********************************************************************/

void CPropPagePMSOptionsPayment::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPagePMSOptionsPayment)
	//}}AFX_DATA_MAP
	DDX_Control( pDX, IDC_COMBO_PAYMENT1, m_comboPayment[0]);
	DDX_Control( pDX, IDC_COMBO_PAYMENT2, m_comboPayment[1]);
	DDX_Control( pDX, IDC_COMBO_PAYMENT3, m_comboPayment[2]);
	DDX_Control( pDX, IDC_COMBO_PAYMENT4, m_comboPayment[3]);
	DDX_Control( pDX, IDC_COMBO_PAYMENT5, m_comboPayment[4]);
	DDX_Control( pDX, IDC_COMBO_PAYMENT6, m_comboPayment[5]);
	DDX_Control( pDX, IDC_COMBO_PAYMENT7, m_comboPayment[6]);
	DDX_Control( pDX, IDC_COMBO_PAYMENT8, m_comboPayment[7]);
	DDX_Control( pDX, IDC_COMBO_PAYMENT9, m_comboPayment[8]);
	DDX_Control( pDX, IDC_COMBO_PAYMENT10, m_comboPayment[9]);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPagePMSOptionsPayment, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPagePMSOptionsPayment)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPagePMSOptionsPayment::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	for ( int n = 0; n < 10; n++ )
	{
		m_comboPayment[n].AddString( "None" );
		m_comboPayment[n].SetItemData(0,0);
	}

	for ( int p = 0; p < DataManager.Payment.GetSize(); p++ )
	{
		CPaymentCSVRecord Payment;
		DataManager.Payment.GetAt( p, Payment );

		switch( Payment.GetPaymentType() )
		{
		case PAYMENT_TYPE_ROOM_AUTO:
		case PAYMENT_TYPE_ROOM_MANUAL:
			break;

		default:
			for ( int nType = 0; nType < 10; nType++ )
			{
				int nPos = m_comboPayment[nType].AddString( Payment.GetDisplayName() );
				m_comboPayment[nType].SetItemData( nPos, Payment.GetPaymentNo() );
			}
			break;
		}
	}

	if ( DataManager.Payment.GetSize() == 0 )
	{
		for( int nType = 0; nType < 10; nType++ )
		{
			m_comboPayment[nType].SetCurSel(0);
			m_comboPayment[nType].EnableWindow( FALSE );
		}
	}
	else
	{
		for ( int nType = 0; nType < 10; nType++ )
		{
			int nPayNo = PMSOptions.GetPaymentType(nType);

			int nMatch = 0;
			
			for ( int nSel = 1; nSel < m_comboPayment[nType].GetCount(); nSel++ )
			{
				if ( m_comboPayment[nType].GetItemData( nSel ) == nPayNo )
				{
					nMatch = nSel;
					break;
				}
			}

			m_comboPayment[nType].SetCurSel( nMatch );
		}
	}

	return TRUE;  
}

/**********************************************************************/

BOOL CPropPagePMSOptionsPayment::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPagePMSOptionsPayment::OnSetActive() 
{
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	return bResult;
}

/**********************************************************************/

void CPropPagePMSOptionsPayment::SaveRecord()
{
	for ( int nType = 0; nType < 10; nType++ )
	{
		int nSel = m_comboPayment[nType].GetCurSel();

		if ( ( nSel >= 0 ) && ( nSel < m_comboPayment[nType].GetCount() ) )
			PMSOptions.SetPaymentType( nType, m_comboPayment[nType].GetItemData(nSel) );
		else
			PMSOptions.SetPaymentType( nType, 0 );
	}
}

/**********************************************************************/
#endif
/**********************************************************************/
