#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CPMSHighlightDlg : public CDialog
{
	DECLARE_DYNAMIC(CPMSHighlightDlg)

public:
	CPMSHighlightDlg( int nHighlightMode, CWnd* pParent = NULL); 
	virtual ~CPMSHighlightDlg();

	enum { IDD = IDD_PMS_HIGHLIGHT };
	CButton m_checkCredit;
	CButton m_checkDebit;

public:
	int GetHighlightMode(){ return m_nHighlightMode; }

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()

private:
	int m_nHighlightMode;
};

/**********************************************************************/
#endif
/**********************************************************************/
#endif
/**********************************************************************/
