/**********************************************************************/
#include "EcrmanOptionsIni.h"
#include "LocationCSVArray.h"
/**********************************************************************/
#include "PropertySheetServer.h"
/**********************************************************************/
IMPLEMENT_DYNAMIC(CPropertySheetServer, CSpinPropertySheet)
/**********************************************************************/

CPropertySheetServer::CPropertySheetServer(WORD wSpinID, bool bFullEdit, int nServerDbIdx, int nServerLocIdx, int nTerminalLocIdx, int nServerIdx, CListDataDlgChangeFlag& WarnCancelAfterChange, CWnd* pWndParent)
	: CSpinPropertySheet(TRUE, wSpinID, "", pWndParent), m_WarnCancelAfterChange(WarnCancelAfterChange)
{
	m_pListDataDlg = NULL;
	m_psh.dwFlags |= PSH_NOAPPLYNOW;

	m_nServerDbIdx = nServerDbIdx;
	m_nServerLocIdx = nServerLocIdx;
	m_nTerminalLocIdx = nTerminalLocIdx;

	m_bFullEdit = bFullEdit;

	m_PageGeneral.SetServerRecord(&m_ServerRecord);
	m_PageGeneral.SetFullEditFlag(bFullEdit);
	m_PageGeneral.SetServerDbIdx(m_nServerDbIdx);
	m_PageGeneral.SetServerLocIdx(m_nServerLocIdx);
	AddPage(&m_PageGeneral);
	m_PageArray.Add(1);

	if ((EcrmanOptions.GetFeaturesServerLogonFlag() == TRUE) && (TRUE == m_bFullEdit))
	{
		m_PageLogon.SetServerRecord(&m_ServerRecord);
		m_PageLogon.SetFullEditFlag(TRUE);
		AddPage(&m_PageLogon);
		m_PageArray.Add(2);
	}

	if (m_nTerminalLocIdx != -1)
	{
		m_PageRestrict.SetFullEditFlag(bFullEdit);
		m_PageRestrict.SetServerRecord(&m_ServerRecord);
		m_PageRestrict.SetRestrictionRecord(&m_RestrictionRecord);
		AddPage(&m_PageRestrict);
		m_PageArray.Add(3);
	}

	m_nServerIdx = nServerIdx;
	DataManagerNonDb.Server.GetAt(m_nServerIdx, m_ServerRecord);

	if (m_nTerminalLocIdx != -1)
	{
		DataManagerNonDb.ServerRestriction.GetRestrictions(m_ServerRecord, m_RestrictionRecord);
	}

	UpdateTitle();
}

/**********************************************************************/

CPropertySheetServer::~CPropertySheetServer()
{
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropertySheetServer, CSpinPropertySheet)
	//{{AFX_MSG_MAP(CPropertySheetServer)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropertySheetServer::OnInitDialog()
{
	CPropertySheet::OnInitDialog();

	if ( ( DealerFlags.GetServerHomeLocationFlag() == FALSE ) || ( FALSE == m_bFullEdit ) )
	{
		RECT rc;

		// resize the sheet
		GetWindowRect(&rc);
		ScreenToClient(&rc);
		rc.bottom -= 20;
		MoveWindow(&rc);

		// resize the CTabCtrl
		CTabCtrl* pTab = GetTabControl();
		ASSERT(pTab);
		pTab->GetWindowRect(&rc);
		ScreenToClient(&rc);
		rc.bottom -= 20;
		pTab->MoveWindow(&rc);

		// resize the page
		CPropertyPage* pPage = GetActivePage();
		ASSERT(pPage);
		// store page size in m_PageRect
		pPage->GetWindowRect(&m_PageRect);
		ScreenToClient(&m_PageRect);
		m_PageRect.bottom -= 20;
		pPage->MoveWindow(&m_PageRect);

		// move the OK, Cancel, and Apply buttons
		CWnd* pWnd = GetDlgItem(IDOK);
		pWnd->GetWindowRect(&rc);
		rc.top -= 20;
		rc.bottom -= 20;
		ScreenToClient(&rc);
		pWnd->MoveWindow(&rc);

		pWnd = GetDlgItem(IDCANCEL);
		pWnd->GetWindowRect(&rc);
		rc.top -= 20;
		rc.bottom -= 20;
		ScreenToClient(&rc);
		pWnd->MoveWindow(&rc);
	}

	CenterWindow();
	return TRUE;
}

