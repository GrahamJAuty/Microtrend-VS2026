/**********************************************************************/
#include "MaxLengths.h"
/**********************************************************************/
#include "OPNameArray.h"
/**********************************************************************/

COPNameMiniInfo::COPNameMiniInfo()
{
	m_nType = 0;
	m_nPrinterNo = 0;
	m_bDefer = FALSE;
	m_strName = "";
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

COPNameInfo::COPNameInfo()
{
	Reset();
}

/**********************************************************************/

void COPNameInfo::Reset()
{
	m_nPrinterNo = 0;
	m_bDeferDatabase = TRUE;
	m_bDeferLocation = TRUE;
	ResetNames();
}

/**********************************************************************/

void COPNameInfo::ResetNames()
{
	m_strNameSystem = "";
	m_strNameDatabase = "";
	m_strNameLocation = "";
}

/**********************************************************************/

void COPNameInfo::SetPrinterNo( int n )
{
	if ((n >= 1) && (n <= MAX_OP_NUMBER))
	{
		m_nPrinterNo = n;
	}
}

/**********************************************************************/

void COPNameInfo::SetNameSystem( CString str )
{
	if (str.GetLength() <= MAX_LENGTH_OPNAME)
	{
		m_strNameSystem = str;
	}
}

/**********************************************************************/

void COPNameInfo::SetNameDatabase(CString str)
{
	if (str.GetLength() <= MAX_LENGTH_OPNAME)
	{
		m_strNameDatabase = str;
	}
}

/**********************************************************************/

void COPNameInfo::SetNameLocation(CString str)
{
	if (str.GetLength() <= MAX_LENGTH_OPNAME)
	{
		m_strNameLocation = str;
	}
}

/**********************************************************************/

void COPNameInfo::Add (COPNameInfo& source )
{
}

/**********************************************************************/

int COPNameInfo::Compare( COPNameInfo& source, int nHint )
{
	if (m_nPrinterNo != source.m_nPrinterNo)
	{
		return ((m_nPrinterNo > source.m_nPrinterNo) ? 1 : -1);
	}
	else
	{
		return 0;
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

COPNameTable::COPNameTable(void)
{
	for ( int n = 1; n <= MAX_OP_NUMBER; n++ )
	{
		CString str = "";
		str.Format( "Printer %d", n );
		AddOPName( n, str );
	}
}

/**********************************************************************/

void COPNameTable::AddOPName( int nPrinterNo, const char* szName )
{
	COPNameInfo infoOPName;
	infoOPName.SetPrinterNo( nPrinterNo );
	infoOPName.SetNameSystem( szName );
	m_arrayOPNames.DirectAdd( infoOPName );
}

/**********************************************************************/

void COPNameTable::ResetOPNameList( int nOPNameMode )
{
	switch( nOPNameMode )
	{
	case OPNAME_SYSTEM:
		for ( int n = 0; n < GetSize(); n++ )
		{
			COPNameInfo infoOPName;
			m_arrayOPNames.GetAt( n, infoOPName );
			infoOPName.SetNameSystem("");
			m_arrayOPNames.SetAt( n, infoOPName );
		}
		break;

	case OPNAME_DATABASE:
		for ( int n = 0; n < GetSize(); n++ )
		{
			COPNameInfo infoOPName;
			m_arrayOPNames.GetAt( n, infoOPName );
			infoOPName.SetNameDatabase("");
			infoOPName.SetDeferFlagDatabase( TRUE );
			m_arrayOPNames.SetAt( n, infoOPName );
		}
		break;

	case OPNAME_LOCATION:
		for ( int n = 0; n < GetSize(); n++ )
		{
			COPNameInfo infoOPName;
			m_arrayOPNames.GetAt( n, infoOPName );
			infoOPName.SetNameLocation("");
			infoOPName.SetDeferFlagLocation( TRUE );
			m_arrayOPNames.SetAt( n, infoOPName );
		}
		break;
	}
}

/**********************************************************************/

bool COPNameTable::GetOPName( COPNameMiniInfo& infoMini )
{
	bool bResult = FALSE;

	COPNameInfo infoOPName;
	infoOPName.SetPrinterNo( infoMini.m_nPrinterNo );

	int nPrinterIdx;
	if (m_arrayOPNames.Find(infoOPName, nPrinterIdx) == TRUE)
	{
		bResult = GetOPName(nPrinterIdx, infoMini);
	}

	return bResult;
}

/**********************************************************************/

bool COPNameTable::GetOPName( int nPrinterIdx, COPNameMiniInfo& infoMini )
{
	bool bResult = FALSE;

	if ( ( nPrinterIdx >= 0 ) && ( nPrinterIdx < m_arrayOPNames.GetSize() ) )
	{
		COPNameInfo infoOPName;
		m_arrayOPNames.GetAt( nPrinterIdx, infoOPName );

		infoMini.m_nPrinterNo = infoOPName.GetPrinterNo();
			
		switch( infoMini.m_nType )
		{
		case OPNAME_SYSTEM:
			infoMini.m_bDefer = FALSE;
			infoMini.m_strName = infoOPName.GetNameSystem();
			bResult = TRUE;
			break;

		case OPNAME_DATABASE:
			infoMini.m_bDefer = infoOPName.GetDeferFlagDatabase();
			infoMini.m_strName = infoOPName.GetNameDatabase();
			bResult = TRUE;
			break;

		case OPNAME_LOCATION:
			infoMini.m_bDefer = infoOPName.GetDeferFlagLocation();
			infoMini.m_strName = infoOPName.GetNameLocation();	
			bResult = TRUE;
			break;
		}
	}
	return bResult;
}

/**********************************************************************/

void COPNameTable::UpdateOPName( COPNameMiniInfo& infoMini )
{
	COPNameInfo infoOPName;
	infoOPName.SetPrinterNo( infoMini.m_nPrinterNo );

	int nPrinterIdx = 0;
	if (m_arrayOPNames.Find(infoOPName, nPrinterIdx) == TRUE)
	{
		UpdateOPName(nPrinterIdx, infoMini);
	}
}

/**********************************************************************/

void COPNameTable::UpdateOPName( int nPrinterIdx, COPNameMiniInfo& infoMini )
{
	if ( ( nPrinterIdx >= 0 ) && ( nPrinterIdx < m_arrayOPNames.GetSize() ) )
	{
		COPNameInfo infoOPName;
		m_arrayOPNames.GetAt( nPrinterIdx, infoOPName );

		if ( infoOPName.GetPrinterNo() == infoMini.m_nPrinterNo )
		{
			switch( infoMini.m_nType )
			{
			case OPNAME_SYSTEM:
				infoOPName.SetNameSystem( infoMini.m_strName );
				break;

			case OPNAME_DATABASE:
				infoOPName.SetNameDatabase( infoMini.m_strName );
				infoOPName.SetDeferFlagDatabase( infoMini.m_bDefer );
				break;

			case OPNAME_LOCATION:
				infoOPName.SetNameLocation( infoMini.m_strName );
				infoOPName.SetDeferFlagLocation( infoMini.m_bDefer );
				break;
			}

			m_arrayOPNames.SetAt( nPrinterIdx, infoOPName );
		}
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

COPNameArray::COPNameArray( int nOPNameMode, COPNameTable& OPNameTable ) : CSharedStringArray(), m_OPNameTable( OPNameTable )
{
	m_nOPNameMode = nOPNameMode;
}

/**********************************************************************/

int COPNameArray::Open( const char* szFilename, int nMode )
{
	RemoveAt( 0, GetSize() );
	
	m_OPNameTable.ResetOPNameList( m_nOPNameMode );

	int nReply = CSharedStringArray::Open ( szFilename, nMode );
	
	if ( nReply != DB_ERR_SHARE )
	{
		for (int n = 0; n < GetSize(); n++)
		{
			CCSV csv(GetAt(n));

			if (csv.GetInt(0) == 1)
			{
				COPNameMiniInfo infoMini;
				infoMini.m_nType = m_nOPNameMode;
				infoMini.m_nPrinterNo = csv.GetInt(1);
				infoMini.m_bDefer = csv.GetBool(2);
				infoMini.m_strName = csv.GetString(3);
				m_OPNameTable.UpdateOPName(infoMini);
			}
		}

		if (OPNAME_SYSTEM == m_nOPNameMode)
		{
			for (int n = 0; n < m_OPNameTable.GetSize(); n++)
			{
				COPNameMiniInfo info;
				m_OPNameTable.GetOPName(n, info);

				if ( info.m_strName == "" )
				{
					info.m_strName.Format("Printer %d", info.m_nPrinterNo);
					m_OPNameTable.UpdateOPName(n, info);
				}
			}
		}
	}

	RemoveAt( 0, GetSize() );
	return nReply;
}

/**********************************************************************/

void COPNameArray::PrepareForWrite()
{
	RemoveAt( 0, GetSize() );

	for ( int nIdx = 0; nIdx < m_OPNameTable.GetSize(); nIdx++ )
	{
		COPNameMiniInfo infoMini;
		infoMini.m_nType = m_nOPNameMode;
		m_OPNameTable.GetOPName( nIdx, infoMini );

		CCSV csv;
		csv.Add(1);
		csv.Add(infoMini.m_nPrinterNo );
		csv.Add(infoMini.m_bDefer );
		csv.Add(infoMini.m_strName);
		Add( csv.GetLine() );
	}
}

/**********************************************************************/

void COPNameArray::UpdateOPName( COPNameMiniInfo& infoMini )
{
	infoMini.m_nType = m_nOPNameMode;
	m_OPNameTable.UpdateOPName( infoMini );
}

/**********************************************************************/

void COPNameArray::UpdateOPName( int nPrinterIdx, COPNameMiniInfo& infoMini )
{
	infoMini.m_nType = m_nOPNameMode;
	m_OPNameTable.UpdateOPName( nPrinterIdx, infoMini );
}

/**********************************************************************/

void COPNameArray::GetOPName( COPNameMiniInfo& infoMini )
{
	infoMini.m_nType = m_nOPNameMode;
	m_OPNameTable.GetOPName( infoMini );
}

/**********************************************************************/

void COPNameArray::GetOPName( int nPrinterIdx, COPNameMiniInfo& infoMini )
{
	infoMini.m_nType = m_nOPNameMode;
	m_OPNameTable.GetOPName( nPrinterIdx, infoMini );
}

/**********************************************************************/

void COPNameArray::ResetOPNameList()
{
	m_OPNameTable.ResetOPNameList( m_nOPNameMode );
}

/**********************************************************************/
