#pragma once
/**********************************************************************/
//IMPORTANT : YOU **MUST** UPDATE KEYBOARD MANAGER IF YOU ADD A NEW
//SPOS VERSION TO THE LIST !!!
/**********************************************************************/
#define SPOS_V4 9
#define SPOS_V4_210 10
#define SPOS_V4_321 11
#define SPOS_V4_389 12
#define SPOS_V4_423 15
#define SPOS_V4_425 13
#define SPOS_V4_494 14
#define SPOS_V4_614 23
#define SPOS_V4_634 18
#define SPOS_V4_682 19
#define SPOS_V4_719 20
#define SPOS_V4_753 21
#define SPOS_V4_780 22
#define SPOS_V4_827 24
#define SPOS_V4_837 25
#define SPOS_V4_861 26
#define SPOS_V4_958 27
#define SPOS_V4_992 28
#define SPOS_V4_999 29
#define SPOS_V4_1071 30
#define SPOS_V4_1092 31
#define SPOS_V4_1096 32
#define SPOS_V4_1124 33
#define SPOS_V4_1216 34
#define SPOS_V4_1300 35
#define SPOS_V4_1392 36
#define SPOS_V4_1510 37
#define SPOS_V4_1519 38
#define SPOS_V4_1522 39
#define SPOS_V4_1525 40
#define SPOS_V4_1734 41
#define SPOS_V4_1901 42
#define SPOS_V4_1977 43
#define SPOS_V4_2012 44
#define SPOS_V4_2067 45
#define SPOS_V4_2133 46
#define SPOS_V4_2161 47
#define SPOS_V4_2455 48
#define SPOS_V4_2606 49
#define SPOS_V4_2_369 50
#define SPOS_V4_2_631 51
#define SPOS_V4_2_714 52
#define SPOS_V4_2_919 53
#define SPOS_V4_2_959 54
#define SPOS_V4_2_1081 55
#define SPOS_V4_2_1087 56
#define SPOS_V4_2_1456 57
#define SPOS_V4_2_1384 58 //*** ADDDED OUT OF SEQUENCE
#define SPOS_V4_2_1530 59
#define SPOS_V4_2_1579 60
#define SPOS_V4_2_1605 61
#define SPOS_V4_2_1618 62
#define SPOS_V4_2_1622 63
#define SPOS_V4_3_18 64
#define SPOS_V4_3_93 65
#define SPOS_V4_3_104 66
/**********************************************************************/

class CSPOSVersions
{
public:
	static bool CheckSPOSVersion( int nVersion, int nMinVersion );
	static bool IsValidSPOSVersion( int nVersion );
	static int GetSPOSFeatureNumber( int nVersion );
	static int GetSPOSBaseVersion( int nVersion );
};

/**********************************************************************/
