#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "..\CommonEcrStock\LocationSelector.h"
/**********************************************************************/

class CPluDeleteUnlinkedDlg : public CDialog
{
public:
	CPluDeleteUnlinkedDlg(CWnd* pParent = NULL);   

	//{{AFX_DATA(CPluDeleteUnlinkedDlg)
	enum { IDD = IDD_PLU_DELETE_UNLINKED };
	CSSComboBoxEx	m_comboDatabase;
	//}}AFX_DATA
	int m_nAction;

	//{{AFX_VIRTUAL(CPluDeleteUnlinkedDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL
	
protected:
	//{{AFX_MSG(CPluDeleteUnlinkedDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	int PluDeleteInternal();

private:
	CLocationSelector m_LocationSelector;
};

//{{AFX_INSERT_LOCATION}}

/**********************************************************************/
#endif
/**********************************************************************/
