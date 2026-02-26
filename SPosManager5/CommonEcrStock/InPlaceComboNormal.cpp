/**********************************************************************/
 
/**********************************************************************/
#include "InPlaceComboNormal.h"
/**********************************************************************/
CInPlaceComboNormal* CInPlaceComboNormal::m_pInPlaceCombo = NULL; 
/**********************************************************************/

CInPlaceComboNormal::CInPlaceComboNormal()
{
	m_iRowIndex = -1;
	m_iColumnIndex = -1;
	m_bESC = FALSE;
}

/**********************************************************************/

CInPlaceComboNormal::~CInPlaceComboNormal()
{
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CInPlaceComboNormal, CSSComboBox)
	//{{AFX_MSG_MAP(CInPlaceComboNormal)
	ON_WM_CREATE()
	ON_WM_KILLFOCUS()
	ON_WM_CHAR()
	ON_CONTROL_REFLECT(CBN_CLOSEUP, OnCloseup)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

int CInPlaceComboNormal::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CComboBox::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}
	
	// Set the proper font
	CFont* pFont = GetParent()->GetFont();
	SetFont(pFont);
	
	SetFocus();

	ResetContent(); 
	for (POSITION Pos_ = m_DropDownList.GetHeadPosition(); Pos_ != NULL;)
	{
		AddString((LPCTSTR) (m_DropDownList.GetNext(Pos_)));
	}

	return 0;
}

/**********************************************************************/

BOOL CInPlaceComboNormal::PreTranslateMessage(MSG* pMsg) 
{
	// If the message if for "Enter" or "Esc"
	// Do not process
	if (pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
		{
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);
			// DO NOT process further
			return TRUE;				
		}
	}
	
	return CSSComboBox::PreTranslateMessage(pMsg);
}

/**********************************************************************/

void CInPlaceComboNormal::OnKillFocus(CWnd* pNewWnd) 
{
	CSSComboBox::OnKillFocus(pNewWnd);
	
	CString str = "0";
	if ( FALSE == m_bESC )
	{
		int nSel = GetCurSel();
		if ( ( nSel >= 0 ) && ( nSel < GetCount() ) )
			str.Format( "%d", nSel + 1 );
	}

	// Send Notification to parent of ListView ctrl
	LV_DISPINFO dispinfo;
	dispinfo.hdr.hwndFrom = GetParent()->m_hWnd;
	dispinfo.hdr.idFrom = GetDlgCtrlID();
	dispinfo.hdr.code = LVN_ENDLABELEDIT;

	dispinfo.item.mask = LVIF_TEXT;
	dispinfo.item.iItem = m_iRowIndex;
	dispinfo.item.iSubItem = m_iColumnIndex;
	dispinfo.item.pszText = LPTSTR((LPCTSTR)str);
	dispinfo.item.cchTextMax = str.GetLength();
	
	GetParent()->SendMessage(WM_NOTIFY, GetParent()->GetDlgCtrlID(), (LPARAM)&dispinfo);

	// Close the control
	SendMessage(WM_CLOSE);
}

/**********************************************************************/

void CInPlaceComboNormal::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// If the key is "Esc" set focus back to the list control
	if (nChar == VK_ESCAPE || nChar == VK_RETURN)
	{
		if (nChar == VK_ESCAPE)
		{
			m_bESC = TRUE;
		}

		GetParent()->SetFocus();
		return;
	}
	
	CSSComboBox::OnChar(nChar, nRepCnt, nFlags);
}

/**********************************************************************/

void CInPlaceComboNormal::OnCloseup() 
{
	// Set the focus to the parent list control
	GetParent()->SetFocus();
}

/**********************************************************************/

CInPlaceComboNormal* CInPlaceComboNormal::GetInstance()
{
	if(m_pInPlaceCombo == NULL)
	{
		m_pInPlaceCombo = new CInPlaceComboNormal;
	}
	return m_pInPlaceCombo;
}

/**********************************************************************/

void CInPlaceComboNormal::DeleteInstance()
{
	delete m_pInPlaceCombo;
	m_pInPlaceCombo = NULL;
}

/**********************************************************************/

BOOL CInPlaceComboNormal::ShowComboCtrl(DWORD dwStyle, const CRect &rCellRect, CWnd* pParentWnd, UINT uiResourceID,
								  int iRowIndex, int iColumnIndex, CStringList* pDropDownList, int iCurSel /*= -1*/)
{
	m_iRowIndex = iRowIndex;
	m_iColumnIndex = iColumnIndex;
	m_bESC = FALSE;
	
	m_DropDownList.RemoveAll(); 
	m_DropDownList.AddTail(pDropDownList);

	BOOL bRetVal = TRUE;

	if (NULL == m_pInPlaceCombo->m_hWnd) 
	{
		bRetVal = m_pInPlaceCombo->Create(dwStyle, rCellRect, pParentWnd, uiResourceID); 
	}

	SetCurSel(iCurSel);

	return bRetVal;
}

/**********************************************************************/
