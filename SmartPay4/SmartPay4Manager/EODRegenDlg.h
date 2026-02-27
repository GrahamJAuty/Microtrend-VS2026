#pragma once
//*******************************************************************
#include "resource.h"
//*******************************************************************

class CEODRegenDlg : public CSSAutoShutdownDialog
{
public:
	CEODRegenDlg( CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	enum { IDD = IDD_ENDOFDAY_REGEN };
	int m_nRegenType;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
};

//*******************************************************************
