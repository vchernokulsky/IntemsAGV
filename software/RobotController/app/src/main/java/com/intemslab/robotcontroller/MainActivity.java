package com.intemslab.robotcontroller;

import android.content.Context;
import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

public class MainActivity extends AppCompatActivity {
    private Context context;
    public static final String EXTRA_MESSAGE = "com.intemslab.robotcontroller.MESSAGE";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        context = this;
        Button ctrl_pannel_btn = (Button) findViewById(R.id.control_panel_button);
        ctrl_pannel_btn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(context, DeviceControllerActivity.class);
                EditText editText = (EditText) findViewById(R.id.robot_url);
                String message = editText.getText().toString();
                intent.putExtra(EXTRA_MESSAGE, message);
                startActivity(intent);
            }
        });
        Button ctrl_phone_btn = (Button) findViewById(R.id.smartphone_control_button);
        ctrl_phone_btn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(context, SpeedControlActivity.class);
                EditText editText = (EditText) findViewById(R.id.robot_url);
                String message = editText.getText().toString();
                intent.putExtra(EXTRA_MESSAGE, message);
                startActivity(intent);
            }
        });
        Button custom_message = (Button) findViewById(R.id.custom_message);
        custom_message.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(context, CustomMessage.class);
                EditText editText = (EditText) findViewById(R.id.robot_url);
                String message = editText.getText().toString();
                intent.putExtra(EXTRA_MESSAGE, message);
                startActivity(intent);
            }
        });
        Button ble_button = (Button) findViewById(R.id.ble);
        ble_button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                System.out.println("click");
                Intent intent = new Intent(context, BleControllerActivity.class);
                startActivity(intent);
            }
        });
    }
}
