#pragma once
//*******************************************************************
#include "resource.h"
//*******************************************************************

class CHotlistTraceDlg : public CDialog
{
public:
	CHotlistTraceDlg( __int64 nUserID, CWnd* pParent = NULL);  
	virtual ~CHotlistTraceDlg();
	enum { IDD = IDD_HOTLISTTRACE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg void OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()

public:
	__int64 m_nRequestedUserID;

private:
	CString CreateDisplayLine ( int nLine, __int64 nUserID, const char* szDate = "", const char* szTime = "" );

private:
	CArray<__int64, __int64> m_arrayUserIDs;
	CSSListCtrl	m_list;
};

//*******************************************************************
