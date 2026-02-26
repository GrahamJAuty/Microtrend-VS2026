#pragma once
/**********************************************************************/
 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
//include "MySSListCtrl.h"
/**********************************************************************/
#ifdef POSTRAY_UTILITY
/**********************************************************************/
#define POSTRAY_TABTYPE_NETWORK 0
#define POSTRAY_TABTYPE_TASK 1
#define POSTRAY_TABTYPE_FNB 2
#define POSTRAY_TABTYPE_GENERAL 3
#define POSTRAY_TABTYPE_S4LABOUR 4
#define POSTRAY_TABTYPE_LIPA 5
#define POSTRAY_TABTYPE_CHARTWELLS 6
#define POSTRAY_TABTYPE_BACKUP 7
/**********************************************************************/

class CPosTrayStatusDlg : public CDialog
{
public:
	CPosTrayStatusDlg( CPosTrayStatusDlg** pParentToThis, CWnd* pParent = NULL);   
	virtual ~CPosTrayStatusDlg();

public:
	int GetActiveTab();
	void DisplayCashRSPStatus( const char* sz );

private:
	CStringArray* GetVisibleTaskArray();
	int GetSafeSelectionTaskType();
	int GetSafeSelectionTaskType( int nMin, int nMax );
	void SelectTaskType();
	void SelectLineTask( int nIndex );
	void SelectLineTaskType( int nIndex );
	
public:
	void UpdateNetworkList( CString& strStatus, CStringArray& arrayList, COleDateTime& time );
	void UpdateStatusLine( CString& strStatus, COleDateTime& time );
	void UpdateTaskList( int nTaskType, CStringArray& arrayList, CStringArray& arrayDest);
	void UpdateTaskListProcessTran(CStringArray& arrayList);
	void UpdateTaskListReportEpos( CStringArray& arrayList );
	void UpdateTaskListReportHist( CStringArray& arrayList );
	void UpdateTaskListReportStkLevel( CStringArray& arrayList );
	void UpdateTaskListReportTimeAttend(CStringArray& arrayList);
	void UpdateTaskListExportSales( CStringArray& arrayList );
	void UpdateTaskListExportPayment( CStringArray& arrayList );
	void UpdateTaskListExportVoid( CStringArray& arrayList );
	void UpdateTaskListExportDiscount( CStringArray& arrayList );
	void UpdateNonTaskList(CString& strStatus, CStringArray& arrayList, COleDateTime& time, CSSListCtrlVirtual& DestList, CStringArray& DestArray, bool bMergeExistingList = FALSE);
	void UpdateFNBList( CString& strStatus, CStringArray& arrayList, COleDateTime& time );
	void UpdateChartwellsList(CString& strStatus, CStringArray& arrayList, COleDateTime& time);
	void UpdateS4LabourList( CString& strStatus, CStringArray& arrayList, COleDateTime& time );
	void UpdateLIPAList( CString& strStatus, CStringArray& arrayList, COleDateTime& time );
	void UpdateBackupList(CString& strStatus, CStringArray& arrayList, COleDateTime& time);
	void UpdateGeneralList( CString& strStatus, CStringArray& arrayList, COleDateTime& time );

public:
	enum { IDD = IDD_POSTRAY_STATUS };

protected:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	virtual void PostNcDestroy();
	afx_msg void OnTcnSelchangeTabs(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnGetDispInfoListNetwork(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetDispInfoListType(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetDispInfoListTask(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetDispInfoListFNB(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetDispInfoListChartwells(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetDispInfoListS4Labour(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetDispInfoListLIPA(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetDispInfoListGeneral(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetDispInfoListBackup(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelectType(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelectTask(NMHDR* pNMHDR, LRESULT* pResult);
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()

private:
	void SelectList();
	void ShowList(CSSListCtrlVirtual& list, int nListType, int nVisibleType);
	void PrepareLineNetwork( int nIndex );
	void PrepareLineTask( int nIndex );
	void PrepareLineFNB( int nIndex );
	void PrepareLineChartwells( int nIndex);
	void PrepareLineS4Labour( int nIndex );
	void PrepareLineLIPA( int nIndex );
	void PrepareLineGeneral( int nIndex );
	void PrepareLineBackup( int nIndex );
	void UpdateStatusTime( COleDateTime& time );
	void PrepareTabPagesAndTaskTypes();
	void ResetTabPages();
	
private:
	CPosTrayStatusDlg** m_pParentToThis;

private:
	CTabCtrl m_TabStatus;
	CSSListCtrlVirtual m_listNetwork;
	CSSListCtrlVirtual m_listType;
	CSSListCtrlVirtual m_listTask;
	CSSListCtrlVirtual m_listFNB;
	CSSListCtrlVirtual m_listChartwells;
	CSSListCtrlVirtual m_listS4Labour;
	CSSListCtrlVirtual m_listLIPA;
	CSSListCtrlVirtual m_listGeneral;
	CSSListCtrlVirtual m_listBackup;
	CStatic m_staticTime;
	CStatic m_staticStatus1;
	CStatic m_staticStatus2;

private:
	CStringArray m_strArrayLinesNetwork;
	CStringArray m_strArrayLinesTaskProcessTran;
	CStringArray m_strArrayLinesTaskReportEpos;
	CStringArray m_strArrayLinesTaskReportHist;
	CStringArray m_strArrayLinesTaskReportStkLevel;
	CStringArray m_strArrayLinesTaskReportTimeAttend;
	CStringArray m_strArrayLinesTaskExportSales;
	CStringArray m_strArrayLinesTaskExportPayment;
	CStringArray m_strArrayLinesTaskExportVoid;
	CStringArray m_strArrayLinesTaskExportDiscount;
	CStringArray m_strArrayLinesFNB;
	CStringArray m_strArrayLinesChartwells;
	CStringArray m_strArrayLinesS4Labour;
	CStringArray m_strArrayLinesLIPA;
	CStringArray m_strArrayLinesGeneral;
	CStringArray m_strArrayLinesBackup;
	CString m_strDisplayLineNetwork;
	CString m_strDisplayLineTask;
	CString m_strDisplayLineFNB;
	CString m_strDisplayLineChartwells;
	CString m_strDisplayLineS4Labour;
	CString m_strDisplayLineLIPA;
	CString m_strDisplayLineGeneral;
	CString m_strDisplayLineBackup;

private:
	int m_nTabPage;
	int m_nSelectedTaskType;
	int m_nSelectedTaskIdx;
	CArray<int,int> m_arrayTabPages;
	CArray<int,int> m_arrayTaskTypes;
};

/**********************************************************************/
#endif
/**********************************************************************/
