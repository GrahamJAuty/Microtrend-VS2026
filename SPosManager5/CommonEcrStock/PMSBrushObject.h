#pragma once
/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/

class CPMSBrushObject
{
public:
	CPMSBrushObject( COLORREF r ){ m_Brush.CreateSolidBrush(r); }
	~CPMSBrushObject(void){ m_Brush.DeleteObject(); }

public:
	CBrush* GetBrush(){ return &m_Brush; }
	
private:
	CBrush m_Brush;
};

/**********************************************************************/
#endif
/**********************************************************************/
