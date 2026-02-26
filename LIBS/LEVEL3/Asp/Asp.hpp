/**********************************************************************/
#pragma once
/**********************************************************************/

#define EXECUTE_SEND_STRING	0
#define EXECUTE_SEND_MIME_OK	1

/**********************************************************************/

#define ASP_CHARACTER_SET	"windows-1252"

// common attributes

#define ASP_ID				"Id"				// eg Id="acSearch"
#define ASP_CSS_CLASS		"CssClass"		// eg CssClass="btn"
#define ASP_BACK_COLOR		"BackColor"		// eg BackColor="White"
#define ASP_TEXT_COLOR		"TextColor"		// eg TextColor="Black" or TextColor="#606060"
#define ASP_BORDER_COLOR		"BorderColor"		// eg BorderColor="LightGray"
#define ASP_BORDER_WIDTH		"BorderWidth"		// eg BorderWidth="1px"
#define ASP_BORDER_STYLE		"BorderStyle"		// eg BorderStyle="solid"
#define ASP_CELL_PADDING		"CellPadding"		// eg CellPadding="4px"
#define ASP_CELL_SPACING		"CellSpacing"		// eg CellSpacing="0px"
#define ASP_WIDTH			"Width"			// eg Width="100%" or Width="800px"
#define ASP_STYLE			"Style"			// eg Style="display:inline-block;position:absolute;width:600px;"

// object specific attributes

#define ASP_TEXT			"Text"			// eg Text="200"
#define ASP_CHECKED			"Checked"			// eg Checked="checked"
#define ASP_SQL			"Sql"			// eg Sql="SELECT CustNo,Customer,Dealer,Comment FROM private/custlist/custlist.csv WHERE CustNo>=5000;"
#define ASP_SORT			"Sort"			// eg Sort="true"
#define ASP_BACK_COLOR_ALT	"BackColorAlt"		// eg BackColorAlt="Beige"
#define ASP_ROWS			"Rows"			// eg Rows="10"
#define ASP_COLUMNS			"Columns"			// eg Columns="10"
#define ASP_MAX_LENGTH		"MaxLength"		// eg MaxLength="120"
#define ASP_NAME			"Name"
#define ASP_REPEAT_COLUMNS	"RepeatColumns"	// eg RepeatColumns="5"
#define ASP_REPEAT_LAYOUT	"RepeatLayout"		// eg RepeatLayout="Table"
#define ASP_WEB_SERVICE_URL	"WebServiceUrl"	// eg WebServiceUrl="GPSWebService.ws"
#define ASP_TEXT_ALIGN		"TextAlign[]"		// eg TextAlign[]="left,left,left,left"
#define ASP_SORT_TYPE		"SortType[]"		// eg SortType[]="int,text,text,text"

/**********************************************************************/

class CAsp;

/**********************************************************************/

class CAspArray
{
public:
	~CAspArray();
	void AddObject ( CAsp* pObject );
	CAsp* FindID ( const char* szID );

private:
	void RemoveAll();

private:
	CPtrArray m_arPtr;
};

/**********************************************************************/
/*
	Asp::Execute()		order of events:

	1 create object
	2 call AspFunction (postback=false)
	3 parse user params
	4 call AspFunction (postback=true)
	5 create html
*/
/**********************************************************************/

class CAsp
{
public:
	CAsp();
	virtual ~CAsp() {}; // virtual so all derrived classes will be deleted correctly
	void Execute ( void (*)(bool,CCSV&,CAspArray&,CCSV&), const char* szSourceFilename, CString& strDest, CCSV& csvUserParams, CCSV& csvSessionCookies );

	CString GetId() { return m_strId; }
	CString GetTextColor() { return m_strTextColor; }
	void SetCssClass ( const char* szCssClass ) { m_strCssClass = szCssClass; }

protected:
	void ParseAttributes ( CCSV& csvAttributes, const char* szAttributes );
	void SetAttributes ( CCSV& csvAttributes );
	CString GetId ( CCSV& csvAttributes );

protected:
	void AddStyleAttribute ( const char* szStyle );	// eg. width:40px;
	CString GetStyleAttribute();
	CString GetCss ( const char* szLabel, const char* szValue );

protected:				// common attributes
	CString m_strId;
	CString m_strCssClass;
	CString m_strTextColor;
	CString m_strBackColor;
	CString m_strBorderColor;
	CString m_strBorderWidth;
	CString m_strBorderStyle;
	CString m_strCellPadding;
	CString m_strCellSpacing;
	CString m_strWidth;
private:
	CString m_strStyle;		// style elements contatenated
};

/**********************************************************************/
/*
	Atributes:

	Id, CssClass, Style, Text, MaxLength, Rows, WebServiceUrl
*/
/**********************************************************************/

class CAspAutoComplete : public CAsp
{
public:
	CAspAutoComplete();
	void SetAttributes ( CCSV& csvAttributes );
	void ParseUserParams ( CCSV& csvUserParams );
	void Write ( CString& strScript, CString& strStyle, CString& strBody, CString strPadding );

	void SetText ( const char* szText ) { m_strText = szText; }

private:
	void AddScript ( const char* szScript );

protected:				// attributes
	CString m_strText;
	CString m_strMaxLength;
	CString m_strWebServiceUrl;
	CString m_strRows;

private:
	CString m_strScript;
};

/**********************************************************************/
/*
	Atributes:

	Id, CssClass, Style, Text, Name
*/
/**********************************************************************/

class CAspButton : public CAsp
{
public:
	CAspButton();
	void SetAttributes ( CCSV& csvAttributes );
	void ParseUserParams ( CCSV& csvUserParams );
	void Write ( CString& strStyle, CString& strBody, CString strPadding );
	void SetText ( const char* szText ) { m_strText = szText; }

protected:				// attributes
	CString m_strText;
	CString m_strName;
};

