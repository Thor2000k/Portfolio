import java.util.ArrayList;

/* Algoritme og datastruktur projekt del 1
 * gruppe: Thor Skjold Haagensen (Thhaa16) og Mads Kempf (makem16) 
 */

public class PQHeap implements PQ {

	private ArrayList<Element> heap;

	public PQHeap(int maxElms) {
		// constructor for the heap used by Heapsort, makes an array with a
		// start capacity defined by the user.
		this.heap = new ArrayList<Element>(maxElms);
	}
	
	private void condSwitch(int i){
		
//		called with an index to switch the element in that index with it's parent if this is needed.
//		calls itself recursively in the event there is more switches to be done.
		
		Element temp;
		
		if(i > 2){
			if(heap.get(i).key < heap.get((i-1)/2).key){
				temp = heap.get((i-1)/2);
				heap.set((i-1)/2, heap.get(i));
				heap.set(i, temp);
				condSwitch((i-1)/2);
			}
		}
		else if(i == 1 || i == 2){
			if(heap.get(i).key < heap.get(0).key){
				temp = heap.get(0);
				heap.set(0, heap.get(i));
				heap.set(i, temp);
			}
		}
		
	}//end of condSwitch

	public Element extractMin() {

		/*
		 * takes the highest prioritized element in the list and returns it to
		 * the user, then removes the element from the list. 
		 * after this the lowest prioritized element in the list is set at the 
		 * front of the array and then minHeapify is called to ensure the array remains in Heap-order.
		 */
		if (heap.size() < 1) {
			System.err.println("Heap empty");
		}

		Element minElm = heap.get(0);
		heap.set(0, heap.get(heap.size() - 1));
		heap.remove(heap.size() - 1);
		minHeapify(heap, 0);
			//calling minHeapify to remedy any complications caused by having the biggest element on the top.

		return minElm;
	}// end of extractMin

	public void minHeapify(ArrayList<Element> A, int i) {
		
		/*
		 * minHeapify tests if a node (or index in our array) and it's children are in heap order.
		 * ##should work on empty arrays :)
		 */

		try {
			int l;
			int r;
			l = i * 2 + 1;
			r = i * 2 + 2;
			//the integers l and r are set to the indexes of the left and right child of the parent element.
			//this is 0 index based.
			int smallest;
			Element temp;
			// smallest and temp are temporary variables used later.

			if (l <= A.size() - 1) {
				if (A.get(l).key < A.get(i).key) {
					smallest = l;
				} else {
					smallest = i;
				}
			} else {
				smallest = i;
			}
			/* 
			 * in the above if statements we first test if the index found as the index of the left child
			 * exists if it doesn't we move on. if the index exists, the key for the element of the 
			 * left child is tested against the key value of the parent to find out if the Heap structure
			 * is kept. If the child key-value is "better" than the parent the child is set to the variable
			 * smallest, else the parent is set to smallest.
			 */
			if (r <= A.size() - 1) {
				if (A.get(r).key < A.get(smallest).key) {
					smallest = r;
				}
			} //same as above but with the right child if it exists.

			if (smallest != i) {
				temp = A.get(i);
				A.set(i, A.get(smallest));
				A.set(smallest, temp);
				minHeapify(A, smallest);
			} //if the heap order is not kept a change happens here and minHeapify is called again.

		} catch (IndexOutOfBoundsException e) {
			System.err.println("A mistake happened");
		}
	}// end of minHeapify

	public void insert(Element e) {
		heap.add(e);
		condSwitch(heap.size()-1);


	}// end of insert

}// end of PQHeap
