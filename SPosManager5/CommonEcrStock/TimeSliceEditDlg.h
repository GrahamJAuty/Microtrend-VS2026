#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "TimeSliceCSVArray.h"
/**********************************************************************/

class CTimeSliceEditDlg : public CSSDialog
{
public:
	CTimeSliceEditDlg( CTimeSliceCSVRecord& TimeSlice, CWnd* pParent = NULL);   

	//{{AFX_DATA(CTimeSliceEditDlg)
	enum { IDD = IDD_TIMESLICE_EDIT };
	CDateTimeCtrl	m_TimePickerStart;
	CDateTimeCtrl	m_TimePickerEnd;
	CString	m_strName;
	int m_nGroup;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CTimeSliceEditDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CTimeSliceEditDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CTimeSliceCSVRecord& m_TimeSlice;
};

//{{AFX_INSERT_LOCATION}}
/**********************************************************************/
#endif
/**********************************************************************/
