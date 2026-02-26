#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CDefaultCategoryDlg : public CDialog
{
public:
	CDefaultCategoryDlg(CWnd* pParent = NULL);   

	//{{AFX_DATA(CDefaultCategoryDlg)
	enum { IDD = IDD_DEFAULT_CATEGORY };
	int		m_nDefType;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDefaultCategoryDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDefaultCategoryDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/**********************************************************************/
#endif
/**********************************************************************/

