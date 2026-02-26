#pragma once
/*********************************************************************/

class CDefTextUnit : public CSharedStringArray 
{
public:
	CDefTextUnit( int nType );

public:
	int Open( const char* szFilename, int nMode );
	
public:
	void SetDefaults();
	const char* GetText ( int nIndex );
	void AddUnit ( const char* szText );
	void RemoveUnit ( int nIndex );
	void SetText( int nIndex, const char* szText );
	int GetType() { return m_nType; }
	void SwapWithNext( int nIdx );

public:
	void CopyFrom( CDefTextUnit& Source );

private:
	void Read( SysFileInfo info );
	void Write( SysFileInfo info );

private:
	CString m_strText;
	int m_nType;
};

/**********************************************************************/

class CDefTextSystemCustomFieldSet : public CDefTextUnit
{
public:
	CDefTextSystemCustomFieldSet();
};

/**********************************************************************/
