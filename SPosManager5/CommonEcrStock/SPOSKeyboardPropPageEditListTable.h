#pragma once
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CSPOSKeyboardPropPageEditListTable : public CSSPropertyPage
{
public:
	CSPOSKeyboardPropPageEditListTable();
	~CSPOSKeyboardPropPageEditListTable();

	//{{AFX_DATA(CSPOSKeyboardPropPageEditListTable)
	enum { IDD = IDD_PROPPAGE_EDITLIST_TABLE };
	CSSComboBox	m_comboTextSize;
	CSSComboBox	m_comboVertical;
	CSSComboBox	m_comboHorizontal;
	int		m_nForeground;
	int		m_nBackground;
	BOOL	m_bBold;
	//}}AFX_DATA
	CSPOSKeyboardColourButton	m_buttonColour[65];

	//{{AFX_VIRTUAL(CSPOSKeyboardPropPageEditListTable)
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
	void ValidateKeySettings();
	
protected:
	//{{AFX_MSG(CSPOSKeyboardPropPageEditListTable)
	virtual BOOL OnInitDialog();
	afx_msg void OnKillFocusBackground();
	afx_msg void OnKillFocusForeground();
	afx_msg void OnSelectFontSize();
	afx_msg void OnCheckBold();
	afx_msg void OnSelectAlign();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void SaveRecord();

private:
	CSPOSKeyboardListRecord* m_pListRecord;

private:
	DWORD m_nButtonID[65];

private:
	CSPOSKeyboardKeyRecord m_KeyRecord;
	int m_nStartKeyIdx;
	int m_nEndKeyIdx;
};

/**********************************************************************/

