#pragma once
/**********************************************************************/

struct CLoyRspThreadInfo
{
public:
	CLoyRspThreadInfo();

public:
	CStringArray* m_pResponseBuffer;
};

/**********************************************************************/

class CLoyRspThread
{
public:
	static UINT DoWork( LPVOID pParam );

private:
	static void DoWorkInternal( CLoyRspThreadInfo* pInfo );
};

/**********************************************************************/
