#pragma once
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#define STOCKDELETE_CONFIRM 0
#define STOCKDELETE_CANCEL 1
#define STOCKDELETE_CONTINUE 2
/**********************************************************************/

class CStockDeleteCloseDlg : public CDialog
{
	DECLARE_DYNAMIC(CStockDeleteCloseDlg)

public:
	CStockDeleteCloseDlg( int nType, int nCount, CWnd* pParent = NULL);   
	virtual ~CStockDeleteCloseDlg();

	enum { IDD = IDD_STOCK_DELETE_CLOSE };

protected:
	virtual void OnOK();
	virtual void OnCancel();

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonConfirm();
	afx_msg void OnButtonCancel();
	afx_msg void OnButtonContinue();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	DECLARE_MESSAGE_MAP()

private:
	CStatic m_static1;
	CStatic m_static2;
	CStatic m_static3;
	CStatic m_static4;
	CButton m_buttonConfirm;
	CButton m_buttonCancel;

public:
	int m_nAction;

private:
	int m_nType;
	int m_nCount;
};

/**********************************************************************/

