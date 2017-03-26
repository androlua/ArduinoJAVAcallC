JNI编程读取Arduino的串号数据
JNI编程自行查找
参考文档
http://todbot.com/arduino/host/arduino-serial/arduino-serial.c
https://chrisheydrick.com/2012/06/13/how-to-read-serial-data-from-an-arduino-in-linux-with-c-part-2/
https://salilkapur.wordpress.com/2013/03/08/communicating-with-arduino-using-c/
1 编译带有native声明的.java文件 javac ArduinoSerial.java
2 生成.h头文件 javah ArduinoSerial
3 根据.h头文件编写.c文件完成native方法的C语言实现
4 编译.c文件
 gcc -fPIC -I/usr/local/jdk1.8/include -I/usr/local/jdk1.8/include/linux -shared -o libArduinoSerial.so ArduinoSerial.c
-I后面的参数为jdk路径中的/usr/local/jdk1.8/include和/usr/local/jdk1.8/include/linux
编译时要用到这两个目录中的头文件
5 将生成的so文件复制到
java.library.path==/usr/java/packages/lib/amd64:/usr/lib64:/lib64:/lib:/usr/lib 其中的任意一个
我采用的是
sudo cp libArduinoSerial.so /usr/lib
6 在测试类中先加载本地库System.loadLibrary("ArduinoSerial");
7 调用native方法
说明在进行jni编程中静态方法的 native实现要复杂一些所以本例使用了非静态方法

