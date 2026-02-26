#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/

class CZDeliveryImporterStockman  
{
public:
	CZDeliveryImporterStockman();
	void LookForDeliveries( int nLocIdx, int nEcrIdx );
	
private:
	void ProcessPendingFiles();
	bool ProcessPendingFile( CString& strPath, CString& strFilename );

protected:
	void ProcessDelivery( CStringArray& StockDetails );
	void ProcessException( CStringArray& StockDetails );
	
	void WriteHistoryBuffer();
	void WriteAuditBuffers();
	void WriteRetryBuffer();

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
	CStringArray m_arrayAuditBuffer;
	CStringArray m_arrayExceptionsBuffer;
	CStringArray m_arrayRetryBuffer;
};

/**********************************************************************/
#endif
/**********************************************************************/
