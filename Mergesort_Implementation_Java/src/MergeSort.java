import java.util.ArrayList;
import java.util.Scanner;
import java.util.stream.IntStream;


/**
 * Created by Thor on 3/8/2017.
 */
public class MergeSort extends Merge {

    public void main(String[] args){
        ArrayList<Integer> test = new ArrayList<>();

        int n = 0;
        Scanner sc = new Scanner(System.in);
        while(sc.hasNextInt()){
            int i = sc.nextInt();
            test.add(n, i);
            System.out.print(test.get(n));
            n ++;
        }
        listFiller(test);
        mergeSort(test,1,test.size());
        System.out.println(test.toString());

    }

    public void listFiller(ArrayList<Integer> A){
        System.out.println(A.toString());
        int k = 0;
        for (int i = 0; i <  10; i++) {
            int rand = (int) Math.random();
            A.set(k, rand);
            k ++;
        }

    }

    public void mergeSort(ArrayList<Integer> A, int p, int r){
        if(p < r){
            int q = (p + r)/2;
            mergeSort(A, p, q);
            mergeSort(A, p + 1, r);
            merge(A,p,q,r);
        }
        else{}
    }
}
