#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "..\CommonEcrStock\SingleListHelpers.h"
/**********************************************************************/
#include "ListDataDlg.h"
/**********************************************************************/

class CSingleListContentHelpersWalletStampRule : public CSingleListContentHelpers
{
public:
	CSingleListContentHelpersWalletStampRule();

public:
	int GetArraySize();
	bool PrepareLine( int nIndex, CString& strLine );
	void DeleteDatabaseEntry( int nIndex );
	void SetColumns( CSSListCtrlVirtual& list, CWnd* pParent );
};

/**********************************************************************/

class CListDataWalletStampRuleDlg : public CListDataDlg
{
public:
	CListDataWalletStampRuleDlg( CWnd* pParent = NULL);
	~CListDataWalletStampRuleDlg();
	
private:
	virtual void SetTitle();
	virtual void SetButtonTexts();
	virtual void SetColumns();
	
	virtual UINT GetDefaultButtonID() { return IDC_BUTTON2; }
	virtual void HandleButton1();	//ADD
	virtual void HandleButton2();	//EDIT
	virtual void HandleButton3();	//DELETE
	virtual void HandleButton4();	//PRINT
	virtual void HandleButton5();	//DOWNLOAD
	virtual void HandleDefaultButton() { HandleButton2(); }

	virtual bool HandleOK();

	void HandleDownload( CArray<int,int>& arrayLocIdx );

	virtual bool SearchText(int nIndex, CString strSearchText);

private:
	CReportConsolidationArray<CSortedIntByInt> m_arrayOriginalOfferEdition;
};

/**********************************************************************/
#endif
/**********************************************************************/
