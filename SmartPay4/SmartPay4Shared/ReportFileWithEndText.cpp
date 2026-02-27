/**********************************************************************/
#include "ReportFileWithEndText.h"
/**********************************************************************/

void CReportFileWithEndText::Close()
{
	if ( TRUE == m_bFileOpened )
	{
		CStringArray arrayEndText;

		for ( int n = 1; n <= REPORT_ENDTEXT_COUNT; n++ )
		{
			CString strText = System.GetReportEndText(n);
			::TrimSpacesFromString( strText, FALSE );

			if ( strText != "" )
			{
				while ( arrayEndText.GetSize() < n - 1 )
				{
					arrayEndText.Add( "" );
				}

				arrayEndText.Add( strText );
			}
		}

		if ( arrayEndText.GetSize() > 0 )
		{
			WriteLine( "" );

			for ( int n = 0; n < arrayEndText.GetSize(); n++ )
			{
				WriteLine( arrayEndText.GetAt(n) );
			}
		}
	}

	CReportFile::Close();
}

/**********************************************************************/

