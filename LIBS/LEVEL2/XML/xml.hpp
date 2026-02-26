/**********************************************************************/
#pragma once
/**********************************************************************/

#include <afxinet.h>

/**********************************************************************/

class CSSXML
{
public:
	CSSXML();
	CSSXML ( const char* szDeclaration );
	void Begin ( const char* szElement );
	void Element ( const char* szElement, const char* szValue, const char* szType = "" );
	void Attribute ( const char* szAttribute, const char* szValue );
	void End();
	CString GetXML();
	bool WriteXML ( const char* szFilename, bool bCRLF=FALSE );

public:
	int GetXMLLineCount() { return m_arrayXML.GetSize(); }
	CString GetXMLLine(int n);

private:
	void Begin ( const char* szElement, const char* szValue );
	void EndAll();

private:
	CStringArray m_arrayXML;
	CStringArray m_arrayStack;
	CString m_strCurrentElement;
	CString m_strCurrentValue;
	CString m_strCurrentAttribute;
};

/**********************************************************************/

class CXMLNodeArray : public CPtrArray
{
public:
	CXMLNodeArray();
	CXMLNodeArray ( CXMLNodeArray* pParent, const char* szName );
	CXMLNodeArray* GetAt ( int n ) { return (CXMLNodeArray*)(CPtrArray::GetAt(n)); }

public:
	bool IsElement();
	bool IsAttribute();
	bool IsText();
	CString GetName() { return m_strName; }			// full name including namespace
	CString GetLocalName() { return m_strLocalName; }	// name excluding namespace
	CString GetValue() { return m_strValue; }		// value of text or attribute

public:
	CString GetFullName();	// DO NOT USE! does not handle namespace. eg. "xml.Transactions.Transaction"

public:
	CXMLNodeArray* m_pParent;
	CString m_strName;		// full name including namespace
	CString m_strLocalName;	// name excluding namespace
	CString m_strValue;
	int m_nType;
};

/**********************************************************************/
/*	private class Do Not Use!								*/
/**********************************************************************/

class CInhouseXMLParser
{
public:
	CInhouseXMLParser();
	virtual ~CInhouseXMLParser();
	CString ParseFile ( const char* szFilename, bool bValidate );
	CString Parse ( CString& strXML, bool bValidate );
	int FindNodes ( CXMLNodeArray& arFound, const char* szName );
	CXMLNodeArray* FindNode ( const char* szName );
	void Display ( const char* szFilename );

private:
	bool GetNext ( CString& strBuf, int& nOffset );
	void Begin ( const char* szElement );
	bool End();
	void Value ( const char* szValue, int nType );
	void Attribute ( const char* szAttribute );
	void RemoveAll();
	void RemoveAll ( CXMLNodeArray* pArray );
	void FindNodes ( CXMLNodeArray& arFound, CXMLNodeArray* pArray, const char* szName, bool bUseNamespace );
	CXMLNodeArray* FindNode ( CXMLNodeArray* pArray, const char* szName, bool bUseNamespace );
	void Display ( CXMLNodeArray* pArray, CSSFile& file );

protected:
	CXMLNodeArray* m_pNode;

private:
	CXMLNodeArray* m_pRoot;
};

/**********************************************************************/

class CSSXMLParser : public CInhouseXMLParser
{
public:
	CString ParseFile ( const char* szFilename, bool bValidate );							// returns error message. returns "" if ok
	int GetNode ( CXMLNodeArray* pNodeStart, CXMLNodeArray& arNodesFound, const char* szName );	// eg "xml.Transactions.Transaction"
																				// pNodeStart = NULL means start at beginning
																				// returns number of nodes found

	CString GetValue ( CXMLNodeArray* pNode, const char* szName );							// eg "Id" (can only be single name)

	CString GetValue ( const char* szPath, const char* szName );							// for getting values that are not in an array
																				// eg GetValue ( "xml.WebService", "Version" )

	void Display ( CXMLNodeArray* pNodeStart, const char* szFilename );						// pNodeStart = NULL means start at beginning

private:
	void Display ( CXMLNodeArray* pNode, CSSFile& file );
	void GetNode ( CXMLNodeArray& arNodesFound, CXMLNodeArray* pArray, const char* szName );
};

/**********************************************************************/

class CSSSoap : public CSSXML
{
public:
	CSSSoap ( const char* szVersion = "1.1" );
	CSSSoap ( const char* szDeclaration, const char* szVersion );
	void SetSoapAction ( const char* szSoapAction );

//	prefixes element/attribute with soap11: or soap12:
	void BeginSoap ( const char* szElement );
	void AttributeSoap ( const char* szAttribute, const char* szValue );

//	eg szObjectName = "/webservices/tempconvert.asmx"
	CString SendHttp ( const char* szIPAddress, const char* szObjectName, const char* szResponseFilename, bool bValidate, const char* szAgent );
	CString SendHttps ( const char* szIPAddress, const char* szObjectName, const char* szResponseFilename, bool bValidate, const char* szAgent );

	int FindNodes ( CXMLNodeArray& arFound, const char* szName ) { return m_Parse.FindNodes ( arFound, szName ); }
	CXMLNodeArray* FindNode ( const char* szName ) { return m_Parse.FindNode ( szName ); }

private:
	void Init ( const char* szVersion );
	CString Send ( bool bUseHttps, const char* szIPAddress, const char* szObjectName, const char* szResponseFilename, bool bValidate, const char* szAgent );

private:
	CInhouseXMLParser m_Parse;
	CString m_strSoapVersion;
	CString m_strContentType;
	CString m_strSoapAction;
};

