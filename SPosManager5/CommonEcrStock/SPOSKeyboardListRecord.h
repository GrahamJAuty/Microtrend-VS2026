#pragma once
/**********************************************************************/
#define MAX_LIST_NO 32767
#define MAX_LIST_PAGES 100
#define MAX_LIST_KEYS 1000
/**********************************************************************/
#define LISTTYPE_ITEM_STANDARD 0
#define LISTTYPE_ITEM_LARGE 1
#define LISTTYPE_FUNCTION_STANDARD 2
#define LISTTYPE_FUNCTION_LARGE 3
#define LISTTYPE_FUNCTION_BOTTOM 4
/**********************************************************************/
#define LISTACTION_NORMAL 0
#define LISTACTION_SERVER 1
#define LISTACTION_TABLE 2
/**********************************************************************/

class CSPOSKeyboardListRecord  
{
public:
	CSPOSKeyboardListRecord();
	void Reset();
	
public:
	__int64 GetListNo() { return m_nListNo; }
	const char* GetName() { return m_strName; }
	int GetFormat() { return m_nFormat; }
	bool GetStaydownFlag() { return m_bStaydown; }
	int GetNextList() { return m_nNextList; }
	int GetMinItems() { return m_nMinItems; }
	int GetMaxItems() { return m_nMaxItems; }
	int GetPageCount() { return m_nPageCount; }
	int GetListType() { return m_nListType; }
	int GetListActionType(){ return m_nListActionType; }
	int GetTableNoLo(){ return m_nTableNoLo; }
	int GetTableNoHi(){ return m_nTableNoHi; }
	int GetTableArea(){ return m_nTableArea; }
	bool GetPriorityFlag(){ return m_bPriority; }
	int GetStackMode(){ return m_nStackMode; }

	int GetForeground86() { return m_nForeground86; }
	int GetBackground86() { return m_nBackground86; }
	bool GetBorder86(){ return m_bBorder86; }
	int GetFontSize86(){ return m_nFontSize86; }
	bool GetBold86(){ return m_bBold86; }
	int GetWidth86(){ return m_nWidth86; }
	int GetHeight86(){ return m_nHeight86; }

	bool GetAllowChangeFlag(){ return m_bAllowChange; }
	bool GetOfferPluLinkFlag() { return m_bOfferPluLink;  }
	
	bool MatchViewModeType( int nMode );

	void SetListNo( __int64 n );
	void SetName( const char* szName );
	void SetFormat( int nFormat );
	void SetFormat( int nRows, int nCols );
	void SetStaydownFlag( bool b ) { m_bStaydown = b; }
	void SetNextList( int n );
	void SetMinItems( int n ); 
	void SetMaxItems( int n ); 
	void SetPageCount( int n );
	void SetListType( int n );
	void SetListActionType( int n );
	void SetTableNoLo( int n );
	void SetTableNoHi( int n );
	void SetTableArea( int n );
	void SetPriorityFlag( bool b ){ m_bPriority = b; }
	void SetStackMode( int n );

	void SetForeground86( int n );
	void SetBackground86( int n );
	void SetBorder86( bool b ){ m_bBorder86 = b; }
	void SetFontSize86( int n );
	void SetBold86( bool b ){ m_bBold86 = b; }
	void SetWidth86( int n );
	void SetHeight86( int n );

	void SetAllowChangeFlag( bool b ){ m_bAllowChange = b; }
	void SetOfferPluLinkFlag(bool b) { m_bOfferPluLink = b; }
	
	void SetDefaultListFormat();
	
	int GetRowCount() { return m_nFormat / 100; }
	int GetColumnCount() { return m_nFormat % 100; }
	int GetKeysPerPage() { return GetRowCount() * GetColumnCount(); }
	
	bool ValidateFormat( int nRows, int nCols );
	bool ValidateFormat( int nFormat );

	int GetMaxNewPages( int nCurrentPages );
	const char* GetPageError( int n, int nPageCount );

public:
	void GetKeyPixelSize( int& nKeyWidth, int& nKeyHeight, int& nXKeySpace, int& nYKeySpace );
	
private:
	int CalculateListWidth( double dPercent );
	int CalculateListHeight( double dPercent );
	int CalculateListPos( int nRange, double dPercent );
	
private:
	__int64 m_nListNo;
	CString m_strName;
	int m_nFormat;
	bool m_bStaydown;
	int m_nNextList;
	int m_nMinItems;
	int m_nMaxItems;
	int m_nPageCount;
	int m_nListType;
	int m_nListActionType;
	int m_nTableNoLo;
	int m_nTableNoHi;
	int m_nTableArea;
	bool m_bPriority;
	int m_nStackMode;

	int m_nForeground86;
	int m_nBackground86;
	bool m_bBorder86;
	int m_nFontSize86;
	bool m_bBold86;
	int m_nWidth86;
	int m_nHeight86;

	bool m_bAllowChange;
	bool m_bOfferPluLink;

	int m_nTotalScreenWidth;
	int m_nTotalScreenHeight;

	CString m_strComboText;
	CString m_strPageError;
};

/**********************************************************************/

