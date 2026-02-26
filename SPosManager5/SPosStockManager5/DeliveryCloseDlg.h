#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#define SESSION_FINALISE 0
#define SESSION_FILE 1
#define SESSION_DISCARD 2
#define SESSION_ABANDON 3
#define SESSION_CONTINUE 4
/**********************************************************************/

class CDeliveryCloseDlg : public CDialog
{
	DECLARE_DYNAMIC(CDeliveryCloseDlg)

public:
	CDeliveryCloseDlg( int nType, bool bStocktake, CWnd* pParent = NULL);   
	virtual ~CDeliveryCloseDlg();

	enum { IDD = IDD_DELIVERY_CLOSE };

protected:
	virtual void OnOK();
	virtual void OnCancel();

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonFinalise();
	afx_msg void OnButtonFile();
	afx_msg void OnButtonDiscard();
	afx_msg void OnButtonAbandon();
	afx_msg void OnButtonContinue();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	DECLARE_MESSAGE_MAP()
	CButton m_buttonFile;
	CButton m_buttonFinalise;
	CButton m_buttonDiscard;
	CButton m_buttonAbandon;

public:
	int m_nAction;

private:
	int m_nType;
	bool m_bStocktake;
};

/**********************************************************************/
#endif
/**********************************************************************/

