#pragma
/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "..\CommonEcrStock\LocationSelector.h"
/**********************************************************************/
#include "DateSelectorRange.h"
#include "DepartmentSelector.h"
#include "EposReports.h"
#include "EposSelectArray.h"
#include "ReportEposDlgHelpers.h"
#include "ServerSelectionList.h"
#include "SessionCSVArray.h"
/**********************************************************************/

class CReportEposPMSDlg : public CSSDialog
{
public:
	CReportEposPMSDlg( int nPresetNo, bool bPresetPrint, bool bAutoRunPreset, CWnd* pParent = NULL);
	~CReportEposPMSDlg();

	int GetPresetError(){ return m_nPresetError; }

	//{{AFX_DATA(CReportEposPMSDlg)
	enum { IDD = IDD_REPORT_EPOS_PMS };
	
	CSSComboBox	m_comboBatch;
	
	CSSComboBox	m_comboDates;
	CDateTimeCtrl m_DatePickerTo;
	CDateTimeCtrl m_DatePickerFrom;
	CSSComboBox	m_comboSession;

	CButton m_radioPMSConsolidate1;
	CButton m_radioPMSConsolidate2;
	CButton m_radioPMSConsolidate3;
	CButton m_checkPending;
	CButton m_checkActive;
	CButton m_checkComplete;
	CButton m_checkCancelled;

	CButton m_radioConsolDefault;
	CButton m_radioConsolSaved;
	CButton m_radioConsolAdhoc;
	CButton m_radioConsolSystem;
	
	CButton	m_checkReportLocations;
	CButton	m_checkReportEpos;
	CButton	m_checkReportAllLocations;
	CButton	m_buttonPrintQuick;
	
	CStatic m_staticPreset;
	CButton m_buttonPreset;
	CSSComboBox	m_comboPreset;
	CSSComboBoxEx m_comboLocation;
	CSSTabbedComboBox	m_comboTerminal;
	CSSTabbedComboBox	m_comboTermDate;
	CEdit m_editDeptNo;
	CSSComboBox	m_comboDeptName;
	CSSComboBox	m_comboPluFilter;
	CSSComboBox	m_comboPluTime;
	CButton m_checkItemServer;
	
	CButton	m_buttonGraphSingle;
	CButton	m_buttonPrintMultiple;
	CButton	m_buttonDisplaySingle;
	
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CReportEposPMSDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CReportEposDlg)

	virtual BOOL OnInitDialog();
	
	afx_msg void OnSelectBatch();
	
	afx_msg void OnSelectReport(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult);
	
	afx_msg void OnSelectDates();
	afx_msg void OnChangeDateFrom(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeDateTo(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg void OnRadioPMSConsolidate1();
	afx_msg void OnRadioPMSConsolidate2();
	afx_msg void OnRadioPMSConsolidate3();
	
	afx_msg void OnCheckQuickPrint();
	afx_msg void OnButtonPrintQuick();
	
	afx_msg void OnButtonPreset();
	afx_msg void OnSelectPreset();
	afx_msg void OnSelectLocation();
	afx_msg void OnKillFocusDeptNo();
	afx_msg void OnSelectDeptName();
	afx_msg void OnButtonPrintMultiple();
	afx_msg void OnButtonDisplaySingle();
	afx_msg void OnButtonGraphSingle();

	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
private:
	int GetSelectedReportType();

private:
	CEposSelectArray m_SelectArray;
	COleDateTime m_oleDateFrom;
	COleDateTime m_oleDateTo;
	
private:
	CSSListTaggedSelectCtrl m_listReport;
	CDateSelectorRange m_DateSelectorRange;
	CDepartmentSelector m_DepartmentSelector;
	CLocationSelector m_LocationSelector;

private:
	int m_nPresetNo;
	int m_nPresetError;
	bool m_bPresetPrint;
	bool m_bAutoRunPreset;

private:
	CReportEposDlgHelpers m_dlgHelpers;
	
	friend class CReportEposDlgHelpers;
};

/**********************************************************************/
#endif
/**********************************************************************/
