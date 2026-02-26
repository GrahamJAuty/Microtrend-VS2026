#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "StockImportFilterArray.h"
#include "SuppRefFinder.h"
/**********************************************************************/

class CStockImportFilterWrapperDlg : public CDialog
{
public:
	CStockImportFilterWrapperDlg( int nSpIdx, bool bClosing, const char* szPath, CSuppRefFinder& SuppRefFinder, CStockImportFilterArray& arrayImport, CWnd* pParent );  
	~CStockImportFilterWrapperDlg();

	//{{AFX_DATA(CStockImportFilterWrapperDlg)
	enum { IDD = IDD_STOCK_IMPORT_FILTER_WRAPPER };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CStockImportFilterWrapperDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

public:
	CStatic m_staticLines;
	CStatic m_staticExceptions;
	CStatic m_staticSelected;
	CButton m_buttonExceptions;
	CButton m_buttonPreview;
	CButton m_buttonSelect;
	CButton m_radio1;
	CButton m_radio2;
	CButton m_radio3;
	CButton m_buttonOK;
	CButton m_buttonCancel;

protected:
	//{{AFX_MSG(CStockImportFilterWrapperDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	void OnButtonImport();
	void OnButtonExceptions();
	void OnButtonPreview();
	void OnButtonSelect();
	DECLARE_MESSAGE_MAP()

public:
	void SetStocktakeNo( int n ){ m_nStocktakeNo = n; }

public:
	int GetAction(){ return m_nAction; }
	bool GotData(){ return ( m_nLinesFound != 0 ); }
	void ProcessImportData();

private:
	void ProcessImportDataInternal();
	void UpdateSelectionCount();

private:
	void AuditExceptions( CSSFile& fileExceptions, CCSV& csvOriginalData, int nError, const char* szStockCode );
	void AuditImportData ( CSSFile& fileImportReport, CCSV& csvOriginalData, int nError );
	const char* GetErrorText( int nError );

private:
	void UpdateImportArray();

private:
	int CheckStockLevelStatusClosing( int nStockIdx );
	int CheckStockLevelStatusOpening( int nStockIdx );

private:
	int m_nSpIdx;
	bool m_bClosing;
	int m_nStocktakeNo;
	int m_nImportFileReportType;
	int m_nAuditReportType;
	int m_nExceptionReportType;
	
private:
	int m_nLinesFound;
	int m_nExceptionLines;

private:
	CString m_strHeaderImportPath;
	CString m_strHeaderReference1;
	CString m_strHeaderReference2;
	CString m_strHeaderSuppName;
	CString m_strHeaderSpName1;
	CString m_strHeaderSpName2;

private:
	COleDateTime m_ImportDate;

private:
	CString m_strTitle;
	CString m_strRadio1;
	CString m_strRadio2;
	CString m_strRadio3;
	CString m_strError;

public:
	int m_nAction;
	CStockImportFilterArray& m_arrayImport;

private:
	CSuppRefFinder& m_SuppRefFinder;
	int m_nSuppNo;
};

/**********************************************************************/
#endif
/**********************************************************************/
