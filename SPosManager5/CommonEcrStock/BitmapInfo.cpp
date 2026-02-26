/**********************************************************************/
 
/**********************************************************************/
//#include "globalfunctions.h"
/**********************************************************************/
#include "BitmapInfo.h"
/**********************************************************************/

CBitmapInfo::CBitmapInfo()
{
	m_bCreatedDC = FALSE;
	m_bGotImage = FALSE;
	m_nWidth = 0;
	m_nHeight = 0;
	m_strImagePath = "";
}

/**********************************************************************/

void CBitmapInfo::CreateOwnDC( CDC* pDC )
{
	if ( FALSE == m_bCreatedDC )
	{
		if ( m_dcImage.CreateCompatibleDC( pDC ) != NULL )
		{
			m_bCreatedDC = TRUE;
			m_bGotImage = FALSE;
			m_nWidth = 0;
			m_nHeight = 0;
			m_strImagePath = "";
		}
	}
}

/**********************************************************************/

void CBitmapInfo::DeleteOwnDC()
{
	if ( TRUE == m_bCreatedDC )
	{
		m_dcImage.DeleteDC();
		m_bCreatedDC = FALSE;
		m_bGotImage = FALSE;
		m_nWidth = 0;
		m_nHeight = 0;
		m_strImagePath = "";
	}
}

/**********************************************************************/

void CBitmapInfo::LoadImage( const char* szImagePath, bool bForceReload )
{
	if ( TRUE == m_bCreatedDC )
	{
		if ( ( m_strImagePath != szImagePath ) || ( TRUE == bForceReload ) )
		{
			m_bGotImage = FALSE;

			if ( ::FileExists( szImagePath ) == TRUE )
			{
				CImage Image;
				if ( ( Image.Load( szImagePath ) & 0x80000000 ) == 0 )
				{
					m_nWidth = Image.GetWidth();
					m_nHeight = Image.GetHeight();
					m_bGotImage = TRUE;
					m_strImagePath = szImagePath;
					
					CBitmap bm; 
					bm.Attach( Image.Detach() );
					m_dcImage.SelectObject( &bm );
				}
			}

			if ( FALSE == m_bGotImage )
			{
				m_nWidth = 0;
				m_nHeight = 0;
				m_strImagePath = "";
			}
		}
	}
}

/**********************************************************************/
