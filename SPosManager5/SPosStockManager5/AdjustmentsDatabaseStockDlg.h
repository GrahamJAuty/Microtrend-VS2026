#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "AdjustmentsEditStockDlg.h"
#include "ColumnManager.h"
#include "StocktakeCSVArray.h"
/**********************************************************************/

class CAdjustmentsDatabaseStockDlg : public CDialog
{
public:
	CAdjustmentsDatabaseStockDlg( const char* szReference1, const char* szReference2, int nSpIdx, CStocktakeCSVRecord* pStocktake, CWnd* pParent = NULL );   
	~CAdjustmentsDatabaseStockDlg();

	void SetPreviousSession( const char* sz ){ m_strPreviousSession = sz; }
	void SetAutoImportFile( const char* sz ){ m_strAutoImportFile = sz; }

	//{{AFX_DATA(CAdjustmentsDatabaseStockDlg)
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CAdjustmentsDatabaseStockDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CAdjustmentsDatabaseStockDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnSave();
	afx_msg void OnColumnRestore();
	afx_msg void OnColumnDefault();
	afx_msg void OnAdd();
	afx_msg void OnPreview();
	afx_msg void OnDelete();
	afx_msg void OnEdit();
	afx_msg void OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnListCacheHint(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnImportPlu();
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
	void ImportPluInternal( CString& strImportPath, int nAction, bool bAutoImport );
	void InitialImportPlu();

private:
	void AuditAdjustments ( COleDateTime date, bool bUpdateStock );
	void AuditAdjustmentsInternal ( COleDateTime date, bool bUpdateStock );
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
	CString m_strLastReason;
	int m_nSpIdx;
	int m_nSpNo;

private:
	CStringArray m_arrayAdjustments;
	CStringArray m_ImportFilesArray;
	CString m_strDisplayLine;
	CString m_strTitle;
	CString m_strPreviousSession;
	CString m_strAutoImportFile;

private:
	CAdjustmentsEditStockDlg* m_pAdjustmentsStockEditDlg;

private:
	CColumnManager m_ColumnManager;
	int m_nCacheIndex;
	CCSV m_CacheCSV;

private:
	CStocktakeCSVRecord* m_pStocktake;
	int m_nStkTakeNo;
};

/**********************************************************************/
#endif				//__AdjustmentsStockDatabaseDlg_H__
/**********************************************************************/
