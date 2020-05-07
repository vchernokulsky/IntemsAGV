package com.intemslab.robotcontroller;

import android.bluetooth.BluetoothGattCharacteristic;
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
import android.view.View;
import android.widget.Button;
import android.widget.SeekBar;
import android.widget.TextView;

import com.intemslab.protocolview.ProtractorView;

import org.json.JSONException;
import org.json.JSONObject;

import java.util.Timer;
import java.util.TimerTask;

public class BleControllerActivity extends AppCompatActivity {
    private final static String TAG = BleControllerActivity.class.getSimpleName();

    TextView txtLinearSpeed;
    TextView txtAngularSpeed;

    Button btnConnect;

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

        btnConnect = findViewById(R.id.btnConnect);

        txtLinearSpeed = findViewById(R.id.textView);
        txtLinearSpeed.setText("" + current);

        seekAngularSpeed = findViewById(R.id.protractor_view);

        txtAngularSpeed = findViewById(R.id.textView2);
        txtAngularSpeed.setText("" + 0);

        // Setup GATT service
        Intent gattServiceIntent = new Intent(this, BluetoothLeService.class);
        bindService(gattServiceIntent, serviceConnection, BIND_AUTO_CREATE);

        // Activity widgets event listeners
        btnConnect.setOnClickListener(new Button.OnClickListener(){
            @Override
            public void onClick(View v) {
                //boolean isConn = bluetoothService.connect("");
            }
        });

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

    private final Object locker = new Object();
    // Private behavior methods
    public void setTwist() {
        //sending message to device
        if(bluetoothService != null) {
            synchronized (locker) {
                int twist = (int)Math.round(twistLinear * 100);
                String chunk0 = "*" + twist + ":" + twist + "#";
                bluetoothService.sendMessage(chunk0);
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
        super.onPause();
        unregisterReceiver(gattUpdateReceiver);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        unbindService(serviceConnection);
        bluetoothService = null;
    }
}
