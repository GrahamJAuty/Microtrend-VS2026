/*********************************************************************/
#ifdef STOCKMAN_SYSTEM
/*********************************************************************/
#pragma once
/*********************************************************************/
 
/**********************************************************************/

class CDefTextDelUnit : public CSharedStringArray 
{
public:
	CDefTextDelUnit();
	
public:
	const char* GetText ( int nIndex );
	const char* GetComboText ( int nIndex );
	int GetDUItems ( int nIndex );
	double GetDUItemSize( int nIndex );
	double GetTotalSU( int nIndex );
	void SetDelUnit ( int nIndex, const char* szText, int nDUItems, double dDUItemSize );
	void AddDelUnit ( const char* szText, int nDUItems, double dDUItemSize );
	void RemoveDelUnit ( int nIndex );
	void SwapWithNext( int nIdx );


public:
	void CopyFrom( CDefTextDelUnit& Source );
	
private:
	CString m_strText;
	CString m_strComboText;
	CString m_strFilename;
};

/**********************************************************************/
#endif
/**********************************************************************/