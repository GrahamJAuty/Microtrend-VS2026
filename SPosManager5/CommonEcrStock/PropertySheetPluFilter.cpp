/**********************************************************************/
#include "PropertySheetPluFilter.h"
/**********************************************************************/
IMPLEMENT_DYNAMIC(CPropertySheetPluFilter, CSpinPropertySheet)
/**********************************************************************/

CPropertySheetPluFilter::CPropertySheetPluFilter( WORD wSpinID, int nIndex, bool bSystem, bool bAllowSpin, CListDataDlgChangeFlag& WarnCancelAfterChange, CWnd* pWndParent)
	 : CSpinPropertySheet(bAllowSpin, wSpinID, "", pWndParent ), m_PluFilterRecord( NODE_PLUFILTER_NAME ), m_WarnCancelAfterChange( WarnCancelAfterChange )
{	
	m_pListDataDlg = NULL;
	
	m_nIndex = nIndex;
	m_bSystem = bSystem;

	if ( TRUE == m_bSystem )
	{
		m_pFilterNames = &DataManagerNonDb.SystemPluFilterNames;
		m_pFilterItems = &DataManagerNonDb.SystemPluFilterItems;
	}
	else
	{
		m_pFilterNames = &DataManager.DbPluFilterNames;
		m_pFilterItems = &DataManager.DbPluFilterItems;
	}

	m_bAllowSpin = bAllowSpin;

	m_Page1.SetEntryType( OFFERENTRY_PLU );
	m_Page1.SetNodeType( NODE_PLUFILTER_NAME );
	m_Page1.m_psp.dwFlags |= PSP_USETITLE;
	m_Page1.m_psp.pszTitle = "Plu Filter Items";

	m_psh.dwFlags |= PSH_NOAPPLYNOW;	
	AddPage( &m_Page1 );

	m_Page1.SetPluFilterMode( m_bSystem );
	m_Page1.SetOfferGroupRecord( &m_PluFilterRecord );
	
	m_pFilterNames -> GetAt ( m_nIndex, m_PluFilterRecord );

	m_Page1.BuildPluArray();
	
	SetActivePage(1);
	UpdateTitle();
}

/**********************************************************************/

CPropertySheetPluFilter::~CPropertySheetPluFilter()
{
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropertySheetPluFilter, CSpinPropertySheet)
	//{{AFX_MSG_MAP(CPropertySheetPluFilter)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

void CPropertySheetPluFilter::SpinNext()
{
	if ( m_nIndex < m_pFilterNames -> GetSize() - 1 )
	{
		int nPage = GetActiveIndex() + 1;

		switch ( nPage )
		{
		case 1:	m_Page1.SaveRecord();	break;
		}
		
		SaveRecord();
		m_nIndex++;
		m_pFilterNames -> GetAt ( m_nIndex, m_PluFilterRecord );
		UpdateTitle();

		m_Page1.Refresh();
		
		if ( m_pListDataDlg != NULL )
			m_pListDataDlg -> SelectLine ( m_nIndex );
	}	
}

/**********************************************************************/

void CPropertySheetPluFilter::SpinPrevious()
{
	if ( m_nIndex >= 1 )
	{
		int nPage = GetActiveIndex() + 1;

		switch ( nPage )
		{
		case 1:	m_Page1.SaveRecord();	break;
		}
		
		SaveRecord();
		m_nIndex--;
		m_pFilterNames -> GetAt ( m_nIndex, m_PluFilterRecord );
		UpdateTitle();

		m_Page1.Refresh();
		
		if ( m_pListDataDlg != NULL )
			m_pListDataDlg -> SelectLine ( m_nIndex );
	}
}

/**********************************************************************/

void CPropertySheetPluFilter::SaveRecord()
{
	m_pFilterNames -> SetAt ( m_nIndex, m_PluFilterRecord );

	int nFilterNo = m_PluFilterRecord.GetGroupNo();
	m_pFilterItems -> PurgeGroup( nFilterNo );
	
	m_Page1.SaveOfferGroupEntries();

	m_WarnCancelAfterChange.SetFlag();
}

/**********************************************************************/

void CPropertySheetPluFilter::UpdateTitle()
{
	CString strTitle;
	strTitle.Format( "Edit Plu Report Filter (%d)", m_PluFilterRecord.GetGroupNo() );
	SetTitle( strTitle );
}

/**********************************************************************/