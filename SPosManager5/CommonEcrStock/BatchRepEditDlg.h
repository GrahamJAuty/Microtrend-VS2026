#pragma once
/**********************************************************************/
#include "EposReportSelect.h"
#include "PresetReportEposBatch.h"
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CBatchRepEditDlg : public CDialog
{
public:
	CBatchRepEditDlg( CPresetReportInfo& BatchRecord, CWnd* pParent = NULL);   

	//{{AFX_DATA(CBatchRepEditDlg)
	enum { IDD = IDD_BATCHREP_EDIT };
	CButton	m_checkLocations;
	CButton	m_checkTerminals;
	CButton	m_checkAll;
	CString	m_strName;
	int m_nActionType;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CBatchRepEditDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CBatchRepEditDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnButtonAllReport();
	afx_msg void OnButtonNoneReport();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void GetRecordData();
	void AddList();

private:
	CSSListTaggedSelectCtrl m_listReports;
	CPresetReportInfo& m_Base;
	CPresetReportInfoEposBatch m_Preset; 

private:
	CWordArray m_arrayReportTypes;

private:
	CEposReportSelect m_EposReportSelect;
};

/**********************************************************************/
//{{AFX_INSERT_LOCATION}}
/**********************************************************************/


