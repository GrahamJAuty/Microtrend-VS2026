#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "StockpointCSVArray.h"
#include "afxwin.h"
/**********************************************************************/

class CPropPageStockpointTransfers : public CPropertyPage
{
public:
	CPropPageStockpointTransfers();
	~CPropPageStockpointTransfers();

	//{{AFX_DATA(CPropPageStockpointTransfers)
	enum { IDD = IDD_PROPPAGE_STOCKPOINT_TRANSFERS };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPropPageStockpointTransfers)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL
	
protected:
	//{{AFX_MSG(CPropPageStockpointTransfers)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void SetStockpointBuffer( CStockpointCSVArray* pBuffer ) { m_pStockpointBuffer = pBuffer; }

private:
	void GetRecordData();
	void SaveRecord();

private:
	CStockpointCSVArray* m_pStockpointBuffer;

private:
	CButton m_checkTransfer1;
	CButton m_checkTransfer3;
	CButton m_checkTransfer4;
};

/**********************************************************************/
#endif
/**********************************************************************/
