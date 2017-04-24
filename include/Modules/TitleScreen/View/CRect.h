#ifndef BF_CRECT_H
#define BF_CRECT_H

namespace RunTheWorld {
	class CRect {
		public:
		
		int mX0;
		int mY0;
		int mX1;
		int mY1;		
		
		explicit CRect();
		explicit CRect(int x0, int y0, int x1, int y1 );
		
		int getDX();
		int getDY();
		bool isInside( int x, int y );
		bool insersects( const CRect& another );	
	};
}
#endif