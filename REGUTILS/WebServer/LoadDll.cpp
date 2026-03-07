/**********************************************************************/

#include <io.h>
#include "Defines.h"
#include "MyServer.h"

/**********************************************************************/

class CDllObj
{
public:
	CDllObj ( const char* szName, HINSTANCE hInst, void* pFunction )
		: m_strName(szName), m_hInst(hInst), m_pFunction(pFunction) {}
public:
	CString m_strName;		// eg GPS
	HINSTANCE m_hInst;
	void* m_pFunction;
};

/**********************************************************************/

void* CMyServer::LoadDll ( const char* szName )
{
	void* pFunc = NULL;

	CString strName ( szName );
	strName.MakeLower();			// convert name to lower case

	for ( int i = 0 ; i < m_arDll.GetSize() ; i++ )		// check if dll already loaded
	{
		CDllObj* pObj = (CDllObj*) m_arDll.GetAt(i);
		if ( strName == pObj->m_strName )		// found it
		{
			pFunc = pObj->m_pFunction;
			break;
		}
	}

	if ( pFunc == NULL )		// dll not loaded
	{
		CString strFilename;
		strFilename.Format ( "%s/%s.dll", DLL_PATH, strName );
		if ( _access ( strFilename, 0 ) == 0 )					// file exists
		{
			HINSTANCE hInst = ::LoadLibrary ( strFilename );		// load dll

			if ( hInst != NULL )
			{
				int (*pFunction)( bool bPostBack, CCSV& csvUserParams, CAspArray& arObjects, CCSV& csvSessionCookies );
				pFunction = (int(*)( bool bPostBack, CCSV& csvUserParams, CAspArray& arObjects, CCSV& csvSessionCookies )) ::GetProcAddress ( hInst, "ExecuteFunction" );

				CDllObj* pObj = new CDllObj ( strName, hInst, pFunction );
				if ( pObj != NULL )
				{
					m_arDll.Add ( pObj );
				}

				pFunc = pFunction;

				CString str;
				str.Format ( "Loaded %s.dll", strName );
				Log ( "", "", str, -1 );
			}
		}
	}

	return pFunc;
}

/**********************************************************************/

void CMyServer::UnloadDlls()
{
	for ( int i = 0 ; i < m_arDll.GetSize() ; i++ )
	{
		CDllObj* pObj = (CDllObj*) m_arDll.GetAt(i);

		::FreeLibrary ( pObj->m_hInst );		// free library

		CString str;
		str.Format ( "Unloaded %s.dll", pObj->m_strName );
		Log ( "", "", str, -1 );

		delete pObj;
	}

	m_arDll.RemoveAll();
}

/**********************************************************************/
