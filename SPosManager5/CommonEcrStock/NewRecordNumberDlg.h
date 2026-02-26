#pragma once
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CNewRecordNumberDlg : public CSSDialog
{
public:
	CNewRecordNumberDlg( int nType, CWnd* pParent = NULL);   
	
	//{{AFX_DATA(CNewRecordNumberDlg)
	enum { IDD = IDD_NEWRECORD_NUMBER };
	CStatic	m_staticLabel;
	int		m_nNumber;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CNewRecordNumberDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);  
	//}}AFX_VIRTUAL

public:
	void SetCopySource( int n ){ m_nCopySource = n; }

protected:
	//{{AFX_MSG(CNewRecordNumberDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CEdit* GetEditRecordNo();

private:
	int m_nType;
	int m_nCopySource;
};

/**********************************************************************/
