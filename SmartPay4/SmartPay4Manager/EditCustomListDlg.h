#pragma once
#include "SmartPay4Manager.h"

// CEditCustomListDlg dialog

class CEditCustomListDlg : public CSSAutoShutdownDialog
{
public:
	CEditCustomListDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEditCustomListDlg();

// Dialog Data
	enum { IDD = IDD_EDITCUSTOMLIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_strCustomField;
	CString m_strFieldName;
};
