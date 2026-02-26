#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "ColumnManager.h"
#include "ReturnsEditDlg.h"
#include "StocktakeCSVArray.h"
#include "SuppRefFinder.h"
/**********************************************************************/

class CReturnsDatabaseDlg : public CDialog
{
public:
	CReturnsDatabaseDlg( const char* szReference1, const char* szReference2, int nSuppIdx, int nSpIdx, CStocktakeCSVRecord* pStocktake, CWnd* pParent = NULL );   
	~CReturnsDatabaseDlg();

	void SetPreviousSession( const char* sz ){ m_strPreviousSession = sz; }
	void SetAutoImportFile( const char* sz ){ m_strAutoImportFile = sz; }
	
	//{{AFX_DATA(CReturnsDatabaseDlg)
	enum { IDD = IDD_RETURNS_DATABASE };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CReturnsDatabaseDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CReturnsDatabaseDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnSave();
	afx_msg void OnColumnRestore();
	afx_msg void OnColumnDefault();
	afx_msg void OnImportPlu();
	afx_msg void OnImportWrapper();
	afx_msg void OnAdd();
	afx_msg void OnPreview();
	afx_msg void OnDelete();
	afx_msg void OnEdit();
	afx_msg void OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnListCacheHint(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	virtual void OnCancel();
	afx_msg long OnUserButton ( WPARAM wIndex, LPARAM lParam );
	afx_msg long OnProcessReturn ( WPARAM wIndex, LPARAM lParam );
	afx_msg long OnInitialAddItem ( WPARAM wIndex, LPARAM lParam );
	afx_msg long OnInitialImportPlu ( WPARAM wIndex, LPARAM lParam );
	DECLARE_MESSAGE_MAP()

private:
	void DisplayScreen ( int nIndex );
	const char* GetDisplayLine( int nIndex );
	void ImportPluInternal( CString& strImportPath, int nAction, bool bAutoImport );
	void ImportWrapperInternal( CString& strImportPath, int nAction );
	void InitialImportPlu();

private:
	void AuditReturns ( COleDateTime date, bool bUpdateStock );
	void AuditReturnsInternal ( COleDateTime date, bool bUpdateStock );
	void WriteSessionFile( bool bDetail, bool bAutoImport );

private:
	void UpdateAutoReferenceNo();
	void DeleteImportFiles();
	void DeleteSessionFile();

private:
	HICON m_hIcon;
	bool m_bInitDialog;
	CSSListCtrlVirtual m_listStock;	

private:
	CString m_strReference1;
	CString m_strReference2;
	CString m_strSupplier;
	int m_nSuppIdx;
	int m_nSuppNo;
	int m_nSpIdx;
	int m_nSpNo;

private:
	CStringArray m_arrayReturns;
	CStringArray m_ImportFilesArray;
	CString m_strDisplayLine;
	CString m_strTitle;
	CString m_strPreviousSession;
	CString m_strAutoImportFile;

private:
	CSuppRefFinder m_SuppRefFinder;

private:
	CReturnsEditDlg* m_pReturnsEditDlg;

private:
	CColumnManager m_ColumnManager;
	int m_nCacheIndex;
	CCSV m_CacheCSV;

private:
	CStocktakeCSVRecord* m_pStocktake;
	int m_nStkTakeNo;
};

/**********************************************************************/
#endif				//__RETURNSDATABASEDLG_H__
/**********************************************************************/
