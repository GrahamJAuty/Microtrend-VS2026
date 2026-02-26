#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "AdhocDeliveryEditDlg.h"
#include "ColumnManager.h"
#include "StocktakeCSVArray.h"
#include "SuppRefFinder.h"
/**********************************************************************/

class CAdhocDeliveryDatabaseDlg : public CDialog
{
public:
	CAdhocDeliveryDatabaseDlg( const char* szReference1, const char* szReference2, int nSuppIdx, int nSpIdx, CStocktakeCSVRecord* pStocktake, CWnd* pParent = NULL );   
	~CAdhocDeliveryDatabaseDlg();

	void SetPreviousSession( const char* sz ){ m_strPreviousSession = sz; }
	void SetAutoImportPluFile( const char* sz ){ m_strAutoImportPluFile = sz; }
	void SetAutoImportWrapperFile( const char* sz ){ m_strAutoImportWrapperFile = sz; }
	
	//{{AFX_DATA(CAdhocDeliveryDatabaseDlg)
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CAdhocDeliveryDatabaseDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CAdhocDeliveryDatabaseDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnSave();
	afx_msg void OnColumnRestore();
	afx_msg void OnColumnDefault();
	afx_msg void OnImportPlu();
	afx_msg void OnAdd();
	afx_msg void OnPreview();
	afx_msg void OnDelete();
	afx_msg void OnEdit();
	afx_msg void OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnListCacheHint(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnImportWrapper();
	//}}AFX_MSG
	virtual void OnCancel();
	afx_msg long OnUserButton ( WPARAM wIndex, LPARAM lParam );
	afx_msg long OnProcessDeliveryItem ( WPARAM wIndex, LPARAM lParam );
	afx_msg long OnInitialAddItem ( WPARAM wIndex, LPARAM lParam );
	afx_msg long OnInitialImportPlu ( WPARAM wIndex, LPARAM lParam );
	afx_msg long OnInitialImportWrapper ( WPARAM wIndex, LPARAM lParam );
	DECLARE_MESSAGE_MAP()

private:
	void DisplayScreen ( int nIndex );
	const char* GetDisplayLine( int nIndex );
	void ImportPluInternal( CString& strImportPath, int nAction, bool bAutoImport );
	void ImportWrapperInternal( CString& strImportPath, int nAction, bool bAutoImport );
	void InitialImportPlu();
	void InitialImportWrapper();

private:
	void AuditDeliveries ( COleDateTime date, bool bUpdateStock );
	void AuditDeliveriesInternal ( COleDateTime date, bool bUpdateStock );
	void WriteSessionFile( bool bDetail, bool bAutoImport );

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
	CStringArray m_arrayDeliveries;
	CStringArray m_ImportFilesArray;
	CString m_strDisplayLine;
	CString m_strTitle;
	CString m_strPreviousSession;
	CString m_strAutoImportPluFile;
	CString m_strAutoImportWrapperFile;

private:
	CSuppRefFinder m_SuppRefFinder;
	
private:
	CAdhocDeliveryEditDlg* m_pDeliveryEditDlg;
	int m_nNewPluDeptNo;

private:
	void UpdateAutoReferenceNo();
	void DeleteImportFiles();
	void DeleteSessionFile();
	void SaveStockChanges( bool bLevels );

private:
	CColumnManager m_ColumnManager;
	int m_nCacheIndex;
	CCSV m_CacheCSV;

private:
	CStocktakeCSVRecord* m_pStocktake;
	int m_nStkTakeNo;

};

/**********************************************************************/
#endif				//__ADHOCDELIVERYDATABASEDLG_H__
/**********************************************************************/

