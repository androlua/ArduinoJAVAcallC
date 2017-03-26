/**
 * Created by wuhuaiqian on 17-3-26.
 */
public final  class  ArduinoSerial {
    /**
     *
     * @param serialport 串口路径例如 /dev/ttyUSB0
     * @param baud    波特率 9600
     * @return arduino模拟量AD转换后的值
     * 用非静态方法比静态方法的JNI编程简单
     */
    public  native String readTemperature(String serialport,int baud);
}
