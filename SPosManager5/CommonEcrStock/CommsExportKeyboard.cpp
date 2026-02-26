/**********************************************************************/
 
/**********************************************************************/
#include "CommsErrors.h"
#include "DatabaseCSVArray.h"
#include "KeyboardSetCSVArray.h"
#include "LocationCSVArray.h"
#include "LocationMasterMap.h"
#include "NetworkCSVArray.h"
#include "NodeTypes.h"
/**********************************************************************/
#include "CommsExportKeyboard.h"
/**********************************************************************/

CCommsExportKeyboard::CCommsExportKeyboard( CJobListCSVRecord& JobInfo, int nKbNo, CKeyboardMap& KeyboardMap ) : m_KeyboardMap( KeyboardMap )
{
	m_strSourcePath = "";
	m_strDestPath = "";
	m_nNetworkIdx = JobInfo.GetNetworkIdx();
	m_nLocIdx = JobInfo.GetLocIdx();
	m_nTerminalNo = JobInfo.GetTerminalNo();
	m_bImmediate = JobInfo.GetIsImmediateFlag();
	m_nKbNo = nKbNo;
}

/**********************************************************************/

int CCommsExportKeyboard::PreparePaths()
{
	if ( m_nNetworkIdx < 0 || m_nNetworkIdx >= dbNetwork.GetSize() )
		return COMMS_ERROR_BADNETIDX;

	switch( dbNetwork.GetConnectionType( m_nNetworkIdx ) )
	{
	case CONNECTION_TYPE_STANDARD_NONE:
	case CONNECTION_TYPE_SPTBOOK_NONE:
	case CONNECTION_TYPE_SPTBOOK_WEB:
		return COMMS_ERROR_NOCOMMS;
	}

	if ( m_nLocIdx < 0 || m_nLocIdx >= dbLocation.GetSize() )
		return COMMS_ERROR_BADLOCIDX;

	CKeyboardMapInfo info;
	m_KeyboardMap.GetLocInfo( m_nLocIdx, info );

	switch( info.m_nEntryType )
	{
	case KBD_MAP_LOC:
		m_strSourcePath = dbLocation.GetFolderPathDataKeyboard( m_nLocIdx, m_nKbNo );
		break;

	case KBD_MAP_SET:
		m_strSourcePath = dbKeyboardSet.GetFolderPathKeyboard( info.m_nKbsIdx, m_nKbNo );
		break;

	default:
		return COMMS_ERROR_NOKEYBOARD;
		break;
	}

	if ( ::ExistSubdirectory ( m_strSourcePath ) == FALSE )
		return COMMS_ERROR_NOKEYBOARD;

	m_strDestPath = LocationMasterMap.GetDownloadFolder( m_nLocIdx, m_nTerminalNo, m_bImmediate );

	return COMMS_ERROR_NONE;
}

/**********************************************************************/
