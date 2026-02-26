//**********************************************************************
#include "AccountIdentifier.h"
//**********************************************************************

CAccountIdentifier::CAccountIdentifier( const char* szID, int nIDType )
{
	m_strID = szID;
	
	switch( nIDType )
	{
	case nINTERFACE_SWIPEv1:
	case nINTERFACE_SWIPEv2:
	case nINTERFACE_MIFAREv1:
	case nINTERFACE_MIFAREv2:
	case nINTERFACE_MIFAREv3:
	case nINTERFACE_GENERALv1:
	case nINTERFACE_QRINFO:
	case nINTERFACE_SIMPSINNS:
		m_nIDType = nIDType;
		break;

	default:
		m_nIDType = nINTERFACE_SWIPEv1;
		break;
	}
}

//**********************************************************************
