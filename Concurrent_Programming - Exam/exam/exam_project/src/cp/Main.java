package cp;


import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

/**
 * This class is present only for helping you in testing your software.
 * It will be completely ignored in the evaluation.
 * 
 * @author Fabrizio Montesi <fmontesi@imada.sdu.dk>
 */
public class Main
{
	public static void main( String[] args ) throws IOException, InterruptedException {
		Path path = Paths.get("/Users/Thor/Desktop/cp2017-master/exam/data_example");
		long startTime = System.nanoTime();


		System.out.println(Exam.findAny(path, 100000000, 0));
		//System.out.println(Exam.findAll(path));
		//Exam.stats(path);
		Result rs = Exam.findAny(path, 1000000, 0);
		System.out.println(rs.number());
		System.out.println(rs.path());
		long endTime = System.nanoTime();
		long duration = (endTime - startTime) / 1000000;
		System.out.println("the time it took was :" + duration );

	}
}
