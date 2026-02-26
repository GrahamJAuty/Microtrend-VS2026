#pragma once
/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/

class CPMSReportUnpaid
{
public:
	CPMSReportUnpaid();
	void CreateReport();

public:
	void SetIncludePendingFlag( bool b ){ m_bIncludePending = b; }
	void SetIncludeActiveFlag( bool b ){ m_bIncludeActive = b; }
	void SetIncludeCompleteFlag( bool b ){ m_bIncludeComplete = b; }
	void SetIncludeCancelledFlag( bool b ){ m_bIncludeCancelled = b; }
	void SetLinesFlag( bool b ){ m_bLines = b; }
	void SetIncludeTrainingFlag( bool b ){ m_bIncludeTraining = b; }

private:
	bool CreateReportInternal();

private:
	void AddColumnInvoiceNum();
	void AddColumnBookingRef();
	void AddColumnRoomNo();
	void AddColumnRoomName();
	void AddColumnCheckIn();
	void AddColumnNights();
	void AddColumnCheckOut();
	void AddColumnGuests();
	void AddColumnGuestName();
	void AddColumnBillingName();
	void AddColumnBalance();
	void AddColumnCR();

private:
	CReportFile m_ReportFile;
	CString m_strParamsFilename;
	bool m_bIncludePending;
	bool m_bIncludeActive;
	bool m_bIncludeComplete;
	bool m_bIncludeCancelled;
	bool m_bLines;
	bool m_bIncludeTraining;
};

/**********************************************************************/
#endif
/**********************************************************************/
