package engine.sve.com.sveshow;

import android.content.Context;
import android.graphics.SurfaceTexture;
import android.os.Environment;
import android.view.TextureView;

import com.sve.engine.SVEEngine;

public class SVETextrueView extends TextureView {
    SVEEngine mSVEngine;
    public SVETextrueView(Context context) {
        super(context);
   setSurfaceTextureListener(new SurfaceTextureListener() {
       @Override
       public void onSurfaceTextureAvailable(SurfaceTexture surface, int width, int height) {
           //SVEEngine.getinstance();
           mSVEngine= SVEEngine.getinstance();
           mSVEngine.init( Environment.getExternalStorageDirectory().getPath() + "/show/res/sve.bundle/", 720, 1280,1280,720);
           mSVEngine.resume();
           mSVEngine.openFaceBeauty(2);
           mSVEngine.setSurfaceTextureDraw(surface);
       }

       @Override
       public void onSurfaceTextureSizeChanged(SurfaceTexture surface, int width, int height) {

       }

       @Override
       public boolean onSurfaceTextureDestroyed(SurfaceTexture surface) {
           return false;
       }

       @Override
       public void onSurfaceTextureUpdated(SurfaceTexture surface) {

       }
   });
    }


}
