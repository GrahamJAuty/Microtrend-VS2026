#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "DateSelectorRange.h"
#include "EposSelectArray.h"
#include "..\CommonEcrStock\LocationSelector.h"
#include "SessionCSVArray.h"
/**********************************************************************/
//include "MySSDialog.h"
/**********************************************************************/

class CDorbiereExportDlg : public CSSDialog
{
public:
	CDorbiereExportDlg( CWnd* pParent = NULL);
	~CDorbiereExportDlg();

	//{{AFX_DATA(CDorbiereExportDlg)
	enum { IDD = IDD_DORBIERE_EXPORT };
	CSSComboBox	m_comboRound;
	CSSComboBox	m_comboSession;
	CSSComboBox	m_comboDates;
	CDateTimeCtrl	m_DatePickerTo;
	CDateTimeCtrl	m_DatePickerFrom;
	CSSComboBoxEx	m_comboLocation;
	CSSTabbedComboBox	m_comboTerminal;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDorbiereExportDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDorbiereExportDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelectDate();
	afx_msg void OnSelectLocation();
	afx_msg void OnChangeDateFrom(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeDateTo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonDisplay();
	afx_msg void OnButtonExport();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void GetReportDates();
	bool GetAdhocReportSession();

private:
	bool CreateReport( bool bExport );
	//bool CreateReport( CEposReport* pReport, int nConLevel, CString& strTitle, CString& strSessionTitle, bool bExport );
		
private:
	COleDateTime m_oleDateFrom;
	COleDateTime m_oleDateTo;
	CSessionCSVRecord m_ReportSession;
	CSessionCSVRecord m_ReportSessionAdhoc;
	
private:
	CDateSelectorRange m_DateSelectorRange;
	CLocationSelector m_LocationSelector;
	CEposSelectArray m_SelectArray;
};

/**********************************************************************/
#endif
/**********************************************************************/
