#pragma once
/**********************************************************************/
#include "DateSelectorRange.h"
#include "DepartmentSelector.h"
#include "DeptAndPluComboHelpers.h"
#include "EODDateTimeFilter.h"
#include "EposReportSelect.h"
#include "EposSelectArray.h"
#include "..\CommonEcrStock\LocationSelector.h"
#include "PresetReportEpos.h"
/**********************************************************************/

class CReportEposDlgHelpers
{
public:
	CReportEposDlgHelpers();

public:
	void SetControlPointers( void* pDlg );

public:
	void HandleInitDialog( int nPresetNo, bool bPresetPrint, bool bAutoRunPreset, int& nPresetError );
	void HandleSelectBatch();
	void HandleSelectReport();
	void HandleSelectDates();
	void HandleChangeDateFrom();
	void HandleChangeDateTo();
	void HandleCheckQuickPrint();
	void HandleSelectLocation();
	void HandleKillFocusDeptNo();
	void HandleSelectDeptName();
	void HandleDisplaySingle();
	void HandlePrintMultiple();
	void HandlePrintQuick();
	void HandleGraphSingle();
	void HandlePMSRadioConsolidate( int n );
	void HandleButtonPreset();
	void HandleSelectPreset();

public:
	void CreatePreset( CPresetReportInfo& Preset );
	void FillPresetCombo( int nSelection );

public:
	bool CreateReport( int nReportType, int nConLevel, CString& strTitle, bool bPrint, bool bQuick );
	void CreateGraph( int nReportType, int nConLevel, CString& strTitle );

public:
	bool CheckGraphableJobs();
	bool CheckPrintableJobs();
	bool CheckQuickPrintOptions();
	bool CheckTrainingOption();
	int GetSelectedReportType();
	void SelectReportType( int n );
	void GetReportDates();
	bool GetAdhocReportSession();

private:
	bool CreateReportInternal( CEposReport* pReport, int nConLevel, CString& strTitle, CString& strSessionTitle, bool bFixStartDate, bool bPrint, bool bQuick );
	void CreateGraphInternal( CEposReport* pReport, int nConLevel, CString& strTitle, CString& strSessionTitle );

private:
	void CreateReportList();
	void SetReportsTypeBatch( CPresetReportInfo& BatchReport, bool bAll );

private:
	CWnd* m_pParentWnd;
	CWordArray m_arrayReportTypesMaster;
	CWordArray m_arrayReportTypesBatch;

private:
	CDateSelectorRange* m_pDateSelectorRange;
	CDepartmentSelector* m_pDepartmentSelector;
	CLocationSelector* m_pLocationSelector;
	CDeptAndPluComboHelpers m_DeptAndPluComboHelpers;

private:
	CSSComboBox* m_pComboBatch;
	CSSListTaggedSelectCtrl* m_pListReport;
	
	CSSComboBox* m_pComboDates;
	CDateTimeCtrl*	m_pDatePickerTo;
	CDateTimeCtrl*	m_pDatePickerFrom;
	CSSComboBox* m_pComboSession;

	CButton* m_pCheckPending;
	CButton* m_pCheckActive;
	CButton* m_pCheckComplete;
	CButton* m_pCheckCancelled;
	CButton* m_pRadioPMSConsolidate1;
	CButton* m_pRadioPMSConsolidate2;
	CButton* m_pRadioPMSConsolidate3;

	CButton* m_pRadioConsolDefault;
	CButton* m_pRadioConsolSaved;
	CButton* m_pRadioConsolAdhoc;
	CButton* m_pRadioConsolSystem;

	CButton* m_pCheckReportAllLocations;
	CButton* m_pCheckReportLocations;
	CButton* m_pCheckReportEpos;
	CButton* m_pButtonPrintQuick;
	
	CSSComboBox* m_pComboPreset;
	CSSComboBoxEx* m_pComboLocation;
	CSSTabbedComboBox* m_pComboTerminal;
	CSSTabbedComboBox* m_pComboTermDate;
	CButton* m_pButtonDisplaySingle;
	CButton* m_pButtonPrintMultiple;
	CButton* m_pButtonGraphSingle;

	CButton* m_pCheckItemServer;

	CEdit* m_pEditDeptNo;
	CSSComboBox* m_pComboPluFilter;
	CSSComboBox* m_pComboPluTime;

private:
	CEposSelectArray m_SelectArray;
	COleDateTime m_oleDateFrom;
	COleDateTime m_oleDateTo;
	COleDateTime m_oleFutureDateTo;
	CSessionCSVRecord m_ReportSession;
	CSessionCSVRecord m_ReportSessionAdhoc;
	
private:
	int m_nPMSConsolidationMode;

private:
	CEposReportSelect m_EposReportSelect;
};

/**********************************************************************/
