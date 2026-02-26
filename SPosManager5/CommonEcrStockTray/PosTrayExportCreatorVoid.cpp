/**********************************************************************/
 
/**********************************************************************/
//#include "DataManagerNonDb.h"
#include "FileRemover.h"
#include "PosTrayExportConsolidatorVoid.h"
#include "PosTrayOptions.h"
/**********************************************************************/
#include "PosTrayExportCreatorVoid.h"
/**********************************************************************/

CPosTrayExportCreatorVoid::CPosTrayExportCreatorVoid( CPosTrayTask& Task, bool& bQuitNow, void* pHostDialog, int nManualExportNo ) : CPosTrayExportCreatorBase( Task, bQuitNow, pHostDialog, nManualExportNo )
{
	m_strExportFileType = "void";
	m_strEmailSubjectType = "Voids";
	m_strInternalFolderName = "EXPORTVOID";
}

/**********************************************************************/

__int64 CPosTrayExportCreatorVoid::ProcessTaskInternal()
{
	__int64 nLineCount = 0;

	PrepareDatesAndFolders();

	if ( ::ExistSubdirectory( m_strExportFolderPending ) == FALSE )
		return -1;

	CTimeSliceMapCSVRecord TimeSliceRecord;
	PrepareSessionAndTimeSlice( TimeSliceRecord );

	CArray<int,int> arrayFields;
	for ( int n = 1; n <= POSTRAY_MAX_FIELDS_VOID; n++ )
		arrayFields.Add( m_Task.GetBatchExportFieldVoid(n) );

	int nDbIdx = 0;
	int nDbNo = m_Task.GetBatchConDbNo();
	if ( dbDatabase.FindDatabaseByNumber( nDbNo, nDbIdx ) == FALSE )
		nDbIdx = 0;

	CDataManagerInfo info;
	DataManager.OpenDatabaseReadOnly( nDbIdx, info, FALSE );

	int nExportDateType = m_Task.GetTaskExportDateType();
	if ( 0 == nExportDateType )
		nExportDateType = PosTrayOptions.GetExportDateType();

	CEposSelectArray SelectArray;
	CPosTrayExportConsolidatorVoid ExportConsolidator( nExportDateType, SelectArray, m_bQuitNow, m_pHostDialog );
	ExportConsolidator.Reset( arrayFields, m_Task, m_oleDateFrom, m_oleDateTo, m_ReportSession, TimeSliceRecord );
	ExportConsolidator.SetSaleTimeFlag( m_Task.GetBatchSaleTimeFlag() );
	
	if ( ExportConsolidator.PrepareFields() == TRUE )
	{
		ExportConsolidator.Consolidate();

		m_strTempFilepath = "";
		m_strTempFilepath += m_strExportFolderPending;
		m_strTempFilepath += "\\Void.tmp";

		if ( FALSE == m_bQuitNow )
		{
			nLineCount = ExportConsolidator.CreateExport( m_strTempFilepath );

			if ( nLineCount >= 2 )
				if ( ProcessExport() == FALSE )
					nLineCount = -1;

			CFileRemover FileRemover( m_strTempFilepath );
		}
	}

	return nLineCount;
}

/**********************************************************************/
