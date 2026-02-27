#pragma once
/**********************************************************************/
 
/**********************************************************************/
#define MAX_ADDRESSES_PER_NICKNAME 10
/**********************************************************************/

class CDefTextEmailAddress : public CSharedStringArray 
{
public:
	CDefTextEmailAddress();
	
public:
	const char* GetNickname( int nIndex );
	int GetEmailAddressCount( int nIndex );
	const char* GetEmailAddress ( int nIndex, int nPos );
	const char* GetDisplayEmailAddress ( int nIndex );
	
	const char* GetComboText ( int nIndex );
	
	void SetAddressBookEntry ( int nIndex, const char* szName, CStringArray& arrayEmail );
	void AddAddressBookEntry ( const char* szName, CStringArray& arrayEmail );
	void RemoveAddressBookEntry ( int nIndex );
	
	void SwapWithNext( int nIndex );
	int FindNameOrFirstAddress( const char* szName );

public:
	void CopyFrom( CDefTextEmailAddress& Source );
	
private:
	CString m_strNickname;
	CString m_strEmailAddress;
	CString m_strDisplayEmailAddress;
	CString m_strComboText;
	CString m_strFilename;
};

/**********************************************************************/
extern CDefTextEmailAddress EmailAddresses;		/* global */
/**********************************************************************/
