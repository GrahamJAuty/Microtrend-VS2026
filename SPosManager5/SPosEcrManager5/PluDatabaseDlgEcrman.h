#pragma once
/**********************************************************************/
#ifndef STOCKMAN_SYSTEM
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "ColumnManager.h"
/**********************************************************************/

class CPluDatabaseDlgEcrman : public CDialog
{
public:
	CPluDatabaseDlgEcrman( int nDbIdx, CWnd* pParent = NULL );
	~CPluDatabaseDlgEcrman();

	//{{AFX_DATA(CPluDatabaseDlgEcrman)
	enum { IDD = IDD_PLU_DATABASE_ECRMAN };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPluDatabaseDlgEcrman)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	//}}AFX_VIRTUAL

protected:
	afx_msg void OnContextMenu(CWnd*, CPoint point);
	//{{AFX_MSG(CPluDatabaseDlgEcrman)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPluRangeDelete();
	afx_msg void OnPluDeleteCurrent();
	afx_msg void OnPluDatabaseAdd();
	afx_msg void OnPluExportCsv();
	afx_msg void OnPluImportCsv();
	afx_msg void OnPluPrtPricelist();
	afx_msg void OnPluRangePrices();
	afx_msg void OnPluRangeSettings();
	afx_msg void OnPluPrtProgram();
	afx_msg void OnPluRangeProfit();
	afx_msg void OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPluRangeAdd();
	afx_msg void OnPluEdit();
	afx_msg void OnFilterPluRecords();
	afx_msg void OnNoFilterPluRecords();
	afx_msg void OnRangeStatus();
	afx_msg void OnFilterPluStatus();
	afx_msg void OnPluDatabaseCopy();
	afx_msg void OnColumnEdit();
	afx_msg void OnColumnSave();
	afx_msg void OnColumnRestore();
	afx_msg void OnColumnDefault();
	afx_msg void OnPluPrtBarcodeLinks();
	afx_msg void OnBarcodeNew();
	afx_msg void OnShelfNew();
	afx_msg void OnBarcodeSaved();
	afx_msg void OnShelfSaved();
	afx_msg void OnRangeStockLevels();
	afx_msg void OnImportDelivery();
	afx_msg void OnPluRangeModifierPrices();
	afx_msg void OnPluRangeModifierSettings();
	afx_msg void OnListCacheHint(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPluDatabaseDeliver();
	afx_msg void OnPluDatabaseShelf();
	afx_msg void OnPluDatabaseProduct();
	afx_msg void OnPluExportEpos();
	afx_msg void OnReportsCustom1();
	afx_msg void OnReportsCustom2();
	afx_msg void OnReportsCustom3();
	afx_msg void OnReportsCustom4();
	afx_msg void OnReportsCustom5();
	//}}AFX_MSG
	virtual void OnCancel();
	void OnButtonPluNumber();
	void OnButtonSuppRef();
	afx_msg long OnUserButton ( WPARAM wIndex, LPARAM lParam );
	DECLARE_MESSAGE_MAP()

public:
	void SetReadOnlyFlag( bool b ){ m_bReadOnly = b; }
	bool GetCanDoChangesTaskFlag(){ return m_bCanDoChangesTask; }
	bool GetCanDeleteBarcodeFlag(){ return m_bCanDeleteBarcode; }

private:
	void DisplayScreen ( int nIndex );
	bool HandleCSVImportForMaster( CString& strFilename );
	bool HandleCSVImportForPriceSet( CString& strFilename );
	
private:
	void AddPluRange ( bool bCopy );
	void EditPlu ( int nIndex );
	void BuildRecordList();
	void WriteMasterDatabase();
	void WritePriceSetFile();
	bool GetNextPluNo( CString& strPluNo );
	void PrintSingleLabel( bool bProduct ); 

private:
	void DoCustomListReport( int nRepNum );
	void SetCustomListMenuNames();

private:
	bool ContextMenuSort( CPoint& point );
	void SortListInternal( int nType );
	
private:
	CMenuChanger m_MenuChanger;
	CPluFilterArray m_FilterArray;
	int m_nDbNo;
	HICON m_hIcon;
	bool m_bInitDialog;
	CString m_strPath;
	CSSListCtrlVirtual m_listPlu;
	CString m_strDbName;
	int m_nPluSearchMode;
	int m_nNewPluDeptNo;

private:
	bool m_bReadOnly;
	bool m_bCanDoChangesTask;
	bool m_bCanDeleteBarcode;
	
private:
	CColumnManager m_ColumnManager;
	int m_nCacheIndex;
	CCSV m_CacheCSV;
};

/**********************************************************************/
#endif				
/**********************************************************************/
#endif				
/**********************************************************************/
