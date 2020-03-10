import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;

/* Algoritme og datastruktur projekt del 1
 * gruppe: Thor Skjold Haagensen (Thhaa16) og Mads Kempf (makem16) 
 */

public class Encode {

	public static void main(String[] args) throws Exception {

		File inputfile = new File(args[0]); //place to save the file for updating the inputstream later.

		FileInputStream inFile = new FileInputStream(args[0]); //inputStream for reading from the file, we only need to read bytes.
		FileOutputStream outFile = new FileOutputStream(args[1]); //fileoutputstream to put inside the bitoutpustream.
		BitOutputStream out = new BitOutputStream(outFile); //bitoutputstream from the library given by the teacher 
									//this can write bit by bit to a file.

		int[] inputArray = makeInputArray(inFile); //array of integers to hold the frequences for integers read by the inputstream
								//the function makeInputArray returns a int[].

		PQ pq = heapMake(inputArray); //making a priority queue from the list of frequencies with the smallest frequency first.
		Element hoff = hoffmanMake(pq); // making a hoffman tree, hoffmanMake returns a Element which is the root in the hoffman-tree.
		HashMap<Integer, ArrayList<Integer>> tabel = tabelMake(hoff); //making a table with the hoffman codes for each integer
		hoffFindAndWrite(out, inFile, tabel, inputfile, inputArray); //reads through the input file again and writes to the output file

		inFile.close();
		out.close();

	}// end of main.
	
	/*
	*reads the input File byte by byte, since there are 256 possible combinations of 0 and 1 with the length 8
	*
	*/
	public static int[] makeInputArray(FileInputStream inB) {
		int[] Q = new int[256];
		int c;

		try {

			while ((c = inB.read()) != -1) {
				Q[c]++;
			}

		} catch (IOException e) {
			e.printStackTrace();
		}
		return Q;
	}// end of makeInputArray.

	public static Element hoffmanMake(PQ Q) {

		for (int n = 1; n < 256; n++) {
			Element x = Q.extractMin();
			Element y = Q.extractMin();
			Tree b = new Tree(x.key + y.key);
			b.insertLC(x);
			b.insertRC(y);
			Element q = new Element(b.getSum(), b);
			Q.insert(q);
		}

		return Q.extractMin();
	}// end of hoffmanMake.

	public static PQ heapMake(int[] C) {
		PQ heap = new PQHeap(256);

		for (int k = 0; k < C.length; k++) {
			heap.insert(new Element(C[k], k));
		}
		return heap;
	}// end of heapMake.

	private static void hoffFindAndWrite(BitOutputStream outB, FileInputStream inF, HashMap<Integer, ArrayList<Integer>> tabel, File infile,
			int[] freq) {
		int c;
		try {
			inF = new FileInputStream(infile);
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		}

		try {
			for (int j : freq) {
				outB.writeInt(j);
			}
		} catch (IOException e) {
			e.printStackTrace();
		}
		
		try{
			while((c = inF.read()) != -1){
				ArrayList<Integer> code = tabel.get(c);
				for(int r : code){
					outB.writeBit(r);
				}
			}
		} catch (IOException e) {
			e.printStackTrace();
		}

	}// end of hoffFindAndWrite

	public static HashMap<Integer, ArrayList<Integer>> tabelMake(Element root) {
		HashMap<Integer, ArrayList<Integer>> tabel = new HashMap<Integer, ArrayList<Integer>>();
		
		for (int c = 0; c < 256; c++) {
			boolean isfound = false;
			ArrayList<Integer> outList = new ArrayList<Integer>();
			recurseFind(c, root, isfound, outList);
			ArrayList<Integer> finalstuff = new ArrayList<Integer>();
			for(int i = outList.size()-1; i>=0; i--){
				finalstuff.add(outList.get(i));
			}
			tabel.put(c, finalstuff);
		}
		return tabel;
	}

	public static boolean recurseFind(int k, Element e, boolean bfound, ArrayList<Integer> out) {
		Object o = e.data;
		if (o instanceof Integer) {
			int oi = (int) o;
			if (oi == k) {
				bfound = true;
				return bfound;
			} else {
				return bfound;
			}
		}

		if (o.getClass() == Tree.class) {
			Tree ot = (Tree) o;
			if (ot.getSum() == 0) {
				return bfound;
			} else {
				if (ot.getLC().key != 0) {
					bfound = recurseFind(k, ot.getLC(), bfound, out);
					if (bfound == true) {
						out.add(0);
						return bfound;
					}
				}

				if (ot.getRC().key != 0) {
					bfound = recurseFind(k, ot.getRC(), bfound, out);
					if (bfound == true) {
						out.add(1);
						return bfound;
					}
				}
			}
		}
		return bfound;
	}// end of recurseFind

}// end of Encode.
