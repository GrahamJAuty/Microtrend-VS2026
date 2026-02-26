#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "ListDataDlg.h"
#include "..\CommonEcrStock\SingleListHelpers.h"
/**********************************************************************/

class CSingleListContentHelpersPromotion : public CSingleListContentHelpers
{
public:
	CSingleListContentHelpersPromotion();

public:
	int GetArraySize();
	bool PrepareLine( int nIndex, CString& strLine );
	void DeleteDatabaseEntry( int nIndex );
	void SetColumns( CSSListCtrlVirtual& list, CWnd* pParent );
};

/**********************************************************************/

class CListDataPromotionDlg : public CListDataDlg
{
public:
	CListDataPromotionDlg( CWnd* pParent = NULL); 
	~CListDataPromotionDlg();
	
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
};

/**********************************************************************/
#endif
/**********************************************************************/
