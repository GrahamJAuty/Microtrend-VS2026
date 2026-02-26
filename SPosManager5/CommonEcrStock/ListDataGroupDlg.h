#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "ListDataDlg.h"
#include "..\CommonEcrStock\SingleListHelpers.h"
/**********************************************************************/

class CSingleListContentHelpersGroup : public CSingleListContentHelpers
{
public:
	CSingleListContentHelpersGroup( int nGroupType, CGroupCSVArrayBase* pGroupArray );

public:
	int GetArraySize();
	bool PrepareLine( int nIndex, CString& strLine );
	void DeleteDatabaseEntry( int nIndex );
	void SetColumns( CSSListCtrlVirtual& list, CWnd* pParent );
	
private:
	void FilterSelectedItemArray( CReportConsolidationArray<CSortedIntItem>&  arraySelected );

private:
	int m_nGroupType;
	CGroupCSVArrayBase* m_pGroupArray;
};

/**********************************************************************/

class CListDataGroupDlg : public CListDataDlg
{
public:
	CListDataGroupDlg( int nGroupType, int nGroupNoSel, CWnd* pParent = NULL); 
	~CListDataGroupDlg();

public:
	int GetGroupNoSel(){ return m_nGroupNoSel; }
	
private:
	virtual void SetTitle();
	virtual void SetButtonTexts();
	virtual void SetColumns();
	virtual void PrepareList();

	virtual UINT GetDefaultButtonID() { return IDC_BUTTON2; }
	virtual void HandleButton1();	//ADD
	virtual void HandleButton2();	//EDIT
	virtual void HandleButton3();	//DELETE
	virtual void HandleButton4();	//PRINT
	virtual void HandleButton5();	//UPLOAD
	virtual void HandleDefaultButton() { HandleButton2(); }
	virtual bool HandleOK();

private:
	bool IsGroupZero( int nIndex );
	void HandleDownload( int nDbIdx, CArray<int,int>& arrayLocIdx );
	bool SearchText(int nIndex, CString strSearchText);

private:
	void PrintEposGroupReport();
	void PrintConsolGroupReport();
	void PrintReportGroupReport();
	void PrintPaymentGroupReport();
	void PrintAllergenReport();

private:
	void GroupZeroMessage();

private:
	int m_nGroupType;
	int m_nGroupNoSel;

private:
	CGroupCSVArrayBase* m_pGroupArray;
};

/**********************************************************************/
#endif
/**********************************************************************/