/**********************************************************************/
/*
	Atributes:

	Id, CssClass, Style, Text, Checked
*/
/**********************************************************************/

class CAspCheckBox : public CAsp
{
public:
	CAspCheckBox();
	void SetAttributes ( CCSV& csvAttributes );
	void ParseUserParams ( CCSV& csvUserParams );
	void Write ( CString& strStyle, CString& strBody, CString strPadding );
	void SetText ( const char* szText ) { m_strText = szText; }

protected:				// attributes
	CString m_strText;
	CString m_strChecked;
};

/**********************************************************************/
/*
	Atributes:

	Id, RepeatColumns, RepeatLayout
*/
/**********************************************************************/

class CAspCheckBoxList : public CAsp
{
public:
	CAspCheckBoxList();
	void SetAttributes ( CCSV& csvAttributes );
	void ParseUserParams ( CCSV& csvUserParams );
	void Write ( CString& strStyle, CString& strBody, CString strPadding );
	void AddListItem ( const char* szText, bool bSelected );
	CString GetText ( int nIndex );
	bool GetSelected ( int nIndex );
	int GetSize() { return m_arList.GetSize(); }

private:
	void SetSelected ( int nIndex, bool b );
	CString GetItemHtml ( int x, int y );

private:
	CStringArray m_arList;
	int m_nRepeatColumns;
	CString m_strRepeatLayout;
};

/**********************************************************************/

class CAspDataGrid : public CAsp
{
public:
	CAspDataGrid();
	void SetAttributes ( CCSV& csvAttributes );
	void ParseUserParams ( CCSV& csvUserParams );
	void Write ( CString& strStyle, CString& strBody, CString strPadding );
	void SetTextAlign ( const char* szCSVTextAlign );

	CString GetCurrentSortField() { return m_strCurrentSortField; }
	CString GetCurrentSortDirection() { return m_strCurrentSortDirection; }

private:
	CString GetTextAlign ( CCSV& csv, int n );

public:
	CSSUnsortedDatabase m_db;	// database will close when object destroyed
	CSSDatabaseView m_dbView;

	bool m_bHide;

protected:				// attributes
	bool m_bSort;
	CString m_strSQL;
	CString m_strBackColorAlt;
	CString m_strCSVTextAlign;		// eg. left,left,right etc
	CString m_strCSVSortType;		// eg. int,text,float,date,time etc

private:
	CString m_strCurrentSortField;
	CString m_strCurrentSortDirection;
};

/**********************************************************************/

class CAspHtml : public CAsp
{
public:
	CAspHtml();
	void SetAttributes ( CCSV& csvAttributes );
	void ParseUserParams ( CCSV& csvUserParams );
	void Write ( CString& strHead, CString& strScript, CString& strStyle, CString& strBody, CString strPadding );

	void AddHead ( const char* szHead );
	void AddStyle ( const char* szStyle );
	void AddScript ( const char* szScript );
	void AddBody ( const char* szBody );

protected:				// attributes
	CString m_strHead;
	CString m_strStyle;
	CString m_strScript;
	CStringArray m_arBody;
};

/**********************************************************************/

class CAspLabel : public CAsp
{
public:
	CAspLabel();
	void SetAttributes ( CCSV& csvAttributes );
	void ParseUserParams ( CCSV& csvUserParams );
	void Write ( CString& strStyle, CString& strBody, CString strPadding );
	void SetText ( const char* szText ) { m_strText = szText; }

protected:				// attributes
	CString m_strText;
};

/**********************************************************************/

class CAspTextBox : public CAsp
{
public:
	CAspTextBox();
	void SetAttributes ( CCSV& csvAttributes );
	void ParseUserParams ( CCSV& csvUserParams );
	void Write ( CString& strStyle, CString& strBody, CString strPadding );
	void SetText ( const char* szText ) { m_strText = szText; }

protected:				// attributes
	CString m_strText;
	CString m_strColumns;
	CString m_strMaxLength;
};

/**********************************************************************/

class CHTML
{
public:
	CHTML ( const char* szPadding="", bool bCRLF=TRUE, int nTabSize=2 );
	void Begin ( const char* szElement, const char* szValue="" );
	void Attribute ( const char* szAttribute, const char* szValue );
	void End();
	CString GetHTML();

private:
	CString GetPadding();

private:
	CStringArray m_arHtml;
	CStringArray m_arStack;
	CString m_strPadding;
	bool m_bCRLF;
	int m_nTabSize;

	CString m_strCurrentTag;
	CString m_strCurrentElement;
	CString m_strCurrentValue;
	int m_nCurrentTabLevel;
};

/**********************************************************************/

class CSS
{
public:
	void Label ( const char* szLabel );
	void Add ( const char* szProperty, const char* szValue );
	CString GetCss();

private:
	CString m_strLabel;
	CString m_strProperties;
};

/**********************************************************************/

bool GetParamValue ( CCSV& csvParams, const char* szLabel, CString& strValue );

bool GetDouble ( CCSV& csvParams, const char* szLabel, double& dValue );
double GetDouble ( CCSV& csvParams, const char* szLabel, double dDefault, double dMin, double dMax );

bool GetInt ( CCSV& csvParams, const char* szLabel, int& nValue );
int GetInt ( CCSV& csvParams, const char* szLabel, int nDefault, int nMin, int nMax );

void GetTokens ( CCSV& csvTokens, const char* szBuf, const char* szDelimiters, const char* szQuotes );

/**********************************************************************/
