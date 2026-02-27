/******************************************************************************/
#include "EposPaymentDatabase.h"
/******************************************************************************/

static const char* szHEADER = "No,Text,Group";
static const char* szSORTFIELD = "No,ascending";

/******************************************************************************/

CEposPaymentDatabase::CEposPaymentDatabase()
{
}

/******************************************************************************/
//	mode = DB_READONLY

int CEposPaymentDatabase::Read()
{
	CString strFilename = Filenames.GetPaymentTypeFilename();		

	CString strEposFile = System.GetEposPaymentTypeFilename();			// P39 \ Db001 \ smfile01.dat 
	if ( ::FileExists ( strEposFile ) == TRUE )							// see if have any new types
	{
		CFileRecycler::SendToRecycleBin ( strFilename );								// delete current internal file
		::MoveFile ( strEposFile, strFilename );						// create use internal file
	}

	int nRetries = 10;

	CreateSSDatabase ( strFilename, "", szHEADER, szSORTFIELD, nRetries );			// create db if not found
	Close();

	return OpenSSDatabaseReadOnly ( strFilename, szSORTFIELD, nRetries );
}	

//*********************************************************************

