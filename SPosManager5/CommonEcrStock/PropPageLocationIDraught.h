#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "LocationCSVArray.h"
/**********************************************************************/

class CPropPageLocationIDraught : public CSSPropertyPage
{
public:
	CPropPageLocationIDraught();
	~CPropPageLocationIDraught();

	//{{AFX_DATA(CPropPageLocationIDraught)
	enum { IDD = IDD_PROPPAGE_LOCATION_IDRAUGHT };
	//}}AFX_DATA
	
	//{{AFX_VIRTUAL(CPropPageLocationIDraught)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CPropPageLocationIDraught)
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

private:
	CString m_strIDraughtID;
};

/**********************************************************************/
#endif
/**********************************************************************/
