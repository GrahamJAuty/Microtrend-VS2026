/**********************************************************************/
#include "FileRemover.h"
#include "PosTrayExportConsolidatorPayment.h"
#include "PosTrayOptions.h"
/**********************************************************************/
#include "PosTrayExportCreatorPayment.h"
/**********************************************************************/

CPosTrayExportCreatorPayment::CPosTrayExportCreatorPayment( CPosTrayTask& Task, bool& bQuitNow, void* pHostDialog, int nManualExportNo ) : CPosTrayExportCreatorBase( Task, bQuitNow, pHostDialog, nManualExportNo )
{
	m_strExportFileType = "payment";
	m_strEmailSubjectType = "Payments";
	m_strInternalFolderName = "EXPORTPAYMENT";
}

/**********************************************************************/

__int64 CPosTrayExportCreatorPayment::ProcessTaskInternal()
{
	__int64 nLineCount = 0;

	PrepareDatesAndFolders();

	if ( ::ExistSubdirectory( m_strExportFolderPending ) == FALSE )
		return -1;

	CTimeSliceMapCSVRecord TimeSliceRecord;
	PrepareSessionAndTimeSlice( TimeSliceRecord );

	CArray<int,int> arrayFields;
	for ( int n = 1; n <= POSTRAY_MAX_FIELDS_PAYMENT; n++ )
		arrayFields.Add( m_Task.GetBatchExportFieldPayment(n) );

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
	CPosTrayExportConsolidatorPayment ExportConsolidator( nExportDateType, SelectArray, m_bQuitNow, m_pHostDialog );
	ExportConsolidator.Reset( arrayFields, m_Task, m_oleDateFrom, m_oleDateTo, m_ReportSession, TimeSliceRecord );
	
	if ( ExportConsolidator.PrepareFields() == TRUE )
	{
		ExportConsolidator.Consolidate();

		m_strTempFilepath = "";
		m_strTempFilepath += m_strExportFolderPending;
		m_strTempFilepath += "\\Payment.tmp";

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
