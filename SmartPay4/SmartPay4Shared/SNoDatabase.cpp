#include "SNoDatabase.h"

static const char* szHEADER = "SNo,Name,TList";
static const char* szSORTFIELD = "SNo,ascending";

/******************************************************************************/
//	mode = DB_READONLY, DB_READWRITE

int CSNoDatabase::OpenForSharing ( CString strFilename, int nMode )
{
	if ( strFilename == "" )
	{
		strFilename = Filenames.GetTerminalSetFilename();		// .., \P44 \ SysFiles \ file0044.dat
	}
	int nRetries = 10;

	CreateSSDatabase ( strFilename, "", szHEADER, szSORTFIELD, nRetries );	// create db if not found
	Close();

	if ( nMode == DB_READONLY )
		return OpenSSDatabaseReadOnly ( strFilename, szSORTFIELD, nRetries );

	return OpenSSDatabaseReadWrite ( strFilename, "", szSORTFIELD, nRetries );
}	

//*********************************************************************


