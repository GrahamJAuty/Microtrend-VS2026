/**********************************************************************/
#ifndef SPOSKEYBOARDVERSION_H
#define SPOSKEYBOARDVERSION_H
/**********************************************************************/
#include "SPOSVersions.h"
/**********************************************************************/

class CSPOSKeyboardVersion
{
public:
	CSPOSKeyboardVersion();
	void Reset();

public:
	void SetSPOSVersion( int n );
	int GetSPOSVersion(){ return m_nSPOSVersion; }

private:
	int m_nSPOSVersion;
};

/**********************************************************************/
#endif
/**********************************************************************/
