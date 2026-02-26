#pragma once
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "PluAddOrCopyInfo.h"
/**********************************************************************/

class CPluRangeCopyDlg : public CSSDialog
{
public:
	CPluRangeCopyDlg( CPluAddOrCopyInfo& PluInfo, CWnd* pParent = NULL);  

	//{{AFX_DATA(CPluRangeCopyDlg)
	enum { IDD = IDD_PLU_RANGE_COPY };
	CButton	m_checkCreate;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPluRangeCopyDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CPluRangeCopyDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CPluAddOrCopyInfo& m_PluInfo;
};

/**********************************************************************/
