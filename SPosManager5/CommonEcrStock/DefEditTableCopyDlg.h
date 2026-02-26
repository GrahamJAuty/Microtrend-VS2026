#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "ListDataDlg.h"
/**********************************************************************/

class CDefEditTableCopyDlg : public CSSDialog
{
public:
	CDefEditTableCopyDlg( int nIndex, CWnd* pParent = NULL);  

	//{{AFX_DATA(CDefEditTableCopyDlg)
	enum { IDD = IDD_DEF_EDIT_TABLE_COPY };
	
	CButton m_checkCopySharpPos;
	CButton m_checkAccount;
	CButton m_checkLimit;
	CButton m_checkAutoStatus1;
	CButton m_checkAutoStatus2;
	/*****/
	CButton m_checkCopyReport;
	CSSTabbedComboBox m_comboGroup[3];
	CButton m_checkFilter;
	/*****/
	CButton m_checkCreate;
	CButton m_checkModify;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDefEditTableCopyDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

private:
	CEdit* GetEditLimit();
	CEdit* GetEditTableStart();
	CEdit* GetEditTableEnd();

private:
	void GetData();
	bool CopyTable();
	void MergeTableSettings(CTableNameCSVRecord& Table, bool bCopySharpPos, bool bCopyReport);

protected:
	//{{AFX_MSG(CDefEditTableCopyDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnToggleCopySharpPos();
	afx_msg void OnToggleCopyReport();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
private:
	CTableNameCSVRecord m_TableRecord;
	int m_nIndex;

public:
	int m_nTablesCreated;
	int m_nTablesModified;
};

//{{AFX_INSERT_LOCATION}}
/**********************************************************************/
#endif
/**********************************************************************/
