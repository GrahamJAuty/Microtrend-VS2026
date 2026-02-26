#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "LocationCSVArray.h"
#include "MultilineTextBuffer.h"
/**********************************************************************/

class CPropPageLocationNotes : public CSSPropertyPage
{
public:
	CPropPageLocationNotes();
	~CPropPageLocationNotes();

	//{{AFX_DATA(CPropPageLocationNotes)
	enum { IDD = IDD_PROPPAGE_LOCATION_NOTES };
	//}}AFX_DATA
	CEdit m_editNotes;
	
	//{{AFX_VIRTUAL(CPropPageLocationNotes)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CPropPageLocationNotes)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void Refresh();
	bool UpdateRecord();

public:
	void SetLocationBuffer( CLocationCSVArray* pBuffer ) { m_pLocationBuffer = pBuffer; }
	void SetLocationNotesBuffer( CMultilineTextBuffer* pBuffer ) { m_pLocationNotes = pBuffer; }
	
private:
	void GetRecordData();
	void SaveRecord();

private:
	CLocationCSVArray* m_pLocationBuffer;
	CMultilineTextBuffer* m_pLocationNotes;
};

/**********************************************************************/
#endif
/**********************************************************************/
