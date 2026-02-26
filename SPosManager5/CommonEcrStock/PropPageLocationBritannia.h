#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "LocationCSVArray.h"
/**********************************************************************/

class CPropPageLocationBritannia : public CSSPropertyPage
{
public:
	CPropPageLocationBritannia();
	~CPropPageLocationBritannia();

	//{{AFX_DATA(CPropPageLocationBritannia)
	enum { IDD = IDD_PROPPAGE_LOCATION_BRITANNIA };
	//}}AFX_DATA
	CSSComboBox m_comboPrice;
	
	//{{AFX_VIRTUAL(CPropPageLocationBritannia)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CPropPageLocationBritannia)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

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
};

/**********************************************************************/
#endif
/**********************************************************************/
