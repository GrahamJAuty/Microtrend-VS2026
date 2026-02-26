#pragma once
/**********************************************************************/
#ifndef STOCKMAN_SYSTEM
/**********************************************************************/
 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CPluApparentDlg : public CSSDialog
{
public:
	CPluApparentDlg(CWnd* pParent = NULL);   

	//{{AFX_DATA(CPluApparentDlg)
	enum { IDD = IDD_PLU_APPARENT };
	double	m_dApparent;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPluApparentDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CPluApparentDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/**********************************************************************/
#endif
/**********************************************************************/


