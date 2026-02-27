/******************************************************************************/
#include "EposPaymentGroupDatabase.h"
/******************************************************************************/

static const char* szHEADER = "No,Text";
static const char* szSORTFIELD = "No,ascending";

/******************************************************************************/

CEposPaymentGroupDatabase::CEposPaymentGroupDatabase()
{
}

/******************************************************************************/
//	mode = DB_READONLY

int CEposPaymentGroupDatabase::Read()
{
	CString strFilename = Filenames.GetPaymentGroupFilename();	

	CString strEposFile = System.GetEposPaymentGroupFilename();			// P39 \ Db001 \ smfile02.dat 
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

