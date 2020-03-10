import java.util.ArrayList;

/**
 * Created by Thor on 3/8/2017.
 */
public class Merge {

    public void merge(ArrayList<Integer> A, int p, int q, int r){
        int n1 = q - p + 1;
        int n2 = r - q;
        ArrayList<Integer> L = new ArrayList(n1 + 1);
        ArrayList<Integer> R = new ArrayList(n2 + 1);

        for (int i = 0; i < n1 - 1; i++) {
            L.set(i, A.get(p + i -1));
        }

        for (int j = 0; j < n2 - 1; j++) {
            R.set(j, A.get(q + j));
        }
        L.set(n1 + 1, Integer.MAX_VALUE);
        R.set(n2 + 1, Integer.MAX_VALUE);
        int i = 1;
        int j = 1;

        for (int k = p; k < r; k++) {
            if(L.get(i) <= R.get(j)){
                A.set(k, L.get(i));
                i ++;
            }
            else{
                A.set(k, R.get(j));
                j ++;
            }

        }
    }
}
