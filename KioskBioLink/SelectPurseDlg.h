#pragma once
//*********************************************************************
#include "Cardlink.h"
//*********************************************************************

class CSelectPurseDlg : public CSSColourDialog
{
	DECLARE_DYNAMIC(CSelectPurseDlg)

public:
	CSelectPurseDlg(CWnd* pParent); 
	virtual ~CSelectPurseDlg();

	enum { IDD = IDD_SELECTPURSEDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonTopup1();
	afx_msg void OnButtonTopup2();

	DECLARE_MESSAGE_MAP()

public:
	int m_nTopupPurse = -1;

private:
	CSSColourStatic m_staticPrompt;
	CSSColourButton m_buttonTopup1;
	CSSColourButton m_buttonTopup2;
	CSSColourButton m_buttonCancel;
};

//*********************************************************************