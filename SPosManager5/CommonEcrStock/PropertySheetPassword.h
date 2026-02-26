#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "PropPagePasswordGeneral.h"
#include "PropPagePasswordTicks.h"
#include "PropPagePasswordPluFilter.h"
/**********************************************************************/

class CPropertySheetPassword : public CSpinPropertySheet
{
	DECLARE_DYNAMIC(CPropertySheetPassword)

public:
	CPropertySheetPassword( WORD wSpinID, int nPassWordIdx, CWnd* pWndParent = NULL);

public:
	CPropPagePasswordGeneral m_Page1;
	CPropPagePasswordPluFilter m_Page2;
	CPropPagePasswordTicks m_Page3;
	CPropPagePasswordTicks m_Page4;
	CPropPagePasswordTicks m_Page5;
	CPropPagePasswordTicks m_Page6;
	CPropPagePasswordTicks m_Page7;
	CPropPagePasswordTicks m_Page8;
	CPropPagePasswordTicks m_Page9;
	CPropPagePasswordTicks m_Page10;
	CPropPagePasswordTicks m_Page11;
	
public:
	//{{AFX_VIRTUAL(PropertySheetPassword)
	//}}AFX_VIRTUAL

public:
	virtual ~CPropertySheetPassword();

	public:
	void SaveRecord();
	int GetIndex() { return m_nPasswordIdx; }

private:
	void UpdateTitle();
	virtual void SpinPrevious();
	virtual void SpinNext();

protected:
	//{{AFX_MSG(CPropertySheetPassword)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	int m_nPasswordIdx;
	CPasswordCSVRecord m_PasswordRecord;
	CByteArray m_PageArray;
};

/**********************************************************************/
#endif
/**********************************************************************/
