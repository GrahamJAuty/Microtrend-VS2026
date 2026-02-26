/**********************************************************************/
#include "MaxLengths.h"
/**********************************************************************/
#include "PropPageLocationIDraught.h"
/**********************************************************************/

CPropPageLocationIDraught::CPropPageLocationIDraught() : CSSPropertyPage(CPropPageLocationIDraught::IDD)
{
	//{{AFX_DATA_INIT(CPropPageLocationIDraught)
	//}}AFX_DATA_INIT
	m_pLocationBuffer = NULL;
}

/**********************************************************************/

CPropPageLocationIDraught::~CPropPageLocationIDraught()
{
}

/**********************************************************************/

void CPropPageLocationIDraught::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageLocationIDraught)
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_IDRAUGHT, m_strIDraughtID);
	DDV_MaxChars(pDX, m_strIDraughtID, MAX_LENGTH_IDRAUGHT_ID );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPageLocationIDraught, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPageLocationIDraught)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPageLocationIDraught::OnInitDialog() 
{
	GetRecordData();
	CPropertyPage::OnInitDialog();
	return TRUE;  
}

/**********************************************************************/

BOOL CPropPageLocationIDraught::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPageLocationIDraught::OnSetActive() 
{
	GetRecordData();
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	return bResult;
}

/**********************************************************************/

bool CPropPageLocationIDraught::UpdateRecord()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		return TRUE;
	}
	return FALSE;
}

/**********************************************************************/

void CPropPageLocationIDraught::Refresh()
{
	UpdateData ( FALSE );
}

/**********************************************************************/

void CPropPageLocationIDraught::GetRecordData()
{
	m_strIDraughtID = m_pLocationBuffer -> GetIDraughtID(0);
}

/**********************************************************************/

void CPropPageLocationIDraught::SaveRecord()
{
	m_pLocationBuffer -> SetIDraughtID( 0, m_strIDraughtID );
}

/**********************************************************************/

