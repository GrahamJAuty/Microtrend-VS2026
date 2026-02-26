/**********************************************************************/
//#include "globalfunctions.h"
#include "MaxLengths.h"
/**********************************************************************/
#include "MultilineTextBuffer.h"
/**********************************************************************/

CMultilineTextBuffer::CMultilineTextBuffer()
{
	m_strFilename = "";
	m_nMaxLength = 100;
}

/**********************************************************************/

void CMultilineTextBuffer::SetPMSGroupSource( int n )
{
	m_strFilename = "";
	m_nMaxLength = MAX_LENGTH_EMAIL_SIGNATURE;

	switch(n)
	{
	case 2:
	case 3:
	case 4:
	case 5:
		{
			CFilenameUpdater FnUp( SysFiles::PMSEmailSignatureExtra, 0, n );
			m_strFilename = FnUp.GetFilenameToUse();
		}
		break;

	case 1:
	default:
		{
			CFilenameUpdater FnUp ( SysFiles::PMSEmailSignatureBase );
			m_strFilename = FnUp.GetFilenameToUse();
		}
		break;
	}
}

/**********************************************************************/

void CMultilineTextBuffer::SetLocationNotesSource( int nLocIdx )
{
	m_nMaxLength = MAX_LENGTH_LOCATION_NOTES;
	CFilenameUpdater FnUp( SysFiles::LocationNotes, nLocIdx );
	m_strFilename = FnUp.GetFilenameToUse();
}

/**********************************************************************/

void CMultilineTextBuffer::Read()
{
	int nTotalLength = 0;
	m_arrayBufferOld.RemoveAll();
	m_arrayBufferNew.RemoveAll();

	CSSFile fileText;
	if ( fileText.Open( m_strFilename, "rb" ) == TRUE )
	{
		CString strBuffer;
		while( fileText.ReadString( strBuffer ) == TRUE )
		{
			if ( nTotalLength + strBuffer.GetLength() > m_nMaxLength )
				strBuffer = strBuffer.Left( m_nMaxLength - nTotalLength );

			m_arrayBufferOld.Add( strBuffer );
			m_arrayBufferNew.Add( strBuffer );
			nTotalLength += strBuffer.GetLength();

			if ( nTotalLength >= m_nMaxLength )
				break;
		}
	}
}

/**********************************************************************/

void CMultilineTextBuffer::CopyBufferToEditBox( CEdit& edit )
{
	CString strWindowText = "";
	for ( int n = 0; n < m_arrayBufferNew.GetSize(); n++ )
	{
		strWindowText += m_arrayBufferNew.GetAt(n);

		if ( n < m_arrayBufferNew.GetSize() - 1 )
			strWindowText += "\r\n";
	}

	edit.SetWindowText( strWindowText );
}

/**********************************************************************/

void CMultilineTextBuffer::CopyEditBoxToBuffer( CEdit& edit )
{
	m_arrayBufferNew.RemoveAll();

	CString strText;
	edit.GetWindowText( strText );

	int nEditLength = strText.GetLength();
	int nSaveLength = 0;
			
	CString strLine = "";
	for ( int nPos = 0; nPos < nEditLength; nPos++ )
	{
		if ( nPos == nEditLength - 1 )
		{
			strLine += strText.GetAt(nPos);
			nSaveLength++;
		}
		else
		{
			if ( strText.Mid( nPos, 2 ) == "\r\n" )
			{
				m_arrayBufferNew.Add( strLine );	
				strLine = "";
				nPos++;
			}
			else
			{
				strLine += strText.GetAt(nPos);
				nSaveLength++;
			}
		}

		if ( nSaveLength >= m_nMaxLength )
			break;
	}

	if ( strLine != "" )
		m_arrayBufferNew.Add( strLine );

}

/**********************************************************************/

void CMultilineTextBuffer::WriteIfChanged()
{
	bool bChanged = ( m_arrayBufferNew.GetSize() != m_arrayBufferOld.GetSize() );

	if ( FALSE == bChanged )
	{
		for ( int n = 0; n < m_arrayBufferNew.GetSize(); n++ )
		{
			CString strOld = m_arrayBufferOld.GetAt(n);
			CString strNew = m_arrayBufferNew.GetAt(n);

			if ( strOld != strNew )
			{
				bChanged = TRUE;
				break;
			}
		}
	}
	
	if ( TRUE == bChanged )
	{
		CSSFile fileText;
		if ( fileText.Open( m_strFilename, "wb" ) == TRUE )
		{
			for ( int n = 0; n < m_arrayBufferNew.GetSize(); n++ )
				fileText.WriteLine( m_arrayBufferNew.GetAt(n) );
		}
	}
}

/**********************************************************************/

void CMultilineTextBuffer::AppendToFile( CReportFile& ReportFile )
{
	if ( m_arrayBufferOld.GetSize() != 0 )
	{
		ReportFile.WriteLine( "" );
		ReportFile.WriteLine( "" );
		for ( int n = 0; n < m_arrayBufferOld.GetSize(); n++ )
		{
			CString strLine = m_arrayBufferOld.GetAt(n);
			::HTMLEuroConvert( strLine );
			ReportFile.WriteLine( strLine );
		}
	}
}

/**********************************************************************/
