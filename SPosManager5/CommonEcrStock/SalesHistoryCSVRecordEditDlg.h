#pragma once
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CSalesHistoryCSVRecordEditDlg : public CDialog
{
public:
	CSalesHistoryCSVRecordEditDlg( CWnd* pParent = NULL);   
	
	//{{AFX_DATA(CSalesHistoryCSVRecordEditDlg)
	enum { IDD = IDD_SALESHISTORY_EDIT };
	CSSTabbedComboBox m_comboType;
	CString	m_strName;
	//}}AFX_DATA

	CButton m_checkTotal[8];
	CButton m_checkHeader[8];
	CSSComboBox m_comboField[8];

	int m_checkTotalID[8];
	int m_checkHeaderID[8];
	int m_comboFieldID[8];
	
	//{{AFX_VIRTUAL(CSalesHistoryCSVRecordEditDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CSalesHistoryCSVRecordEditDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelectField();
	afx_msg void OnButtonTidy();
	afx_msg void OnToggleTopBottom();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	virtual void FillCombos();
	virtual void SelectField();
	void GetFieldSettings( CWordArray& arrayTemp );
	void PrepareControlArrays();
	
protected:
	virtual const char* GetName() = 0;
	virtual const char* GetTitle() = 0;
	virtual void PrepareCombos( CWordArray& validFields, CWordArray& reportFields ) = 0;
	virtual void TidyFields() = 0;
	virtual void ProcessChanges() = 0;

	virtual bool SpecialValidate(){ return TRUE; }

protected:
	bool m_bChart;
	bool m_bAdhoc;
};

/**********************************************************************/
//{{AFX_INSERT_LOCATION}}
/**********************************************************************/
