#pragma once
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "..\CommonEcrStock\ComboListCtrl.h"
/**********************************************************************/

class CPosTrayEmailJobsDlg : public CDialog
{
public:
	CPosTrayEmailJobsDlg(CPosTrayTask& batch, CStringArray& arrayEmailAddresses, CString& strEmailByReport, CWnd* pParent = NULL);

	//{{AFX_DATA(CPosTrayEmailJobsDlg)
	enum { IDD = IDD_POSTRAY_EMAIL_JOBS };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPosTrayEmailJobsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL
	CSSComboBox m_comboEmail[POSTRAY_MAX_EMAIL_NICKNAMES];

private:
	CArray<CellTypeInfo, CellTypeInfo> m_arrayCellTypes;
	CComboListCtrl m_listReports;

protected:
	//{{AFX_MSG(CPosTrayEmailJobsDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnButtonList();
	afx_msg void OnButtonReset();
	//}}AFX_MSG
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg LRESULT PopulateComboList(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT GetCellType(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndLabelEditVariableCriteria(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

public:
	BOOL PreTranslateMessage(MSG* pMsg);

private:
	void FillEmailCombos();
	void UpdateSelEmailCombos();
	void AddReportToList(CString strReportName, int nEmailIdx);

private:
	CArray<CPosTraySelectedReport> m_arrayReports;
	CStringArray& m_arrayEmailAddresses;
	CPosTrayTask& m_batch;
	CString& m_strEmailByReport;
	CellTypeInfoTextBuffer m_arrayComboTexts;
};

//{{AFX_INSERT_LOCATION}}

/**********************************************************************/
