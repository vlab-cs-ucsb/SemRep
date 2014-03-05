package Debugger;


import java.io.IOException;
import java.io.Writer;

public class Debugger {
	public static Writer outputFile = null;

	public static void debug(String output, int dlevel, int actualDLevel){
		if (actualDLevel >= dlevel)
			try {
				if (outputFile != null)
					outputFile.write(output);
				System.out.print(output);
			} catch (Exception e) {
				e.printStackTrace();
			}
	}
}
