/**********************************************************************/
#include "DiagnosticTimer.h"
/**********************************************************************/

CDiagnosticTimer::CDiagnosticTimer()
{
	Begin();
}

/**********************************************************************/

void CDiagnosticTimer::Begin()
{
	m_startTime = GetTickCount64();
}

/**********************************************************************/

void CDiagnosticTimer::Report()
{
	LONGLONG endTime = GetTickCount64();

	CString str = "";
	str.Format("%I64d", endTime - m_startTime);
	AfxMessageBox(str);
}

/**********************************************************************/
