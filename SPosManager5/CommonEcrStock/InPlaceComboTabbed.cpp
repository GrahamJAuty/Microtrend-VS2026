/**********************************************************************/
#include "InPlaceComboTabbed.h"
/**********************************************************************/
CInPlaceComboTabbed* CInPlaceComboTabbed::m_pInPlaceCombo = NULL; 
/**********************************************************************/

CInPlaceComboTabbed::CInPlaceComboTabbed()
{
	m_iRowIndex = -1;
	m_iColIndex = -1;
	m_bESC = FALSE;
}

/**********************************************************************/

CInPlaceComboTabbed::~CInPlaceComboTabbed()
{
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CInPlaceComboTabbed, CSSTabbedComboBox)
	//{{AFX_MSG_MAP(CInPlaceComboTabbed)
	ON_WM_CREATE()
	ON_WM_KILLFOCUS()
	ON_WM_CHAR()
	ON_CONTROL_REFLECT(CBN_CLOSEUP, OnCloseup)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

int CInPlaceComboTabbed::OnCreate(LPCREATESTRUCT lpCreateStruct) 
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
	
	//for (POSITION Pos_ = m_DropDownList.GetHeadPosition(); Pos_ != NULL;)
	//{
	//	AddString((LPCTSTR) (m_DropDownList.GetNext(Pos_)));
	//}

	return 0;
}

/**********************************************************************/

BOOL CInPlaceComboTabbed::PreTranslateMessage(MSG* pMsg) 
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
	
	return CSSTabbedComboBox::PreTranslateMessage(pMsg);
}

/**********************************************************************/

void CInPlaceComboTabbed::OnKillFocus(CWnd* pNewWnd) 
{
	CSSTabbedComboBox::OnKillFocus(pNewWnd);
	
	CString str = "0";
	if ( FALSE == m_bESC )
	{
		int nSel = GetCurSel();
		if ( ( nSel >= 0 ) && ( nSel < GetStringCount() ) )
			str.Format( "%d", nSel + 1 );
	}

	// Send Notification to parent of ListView ctrl
	LV_DISPINFO dispinfo;
	dispinfo.hdr.hwndFrom = GetParent()->m_hWnd;
	dispinfo.hdr.idFrom = GetDlgCtrlID();
	dispinfo.hdr.code = LVN_ENDLABELEDIT;

	dispinfo.item.mask = LVIF_TEXT;
	dispinfo.item.iItem = m_iRowIndex;
	dispinfo.item.iSubItem = m_iColIndex;
	dispinfo.item.pszText = LPTSTR((LPCTSTR)str);
	dispinfo.item.cchTextMax = str.GetLength();
	
	GetParent()->SendMessage(WM_NOTIFY, GetParent()->GetDlgCtrlID(), (LPARAM)&dispinfo);

	// Close the control
	SendMessage(WM_CLOSE);
}

/**********************************************************************/

void CInPlaceComboTabbed::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
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
	
	CSSTabbedComboBox::OnChar(nChar, nRepCnt, nFlags);
}

/**********************************************************************/

void CInPlaceComboTabbed::OnCloseup() 
{
	// Set the focus to the parent list control
	GetParent()->SetFocus();
}

/**********************************************************************/

CInPlaceComboTabbed* CInPlaceComboTabbed::GetInstance()
{
	if(m_pInPlaceCombo == NULL)
	{
		m_pInPlaceCombo = new CInPlaceComboTabbed;
	}
	return m_pInPlaceCombo;
}

/**********************************************************************/

void CInPlaceComboTabbed::DeleteInstance()
{
	delete m_pInPlaceCombo;
	m_pInPlaceCombo = NULL;
}

/**********************************************************************/

BOOL CInPlaceComboTabbed::ShowComboCtrl(DWORD dwStyle, const CRect &rCellRect, CWnd* pParentWnd, UINT uiResourceID,
								  int iRowIndex, int iColIndex, CStringList* pDropDownList, 
								  int iCurSel /*= -1*/)
{
	m_iRowIndex = iRowIndex;
	m_iColIndex = iColIndex;
	m_bESC = FALSE;

	BOOL bRetVal = TRUE;

	if (NULL == m_pInPlaceCombo->m_hWnd) 
	{
		bRetVal = m_pInPlaceCombo->Create(dwStyle, rCellRect, pParentWnd, uiResourceID); 
	}

	bool bFirstLine = TRUE;
	for ( POSITION pos = pDropDownList -> GetHeadPosition(); pos != NULL;)	
	{
		CString strLine = pDropDownList -> GetNext( pos );

		if ( TRUE == bFirstLine )
		{
			CArray<int,int> arrayTabStops;
			
			CCSV csv( strLine );
			for ( int n = 0; ( n < csv.GetSize() ) && ( n < 8 ); n++ )
				arrayTabStops.Add( csv.GetInt(n) );

			SetTabStops( arrayTabStops );

			bFirstLine = FALSE;
		}
		else
		{
			AddItem( strLine );
		}
	}

	SetCurSel(iCurSel);

	return bRetVal;
}

/**********************************************************************/
