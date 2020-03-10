
public class Suduko {

  public static void main(String[] args) {

    Field field = new Field();
    field.fromFile(args[0]);
    try {
      if(Suduko.solve(field, 0, 0))
    	  System.out.println("Yay!");
    } catch (SolvedException e) {}
    	System.out.println(field);
  }
  
  
  public static boolean solve(Field f, int i, int j) throws SolvedException {
	 if (i >= Field.SIZE) {
    	return true;
    }
    else{
    	
    	if(f.isEmpty(i,j)){
    		for(int val = 1; val <= 9; val ++){
    			if(f.tryValue(val, i, j)){
        			if(j == Field.SIZE - 1){
        				if(solve(f,i+1,0)) return true;
        			}
        			else{
        				if(solve(f,i,j+1)) return true;
        			}
        		}
        		
        		}
    			f.clear(i, j);
    			return false;
    	}
    	
    	else{
    		if(j == Field.SIZE - 1){
    			return solve(f,i+1,0);
    		}
    		else{
    			return solve(f,i,j+1);
    		}
    	}
    }
  }
  
  
}