/**********************************************************************/
#pragma once
/**********************************************************************/

class CPosTrayTokenFiles
{
public:
	static void CreateStopFile();
	static void DeleteStopFile();
	static bool StopFileExists();

public:
	static void CreateSystemUpdateFile();
	static void DeleteSystemUpdateFile();
	static bool SystemUpdateFileExists();
	static void GetSystemUpdateFileTime( COleDateTime& oleTime);

public:
	static void CreateRetryEmailFile();
	static void DeleteRetryEmailFile();
	static bool RetryEmailFileExists();
};

/**********************************************************************/
