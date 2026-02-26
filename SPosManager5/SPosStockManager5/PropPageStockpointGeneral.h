#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "AddressInfo.h"
#include "StockpointCSVArray.h"
/**********************************************************************/

class CPropPageStockpointGeneral : public CSSPropertyPage
{
public:
	CPropPageStockpointGeneral();
	~CPropPageStockpointGeneral();

	//{{AFX_DATA(CPropPageStockpointGeneral)
	enum { IDD = IDD_PROPPAGE_STOCKPOINT_GENERAL };
	CString	m_strAddress1;
	CString	m_strAddress2;
	CString	m_strAddress3;
	CString	m_strName;
	CString	m_strPhone;
	CString	m_strPostcode;
	CString	m_strEmail;
	CString	m_strFax;
	CStatic m_staticImportSite;
	int m_nImportSiteNo;
	//}}AFX_DATA
	CEdit* GetEditImportSite();

	//{{AFX_VIRTUAL(CPropPageStockpointGeneral)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CPropPageStockpointGeneral)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void SetAddressInfo( CAddressInfo* pInfo ) { m_pAddressInfo = pInfo; }
	void SetStockpointBuffer( CStockpointCSVArray* pBuffer ) { m_pStockpointBuffer = pBuffer; }

private:
	void GetRecordData();
	void SaveRecord();

private:
	CAddressInfo* m_pAddressInfo;
	CStockpointCSVArray* m_pStockpointBuffer;
};

/**********************************************************************/
#endif
/**********************************************************************/
