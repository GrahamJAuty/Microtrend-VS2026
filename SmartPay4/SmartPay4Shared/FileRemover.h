#pragma once
/**********************************************************************/

class CFileRemover  
{
public:
	CFileRemover();
	CFileRemover( const char* szFile );

public:
	bool RemoveFile( const char* szFile );
	void RemoveFolder( const char* szFolder );
	void RemoveFolderContents( const char* szFolder, int& nFileNo ); 

private:
	bool IsProtected( const char* szFile );
};

/**********************************************************************/
