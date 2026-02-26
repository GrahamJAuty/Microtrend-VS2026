#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/

class CReorderImportHelper
{
public:
	CReorderImportHelper(void);

public:
	bool CheckHeader( const char* szHeader );
	void ShowHeaderError();

public:
	bool IsWrapperImport(){ return m_bWrapperMode; }
	bool GotMinField(){ return (m_nFieldIdxMin != -1); }
	bool GotReorderField(){ return (m_nFieldIdxReorder != -1); }
	bool GotMaxField(){ return (m_nFieldIdxMax != -1); }

public:
	double GetMinQty( CCSV& csv );
	double GetReorderQty( CCSV& csv );
	double GetMaxQty( CCSV& csv );

private:
	bool m_bAllowReorder;
	
private:
	bool m_bWrapperMode;
	int m_nFieldIdxMin;
	int m_nFieldIdxReorder;
	int m_nFieldIdxMax;
	int m_nHeaderError;
};

/**********************************************************************/
#endif
/**********************************************************************/
