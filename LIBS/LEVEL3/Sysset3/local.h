/**********************************************************************/
#pragma once
/**********************************************************************/

#define SYSSET_DATA_LEN 64

#define SYSSET_EXPIRY_SYSTEM 0
#define SYSSET_EXPIRY_DEALER 1

#define SYSSET_ENABLE_PROGRAM 4		// used to be 2 (before md5)
#define SYSSET_ENABLE_DEALER_EXPIRY 5	// used to be 3 (before md5)

#define SYSSET_VALID_REGISTER		"0123456789 "		// valid registration chars
#define SYSSET_VALID_TELEPHONE	"0123456789+() "	// valid telephone chars

#define MES	CSysset::DecodeMessage

/**********************************************************************/
