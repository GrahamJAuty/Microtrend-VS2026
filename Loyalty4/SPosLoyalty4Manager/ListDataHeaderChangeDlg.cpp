/**********************************************************************/
#include "ListDataHeaderChangeDlg.h"
/**********************************************************************/

CSingleListContentHelpersHeaderChange::CSingleListContentHelpersHeaderChange( CString strOldHeader, CString strNewHeader ) 
{
	CCSV csvOld(strOldHeader);

	for (int n = 0; n < csvOld.GetSize(); n++)
	{
		m_arrayOldHeader.Add(csvOld.GetString(n));
	}

	CCSV csvNew(strNewHeader);

	for (int n = 0; n < csvNew.GetSize(); n++)
	{
		m_arrayNewHeader.Add(csvNew.GetString(n));
	}
}

/**********************************************************************/

int CSingleListContentHelpersHeaderChange::GetArraySize()
{
	int nSizeOld = m_arrayOldHeader.GetSize();
	int nSizeNew = m_arrayNewHeader.GetSize();
	return max(nSizeOld, nSizeNew);
}

/**********************************************************************/

bool CSingleListContentHelpersHeaderChange::PrepareLine( int nIndex, CString& strLine )
{
	strLine = "";

	bool bResult = FALSE;
	if ( ( nIndex >= 0 ) && ( nIndex < GetArraySize() ) )
	{
		CCSV csv('\t');

		csv.Add(nIndex + 1);

		if (nIndex < m_arrayOldHeader.GetSize())
		{
			csv.Add(m_arrayOldHeader.GetAt(nIndex));
		}
		else
		{
			csv.Add("--");
		}

		if (nIndex < m_arrayNewHeader.GetSize())
		{
			csv.Add(m_arrayNewHeader.GetAt(nIndex));
		}
		else
		{
			csv.Add("--");
		}

		strLine = csv.GetLine();
		bResult = TRUE;
	}
	
	return bResult;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListDataHeaderChangeDlg::CListDataHeaderChangeDlg( const char* szTitle, CString strOldHeader, CString strNewHeader, CWnd* pParent) : CListDataDlg ( pParent )
{
	m_strHeader1Description = "";
	m_strHeader2Description = "";

	m_bMultiSelect = FALSE;
	m_pContentHelpers = new CSingleListContentHelpersHeaderChange( strOldHeader, strNewHeader );
	m_pDisplayHelpers = new CSingleListDisplayHelpers( m_listData, *m_pContentHelpers );
	m_strTitle = szTitle;
}

/**********************************************************************/

CListDataHeaderChangeDlg::~CListDataHeaderChangeDlg()
{
	delete m_pDisplayHelpers;
	delete m_pContentHelpers;
}

/**********************************************************************/

void CListDataHeaderChangeDlg::SetTitle()
{
	SetWindowText ( m_strTitle );
}

/**********************************************************************/

void CListDataHeaderChangeDlg::SetButtonTexts()
{
	m_button1.ShowWindow ( SW_HIDE );
	m_button2.ShowWindow ( SW_HIDE );
	m_button3.ShowWindow ( SW_HIDE );
	m_button4.ShowWindow ( SW_HIDE );
	m_button5.ShowWindow ( SW_HIDE );
	m_button6.ShowWindow ( SW_HIDE );
	MoveControl( &m_button1, 368, 7 );
	ShowAndEnableWindow(&m_buttonOK, FALSE);
	m_buttonCancel.SetWindowText("Close");
}

/**********************************************************************/

void CListDataHeaderChangeDlg::PrepareList()
{
	CListDataDlg::PrepareList();
	SelectLine(0);
}

/**********************************************************************/

void CListDataHeaderChangeDlg::SetColumns()
{
	m_listData.SubclassDlgItem(IDC_LIST, this);
	m_listData.ModifyStyle(0, LVS_SINGLESEL, 0);
	m_listData.InsertColumn(0, "Col", LVCFMT_LEFT, 60);
	m_listData.InsertColumn(1, m_strHeader1Description, LVCFMT_LEFT, 210);
	m_listData.InsertColumn(2, m_strHeader2Description, LVCFMT_LEFT, 210);
}

/**********************************************************************/
