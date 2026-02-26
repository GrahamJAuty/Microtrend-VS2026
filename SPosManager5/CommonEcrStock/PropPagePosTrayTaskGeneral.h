#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "PosTrayTask.h"
#include "PosTrayTaskInfo.h"
#include "PosTrayFileSet.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

class CPropPagePosTrayTaskGeneral : public CSSPropertyPage
{
public:
	CPropPagePosTrayTaskGeneral();
	~CPropPagePosTrayTaskGeneral();

	//{{AFX_DATA(CPropPagePosTrayTaskGeneral)
	enum { IDD = IDD_PROPPAGE_POSTRAY_TASK_GENERAL };
	//}}AFX_DATA
	CEdit m_editName;
	CStatic m_staticBatch;
	CSSComboBox m_comboBatch;
	CSSComboBox m_comboEmail;
	CStatic m_staticFormatEmail;
	CSSComboBox m_comboFormatEmail;
	CButton m_buttonEmailList;
	CButton m_buttonEmailJobs;
	CStatic m_staticFormatExport;
	CSSComboBox m_comboFormatExport;
	CStatic m_staticPrint;
	CSSComboBox m_comboPrint;
	CStatic m_staticDate;
	CSSComboBox m_comboDate;
	CStatic m_staticFTP;
	CButton m_checkFTP;
	CEdit m_editFTP;
	CButton m_checkFolder;
	CEdit m_editFolder;
	CButton m_buttonFolder;
	CButton m_checkFilename;
	CStatic m_staticPrefix;
	CEdit m_editFilename;
	CStatic m_staticFilename;
	CSSComboBox m_comboFilename;
	CStatic m_staticLocInfo;
	CSSComboBox m_comboLocInfo;

	//{{AFX_VIRTUAL(CPropPagePosTrayTaskGeneral)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL
	
protected:
	//{{AFX_MSG(CPropPagePosTrayTaskGeneral)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnButtonBatchEdit();
	afx_msg void OnButtonBatchList();
	afx_msg void OnButtonEmailList();
	afx_msg void OnButtonEmailJobs();
	afx_msg void OnToggleFTP();
	afx_msg void OnToggleFolder();
	afx_msg void OnButtonFolder();
	afx_msg void OnToggleFilename();
	DECLARE_MESSAGE_MAP()
	
public:
	void SetEmailTask( CPosTrayTask* pTask ){ m_pTask = pTask; }
	void SetBatchInfoArray( CReportConsolidationArray<CPosTrayBatchInfo>* pArray ){ m_pArrayBatchInfo = pArray; }
	void SetPosTrayFileSet( CPosTrayFileSet* pFileSet ){ m_pPosTrayFileSet = pFileSet; }

private:
	void FillBatchCombo( int nBatchNo );
	void FillFilenameFormatCombo( bool bExport, bool bPrefix, int nSel );
	void EditBatch( int nIndex );
	void FillEmailCombo();
	void UpdateDefaultEmailAddress();
	void UpdateDefaultEmailCombo();

private:
	void SaveRecord();
	void GetRecordData();

private:
	CPosTrayTask* m_pTask;
	CPosTrayFileSet* m_pPosTrayFileSet;
	CReportConsolidationArray<CPosTrayBatchInfo>* m_pArrayBatchInfo;
	CString m_strDefaultFilename;
	CStringArray m_arrayEmailAddresses;
	CString m_strEmailByReport;
};

/**********************************************************************/
#endif
/**********************************************************************/
