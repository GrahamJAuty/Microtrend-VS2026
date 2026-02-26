#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CStockImportBasicDlg : public CDialog
{
public:
	CStockImportBasicDlg( int nSpIdx, const char* szPath, CWnd* pParent = NULL);  
	~CStockImportBasicDlg();

	//{{AFX_DATA(CStockImportBasicDlg)
	enum { IDD = IDD_STOCK_IMPORT_BASIC };
	CSSComboBox	m_comboAction;
	CButton	m_buttonOK;
	CButton	m_buttonCancel;
	CButton	m_buttonClose;
	CSSComboBox	m_comboReport;
	CButton	m_buttonImports;
	CButton	m_buttonExceptions;
	CString	m_strLinesFound;
	CString	m_strImport;
	CString	m_strExceptions;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CStockImportBasicDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CStockImportBasicDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnButtonDisplay();
	afx_msg void OnButtonClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	virtual void ShowImportFile();
	virtual void ShowStockChanges(){};
	virtual void FillActionCombo() = 0;
	virtual void ShowExceptions();

	virtual void GetImportLineInfo( CCSV& csvImport, CString& strPluNo, CCSV& csvOriginalData, double& dPluQty, double& dPluValue );
	
	virtual void AuditExceptions ( CSSFile& file, CSSFile& fileTempReport, CStringArray& StockDetails, CCSV& csvOriginalData );
	virtual void AuditStockChanges ( CSSFile& file, CSSFile& fileTempReport, CStringArray& StockDetails, CCSV& csvOriginalData, int nAction ){}
	virtual void AuditIgnored ( CSSFile& fileTempRecord, CStringArray& StockDetails, CCSV& csvOriginalData ) {}

	virtual int GetSpNo2(){ return 0; }

public:
	bool GotData() { return ( m_nLinesFound != 0 ); }
	virtual void ProcessImportData( int nAction );
	int GetAction() { return m_nAction; }

private:
	virtual void ProcessImportDataInternal( int nAction );
	bool UpdateAction();

protected:
	CString m_strTitle;
	COleDateTime m_ImportDate;
	int m_nSpIdx;
	int m_nImportFileReportType;
	int m_nStockChangesReportType;
	int m_nExceptionReportType;
	int m_nLinesFound;
	int m_nExceptionLines;
	int m_nTotalExceptions;
	int m_nStocktakeNo;
	int m_nAction;
	
protected:
	CString m_strHeaderReference1;
	CString m_strHeaderReference2;
	CString m_strHeaderSuppName;
	CString m_strHeaderSpName1;
	CString m_strHeaderSpName2;
	CString m_strHeaderImportPath;
};

/**********************************************************************/
#endif
/**********************************************************************/

