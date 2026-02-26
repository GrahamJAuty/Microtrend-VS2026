/**********************************************************************/
#include "MaxLengths.h"
/**********************************************************************/
#include "PropPageLocationNotes.h"
/**********************************************************************/

CPropPageLocationNotes::CPropPageLocationNotes() : CSSPropertyPage(CPropPageLocationNotes::IDD)
{
	//{{AFX_DATA_INIT(CPropPageLocationNotes)
	//}}AFX_DATA_INIT
	m_pLocationBuffer = NULL;
	m_pLocationNotes = NULL;
}

/**********************************************************************/

CPropPageLocationNotes::~CPropPageLocationNotes()
{
}

/**********************************************************************/

void CPropPageLocationNotes::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageLocationNotes)
	//}}AFX_DATA_MAP
	DDX_Control( pDX, IDC_EDIT_NOTES, m_editNotes);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPageLocationNotes, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPageLocationNotes)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPageLocationNotes::OnInitDialog() 
{
	GetRecordData();
	CPropertyPage::OnInitDialog();
	m_pLocationNotes -> CopyBufferToEditBox ( m_editNotes );
	return TRUE;  
}

/**********************************************************************/

BOOL CPropPageLocationNotes::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPageLocationNotes::OnSetActive() 
{
	GetRecordData();
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	return bResult;
}

/**********************************************************************/

bool CPropPageLocationNotes::UpdateRecord()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		return TRUE;
	}
	return FALSE;
}

/**********************************************************************/

void CPropPageLocationNotes::Refresh()
{
	UpdateData ( FALSE );
}

/**********************************************************************/

void CPropPageLocationNotes::GetRecordData()
{
}

/**********************************************************************/

void CPropPageLocationNotes::SaveRecord()
{
	m_pLocationNotes -> CopyEditBoxToBuffer ( m_editNotes );
}

/**********************************************************************/

