#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
 
/**********************************************************************/
#include "ProcessedOrderEntity.h"
#include "ProcessedOrderPreviewDatabase.h"
/**********************************************************************/
#define ORDERHEADER_VERSION 1
/**********************************************************************/

class CProcessedOrderHeader : public CCSVRecord
{
public:
	CProcessedOrderHeader(); //fill with default values
	
	virtual ~CProcessedOrderHeader();
	virtual void ConvertToCSV ( CCSV& csv );
	virtual void ConvertToCSVForFile ( CCSV& csv );
	virtual void ConvertFromCSV ( CCSV& csv ); 

	int GetOrderNum() { return m_nOrderNum; }
	int GetSuppNo() { return m_nSuppNo; }
	const char* GetDateString() { return m_strDate; }
	const char* GetTimeString() { return m_strTime; }
	bool IsModified(){ return m_bModified; }
	int GetOrderType(){ return m_nOrderType; }

	const char* GetDisplayDate();
	
	void SetOrderNum( int nNum ) { m_nOrderNum = nNum; }
	void SetSuppNo( int nNum ) { m_nSuppNo = nNum; }
	void SetDate();
	void SetDateString( const char* szDate ); 
	void SetTimeString( const char* szTime ); 
	void SetModifiedFlag( bool b ){ m_bModified = b; }
	void SetOrderType( int n ){ m_nOrderType = n; }

	CProcessedOrderHeader& operator= ( CProcessedOrderHeader& source );

private:
	void ClearRecord();
	void V1ConvertFromCSV( CCSV& csv );
	
private:
	//Record fields
	int m_nOrderNum;
	int m_nSuppNo;
	CString m_strDate;
	CString m_strTime;
	bool m_bModified;
	int m_nOrderType;

private:
	CString m_strDisplayDate;
};

/**********************************************************************/
#endif
/**********************************************************************/
