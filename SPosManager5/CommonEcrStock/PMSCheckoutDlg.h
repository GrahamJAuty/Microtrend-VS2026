#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "PMSBookingCSVArray.h"
#include "PMSRoomStatusInfo.h"
/**********************************************************************/

class CPMSCheckoutDlg : public CSSDialog
{
public:
	CPMSCheckoutDlg( int nRoomIdx, CPMSBookingCSVRecord& BookingRecord, CWnd* pParent = NULL);   
	
	//{{AFX_DATA(CPMSCheckoutDlg)
	enum { IDD = IDD_PMS_CHECKOUT };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPMSCheckoutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL
	CSSTabbedComboBox m_comboCheckIn;
	CSSTabbedComboBox m_comboCheckOutExpected;
	CSSTabbedComboBox m_comboCheckOutActual;
	CStatic m_staticWarning;

private:
	void UpdateWarningText();

protected:
	//{{AFX_MSG(CPMSCheckoutDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	int m_nRoomIdx;
	int m_nLastPossibleCheckOutDay;
	CPMSRoomStatusInfo m_infoRoom;
	CPMSBookingCSVRecord& m_BookingRecord;
};

/**********************************************************************/
#endif
/**********************************************************************/
#endif
/**********************************************************************/
