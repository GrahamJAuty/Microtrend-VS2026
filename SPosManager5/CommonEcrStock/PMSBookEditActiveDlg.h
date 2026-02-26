#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "PMSBookingCSVArray.h"
#include "PMSStructs.h"
/**********************************************************************/

class CPMSBookEditActiveDlg : public CSSDialog
{
	DECLARE_DYNAMIC(CPMSBookEditActiveDlg)

public:
	CPMSBookEditActiveDlg( int nRoomIdx, CPMSBookingCSVRecord& BookingRecord, CWnd* pParent = NULL);   
	virtual ~CPMSBookEditActiveDlg();

public:
	CSSTabbedComboBox m_comboCheckIn;
	CDateTimeCtrl m_datePickerCheckIn;

	CSSTabbedComboBox m_comboCheckOut;
	CDateTimeCtrl m_datePickerCheckOut;

	CStatic m_staticMaxGuests;
	CStatic m_staticMaxNights;

	enum { IDD = IDD_PMS_BOOK_EDIT_ACTIVE };

public:
	virtual BOOL OnInitDialog();

public:	
	afx_msg void OnKillFocusNights();
	
	afx_msg void OnChangeDateCheckOut();
	afx_msg void OnOpenCheckOut();
	afx_msg void OnDatePickerCheckOut(NMHDR* pNMHDR, LRESULT* pResult);

private:
	CEdit* GetEditNights();
	CEdit* GetEditGuests();
	
private:
	void FillCheckInCombo();
	void FillCheckOutCombo();
	void FillDateCombo( CSSTabbedComboBox& combo, int& nBaseDay, int nSelDay, bool bCheckout );
	void UpdateNights();
	void UpdateCheckOutDay();
	void UpdateDatePickerDates();
	bool ValidateNights( bool bPrompt );
	void UpdateMaxNightsDisplay();
	bool ValidateEntry();
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnOK();
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()

public:
	int m_nSearchCheckOutBase;
	int m_nSearchCheckOutDay;
	int m_nSearchGuests;

private:
	CPMSRoomCSVRecord m_Room;
	CPMSBookingCSVRecord& m_BookingRecord;
	int m_nCheckInDay;
	int m_nMinCheckOutDay;
	int m_nMaxCheckOutDay;
};

/**********************************************************************/
#endif
/**********************************************************************/
#endif
/**********************************************************************/
