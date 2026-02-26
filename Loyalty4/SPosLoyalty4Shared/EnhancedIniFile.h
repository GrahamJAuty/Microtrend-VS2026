#pragma once
/**********************************************************************/

struct CIniFileInt
{
public:
	CIniFileInt();
	void Create( const char* szLabel, int nMin, int nMax, int nDefault );

public:
	void Reset( int nValue );
	void Reset(){ m_nValue = m_nDefault; }
	
public:
	const char* GetLabel(){ return m_strLabel; }
	int GetValue(){ return m_nValue; }
	int GetDefault(){ return m_nDefault; }

public:
	void SetValue( int nValue );
	void SaveValue(){ m_nSavedValue = m_nValue; }
	void RestoreValue(){ m_nValue = m_nSavedValue; }

private:
	CString m_strLabel;
	int m_nValue;
	int m_nSavedValue;
	int m_nMin;
	int m_nMax;
	int m_nDefault;
};

/**********************************************************************/

struct CIniFileInt64
{
public:
	CIniFileInt64();
	void Create( const char* szLabel, __int64 nMin, __int64 nMax, __int64 nDefault );

public:
	void Reset( __int64 nValue );
	void Reset(){ m_nValue = m_nDefault; }
	
public:
	const char* GetLabel(){ return m_strLabel; }
	__int64 GetValue(){ return m_nValue; }
	__int64 GetDefault(){ return m_nDefault; }

public:
	void SetValue( __int64 nValue );
	void SaveValue(){ m_nSavedValue = m_nValue; }
	void RestoreValue(){ m_nValue = m_nSavedValue; }

private:
	CString m_strLabel;
	__int64 m_nValue;
	__int64 m_nSavedValue;
	__int64 m_nMin;
	__int64 m_nMax;
	__int64 m_nDefault;
};

/**********************************************************************/

struct CIniFileBool
{
public:
	CIniFileBool();
	void Create( const char* szLabel, bool bDefault );

public:
	void Reset( bool bValue ){ m_bValue = bValue; }
	void Reset(){ m_bValue = m_bDefault; }
	
public:
	const char* GetLabel(){ return m_strLabel; }
	bool GetValue(){ return m_bValue; }
	bool GetDefault(){ return m_bDefault; }

public:
	void SetValue( bool bValue ){ m_bValue = bValue; }
	void SaveValue(){ m_bSavedValue = m_bValue; }
	void RestoreValue(){ m_bValue = m_bSavedValue; }

private:
	CString m_strLabel;
	bool m_bValue;
	bool m_bSavedValue;
	bool m_bDefault;
};

/**********************************************************************/

struct CIniFileDouble
{
public:
	CIniFileDouble();
	void Create( const char* szLabel, double dMin, double dMax, double dDefault );

public:
	void Reset( double dValue );
	void Reset(){ m_dValue = m_dDefault; }
	
public:
	const char* GetLabel(){ return m_strLabel; }
	double GetValue(){ return m_dValue; }
	double GetDefault(){ return m_dDefault; }

public:
	void SetValue( double dValue );
	void SaveValue(){ m_dSavedValue = m_dValue; }
	void RestoreValue(){ m_dValue = m_dSavedValue; }

private:
	CString m_strLabel;
	double m_dValue;
	double m_dSavedValue;
	double m_dMin;
	double m_dMax;
	double m_dDefault;
};

/**********************************************************************/

struct CIniFileString
{
public:
	CIniFileString();
	void Create( const char* szLabel, int nMaxLen, const char* szDefault );

public:
	void Reset( const char* szValue );
	void Reset(){ m_strValue = m_strDefault; }
	
public:
	const char* GetLabel(){ return m_strLabel; }
	const char* GetValue(){ return m_strValue; }
	const char* GetDefault(){ return m_strDefault; }

public:
	void SetValue( const char* szValue );
	void SaveValue(){ m_strSavedValue = m_strValue; }
	void RestoreValue(){ m_strValue = m_strSavedValue; }

private:
	CString m_strLabel;
	CString m_strValue;
	CString m_strSavedValue;
	int m_nMaxLen;
	CString m_strDefault;
};

/**********************************************************************/

struct CIniFileIntArray
{
public:
	CIniFileIntArray();
	void Create( const char* szLabel, int nMin, int nMax, int nDefault, int nCount );

public:
	void Reset( int nIdx, int nValue );
	void Reset( int nIdx );
	
public:
	const char* GetLabel(){ return m_strLabel; }
	int GetDefault(){ return m_nDefault; }
	int GetCount(){ return m_arrayValues.GetSize(); }

public:
	int GetValue( int nIdx );
	void SetValue( int nIdx, int nValue );

private:
	CString m_strLabel;
	CArray<int,int> m_arrayValues;
	int m_nMin;
	int m_nMax;
	int m_nDefault;
};

/**********************************************************************/

struct CIniFileInt64Array
{
public:
	CIniFileInt64Array();
	void Create( const char* szLabel, __int64 nMin, __int64 nMax, __int64 nDefault, int nCount );

public:
	void Reset( int nIdx, __int64 nValue );
	void Reset( int nIdx );
	
public:
	const char* GetLabel(){ return m_strLabel; }
	__int64 GetDefault(){ return m_nDefault; }

public:
	__int64 GetValue( int nIdx );
	void SetValue( int nIdx, __int64 nValue );

private:
	CString m_strLabel;
	CArray<__int64,__int64> m_arrayValues;
	__int64 m_nMin;
	__int64 m_nMax;
	__int64 m_nDefault;
};

/**********************************************************************/

struct CIniFileStringArray
{
public:
	CIniFileStringArray();
	void Create( const char* szLabel, int nMaxLen, const char* szDefault, int nCount );

public:
	void Reset( int nIdx, const char* szValue );
	void Reset( int nIdx );
	
public:
	const char* GetLabel(){ return m_strLabel; }
	const char* GetDefault(){ return m_strDefault; }

public:
	const char* GetValue( int nIdx );
	void SetValue( int nIdx, const char* szValue );

private:
	CString m_strLabel;
	CStringArray m_arrayValues;
	int m_nMaxLen;
	CString m_strDefault;

private:
	CString m_strValue;
};

/**********************************************************************/

class CEnhancedIniFile : public CSSIniFile
{
public:
	CEnhancedIniFile( int nFlags = SS_INI_NORMAL );

public:
	void ReadInt( CIniFileInt& field );
	void ReadInt( CIniFileIntArray& field, int nIdx );
	void ReadInt64( CIniFileInt64& field );
	void ReadInt64( CIniFileInt64Array& field, int nIdx );
	void ReadBool( CIniFileBool& field );
	void ReadDouble( CIniFileDouble& field );
	void ReadString( CIniFileString& field );
	void ReadString( CIniFileStringArray& field, int nIdx );
	
public:
	void WriteInt( CIniFileInt& field );
	void WriteInt( CIniFileIntArray& field, int nIdx );
	void WriteInt64( CIniFileInt64& field );
	void WriteInt64( CIniFileInt64Array& field, int nIdx );
	void WriteBool( CIniFileBool& field );
	void WriteDouble( CIniFileDouble& field, int nDPs );
	void WriteString( CIniFileString& field );
	void WriteString( CIniFileStringArray& field, int nIdx );
};

/**********************************************************************/
