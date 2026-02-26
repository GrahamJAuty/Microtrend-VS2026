#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "..\CommonEcrStock\LocationSelector.h"
#include "PosTrayExportFieldDlg.h"
#include "PosTrayExportFieldNames.h"
/**********************************************************************/

class CPosTrayBatchExportPaymentDlg : public CSSDialog
{
public:
	CPosTrayBatchExportPaymentDlg( CPosTrayTask& BatchEmail, CWnd* pParent = NULL);
	~CPosTrayBatchExportPaymentDlg();

	//{{AFX_DATA(CPosTrayBatchExportPaymentDlg)
	enum { IDD = IDD_POSTRAY_BATCH_EXPORT_PAYMENT };

	CStatic m_staticName;
	CEdit m_editName;
	
	CTabCtrl m_TabFilter;
	
	CStatic m_staticDate;
	CStatic m_staticDate1;
	CStatic m_staticDate2;
	CStatic m_staticDate3;
	CStatic m_staticDate4;
	CSSComboBox	m_comboDates;
	CSSComboBox	m_comboRound;
	CSSComboBox	m_comboSession;
	CSSComboBox m_comboTimeSlice;

	CStatic m_staticDatabase;
	CStatic m_staticDatabase1;
	CStatic m_staticDatabase2;
	CSSComboBoxEx m_comboLocation;
	CSSTabbedComboBox m_comboTerminal;
	CButton m_checkGuest;
	
	CSSListCtrlVirtual m_listFields;

	CButton m_buttonOK;
	CButton m_buttonCancel;
	
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPosTrayBatchExportPaymentDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CPosTrayBatchExportPaymentDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelectTabFields(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSelectTabFilter(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSelectLocation();
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonFieldAdd();
	afx_msg void OnButtonFieldChange();
	afx_msg void OnButtonFieldRemove();
	afx_msg void OnButtonFieldTidy();
	afx_msg void OnButtonFieldMoveUp();
	afx_msg void OnButtonFieldMoveDown();
	afx_msg long OnAddField( WPARAM wIndex, LPARAM lParam );
	//}}AFX_MSG
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()
	
private:
	void FillTimeSliceCombo();
		
private:
	void LoadEmailBatchSettings();
	void SaveEmailBatchSettings();
	
private:
	bool PrepareLine( int nIndex );
	void CreateFieldDisplayLine( int nIndex, CPosTrayExportFieldName& Name );
	bool SelectExistingItem( int nFieldType );

public:
	void AddField( int nFieldType );
				
private:
	CSessionCSVRecord m_ReportSession;
	
private:
	CLocationSelector m_LocationSelector;
	
private:
	CPosTrayTask& m_BatchEmail;
	CArray<int,int> m_arrayFieldTypes;

private:
	CString m_strDisplayLine;

private:
	CPosTrayExportFieldDlg* m_pFieldsDlg;
};

/**********************************************************************/
#endif
/**********************************************************************/
