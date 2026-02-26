#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/

class CZSalesImporterStockman  
{
public:
	CZSalesImporterStockman();
	void LookForSales( int nLocIdx, int nEcrIdx );
	void LookForPMSSales();

private:
	void ProcessPendingFiles( bool bPMS );
	bool ProcessPendingFile( CString& strPath, CString& strFilename, bool bPMS );

protected:
	void ProcessSale( CStringArray& StockDetails, bool bWastage, bool bPMS );
	void ProcessException( CStringArray& StockDetails, bool bWastage, bool bPMS );
	
	void WriteHistoryBuffer();
	void WriteAuditBuffers( bool bWastage );
	void WriteRetryBuffer( bool bPMS );

protected:
	int m_nLocIdx;
	int m_nEcrIdx;
	int m_nDbIdx;
	int m_nSpIdx;
	int m_nNetworkIdx;
	CStringArray m_arrayFilesToImport;
	COleDateTime m_auditDate;
	CString m_strEcrName;

protected:
	CString m_strDateNow;
	CString m_strTimeNow;
	CString m_strDateOfSale;
	CString m_strTimeOfSale;
	CString m_strReportDate;
	CString m_strSalesHistoryMonth;
	CString m_strRetryDate;
	
protected:
	CString m_strLinePluNo;
	CString m_strLineShortPluNo;
	double m_dLinePluQty;
	double m_dLinePluValue;
	int m_nLinePluLevel;

protected:
	CStringArray m_arrayAuditBufferSales;
	CStringArray m_arrayExceptionsBufferSales;
	CStringArray m_arrayAuditBufferWastage;
	CStringArray m_arrayExceptionsBufferWastage;
	CStringArray m_arrayRetryBuffer;
	CStringArray m_arrayHistoryBuffer;
};

/**********************************************************************/
#endif
/**********************************************************************/
