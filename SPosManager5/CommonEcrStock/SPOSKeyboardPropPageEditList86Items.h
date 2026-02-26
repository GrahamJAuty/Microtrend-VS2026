#pragma once
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "SPOSKeyboardColourButton.h"
/**********************************************************************/

class CSPOSKeyboardPropPageEditList86Items : public CSSPropertyPage
{
public:
	CSPOSKeyboardPropPageEditList86Items();
	~CSPOSKeyboardPropPageEditList86Items();

	//{{AFX_DATA(CSPOSKeyboardPropPageEditList86Items)
	enum { IDD = IDD_PROPPAGE_EDITLIST_86ITEMS };
	CSSComboBox	m_comboTextSize;
	int		m_nForeground;
	int		m_nBackground;
	int		m_nHeight;
	int		m_nWidth;
	BOOL	m_bNotTransparent;
	BOOL	m_bBold;
	BOOL	m_bBorder;
	//}}AFX_DATA
	CSPOSKeyboardColourButton	m_buttonColour[65];

	//{{AFX_VIRTUAL(CSPOSKeyboardPropPageEditList86Items)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL
	afx_msg long OnColourButtonClick ( WPARAM wIndex, LPARAM lParam );
	
public:
	void SetListRecord( CSPOSKeyboardListRecord* pRecord );

private:
	CEdit* GetEditBackground();

private:
	void ValidateKeySettings();
	
protected:
	//{{AFX_MSG(CSPOSKeyboardPropPageEditList86Items)
	virtual BOOL OnInitDialog();
	afx_msg void OnKillFocusBackground();
	afx_msg void OnKillFocusForeground();
	afx_msg void OnKillFocus86Size();
	afx_msg void OnCheckTrans();
	afx_msg void OnSelectFontSize();
	afx_msg void OnCheckBold();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void SaveRecord();

private:
	CSPOSKeyboardListRecord* m_pListRecord;

private:
	DWORD m_nButtonID[65];
};

/**********************************************************************/

