#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "ColourButton.h"
#include "SharedResources.h"
/**********************************************************************/

class CColourEditDlg : public CSSDialog
{
public:
	CColourEditDlg( int nColourRef, const char* szTitle, CWnd* pMainWnd, bool bKeyboardEditor, CWnd* pParent ); 

	//{{AFX_DATA(CColourEditDlg)
	enum { IDD = IDD_COLOUR_EDIT };
	CSliderCtrl	m_sliderRed;
	CSliderCtrl	m_sliderGreen;
	CSliderCtrl	m_sliderBlue;
	int		m_nBlue;
	int		m_nGreen;
	int		m_nRed;
	//}}AFX_DATA
	CColourButtonBase m_buttonColour[18];

// Overrides
	//{{AFX_VIRTUAL(CColourEditDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	//}}AFX_VIRTUAL
	afx_msg long OnColourButtonMessage ( WPARAM wIndex, LPARAM lParam );

public:
	void SetSuggestedColour( int nID, int nColour );
	int GetColourRef();

protected:
	//{{AFX_MSG(CColourEditDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnKillFocusColour();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void UpdateSliderRed( int nPos );
	void UpdateSliderGreen( int nPos );
	void UpdateSliderBlue( int nPos );
	void SendColourMessage();

private:
	DWORD m_nButtonID[18];
	int m_nColourRef;
	CString m_strTitle;
	CWnd* m_pMainWnd;
	bool m_bKeyboardEditor;
};

//{{AFX_INSERT_LOCATION}}
/**********************************************************************/
#endif 
/**********************************************************************/

