/**********************************************************************/
#include "DataManagerBase.h"
//#include "globalfunctions.h"
/**********************************************************************/
#include "SpecialChangesHelper.h"
/**********************************************************************/

CSpecialChangesHelper::CSpecialChangesHelper(void)
{
	m_nLocIdx = -1;
	m_nTerminalNo = -1;
}

/**********************************************************************/

const char* CSpecialChangesHelper::GetLocPluFilename()
{
	CFilenameUpdater FnUp( SysFiles::LocPluEcr, m_nLocIdx );
	m_strLocPluFilename = FnUp.GetFilenameToUse();

	if ( ( m_nTerminalNo >= 1 ) && ( m_nTerminalNo <= 999 ) )
	{
		m_strLocPluFilename.MakeUpper();

		int nLength = m_strLocPluFilename.GetLength();
		if ( nLength > 12 )
		{
			if ( m_strLocPluFilename.Right(12) == "FILE9003.DAT" )
			{
				CString strMaster;
				strMaster.Format( "MASTER%3.3d\\", m_nTerminalNo );
				
				m_strLocPluFilename = m_strLocPluFilename.Left( nLength - 12 );
				m_strLocPluFilename += strMaster;
				CreateSubdirectory( m_strLocPluFilename );

				m_strLocPluFilename += "FILE9003.DAT";
			}
		}
	}

	return m_strLocPluFilename;
}

/**********************************************************************/

void CSpecialChangesHelper::InitialiseDownloadJob( int nLocIdx, int nTerminalNo, int nParam1 )
{
	m_nLocIdx = nLocIdx; 
	m_nTerminalNo = nTerminalNo;
	m_nParam1 = nParam1;

	m_arrayEcrPlu.Close();
	m_arrayEcrPlu.RemoveAt( 0, m_arrayEcrPlu.GetSize() );
	m_arrayCurrentDownload.RemoveAt( 0, m_arrayCurrentDownload.GetSize() );
	m_arrayEcrPlu.Open( GetLocPluFilename(), DB_READONLY );
}

/**********************************************************************/

bool CSpecialChangesHelper::HasPluChanged( CPluCSVRecord& PluRecord )
{
	int nPluIdx;
	__int64 nPluNo = PluRecord.GetPluNoInt();
	if ( m_arrayEcrPlu.FindPluByNumber( nPluNo, nPluIdx ) == FALSE )
		return TRUE;
	
	CPluCSVRecord EcrPluRecord;
	m_arrayEcrPlu.GetAt( nPluIdx, EcrPluRecord );
	return ( EcrPluRecord.GetChange( PluRecord ) == EPOS_CHANGE );
}

/**********************************************************************/

void CSpecialChangesHelper::NoteDownloadedPlu( CPluCSVRecord& PluRecord )
{
	int nPluIdx;
	__int64 nPluNo = PluRecord.GetPluNoInt();
	if ( m_arrayCurrentDownload.FindPluByNumber( nPluNo, nPluIdx ) == FALSE )
		m_arrayCurrentDownload.InsertAt( nPluIdx, PluRecord );
	else
		m_arrayCurrentDownload.SetAt( nPluIdx, PluRecord );
}

/**********************************************************************/

void CSpecialChangesHelper::FinaliseDownloadJob()
{
	m_arrayEcrPlu.Close();

	if ( m_arrayEcrPlu.Open( GetLocPluFilename(), DB_READWRITE ) != DATAMANAGER_ERROR_NONE )
		return;

	if ( 0 == m_nParam1 )
		m_arrayEcrPlu.RemoveAt( 0, m_arrayEcrPlu.GetSize() );

	for ( int n = 0; n < m_arrayCurrentDownload.GetSize(); n++ )
	{
		CPluCSVRecord PluRecord;
		m_arrayCurrentDownload.GetAt( n, PluRecord );

		int nPluIdx;
		__int64 nPluNo = PluRecord.GetPluNoInt();
		if ( m_arrayEcrPlu.FindPluByNumber( nPluNo, nPluIdx ) == FALSE )
			m_arrayEcrPlu.InsertAt( nPluIdx, PluRecord );
		else
			m_arrayEcrPlu.SetAt( nPluIdx, PluRecord );
	}

	m_arrayEcrPlu.ForceWrite();
	m_arrayEcrPlu.Close();
}

/**********************************************************************/
