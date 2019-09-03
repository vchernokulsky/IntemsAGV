package com.intemslab.robotcontroller;

import android.os.Handler;

import java.io.BufferedWriter;
import java.io.DataOutputStream;
import java.io.OutputStreamWriter;
import java.net.HttpURLConnection;
import java.net.URL;


public class PostRequest {

    private String url_string = "http://192.168.55.110:8008";
    private Handler handler;

    public PostRequest(){}
    public PostRequest(String url_string){
        this.url_string = url_string;
    }
    public PostRequest(String url_string,Handler handler){
        this.url_string = url_string;
        this.handler = handler;
    }

        public void sendPost(final String speeds) {
            Thread thread = new Thread(new Runnable() {
                @Override
                public void run() {
                    try {
                            URL url = new URL(url_string);
                            HttpURLConnection conn = (HttpURLConnection) url.openConnection();
                            conn.setRequestMethod("POST");
                            conn.setRequestProperty("Content-Type", "application/json;charset=UTF-8");
                            conn.setRequestProperty("Accept", "application/json");
                            conn.setDoOutput(true);
                            conn.setDoInput(false);

//                            JSONObject jsonParam = new JSONObject();
//                            jsonParam.put("x", "2");
//                            String myString = jsonParam.toString();


                            DataOutputStream out = new DataOutputStream(conn.getOutputStream());
                            BufferedWriter writer = new BufferedWriter(new OutputStreamWriter(out, "UTF-8"));
                            writer.write(speeds);
                            writer.flush();
                            writer.close();
                            out.close();
                            conn.setConnectTimeout(200);
                            conn.setReadTimeout(500);
                            int status = conn.getResponseCode();
                            conn.disconnect();
                            if (handler != null){
                                handler.sendEmptyMessage(0);
                            }
                        } catch (Exception e) {
                            e.printStackTrace();
                        if (handler != null){
                            handler.sendEmptyMessage(1);
                        }
                        }

                }
            });
            thread.start();
        }

    }
