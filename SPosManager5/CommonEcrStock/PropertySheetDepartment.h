#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "ListDataDlg.h"
#include "DepartmentCSVArray.h"
#include "PropPageDepartmentGeneral.h"
#include "PropPageDepartmentProgram.h"
#include "PropPageDepartmentSpecimen.h"
#include "PropPageDepartmentImage.h"
/**********************************************************************/

class CPropertySheetDepartment : public CSpinPropertySheet
{
	DECLARE_DYNAMIC(CPropertySheetDepartment)

public:
	CPropertySheetDepartment( WORD wSpinID, int nDeptIdx, CListDataDlgChangeFlag& WarnCancelAfterChange, CWnd* pWndParent );
	virtual ~CPropertySheetDepartment();

public:
	CPropPageDepartmentGeneral m_Page1;
	CPropPageDepartmentProgram m_Page2;
	CPropPageDepartmentSpecimen m_Page3;
	CPropPageDepartmentImage m_Page4;
	
public:
	//{{AFX_VIRTUAL(CPropertySheetDepartment)
	//}}AFX_VIRTUAL

public:
	void SaveRecord();
	void UndoChanges();
	int GetIndex() { return m_nDeptIdx; }

public:
	void SetListDataDlg( CListDataDlg* pDlg ) { m_pListDataDlg = pDlg; }

private:
	void UpdateTitle();
	virtual void SpinPrevious();
	virtual void SpinNext();

protected:
	//{{AFX_MSG(CPropertySheetDepartment)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	int m_nDeptIdx;
	CDepartmentCSVRecord m_DeptRecord;
	CByteArray m_PageArray;
	CListDataDlgChangeFlag& m_WarnCancelAfterChange;

private:
	CListDataDlg* m_pListDataDlg;
};

/**********************************************************************/
#endif
/**********************************************************************/
