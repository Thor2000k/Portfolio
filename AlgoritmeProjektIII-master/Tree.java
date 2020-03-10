
public class Tree {
	
	private Element leftChild;
	private Element rightChild;
	private int Sum;
	

	public Tree(int k) {
		this.Sum = k;
		this.leftChild = null;
		this.rightChild = null;
	}

	public void insertLC(Element L) {
		this.leftChild = L;
	}

	public void insertRC(Element R) {
		this.rightChild = R;
	}

	public Element getLC() {
		return leftChild;
	}

	public Element getRC() {
		return rightChild;
	}

	public int getSum() {
		return Sum;
	}

}

