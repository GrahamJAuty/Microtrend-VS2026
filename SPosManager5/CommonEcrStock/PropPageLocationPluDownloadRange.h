#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "LocationCSVArray.h"
/**********************************************************************/

class CPropPageLocationPluDownloadRange : public CSSPropertyPage
{
public:
	CPropPageLocationPluDownloadRange();
	~CPropPageLocationPluDownloadRange();

	//{{AFX_DATA(CPropPageLocationPluDownloadRange)
	enum { IDD = IDD_PROPPAGE_LOCATION_PLUDOWNLOADRANGE };
	//}}AFX_DATA
	CSSComboBox m_comboType;
	CEdit* GetEditPluFrom(int n){ return GetEdit( m_nPluFromID[n] ); }
	CEdit* GetEditPluTo(int n){ return GetEdit( m_nPluToID[n] ); }
	
	//{{AFX_VIRTUAL(CPropPageLocationPluDownloadRange)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL
	afx_msg void OnSelectType();
	afx_msg void OnKillFocusPluFrom1();
	afx_msg void OnKillFocusPluFrom2();
	afx_msg void OnKillFocusPluFrom3();
	afx_msg void OnKillFocusPluFrom4();
	afx_msg void OnKillFocusPluFrom5();
	afx_msg void OnKillFocusPluTo1();
	afx_msg void OnKillFocusPluTo2();
	afx_msg void OnKillFocusPluTo3();
	afx_msg void OnKillFocusPluTo4();
	afx_msg void OnKillFocusPluTo5();

protected:
	//{{AFX_MSG(CPropPageLocationPluDownloadRange)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void Refresh();
	bool UpdateRecord();

public:
	void SetLocationBuffer( CLocationCSVArray* pBuffer ) { m_pLocationBuffer = pBuffer; }

private:
	void KillFocusPlu( CEdit* pEdit );
	
private:
	void GetRecordData();
	void SaveRecord();

private:
	CLocationCSVArray* m_pLocationBuffer;

private:
	int m_nPluFromID[5];
	int m_nPluToID[5];
};

/**********************************************************************/
#endif
/**********************************************************************/