/**********************************************************************/
/* these classes are for use by CSSJson only						*/
/**********************************************************************/

class CJsonLabelValueObj : public CObject
{
public:
	CJsonLabelValueObj ( const char* szLabel, const char* szValue );

public:
	CString m_strLabel;
	CString m_strValue;
};

class CJsonLabelValueArray : public CPtrArray
{
public:
	virtual ~CJsonLabelValueArray();
	void Add ( const char* szLabel, const char* szValue );
	CString GetLabel ( int n );
	CString GetValue ( int n );
	void RemoveAll();
};

/**********************************************************************/

class CSSJson
{
public:
	CSSJson();
	bool ParseFile ( const char* szFilename, bool bTestMode=FALSE );
	CString GetValue ( const char* szLabel );	// eg. GetValue ( "Sharp.Students[0].MIS_ID" )

public:
	// do not use the following function (for internal use only)
	CString GetLabel ( int nIndex ) { return m_arJson.GetLabel(nIndex); }

private:
	bool DiscardWhitespace ( const char* szFilename, bool bTestMode );
	void ParseLine ( CString& strBuf );
	CString GetFullLabel();
	void PushLabel ( CString strLabel );
	CString PopLabel();
	bool IsArray ( CString strName );
	void IncrementArray ( CString& strName );
	void CreateSortIndex();
	bool Find ( const char* szLabel, int& nIndex );

private:
	CJsonLabelValueArray m_arJson;
	CStringArray m_arStack;
	CUIntArray m_arIndex;
};

/**********************************************************************/
/* private class - do not use									*/
/**********************************************************************/

class CJsonObject
{
public:
	CJsonObject ( const char* szObject );
	CString GetValue ( const char* szLabel );

	int GetLabelCount() { return m_arLabel.GetSize(); }
	CString GetLabel ( int nIndex );

	int GetValueCount() { return m_arValue.GetSize(); }
	CString GetValue ( int nIndex );

private:
	void Parse ( const char* szObject );
	CString GetNextLabelValuePair ( CString& strObject, int& nCurPos );
	void AddLabelValue ( CString& strLabelValuePair );
	CString TrimNonEscapeQuotes ( CString strLabelValue );

private:
	CStringArray m_arLabel;
	CStringArray m_arValue;
};

/**********************************************************************/
/*	This is not a general purpose json parser. It is specifically	*/
/*	for tucasi and assumes each object has the same number of		*/
/*	fields and the same label names for each field				*/
/**********************************************************************/

class CTucasiJson
{
public:
	CTucasiJson() {};
	virtual ~CTucasiJson();
	bool ParseFile ( const char* szFilename );	// false = cannot open file

	int GetObjectCount() { return m_arObject.GetSize(); }			// number of objects (records)
	const char* GetValue ( const char* szLabel, int nIdxRecord );

	int GetLabelCount();					// number of labels (fields) per record (assumes all records same size)
	const char* GetLabel ( int nIdxField );		// assumes record 0

	bool FindLabel ( const char* szLabel );		// returns true if label exists (assumes record 0)

	int GetLabelCount ( int nIdxRecord );
	CString GetLabel ( int nIdxRecord, int nIdxField );
	CString GetValue ( int nIdxRecord, int nIdxField );

private:
	void RemoveAll();
	void Parse ( CString& strJson );
	void DiscardWhitespace ( CString& strBuf );
	CString GetNextObject ( CString& strJson, int& nCurPos );

private:
	CPtrArray m_arObject;		// array of CJsonObject pointers
};

/**********************************************************************/
/*	This is not a general purpose json creator. It is specifically	*/
/*	for tucasi.											*/
/**********************************************************************/

class CTucasiJsonOut
{
public:
	CTucasiJsonOut() {};
	virtual ~CTucasiJsonOut();
	void RemoveAll();

	void AddObject();
	void Add ( const char* szLabel, const char* szValue, bool bQuotes=TRUE );
	void GetJson ( CString& strBuf );

private:
	void GetObject ( CString& strBuf, int nIndex );

private:
	CPtrArray m_arObject;	// array of CStringArray pointers
};

/**********************************************************************/

CString HttpGet ( bool bUseHttps, const char* szIPAddress, const char* szObjectName, CStringArray& arHeaders, const char* szBody, const char* szResponseFilename, const char* szAgent );
CString HttpPost ( bool bUseHttps, const char* szIPAddress, const char* szObjectName, CStringArray& arHeaders, const char* szBody, const char* szResponseFilename, const char* szAgent);

/**********************************************************************/

CString TucasiGet ( bool bUseHttps, const char* szIPAddress, const char* szObjectName, const char* szBody, const char* szResponseFilename, const char* szUsername, const char* szPassword, const char* szProviderName, const char* szProviderVersion, const char* szAgent );
CString TucasiPost ( bool bUseHttps, const char* szIPAddress, const char* szObjectName, const char* szBody, const char* szResponseFilename, const char* szUsername, const char* szPassword, const char* szProviderName, const char* szProviderVersion, const char* szAgent );

/**********************************************************************/

extern const char* szVERSION_XML;

/**********************************************************************/