/**********************************************************************/

void CPropertySheetServer::SpinNext()
{
	int nSortIdx = DataManagerNonDb.Server.GetSortIdxFromArrayIdx(m_nServerIdx);

	if (nSortIdx < DataManagerNonDb.Server.GetSortArraySize() - 1)
	{
		int nPage = m_PageArray.GetAt(GetActiveIndex());

		bool bUpdate = FALSE;

		switch (nPage)
		{
		case 1:
			bUpdate = m_PageGeneral.UpdateRecord();
			break;

		case 2:
			bUpdate = m_PageLogon.UpdateRecord();
			break;

		case 3:
			bUpdate = m_PageRestrict.UpdateRecord();
			break;
		}

		if (FALSE == bUpdate)
		{
			return;
		}

		SaveRecord();
		m_nServerIdx = DataManagerNonDb.Server.GetArrayIdxFromSortIdx(nSortIdx + 1);
		DataManagerNonDb.Server.GetAt(m_nServerIdx, m_ServerRecord);

		if (m_nTerminalLocIdx != -1)
		{
			DataManagerNonDb.ServerRestriction.GetRestrictions(m_ServerRecord, m_RestrictionRecord);
		}

		UpdateTitle();

		switch (nPage)
		{
		case 1:
			m_PageGeneral.Refresh();
			break;

		case 2:
			m_PageLogon.Refresh();
			break;

		case 3:
			m_PageRestrict.Refresh();
			break;
		}

		if (m_pListDataDlg != NULL)
		{
			m_pListDataDlg->SelectLine(nSortIdx + 1);
		}
	}
}

/**********************************************************************/

void CPropertySheetServer::SpinPrevious()
{
	int nSortIdx = DataManagerNonDb.Server.GetSortIdxFromArrayIdx(m_nServerIdx);

	if (nSortIdx >= 1)
	{
		int nPage = m_PageArray.GetAt(GetActiveIndex());

		bool bUpdate = FALSE;

		switch (nPage)
		{
		case 1:
			bUpdate = m_PageGeneral.UpdateRecord();
			break;

		case 2:
			bUpdate = m_PageLogon.UpdateRecord();
			break;

		case 3:
			bUpdate = m_PageRestrict.UpdateRecord();
			break;
		}

		if (FALSE == bUpdate)
		{
			return;
		}

		SaveRecord();
		m_nServerIdx = DataManagerNonDb.Server.GetArrayIdxFromSortIdx(nSortIdx - 1);
		DataManagerNonDb.Server.GetAt(m_nServerIdx, m_ServerRecord);

		if (m_nTerminalLocIdx != -1)
		{
			DataManagerNonDb.ServerRestriction.GetRestrictions(m_ServerRecord, m_RestrictionRecord);
		}

		UpdateTitle();

		switch (nPage)
		{
		case 1:
			m_PageGeneral.Refresh();
			break;

		case 2:
			m_PageLogon.Refresh();
			break;

		case 3:
			m_PageRestrict.Refresh();
			break;
		}

		if (m_pListDataDlg != NULL)
		{
			m_pListDataDlg->SelectLine(nSortIdx - 1);
		}
	}
}

/**********************************************************************/

void CPropertySheetServer::SaveRecord()
{
	DataManagerNonDb.Server.SetAt ( m_nServerIdx, m_ServerRecord );
	DataManagerNonDb.ServerRestriction.SetRestrictions ( m_RestrictionRecord );
	m_WarnCancelAfterChange.SetFlag();
}

/**********************************************************************/

void CPropertySheetServer::UpdateTitle()
{
	CServerCSVRecord ServerRecord;
	DataManagerNonDb.Server.GetAt ( m_nServerIdx, ServerRecord );

	{
		CString strTitle;
		strTitle.Format ( "S%3.3d %s", 
			ServerRecord.GetServerNo(), 
			ServerRecord.GetReportName() );

		SetTitle ( strTitle );
	}
}

/**********************************************************************/
