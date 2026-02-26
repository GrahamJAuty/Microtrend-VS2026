/**********************************************************************/
#include "ExportDate.h"
/**********************************************************************/

CExportDate::CExportDate()
{
	m_strDate = "";
	m_strLastPluFile = "";
	m_strLastStockFile = "";
}

/**********************************************************************/

void CExportDate::SetDate ( CTime& time )
{
	m_strDate.Format ( "%4.4d%2.2d%2.2d%2.2d%2.2d%2.2d", time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond() );
}

/**********************************************************************/

const char* CExportDate::GetCSVLine()
{
	CCSV csv;
	csv.Add(m_strDate);
	csv.Add(m_strLastPluFile);
	csv.Add(m_strLastStockFile);
	m_strCSVLine = csv.GetLine();
	return m_strCSVLine;
}

/**********************************************************************/

void CExportDate::Read()
{
	m_strDate = "";
	m_strLastPluFile = "";
	m_strLastStockFile = "";
	
	CFilenameUpdater FnUp( SysFiles::ExportDate, DataManager.GetActiveDbIdx() );

	CSSFile fileExport;
	if ( fileExport.Open ( FnUp.GetFilenameToUse(), "rb" ) == TRUE )
	{
		CString strBuffer;
		if ( fileExport.ReadString ( strBuffer ) == TRUE )
		{
			CCSVExportDate csv ( strBuffer );
			m_strDate = csv.GetDate();
			m_strLastPluFile = csv.GetLastPluFile();
			m_strLastStockFile = csv.GetLastStockFile();
		}
	}
}
	
/**********************************************************************/

void CExportDate::Write()
{
	CSSFile fileExport;
	CFilenameUpdater FnUp( SysFiles::ExportDate, DataManager.GetActiveDbIdx() );
	if ( fileExport.Open ( FnUp.GetFilenameToUse(), "wb" ) == TRUE )
		fileExport.WriteLine ( GetCSVLine() );
}

/**********************************************************************/

