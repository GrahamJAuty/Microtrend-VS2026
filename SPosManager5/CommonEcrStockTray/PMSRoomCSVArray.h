#pragma once
/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
/*	ROOM DATABASE												
/**********************************************************************/
#define ROOMDB_VERSION 5
/**********************************************************************/
#define PMS_ERROR_NONE 0
#define PMS_ERROR_ABANDON 1
#define PMS_ERROR_FOLDER_NONE 2
#define PMS_ERROR_FOLDER_NAME 3
#define PMS_ERROR_FOLDER_EXIST 4
/**********************************************************************/

namespace PMSRoom
{
const recInt		RoomNo				= { "RoomNo", 1, 9999 };
const recString		RoomName			= { "RoomName", 0, 15 };
const recInt		RoomGroup			= { "RoomGroup", 1, 5 };
const recInt		Sleeps				= { "Sleeps", 1, 6 };
const recInt		RoomType			= { "RoomType", 0, 5 };
const recDouble		RoomRate			= { "RoomRate", 0.0, 500.0 };
const recDouble		ExtendedRoomRate	= { "ExtendedRoomRate", 0.0, 99999.99 };
const recString		RoomTypeText		= { "RoomTypeText", 0, 15 };
};

/**********************************************************************/

class CPMSRoomCSVRecord : public CCSVRecord
{
public:
	CPMSRoomCSVRecord();
	virtual ~CPMSRoomCSVRecord() {}
	virtual void ConvertToCSV ( CCSV& csv );
	virtual void ConvertFromCSV ( CCSV& csv );

	CString StripSpaces ( const char* szText );

public:
	int GetRoomNo()					{ return m_nRoomNo; }
	const char* GetRoomName()		{ return m_strRoomName; }
	int GetSleeps()					{ return m_nSleeps; }
	int GetRoomType()				{ return m_nRoomType; }
	int GetRoomGroup()				{ return m_nRoomGroup; }
	bool GetRoomSingleRateFlag()	{ return m_bRoomSingleRate; }
	double GetRoomOnlyRate()		{ return m_dRoomOnlyRate; }
	double GetRoomBBRate()			{ return m_dRoomBBRate; }
	double GetRoomDBBRate()			{ return m_dRoomDBBRate; }
	double GetRoomOnlySingleRate()	{ return m_dRoomOnlySingleRate; }
	double GetRoomBBSingleRate()	{ return m_dRoomBBSingleRate; }
	double GetRoomDBBSingleRate()	{ return m_dRoomDBBSingleRate; }
	bool GetRoomAllowExtraBedFlag()	{ return m_bRoomAllowExtraBed; }
	double GetRoomExtraBedRate()	{ return m_dRoomExtraBedRate; }
	bool GetRoomAllowExtraCotFlag()	{ return m_bRoomAllowExtraCot; }
	double GetRoomExtraCotRate()	{ return m_dRoomExtraCotRate; }
	
public:
	void SetRoomNo					( int n );
	void SetRoomName				( const char* sz );
	void SetSleeps					( int n );
	void SetRoomType				( int n );
	void SetRoomGroup				( int n );
	void SetRoomSingleRateFlag		( bool b ){ m_bRoomSingleRate = b; }
	void SetRoomOnlyRate			( double d );
	void SetRoomBBRate				( double d );
	void SetRoomDBBRate				( double d );
	void SetRoomOnlySingleRate		( double d );
	void SetRoomBBSingleRate		( double d );
	void SetRoomDBBSingleRate		( double d );
	void SetRoomAllowExtraBedFlag	( bool b ){ m_bRoomAllowExtraBed = b; }
	void SetRoomExtraBedRate		( double d );
	void SetRoomAllowExtraCotFlag	( bool b ){ m_bRoomAllowExtraCot = b; }
	void SetRoomExtraCotRate		( double d );
	
public:
	const char* GetRoomTypeName( bool bBrackets );
	const char* GetDefaultName();
	
private:
	void ClearRecord();
	void V1To5ConvertFromCSV ( CCSV& csv, int nVer );

private:
	int m_nRoomNo;
	CString m_strRoomName;
	int m_nSleeps;
	int m_nRoomType;
	int m_nRoomGroup;
	bool m_bRoomSingleRate;
	double m_dRoomOnlyRate;
	double m_dRoomBBRate;
	double m_dRoomDBBRate;
	double m_dRoomOnlySingleRate;
	double m_dRoomBBSingleRate;
	double m_dRoomDBBSingleRate;
	bool m_bRoomAllowExtraBed;
	double m_dRoomExtraBedRate;
	bool m_bRoomAllowExtraCot;
	double m_dRoomExtraCotRate;
	
private:
	CString m_strRoomTypeName;
	CString m_strDefaultName;
};

/**********************************************************************/

class CPMSRoomCSVArray : public CSharedCSVArray
{
public:
	CPMSRoomCSVArray();

public:
	int Open( const char* szFilename, int nMode );

	const char* GetRoomNameByRoomNo( int nRoomNo );
	const char* GetDefaultNameByRoomNo( int nRoomNo );

	bool FindRoomByNumber ( int nRoomNo, int& nIndex );

public:
	int CheckPOSFolders();

private:
	int CheckRoomFolderInternal();
	int CheckPostFolderInternal();

private:
	CString m_strRoomName;
	CString m_strDefaultName;
};

/**********************************************************************/
#endif
/**********************************************************************/
