#pragma once
/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/

class CPMSReportRoomList
{
public:
	CPMSReportRoomList();
	void CreateReport();

	void SetFilterOccupied1( bool b ){ m_bOccupied1 = b; }
	void SetFilterOccupied2( bool b ){ m_bOccupied2 = b; }
	void SetFilterOccupied3( bool b ){ m_bOccupied3 = b; }
	void SetFilterVacant1( bool b ){ m_bVacant1 = b; }
	void SetFilterVacant2( bool b ){ m_bVacant2 = b; }
	void SetFilterVacant3( bool b ){ m_bVacant3 = b; }
	void SetBalanceFlag( bool b ){ m_bBalance = b; }
	void SetLinesFlag( bool b ){ m_bLines = b; }
	void SetTrainingFlag( bool b ){ m_bTraining = b; }

private:
	bool CreateRoomListReport();
	bool IsRoomBooked( int nRoomNo, int nDayNumber );

private:
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
	bool m_bOccupied1;
	bool m_bOccupied2;
	bool m_bOccupied3;
	bool m_bVacant1;
	bool m_bVacant2;
	bool m_bVacant3;
	bool m_bBalance;
	bool m_bLines;
	bool m_bTraining;
};

/**********************************************************************/
#endif
/**********************************************************************/
