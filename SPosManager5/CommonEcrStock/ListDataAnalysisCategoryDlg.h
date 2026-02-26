#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "ListDataDlg.h"
#include "..\CommonEcrStock\SingleListHelpers.h"
/**********************************************************************/

class CSingleListContentHelpersAnalysisCategory : public CSingleListContentHelpers
{
public:
	CSingleListContentHelpersAnalysisCategory();

public:
	int GetArraySize();
	bool PrepareLine( int nIndex, CString& strLine );
	void DeleteDatabaseEntry( int nIndex ){}
	void SetColumns( CSSListCtrlVirtual& list, CWnd* pParent ){}
};

/**********************************************************************/

class CListDataAnalysisCategoryDlg : public CListDataDlg
{
public:
	CListDataAnalysisCategoryDlg( CWnd* pParent = NULL); 
	~CListDataAnalysisCategoryDlg();
	
private:
	virtual void SetTitle();
	virtual void SetButtonTexts();
	virtual void SetColumns();

	virtual UINT GetDefaultButtonID() { return IDC_BUTTON1; }
	virtual void HandleButton1();	//EDIT
	virtual void HandleButton4();	//DOWNLOAD
	virtual void HandleDefaultButton() { HandleButton1(); }

	void HandleDownload( CArray<int,int>& arrayLocIdx );
};

/**********************************************************************/
#endif
/**********************************************************************/
