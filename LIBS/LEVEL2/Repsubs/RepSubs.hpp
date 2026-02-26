/**********************************************************************/
#pragma once
/**********************************************************************/

/**********************************************************************/
/*	Report Manager Commands:									*/
/**********************************************************************/
/*
	<RIGHT>		Applies to Headers and Footers only
	<DATE>		Applies to Headers and Footers only
	<TIME>		Applies to Headers and Footers only
	<PAGENO>		Applies to Headers and Footers only

	<LI>			Display horizontal line in column (or all columns if only item)

	<COLUMN>Align,Width,Text{,NumberOfSubitems,Align,PercentWidth,Text...}

	<HEADER>
	<FOOTER>
	<TITLE>

	<HIDE>
	<HIDEn>		n = 1 to 9

	<..>			ignore column width (column spanning)

	<NEWPAGE>		new page
	<NEWPAGE>,n	new page and set page number to n

	<CF#RRGGBB>	foreground colour hex (precedes text string)
	<CB#RRGGBB>	background colour hex (precedes text string)
	<AL>			align left (precedes text string)
	<AC>			align center (precedes text string)
	<AR>			align right (precedes text string)
*/
/**********************************************************************/
/*	RULER.HPP												*/
/**********************************************************************/

class CSubTab : public CObject		// for CTabRuler use only!
{
public:
	CSubTab ( const char* szText, int nAlign, double dPercentWidth );

	const char* GetText() { return m_strText; }
	int GetAlign() { return m_nAlign; }
	double GetPercentWidth() { return m_dPercentWidth; }

private:
	CString m_strText;
	int m_nAlign;
	double m_dPercentWidth;
};

/**********************************************************************/

class CSubTabArray : public CObArray	// for CTabRuler use only!
{
public:
	~CSubTabArray();
	void Add ( const char* szText, int nAlign, double dPercentWidth );
	CSubTab* GetAt ( int nIndex ) { return (CSubTab*) CObArray::GetAt(nIndex); }
	void RemoveAll();
};

/**********************************************************************/

class CTabStop : public CObject		// for CTabRuler use only!
{
public:
	CTabStop ( const char* szText, int nAlign, int nWidth, int nOrder );
	~CTabStop();

	void AddSubTab ( const char* szText, int nAlign, double dPercentWidth );

	const char* GetText() { return m_strText; }
	int GetAlign() { return m_nAlign; }
	int GetWidth() { return m_nWidth; }
	int GetOrder() { return m_nOrder; }

	void SetWidth ( int nWidth );
	void SetAlign ( int nAlign );
	void SetOrder ( int nOrder );

	int GetSubTabSize();
	const char* GetSubTabText ( int nIndex );
	int GetSubTabAlign ( int nIndex );
	int GetSubTabWidth ( int nIndex );
	double GetSubTabPercentWidth ( int nIndex );

private:
	CSubTabArray m_SubTabArray;
	CString m_strText;
	int m_nAlign;
	int m_nWidth;
	int m_nOrder;
};

/**********************************************************************/

class CTabStopArray : public CObArray	// for CTabRuler use only!
{
public:
	~CTabStopArray();
	void Add ( const char* szText, int nAlign, int nWidth, int nOrder );
	CTabStop* GetAt ( int nIndex ) { return (CTabStop*) CObArray::GetAt(nIndex); }
	void RemoveAll();

	void AddSubTab ( int nIndex, const char* szText, int nAlign, double dPercentWidth );

	const char* GetText ( int nIndex );
	int GetAlign ( int nIndex );
	int GetWidth ( int nIndex );
	int GetOrder ( int nIndex );

	void SetWidth ( int nIndex, int nWidth );
	void SetAlign ( int nIndex, int nAlign );
	void SetOrder ( int nIndex, int nOrder );

	int GetSubTabSize ( int nIndex );
	const char* GetSubTabText ( int nIndex, int n );
	int GetSubTabAlign ( int nIndex, int n );
	int GetSubTabWidth ( int nIndex, int n );
	double GetSubTabPercentWidth ( int nIndex, int n );
};

/**********************************************************************/

class CTabRuler		// for report manager use only!
{
public:
	CTabRuler();
	~CTabRuler();

	CTabRuler& operator= ( CTabRuler& );	// assignment operator

	void RemoveAll();
	void AddColumn ( const char* szText, int nAlign, int nWidth );
	void AddSubColumn ( int nIndex, const char* szText, int nAlign, double dPercentWidth );

	int GetSize() { return m_TabStopArray.GetSize(); }
	const char* GetText ( int nIndex );
	int GetAlign ( int nIndex );
	int GetWidth ( int nIndex );
	int GetOrder ( int nIndex );

	void SetWidth ( int nIndex, int nWidth );
	void SetAlign ( int nIndex, int nAlign );
	void SetOrder ( int nIndex, int nOrder );

	int GetSubTabSize ( int nIndex );
	const char* GetSubTabText ( int nIndex, int n );
	int GetSubTabAlign ( int nIndex, int n );
	int GetSubTabWidth ( int nIndex, int n );
	double GetSubTabPercentWidth ( int nIndex, int n );
	int GetTotalWidth() { return m_nTotalWidth; }

