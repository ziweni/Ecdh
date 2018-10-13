package com.shanling.demo;

import android.content.ClipboardManager;
import android.content.Context;
import android.content.DialogInterface;
import android.support.v7.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity {

    static {
        System.loadLibrary("native-lib");
    }

    public native String stringFromJNI();

    AppEcdh Ecc = new AppEcdh();

    private TextView s_sha_key;
    private TextView c_sha_key;
    private TextView Text_Info;

    private EditText s_pub_key;
    private EditText c_pub_key;

    private Button CopyButt;
    private Button OkButt;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // 对组件进行实例化
        Text_Info = (TextView) findViewById(R.id.Text_Info);
        s_sha_key = (TextView) findViewById(R.id.s_sha_key);
        c_sha_key = (TextView) findViewById(R.id.c_sha_key);
        s_pub_key = (EditText) findViewById(R.id.s_pub_key);
        c_pub_key = (EditText) findViewById(R.id.c_pub_key);
        CopyButt = (Button) findViewById(R.id.copy_button);
        OkButt = (Button) findViewById(R.id.ok_button);

        Text_Info.setText(stringFromJNI());

        if(Ecc.GenereateKey() == 1){
            c_pub_key.setText(util.buf_to_string(Ecc.get_c_pub_key()));
            c_sha_key.setText(util.buf_to_string(Ecc.get_g_share_key()));
        };

        CopyButt.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                // 从API11开始android推荐使用android.content.ClipboardManager
                // 为了兼容低版本我们这里使用旧版的android.text.ClipboardManager，虽然提示deprecated，但不影响使用。
                ((ClipboardManager) getSystemService(Context.CLIPBOARD_SERVICE)).setText(c_pub_key.getText());
                Toast.makeText(MainActivity.this, "已复制到剪辑版!", Toast.LENGTH_LONG).show();
            }
        });
    }

    public void OnClick(View v) {
        byte[] _s_Share_key, _s_Pub_key = util.string_to_buf(s_pub_key.getText().toString());
        if(_s_Pub_key.length == 0 || _s_Pub_key.length != 49) {
            new AlertDialog.Builder(this).setTitle("错误：").setMessage("_s_Pub_key 为空或格式不正确！").setPositiveButton("确定", new DialogInterface.OnClickListener() {
                @Override
                public void onClick(DialogInterface dialog, int which) {
                }
            }).show();
            return;
        }
        if((_s_Share_key = Ecc.calShareKeyMd5ByPeerPublicKey(_s_Pub_key)) == null)
        {
            s_sha_key.setText("协商失败！");
        }else{
            s_sha_key.setText(util.buf_to_string(Ecc.get_g_share_key()));
        }
    }
}
