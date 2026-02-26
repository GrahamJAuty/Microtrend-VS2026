#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "PMSRoomCSVArray.h"
#include "PMSStructs.h"
#include "PMSTariffMap.h"
/**********************************************************************/

class CPMSBookRoomDlg : public CSSDialog
{
	DECLARE_DYNAMIC(CPMSBookRoomDlg)

public:
	CPMSBookRoomDlg( int nRoomIdx, CPMSBookingResponseInfo& Response, CPMSBookingCSVRecord& BookingRecord, CWnd* pParent = NULL);   
	virtual ~CPMSBookRoomDlg();

public:
	CSSTabbedComboBox m_comboCheckIn;
	CDateTimeCtrl m_datePickerCheckIn;

	CSSTabbedComboBox m_comboCheckOut;
	CDateTimeCtrl m_datePickerCheckOut;

	CStatic m_staticMaxNights;
	CStatic m_staticMaxGuests;
	CButton m_buttonAction1;
	CButton m_buttonAction2;

	CSSTabbedComboBox m_comboTariff;
	CSSComboBox m_comboRate;

	enum { IDD = IDD_PMS_BOOK_ROOM };

public:
	virtual BOOL OnInitDialog();

public:
	afx_msg void OnChangeDateCheckIn();
	afx_msg void OnOpenCheckIn();
	afx_msg void OnDatePickerCheckIn(NMHDR* pNMHDR, LRESULT* pResult);
	
	afx_msg void OnKillFocusNights();
	
	afx_msg void OnChangeDateCheckOut();
	afx_msg void OnOpenCheckOut();
	afx_msg void OnDatePickerCheckOut(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg void OnSelectTariff();
	afx_msg void OnSelectRate();
	
	afx_msg void OnButtonAction1();
	afx_msg void OnButtonAction2();

private:
	CEdit* GetEditNights();
	CEdit* GetEditGuests();
	CEdit* GetEditRate();
	CEdit* GetEditBed();
	
private:
	void FillCheckInCombo();
	void FillCheckOutCombo();
	void FillDateCombo( CSSTabbedComboBox& combo, int& nBaseDay, int nSelDay, bool bCheckout );
	void UpdateNights();
	void UpdateCheckOutDay();
	void UpdateDatePickerDates();
	bool ValidateNights( bool bPrompt );
	void UpdateMaxNightsDisplay();
	bool ValidateEntry( bool bCheckIn );
	void UpdateRoomRate();
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnOK();
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()

public:
	int m_nSearchCheckInBase;
	int m_nSearchCheckInDay;
	int m_nSearchCheckOutBase;
	int m_nSearchCheckOutDay;
	int m_nSearchGuests;
	int m_nAction;

private:
	CPMSBookingCSVRecord& m_BookingRecord;
	CPMSBookingResponseInfo& m_Response;
	CPMSRoomCSVRecord m_Room;
	bool m_bCanEditRoomRate;
	bool m_bAllowCheckIn;
	int m_nTariffType;
	int m_nRoomRateType;

private:
	int m_nRoomIdx;
	CPMSTariffMap m_TariffMap;
};

/**********************************************************************/
#endif
/**********************************************************************/
#endif
/**********************************************************************/
