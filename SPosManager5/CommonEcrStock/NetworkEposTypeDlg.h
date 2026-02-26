#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "NetworkSPOSVersionArray.h"
/**********************************************************************/

struct CNetworkEposTabDetails
{
	int m_nStartIdx;
	int m_nListSize;
};

/**********************************************************************/

class CNetworkEposTypeDlg : public CDialog
{
public:
	CNetworkEposTypeDlg( int nSPOSVersion, CNetworkSPOSVersionArray& arraySPOSVersion, CWnd* pParent = NULL); 
	virtual ~CNetworkEposTypeDlg();

	//{{AFX_DATA(CNetworkEposTypeDlg)
	enum { IDD = IDD_NETWORK_EPOSTYPE };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CNetworkEposTypeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL
	CTabCtrl m_TabType;
	CSSListCtrl m_listData;
	CStatic m_staticInfo;

protected:
	//{{AFX_MSG(CNetworkEposTypeDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	afx_msg void OnTcnSelchangeTabs(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	DECLARE_MESSAGE_MAP()

private:
	void AddTabDetails( int nStartIdx, int nListSize );
	
private:
	void AddList();

public:
	int m_nSPOSVersion;

private:
	int m_nActiveTab;	
	CArray<CNetworkEposTabDetails,CNetworkEposTabDetails> m_arrayTabDetails;
	CNetworkSPOSVersionArray& m_arraySPOSVersion;
	int m_nArrayPos;
};

/**********************************************************************/
#endif
/**********************************************************************/

