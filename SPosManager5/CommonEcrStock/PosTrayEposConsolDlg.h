#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
 
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "EposReportSelect.h"
#include "PosTrayEposConsolParamsArray.h"
/**********************************************************************/

class CPosTrayEposConsolDlg : public CSSDialog
{
public:
	CPosTrayEposConsolDlg( CEposReportSelectInfo& infoReport, CPosTrayEposConsolParamsInfo& infoParams, int nTaskType, CWnd* pParent = NULL);   

	//{{AFX_DATA(CPosTrayEposConsolDlg)
	enum { IDD = IDD_POSTRAY_EPOS_CONSOL };
	//}}AFX_DATA
	
	CButton m_radioPosTray;
	CButton m_radioDefault;
	CButton m_radioSaved;
	CButton m_radioSummary;
	CButton m_radioCustom;

	CButton m_checkConsolType1;
	CButton m_checkConsolType2;
	CButton m_checkConsolType3;
	CButton m_checkConsolType4;

	//{{AFX_VIRTUAL(CPosTrayEposConsolDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL
	afx_msg void OnSelectType();

public:
	static bool CanDoAdhocReport( CEposReportSelectInfo& infoReport );

private:
	const char* GetCSVLine();
	void UpdateCustomTicks( const char* szParams );
	int GetSelectedType();

protected:
	//{{AFX_MSG(CPosTrayEposConsolDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	int m_nConsolidationOptionType;

private:
	CEposReportSelectInfo& m_infoReport;
	CPosTrayEposConsolParamsInfo& m_infoParams;
	int m_nTaskType;
	CString m_strCSVLine;
};

/**********************************************************************/
#endif
/**********************************************************************/
