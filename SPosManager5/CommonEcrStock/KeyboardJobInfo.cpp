/**********************************************************************/
 
/**********************************************************************/
#include "JobNumbers.h"
/**********************************************************************/
#include "KeyboardJobInfo.h"
/**********************************************************************/

CKeyboardJobInfo::CKeyboardJobInfo()
{
	m_nLocIdx = 0;
	m_nKbNo = 1;
}

/**********************************************************************/

int CKeyboardJobInfo::GetJobNo()
{
	switch( m_nKbNo )
	{
	case 1:		return DN_KBD1;
	case 2:		return DN_KBD2;
	case 3:		return DN_KBD3;
	case 4:		return DN_KBD4;
	case 5:		return DN_KBD5;
	case 6:		return DN_KBD6;
	case 7:		return DN_KBD7;
	case 8:		return DN_KBD8;
	case 9:		return DN_KBD9;
	case 10:	return DN_KBD10;
	case 11:	return DN_KBD11;
	case 12:	return DN_KBD12;
	case 13:	return DN_KBD13;
	case 14:	return DN_KBD14;
	default:	return 0;
	}
}

/**********************************************************************/

int CKeyboardJobInfo::GetKbNoFromJobNo( int nJobNo )
{
	switch( nJobNo )
	{
	case DN_KBD1:		return 1;
	case DN_KBD2:		return 2;
	case DN_KBD3:		return 3;
	case DN_KBD4:		return 4;
	case DN_KBD5:		return 5;
	case DN_KBD6:		return 6;
	case DN_KBD7:		return 7;
	case DN_KBD8:		return 8;
	case DN_KBD9:		return 9;
	case DN_KBD10:		return 10;
	case DN_KBD11:		return 11;
	case DN_KBD12:		return 12;
	case DN_KBD13:		return 13;
	case DN_KBD14:		return 14;
	default:			return 0;
	}
}

/**********************************************************************/
