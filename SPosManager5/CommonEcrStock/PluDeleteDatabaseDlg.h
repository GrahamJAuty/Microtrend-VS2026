#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "ColumnManager.h"
#include "..\CommonEcrStock\PluFilterArray.h"
/**********************************************************************/

class CPluDeleteDatabaseDlg : public CDialog
{
public:
#ifdef STOCKMAN_SYSTEM
	CPluDeleteDatabaseDlg( bool bDeleteItems, CWnd* pParent = NULL );   
#endif
	CPluDeleteDatabaseDlg( bool bDeleteItems, CByteArray& FlagArray, CWnd* pParent = NULL );   

	~CPluDeleteDatabaseDlg();

	//{{AFX_DATA(CPluDeleteDatabaseDlg)
	enum { IDD = IDD_PLU_DELETE_DATABASE };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPluDeleteDatabaseDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	//}}AFX_VIRTUAL

protected:
	afx_msg void OnContextMenu(CWnd*, CPoint point);
	//{{AFX_MSG(CPluDeleteDatabaseDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPluDeleteRange();
	afx_msg void OnDefaultButton();
	afx_msg void OnPluDeleteAll();
	afx_msg void OnPluDeleteNone();
	afx_msg void OnColumnSave();
	afx_msg void OnColumnRestore();
	afx_msg void OnColumnDefault();
	afx_msg void OnListCacheHint(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	afx_msg void OnPrint();
	virtual void OnCancel();
	afx_msg long OnUserButton ( WPARAM wIndex, LPARAM lParam );
	DECLARE_MESSAGE_MAP()

public:
	int GetFilterSize() { return m_FilterArray.GetSize(); }
	bool GetDoneDeleteFlag() { return m_bDoneDelete; }

private:
	void OnFindPluNumber();
	void DisplayScreen ( int nIndex );
	void ToggleFlag();
	void SelectAll ( int nFlag );
	void AddReportColumn( CReportFile& ReportFile, CCSV& csv, const char* szType, UINT nAlign, int nWidth );

private:
	void SortListInternal( int nType );

private:
	HICON m_hIcon;
	CString m_strDatabaseName;
	CPluFilterArray m_FilterArray;

private:
	bool m_bInitDialog;
	CSSListCtrlVirtual m_listPlu;	
	int m_nDeletionCount;

private:
	CString m_strTitle;
	bool m_bUnusedMode;
	bool m_bDeleteItems;

private:
	CColumnManager m_ColumnManager;
	int m_nCacheIndex;
	CCSV m_CacheCSV;

private:
	bool m_bDoneDelete;
};

/**********************************************************************/
#endif
/**********************************************************************/
