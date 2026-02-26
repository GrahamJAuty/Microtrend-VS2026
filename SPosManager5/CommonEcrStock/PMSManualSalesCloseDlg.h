#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#define PMSSALES_CONFIRM 0
#define PMSSALES_CANCEL 1
#define PMSSALES_CONTINUE 2
/**********************************************************************/

class CPMSManualSalesCloseDlg : public CDialog
{
	DECLARE_DYNAMIC(CPMSManualSalesCloseDlg)

public:
	CPMSManualSalesCloseDlg( int nPMSActionType, CWnd* pParent = NULL);   
	virtual ~CPMSManualSalesCloseDlg();

	enum { IDD = IDD_PMS_MANUAL_SALES_CLOSE };

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
	CButton m_buttonConfirm;
	CButton m_buttonCancel;

public:
	int m_nPMSActionType;
	int m_nAction;
};

/**********************************************************************/
#endif
/**********************************************************************/
#endif
/**********************************************************************/
