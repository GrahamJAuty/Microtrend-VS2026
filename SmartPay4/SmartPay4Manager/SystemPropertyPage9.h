#pragma once
/**********************************************************************/
#include "ColourButton.h"
/**********************************************************************/
#include "Resource.h"
/**********************************************************************/

class CSystemPropertyPage9 : public CSSAutoShutdownPropertyPage
{
	DECLARE_DYNCREATE(CSystemPropertyPage9)

public:
	CSystemPropertyPage9();
	~CSystemPropertyPage9();

public:
	//{{AFX_DATA(CSystemPropertyPage9)
	enum { IDD = IDD_PROPPAGE_SYSTEM9 };
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

	//{{AFX_VIRTUAL(CSystemPropertyPage9)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CSystemPropertyPage9)
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

