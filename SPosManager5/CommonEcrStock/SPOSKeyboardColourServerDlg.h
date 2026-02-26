#pragma once
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CSPOSKeyboardColourServerDlg : public CDialog
{
public:
	CSPOSKeyboardColourServerDlg( CSPOSKeyboardKeyRecord& KeyRecord, int nType, CWnd* pParent = NULL);  

	//{{AFX_DATA(CSPOSKeyboardColourServerDlg)
	enum { IDD = IDD_SPOS_KEYBOARD_COLOUR_SERVER };
	CSSComboBox	m_comboVertical;
	CSSComboBox	m_comboHorizontal;
	CSSComboBox	m_comboBold;
	CSSComboBox	m_comboFontSize;
	//}}AFX_DATA
	CStatic m_static1;
	CStatic m_static2;
	CStatic m_static3;
	CStatic m_static4;

	//{{AFX_VIRTUAL(CSPOSKeyboardColourServerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL
	CSPOSKeyboardColourButton m_buttonColour[65];

protected:
	//{{AFX_MSG(CSPOSKeyboardColourServerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelectFontSize();
	virtual void OnOK();
	afx_msg void OnSelectBold();
	afx_msg void OnSelectHorizontal();
	afx_msg void OnSelectVertical();
	afx_msg void OnButtonDefault();
	afx_msg void OnButtonReset();
	//}}AFX_MSG
	afx_msg long OnColourButtonClick ( WPARAM wIndex, LPARAM lParam );
	afx_msg long OnColourButtonDoubleClick ( WPARAM wIndex, LPARAM lParam );
	DECLARE_MESSAGE_MAP()
	
public:
	void SetForeground( int n );
	void SetBackground( int n );
	void SetFontSize( int n );
	void SetHAlign( int n );
	void SetVAlign( int n );
	void SetBold( int n );

private:
	void UpdatePreviewButton();
	void EditColour( int nColourId );

private:
	void FillFontCombo();
	void FillHorizontalCombo();
	void FillVerticalCombo();
	void FillBoldCombo();
	
public:
	int m_nForeground;
	int m_nBackground;
	int m_nFontSize;
	int m_nHAlign;
	int m_nVAlign;
	int m_nBold;

private:
	CSPOSKeyboardKeyRecord& m_KeyRecord;
	int m_nType;
	DWORD m_nButtonID[65];
};

//{{AFX_INSERT_LOCATION}}
/**********************************************************************/
