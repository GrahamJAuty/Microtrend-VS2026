#pragma once
/**********************************************************************/
#include "TermFileListInfo.h"
/**********************************************************************/

class CCustomerActivityChecker
{
public:
	CCustomerActivityChecker( int nLocIdx );
	~CCustomerActivityChecker(void);

private:
	void GetTermFileList();
	void CheckActivity();

private:
	int m_nLocIdx;
	CString m_strCheckDateFrom;
	CString m_strCheckDateTo;

private:
	CArray<CTermFileListInfo,CTermFileListInfo> m_arrayFileInfo;
	int m_nTotalFileSize;
};

/**********************************************************************/