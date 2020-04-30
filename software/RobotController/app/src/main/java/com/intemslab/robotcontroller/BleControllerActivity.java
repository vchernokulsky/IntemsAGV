package com.intemslab.robotcontroller;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.View;
import android.widget.SeekBar;
import android.widget.TextView;

import com.intemslab.protocolview.ProtractorView;

import java.util.Timer;
import java.util.TimerTask;

public class BleControllerActivity extends AppCompatActivity {

    private volatile boolean publishLinVelocity;
    private volatile boolean publishAngVelocity;

    SeekBar seekBar;
    TextView textView;

    ProtractorView protractorView;
    TextView textView2;

    double twistLinear;
    double twistAngular;

    int min = 0, max = 100, current = 0;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_ble_controller);
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        getSupportActionBar().setTitle("BLE Controller");
        getSupportActionBar().setDisplayHomeAsUpEnabled(true);
//        getSupportActionBar().setHomeButtonEnabled(true);
        toolbar.setNavigationOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                onBackPressed();// возврат на предыдущий activity
            }
        });

        textView = findViewById(R.id.textView);
        seekBar = findViewById(R.id.seekBar);
        seekBar.setProgress(max - min);
        seekBar.setProgress(current - min);
        textView.setText("" + current);


        textView2 = findViewById(R.id.textView2);
        textView2.setText("" + 0);
        protractorView = findViewById(R.id.protractor_view);

        seekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                twistLinear = (double) progress / 100.0;
                textView.setText("" + progress);
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
                publishLinVelocity = true;
            }

            @Override
            public void onStopTrackingTouch(SeekBar sBar) {
                seekBar.setProgress(0);
                twistLinear = 0;
                setTwist();
                publishLinVelocity = false;
            }
        });

        protractorView.setOnProtractorViewChangeListener(new ProtractorView.OnProtractorViewChangeListener() {
            @Override
            public void onProgressChanged(ProtractorView protractorView, int progress, boolean fromUser) {
                twistAngular = (double) (progress - 90) / 90.0;
                textView2.setText("" + Math.round((progress - 90) / 0.9));
            }

            @Override
            public void onStartTrackingTouch(ProtractorView protractorView) {
                publishAngVelocity = true;
            }

            @Override
            public void onStopTrackingTouch(ProtractorView protractorView) {
                protractorView.setAngle(90);
                twistAngular = 0;
                setTwist();
                publishAngVelocity = false;
            }
        });
        startPub();
    }

    public void setTwist() {
        //sending message to device
        System.out.println("Linear: " + twistLinear + "; Angular: " + twistAngular);

    }

    public void startPub() {

        Timer publisherTimer = new Timer();
        publisherTimer.schedule(new TimerTask() {
            @Override
            public void run() {
                if (publishLinVelocity || publishAngVelocity) {
                    setTwist(  );
                }
            }
        }, 0, 80);
    }


}
