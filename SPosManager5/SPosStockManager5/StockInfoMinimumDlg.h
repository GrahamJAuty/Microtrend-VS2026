#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SuppRefFinder.h"
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
//include "MyComboBox.h"
#include "ReorderImportHelper.h"
/**********************************************************************/

class CStockInfoMinimumDlg : public CDialog
{
public:
	CStockInfoMinimumDlg( int nSpIdx, CReorderImportHelper& ImportHelper, CSuppRefFinder& SuppRefFinder, CWnd* pParent = NULL );

	//{{AFX_DATA(CStockInfoMinimumDlg)
	enum { IDD = IDD_STOCK_INFO_MINIMUM };
	CButton	m_buttonOK;
	CButton	m_buttonCancel;
	CButton	m_buttonClose;
	CStatic	m_staticImport;
	CStatic	m_staticLines;
	CStatic	m_staticExceptions;
	CSSComboBox	m_comboReport;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CStockInfoMinimumDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

public:
	const char* GetTitle();
	static void GetTitle( bool bAllowReorder, CString& strTitle );

public:
	bool GotData() { return m_nLinesFound > 0; }

public:
	void ProcessImportData( const char* szPath );

private:
	void ProcessImportDataPlu();
	void ProcessImportDataWrapper();
	void DisplayPluReport();
	void DisplayWrapperReport();
	void DisplayPluExceptions();
	void DisplayWrapperExceptions();
	
protected:
	//{{AFX_MSG(CStockInfoMinimumDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonDisplay();
	afx_msg void OnButtonClose();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	int m_nSpIdx;
	CString m_strImportPath;
	CSuppRefFinder& m_SuppRefFinder;

private:
	bool m_bAllowReorder;
	bool m_bGotData;
	CString m_strLinesFound;
	CString m_strExceptions;
	int m_nLinesFound;
	int m_nExceptionLines;
	CString m_strTitle;

private:
	CReorderImportHelper m_ImportHelper;
};

//{{AFX_INSERT_LOCATION}}
/**********************************************************************/
#endif
/**********************************************************************/
