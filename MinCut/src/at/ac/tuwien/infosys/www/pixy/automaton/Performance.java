package at.ac.tuwien.infosys.www.pixy.automaton;

import java.util.HashMap;
import java.util.Map.Entry;

/**
 * TODO create naming conventions for timer map
 * @author baki
 *
 */
public class Performance {
	
	private static long startTime, stopTime;
	private static long initialMemory, finalMemory;
	
	private static HashMap<String, Long> timer = new HashMap<String, Long>();
	
	private static final long MEGABYTE = 1024L * 1024L;
	private static final long KILOBYTE = 1024L;
	
	public static long bytesToMegabytes(long bytes) {
		return bytes / MEGABYTE;
	}
	
	public static long bytesToKilobytes(long bytes) {
		return bytes / KILOBYTE;
	}
	
	public static void startMemory() {
		Runtime.getRuntime().gc();
		Runtime.getRuntime().gc();
		initialMemory = Runtime.getRuntime().totalMemory() -
				Runtime.getRuntime().freeMemory();
	}
	
	public static void stopMemory() {
		finalMemory = Runtime.getRuntime().totalMemory() -
				Runtime.getRuntime().freeMemory();	
	}
	
	public static long usedMemory() {
		return finalMemory - initialMemory;
	}

	public static void startTimer() {
		startTime = System.currentTimeMillis();
	}
	
	public static void startTimer( String context) {
		timer.put(context, System.currentTimeMillis());
	}
	
	public static void stopTimer() {
		stopTime = System.currentTimeMillis();
	}
	
	public static void stopTimer(String context) {
		long end_time = System.currentTimeMillis();
		long start_time = timer.get(context);
		timer.put(context, (end_time - start_time) );
	}
	
	public static long elapsedTime() {
		return stopTime - startTime;
	}
	
	public static long elapsedTime(String context) {
		return timer.get(context);
	}
	
	public static void setTime(String context, long time) {
		timer.put(context, time);
	}
	
	public static void printTimers() {
		System.out.println("\t~~~\tTimers\t~~~");
		for (Entry<String, Long> e : timer.entrySet()) {
			System.out.println(String.format("%-20s : %s", e.getKey(), e.getValue()));
		}
	}
	
	public static HashMap<String, Long> getAllTimers() {
		return Performance.timer;
	}
}
