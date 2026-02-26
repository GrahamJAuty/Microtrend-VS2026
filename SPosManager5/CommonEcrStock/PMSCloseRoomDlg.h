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
/**********************************************************************/

class CPMSCloseRoomDlg : public CSSDialog
{
	DECLARE_DYNAMIC(CPMSCloseRoomDlg)

public:
	CPMSCloseRoomDlg( bool bEdit, int nRoomIdx, CPMSBookingResponseInfo& Response, CPMSClosedRoomCSVRecord& BookingRecord, CWnd* pParent = NULL);   
	virtual ~CPMSCloseRoomDlg();

public:
	CSSTabbedComboBox m_comboCheckIn;
	CDateTimeCtrl m_datePickerCheckIn;

	CSSTabbedComboBox m_comboCheckOut;
	CDateTimeCtrl m_datePickerCheckOut;

	CStatic m_staticMaxNights;
	CButton m_buttonAction1;
	CButton m_buttonAction2;

	CSSComboBox m_comboReason;
	CEdit m_editReason;

	enum { IDD = IDD_PMS_CLOSE_ROOM };

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

	afx_msg void OnButtonAction1();
	afx_msg void OnButtonAction2();

	afx_msg void OnSelectReason();
	afx_msg void OnButtonReason();

private:
	CEdit* GetEditNights();
	
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
	void GetSelectedReason( CString& strReason, bool& bCustom );
	void FillReasonCombo( const char* szReason );
	
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
	int m_nAction;

private:
	bool m_bEdit;
	CPMSClosedRoomCSVRecord& m_ClosureRecord;
	CPMSBookingResponseInfo& m_Response;
	CPMSRoomCSVRecord m_Room;
};

/**********************************************************************/
#endif
/**********************************************************************/
#endif
/**********************************************************************/
