#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "DateSelectorRange.h"
#include "PMSCalendarDlg.h"
#include "PMSCustomerCSVArray.h"
#include "PMSRoomStatusInfo.h"
/**********************************************************************/

struct CPMSAccountFindInfo
{
	int m_nInvoiceNum;
	int m_nCheckInDay;
	int m_nBookingId;
	int m_nBookingPart;
	int m_nRoomNo;
	CString m_strRoomName;
	int m_nCustomerId;
	int m_nGuests;
	CString m_strGuestName;
	CString m_strBillingName;
	int m_nBookingPartStartDay;
	int m_nBookingPartNights;
	bool m_bCheckedOut;
	CString m_strSortKey;
	CString m_strGroupKey;
};

/**********************************************************************/

class CPMSAccountFindDlg : public CSSDialog
{
public:
	CPMSAccountFindDlg( int nType, CWnd* pParent = NULL);
	void SetCalendarDlg( CPMSCalendarDlg* pDlg ){ m_pCalendarDlg = pDlg; }
	int GetLastEditedBookingId(){ return m_nLastEditedBookingId; }
	
public:
	CSSComboBox	m_comboDates;
	CDateTimeCtrl m_DatePickerTo;
	CDateTimeCtrl m_DatePickerFrom;
	CButton m_checkTraining;
	CButton m_buttonAction1;
	CButton m_buttonAction2;
	CButton m_buttonAction3;
	CStatic m_staticInvoiceNum;
	CString m_strName;
	CString m_strRoomNo;
	CString m_strReference;
	CString m_strInvoiceNum;

	enum { IDD = IDD_PMS_ACCOUNT_FIND };

public:
	virtual BOOL OnInitDialog();

public:
	afx_msg void OnSelectDate();
	afx_msg void OnChangeDateFrom(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeDateTo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonUpdate();
	afx_msg void OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonPrint();
	afx_msg void OnButtonAction1();
	afx_msg void OnButtonAction2();
	afx_msg void OnButtonAction3();
	afx_msg void OnKillFocusReference();
	afx_msg void OnKillFocusInvoiceNum();

	virtual void DoDataExchange(CDataExchange* pDX);    
	DECLARE_MESSAGE_MAP()

private:
	CEdit* GetEditReference();
	CEdit* GetEditInvoiceNum();

public:
	void SelectLine( int nIndex );
	
private:
	int GetArraySize();
	bool PrepareLine( int nIndex );
	int GetSafeSelection();
	int GetSafeSelection( int nMin, int nMax );
	//void UpdateListEntry( int nSel );
	void RemoveListEntry( int nSel );

private:
	void UpdateList( int nSelectStartDay, int nSelectBookingId, int nSelectBookingPart );
	void UpdateListNormal( int nSelectStartDay, int nSelectBookingId, int nSelectBookingPart );
	void UpdateListCancelled( int nSelectStartDay, int nSelectBookingId, int nSelectBookingPart );
	
private:
	void UpdateSearchDates();
	void UpdateListAfterDateChange();

private:
	void DeleteBooking();
	void ProcessSales();
	void ProcessPayments();
	void ProcessInvoice();
	void ProcessBooking();
	bool FinaliseAccountChanges( int nSel, int nBookingId );
	
private:
	bool CheckBooking( int nBookingId, CPMSBookingCSVRecord& BookingRecord, CPMSCustomerCSVRecord& CustomerRecord );
	bool FindBookingPart( int nCheckInDay, int nBookingId, int nBookingPart, int& nPos );
	
private:
	void SaveRecord();

private:
	CArray<CPMSAccountFindInfo,CPMSAccountFindInfo> m_arrayAccountFindInfo;
	
private:
	CSSListCtrlVirtual m_listData;
	CString m_strDisplayLine;
	int m_nType;

private:
	int m_nSearchStartDay;
	int m_nSearchEndDay;
	int m_nSearchRoomNo;
	int m_nSearchReference;
	int m_nSearchInvoiceNum;
	int m_nLastEditedBookingId;

private:
	CDateSelectorRange m_DateSelectorRange;
	CPMSCalendarDlg* m_pCalendarDlg;
};


/**********************************************************************/
#endif
/**********************************************************************/
#endif
/**********************************************************************/
