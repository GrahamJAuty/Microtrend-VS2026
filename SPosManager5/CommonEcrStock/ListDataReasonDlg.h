#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "..\CommonEcrStock\CascadingReasonCSVArray.h"
#include "..\CommonEcrStock\SingleListHelpers.h"
/**********************************************************************/
#include "ListDataDlg.h"
#include "LocationSelectorEntity.h"
/**********************************************************************/

class CSingleListContentHelpersReason : public CSingleListContentHelpers
{
public:
	CSingleListContentHelpersReason( CLocationSelectorEntity& LocSelEntity );

public:
	int GetArraySize();
	bool PrepareLine( int nIndex, CString& strLine );
	void DeleteDatabaseEntry( int nIndex );
	void SetColumns( CSSListCtrlVirtual& list, CWnd* pParent );
	void GetReasonInfo( CCascadingReasonCSVRecord& Reason, CString& strSource, CString& strText, bool& bManualEntry );

public:
	const char* GetGroupText( CCascadingReasonCSVRecord& Table, int nLevel );

private:
	CLocationSelectorEntity& m_LocSelEntity;
	CCascadingReasonCSVArray* m_pReasonArray;
	CString m_strLocalType;
};

/**********************************************************************/

class CListDataReasonDlg : public CListDataDlg
{
public:
	CListDataReasonDlg( CLocationSelectorEntity& LocSelEntity, CWnd* pParent = NULL); 
	~CListDataReasonDlg();
	
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
	
private:
	void HandleDownload();

private:
	CLocationSelectorEntity& m_LocSelEntity;
	CCascadingReasonCSVArray* m_pReasonArray;
};

/**********************************************************************/
#endif
/**********************************************************************/
