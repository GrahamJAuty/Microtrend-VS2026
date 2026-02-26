#pragma once
/**********************************************************************/
#include "SPOSKeyboardColourButton.h"
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CSPOSKeyboardColourGlobalDlg : public CDialog
{
public:
	CSPOSKeyboardColourGlobalDlg( __int64 nVisibleListNo, CWnd* pParent = NULL);  

	//{{AFX_DATA(CSPOSKeyboardColourGlobalDlg)
	enum { IDD = IDD_SPOS_KEYBOARD_COLOUR_GLOBAL };
	CSSComboBox	m_comboVertical;
	CSSComboBox	m_comboHorizontal;
	CSSComboBox	m_comboBold;
	CSSComboBox	m_comboColour;
	CSSComboBox	m_comboFontSize;
	CSSComboBox m_comboHide;
	CSSComboBox m_comboDefault;
	CSSComboBox m_comboChanges;
	CButton m_checkItemStandard;
	CButton m_checkItemLarge;
	CButton m_checkFunctionStandard;
	CButton m_checkFunctionLarge;
	CButton m_checkBottomBar;
	BOOL	m_bControl;
	BOOL	m_bEmpty;
	BOOL	m_bFunction;
	BOOL	m_bKeyText;
	BOOL	m_bRemoveImage;
	BOOL	m_bList;
	BOOL	m_bMacro;
	BOOL	m_bModifier;
	BOOL	m_bPayment;
	BOOL	m_bPlu;
	BOOL	m_bServer;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CSPOSKeyboardColourGlobalDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL
	CSPOSKeyboardColourButton m_buttonColour[65];

protected:
	//{{AFX_MSG(CSPOSKeyboardColourGlobalDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelectFontSize();
	afx_msg void OnSelectColourMode();
	afx_msg void OnSelectChanges();
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
	afx_msg void OnButtonAll();
	afx_msg void OnButtonNone();
	
public:
	void SetForeground( int n );
	void SetBackground( int n );
	void SetFontSize( int n );
	void SetHAlign( int n );
	void SetVAlign( int n );
	void SetBold( int n );
	void SetHide( int n );
	void SetDefault( int n );

private:
	void UpdatePreviewButton();
	void EditColour( int nColourId );

private:
	void FillFontCombo();
	void FillColourCombo();
	void FillHorizontalCombo();
	void FillVerticalCombo();
	void FillBoldCombo();
	void FillHideCombo();
	void FillDefaultCombo();
	void SelectKeyTypes( bool b );

public:
	int m_nForeground;
	int m_nBackground;
	int m_nFontSize;
	int m_nHAlign;
	int m_nVAlign;
	int m_nBold;
	int m_nHide;
	int m_nDefault;

private:
	DWORD m_nButtonID[65];
	__int64 m_nVisibleListNo;
};

//{{AFX_INSERT_LOCATION}}
/**********************************************************************/
