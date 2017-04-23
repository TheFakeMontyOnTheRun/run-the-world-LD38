#ifndef IRENDERER_H
#define IRENDERER_H
namespace odb {
    class CRenderer {
    private:
        CControlCallback mOnKeyPressedCallback;
        CControlCallback mOnKeyReleasedCallback;
     public:
        CRenderer( CControlCallback keyPressedCallback, CControlCallback keyReleasedCallback );
        void render( const CGame& game, long ms );
        void handleSystemEvents();
        void sleep( long ms );

        void fill(float x0, float x1, float y0, float x2, float x3, float y1, int count);
    };
}
#endif
