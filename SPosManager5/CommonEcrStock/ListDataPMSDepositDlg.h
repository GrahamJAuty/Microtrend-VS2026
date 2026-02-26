#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/
#include "ListDataDlg.h"
#include "..\CommonEcrStock\SingleListHelpers.h"
/**********************************************************************/

class CSingleListContentHelpersPMSDeposit : public CSingleListContentHelpers
{
public:
	CSingleListContentHelpersPMSDeposit( CPMSBookingCSVRecord& BookingRecordBuffer );

public:
	int GetArraySize();
	bool PrepareLine( int nIndex, CString& strLine );
	void DeleteDatabaseEntry( int nIndex );
	void SetColumns( CSSListCtrlVirtual& list, CWnd* pParent );

private:
	CPMSBookingCSVRecord& m_BookingRecordBuffer;
};

/**********************************************************************/

class CListDataPMSDepositDlg : public CListDataDlg
{
public:
	CListDataPMSDepositDlg( CPMSBookingCSVRecord& BookingRecord, CWnd* pParent = NULL); 
	~CListDataPMSDepositDlg();
	
private:
	virtual void SetTitle();
	virtual void SetButtonTexts();
	virtual void SetColumns();

	virtual UINT GetDefaultButtonID() { return IDOK; }
	virtual void HandleDefaultButton() { HandleButton3(); }
	virtual void HandleButton1();
	virtual void HandleButton2();
	virtual void HandleButton3();
	virtual void HandleButton4();
	virtual bool HandleOK();

private:
	CPMSBookingCSVRecord& m_BookingRecord;
	CPMSBookingCSVRecord m_BookingRecordBuffer;
};

/**********************************************************************/
#endif
/**********************************************************************/
#endif
/**********************************************************************/
