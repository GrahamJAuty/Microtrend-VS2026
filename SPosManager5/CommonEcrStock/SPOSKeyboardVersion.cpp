/**********************************************************************/
#include "SPOSKeyboardVersion.h"
/**********************************************************************/

CSPOSKeyboardVersion::CSPOSKeyboardVersion()
{
	Reset();
}

/**********************************************************************/

void CSPOSKeyboardVersion::Reset()
{
	m_nSPOSVersion = SPOS_V4;
}

/**********************************************************************/

void CSPOSKeyboardVersion::SetSPOSVersion( int n )
{
	switch( n )
	{
	case SPOS_V4:
	case SPOS_V4_210:
	case SPOS_V4_321:
	case SPOS_V4_389:
	case SPOS_V4_423:
	case SPOS_V4_425:
	case SPOS_V4_494:
	case SPOS_V4_614:
	case SPOS_V4_634:
	case SPOS_V4_682:
	case SPOS_V4_719:
	case SPOS_V4_753:
	case SPOS_V4_780:
	case SPOS_V4_827:
	case SPOS_V4_837:
	case SPOS_V4_861:
	case SPOS_V4_958:
	case SPOS_V4_992:
	case SPOS_V4_999:
	case SPOS_V4_1071:
	case SPOS_V4_1092:
	case SPOS_V4_1096:
	case SPOS_V4_1124:
	case SPOS_V4_1216:
	case SPOS_V4_1300:
	case SPOS_V4_1392:
	case SPOS_V4_1510:
	case SPOS_V4_1519:
	case SPOS_V4_1522:
	case SPOS_V4_1525:
	case SPOS_V4_1734:
	case SPOS_V4_1901:
	case SPOS_V4_1977:
	case SPOS_V4_2012:
	case SPOS_V4_2067:
	case SPOS_V4_2133:
	case SPOS_V4_2161:
	case SPOS_V4_2455:
	case SPOS_V4_2606:
	case SPOS_V4_2_369:
	case SPOS_V4_2_631:
	case SPOS_V4_2_714:
	case SPOS_V4_2_919:
	case SPOS_V4_2_959:
	case SPOS_V4_2_1081:
	case SPOS_V4_2_1087:
	case SPOS_V4_2_1384:
	case SPOS_V4_2_1456:
	case SPOS_V4_2_1530:
	case SPOS_V4_2_1579:
	case SPOS_V4_2_1605:
	case SPOS_V4_2_1618:
	case SPOS_V4_2_1622:
	case SPOS_V4_3_18:
	case SPOS_V4_3_93:
	case SPOS_V4_3_104:
		m_nSPOSVersion = n;
		break;
	}
}

/**********************************************************************/
