/**********************************************************************/
#include "EposSelectArray.h"
/**********************************************************************/

class CTerminalDateRangeListBuilder
{
public:
	CTerminalDateRangeListBuilder(CEposSelectArray& SelectArray, bool bEODMode, int nTermDateType, bool bGuestToTerminal, int nPMSAllowMode, int nPMSDateMode);
	void CreateTerminalList();

public:
	int GetLineCount() { return m_arrayLines.GetSize(); }
	CString GetLine(int n);
	CString GetGuestAccountStatus() { return m_strGuestAccountStatus; }
	
private:
	bool SetGuestAccountStatus();
	void CreateTerminalListTerminal();
	void CreateTerminalListLocation();
	void CreateTerminalListSystem();
	void CreateTerminalListEODTerminal();
	void CreateTerminalListEODLocation();
	void CreateTerminalListEODSystem();
	void ProcessEposDetailsLocationLine(CEposSelectArrayListItem& LocationListItem, int nTerminals, int nWithData, bool bEOD);

private:
	CString GetEODDateRangeString(CSortedDateTimeItem& timeStart, CSortedDateTimeItem& timeEnd);
	CString GetSystemTerminalSummaryString(CEposSelectArrayListItem& SystemListItem, int nTerminals, int nWithData);

private:
	CEposSelectArray& m_SelectArray;

private:
	CStringArray m_arrayLines;
	CString m_strGuestAccountStatus;

private:
	bool m_bEODMode;
	int m_nTermDateType;
	bool m_bGuestToTerminal;
	int m_nPMSAllowMode;
	int m_nPMSDateMode;
};

/**********************************************************************/
