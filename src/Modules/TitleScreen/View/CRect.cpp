#include "Modules/TitleScreen/View/CRect.h"

namespace RunTheWorld {
	CRect::CRect(): mX0( 0 ), mX1( 0 ), mY0( 0 ), mY1( 0 ) {
	}
	
	CRect::CRect(int x0, int y0, int x1, int y1 ): mX0( x0 ), mX1( x1 ), mY0( y0 ), mY1( y1 ) {
	}
			
	int CRect::getDX() {
		return mX1 - mX0;
	}
	
	int CRect::getDY() {
		return mY1 - mY0;
	}
	
	bool CRect::isInside( int x, int y ) {
		return (mX0 <= x && x <= mX1 ) && ( mY0 <= y && y <= mY1 );
	}
}