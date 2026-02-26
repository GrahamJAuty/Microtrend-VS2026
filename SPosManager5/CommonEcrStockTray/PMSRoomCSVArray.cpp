/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/
#include "PMSOptions.h"
/**********************************************************************/
#include "PMSRoomCSVArray.h"
/**********************************************************************/

CPMSRoomCSVArray::CPMSRoomCSVArray() : CSharedCSVArray ( ',', '"', FALSE, TRUE, TRUE )
{
}

/**********************************************************************/

int CPMSRoomCSVArray::Open( const char* szFilename, int nMode )
{
	RemoveAt( 0, GetSize() );

	int nReply = CSharedCSVArray::Open ( szFilename, nMode );
	
	if ( nReply != DB_ERR_SHARE )
	{
		if ( nMode == DB_READONLY )	// if file does not exist use default values
			nReply = DB_ERR_NONE;
	}

	return nReply;
}

/**********************************************************************/

bool CPMSRoomCSVArray::FindRoomByNumber ( int nRoomNo, int& nIndex )
{
	int nStart = 0;
	int nEnd = GetSize() - 1;

	CPMSRoomCSVRecord Record;

	while ( nStart <= nEnd )
	{
		nIndex = ( nStart + nEnd ) / 2;

		GetAt ( nIndex, Record );
		int nArrayRoomNo = Record.GetRoomNo();

		if ( nArrayRoomNo < nRoomNo )
			nStart = nIndex + 1;

		if ( nArrayRoomNo > nRoomNo )
			nEnd = nIndex - 1;

		if ( nArrayRoomNo == nRoomNo )
			return TRUE;
	}

	nIndex = nStart;
	return FALSE;
}

/**********************************************************************/

const char* CPMSRoomCSVArray::GetRoomNameByRoomNo( int nRoomNo )
{
	int nRoomIdx;
	if ( FindRoomByNumber ( nRoomNo, nRoomIdx ) )
	{
		CPMSRoomCSVRecord Room;
		GetAt ( nRoomIdx, Room );
		m_strRoomName = Room.GetRoomName();
	}
	else
		m_strRoomName = "Unknown";

	return m_strRoomName;
}

/**********************************************************************/

const char* CPMSRoomCSVArray::GetDefaultNameByRoomNo( int nRoomNo )
{
	m_strDefaultName.Format( "Room %d", nRoomNo );
	return m_strDefaultName;
}

/**********************************************************************/

int CPMSRoomCSVArray::CheckPOSFolders()
{
	int nResultRoom = CheckRoomFolderInternal();
	int nResultPost = CheckPostFolderInternal();

	if ( ( PMS_ERROR_NONE == nResultRoom ) && ( PMS_ERROR_NONE == nResultPost ) )
		return PMS_ERROR_NONE;

	CString strMsg = "Unable to access Guest Account folder on EPOS terminal.";

	switch( nResultRoom )
	{
	case PMS_ERROR_FOLDER_NONE:	
		strMsg += "\n\n(No room folder specified)";
		break;

	case PMS_ERROR_FOLDER_EXIST:	
		strMsg += "\n\n(The specified room folder does not exist)";
		break;

	case PMS_ERROR_FOLDER_NAME:	
		strMsg += "\n\n(The path to the room folder is invalid)";
		break;
	}

	switch( nResultPost )
	{
	case PMS_ERROR_FOLDER_NONE:	
		strMsg += "\n\n(No post folder specified)";
		break;

	case PMS_ERROR_FOLDER_EXIST:	
		strMsg += "\n\n(The specified post folder does not exist)";
		break;

	case PMS_ERROR_FOLDER_NAME:	
		strMsg += "\n\n(The path to the post folder is invalid)";
		break;
	}

	strMsg += "\n\nWould you like to edit the Guest Account settings and try again.";

	if ( Prompter.YesNo( strMsg, TRUE ) != IDYES )
		return PMS_ERROR_ABANDON;
	else
		return PMS_ERROR_FOLDER_NAME;
}

/**********************************************************************/

int CPMSRoomCSVArray::CheckRoomFolderInternal()
{
	CString strPath = PMSOptions.GetEposLinkRoomPath();

	if ( strPath == "" )
		return PMS_ERROR_FOLDER_NONE;

	if ( strPath.GetLength() < 5 )
		return PMS_ERROR_FOLDER_NAME;

	strPath.MakeUpper();
	if ( strPath.Right(5) != "\\ROOM" )
		return PMS_ERROR_FOLDER_NAME;

	if ( ::ExistSubdirectory( strPath ) == FALSE )
		return PMS_ERROR_FOLDER_EXIST;

	return PMS_ERROR_NONE;
}

/**********************************************************************/

int CPMSRoomCSVArray::CheckPostFolderInternal()
{
	CString strPath = PMSOptions.GetEposLinkPostPath();

	if ( strPath == "" )
		return PMS_ERROR_FOLDER_NONE;

	if ( strPath.GetLength() < 5 )
		return PMS_ERROR_FOLDER_NAME;

	strPath.MakeUpper();
	if ( strPath.Right(5) != "\\POST" )
		return PMS_ERROR_FOLDER_NAME;

	if ( ::ExistSubdirectory( strPath ) == FALSE )
		return PMS_ERROR_FOLDER_EXIST;

	return PMS_ERROR_NONE;
}

/**********************************************************************/
#endif
/**********************************************************************/
