package com.intemslab.robotcontroller;

public class SpeedCalculation {
    private final double MAX_LIN_SPEED = 100.0;
    private final double MIN_LIN_SPEED = -100.0;
    private final double MAX_ANG_SPEED = 100.0;
    private final double MIN_ANG_SPEED = -100.0;

    private final double LIN_STEP = 0.1;
    private final double ANG_STEP = 0.1;

    private final double FORWARD_LIN_THRESHOLD = 0.7;
    private final double BACKWARD_LIN_THRESHOLD = 0.2;
    private final double FORWARD_ANG_THRESHOLD = 0.7;
    private final double BACKWARD_ANG_THRESHOLD = 0.2;

    private double linear;
    private double angular;

    public SpeedCalculation(){
        linear = 0;
        angular = 0;
    }

    public String getLinearString(){
        return String.valueOf(linear);
    }

    public String getAngularString(){
        return String.valueOf(angular);
    }
    public String getSpeedsJson(){
        String res = "{\"lin\":"+ String.valueOf(linear)+",\"ang\":"+ String.valueOf(angular)+"}";
        return res;
    }

    public boolean stop(){
        if (linear == 0 && angular == 0){
            return false;
        }
        linear = 0;
        angular = 0;
        return true;
    }

    public String updateSpeed(String str){
        String[] values = str.split(";");
        boolean updated = false;
        if(values.length == 3){
            try {
                int stop = Integer.parseInt(values[2]);
                if(stop == 1){
                    updated = stop();
                } else {
                    double lin = Double.parseDouble(values[0]);
                    boolean updated_l = updateLinearSpeed(lin);
                    double ang = Double.parseDouble(values[1]);
                    boolean updated_a =updateAngularSpeed(ang);
                    updated = updated_a || updated_l;
                }
            }
            catch (NumberFormatException e)
            {
                e.printStackTrace();
            }
        }
        if (updated){
            String res = "{\"lin\":"+ String.valueOf(linear)+",\"ang\":"+ String.valueOf(angular)+"}";
            return res;
        } else {
            return null;
        }

    }

    public double roundAvoid(double value, int places) {
        double scale = Math.pow(10, places);
        return Math.round(value * scale) / scale;
    }

    public boolean updateLinearSpeed(final double lin) {
        boolean updated = false;
        if (lin > FORWARD_LIN_THRESHOLD && lin < MAX_LIN_SPEED){
            double cur_lin = linear + LIN_STEP;
            if (cur_lin > MAX_LIN_SPEED){
                linear = MAX_LIN_SPEED;
            } else {
                linear = roundAvoid(cur_lin,3);
            }
            updated = true;
        }
        if (lin < BACKWARD_LIN_THRESHOLD && lin > MIN_LIN_SPEED){
            double cur_lin = linear - LIN_STEP;
            if (cur_lin < MIN_LIN_SPEED){
                linear = MIN_LIN_SPEED;
            } else {
                linear = roundAvoid(cur_lin,3);
            }
            updated = true;
        }
        return updated;
    }

    public boolean updateAngularSpeed(final double ang) {
        boolean updated = false;
        if (ang > FORWARD_ANG_THRESHOLD && ang < MAX_ANG_SPEED){
            double cur_ang = angular + ANG_STEP;
            if (cur_ang > MAX_ANG_SPEED){
                angular = MAX_ANG_SPEED;
            } else {
                angular = roundAvoid(cur_ang,3);
            }
            updated = true;
        }
        if (ang < BACKWARD_ANG_THRESHOLD && ang > MIN_ANG_SPEED){
            double cur_ang = angular - ANG_STEP;
            if (cur_ang < MIN_ANG_SPEED){
                angular = MIN_ANG_SPEED;
            } else {
                angular = roundAvoid(cur_ang,3);
            }
            updated = true;
        }
        return updated;
    }
}
