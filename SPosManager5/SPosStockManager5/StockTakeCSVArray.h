#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
 
/**********************************************************************/
#define STOCKTAKE_VERSION 4
#define MAX_STOCKTAKES 20
/**********************************************************************/

class CStocktakeCSVRecord : public CCSVRecord
{
public:
	CStocktakeCSVRecord(); 
	virtual ~CStocktakeCSVRecord(){}
	virtual void ConvertToCSV ( CCSV& csv ); 
	virtual void ConvertFromCSV ( CCSV& csv ); 
	
	void SetStocktakeNo( int n );
	void SetStocktakeType( int n );
	void SetClosingStockZeroFlag( bool b ) { m_bClosingStockZero = b; }
	void SetCategory( const char* szCategory ) { m_strCategory = szCategory; }
	void SetSupplier( const char* szSupplier ) { m_strSupplier = szSupplier; }
	void SetActiveOnlyFlag( bool b ) { m_bActiveOnly = b; }
	void SetComment ( const char* szComment ) { m_strComment = szComment; }
	void SetDate ( const char* szDate );
	void SetItemCount( int n ) { m_nItemCount = n; }

	int GetStocktakeNo() { return m_nStocktakeNo; }
	int GetStocktakeType() { return m_nStocktakeType; }
	bool GetClosingStockZeroFlag() { return m_bClosingStockZero; }
	const char* GetStocktakeTypeText();
	const char* GetCategory() { return m_strCategory; }
	const char* GetSupplier() { return m_strSupplier; }
	bool GetActiveOnlyFlag(){ return m_bActiveOnly; }
	const char* GetComment() { return m_strComment; }
	const char* GetDate() { return m_strDate; }
	const char* GetFormattedDate();
	int GetItemCount() { return m_nItemCount; }

	COleDateTime GetOleDateTime();

private:
	void ClearRecord();
	void V1ConvertFromCSV( CCSV& csv );
	void V2ConvertFromCSV( CCSV& csv );
	void V3to4ConvertFromCSV( CCSV& csv, int nVer );

private:
	int m_nStocktakeNo;
	int m_nStocktakeType;
	bool m_bClosingStockZero;
	CString m_strCategory;
	CString m_strSupplier;
	bool m_bActiveOnly;
	CString m_strComment;
	CString m_strDate;
	CString m_strFormattedDate;
	int m_nItemCount;

private:
	CString m_strStocktakeType;
};

/**********************************************************************/

class CStocktakeCSVArray : public CCSVArray
{
public:
	CStocktakeCSVArray();
	bool Read( int nSpIdx );
	bool Write( int nSpIdx );
	void RemoveAll() { RemoveAt( 0, GetSize() ); }

public:
	void AddStocktake( int& nStocktakeIdx, int& nStocktakeNo );
	void RemoveStocktake( int nIndex );
	int GetNextFreeNumber();
};

/**********************************************************************/
#endif
/**********************************************************************/
