#pragma once
/**********************************************************************/
 
/**********************************************************************/
#define MODIFIER_VERSION 2
/**********************************************************************/
#define MODIFIER_COUNT 9
/**********************************************************************/

class CModifierCSVRecord : public CCSVRecord
{
public:
	CModifierCSVRecord(); 
	virtual ~CModifierCSVRecord() {}
	virtual void ConvertToCSV ( CCSV& csv ); 
	virtual void ConvertFromCSV ( CCSV& csv ); 
	
public:
	const char* GetName() { return m_strName; }
	double GetMultiplierForQty() { return m_dMultiplierForQty; }
	double GetMultiplierForPoints() { return m_dMultiplierForPoints; }

	void SetName( const char* szName );
	void SetMultiplierForQty( double dMultiplier );
	void SetMultiplierForPoints( double dMultiplier );

private:
	void ClearRecord();
	void V1ConvertFromCSV( CCSV& csv );
	void V2ConvertFromCSV( CCSV& csv );
	
private:
	CString m_strName;
	CString m_strDisplayName;
	double m_dMultiplierForQty;
	double m_dMultiplierForPoints;
};

/**********************************************************************/

class CModifierCSVArray : public CSharedCSVArray
{
public:
	CModifierCSVArray();
	~CModifierCSVArray();
	int Open( const char* szFilename, int nMode );

public:
	double GetMultiplierForQty( int nModifier );
	double GetMultiplierForPoints( int nModifier );
	const char* GetDisplayName( int nModifier );

	void SetMultiplierForQty( int nModifier, double dMultiplier );
	void SetMultiplierForPoints( int nModifier, double dMultiplier );
	void SetName( int nModifier, const char* szName );

private:
	CString m_strDisplayName;
};

/**********************************************************************/
