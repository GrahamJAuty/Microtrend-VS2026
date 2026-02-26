#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "StockImportFilterArray.h"
/**********************************************************************/

class CStockImportFilterPluDlg : public CSSDialog
{
public:
	CStockImportFilterPluDlg( int nSpIdx, bool bClosing, const char* szPath, CStockImportFilterArray& arrayImport, CWnd* pParent );  
	~CStockImportFilterPluDlg();

	//{{AFX_DATA(CStockImportFilterPluDlg)
	enum { IDD = IDD_STOCK_IMPORT_FILTER_PLU };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CStockImportFilterPluDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	virtual void AdjustControls() {}

protected:
	CStatic m_staticLines;
	CStatic m_staticExceptions;
	CStatic m_staticSelected;
	CButton m_buttonExceptions;
	CButton m_buttonPreview;
	CButton m_buttonSelect;
	CStatic m_staticAction;
	CButton m_radio1;
	CButton m_radio2;
	CButton m_radio3;
	CButton m_buttonOK;
	CButton m_buttonCancel;

protected:
	//{{AFX_MSG(CStockImportFilterPluDlg)
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

protected:
	virtual void ProcessImportDataInternal(){};
	virtual void PreviewImportedData(){};
	void UpdateSelectionCount();

protected:
	void UpdateImportArray();
	
protected:
	int m_nSpIdx;
	bool m_bClosing;
	int m_nStocktakeNo;
	int m_nImportFileReportType;
	int m_nAuditReportType;
	int m_nExceptionReportType;
	int m_nPluExplodeType;
	bool m_bMultiSheet;

protected:
	int m_nLinesFound;
	int m_nExceptionLines;
	int m_nTotalExceptions;

protected:
	CString m_strHeaderImportPath;
	CString m_strHeaderReference1;
	CString m_strHeaderReference2;
	CString m_strHeaderSuppName;
	CString m_strHeaderSpName1;
	CString m_strHeaderSpName2;

protected:
	COleDateTime m_ImportDate;

protected:
	CString m_strTitle;
	CString m_strRadio1;
	CString m_strRadio2;
	CString m_strRadio3;

protected:
	int m_nAction;
	CStockImportFilterArray& m_arrayImport;
};

/**********************************************************************/
#endif
/**********************************************************************/

