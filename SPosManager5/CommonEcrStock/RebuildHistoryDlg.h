#pragma once
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "DateSelectorRange.h"
//include "MySSListCtrl.h"
/**********************************************************************/

struct CRebuildHistoryInfo
{
	int m_nNetworkIdx;
	int m_nLocIdx;
	int m_nKbLength;
};

/**********************************************************************/

class CRebuildHistoryDlg : public CDialog
{
public:
	CRebuildHistoryDlg(CWnd* pParent = NULL); 

	//{{AFX_DATA(CRebuildHistoryDlg)
	enum { IDD = IDD_REBUILD_HISTORY };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRebuildHistoryDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	afx_msg void OnSelectDate();
	afx_msg void OnChangeDateFrom(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeDateTo(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CRebuildHistoryDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonView();
	afx_msg void OnButtonRebuild();
	afx_msg void OnButtonAll();
	afx_msg void OnButtonNone();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void AddList();
	void ShowTransactionLog( bool bAllTicked );

private:
	void ShowTransactionLogInternal( bool bAllTicked );
	void HandleRebuild();
	void RebuildStockSalesHistory();
	void RefreshSelection();

private:
	void GetReportDates();

private:
	CArray<CRebuildHistoryInfo,CRebuildHistoryInfo> m_arrayFileInfo;

private:
	COleDateTime m_OleDateFrom;
	COleDateTime m_OleDateTo;

private:
	CDateSelectorRange m_DateSelectorRange;

private:
	CSSListCtrl m_listLocations;
	CImageList m_ImageList;
	CSSComboBox	m_comboDates;
	CDateTimeCtrl m_DatePickerTo;
	CDateTimeCtrl m_DatePickerFrom;
	CButton m_checkRemoveDuplicates;
};

//{{AFX_INSERT_LOCATION}}
/**********************************************************************/
