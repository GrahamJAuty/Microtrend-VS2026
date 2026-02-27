/******************************************************************************/
#include "LeaversHandler.h"
/******************************************************************************/

CLeaversHandler::CLeaversHandler ( const char* szYear )
{
	m_strLeaversYear = szYear;
	if (m_strLeaversYear == "")
	{
		return;
	}

	CString strLeaversFolder = Filenames.GetFilename ( "Leavers" );		// eg "<data path\>Pnn\Leavers"

	if (::ExistSubdirectory(strLeaversFolder) == FALSE)
	{
		::MakeSubdirectory(strLeaversFolder);
	}

	m_strLeaversFindListFile	= strLeaversFolder + "\\LeaversFind.csv";		// eg "<data path\>Pnn\Leavers\\LeaversFind.csv"

	CString strYearFolder;	strYearFolder.Format ( "%s\\%s",strLeaversFolder, m_strLeaversYear );
	if (::ExistSubdirectory(strYearFolder) == FALSE)				// eg "<data path\>Pnn\Leavers\\2015-2016"
	{
		::MakeSubdirectory(strYearFolder);
	}

	CString strSysFilePath = strYearFolder + "\\SysFiles";				// eg "<data path\>Pnn\Leavers\\2015-2016\\SysFiles"
	if (::ExistSubdirectory(strSysFilePath) == FALSE)
	{
		::MakeSubdirectory(strSysFilePath);
	}

	m_strPurchaseFolder = strYearFolder + "\\History";					// eg "<data path\>Pnn\Leavers\\2015-2016\History"
	if (::ExistSubdirectory(m_strPurchaseFolder) == FALSE)
	{
		::MakeSubdirectory(m_strPurchaseFolder);
	}

	m_strPhotoIDFolder = strYearFolder + "\\PhotoID";					// eg "<data path\>Pnn\Leavers\\2015-2016\\PhotoID"
	if (::ExistSubdirectory(m_strPhotoIDFolder) == FALSE)
	{
		::MakeSubdirectory(m_strPhotoIDFolder);
	}

	m_strLeaverDatabase			= strSysFilePath + "\\file0002.dat";		// leavers database filename in Sysfiles folder
	m_strAuditFilename			= strSysFilePath + "\\file0005.dat";		// audit file for leavers database Sysfiles folder
	m_strSessionAuditFilename	= strSysFilePath + "\\file0005.$$1";		// session audit file for leavers database Sysfiles folder
}

/******************************************************************************/

CLeaversHandler::~CLeaversHandler(void)
{
}

/******************************************************************************/

CString CLeaversHandler::GetPurchaseHistoryFilename ( const char* szCardNo )
{
	CString strFilename = "";
	strFilename.Format ( "%s\\%s.dat", 
		(const char*) m_strPurchaseFolder, 
		(const char*) System.FormatCardNo(szCardNo) );

	return strFilename;
}

/******************************************************************************/

CString CLeaversHandler::GetPhotoIDFilename ( const char* szFilename )
{
	CString strFilename = "";
	strFilename.Format ( "%s\\%s", m_strPhotoIDFolder, szFilename );
	return strFilename;
}

//*******************************************************************
