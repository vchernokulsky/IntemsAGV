What do you need to run this node?

To run this node you need to have file with Haar cascade. For example, "cascade.xml".

How to create your own cascade?

The structure of working directory must be as below:
```
    neg
        <name of the image 1>.jpg
        ...
        <name of the image N>.jpg
    bg.txt
    <image-pattern which we are going to find in app>.jpg
```

The file bg.txt must contain paths to images. For example:
```
    neg/133.screen-3.jpg
    neg/80.maxresdefault.jpg
    neg/2.maxresdefault.jpg
    neg/174.o56465.jpg
    neg/132.PM-Feature_Ford_Transit_File_Mile.jpg
    neg/165.aid296361-v4-1200px-Do-Anything-Step-27.jpg
    neg/95.large-beautiful-drops-transparent-rain-260nw-668593321.jpg
    neg/240.Good-Night-Images-for-Friends.jpg
    neg/141.hartalika-teej_1.jpg
    neg/136.141213_web.jpg
    neg/194.SAINt-JHN-%E2%80%93-Anything-Can-Happen-ft.-Meek-Mill.jpg
    neg/251.google-image-wallpaper-23.jpg
    neg/10.blackholewithclouds_c-1-941x519.jpg
```

To train your own Haar cascade you need to use two utilities from OpenCV:
opencv_createsample and opencv_traincascade. "neg"-directory must store
images that doesn't contain pattern. opencv_createsample takes images
and put the pattern-image on them, so "info"-directory with positive images
appears. opencv_tranecascade creates cascade using images from "data" and
"neg" directories. Remember, if you pass not enough images for required accuracy,
opencv_traincascade will stop with segfault and won't create cascade.

The example of running opencv_createsample:
```
    $opencv_createsamples -img pattern.jpg -bg bg.txt -info info/info.lst -pngoutput info -maxxangle 0.5 -maxyangle 0.5 -maxzangle 0.5 -num 600 -w 50 -h 50
    $opencv_createsamples -info info/info.lst -num 600 -w 50 -h 50 -vec positives.vec
```
The example of running opencv_traincascade:
```
    $opencv_traincascade -data data -vec positives.vec -bg bg.txt -numPos 600 -numNeg 300 -numStages 3 -w 50 -h 50
```