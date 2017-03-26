/**
 * Created by wuhuaiqian on 17-3-26.
 */
public class TestArduino {
    static {
        try {

            System.loadLibrary("ArduinoSerial");
        } catch (Exception e){
            e.printStackTrace();
        }
    }

    public static void main(String[] args) {
	ArduinoSerial a = new ArduinoSerial();
	System.out.println("value in java:\t"+a.readTemperature("/dev/ttyUSB0",9600));
        //System.out.println(new ArduinoSerial().readTemperature("/dev/ttyUSB0",9600));
    }
}
