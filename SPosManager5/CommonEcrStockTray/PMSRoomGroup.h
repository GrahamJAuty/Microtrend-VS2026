#pragma once
/*********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/

class CPMSRoomGroup : public CSharedStringArray 
{
public:
	CPMSRoomGroup();
	bool Read( const char* szFilename );
	
public:
	const char* GetRoomGroupName( int n );
	void SetRoomGroupName( int n, const char* szName );
	
private:
	CString m_strName;
};

/**********************************************************************/
#endif
/**********************************************************************/
