package cp;


import java.io.BufferedReader;
import java.io.IOException;
import java.nio.file.*;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.*;
import java.util.function.Function;

/**
 * 
 * @author Fabrizio Montesi <fmontesi@imada.sdu.dk>
 */
public class Exam
{
	/**
	 * This method recursively visits a directory to find all the text files contained in it and its subdirectories.
	 * 
	 * You should consider only files ending with a .txt suffix. You are guaranteed that they will be text files.
	 * 
	 * You can assume that each text file contains a (non-empty) comma-separated sequence of
	 * (positive) numbers. For example: 100,200,34,25
	 * There won't be any new lines, spaces, etc., and the sequence never ends with a comma.
	 * 
	 * The search is recursive: if the directory contains subdirectories,
	 * these are also searched and so on so forth (until there are no more
	 * subdirectories).
	 * 
	 * This method returns a list of results. The list contains a result for each text file that you find.
	 * Each {@link Result} stores the path of its text file, and the highest number (maximum) found inside of the text file.
	 * 
	 * @param dir the directory to search
	 * @return a list of results ({@link Result}), each giving the highest number found in a file
	 */
	public static List< Result > findAll( Path dir ) throws IOException, InterruptedException {
	    int processors = Runtime.getRuntime().availableProcessors();
        ExecutorService executor = Executors.newFixedThreadPool(processors);
        List< Result > resultset = new ArrayList<>();
        List<Future<Result>> set = new ArrayList<>();

        visit(dir, set, executor, Consumer::new);
        for(Future<Result> rs: set){
            try {
                resultset.add(rs.get());
            } catch (ExecutionException e) {
                e.printStackTrace();
            }

        }
        executor.shutdown();


        return resultset;
    }


	/**
	 * Finds a file that contains at most (no more than) n numbers and such that all
	 * numbers in the file are equal or greater than min.
	 * 
	 * This method searches only for one (any) file in the directory
	 * (parameter dir) such that the condition above is respected.
	 * As soon as one such occurrence is found, the search can be
	 * stopped and the method can return immediately.
	 * 
	 * As for method {@code findAll}, the search is recursive.
	 */
	public static Result findAny( Path dir, int n, int min )
	{
        int processors = Runtime.getRuntime().availableProcessors();
        ExecutorService executor = Executors.newFixedThreadPool(processors);
        List<Future<Result>> set = new ArrayList<>();

        Function<Path, Callable<Result>> task = p -> new ConsumerAny(p, n, min);

        visit(dir, set, executor, task);

        try {
            for (Future<Result> rs : set) {
                if (rs.get() != null) {
                    Result result = rs.get();
                    executor.shutdownNow();
                    return result;
                }
            }
        }catch (InterruptedException e) {
            e.printStackTrace();
        } catch (ExecutionException e) {
            e.printStackTrace();
        }


        return null;
	}

	/**
	 * Computes overall statistics about the occurrences of numbers in a directory.
	 * 
	 * This method recursively searches the directory for all numbers in all files and returns
	 * a {@link Stats} object containing the statistics of interest. See the
	 * documentation of {@link Stats}.
	 */
	public static Stats stats( Path dir ) {throw new UnsupportedOperationException();}


    static void visit(Path dir, List<Future<Result>> set , ExecutorService executor, Function<Path, Callable<Result>> task)
    {
        try {
            DirectoryStream< Path > stream = Files.newDirectoryStream( dir );
            try {

                for( Path subPath : stream ) {
                    if ( Files.isDirectory( subPath ) ) {
                        visit( subPath, set, executor, task );
                    } else if ( subPath.toString().endsWith( ".txt" ) ) {
                        set.add(executor.submit(task.apply(subPath)));
                    }
                }
            } finally {
                stream.close();
            }
        } catch( IOException e ) {
            e.printStackTrace();
        }
    }


    public static class Consumer implements Callable<Result> {
        private final Path path;

	    public Consumer(Path path){
            this.path = path;
        }

        public Result call() throws Exception {
            BufferedReader reader = Files.newBufferedReader( path );
            String line;
            int maxVal = 0;
            line = reader.readLine();
            String[] numbers = line.split(",");

            for(String k :  numbers){
                if(Integer.parseInt(k) > maxVal){
                    maxVal = Integer.parseInt(k);
                }
            }
            Result rs = new ResultClass(path, maxVal);
            return rs;
        }
    }

    public static class ConsumerAny implements Callable<Result>{
        private final Path path;
        private final int n;
        private final int min;

	    public ConsumerAny(Path path, int n, int min){
	        this.path = path;
	        this.n = n;
	        this.min = min;
        }



        public Result call() throws Exception {

            BufferedReader reader = Files.newBufferedReader( path );
            String line;
            int maxVal = 0;
            line = reader.readLine();
            String[] numbers = line.split(",");

            if(numbers.length <= n ){
                for(String k :  numbers){
                    if(Integer.parseInt(k) < min || Thread.currentThread().interrupted()){
                        return null;
                    }
                }
            } else return null;
            Result rs = new ResultClass(path, min);
            return rs;
        }
    }


}

