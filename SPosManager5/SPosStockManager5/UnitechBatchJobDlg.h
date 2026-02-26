#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CUnitechBatchJobDlg : public CDialog
{
public:
	CUnitechBatchJobDlg( int nAction, int nIdx, CWnd* pParent = NULL );   

	//{{AFX_DATA(CUnitechBatchJobDlg)
	CButton	m_checkUploadPlu;
	CButton	m_checkDownloadPlu;
	CButton	m_checkUploadStock;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CUnitechBatchJobDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CUnitechBatchJobDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	int m_nAction;
	int m_nDbIdx;
	int m_nSpIdx;
};

/**********************************************************************/
#endif
/**********************************************************************/
