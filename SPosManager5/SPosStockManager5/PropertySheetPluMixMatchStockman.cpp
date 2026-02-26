/**********************************************************************/
 
/**********************************************************************/
//#include "DataManager.h"
/**********************************************************************/
#include "PropertySheetPluMixMatchStockman.h"
/**********************************************************************/

IMPLEMENT_DYNAMIC(CPropertySheetPluMixMatchStockman, CPropertySheet)

CPropertySheetPluMixMatchStockman::CPropertySheetPluMixMatchStockman( int nPluIdx, CWnd* pWndParent)
	 : CSSPropertySheet( "", pWndParent) 
{
	m_nPluIdx = nPluIdx;

	m_psh.dwFlags |= PSH_NOAPPLYNOW;

	m_Page1.SetPluRecord ( &m_PluRecord );
	m_Page1.SetRecipeRecord( &m_RecipeRecord );
	m_Page1.EditDepartments ( TRUE );
	AddPage(&m_Page1);
	m_PageArray.Add ( 1 );

	//EXPERIMENTAL 
	m_Page1.SetNewStockFlag ( FALSE );
	m_Page1.SetPluChangesBuffer ( &m_PluChangesBuffer );
	//EXPERIMENTAL 
		
	//get record based on this index
	DataManager.Plu.GetAt ( m_nPluIdx, m_PluRecord );
	m_PluChangesBuffer.SetPluNo ( m_PluRecord.GetPluNoString() );
	DataManager.Plu.GetRecipe ( m_nPluIdx, m_PluRecord.GetPluNoInt(), m_RecipeRecord );
	
	UpdateTitle();
}

/**********************************************************************/

CPropertySheetPluMixMatchStockman::~CPropertySheetPluMixMatchStockman()
{
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropertySheetPluMixMatchStockman, CPropertySheet)
	//{{AFX_MSG_MAP(CPropertySheetPluMixMatchStockman)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

void CPropertySheetPluMixMatchStockman::SaveRecord()
{
	DataManager.Plu.SavePluRecord ( m_nPluIdx, m_PluRecord );	
}

/**********************************************************************/

void CPropertySheetPluMixMatchStockman::UpdateTitle()
{
	CString strText; 
	strText.Format ( "Edit Record %d, Plu %s, %s",
		m_nPluIdx + 1,
		m_PluRecord.GetPluNoString(),
		m_PluRecord.GetReportText() );
	
	SetTitle ( strText, 0 );
}

/**********************************************************************/
