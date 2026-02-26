#pragma once
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CSPOSKeyboardPropPageEditListServer : public CSSPropertyPage
{
public:
	CSPOSKeyboardPropPageEditListServer();
	~CSPOSKeyboardPropPageEditListServer();

	//{{AFX_DATA(CSPOSKeyboardPropPageEditListServer)
	enum { IDD = IDD_PROPPAGE_EDITLIST_SERVER };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CSPOSKeyboardPropPageEditListServer)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL
	afx_msg long OnColourButtonClick ( WPARAM wIndex, LPARAM lParam );
	afx_msg void OnSwitchBackground();
	CSPOSKeyboardColourButton	m_buttonColour[7];
	
public:
	void SetListRecord( CSPOSKeyboardListRecord* pRecord );
	void RedrawButton( int n );
	
protected:
	//{{AFX_MSG(CSPOSKeyboardPropPageEditListServer)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void SaveRecord();

private:
	CSPOSKeyboardListRecord* m_pListRecord;
	CSPOSKeyboardKeyRecord m_KeyRecord[4];
	DWORD m_nButtonID[7];

private:
	int m_nBackground;
	int m_nStartKeyIdx;
	int m_nEndKeyIdx;
};

/**********************************************************************/

