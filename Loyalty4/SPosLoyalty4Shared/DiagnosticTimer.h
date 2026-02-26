#pragma once
/**********************************************************************/

class CDiagnosticTimer  
{
public:
	CDiagnosticTimer();

public:
	void Begin();
	void Report();

private:
	LONGLONG m_startTime;
};

/**********************************************************************/
