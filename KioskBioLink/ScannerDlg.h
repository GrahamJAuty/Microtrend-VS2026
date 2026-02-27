#pragma once
//*******************************************************************
#include "Cardlink.h"
//*******************************************************************

class CScannerDlg : public CSSColourDialog
{
	DECLARE_DYNAMIC(CScannerDlg)

public:
	CScannerDlg(CWnd* pParent);
	virtual ~CScannerDlg();

	enum { IDD = IDD_SCANNERDLG };
	CEdit m_editKbInput;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);  
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	CString m_strKbInput = "";

private:
	CSSColourStatic m_staticPrompt;
	CSSColourButton m_buttonCancel;
};

//*******************************************************************