
Serial3.setup(9600);

setTimeout(function(){
  Serial3.write('AT');
//  setTimeout(function(){
//    data = Serial3.read(2);
//    console.log(data);
//  }, 200);
}, 200);

Serial3.on('data', function(ch){
  //cnt++;
  console.log(ch);
});
