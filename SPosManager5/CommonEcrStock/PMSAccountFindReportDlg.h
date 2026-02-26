#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "PMSAccountFindDlg.h"
/**********************************************************************/

class CPMSAccountFindReportDlg : public CSSDialog
{
public:
	CPMSAccountFindReportDlg( CArray<CPMSAccountFindInfo,CPMSAccountFindInfo>& arrayInfo, CArray<int,int>& arrayFilter, bool bUseFilter, int nType, CWnd* pParent = NULL);   
	
	//{{AFX_DATA(CPMSAccountFindReportDlg)
	enum { IDD = IDD_PMS_ACCOUNT_FIND_REPORT };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPMSAccountFindReportDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL
	int m_nSortType;
	BOOL m_bRoom;
	BOOL m_bLines;
	BOOL m_bBalance;
	CButton m_radioInvoice;
	
protected:
	//{{AFX_MSG(CPMSAccountFindReportDlg)
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
	CArray<CPMSAccountFindInfo,CPMSAccountFindInfo>& m_arrayInfo;
	CString m_strFieldSelect;

private:
	CArray<int,int>& m_arrayFilter;
	bool m_bUseFilter;

private:
	int m_nType;
	int m_nSubType;
	bool m_bAllowInvoiceSort;
};

/**********************************************************************/

class CCSVPMSAccountFindKey : public CCSV
{
public:
	CCSVPMSAccountFindKey ( const char* szCSVLine ) : CCSV(szCSVLine){}
	int GetSortType() { return GetInt(0); }
	bool GetRoomFlag() { return GetBool(1); }
	bool GetBalanceFlag() { return GetBool(2); }
	bool GetLinesFlag() { return GetBool(3); }
};

/**********************************************************************/
#endif
/**********************************************************************/
#endif
/**********************************************************************/
