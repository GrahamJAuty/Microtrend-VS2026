#pragma once
/******************************************************************************/
#include "resource.h"
/******************************************************************************/

class CGroupNoDlg : public CSSAutoShutdownDialog
{
public:
	CGroupNoDlg(int nGroupNo, CWnd* pParent = NULL);   
	virtual ~CGroupNoDlg();

// Dialog Data
	enum { IDD = IDD_GROUPNO };
	CEdit* GetEditGroupNo() { return GetEdit(IDC_EDIT_GROUPNO); }

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()

private:
	virtual BOOL CSSAutoShutdownPostInitDialog();

public:
	int m_nGroupNo;
};

/******************************************************************************/
