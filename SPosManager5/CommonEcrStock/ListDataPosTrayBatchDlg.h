#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "ListDataDlg.h"
#include "PosTrayTaskInfo.h"
#include "PosTrayFileSet.h"
#include "..\CommonEcrStock\SingleListHelpers.h"
/**********************************************************************/

class CSingleListContentHelpersPosTrayBatch : public CSingleListContentHelpers
{
public:
	CSingleListContentHelpersPosTrayBatch( CPosTrayFileSet* pFileSet );
	void SetBatchInfoArray( CReportConsolidationArray<CPosTrayBatchInfo>* pArray ){ m_pArrayBatchInfo = pArray; }

public:
	int GetArraySize();
	bool PrepareLine( int nIndex, CString& strLine );
	void DeleteDatabaseEntry( int nIndex );
	void SetColumns( CSSListCtrlVirtual& list, CWnd* pParent );

private:
	CReportConsolidationArray<CPosTrayBatchInfo>* m_pArrayBatchInfo;
	CPosTrayFileSet* m_pPosTrayFileSet;
};

/**********************************************************************/

class CListDataPosTrayBatchDlg : public CListDataDlg
{
public:
	CListDataPosTrayBatchDlg( CPosTrayFileSet* pFileSet, CWnd* pParent = NULL); 
	~CListDataPosTrayBatchDlg();
	
private:
	virtual void SetTitle();
	virtual void SetButtonTexts();
	virtual void SetColumns();
	virtual void PrepareList();

	virtual UINT GetDefaultButtonID() { return IDC_BUTTON2; }
	virtual void HandleButton1();	//ADD
	virtual void HandleButton2();	//COPY
	virtual void HandleButton3();	//EDIT
	virtual void HandleButton4();	//DELETE
	virtual void HandleDefaultButton() { HandleButton3(); }

	virtual bool HandleCancel();
	virtual bool HandleOK();

private:
	void EditItem( int nIndex );

public:
	void SetBatchInfoArray( CReportConsolidationArray<CPosTrayBatchInfo>* pArray );
	void SetBatchNo( int n ){ m_nBatchNo = n; }
	int GetBatchNo(){ return m_nBatchNo; }

private:
	void AddBatch( int nBatchNo, CPosTrayTask& BatchToCopy );
	bool GetNextBatchNo( int& nBatchNo );
	void NewBatchError();

private:
	CPosTrayFileSet* m_pPosTrayFileSet;
	CReportConsolidationArray<CPosTrayBatchInfo>* m_pArrayBatchInfo;
	int m_nBatchNo;
};

/**********************************************************************/
#endif
/**********************************************************************/
