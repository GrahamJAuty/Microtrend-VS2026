#pragma once
//$$******************************************************************
#include "ListDataDlg.h"
#include "SingleListHelpers.h"
//$$******************************************************************
#include "..\SmartPay4ManagerBgnd\DbExportHandler.h"
//$$******************************************************************

class CSingleListContentHelpersDatabaseReport : public CSingleListContentHelpers
{
public:
	CSingleListContentHelpersDatabaseReport( CArray<CDatabaseReportInfo,CDatabaseReportInfo>& arrayReportInfo );

public:
	int GetArraySize();
	bool PrepareLine( int nIndex, CString& strLine );
	void DeleteDatabaseEntry( int nIndex );
	void SetColumns( CSSListCtrlVirtual& list, CWnd* pParent );

private:
	CArray<CDatabaseReportInfo, CDatabaseReportInfo>& m_arrayReportInfo;
};

//$$******************************************************************

class CListDataDatabaseReportDlg : public CListDataDlg
{
public:
	CListDataDatabaseReportDlg(CWnd* pParent = NULL);
	~CListDataDatabaseReportDlg();
	
private:
	virtual void SetTitle();
	virtual void SetButtonTexts();
	virtual void SetColumns();
	virtual void PrepareList();

	virtual UINT GetDefaultButtonID() { return IDOK; }
	virtual void HandleButton1();	//ADD
	virtual void HandleButton2();	//EDIT
	virtual void HandleButton3();	//DELETE
	virtual void HandleButton4();	//MOVE UP
	virtual void HandleButton5();	//MOVE DOWN
	virtual void HandleButton6();	//SHOW REPORT
	virtual void HandleDefaultButton() { HandleButton2(); }
	virtual bool HandleOK();

public:
	void SetLeaversFilename(const char* sz) { m_strLeaversFilename = sz; }
	CString GetReportLabel() { return m_strReportLabel;  }
	bool GetLeaversRunFindFlag() { return m_bLeaversRunFind;  }

private:
	virtual void CSSAutoShutdownPreInitDialog();

private:
	int GetLabel();

public:
	int m_nDefaultOption;
	bool m_bEditReqd;
	CString	m_strReportName;

private:
	CString m_strInitialText;
	CString m_strInitialName;

private:
	CDbExportHandler m_export;
	CString m_strReportLabel;

private:
	CCSV m_csvReportList;
	CArray<CDatabaseReportInfo, CDatabaseReportInfo> m_arrayReportInfo;

private:
	CString m_strLeaversFilename;
	bool m_bLeaversRunFind;
};

//$$******************************************************************
