/**********************************************************************/
#include "PictCtrl.h"
/**********************************************************************/

IMPLEMENT_DYNAMIC(CPictureCtrl, CStatic)
BEGIN_MESSAGE_MAP(CPictureCtrl, CStatic)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

/**********************************************************************/

CPictureCtrl::CPictureCtrl()
{
	m_bBlank = FALSE;
	m_bFailed = FALSE;
	m_strWidth = "";
	m_strHeight = "";
}

/**********************************************************************/

CPictureCtrl::~CPictureCtrl()
{
}

/**********************************************************************/

int CPictureCtrl::OnCreate(LPCREATESTRUCT lpcs)
{
	if (CStatic::OnCreate(lpcs)!=0)
		return -1;

	return 0;
}

/**********************************************************************/

void CPictureCtrl::OnPaint()
{
	CPaintDC dc(this);
	
	if ( ( m_pict ) && ( m_bBlank == FALSE ) ) 
	{
		CRect rcClient;
		GetClientRect(&rcClient);
		CRect rcImage(CPoint(0,0),m_pict.GetImageSize());
		CRect rc;
		rc.IntersectRect(&rcImage, &rcClient);
		m_pict.Render(&dc, rc);
	}
	else
	{
		CRect rcClient;
		GetClientRect(&rcClient);
		CRect rc = rcClient;

		CBrush *pBrush =
			CBrush::FromHandle((HBRUSH)GetWindowLong(m_hWnd, GCL_HBRBACKGROUND));
	
		dc.FillRect(&rcClient, pBrush);

		if ( TRUE == m_bFailed )
			dc.ExtTextOut ( rcClient.left, rcClient.top, ETO_CLIPPED, &rcClient, "Unable to load image", NULL );
	}
}

/**********************************************************************/

BOOL CPictureCtrl::OnEraseBkgnd(CDC* pDC)
{
	// get client rectangle
	CRect rcClient;
	GetClientRect(&rcClient);
	CRect rc = rcClient;

	// get image rectangle
	CRect rcImage(CPoint(0,0), m_pict.GetImageSize());

	// create clipping region
	CRgn clipRgn;
	clipRgn.CreateRectRgnIndirect(&rcClient);
	pDC->SelectClipRgn(&clipRgn);
	pDC->ExcludeClipRect(&rcImage);

	CBrush *pBrush =
		CBrush::FromHandle((HBRUSH)GetWindowLong(m_hWnd, GCL_HBRBACKGROUND));
	pDC->FillRect(&rcClient, pBrush);
	pDC->SelectClipRgn(NULL);

	return TRUE;
}

/**********************************************************************/

BOOL CPictureCtrl::LoadImage(LPCTSTR pszPathName) 
{
	Invalidate();
	BOOL bResult = m_pict.Load(pszPathName);

	if ( bResult == TRUE )
	{
		m_strWidth.Format( "%d", m_pict.GetImageSize().cx );
		m_strHeight.Format( "%d", m_pict.GetImageSize().cy );
	}
	else
	{
		m_strWidth = "";
		m_strHeight = "";
	}

	return bResult;
}

/**********************************************************************/
