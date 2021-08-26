/*
float engularSpeed = 0; // текущая угловая скорость
float linearSpeed = 0; // текущая линейная скорость

float requiredEngularSpeed = 0; // требуемая угловая скорость
float requiredLinearSpeed = 0;  // требуемая линейная скорость

float SpeedError; // ошибка линейной скорости 
float lastSpeedError;  // последняя ошибка линейной скорости для диференциальной составляющей
float lastIntegralSpeedError // последняя линейной ошибка скорости для интегралной составляющей
 
float AngSpError;  // ошибка угловой скорости 
float lastAngSpError; // последняя ошибка угловой скорости для диференциальной составляющей
float lastIntegralAngSpError; // последняя ошибка угловой скорости для интегралной составляющей

float sP; //коэфициент P по линейной скорости
float sI; //коэфициент I по линейной скорости
float sD; //коэфициент D по линейной скорости
float sPMax; //ограничение P составляющей по линейной скорости
float sIMax; //ограничение I составляющей по линейной скорости
float sDMax; //ограничение D составляющей по линейной скорости

float aP; //коэфициент P по угловой скорости
float aI; //коэфициент I по угловой скорости
float aD; //коэфициент D по угловой скорости
float aPMax; //ограничение P составляющей по угловой скорости
float aIMax; //ограничение I составляющей по угловой скорости
float aDMax; //ограничение D составляющей по угловой скорости

*/


void PidRegulator(){
  if(millis()-lastime4>20){ // рулим каждые 20 милисекунд
    SpeedError = requiredLinearSpeed - linearSpeed; // расчитываем ошибку по линейной скорости
    float SpeedSignal =  PID(SpeedError,&lastSpeedError,&lastIntegralSpeedError,&lastTimeSpeed,sP,sI,sD,sPMax,sIMax,sDMax,"SP"); // задаём все праметры в регулятор
  
    AngSpError = requiredEngularSpeed - engularSpeed; // расчитываем ошибку по угловой скорости
    float AngSpSignal =  PID(AngSpError,&lastAngSpError,&lastIntegralAngSpError,&lastTimeAngular,aP,aI,aD,aPMax,aIMax,aDMax,"AN"); // задаём все праметры в регулятор
  
    float RE = SpeedSignal+AngSpSignal; //пересчитываем в воздействие на правый двигатель
    float LE = SpeedSignal-AngSpSignal;  //пересчитываем в воздействие на левый двигатель
  
    RE = floatConstrain(RE,-1.0,1.0); // ограничиваем воздействие на правый двигатель
    LE = floatConstrain(LE,-1.0,1.0); // ограничиваем воздействие на левый двигатель
    
    setAmotorturnSpeed(RE); //крутим правый двигатель
    setBmotorturnSpeed(LE); //крутим левый двигатель
    lastime4 = millis(); // фиксируем последнее время
  }
}

float PID(float error, float *lastError, float *lastIntegralError, int *lastTime, float p, float i, float d, float pM, float iM, float dM, String type){
  
  float Dt = ((float)millis() - (float)*lastTime)/1000; //расчитываем время интегрирования
  float Ppart = error * p * Dt;                         //расчитываем пропорциональную часть
  Ppart = floatConstrain(Ppart,-pM,pM);                 //ограничиваем  пропорциональную часть
  
  float lie = *lastIntegralError;    // интегральная ошика               
  *lastIntegralError = lie + error;  // нарашиваем интегральную ошибку
  
  float Ipart = *lastIntegralError * i * Dt; //считаем интегральную часть
  Ipart = floatConstrain(Ipart,-iM,iM);      //ограничиваем интегральную часть
  
  float differentialError  = (error-*lastError)*Dt; // считаем дифференциальную ошибку
  float Dpart = differentialError * d * Dt;         // считаем дифференциальную часть
  Dpart = floatConstrain(Dpart,-dM,dM);             // ограничиваем дифференциальную часть
  
  float pidOut = Ppart + Ipart + Dpart;  // суманое воздействие на котроллер

  *lastError = error; // задание последней ошибки
  *lastTime = millis(); // задание последнего времени для интегрирования
  
  
    //Serial2.print("PID "); Serial2.print(type); Serial2.print(error) ; Serial2.print("1"); Serial2.print(" |");
    //Serial2.print("PID "); Serial2.print(type); Serial2.print(Ppart) ; Serial2.print("2"); Serial2.print(" |");
   // Serial2.print("PID "); Serial2.print(type); Serial2.print(Ipart) ; Serial2.print("3"); Serial2.print(" |");
    //Serial2.print("PID "); Serial2.print(type); Serial2.print(Dpart) ; Serial2.print("4"); Serial2.print(" |");
    
  
  return pidOut;
}

float floatConstrain(float input, float lower, float upper){
  float output = 0;
  if(input < lower){output = lower;}
  if(input > upper ){output = upper;}
  if(lower < input && input < upper){output = input;}
  return output;
}
