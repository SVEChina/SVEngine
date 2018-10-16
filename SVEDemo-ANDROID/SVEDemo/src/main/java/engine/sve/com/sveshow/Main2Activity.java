package engine.sve.com.sveshow;

import android.Manifest;
import android.content.Intent;
import android.os.Environment;
import android.os.Handler;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

import java.io.File;

public class Main2Activity extends AppCompatActivity {


    private Handler mHandler;
    public void initialize() {
        PermissionUtil.with(Main2Activity.this)
                .listener(new PermissionUtil.RequestPermissionListener() {
                    @Override
                    public void onResult(boolean succ, int requestCode) {
                        if (succ) {
                            FileUtils.deleteAllFiles(new File(Environment.getExternalStorageDirectory() + "sve/res"));
                            FileUtils.getInstance(Main2Activity.this).copyAssetsToSD("sve.bundle", "sve/res").setFileOperateCallback(new FileUtils.FileOperateCallback() {
                                @Override
                                public void onSuccess() {
                                    // TODO: 文件复制成功时，主线程回调
                                }

                                @Override
                                public void onFailed(String error) {
                                    // TODO: 文件复制失败时，主线程回调
                                }
                            });
                        } else {
//                            new AlertDialog.Builder(MainActivity.this)
//                                    .setMessage(R.string.get_permission_faild)
//                                    .setPositiveButton(R.string.yes, null).show();
                        }
                    }
                })
                .request(Manifest.permission.RECORD_AUDIO,
                        Manifest.permission.CAMERA,
                        Manifest.permission.READ_PHONE_STATE,
                        Manifest.permission.WRITE_EXTERNAL_STORAGE);

    }
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main2);
        Button button = findViewById(R.id.svButton);
        initialize();
        FileUtils.deleteAllFiles(new File(Environment.getExternalStorageDirectory() + "sve/res"));
        FileUtils.getInstance(this).copyAssetsToSD("sve.bundle", "sve/res").setFileOperateCallback(new FileUtils.FileOperateCallback() {
            @Override
            public void onSuccess() {
                // TODO: 文件复制成功时，主线程回调
            }

            @Override
            public void onFailed(String error) {
                // TODO: 文件复制失败时，主线程回调
            }
        });
        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                startActivity(new Intent(Main2Activity.this, MainActivity.class));
            }
        });

    }
}
