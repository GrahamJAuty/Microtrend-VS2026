#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#define SE_READDATES_OK 0
#define SE_READDATES_NOFILE 1
#define SE_READDATES_ERROR 2
/**********************************************************************/
#define SE_WEBDATE_NONE 0
#define SE_WEBDATE_NEXT 1
/**********************************************************************/
 
/**********************************************************************/

class CSmartEntertainmentWebAPISalesDate
{
public:
	CSmartEntertainmentWebAPISalesDate(void);
	
public:
	const char* GetWebAPIDateString();
	
public:
	bool CreateFile( int nNetworkIdx );
	int Read( int nNetworkIdx );
	bool Write();

public:
	int GetDateStatus(){ return m_nDateStatus; }
	const char* GetDisplayDateNextRead();

public:
	void GetNextReadDate( COleDateTime& datetime );
	void SetNextReadDate( COleDateTime& datetime );
	
private:
	void ResetDate();
	bool ReadV1Line( CCSV& csv );
	bool StringToOleDateTime( const char* sz, COleDateTime& datetime );
	void OleDateTimeToString( COleDateTime& date, CString& str );

private:
	int m_nNetworkIdx;
	int m_nDateStatus;
	COleDateTime m_oleDateTimeNextRead;
	
private:
	CString m_strWebAPIDateString;
	CString m_strDisplayDateNextRead;
};

/**********************************************************************/
#endif
/**********************************************************************/