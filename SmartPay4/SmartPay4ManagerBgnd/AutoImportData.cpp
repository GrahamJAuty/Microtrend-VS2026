#include "AutoImportData.h"

//*******************************************************************

CAutoImportData::CAutoImportData()
{
	m_nFileFormat			= nIMPORT_FORMAT_CSV;		// 0=csv, 1-sims
	m_nImportMethod			= nIMPORT_METHOD_ALL;
	m_strImportFilename		= "";
	m_bImportPhotoID		= TRUE;

	m_strFilename = Filenames.GetCSVDatabaseImportDataFilename();
}

//*******************************************************************

bool CAutoImportData::Read()
{
	CSSIniFile file;
	bool bReply = file.Read ( m_strFilename );

	m_nFileFormat		= file.GetInt	 ( "FileFormat", m_nFileFormat );
	m_strImportFilename	= file.GetString ( "ImportFile", m_strImportFilename );
	m_nImportMethod		= file.GetInt	 ( "ImportMethod", m_nImportMethod );
	m_bImportPhotoID	= file.GetBool	 ( "ImportPhoto", m_bImportPhotoID );

	return bReply;
}

//*********************************************************************

bool CAutoImportData::Save()
{
	CSSIniFile file;

	file.Set ( "FileFormat", m_nFileFormat );
	file.Set ( "ImportFile", m_strImportFilename );
	file.Set ( "ImportMethod", m_nImportMethod );
	file.Set ( "ImportPhoto", m_bImportPhotoID );

	return file.Write ( m_strFilename );
}

//*********************************************************************

