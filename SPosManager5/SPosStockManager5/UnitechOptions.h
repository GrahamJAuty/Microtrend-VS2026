#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
 
/**********************************************************************/
#define UNITECH_MAX_LENGTH_FOLDER 100
#define UNITECH_MAX_LENGTH_FILENAME 20
#define UNITECH_MAX_COMM_PORT 8
/**********************************************************************/
#define UNITECH_STATUS_TYPE_NONE 0
#define UNITECH_STATUS_TYPE_REGISTRY 1
#define UNITECH_STATUS_TYPE_FILE 2
/**********************************************************************/
#include "EnhancedIniFile.h"
//include "MyComboBox.h"
/**********************************************************************/

class CUnitechOptions 
{
public:
	CUnitechOptions();
	void Reset();

	bool Read();
	bool Write();
	
	bool GetFatalReadErrorFlag()	{ return m_bFatalReadError; }
	void ClearFatalReadErrorFlag()	{ m_bFatalReadError = FALSE; }

	void CopyFrom( CUnitechOptions& source );

private:
	void ReloadFromIniFile( CEnhancedIniFile& iniFile );
	void PrepareIniFile( CEnhancedIniFile& iniFile );

public:
	bool GetEnableFlag()					{ return m_Enable.GetValue(); }
	const char* GetProgramFolder()			{ return m_ProgramFolder.GetValue(); }
	const char* GetProgramFile()			{ return m_ProgramFile.GetValue(); }
	int GetStatusType()						{ return m_StatusType.GetValue(); }
	const char* GetStatusFolder()			{ return m_StatusFolder.GetValue(); }
	const char* GetStatusFile()				{ return m_StatusFile.GetValue(); }
	int GetComPortNo()						{ return m_ComPortNo.GetValue(); }
	bool GetFastSpeedFlag()					{ return m_FastSpeed.GetValue(); }
	int GetNetUseFlag()						{ return m_NetUseFlag.GetValue(); }
	
	void SetEnableFlag( bool b )			{ m_Enable.SetValue(b); }
	void SetProgramFolder( const char* sz )	{ m_ProgramFolder.SetValue(sz); }
	void SetProgramFile( const char* sz )	{ m_ProgramFile.SetValue(sz); }
	void SetStatusType( int n )				{ m_StatusType.SetValue(n); }
	void SetStatusFolder( const char* sz )	{ m_StatusFolder.SetValue(sz); }
	void SetStatusFile( const char* sz )	{ m_StatusFile.SetValue(sz); }	
	void SetComPortNo( int n )				{ m_ComPortNo.SetValue(n); }
	void SetFastSpeedFlag( bool b )			{ m_FastSpeed.SetValue(b); }
	void SetNetUseFlag( int n )				{ m_NetUseFlag.SetValue(n); }

	const char* GetFullProgramPath();
	const char* GetFullStatusPath();

private:
	CIniFileBool m_Enable;
	CIniFileString m_ProgramFolder;
	CIniFileString m_ProgramFile;
	CIniFileInt m_StatusType;
	CIniFileString m_StatusFolder;
	CIniFileString m_StatusFile;
	CIniFileInt m_ComPortNo;
	CIniFileBool m_FastSpeed;
	CIniFileInt m_NetUseFlag;
				
public:
	bool m_bFatalReadError;

private:
	CString m_strFullProgramPath;
	CString m_strFullStatusPath;
};

/**********************************************************************/
extern CUnitechOptions UnitechOptions; /* global */
/**********************************************************************/
#endif
/**********************************************************************/

