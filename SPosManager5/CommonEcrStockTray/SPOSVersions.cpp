/**********************************************************************/
#include "SPOSVersions.h"
/**********************************************************************/

int CSPOSVersions::GetSPOSBaseVersion( int nVersion )
{
	if (CheckSPOSVersion(nVersion, SPOS_V4) == TRUE)
	{
		return SPOS_V4;
	}

	return -1;
}

/**********************************************************************/

bool CSPOSVersions::CheckSPOSVersion( int nVersion, int nMinVersion )
{
	switch( nVersion )
	{
	case SPOS_V4:			
	case SPOS_V4_210:		
	case SPOS_V4_321:		
	case SPOS_V4_389:		
	case SPOS_V4_423:		
	case SPOS_V4_425:		
	case SPOS_V4_494:		
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
		break;

	default:
		return FALSE;
	}

	int nFeature1 = GetSPOSFeatureNumber( nVersion );
	if (0 == nFeature1)
	{
		return FALSE;
	}

	int nFeature2 = GetSPOSFeatureNumber( nMinVersion );
	if (0 == nFeature2)
	{
		return FALSE;
	}

	return ( nFeature1 >= nFeature2 );
}

/**********************************************************************/

bool CSPOSVersions::IsValidSPOSVersion( int nVersion )
{
	return ( GetSPOSFeatureNumber( nVersion ) != 0 );
}

/**********************************************************************/

int CSPOSVersions::GetSPOSFeatureNumber( int nVersion )
{
	switch( nVersion )
	{
	case SPOS_V4:			return 1;
	case SPOS_V4_210:		return 2;
	case SPOS_V4_321:		return 3;
	case SPOS_V4_389:		return 4;
	case SPOS_V4_423:		return 5;
	case SPOS_V4_425:		return 6;
	case SPOS_V4_494:		return 7;
	case SPOS_V4_614:		return 8;
	case SPOS_V4_634:		return 9;
	case SPOS_V4_682:		return 10;
	case SPOS_V4_719:		return 11;
	case SPOS_V4_753:		return 12;
	case SPOS_V4_780:		return 13;
	case SPOS_V4_827:		return 14;
	case SPOS_V4_837:		return 15;
	case SPOS_V4_861:		return 16;
	case SPOS_V4_958:		return 17;
	case SPOS_V4_992:		return 18;
	case SPOS_V4_999:		return 19;
	case SPOS_V4_1071:		return 20;
	case SPOS_V4_1092:		return 21;
	case SPOS_V4_1096:		return 22;
	case SPOS_V4_1124:		return 23;
	case SPOS_V4_1216:		return 24;
	case SPOS_V4_1300:		return 25;
	case SPOS_V4_1392:		return 26;
	case SPOS_V4_1510:		return 27;
	case SPOS_V4_1519:		return 28;
	case SPOS_V4_1522:		return 29;
	case SPOS_V4_1525:		return 30;
	case SPOS_V4_1734:		return 31;
	case SPOS_V4_1901:		return 32;
	case SPOS_V4_1977:		return 33;
	case SPOS_V4_2012:		return 34;
	case SPOS_V4_2067:		return 35;
	case SPOS_V4_2133:		return 36;
	case SPOS_V4_2161:		return 37;
	case SPOS_V4_2455:		return 38;
	case SPOS_V4_2606:		return 39;
	case SPOS_V4_2_369:		return 40;
	case SPOS_V4_2_631:		return 41;
	case SPOS_V4_2_714:		return 42;
	case SPOS_V4_2_919:		return 43;
	case SPOS_V4_2_959:		return 44;
	case SPOS_V4_2_1081:	return 45;
	case SPOS_V4_2_1087:	return 46;
	case SPOS_V4_2_1384:	return 47;
	case SPOS_V4_2_1456:	return 48;
	case SPOS_V4_2_1530:	return 49;
	case SPOS_V4_2_1579:	return 50;
	case SPOS_V4_2_1605:	return 51;
	case SPOS_V4_2_1618:	return 52;
	case SPOS_V4_2_1622:	return 53;
	case SPOS_V4_3_18:		return 54;
	case SPOS_V4_3_93:		return 55;
	case SPOS_V4_3_104:		return 56;
	default:				return 0;
	}
}

/**********************************************************************/
