#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "S4LabourLocationOptions.h"
//include "MyComboBox.h"
/**********************************************************************/

class CPosTrayS4LabourLocationDlg : public CSSDialog
{
public:
	CPosTrayS4LabourLocationDlg( int nLocIdx, CS4LabourLocationOptions& Options, CWnd* pParent = NULL); 
	virtual ~CPosTrayS4LabourLocationDlg();

	enum { IDD = IDD_POSTRAY_S4LABOUR_LOCATION };
	CButton m_checkEnable;
	CEdit m_editLocID;
	CSSComboBox m_comboTerminal;
	CDateTimeCtrl m_DatePickerExport;
	CStatic m_staticExport;
	CSSComboBox m_comboHour;
	
public:
	void GetDateNextExport( COleDateTime& date){ date = m_oleDateNextExport; }

private:
	void UpdateNextExportTime();
	void ShowNextExportTime( COleDateTime& time );

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);  
	afx_msg void OnCheckEnable();
	afx_msg void OnChangeDate(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSelectHour();
	virtual void OnOK();
	
	DECLARE_MESSAGE_MAP()

private:
	int m_nLocIdx;
	CS4LabourLocationOptions& m_Options;
	COleDateTime m_oleDateNextExport;
};

/**********************************************************************/
#endif
/**********************************************************************/

