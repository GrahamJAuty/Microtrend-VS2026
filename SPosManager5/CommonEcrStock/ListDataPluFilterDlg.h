#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "ListDataDlg.h"
#include "..\CommonEcrStock\SingleListHelpers.h"
/**********************************************************************/

class CSingleListContentHelpersPluFilter : public CSingleListContentHelpers
{
public:
	CSingleListContentHelpersPluFilter( COfferGroupsCSVArray* pFilterNames, COfferGroupEntriesCSVArray* pFilterItems );

public:
	int GetArraySize();
	bool PrepareLine( int nIndex, CString& strLine );
	void DeleteDatabaseEntry( int nIndex );
	void SetColumns( CSSListCtrlVirtual& list, CWnd* pParent );

private:
	COfferGroupsCSVArray* m_pFilterNames;
	COfferGroupEntriesCSVArray* m_pFilterItems;
};

/**********************************************************************/

class CListDataPluFilterDlg : public CListDataDlg
{
public:
	CListDataPluFilterDlg( bool bSystem, CWnd* pParent = NULL); 
	~CListDataPluFilterDlg();
	
private:
	virtual void SetTitle();
	virtual void SetButtonTexts();
	virtual void SetColumns();

	virtual UINT GetDefaultButtonID() { return IDC_BUTTON2; }
	virtual void HandleButton1();	//ADD
	virtual void HandleButton2();	//EDIT
	virtual void HandleButton3();	//DELETE
	virtual void HandleButton4();	//PRINT
	virtual void HandleDefaultButton() { HandleButton2(); }

	void HandleDownload( CArray<int,int>& arrayLocIdx );

private:
	bool m_bSystem;
	COfferGroupsCSVArray* m_pFilterNames;
	COfferGroupEntriesCSVArray* m_pFilterItems;
};

/**********************************************************************/
#endif
/**********************************************************************/
