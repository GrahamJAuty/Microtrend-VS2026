#pragma once
//****************************************************************************
#include "cardlink.h"
#include "GTData.h"
//****************************************************************************

class CEmptyCashDlg : public CSSColourDialog
{
	DECLARE_DYNAMIC(CEmptyCashDlg)

public:
	CEmptyCashDlg ( CGTData* pGTData, CWnd* pParent);
	virtual ~CEmptyCashDlg();

	enum { IDD = IDD_EMPTYCASH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonYes();
	afx_msg void OnButtonNo();
	DECLARE_MESSAGE_MAP()

public:
	int m_nReply = IDNO;
	int m_nTotalValue = 0;

private:
	CGTData* m_pGTData = NULL;

	CSSColourButton m_buttonYes;
	CSSColourButton m_buttonNo;
	CSSColourButton m_buttonCancel;
	CSSColourStatic m_staticPrompt;

	CSSColourStatic m_staticText2000;
	CSSColourStatic m_staticText1000;
	CSSColourStatic m_staticText500;
	CSSColourStatic m_staticText200;
	CSSColourStatic m_staticText100;
	CSSColourStatic m_staticText50;
	CSSColourStatic m_staticText20;
	CSSColourStatic m_staticText10;

	CSSColourStatic m_staticQty2000;
	CSSColourStatic m_staticQty1000;
	CSSColourStatic m_staticQty500;
	CSSColourStatic m_staticQty200;
	CSSColourStatic m_staticQty100;
	CSSColourStatic m_staticQty50;
	CSSColourStatic m_staticQty20;
	CSSColourStatic m_staticQty10;

	CSSColourStatic m_static2000;
	CSSColourStatic m_static1000;
	CSSColourStatic m_static500;
	CSSColourStatic m_static200;
	CSSColourStatic m_static100;
	CSSColourStatic m_static50;
	CSSColourStatic m_static20;
	CSSColourStatic m_static10;
	CSSColourStatic m_staticTotalValue;
	CSSColourStatic m_staticTotalText;
};

//****************************************************************************
