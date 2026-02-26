#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "Consolidation.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

class CKbdListNameArray
{
public:
	CKbdListNameArray();

public:
	void Read(int nDbIdx);
	void Read(int nDbIdx, int nLocIdx, int nPriceSet);
	void SetFakeReadFlag() { m_bFakeRead = TRUE;  }
	void ClearFakeReadFlag() { m_bFakeRead = FALSE; }

public:
	int GetSize() { return m_array.GetSize(); }
	int GetListNo(int n);
	const char* GetListName(int n);
	bool GetOfferPluLinkFlag(int n);

private:
	void ReloadFromMemory();

private:
	CReportConsolidationArray<CSortedIntAndStringByInt> m_array;
	CString m_strName;
	bool m_bFakeRead;
};

/**********************************************************************/
extern CKbdListNameArray KbdListNameArray;
/**********************************************************************/
#endif
/**********************************************************************/
