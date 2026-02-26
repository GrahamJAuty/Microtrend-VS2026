#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CPMSPrintUnpaidDlg : public CSSDialog
{
public:
	CPMSPrintUnpaidDlg( CWnd* pParent = NULL);   
	
	//{{AFX_DATA(CPMSPrintUnpaidDlg)
	enum { IDD = IDD_PMS_PRINT_UNPAID };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPMSPrintUnpaidDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL
	CButton m_checkIncludeTraining;
	BOOL m_bPending;
	BOOL m_bActive;
	BOOL m_bComplete;
	BOOL m_bCancelled;
	BOOL m_bPrintLines;
	BOOL m_bIncludeTraining;

protected:
	//{{AFX_MSG(CPMSPrintUnpaidDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonSave();
	virtual void OnOK();
	afx_msg void OnButtonDefault();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	const char* GetCSVLine();

private:
	void SetDefaults();

private:
	CString m_strFieldSelect;
};

/**********************************************************************/

class CCSVPMSPrintUnpaidKey : public CCSV
{
public:
	CCSVPMSPrintUnpaidKey ( const char* szCSVLine ) : CCSV(szCSVLine){}
	bool IncludePending(){ return GetBool(0); }
	bool IncludeActive(){ return GetBool(1); }
	bool IncludeComplete(){ return GetBool(2); }
	bool IncludeCancelled(){ return GetBool(3); }
	bool PrintLines() { return GetBool(4); }
	bool IncludeTraining() { return GetBool(5); }
};

/**********************************************************************/
#endif
/**********************************************************************/
#endif
/**********************************************************************/
