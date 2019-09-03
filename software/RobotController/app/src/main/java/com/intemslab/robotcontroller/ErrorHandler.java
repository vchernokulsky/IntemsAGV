package com.intemslab.robotcontroller;

import android.os.Handler;
import android.os.Message;
import android.widget.TextView;

public class ErrorHandler extends Handler {
    private TextView error;
    ErrorHandler(TextView tw){
        this.error = tw;
    }

    @Override
    public void handleMessage(Message msg) {
        super.handleMessage(msg);
        if(msg.what == 1){
            error.setVisibility(TextView.VISIBLE);
        } else if(msg.what == 0){
            error.setVisibility(TextView.INVISIBLE);
        }
    }
}
