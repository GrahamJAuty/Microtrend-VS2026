#pragma once
//*******************************************************************
#include "resource.h"
//*******************************************************************

class CGroupNoDlg : public CDialog
{
public:
	CGroupNoDlg( bool bScheme, int nGroupNo, CWnd* pParent = NULL);
	virtual ~CGroupNoDlg() {}

// Dialog Data
	enum { IDD = IDD_GROUPNO };
	CStatic m_staticPrompt;
	int m_nGroupNo;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

private:
	bool m_bScheme;
};

/**********************************************************************/

