#pragma once
/**********************************************************************/
#define NETWORK_ERROR_NONE 0
#define NETWORK_ERROR_FOLDER_DELETE 1
#define NETWORK_ERROR_FOLDER_CREATE 2
#define NETWORK_ERROR_FOLDER_RENAME 3
#define NETWORK_ERROR_FILE_DELETE 4
#define NETWORK_ERROR_FILE_CREATE 5
#define NETWORK_ERROR_FILE_RENAME 6
#define NETWORK_ERROR_FILE_APPEND 7
#define NETWORK_ERROR_FILE_COPY 8
/**********************************************************************/

class CNetworkTester  
{
public:
	CNetworkTester();
	void ShowError();

public:
	int TestNetwork();

private:
	void TestNetworkInternal( const char* szPath1, const char* szPath );

private:
	int m_nError;
	int m_nCode;
};

/**********************************************************************/

