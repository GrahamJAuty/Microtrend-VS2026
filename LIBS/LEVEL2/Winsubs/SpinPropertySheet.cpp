/**********************************************************************/
#include "afxdlgs.h"
/**********************************************************************/

IMPLEMENT_DYNAMIC(CSSPropertySheet, CPropertySheet)

CSSPropertySheet::CSSPropertySheet(const char* szTitle, CWnd* pWndParent, UINT nFirstPage)
	: CPropertySheet(szTitle, pWndParent, nFirstPage)
{
}

/**********************************************************************/

int CSSPropertySheet::DoModal()
{
	m_psh.dwFlags |= PSH_USECALLBACK;
	m_psh.pfnCallback = PropSheetProc;
	return CPropertySheet::DoModal();
}

/**********************************************************************/

void CSSPropertySheet::BuildPropPageArray()
{
	CPropertySheet::BuildPropPageArray();

	PROPSHEETPAGE* ppsp = (PROPSHEETPAGE*)m_psh.ppsp;
	for (int nPage = 0; nPage < m_pages.GetSize(); nPage++)
	{
		LPDLGTEMPLATE pTemplate = (LPDLGTEMPLATE)ppsp[nPage].pResource;

		CMyDialogTemplate dlgTemplate;
		dlgTemplate.Attach(pTemplate);
		dlgTemplate.SetFont("MS Shell Dlg 2", 8);
		dlgTemplate.Detach();
	}
}

/**********************************************************************/

int CALLBACK CSSPropertySheet::PropSheetProc(HWND hwndDlg, UINT uMsg, LPARAM lParam)
{
	switch (uMsg)
	{
	case PSCB_PRECREATE:
	{
		LPDLGTEMPLATE pTemplate = (LPDLGTEMPLATE)lParam;

		CMyDialogTemplate dlgTemplate;
		dlgTemplate.Attach(pTemplate);
		dlgTemplate.SetFont("MS Shell Dlg 2", 8);

		dlgTemplate.Detach();
		break;
	}
	default:
		break;
	}

	return 0;
}

/**********************************************************************/
static const int SPIN_BUTTON_WIDTH = 80;
static const int SPIN_BUTTON_HEIGHT = 22;
/**********************************************************************/

IMPLEMENT_DYNAMIC(CSpinPropertySheet, CPropertySheet)

CSpinPropertySheet::CSpinPropertySheet( bool bEnableSpin, WORD wSpinID, const char* szTitle, CWnd* pWndParent, UINT nFirstPage)
	 : CSSPropertySheet(szTitle, pWndParent, nFirstPage)
{
	m_bEnableSpin = bEnableSpin;
	m_wSpinID = wSpinID;
}

/**********************************************************************/

CSpinPropertySheet::~CSpinPropertySheet()
{
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSpinPropertySheet, CPropertySheet)
	//{{AFX_MSG_MAP(CSpinPropertySheet)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

int CSpinPropertySheet::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPropertySheet::OnCreate(lpCreateStruct) == -1)
		return -1;

	if ( m_bEnableSpin == TRUE )
	{
		CRect Rect ( 0, 0, SPIN_BUTTON_WIDTH, SPIN_BUTTON_HEIGHT );
		m_SpinButton.Create ( UDS_HORZ | WS_VISIBLE, Rect, this, m_wSpinID );
		m_SpinButton.SetFocus();
	}
	return 0;
}

/**********************************************************************/

void CSpinPropertySheet::OnSize(UINT nType, int cx, int cy) 
{
	CPropertySheet::OnSize(nType, cx, cy);		// cy = new height of the client area

	if ( m_bEnableSpin == TRUE )
	{
		m_SpinButton.MoveWindow ( 8, cy - SPIN_BUTTON_HEIGHT - 8, SPIN_BUTTON_WIDTH, SPIN_BUTTON_HEIGHT, TRUE );
		m_SpinButton.SetFocus();
	}
}

/**********************************************************************/

BOOL CSpinPropertySheet::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	NM_UPDOWN* pNM_UPDOWN = (NM_UPDOWN*) lParam;

	if ( wParam == m_wSpinID )
	{
		if ( pNM_UPDOWN->iDelta > 0 )
			SpinPrevious();
		else
			SpinNext();
	}
	return CPropertySheet::OnNotify(wParam, lParam, pResult);
}

/**********************************************************************/

