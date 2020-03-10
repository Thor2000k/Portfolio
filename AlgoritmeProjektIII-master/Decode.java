import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;

public class Decode {

    public static void main(String[] args) {
        BitInputStream bitInStream = null;
        FileInputStream inFile = null;
        FileOutputStream outFile = null;

        try {
            inFile = new FileInputStream(args[0]);
            outFile = new FileOutputStream(args[1]);
            bitInStream = new BitInputStream(inFile);

        } catch (FileNotFoundException e) {}

        // Generating an Array of the 256 first bits im the decoded file reusing the method from Encode.java
        int[] iA = makeInputArray(bitInStream);

        //Generating a Pqheap from the input array.
        PQ pq = Encode.heapMake(iA);

        //Generating the hoffman tree from the priority queue that was made from the imput array,
        // this returns the same hoffman tree that was used to decode the original .txt file
        Element hoff = Encode.hoffmanMake(pq);

        //finally calling the decoder method
        decoder(bitInStream,outFile,iA,hoff);


    }//end of main

    /*
    This method reads the first 256 bits of a decoded file sequencially and uses a BitInputStream to translate given bits.
    Afterwards adding the translated bit to another array.
    */
    private static int[] makeInputArray(BitInputStream b)  {
        int[] Q = new int[256];
        int c;
        try{
        for(int i = 0; i < 256; i++){
            c = b.readInt();
            Q[i] = c;
        }}catch(IOException e){
            e.printStackTrace();
        }

        return Q;
    }// end of makeInputArray.

    /*
     This method reads the rest of the coded file each bit at a time, it uses the dhelper to traverse the previously generated
     hoffman tree.

     */
    public static void decoder(BitInputStream bit, FileOutputStream out, int[] arr, Element root){
        int sum = 0;
        int s = 5 ;
        Tree rootT = null;

        for(int i : arr){
            sum = sum + i;
        }

        while( sum > 0){
            int byteI = dhelper(bit, root, -1);
            if(byteI != -1){
                try {
                    out.write(byteI);
                } catch (IOException e) {
                    e.printStackTrace();
                }
                sum--;
            }
        }
    }

    /*
    This method recursively traverses a hoffmantree. it reads a bit and uses that information to move top down through the tree.
    In the first 2 lines of the method we test whether the nodes.data element is an integer if that is the case we know that we
    reached a leafnote and we return that note.
     */

    public static int dhelper(BitInputStream bit, Element data, int result) {
        if(data.data instanceof Integer){
            return (Integer) data.data;
        }


        int b = 5;
        try {
            b = bit.readBit();
        } catch (IOException e) {
            e.printStackTrace();}
        if(b == 0){
            Tree temp = (Tree) data.data;
            data = temp.getLC();
            result = dhelper(bit, data, result);
        }
        else if(b == 1) {
            Tree temp = (Tree) data.data;
            data = temp.getRC();
                result = dhelper(bit, data, result);
        }

        return result;
    }
}
