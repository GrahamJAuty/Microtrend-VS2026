/**********************************************************************/

void CSSRegister::LogRegistration ( const char* szSerialNo, const char* szAccessCode, const char* szUserCode, const char* szProgramName )
{
	CTime tm = CTime::GetCurrentTime();
	CString strDate = tm.Format ( "%d-%m-%Y" );
	CString strTime = tm.Format ( "%H:%M:%S" );

	CCSV csv;
	csv.Add ( strDate );
	csv.Add ( strTime );
	csv.Add ( szSerialNo );
	csv.Add ( szAccessCode );
	csv.Add ( szUserCode );
	csv.Add ( szProgramName );

	CSSFile file;
	if ( _access ( m_strLogFilename, 0 ) != 0 )		// file does not exist
	{
		if ( file.Open ( m_strLogFilename, "wb" ) == TRUE )
		{
			file.WriteLine ( "Date,Time,SerialNo,AccessCode,UserCode,ProgramName" );
			file.WriteLine ( csv.GetLine() );
			file.Close();
		}
	}
	else if ( file.Open ( m_strLogFilename, "ab" ) == TRUE )
	{
		file.WriteLine ( csv.GetLine() );
		file.Close();
	}
}

/**********************************************************************/
