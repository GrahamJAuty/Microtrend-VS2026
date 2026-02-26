#pragma once
/*********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/

class CPMSRoomType : public CSharedStringArray 
{
public:
	CPMSRoomType();
	bool Read( const char* szFilename );
	
public:
	const char* GetRoomTypes( int nSleeps );
	void SetRoomTypes( int nSleeps, const char* szTypes );

public:
	const char* GetText ( int nSleeps, int nSubType );
	bool GetComboFlag( int nSleeps, int nSubType );

	void SetText( int nSleeps, int nSubType, const char* szText );
	void SetComboFlag( int nSleeps, int nSubType, bool bFlag );
	
private:
	CString m_strRoomTypes;
	CString m_strText;
};

/**********************************************************************/
#endif
/**********************************************************************/
