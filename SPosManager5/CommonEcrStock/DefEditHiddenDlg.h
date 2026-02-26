#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
 
/**********************************************************************/
#include "PersistentSelectionEdit.h"
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CDefEditHiddenDlg : public CSSDialog
{
public:
	CDefEditHiddenDlg( CWnd* pParent = NULL);   

	//{{AFX_DATA(CDefEditHiddenDlg)
	enum { IDD = IDD_DEF_EDIT_HIDDEN };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDefEditHiddenDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL
	CPersistentSelectionEdit m_editHidden;

protected:
	//{{AFX_MSG(CDefEditHiddenDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	virtual void OnOK();

private:
	void AppendFlag(CString& strHidden, bool b, int nFlagNum);
	void AppendNum(CString& strHidden, int nNum, int nMin, int nMax, int nFlagNum);
	void ProcessFlag( CString& strFlag );
};

/**********************************************************************/
#endif
/**********************************************************************/
