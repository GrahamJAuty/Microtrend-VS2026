#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#define UPLOAD_TABTYPE_PLU_IMPORT 0
#define UPLOAD_TABTYPE_PLU_ITEM 1
#define UPLOAD_TABTYPE_PLU_TYPE 2
#define UPLOAD_TABTYPE_PLU_PROTECT 3
#define UPLOAD_TABTYPE_CUSTOMER_IMPORT 4
/**********************************************************************/
#define UPLOAD_TICKCOUNT_TYPE 13
#define UPLOAD_TICKCOUNT_PROTECT 1
/**********************************************************************/

class CPropPageLocationUpload : public CSSPropertyPage
{
public:
	CPropPageLocationUpload();
	~CPropPageLocationUpload();

	//{{AFX_DATA(CPropPageLocationUpload)
	enum { IDD = IDD_PROPPAGE_LOCATION_UPLOAD };
	CEdit	m_editPathExport;
	CStatic m_staticID;
	CEdit m_editID;
	CTabCtrl m_TabFileType;
	CStatic m_staticPluType;
	CButton m_radioImportType1;
	CButton m_radioImportType2;
	CButton m_radioImportType3;
	CButton m_radioPluExisting1;
	CButton m_radioPluExisting2;
	CButton m_radioPluExisting3;
	CButton m_radioPluModifier1;
	CButton m_radioPluModifier2;
	CButton m_radioPluModifier3;
	CButton m_checkPluType[UPLOAD_TICKCOUNT_TYPE];
	CButton m_checkPluProtect[UPLOAD_TICKCOUNT_PROTECT];
	CString	m_strPathExport;
	CString m_strID;
	//}}AFX_DATA
	
	//{{AFX_VIRTUAL(CPropPageLocationUpload)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CPropPageLocationUpload)
	virtual BOOL OnInitDialog();
	afx_msg void OnTcnSelchangeTabs(NMHDR *pNMHDR, LRESULT *pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void Refresh();
	bool UpdateRecord();

public:
	void SetLocationBuffer( CLocationCSVArray* pBuffer ) { m_pLocationBuffer = pBuffer; }
	void SetConnectionType( int nType ){ m_nConnectionType = nType; }
	
private:
	void GetRecordData();
	void SaveRecord();
	void SelectTab();
	void SaveTabSettings();

private:
	void ValidatePluImportType();
	void ValidateCustomerImportType();
	void ValidateExistingItemMode();
	bool ExistingItemModeIsGlobal( int& nGlobalMode );
	void EnableTicks( bool bType, bool bProtect );
	
private:
	CLocationCSVArray* m_pLocationBuffer;
	bool m_bWantID;
	int m_nConnectionType;
	int m_nPluImportType;
	int m_nPluExistingItemMode;
	int m_nPluImportMask;
	int m_nPluProtectMask;
	int m_nPluImportModifierMode;
	int m_nCustomerImportType;
	
private:
	WORD m_IDPluType[UPLOAD_TICKCOUNT_TYPE];
	int m_nPluType[UPLOAD_TICKCOUNT_TYPE];
	WORD m_IDPluProtect[UPLOAD_TICKCOUNT_PROTECT];
	
private:
	int m_nTabPage;
	CArray<int,int> m_arrayTabPages;
};

/**********************************************************************/
#endif
/**********************************************************************/
