#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/
#include "MultilineTextBuffer.h"
/**********************************************************************/

class CPMSEmailSignatures
{
public:
	CPMSEmailSignatures();

public:
	void ReadAll();
	void WriteAllIfChanged();

public:
	void CopyBufferToEditBox( int nGroup, CEdit& edit );
	void CopyEditBoxToBuffer( int nGroup, CEdit& edit );

private:
	CMultilineTextBuffer m_arrayBuffer[5];
};

/**********************************************************************/
#endif
/**********************************************************************/
#endif
/**********************************************************************/
