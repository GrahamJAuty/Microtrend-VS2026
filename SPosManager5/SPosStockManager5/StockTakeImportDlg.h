#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "CCSVReportLine.h"
#include "..\CommonEcrStock\ComboListCtrl.h"
//include "MyComboBox.h"
#include "StockTakeCSVArray.h"
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

struct CStocktakeInfo
{
public:
	CStocktakeInfo();

public:
	int m_nStocktakeNo;
	CString m_strType;
	CString m_strComment;
	int m_nLineCount;
};

/**********************************************************************/

struct CStocktakeItemInfo
{
public:
	CStocktakeItemInfo();

public:
	int m_nStocktakeNo;
	int m_nPluIdx;
	int m_nStockIdx;
	double m_dStockPerPlu;
	CString m_strNumber;
	CString m_strQty;
};

/**********************************************************************/

struct CExceptionInfo
{
public:
	CExceptionInfo();

public:
	int m_nType;
	int m_nPluIdx;
	int m_nStockIdx;
	double m_dStockPerPlu;
	CString m_strNumber;
	CString m_strQty;
};

/**********************************************************************/

class CStocktakeImportDlg : public CDialog
{
public:
	CStocktakeImportDlg( int nSpIdx, const char* szFilename, CStocktakeCSVArray& StocktakeArray, CWnd* pParent = NULL); 

public:
	bool CheckImportFile();
	void SetDirectStockCodeFlag(bool b) { m_bDirectStockCode = b; }

	//{{AFX_DATA(CStocktakeImportDlg)
	enum { IDD = IDD_STOCKTAKE_IMPORT };
	//}}AFX_DATA
	CSSComboBox m_comboAction;

	//{{AFX_VIRTUAL(CStocktakeImportDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

private:
	bool FindStocktakeInfo( int nStocktakeNo, int& nPos );
	bool FindStocktakeItemInfo( int nStocktakeNo, int nStockIdx, int& nPos );
	void FindExceptionInfo( int nType, int& nPos );

private:
	void AddException( int nType, CExceptionInfo& info );
	void AppendPluExceptions( CExceptionInfo& info, CStockReportLine& csvOut );
	void AppendStockExceptions( CExceptionInfo& info, CStockReportLine& csvOut );

private:
	void UpdateStockLevels( CArray<int,int>& arrayItemsToProcess );
	void UpdatePendingFile( CArray<int,int>& arrayItemsToKeep, CArray<int,int>& arrayExceptionsToKeep );

protected:
	//{{AFX_MSG(CStocktakeImportDlg)
	afx_msg void OnButtonAll();
	afx_msg void OnButtonNone();
	afx_msg void OnButtonPreview();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void AddList();
	
private:
	CSSListCtrl m_listStocktakes;
	CSSListCtrl m_listException;

private:
	int m_nSpIdx;
	CString m_strFilename;
	bool m_bDirectStockCode;

private:
	int m_nExceptionType[2];
	CArray<CStocktakeInfo,CStocktakeInfo> m_arrayStocktakeInfo;
	CArray<CStocktakeItemInfo,CStocktakeItemInfo> m_arrayStocktakeItemInfo;
	CArray<CExceptionInfo,CExceptionInfo> m_arrayExceptionInfo;
};

/**********************************************************************/
//{{AFX_INSERT_LOCATION}}
/**********************************************************************/
#endif 
/**********************************************************************/

