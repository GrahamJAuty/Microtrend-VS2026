#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
 
/**********************************************************************/
//include "MyComboBox.h"
/**********************************************************************/

class CPropPagePosTrayOptionsLIPA : public CSSPropertyPage
{
public:
	CPropPagePosTrayOptionsLIPA();
	~CPropPagePosTrayOptionsLIPA();

	//{{AFX_DATA(CPropPagePosTrayOptionsLIPA)
	enum { IDD = IDD_PROPPAGE_POSTRAY_OPTIONS_LIPA };
	//}}AFX_DATA
	CDateTimeCtrl m_DatePickerExport;
	CStatic m_staticExport;
	CSSComboBox m_comboHour;
	CButton m_checkFolder;
	CEdit m_editFolder;
	CButton m_buttonFolder;
	
	//{{AFX_VIRTUAL(CPropPagePosTrayOptionsLIPA)
public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	afx_msg void OnChangeDate(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSelectHour();
	afx_msg void OnToggleFolder();
	afx_msg void OnButtonFolder();
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

public:
	void GetDateNextExport( COleDateTime& date){ date = m_oleDateNextExport; }

private:
	void UpdateNextExportTime();
	void ShowNextExportTime( COleDateTime& time );
		
protected:
	//{{AFX_MSG(CPropPagePosTrayOptionsLIPA)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void SaveRecord();
	void GetRecordData();

private:
	COleDateTime m_oleDateNextExport;
};

/**********************************************************************/
#endif
/**********************************************************************/