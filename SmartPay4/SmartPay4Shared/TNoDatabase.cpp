//#include "FilenameHandler.h"
#include "TNoDatabase.h"

static const char* szHEADER = "TNo,SNo";
static const char* szSORTFIELD = "TNo,ascending";

/******************************************************************************/
//	mode = DB_READONLY, DB_READWRITE

int CTNoDatabase::OpenForSharing ( CString strFilename, int nMode )
{
	if ( strFilename == "" )
	{
		strFilename = Filenames.GetTerminalGroupSetFilename();
	}
	int nRetries = 10;

	CreateSSDatabase ( strFilename, "", szHEADER, szSORTFIELD, nRetries );	// create db if not found
	Close();

	if ( nMode == DB_READONLY )
		return OpenSSDatabaseReadOnly ( strFilename, szSORTFIELD, nRetries );

	return OpenSSDatabaseReadWrite ( strFilename, "", szSORTFIELD, nRetries );

}	

//*********************************************************************


