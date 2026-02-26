#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/

template <class T>

class CEntityIDCreator
{
public:
	bool CreateEntityID( T& arrayItems, CString& strNewID );
};

/**********************************************************************/

template <class T> bool CEntityIDCreator<T>::CreateEntityID( T& arrayItems, CString& strNewID )
{
	CString strTimeStamp = "";
	
	{
		COleDateTime timeNow = COleDateTime::GetCurrentTime();
		strTimeStamp.Format( "%3.3X%1.1X%2.2X%2.2X%2.2X%2.2X",
			timeNow.GetYear(),
			timeNow.GetMonth(),
			timeNow.GetDay(),
			timeNow.GetHour(),
			timeNow.GetMinute(),
			timeNow.GetSecond());
	}

	bool bResult = TRUE;
	for ( int n = 0; n < arrayItems.GetSize(); n++ )
	{
		if ( strTimeStamp == arrayItems.GetEntityID(n) )
		{
			bResult = FALSE;
			break;
		}
	}

	if (TRUE == bResult)
	{
		strNewID = strTimeStamp;
	}
	else
	{
		strNewID = "";
	}

	return bResult;
}

/**********************************************************************/
#endif
/**********************************************************************/
