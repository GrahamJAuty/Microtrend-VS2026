#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/

class CTimeSliceQuickSetDlg : public CDialog
{
	DECLARE_DYNAMIC(CTimeSliceQuickSetDlg)

public:
	CTimeSliceQuickSetDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTimeSliceQuickSetDlg();

	enum { IDD = IDD_TIMESLICE_QUICKSET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	int m_nAction;
	BOOL m_bSort;
};

/**********************************************************************/
#endif
/**********************************************************************/
