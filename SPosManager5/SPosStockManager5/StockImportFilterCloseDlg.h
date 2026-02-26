#pragma once
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#define STOCKIMPORT_YES 0
#define STOCKIMPORT_NO 1
#define STOCKIMPORT_CANCEL 2
/**********************************************************************/

class CStockImportFilterCloseDlg : public CDialog
{
	DECLARE_DYNAMIC(CStockImportFilterCloseDlg)

public:
	CStockImportFilterCloseDlg( const char* szTitle, CWnd* pParent = NULL);   
	virtual ~CStockImportFilterCloseDlg();

	enum { IDD = IDD_STOCK_IMPORT_FILTER_CLOSE };

protected:
	virtual void OnOK();
	virtual void OnCancel();

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonYes();
	afx_msg void OnButtonNo();
	afx_msg void OnButtonCancel();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	DECLARE_MESSAGE_MAP()

public:
	int m_nAction;

private:
	CString m_strTitle;
};

/**********************************************************************/