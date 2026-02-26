#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
 
/**********************************************************************/
#include "EposTaskDatabase.h"
/**********************************************************************/
#include "DataAccessHelpers.h"
/**********************************************************************/

class CDataAccessHelpersTask : public CDataAccessHelpers
{
public:
	CDataAccessHelpersTask();
	
public:
	void AdhocTask();
	void PresetTask( int nType );
	bool SetupTasks();
	bool CanRunTask( int nTaskNo );

private:
	void DoTask( CEposTaskDatabase& TaskDatabase, int nTaskNo );
};

/**********************************************************************/
#endif
/**********************************************************************/
