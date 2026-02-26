#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "..\CommonEcrStock\ComboListCtrl.h"
/**********************************************************************/

class CPosTrayFilenameHistoryDlg : public CDialog
{
public:
	CPosTrayFilenameHistoryDlg(CWnd* pParent = NULL); 
	virtual ~CPosTrayFilenameHistoryDlg();

// Dialog Data
	enum { IDD = IDD_POSTRAY_FILENAMES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg LRESULT GetCellType(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndLabelEditVariableCriteria(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

public:
	BOOL PreTranslateMessage(MSG* pMsg);

private:
	CArray<CellTypeInfo,CellTypeInfo> m_arrayCellTypes;
	CComboListCtrl m_listReports;
};

/**********************************************************************/
#endif
/**********************************************************************/

