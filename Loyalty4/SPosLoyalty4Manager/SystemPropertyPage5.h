#pragma once
/**********************************************************************/
#include "ColourButton.h"
/**********************************************************************/
#include "Resource.h"
/**********************************************************************/

class CSystemPropertyPage5 : public CSSPropertyPage
{
	DECLARE_DYNCREATE(CSystemPropertyPage5)

public:
	CSystemPropertyPage5();
	~CSystemPropertyPage5();

public:
	//{{AFX_DATA(CSystemPropertyPage5)
	enum { IDD = IDD_PROPPAGE_SYSTEM5 };
	CButton m_checkMessage;
	CStatic m_staticMessage1;
	CColourButtonBase m_buttonColourMessageBack;
	CStatic m_staticMessage2;
	CColourButtonBase m_buttonColourMessageText;
	CStatic m_staticMessage3;
	CSSComboBox m_comboMessageFont;
	CStatic m_staticMessage4;
	CSSComboBox m_comboMessageSpace;
	CButton m_buttonMessagePreview;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CSystemPropertyPage5)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CSystemPropertyPage5)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonMessageBack();
	afx_msg void OnButtonMessageText();
	afx_msg void OnButtonMessagePreview();
	afx_msg void ToggleEnables();
	//}}AFX_MSG
	afx_msg long OnColourButtonMessage( WPARAM wIndex, LPARAM lParam );
	DECLARE_MESSAGE_MAP()
	
private:
	void SaveRecord();
	void GetRecordData();
	void SetRecordControls();

private:
	int m_nEditingColour;
};

/**********************************************************************/

