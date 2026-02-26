/**********************************************************************/
 
/**********************************************************************/
#include "InPlaceEdit.h"
/**********************************************************************/
#define CTRL_C	0x3
#define CTRL_V	0x16
#define CTRL_X	0x18
/**********************************************************************/
CInPlaceEdit* CInPlaceEdit::m_pInPlaceEdit = NULL;  
/**********************************************************************/

CInPlaceEdit::CInPlaceEdit() : CEditFilter( SS_NUM_SDP, "%d" )
{
	m_iRowIndex= -1;
	m_iColumnIndex = -1;
	m_bESC = FALSE;
}

/**********************************************************************/

CInPlaceEdit::~CInPlaceEdit()
{
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CInPlaceEdit, CEditFilter)
	//{{AFX_MSG_MAP(CInPlaceEdit)
	ON_WM_KILLFOCUS()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

void CInPlaceEdit::OnKillFocus(CWnd* pNewWnd) 
{
	CEdit::OnKillFocus(pNewWnd);

	FormatWindowText();
	
	// TODO: Add your message handler code here

	// Get the text in the edit ctrl
	CString strEdit;
	GetWindowText(strEdit);

	// Send Notification to parent of edit ctrl
	LV_DISPINFO dispinfo;
	dispinfo.hdr.hwndFrom = GetParent()->m_hWnd;
	dispinfo.hdr.idFrom = GetDlgCtrlID();
	dispinfo.hdr.code = LVN_ENDLABELEDIT;

	dispinfo.item.mask = LVIF_TEXT;
	dispinfo.item.iItem = m_iRowIndex;
	dispinfo.item.iSubItem = m_iColumnIndex;
	dispinfo.item.pszText = m_bESC ? LPTSTR((LPCTSTR)m_strWindowText) : LPTSTR((LPCTSTR)strEdit);
	dispinfo.item.cchTextMax = m_bESC ? m_strWindowText.GetLength() : strEdit.GetLength();
	
	GetParent()->SendMessage(WM_NOTIFY, GetParent()->GetDlgCtrlID(), (LPARAM)&dispinfo);

	SendMessage(WM_CLOSE);
}

/**********************************************************************/

BOOL CInPlaceEdit::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (WM_KEYDOWN == pMsg->message && (VK_ESCAPE == pMsg->wParam || VK_RETURN == pMsg->wParam))
	{
		if (VK_ESCAPE == pMsg->wParam)
		{
			m_bESC = TRUE;
		}

		GetParent()->SetFocus();
		return TRUE;
	}

	return CEdit::PreTranslateMessage(pMsg);
}

/**********************************************************************/

int CInPlaceEdit::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	// Set the proper font
	CFont* pFont = GetParent()->GetFont();
	SetFont(pFont);

	ShowWindow(SW_SHOW);
	SetWindowText(m_strWindowText);
	SetSel(0, -1);
	SetFocus();
		  
	return 0;
}

/**********************************************************************/

CInPlaceEdit* CInPlaceEdit::GetInstance()
{
	if(m_pInPlaceEdit == NULL)
	{
		m_pInPlaceEdit = new CInPlaceEdit;
	}
	return m_pInPlaceEdit;
}

/**********************************************************************/

void CInPlaceEdit::DeleteInstance()
{
	delete m_pInPlaceEdit;
	m_pInPlaceEdit = NULL;
}

/**********************************************************************/

BOOL CInPlaceEdit::ShowEditCtrl(DWORD dwStyle, const RECT &rCellRect, CWnd* pParentWnd, 
								UINT uiResourceID, int iRowIndex, int iColumnIndex,
								CString& rstrCurSelection)
{
	m_iRowIndex = iRowIndex;
	m_iColumnIndex = iColumnIndex;
	m_strWindowText = rstrCurSelection;
	m_bESC = FALSE;

	if (NULL == m_pInPlaceEdit->m_hWnd) 
	{
		return m_pInPlaceEdit->Create(dwStyle, rCellRect, pParentWnd, uiResourceID); 
	}	

	return TRUE;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/
CInPlaceEditText* CInPlaceEditText::m_pInPlaceEdit = NULL;
/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CInPlaceEditText::CInPlaceEditText() : CEdit()
{
	m_iRowIndex= -1;
	m_iColumnIndex = -1;
	m_bESC = FALSE;
}

/**********************************************************************/

CInPlaceEditText::~CInPlaceEditText()
{
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CInPlaceEditText, CEdit)
	//{{AFX_MSG_MAP(CInPlaceEditText)
	ON_WM_KILLFOCUS()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

void CInPlaceEditText::OnKillFocus(CWnd* pNewWnd) 
{
	CEdit::OnKillFocus(pNewWnd);

	// Get the text in the edit ctrl
	CString strEdit;
	GetWindowText(strEdit);

	// Send Notification to parent of edit ctrl
	LV_DISPINFO dispinfo;
	dispinfo.hdr.hwndFrom = GetParent()->m_hWnd;
	dispinfo.hdr.idFrom = GetDlgCtrlID();
	dispinfo.hdr.code = LVN_ENDLABELEDIT;

	dispinfo.item.mask = LVIF_TEXT;
	dispinfo.item.iItem = m_iRowIndex;
	dispinfo.item.iSubItem = m_iColumnIndex;
	dispinfo.item.pszText = m_bESC ? LPTSTR((LPCTSTR)m_strWindowText) : LPTSTR((LPCTSTR)strEdit);
	dispinfo.item.cchTextMax = m_bESC ? m_strWindowText.GetLength() : strEdit.GetLength();
	
	GetParent()->SendMessage(WM_NOTIFY, GetParent()->GetDlgCtrlID(), (LPARAM)&dispinfo);

	SendMessage(WM_CLOSE);
}

/**********************************************************************/

BOOL CInPlaceEditText::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (WM_KEYDOWN == pMsg->message && (VK_ESCAPE == pMsg->wParam || VK_RETURN == pMsg->wParam))
	{
		if (VK_ESCAPE == pMsg->wParam)
		{
			m_bESC = TRUE;
		}

		GetParent()->SetFocus();
		return TRUE;
	}

	return CEdit::PreTranslateMessage(pMsg);
}

/**********************************************************************/

int CInPlaceEditText::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	// Set the proper font
	CFont* pFont = GetParent()->GetFont();
	SetFont(pFont);

	ShowWindow(SW_SHOW);
	SetWindowText(m_strWindowText);
	SetSel(0, -1);
	SetFocus();
		  
	return 0;
}

/**********************************************************************/

CInPlaceEditText* CInPlaceEditText::GetInstance()
{
	if(m_pInPlaceEdit == NULL)
	{
		m_pInPlaceEdit = new CInPlaceEditText;
	}
	return m_pInPlaceEdit;
}

/**********************************************************************/

void CInPlaceEditText::DeleteInstance()
{
	delete m_pInPlaceEdit;
	m_pInPlaceEdit = NULL;
}

/**********************************************************************/

BOOL CInPlaceEditText::ShowEditCtrl(DWORD dwStyle, const RECT &rCellRect, CWnd* pParentWnd, 
								UINT uiResourceID, int iRowIndex, int iColumnIndex,
								CString& rstrCurSelection)
{
	m_iRowIndex = iRowIndex;
	m_iColumnIndex = iColumnIndex;
	m_strWindowText = rstrCurSelection;
	m_bESC = FALSE;

	if (NULL == m_pInPlaceEdit->m_hWnd) 
	{
		return m_pInPlaceEdit->Create(dwStyle, rCellRect, pParentWnd, uiResourceID); 
	}	

	return TRUE;
}

/**********************************************************************/
