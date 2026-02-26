#pragma once
/**********************************************************************/
#include "CustomerCSVArray.h"
#include "RepCSV.h"
#include "TermFileListInfo.h"
/**********************************************************************/

class CCustomerExtractor
{
public:
	CCustomerExtractor( int nLocIdx, const char* szDateFrom, const char* szDateTo, CCustomerCSVArray& arrayCustomers );
	~CCustomerExtractor(void);

private:
	void GetTermFileList();
	void ExtractCustomers();

private:
	int m_nLocIdx;
	CString m_strCheckDateFrom;
	CString m_strCheckDateTo;

private:
	CCustomerCSVArray& m_arrayCustomers;
	CArray<CTermFileListInfo,CTermFileListInfo> m_arrayFileInfo;
	int m_nTotalFileSize;
};

/**********************************************************************/