/**********************************************************************/
#include "ServerCSVArray.h"
/**********************************************************************/
#include "ServerDatabaseHeader.h"
/**********************************************************************/

CServerDatabaseHeader::CServerDatabaseHeader(void)
{
	AddField(Server::ServerNo.Label);
	AddField("");	//protected "VersionNo"
	AddField("");	//protected "TimeStamp"
	AddField(Server::ReceiptName.Label);
	AddField(Server::FullName.Label);
	AddField(Server::Authority.Label);
	AddField(Server::Password.Label);
	AddField(Server::Drawer.Label);
	AddField(Server::FlipView.Label);
	AddField(Server::TrainingMode.Label);
	AddField(Server::LogonMacro.Label);
	AddField(Server::OpenTableMode.Label);
	AddField(Server::HomeNwkLocNo.Label);
	AddField(Server::HourlyRate.Label);
	AddField(Server::DailyHours.Label);
	AddField(Server::MidnightShift.Label);
	AddField(Server::TradingColourScheme.Label);
}

/**********************************************************************/

void CServerDatabaseHeader::AddField( const char* szCurrent )
{
	m_arrayCurrentFields.Add( szCurrent );
}

/**********************************************************************/

const char* CServerDatabaseHeader::GetDatabaseHeader()
{
	CCSV csv;
	for (int n = 0; n < m_arrayCurrentFields.GetSize(); n++)
	{
		csv.Add(m_arrayCurrentFields.GetAt(n));
	}

	m_strHeader = csv.GetLine();
	return m_strHeader;
}

/**********************************************************************/

