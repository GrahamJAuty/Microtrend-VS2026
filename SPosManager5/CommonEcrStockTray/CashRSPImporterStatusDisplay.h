#pragma once
/**********************************************************************/
#include "..\SPosTray5\MainFrmPosTray.h"
/**********************************************************************/

class CCashRSPImporterStatusDisplay
{
public:
	CCashRSPImporterStatusDisplay(void);

public:
	virtual bool CanDisplayStatus(){ return FALSE; }
	virtual void DisplayStatus( const char* szStatus ){}
};

/**********************************************************************/
#ifdef POSTRAY_UTILITY
/**********************************************************************/

class CCashRSPImporterStatusDisplayPosTray : public CCashRSPImporterStatusDisplay 
{
public:
	CCashRSPImporterStatusDisplayPosTray( CMainFramePosTray* pMainFrame );

public:
	virtual bool CanDisplayStatus(){ return TRUE; }
	virtual void DisplayStatus( const char* szStatus );

private:
	CMainFramePosTray* m_pMainFrame;
};

/**********************************************************************/
#endif
/**********************************************************************/
