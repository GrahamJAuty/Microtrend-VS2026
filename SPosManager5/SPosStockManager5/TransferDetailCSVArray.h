/**********************************************************************/
#pragma once
/**********************************************************************/
#if defined(STOCKMAN_SYSTEM) || defined(POSTRAY_UTILITY)
/**********************************************************************/
 
/**********************************************************************/
#define TRANSFERDB_VERSION 9
/**********************************************************************/

struct CTransferDetailInfo
{
public:
	CTransferDetailInfo();

public:
	int m_nSpNo;
	double m_dTransferInQty;
	double m_dTransferOutQty;
	double m_dStocktakeTransferInQty;
	double m_dStocktakeTransferOutQty;
};

/**********************************************************************/

class CTransferDetailArray
{
public:
	CTransferDetailArray(){};
	~CTransferDetailArray(){};

public:
	void Reset();

public:
	void GetTransferDetailInfo( CTransferDetailInfo& info );
	void GetTransferDetailInfo( int nIdx, CTransferDetailInfo& info );
	
	void SetTransferDetailInfo( CTransferDetailInfo& info );
	void SetTransferDetailInfo( int nIdx, CTransferDetailInfo& info );

	void AddTransferDetailInfo( CTransferDetailInfo& info );
	
	void RemoveTransferDetailInfo( int nIdx );

	void AddTransferDetailToCSVLine( CCSV& csv );

public:
	int GetSpCount(){ return m_arrayDetail.GetSize(); }

private:
	bool FindTransferDetailInfo( int nSpNo, int& nIdx );

private:
	CArray<CTransferDetailInfo,CTransferDetailInfo> m_arrayDetail;
};

/**********************************************************************/

class CTransferDetailCSVRecord : public CCSVRecord
{
public:
	CTransferDetailCSVRecord();
	virtual ~CTransferDetailCSVRecord();
	
public:
	void Reset();

	virtual void ConvertToCSV ( CCSV& csv ); 
	virtual void ConvertFromCSV ( CCSV& csv ); 
	
	CTransferDetailCSVRecord& operator= ( CTransferDetailCSVRecord& source );
	CTransferDetailCSVRecord& operator+ ( CTransferDetailCSVRecord& source );

	const char* GetStockCode() { return m_strStockCode; }
	
	void BeginStocktake();
	void AbandonStocktake();
	void FinaliseStocktake( int nType );

	void SetStockCode( const char* szStockCode ) { m_strStockCode = szStockCode; }	
	
	void GetTransferDetailInfo( CTransferDetailInfo& info ){ m_arrayDetail.GetTransferDetailInfo( info ); }
	void GetTransferDetailInfo( int nIdx, CTransferDetailInfo& info ){ m_arrayDetail.GetTransferDetailInfo( nIdx, info ); }
	
	void SetTransferDetailInfo( CTransferDetailInfo& info ){ m_arrayDetail.SetTransferDetailInfo( info ); }
	void SetTransferDetailInfo( int nIdx, CTransferDetailInfo& info ){ m_arrayDetail.SetTransferDetailInfo( nIdx, info ); }

	void AddTransferDetailInfo( CTransferDetailInfo& info ){ m_arrayDetail.AddTransferDetailInfo( info ); }	
	
	void AddTransferDetailToCSVLine( CCSV& csv ){ m_arrayDetail.AddTransferDetailToCSVLine( csv ); }
	
	void RemoveTransferDetailInfo( int nIdx ){ m_arrayDetail.RemoveTransferDetailInfo( nIdx ); }

public:
	int GetSpCount(){ return m_arrayDetail.GetSpCount(); }

private:
	void ClearRecord();
	void V9ConvertFromCSV( CCSV& csv );
	
	bool IsZero( CTransferDetailInfo& info );
	
private:
	CString m_strStockCode;

private:
	CTransferDetailArray m_arrayDetail;
};

/**********************************************************************/

class CTransferDetailCSVArray : public CSharedCSVArray
{
public:
	CTransferDetailCSVArray( int nSpNo );
	~CTransferDetailCSVArray();
	void RemoveAll();
	
	int GetSpNo() { return m_nSpNo; }
	
	const char* GetStockCode( int nIndex );
	bool FindStockItemByCode ( int nMasterIdx, const char* szCode, int& nIndex );
	
	bool IsInSync() { return m_bInSync; }
	void SetSyncFlag( bool bFlag ) { m_bInSync = bFlag; }

private:
	bool m_bInSync;
	int m_nSpNo;

private:
	CString m_strStockCode;
};

/**********************************************************************/
#endif
/**********************************************************************/
