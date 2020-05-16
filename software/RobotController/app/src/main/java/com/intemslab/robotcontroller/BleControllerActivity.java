package com.intemslab.robotcontroller;

import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.IBinder;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.util.Pair;
import android.view.View;
import android.widget.SeekBar;
import android.widget.TextView;

import com.intemslab.protocolview.ProtractorView;

import java.util.Date;
import java.util.Timer;
import java.util.TimerTask;

public class BleControllerActivity extends AppCompatActivity {
    private final static String TAG = BleControllerActivity.class.getSimpleName();

    private final Object locker = new Object();

    TextView txtLinearSpeed;
    TextView txtAngularSpeed;

    SeekBar seekLinearSpeed;
    ProtractorView seekAngularSpeed;

    BluetoothLeService bluetoothService;

    // GUI behavior variables
    private volatile boolean publishLinVelocity;
    private volatile boolean publishAngVelocity;
    private volatile boolean isConnected;


    // data model variables
    double twistLinear;
    double twistAngular;
    int min = 0, max = 100, current = 0;

    private final ServiceConnection serviceConnection = new ServiceConnection() {
        @Override
        public void onServiceConnected(ComponentName componentName, IBinder service) {
            BluetoothLeService.LocalBinder aService = (BluetoothLeService.LocalBinder) service;
            bluetoothService = aService.getService();
            if (!bluetoothService.initialize()) {
                Log.e(TAG, "Unable to initialize Bluetooth");
                finish();
            }
            // Automatically connects to the device upon successful start-up initialization.
            bluetoothService.connect(BluetoothConstants.DEVICE_ADDRESS);
        }

        @Override
        public void onServiceDisconnected(ComponentName componentName) {
            bluetoothService = null;
        }
    };

