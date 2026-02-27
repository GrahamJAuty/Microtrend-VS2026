//******************************************************************************
#include "EposAllergyDatabase.h"
//******************************************************************************

static const char* szHEADER = "No,Text";
static const char* szSORTFIELD = "No,ascending";

//******************************************************************************

static const char *szALLERGIES[nMAX_ALLERGIES] =	{	"Celery",			// 1
														"Gluten",			// 2
														"Crustacean",		// 3
														"Eggs",				// 4
														"Fish",				// 5
														"Lupin",			// 6
														"Milk",				// 7
														"Molluscs",			// 8
														"Mustard",			// 9
														"Nuts",				// 10
														"Peanuts",			// 11
														"Sesame Seeds",		// 12
														"Soya",				// 13	
														"Sulphites"			// 14
													};

//******************************************************************************

CEposAllergyDatabase::CEposAllergyDatabase()
{
	m_strFilename = Filenames.GetAllergyDatabaseFilename();

	CString strEposFile = System.GetEposAllergyTextsFilename();			// P39 \ Db001 \ smfile03.dat 
	if (::FileExists(strEposFile) == TRUE)							// see if have any new types
	{
		CFileRecycler::SendToRecycleBin(m_strFilename);							// delete current internal file
		::MoveFile(strEposFile, m_strFilename);						// create use internal file
	}
}

/******************************************************************************/
//	mode = DB_READONLY

int CEposAllergyDatabase::Read()
{
	if (::FileExists(m_strFilename) == FALSE)
	{
		CreateDefault();
	}

	int nRetries = 10;
	return OpenSSDatabaseReadOnly(m_strFilename, szSORTFIELD, nRetries);
}

//*********************************************************************

void CEposAllergyDatabase::CreateDefault()
{
	CSSFile file;
	if (file.Open(m_strFilename, "wb") == TRUE)
	{
		file.WriteLine(szHEADER);

		CString strLine = "";
		for (int n = 0; n < nMAX_ALLERGIES; n++)
		{
			strLine.Format("%d,%s", n + 1, szALLERGIES[n]);
			file.WriteLine(strLine);
		}

		file.Close();
	}
}

//*********************************************************************

