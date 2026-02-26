#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CPropPageLocationGeneral : public CSSPropertyPage
{
public:
	CPropPageLocationGeneral();
	~CPropPageLocationGeneral();

	//{{AFX_DATA(CPropPageLocationGeneral)
	enum { IDD = IDD_PROPPAGE_LOCATION_GENERAL };
	CStatic m_staticPaths;
	CEdit m_editName;
	CStatic m_staticAgresso;
	CEdit m_editAgresso;
	CEdit	m_editPathSales;
	CEdit	m_editPathCommsIn;
	CEdit	m_editPathCommsInNow;
	CStatic m_staticHandheld1;
	CStatic m_staticHandheld2;
	CStatic m_staticExport;
	CButton m_checkExportControl;
	CEdit	m_editPathExportControl;
	CStatic m_staticLoyalty;
	CSSComboBox m_comboLoyalty;
	CString	m_strName;
	CString	m_strPathCommsIn;
	CString	m_strPathCommsInNow;
	CString m_strPathExportControl;
	CString m_strAgresso;
	int m_nExportControlDelay;
	//}}AFX_DATA
	CButton m_buttonBrowseExportControl;

	//{{AFX_VIRTUAL(CPropPageLocationGeneral)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CPropPageLocationGeneral)
	virtual BOOL OnInitDialog();
	afx_msg void OnToggleExportControl();
	afx_msg void OnButtonBrowseCommsIn();
	afx_msg void OnButtonBrowseCommsInNow();
	afx_msg void OnButtonBrowseExportControl();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CEdit* GetEditExportControlDelay();

public:
	void Refresh();
	bool UpdateRecord();

public:
	void SetLocationBuffer( CLocationCSVArray* pBuffer ) { m_pLocationBuffer = pBuffer; }
	
private:
	void GetRecordData();
	void SaveRecord();

private:
	CLocationCSVArray* m_pLocationBuffer;

private:
	CString m_strNetworkPath;
	int m_nConnectionType;
	int m_nSPOSVersion;
};

/**********************************************************************/
#endif
/**********************************************************************/
