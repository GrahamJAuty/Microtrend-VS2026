/**********************************************************************/
 
/**********************************************************************/
//#include "DataManager.h"
/**********************************************************************/
#include "PropertySheetPluMixMatchEcrman.h"
/**********************************************************************/

IMPLEMENT_DYNAMIC(CPropertySheetPluMixMatchEcrman, CSpinPropertySheet)

CPropertySheetPluMixMatchEcrman::CPropertySheetPluMixMatchEcrman( int nPluIdx, CWnd* pWndParent)
	 : CSSPropertySheet( "", pWndParent ) 
{
	m_psh.dwFlags |= PSH_NOAPPLYNOW;

	m_Page1.SetPluRecord ( &m_PluRecord );
	m_Page1.EditDepartments ( TRUE );
	AddPage(&m_Page1);
	m_PageArray.Add(1);
		
	//get record based on this index
	m_nPluIdx = nPluIdx;
	DataManager.Plu.GetAt ( nPluIdx, m_PluRecord );
	UpdateTitle();
}

/**********************************************************************/

CPropertySheetPluMixMatchEcrman::~CPropertySheetPluMixMatchEcrman()
{
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropertySheetPluMixMatchEcrman, CPropertySheet)
	//{{AFX_MSG_MAP(CPropertySheetPluMixMatchEcrman)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

void CPropertySheetPluMixMatchEcrman::SaveRecord()
{
	DataManager.Plu.SavePluRecord ( m_nPluIdx, m_PluRecord );	
}

/**********************************************************************/

void CPropertySheetPluMixMatchEcrman::UpdateTitle()
{
	CString strText; 
	strText.Format ( "Edit Record %d, Plu %s, %s",
		m_nPluIdx + 1,
		m_PluRecord.GetPluNoString(),
		m_PluRecord.GetReportText() );
	
	SetTitle ( strText, 0 );
}

/**********************************************************************/

