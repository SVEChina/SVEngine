package engine.sve.com.sveshow;

import android.app.Activity;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.Handler;
import android.support.annotation.RequiresApi;
import android.support.v4.content.ContextCompat;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

/**
 * 23以上系统检查权限
 * Created by Supreme on 16/1/12.
 */
public class PermissionUtil {

    public static final int DEFALT_REQUEST_CODE = 0x78;

    private static final int MIN_POST_DELAYED_TIME = 250;  //最短0.25秒一次查询权限
    private static final int MAX_POST_DELAYED_TIME = 5000;  //最长7.5秒一次查询权限
    private static final int MAX_POST_DELAYED_NUM = 100;  //最长查询次数

    private Activity mAct;
    private Handler mHandler;

    private int mRequestCode = DEFALT_REQUEST_CODE;

    /** 需要请求的权限 */
    private List<String> mPermissions = new ArrayList<>();

    /** 是否已经执行过请求 */
    private boolean isRequested = false;

    private int mPostDelayedTime = MIN_POST_DELAYED_TIME;
    private int mPostDelayedNum = 1;

    private RequestPermissionListener mRequestPermissionListener;

    private PermissionUtil(Activity act){
        mAct = act;
        mHandler = new Handler();
    }

    public static PermissionUtil with(Activity act){
        return new PermissionUtil(act);
    }

    public PermissionUtil requestCode(int requestCode){
        mRequestCode = requestCode;
        return this;
    }

    /**
     * 检查需要权限
     * @param permission 权限 {@link android.Manifest.permission}
     * @deprecated {@link PermissionUtil#request(String...)}
     */
    public void request(String permission, int requestCode){
        requestCode(requestCode);
        request(permission);
    }

    /**
     * 检查需要权限
     * @param permission 权限 {@link android.Manifest.permission}
     */
    public void request(String... permission){
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            mPermissions.addAll(Arrays.asList(permission));

            isRequested = false;
            mPostDelayedTime = MIN_POST_DELAYED_TIME;
            mPostDelayedNum = 1;
            mHandler.post(checkRunnable);
        }else {
            doResult(true, mRequestCode);
        }
    }

    /**
     * 检查权限返回
     * @param b false 需要申请且申请失败 true 通过申请
     * @param requestCode 请求编号
     */
    private void doResult(boolean b, int requestCode){
        if(mRequestPermissionListener != null){
            mRequestPermissionListener.onResult(b, requestCode);
        }
    }

    private Runnable checkRunnable = new Runnable() {
        @RequiresApi(api = Build.VERSION_CODES.M)
        @Override
        public void run() {
            if(mPermissions != null && mPermissions.size() > 0){  //筛选已经获取到的权限
                List<String> removeList = new ArrayList<>();
                for (String per : mPermissions) {
                    if (PackageManager.PERMISSION_GRANTED == ContextCompat.checkSelfPermission(mAct, per)) {  //已经请求过该权限
                        removeList.add(per);
                    }
                }
                mPermissions.removeAll(removeList);
            }

            if(mPermissions == null || mPermissions.size() == 0){
                doResult(true, mRequestCode);
            } else {
                if(!isRequested) {
                    isRequested = true;

                    //进行权限请求
                    mAct.requestPermissions(mPermissions.toArray(new String[mPermissions.size()]), mRequestCode);
                }

                if(mPostDelayedNum < MAX_POST_DELAYED_NUM) {  //如果超过150秒没响应就不要继续等了
                    mPostDelayedNum++;

                    mHandler.postDelayed(checkRunnable, mPostDelayedTime);

                    if(mPostDelayedTime < MAX_POST_DELAYED_TIME) {
                        mPostDelayedTime += MIN_POST_DELAYED_TIME;
                    }
                }
            }
        }
    };

    /**
     * 设置监听
     * @param l
     */
    public PermissionUtil listener(RequestPermissionListener l){
        mRequestPermissionListener = l;
        return this;
    }

    /**
     * 请求权限监听
     */
    public interface RequestPermissionListener{

        /**
         * 检查权限返回
         * @param succ false 需要申请且申请失败 true 通过申请
         * @param requestCode 请求编号
         */
        void onResult(boolean succ, int requestCode);
    }
}
