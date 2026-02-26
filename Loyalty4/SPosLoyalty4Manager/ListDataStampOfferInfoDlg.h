#pragma once
/**********************************************************************/
#include "resource.h"
/**********************************************************************/
#include "..\SPOSLoyalty4Shared\SQLTable_StampOfferInfo\SQLRowStampOfferInfo.h"
/**********************************************************************/
#include "ListDataDlg.h"
#include "SingleListHelpers.h"
#include "SQLRowSetStampOfferInfo.h"
/**********************************************************************/

class CSingleListContentHelpersStampOfferInfo : public CSingleListContentHelpers
{
public:
	CSingleListContentHelpersStampOfferInfo(CSQLRowSetStampOfferInfo& RowSet);

public:
	int GetArraySize();
	bool PrepareLine(int nIndex, CString& strLine);
	void DeleteDatabaseEntry(int nIndex);
	void SetColumns(CSSListCtrlVirtual& list, CWnd* pParent);

private:
	void FilterSelectedItemArray(CReportConsolidationArray<CSortedIntItem>& arraySelected);

private:
	CSQLRowSetStampOfferInfo& m_RowSet;
};

/**********************************************************************/

class CListDataStampOfferInfoDlg : public CListDataDlg
{
public:
	CListDataStampOfferInfoDlg( CWnd* pParent = NULL); 
	~CListDataStampOfferInfoDlg();
	
private:
	virtual void SetTitle();
	virtual void SetButtonTexts();
	virtual void SetColumns();
	virtual void PrepareList();

	virtual UINT GetDefaultButtonID() { return IDC_BUTTON1; }
	virtual void HandleButton1();	//PLU LIST
	virtual void HandleButton3();	//DELETE EXPIRED
	virtual void HandleButton4();	//RESTORE EXPIRED
	virtual void HandleDefaultButton() { HandleButton1(); }
	virtual bool HandleOK();
	virtual bool HandleCancel();

private:
	CSQLRowSetStampOfferInfo m_RowSet;
};

/**********************************************************************/
