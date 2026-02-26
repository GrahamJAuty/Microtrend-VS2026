/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include "PMSEmailSignatureBuffer.h"
/**********************************************************************/

CPMSEmailSignatures::CPMSEmailSignatures()
{
	for ( int n = 0; n < 5; n++ )
		m_arrayBuffer[n].SetPMSGroupSource(n + 1);
}

/**********************************************************************/

void CPMSEmailSignatures::ReadAll()
{
	for ( int n = 0; n < 5; n++ )
		m_arrayBuffer[n].Read();
}

/**********************************************************************/

void CPMSEmailSignatures::WriteAllIfChanged()
{
	for ( int n = 0; n < 5; n++ )
		m_arrayBuffer[n].WriteIfChanged();
}

/**********************************************************************/

void CPMSEmailSignatures::CopyBufferToEditBox( int nGroup, CEdit& edit )
{
	if ( ( nGroup >= 1 ) && ( nGroup <= 5 ) )
		m_arrayBuffer[ nGroup - 1 ].CopyBufferToEditBox( edit );
}

/**********************************************************************/

void CPMSEmailSignatures::CopyEditBoxToBuffer( int nGroup, CEdit& edit )
{
	if ( ( nGroup >= 1 ) && ( nGroup <= 5 ) )
		m_arrayBuffer[ nGroup - 1 ].CopyEditBoxToBuffer( edit );
}

/**********************************************************************/
#endif
/**********************************************************************/
