/**********************************************************************/
#include "SPOSKeyboardKeyHelpers.h"
/**********************************************************************/
#include "SPOSKeyboardPropertySheetEditList.h"
/**********************************************************************/
IMPLEMENT_DYNAMIC(CSPOSKeyboardPropertySheetEditList, CPropertySheet)
/**********************************************************************/

CSPOSKeyboardPropertySheetEditList::CSPOSKeyboardPropertySheetEditList( int nListIdx, CWnd* pWndParent)
	 : CSSPropertySheet("", pWndParent)
{	
	m_psh.dwFlags |= PSH_NOAPPLYNOW;
	
	m_nSecondTabType = -1;
	
	AddPage( &m_Page1 );
	
	m_nListIdx = nListIdx;
	SPOSKeyboardManager.GetListManager() -> GetListRecord( m_nListIdx, m_ListRecord );

	m_Page1.SetListRecord ( &m_ListRecord );
	m_Page2.SetListRecord ( &m_ListRecord );
	m_Page3.SetListRecord ( &m_ListRecord );
	m_Page4.SetListRecord ( &m_ListRecord );

	m_Page1.SetPropertySheet(this);

	SetSecondTabType( m_ListRecord.GetListActionType() );

	CSPOSKeyboardKeyHelpers KeyHelpers;
	__int64 nListNo = m_ListRecord.GetListNo();
	CString strCombiText = KeyHelpers.GetCombiText( KEY_TYPE_LIST, nListNo );

	CString strTitle;
	strTitle.Format ( "List Settings (%s)",
		(const char*) strCombiText );

	SetTitle ( strTitle );
}

/**********************************************************************/

void CSPOSKeyboardPropertySheetEditList::SetSecondTabType( int nType )
{
	if ( nType != m_nSecondTabType )
	{
		switch( m_nSecondTabType )
		{
		case LISTACTION_NORMAL:
		case LISTACTION_SERVER:
		case LISTACTION_TABLE:
			RemovePage(1);
			break;
		}

		switch( nType )
		{
		case LISTACTION_NORMAL:
			AddPage( &m_Page2 );
			break;

		case LISTACTION_SERVER:
			AddPage( &m_Page3 );
			break;

		case LISTACTION_TABLE:
			AddPage( &m_Page4 );
			break;
		}

		m_nSecondTabType = nType;
	}
}

/**********************************************************************/

CSPOSKeyboardPropertySheetEditList::~CSPOSKeyboardPropertySheetEditList()
{
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSPOSKeyboardPropertySheetEditList, CPropertySheet)
	//{{AFX_MSG_MAP(CSPOSKeyboardPropertySheetEditList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CSPOSKeyboardPropertySheetEditList::OnInitDialog() 
{
	BOOL bResult = CPropertySheet::OnInitDialog();
	return bResult;
}

/**********************************************************************/

bool CSPOSKeyboardPropertySheetEditList::CheckKeyCount()
{
	CSPOSKeyboardListRecord ListRecord;
	SPOSKeyboardManager.GetListManager() -> GetListRecord( m_nListIdx, ListRecord );

	if ( m_ListRecord.GetKeysPerPage() > ListRecord.GetKeysPerPage() )
	{
		int nOldPageCount = ListRecord.GetPageCount();
	
		int nNewMaxPageCount = MAX_LIST_KEYS / m_ListRecord.GetKeysPerPage();
	
		if ( nNewMaxPageCount > MAX_LIST_PAGES ) 
			nNewMaxPageCount = MAX_LIST_PAGES;

		if ( nOldPageCount > nNewMaxPageCount )
		{
			CString strLine1;
			strLine1.Format ( "This list has %d pages of %d x %d for a total of %d keys.\n\n",
				nOldPageCount,
				ListRecord.GetRowCount(),
				ListRecord.GetColumnCount(),
				ListRecord.GetKeysPerPage() * nOldPageCount );

			int nToGo = nOldPageCount - nNewMaxPageCount;
			CString strPages = ( nToGo == 1 ) ? "page" : "pages";
			
			CString strLine2;
			strLine2.Format ( "Changing the format to %d x %d would require %d %s to be deleted.\n\n",
				m_ListRecord.GetRowCount(),
				m_ListRecord.GetColumnCount(),
				nToGo,
				(const char*) strPages );

			CString strLine3;
			strLine3.Format( "Do you wish to change the list format and delete the surplus %s.",
				(const char*) strPages );

			if ( Prompter.YesNo( strLine1 + strLine2 + strLine3 ) != IDYES )
			{
				Prompter.Info( "The settings for this list have not been changed." );
				return FALSE;
			}
		}
	}
	return TRUE;
}

/**********************************************************************/
	
void CSPOSKeyboardPropertySheetEditList::SaveRecord()
{
	CSPOSKeyboardListRecord ListRecord;
	SPOSKeyboardManager.GetListManager() -> GetListRecord( m_nListIdx, ListRecord );
	
	int nOldFormat = ListRecord.GetFormat();
	int nOldPageCount = ListRecord.GetPageCount();
	int nNewMaxPageCount = MAX_LIST_KEYS / m_ListRecord.GetKeysPerPage();
	
	if ( nNewMaxPageCount > 100 )
		nNewMaxPageCount = 100;
	
	SPOSKeyboardManager.GetListManager() -> SetListRecord( m_nListIdx, m_ListRecord );
	SPOSKeyboardManager.GetListManager() -> UpdateKeysForList( m_nListIdx, nOldFormat );

	if ( nOldPageCount > nNewMaxPageCount )
	{
		CWordArray arrayPages;
		for ( int n = 1; n <= nNewMaxPageCount; n++ )
			arrayPages.Add( n );

		SPOSKeyboardManager.GetListManager() -> MovePages( ListRecord.GetListNo(), arrayPages );
	}	
}

/**********************************************************************/
