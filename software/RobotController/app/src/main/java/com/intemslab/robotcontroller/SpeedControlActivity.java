package com.intemslab.robotcontroller;

import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.text.Editable;
import android.text.TextWatcher;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

public class SpeedControlActivity extends AppCompatActivity {

    private SpeedCalculation sc;
    private PostRequest pr;
    private TextView lin_text;
    private TextView ang_text;
    private TextView json;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_speed_control);
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        getSupportActionBar().setTitle("Пульт управления");
        getSupportActionBar().setDisplayHomeAsUpEnabled(true);
//        getSupportActionBar().setHomeButtonEnabled(true);
        toolbar.setNavigationOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v){
                onBackPressed();// возврат на предыдущий activity
            }
        });
        Intent intent = getIntent();
        String message = intent.getStringExtra(MainActivity.EXTRA_MESSAGE);

        lin_text = (TextView) findViewById(R.id.lin_speed);
        ang_text = (TextView) findViewById(R.id.ang_speed);
        json = (TextView) findViewById(R.id.json);

        sc = new SpeedCalculation();
        Handler h = new ErrorHandler((TextView) findViewById(R.id.error));
        pr = new PostRequest(message, h);



        Button lin_up = (Button) findViewById(R.id.lin_up);
        lin_up.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                change_linear_speed(1.0);
            }
        });

        Button lin_down = (Button) findViewById(R.id.lin_down);
        lin_down.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                change_linear_speed(0.0);
            }
        });

        Button ang_up = (Button) findViewById(R.id.ang_up);
        ang_up.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                change_angular_speed(1.0);
            }
        });

        Button ang_down = (Button) findViewById(R.id.ang_down);
        ang_down.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                change_angular_speed(0.0);
            }
        });

        Button stop = (Button) findViewById(R.id.stop);
        stop.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                change_to_stop();
            }
        });



    }

    private void change_linear_speed(double lin){
        boolean changed = sc.updateLinearSpeed(lin);
        if (changed){
            lin_text.setText(sc.getLinearString());
            String speeds_json = sc.getSpeedsJson();
            json.setText(speeds_json);
            pr.sendPost(speeds_json);
        }
    }

    private void change_angular_speed(double ang){
        boolean changed = sc.updateAngularSpeed(ang);
        if (changed){
            ang_text.setText(sc.getAngularString());
            String speeds_json = sc.getSpeedsJson();
            json.setText(speeds_json);
            pr.sendPost(speeds_json);
        }
    }

    private void change_to_stop(){
        boolean changed = sc.stop();
        if (changed){
            lin_text.setText(sc.getLinearString());
            ang_text.setText(sc.getAngularString());
            String speeds_json = sc.getSpeedsJson();
            json.setText(speeds_json);
            pr.sendPost(speeds_json);
        }
    }



}
