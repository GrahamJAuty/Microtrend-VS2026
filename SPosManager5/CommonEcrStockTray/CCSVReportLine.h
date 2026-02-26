#pragma once
/**********************************************************************/
 
/**********************************************************************/

class CCSVReportLine : public CCSV
{
public:
	CCSVReportLine() : CCSV ( '\t' ) {};
	CCSVReportLine( bool bRemoveTrailing, bool bRemoveZeros, bool bMinimiseFloat ) : CCSV ( '\t', '"', bRemoveTrailing, bRemoveZeros, bMinimiseFloat ) {};
	void AppendLineQty ( double dQty, int nDps = -1 );
	void AppendVal ( double dVal, int nDps = -1 );
	void AppendInt ( int nVal );
	void AppendString ( const char* szText );
	void AppendBlanks ( int nCount );
	void AppendYesOrBlank( bool bYes );
	void AppendPercent( double dVal, int nDps = -1 );
};

/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/

class CStockReportLine : public CCSVReportLine
{
public:
	CStockReportLine() : CCSVReportLine() {};
	void AppendStockQty ( double dQty, int nSubUnits = 1, bool bSingleItem = TRUE, int nDps = -1 );
	const char* GetStockLevelString ( double dValue, int nSubUnits );	

private:
	CString m_strStockLevel;
};

/**********************************************************************/
#endif
/**********************************************************************/
