/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
#include "afxwin.h"
/**********************************************************************/
#pragma once

class CStockLevelsAccessDlg : public CDialog
{
	DECLARE_DYNAMIC(CStockLevelsAccessDlg)

public:
	CStockLevelsAccessDlg( bool bStock, CWnd* pParent = NULL);   
	virtual ~CStockLevelsAccessDlg();

	enum { IDD = IDD_STOCKLEVELS_ACCESS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX); 

	DECLARE_MESSAGE_MAP()

public:
	int m_nType;

private:
	bool m_bStock;
	virtual BOOL OnInitDialog();

protected:
	virtual void OnOK();

public:
	CButton m_radioReadWrite;
};

/**********************************************************************/
#endif
/**********************************************************************/
