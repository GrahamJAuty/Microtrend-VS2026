#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "AdjustmentsEditPluDlg.h"
#include "ColumnManager.h"
#include "StocktakeCSVArray.h"
/**********************************************************************/

class CAdjustmentsDatabasePluDlg : public CSSDialog
{
public:
	CAdjustmentsDatabasePluDlg( const char* szReference1, const char* szReference2, int nSpIdx, CStocktakeCSVRecord* pStocktake, CWnd* pParent = NULL );   
	~CAdjustmentsDatabasePluDlg();

	void SetPreviousSession( const char* sz ){ m_strPreviousSession = sz; }
	void SetAutoImportFile( const char* sz ){ m_strAutoImportFile = sz; }

	//{{AFX_DATA(CAdjustmentsDatabasePluDlg)
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CAdjustmentsDatabasePluDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CAdjustmentsDatabasePluDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnSave();
	afx_msg void OnColumnRestore();
	afx_msg void OnColumnDefault();
	afx_msg void OnImport();
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
	afx_msg long OnProcessAdjustment ( WPARAM wIndex, LPARAM lParam );
	afx_msg long OnInitialAddItem ( WPARAM wIndex, LPARAM lParam );
	afx_msg long OnInitialImportPlu ( WPARAM wIndex, LPARAM lParam );
	DECLARE_MESSAGE_MAP()

private:
	void DisplayScreen ( int nIndex );
	const char* GetDisplayLine( int nIndex );
	
private:
	void AuditAdjustments ( COleDateTime date, bool bUpdateStock );
	void AuditAdjustmentsInternal ( COleDateTime date, bool bUpdateStock );
	void WriteSessionFile( bool bDetail, bool bAutoImport );
	void ImportPluInternal( CString& strImportPath, int nAction, bool bAutoImport );
	void InitialImportPlu();
	
private:
	void UpdateAutoReferenceNo();
	void DeleteImportFiles();
	void DeleteSessionFile();

private:
	HICON m_hIcon;
	bool m_bInitDialog;
	CSSListCtrlVirtual m_listStock;	

private:
	int m_nSpIdx;
	int m_nSpNo;

private:
	CStringArray m_arrayAdjustments;
	CStringArray m_ImportFilesArray;
	CString m_strDisplayLine;
	CString m_strTitle;
	CString m_strPreviousSession;
	CString m_strAutoImportFile;
	CString m_strReference1;
	CString m_strReference2;
	CString m_strLastReason;
	
private:
	CAdjustmentsEditPluDlg* m_pAdjustmentsStockEditDlg;

private:
	CColumnManager m_ColumnManager;
	int m_nCacheIndex;
	CCSV m_CacheCSV;

private:
	CStocktakeCSVRecord* m_pStocktake;
	int m_nStkTakeNo;
};

/**********************************************************************/
#endif			
/**********************************************************************/
