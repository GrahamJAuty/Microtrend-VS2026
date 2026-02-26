/**********************************************************************/
#include "PluCSVArray.h"
/**********************************************************************/
#include "PropPageLocationBritannia.h"
/**********************************************************************/

CPropPageLocationBritannia::CPropPageLocationBritannia() : CSSPropertyPage(CPropPageLocationBritannia::IDD)
{
	//{{AFX_DATA_INIT(CPropPageLocationBritannia)
	//}}AFX_DATA_INIT
	m_pLocationBuffer = NULL;
}

/**********************************************************************/

CPropPageLocationBritannia::~CPropPageLocationBritannia()
{
}

/**********************************************************************/

void CPropPageLocationBritannia::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageLocationBritannia)
	//}}AFX_DATA_MAP
	DDX_Control( pDX, IDC_COMBO_PRICE, m_comboPrice );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPageLocationBritannia, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPageLocationBritannia)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPageLocationBritannia::OnInitDialog() 
{
	GetRecordData();
	CPropertyPage::OnInitDialog();

	for ( int p = 1; p <= MAX_PLU_PRICES; p++ )
	{
		CString str;
		str.Format( "Price %d", p );
		m_comboPrice.AddString( str );
	}

	int nBand = m_pLocationBuffer -> GetBritanniaDefaultPriceBand(0);

	if ( ( nBand < 1 ) || ( nBand > MAX_PLU_PRICES ) )
		nBand = 1;

	m_comboPrice.SetCurSel( nBand - 1 );

	return TRUE;  
}

/**********************************************************************/

BOOL CPropPageLocationBritannia::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPageLocationBritannia::OnSetActive() 
{
	GetRecordData();
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	return bResult;
}

/**********************************************************************/

bool CPropPageLocationBritannia::UpdateRecord()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		return TRUE;
	}
	return FALSE;
}

/**********************************************************************/

void CPropPageLocationBritannia::Refresh()
{
	UpdateData ( FALSE );
}

/**********************************************************************/

void CPropPageLocationBritannia::GetRecordData()
{
}

/**********************************************************************/

void CPropPageLocationBritannia::SaveRecord()
{
	m_pLocationBuffer -> SetBritanniaDefaultPriceBand( 0, m_comboPrice.GetCurSel() + 1 );
}

/**********************************************************************/

