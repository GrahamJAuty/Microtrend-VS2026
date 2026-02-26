#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "PMSStructs.h"
#include "PMSBookingCSVArray.h"
/**********************************************************************/

class CPMSBookSearchDlg : public CSSDialog
{
	DECLARE_DYNAMIC(CPMSBookSearchDlg)

public:
	CPMSBookSearchDlg( CPMSBookingCSVRecord& BookingRecord, CWnd* pParent = NULL);   
	virtual ~CPMSBookSearchDlg();

public:
	CSSTabbedComboBox m_comboCheckIn;
	CDateTimeCtrl m_datePickerCheckIn;

	CSSTabbedComboBox m_comboCheckOut;
	CDateTimeCtrl m_datePickerCheckOut;

	CSSListCtrlVirtual m_listRooms;

	CButton m_buttonBook;

	CSSComboBox m_comboRoomType;
	CSSComboBox m_comboRoomRate;

	enum { IDD = IDD_PMS_BOOK_SEARCH };

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

	afx_msg void OnKillFocusGuests();

	afx_msg void OnButtonSearch();

	afx_msg void OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult);
	
private:
	CEdit* GetEditNights();
	CEdit* GetEditGuests();
	
private:
	void FillCheckInCombo();
	void FillCheckOutCombo();
	void FillDateCombo( CSSTabbedComboBox& combo, int& nBaseDay, int nSelDay );
	void UpdateNights();
	void UpdateCheckOutDay();
	void UpdateDatePickerDates();
	void FillRoomTypeCombo( int nRoomSleeps, int nRoomType, int nGuests );
	void UpdateRoomList();

private:
	void PrepareLine( int nIndex );
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnOK();
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()

public:
	int m_nSearchCheckInDayMin;
	int m_nSearchCheckInBase;
	int m_nSearchCheckInDay;
	int m_nSearchCheckOutBase;
	int m_nSearchCheckOutDay;
	int m_nSearchRoomSleeps;
	int m_nSearchRoomSubType;
	int m_nSearchGuests;

public:
	int m_nListStartDay;
	int m_nListNights;
	int m_nListGuests;
	int m_nListRoomSleeps;
	int m_nListRoomSubType;
	int m_nListBookedRooms;
	int m_nListUnbookedDay;

private:
	bool m_bNewBooking;
	CPMSBookingCSVRecord& m_BookingRecord;
	int m_nCurrentBookedRoomNo;
	CArray<CPMSBookSearchListInfo,CPMSBookSearchListInfo> m_arrayRooms;
	CString m_strDisplayLine;
};

/**********************************************************************/
#endif
/**********************************************************************/
#endif
/**********************************************************************/
