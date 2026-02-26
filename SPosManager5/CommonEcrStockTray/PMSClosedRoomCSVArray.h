#pragma once
/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#define PMS_CLOSEDROOM_VERSION 1
/**********************************************************************/
	
namespace PMSClosure
{
	const recInt		ClosureId =			{ "ClosureId", 1, 99999999 };
	const recInt		StartDay =			{ "StartDay", 0, 99999 };
	const recInt		Nights =			{ "Nights", 0, 366 };
	const recString		Reason =			{ "Reason", 0, 30 };
};

/**********************************************************************/

class CPMSClosedRoomCSVRecord : public CCSVRecord
{
public:
	CPMSClosedRoomCSVRecord(); 
	virtual ~CPMSClosedRoomCSVRecord() {}
	virtual void ConvertToCSV ( CCSV& csv ); 
	virtual void ConvertFromCSV ( CCSV& csv );

public:
	void ClearRecord();
	
public:
	int GetClosureId()				{ return m_nClosureId; }
	int GetRoomNo()					{ return m_nRoomNo; }
	int GetStartDay()				{ return m_nStartDay; }
	int GetNights()					{ return m_nNights; }
	const char* GetReason()			{ return m_strReason; }
	
	
	void SetClosureId( int n );
	void SetRoomNo( int n );
	void SetStartDay( int n );
	void SetNights( int n );
	void SetReason( const char* sz );
	
private:
	void V1ConvertFromCSV( CCSV& csv );
	
private:
	int m_nClosureId;
	int m_nRoomNo;
	int m_nStartDay;
	int m_nNights;
	CString m_strReason;
};

/**********************************************************************/

class CPMSClosedRoomCSVArray : public CSharedCSVArray
{
public:
	CPMSClosedRoomCSVArray();
	~CPMSClosedRoomCSVArray();
	int Open( const char* szFilename, int nMode );

public:
	int GetClosureId( int nIdx );
	bool FindClosureById( int nId, int& nClosureIdx );
	int FindFirstFreeId();

public:
	void InsertClosure( CPMSClosedRoomCSVRecord& Closure );
	void RemoveClosureById( int nClosureId );
	
public:
	void BuildInternalTables();
};

/**********************************************************************/
#endif
/**********************************************************************/