    private final BroadcastReceiver gattUpdateReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            final String action = intent.getAction();
            if (BluetoothLeService.ACTION_GATT_CONNECTED.equals(action)) {
                isConnected = true;
                //updateConnectionState(R.string.connected);
                invalidateOptionsMenu();
            } else if (BluetoothLeService.ACTION_GATT_DISCONNECTED.equals(action)) {
                isConnected = false;
                //updateConnectionState(R.string.disconnected);
                invalidateOptionsMenu();
                //clearUI();
            } else if (BluetoothLeService.ACTION_GATT_SERVICES_DISCOVERED.equals(action)) {
                // Show all the supported services and characteristics on the user interface.
                //displayGattServices(mBluetoothLeService.getSupportedGattServices());
            } else if (BluetoothLeService.ACTION_DATA_AVAILABLE.equals(action)) {
                //displayData(intent.getStringExtra(BluetoothLeService.EXTRA_DATA));
            }
        }
    };

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_ble_controller);
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        getSupportActionBar().setTitle("BLE robot control");
        getSupportActionBar().setDisplayHomeAsUpEnabled(true);

        toolbar.setNavigationOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                onBackPressed();// возврат на предыдущий activity
            }
        });

        seekLinearSpeed = findViewById(R.id.seekBar);
        seekLinearSpeed.setProgress(max - min);
        seekLinearSpeed.setProgress(current - min);

        txtLinearSpeed = findViewById(R.id.textView);
        txtLinearSpeed.setText("" + current);

        seekAngularSpeed = findViewById(R.id.protractor_view);

        txtAngularSpeed = findViewById(R.id.textView2);
        txtAngularSpeed.setText("" + 0);

        // Setup GATT service
        Intent gattServiceIntent = new Intent(this, BluetoothLeService.class);
        bindService(gattServiceIntent, serviceConnection, BIND_AUTO_CREATE);

        seekLinearSpeed.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                twistLinear = (double) progress / 100.0;
                txtLinearSpeed.setText("" + progress);
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
                publishLinVelocity = true;
            }

            @Override
            public void onStopTrackingTouch(SeekBar sBar) {
                seekLinearSpeed.setProgress(0);
                twistLinear = 0;
                setTwist();
                publishLinVelocity = false;
            }
        });

        seekAngularSpeed.setOnProtractorViewChangeListener(new ProtractorView.OnProtractorViewChangeListener() {
            @Override
            public void onProgressChanged(ProtractorView protractorView, int progress, boolean fromUser) {
                twistAngular = (double) (progress - 90) / 90.0;
                txtAngularSpeed.setText("" + Math.round((progress - 90) / 0.9));
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

//    void adjust_speeds()
//    {
//        if ((left_linear > max_lin_speed)or(right_linear > max_lin_speed)){
//        float factor = max_lin_speed / max(left_linear, right_linear);
//        left_linear *= factor;
//        right_linear *= factor;
//    }
//        if ((left_linear < -max_lin_speed)or(right_linear < -max_lin_speed)){
//        float factor = max_lin_speed / min(left_linear, right_linear);
//        left_linear *= factor;
//        right_linear *= factor;
//    }
//    }
//    void calculate_speeds(){
//        left_linear = (2 * linear + wheel_separation * angular) / 2;
//        right_linear = (2 * linear - wheel_separation * angular) / 2;
//        adjust_speeds();
//    }


    // Private behavior methods
    private Pair<Integer, Integer> calculateWheelSpeed(int linear, int angular) {
        double leftWheel  = (2*linear + 0.45*angular)/2;
        double rightWheel = (2*linear - 0.45*angular)/2;
        //adjust wheel speed
        if((leftWheel > 70)||(rightWheel>70)) {
            double factor = 70/Math.max(leftWheel, rightWheel);
            leftWheel *= factor;
            rightWheel *= factor;
        }
        if((leftWheel < -70)||(rightWheel  < -70)) {
            double factor = 70/Math.min(leftWheel, rightWheel);
            leftWheel *= factor;
            rightWheel *= factor;
        }
        int iLeftWheel = 0, iRightWheel = 0;
        if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.N) {
            iLeftWheel = Math.toIntExact(Math.round(leftWheel));
            iRightWheel = Math.toIntExact(Math.round(rightWheel));
        }
        return new Pair<>(iLeftWheel, iRightWheel);
    }

    private java.util.Date lastTime = new Date();
    private int lastTwist;
    private int lastLeftSpeed, lastRightSpeed;
    public void setTwist() {
        //sending message to device
        if(bluetoothService != null) {
            synchronized (locker) {
                long delta = (new Date()).getTime() - lastTime.getTime();

                int linear = (int)Math.round(twistLinear * 100);
                int angular = (int)Math.round(twistAngular * 100);
                Pair<Integer, Integer> speedPair = calculateWheelSpeed(linear, angular);

                if((lastLeftSpeed != speedPair.first)
                        || (lastRightSpeed != speedPair.second) || (delta >= 500)) {
                    String msg = "*" + speedPair.first + ":" + speedPair.second + "#";
                    bluetoothService.sendMessage(msg);
                    lastLeftSpeed = speedPair.first;
                    lastRightSpeed = speedPair.second;
                    lastTime = new Date();
                }
            }
        }
        System.out.println("Linear: " + twistLinear + "; Angular: " + twistAngular);
    }

    public void startPub() {
        Timer publisherTimer = new Timer();
        publisherTimer.schedule(new TimerTask() {
            @Override
            public void run() {
                if (publishLinVelocity || publishAngVelocity) {
                    setTwist();
                }
            }
        }, 0, 80);
    }

    // Base class overrides
    @Override
    protected void onPause() {
        //TODO: fix work with bluetooth env
        super.onPause();
        if(bluetoothService != null) {
            bluetoothService.disconnect();
        }
        //unregisterReceiver(gattUpdateReceiver);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if(bluetoothService != null) {
            //TODO: fix work with bluetooth env
            bluetoothService.disconnect();
            bluetoothService = null;
            //unbindService(serviceConnection);
        }
    }
}
