/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/
#include "NodeTypes.h"
/**********************************************************************/
#include "DataManagerNonDbFuncPMS.h"
/**********************************************************************/

CDataManagerNonDbFuncPMS::CDataManagerNonDbFuncPMS() : CDataManagerBase(),
	PMSClosureReasons( NODE_PMSCLOSURE ), 
	PMSClosureReasonsBuffer( NODE_PMSCLOSURE )
{
	m_nStatusPMSClosureReasons = DATAMANAGER_CLOSED;

	m_nStatusPMSClosureReasonsBuffer = DATAMANAGER_CLOSED;
}

/**********************************************************************/

bool CDataManagerNonDbFuncPMS::OpenPMSClosureReasons( int nType, CDataManagerInfo& info )
{
	ResetInfo( info, NODE_PMSCLOSURE );
	CFilenameUpdater FnUp ( SysFiles::PMSClosureReasons );
	return OpenStringArray( PMSClosureReasons, info, FnUp.GetFilenameToUse(), m_nStatusPMSClosureReasons, nType, FALSE );
}

bool CDataManagerNonDbFuncPMS::ClosePMSClosureReasons( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_PMSCLOSURE );
	return CloseStringArray( PMSClosureReasons, info, m_nStatusPMSClosureReasons );
}

bool CDataManagerNonDbFuncPMS::WritePMSClosureReasons( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_PMSCLOSURE );
	return WriteStringArray( PMSClosureReasons, info, m_nStatusPMSClosureReasons );
}

bool CDataManagerNonDbFuncPMS::OpenPMSClosureReasonsBuffer( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_PMSCLOSURE );

	CFilenameUpdater FnUp ( SysFiles::PMSClosureReasons );
	if ( OpenStringArray( PMSClosureReasonsBuffer, info, FnUp.GetFilenameToUse(), m_nStatusPMSClosureReasonsBuffer, DB_READWRITE, FALSE ) == FALSE )
		return FALSE;

	PMSClosureReasons.CopyFrom( PMSClosureReasonsBuffer );
	return TRUE;
}

bool CDataManagerNonDbFuncPMS::ClosePMSClosureReasonsBuffer( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_PMSCLOSURE );
	PMSClosureReasons.CopyFrom( PMSClosureReasonsBuffer );
	return CloseStringArray( PMSClosureReasonsBuffer, info, m_nStatusPMSClosureReasonsBuffer );
}

bool CDataManagerNonDbFuncPMS::WritePMSClosureReasonsBuffer( CDataManagerInfo& info )
{
	ResetInfo( info, NODE_PMSCLOSURE );
	
	CDefTextUnit temp( NODE_PMSCLOSURE );
	temp.CopyFrom( PMSClosureReasonsBuffer );
	PMSClosureReasonsBuffer.CopyFrom( PMSClosureReasons);

	bool bResult = WriteStringArray( PMSClosureReasonsBuffer, info, m_nStatusPMSClosureReasonsBuffer );

	if ( bResult == FALSE )
		PMSClosureReasonsBuffer.CopyFrom( temp );

	return bResult;
}

/**********************************************************************/
#endif
/**********************************************************************/
