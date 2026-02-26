#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "PMSBookingCSVArray.h"
#include "PMSInvoiceLineInfo.h"
#include "PMSRoomStatusTable.h"
#include "PMSStructs.h"
#include "PMSTariffMap.h"
/**********************************************************************/
#define PMS_EDITBOOKING_PENDING_SINGLE_NEW 0
#define PMS_EDITBOOKING_PENDING_SINGLE_EDIT 1
#define PMS_EDITBOOKING_CLOSED_SINGLE_EDIT 2
#define PMS_EDITBOOKING_CLOSED_MULTIPLE_EDIT 3
#define PMS_EDITBOOKING_ACTIVE_SINGLE_NEW 4
#define PMS_EDITBOOKING_ACTIVE_SINGLE_EDIT 5
#define PMS_EDITBOOKING_ACTIVE_MULTIPLE_NEW 6
#define PMS_EDITBOOKING_ACTIVE_MULTIPLE_EDIT 7
#define PMS_EDITBOOKING_CANCELLED_SINGLE_EDIT 8
/**********************************************************************/
#define PMS_EMAIL_CREATE_BOOKING 0
#define PMS_EMAIL_AMEND_BOOKING 1
#define PMS_EMAIL_CANCEL_BOOKING 2
/**********************************************************************/

class CPMSBookEditDlg : public CSSDialog
{
	DECLARE_DYNAMIC(CPMSBookEditDlg)

public:
	CPMSBookEditDlg( int nBookingType, int nRoomIdx, int nAccountIdx, CPMSBookingCSVRecord& BookingRecord, CPMSCustomerCSVRecord& CustomerRecord, CWnd* pParent = NULL);   
	virtual ~CPMSBookEditDlg();

	bool WasBookingEdited(){ return m_bEditedBooking; }
	bool WasBookingEmailed(){ return m_bEmailedBooking; }
	bool GetMultipleAccountsFlag(){ return m_bMultipleAccounts; }
	bool GetDeleteBookingFlag(){ return m_bDeleteBooking; }
	bool GetRestoreBookingFlag(){ return m_bRestoreBooking; }

	void GetTitleText( CString& strText );
	bool CanSendEmail();
	bool CanEnableMultipleAccounts();
	bool CanEditCovers();
	bool CanEditBooking();
	bool CanEditDinner();
	bool CanDeleteBookingOrCancelCheckIn();
	void GetDeleteButtonText( CString& strText );
	void GetOKButtonText( CString& strText );
	void GetCancelButtonText( CString& strText );
	void GetDeletePromptText( CString& strText );
	
public:
	CSSListCtrlVirtual m_listRooms;
	CString m_strGuestName;
	CString m_strBillingName;
	CString m_strAddress1;
	CString m_strAddress2;
	CString m_strAddress3;
	CString m_strPostcode;
	CString m_strPhone;
	CString m_strFax;
	CString m_strEmail;
	CEdit m_editReference;
	CEdit m_editNote;
	CString m_strNote;
	double m_dRoomRate;
	double m_dExtraBedRate;
	CEdit m_editCheckIn;
	CEdit m_editNights;
	CEdit m_editCheckOut;
	CEdit m_editGuests;
	CEdit m_editBalance;
	CEdit m_editDeposit;
	CButton m_buttonSearch;
	CButton m_buttonDelete;
	CButton m_buttonOK;
	CButton m_buttonCancel;
	CButton m_checkAccounts;
	CButton m_checkTraining;
	CButton m_buttonDeposit;
	CButton m_buttonDinner;
	CSSTabbedComboBox m_comboTariff;
	CSSComboBox m_comboRate;

	CStatic m_staticEmail;
	CButton m_checkEmail;
	
	CStatic m_staticCovers;
	int m_nCovers;

	enum { IDD = IDD_PMS_BOOK_EDIT };

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonSearch();
	afx_msg void OnButtonDelete();
	afx_msg void OnButtonDeposit();
	afx_msg void OnButtonDinner();
	afx_msg void OnSelectTariff();
	afx_msg void OnSelectRoomRate();
	afx_msg void OnChangeRoomRate();
	afx_msg void OnChangeExtraBedRate();
	
private:
	CEdit* GetEditCovers();
	CEdit* GetEditRate();
	CEdit* GetEditBed();
		
private:
	void PrepareLine( int nIndex );
	void DisplayBooking();
	void DisplayBalance();
	void SendEmail( int nType );
	void HandleRestore();
	void HandleDelete();
	void UpdateRoomRate();
	void SetDefaultTariffRates();
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnOK();
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()

private:
	bool m_bAccountsTick;
	int m_nBookingType;
	int m_nTariffType;
	int m_nRoomRateType;
	bool m_bEditedBooking;
	bool m_bEmailedBooking;
	bool m_bMultipleAccounts;
	bool m_bDeleteBooking;
	bool m_bRestoreBooking;
	CArray<CPMSBookSearchListInfo,CPMSBookSearchListInfo> m_arrayRooms;

	CPMSBookingCSVRecord& m_BookingRecord;
	CPMSCustomerCSVRecord& m_CustomerRecord;
	CPMSRoomAccountInfo m_infoAccount;
	int m_nActiveRoomIdx;
	int m_nActiveAccountIdx;

private:
	CString m_strDisplayLine;
	bool m_bCanEditRoomRate;

private:
	CPMSTariffMap m_TariffMap;
};

/**********************************************************************/
#endif
/**********************************************************************/
#endif
/**********************************************************************/