	int GetOrderedSize();
	int GetOrderedWidth ( int nIndex );
	int GetOrderedAlign ( int nIndex );
	int GetOrderedIndex ( int nIndex );
	const char* GetOrderedSubTabText ( int nIndex );

private:
	void AddColumn ( const char* szText, int nAlign, int nWidth, int nOrder );
	void CreateOrderedArray();

private:
	CTabStopArray m_TabStopArray;
	CTabStopArray m_OrderedArray;
	int m_nTotalWidth;
	int m_nOrder;
	bool m_bOrdered;
};

/**********************************************************************/
/*	REPFILE.HPP											*/
/**********************************************************************/

class CReportFile
{
public:
	CReportFile();
	~CReportFile();

	bool Open ( const char* szFilename, bool bAppend=FALSE );
	virtual void Close();
	void SetStyle1 ( const char* szText );
	void SetTitle ( const char* szTitle );
	void SetHeader ( const char* szHeader );
	void SetFooter ( const char* szFooter );
	void AddColumn ( const char* szDescription, UINT wTabAlign, int nFieldWidth );
	void AddSubColumn ( int nIndex, const char* szDescription, UINT wTabAlign, double dPercentWidth );
	bool WriteLine ( const char* szText );
	bool WriteLine ( CCSV& csv );
	void WriteHeader();

	CString GetColourText ( const char* szText, DWORD dwFgColour=-1, DWORD dwBgColour=-1 );	// returns Text prefixed with colour info
																			// -1 means ignore colour
protected:
	CSSFile m_file;
	CStringArray m_arrayHeader;
	CString m_strTitle;
	CString m_strFooter;
	CTabRuler m_Ruler;
	bool m_bHeaderModified;
	bool m_bFileOpened;
	bool m_bAppend;
};

/**********************************************************************/
/*	PAGINATE.HPP											*/
/**********************************************************************/

class CPageArray : public CObArray		// for report manager use only!
{
public:
	~CPageArray();
	int Paginate ( CStringArray& TextArray, int nLinesPerPage );
	int GetIndex ( int nIndex );
	int GetCount ( int nIndex );
	int GetPageNo ( int nIndex );
	void RemoveAll();

private:
	void AddPage();
	void SetIndex ( int nIndex );
	void SetCount ( int nCount );
	void SetPageNo ( int nPageNo );
};

/**********************************************************************/
/*	REPORTFILEDOC.HPP										*/
/**********************************************************************/

class CReportFileDoc
{
public:
	CReportFileDoc ( const char* szFilename );

	void SaveAsCSV ( const char* szFilename );
	void SaveAsHTML ( const char* szFilename );

/**********************************************************************/
/*														*/
/*	N.B. The following functions are for report manager use only!	*/
/*														*/
/**********************************************************************/

public:
	CReportFileDoc();

	void Read ( CArchive& ar );
	void SaveAsCSV ( const char* szFilename, CTabRuler& ruler );
	void SaveAsHTML ( const char* szFilename, CTabRuler& ruler );
	void ToggleReveal();
	void RemoveAll();

	int Paginate ( int nLinesPerPage );
	int GetPageNo ( int nPageNo );
	const char* GetPageText ( int nPageNo, int nLine );

	const char* GetReportTitle() { return m_strTitle; }

	CTabRuler& GetRuler() { return m_Ruler; }

	int GetDisplayTextSize();
	const char* GetDisplayTextAt ( int n );

	int GetPrintTextSize();
	const char* GetPrintTextAt ( int n );

	int GetHeaderSize() { return m_HeaderArray.GetSize(); }
	
	const char* GetHeaderAt ( int n ) 
	{ 
		if (n < m_HeaderArray.GetSize())
		{
			return m_HeaderArray.GetAt(n);
		}
		else
		{
			return "";
		}
	}

	int GetFooterSize() { return m_FooterArray.GetSize(); }
	
	const char* GetFooterAt ( int n ) 
	{ 
		if (n < m_FooterArray.GetSize())
		{
			return m_FooterArray.GetAt(n);
		}
		else
		{
			return "";
		}
	}

	int GetSubHeaderSize() { return ( m_strSubHeader == "" ) ? 0 : 1; }
	const char* GetSubHeader() { return m_strSubHeader; }

private:
	bool ProcessHeader ( const char* szBuf );

private:
	CTabRuler m_Ruler;
	CStringArray m_HeaderArray;
	CStringArray m_FooterArray;
	CStringArray m_TextArray;
	CPageArray m_PageArray;
	CString m_strTitle;
	CString m_strSubHeader;
	int m_nRevealLevel;
};

/**********************************************************************/

class CCommandLineBuilder
{
public:
	CCommandLineBuilder();

public:
	void Reset();
	void AddToken( const char* szToken );

public:
	const char* GetLine(){ return m_strLine; }

private:
	CString m_strLine;
};

/**********************************************************************/

extern const char* szVERSION_REPSUBS;

/**********************************************************************/

