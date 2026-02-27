#pragma once
//*******************************************************************
#include "resource.h"
//*******************************************************************
#include "..\SmartPay4ManagerBgnd\DbExportHandler.h"
//*******************************************************************

class CExportPropertyPage1 : public CSSAutoShutdownPropertyPage
{
public:
	CExportPropertyPage1();
	virtual ~CExportPropertyPage1();
	void SetPointer ( CDbExportHandler* pDbExportHandler );

// Dialog Data
	enum { IDD = IDD_PROPPAGE_EXPORT1 };
	CStatic m_staticFilename;
	CEdit m_editFilename;
	CButton m_buttonSelect;
	CButton m_buttonBrowse;

protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	virtual BOOL OnKillActive();
	virtual BOOL OnInitDialog();
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnButtonBrowse();
	afx_msg void OnButtonUp();
	afx_msg void OnButtonDown();
	afx_msg void OnButtonSelect();

	DECLARE_MESSAGE_MAP()

private:
	void SetSelectButtonText();
	int CreateHeaderLine();
	const char* GetHeaderText ( const char* szDisplayName );
	const char* GetDisplayText ( const char* szHeaderText );
	void SetTranslateTable();
	void CreateDisplayText ( const char* szHeaderText, const char* szUserText );

private:
	CDbExportHandler* m_pDbExportHandler;
	CSSListTaggedSelectCtrl m_listFields;
	
	CString m_strHeader;
	bool m_bSelectAll;
	
	CStringArray m_arrayFieldIn;
	CStringArray m_arrayFieldOut;
};

//*******************************************************************
