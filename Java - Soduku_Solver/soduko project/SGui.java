import java.awt.Color;
import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.FileWriter;
import java.io.IOException;

import javax.swing.*;
import javax.swing.table.DefaultTableModel; 

public class SGui extends Suduko implements ActionListener{
	
	public static void main(String[] args){
		SGui sg = new SGui();
	}
	
	String filePath;
	Field currentSudoku;
	JTable jt;
	//handles files from filechooser
	private void handler(String filePath){
		
		Field field = new Field();
	    field.fromFile(filePath);
	    try {
	      if(Suduko.solve(field, 0, 0))
	    	  System.out.println("Yay!");
	    } catch (SolvedException e) {}
	    	System.out.println(field);
	    	currentSudoku = field;
	}
	
	public SGui(){
	
		JFrame window = new JFrame("Soduko solver");
		window.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		window.setVisible(true);
		window.setSize(300, 210);
		window.setLocation(100, 100);
		window.setResizable(false);
		
		JPanel mainPanel = new JPanel();
		
		window.add(mainPanel);
		
		
		// Adding the buttons
		
		JButton buttonNew = new JButton("New Soduko");
		buttonNew.setActionCommand("New file");
		buttonNew.setToolTipText("Choose a soduko file");
		buttonNew.addActionListener(this);
		
		JButton buttonSolve = new JButton("Solve");
		buttonNew.setToolTipText("Solves the selected Soduko");
		buttonSolve.setActionCommand("Solve file");
		buttonSolve.addActionListener(this);
		
		JButton buttonSave = new JButton("Save");
		buttonNew.setToolTipText("Saves the Soduko");
		buttonSave.setActionCommand("Save file");
		buttonSave.addActionListener(this);
		
		mainPanel.add(buttonNew);
		mainPanel.add(buttonSolve);
		mainPanel.add(buttonSave);
		
		// Adding the table
			
		jt = new JTable(9, 9);
		
		jt.setPreferredSize(new Dimension(170, 300));
		jt.setShowGrid(true);
		jt.setGridColor(Color.BLACK);
		mainPanel.add(jt);
		jt.setVisible(true);
		

		}

	

	public void actionPerformed(ActionEvent e) {

		
		switch(e.getActionCommand()) {
		
			case "New file":
			    JFileChooser fc = new JFileChooser();
				int returnValue = fc.showOpenDialog(null);
				
				if(returnValue == JFileChooser.APPROVE_OPTION){
					
					Integer[] cn = new Integer[9];
					for(int i = 0; i < cn.length; i++) cn[i] = i;
					
					filePath = fc.getSelectedFile().getAbsolutePath();
					currentSudoku = new Field();
					currentSudoku.fromFile(filePath);
					DefaultTableModel model = new DefaultTableModel(currentSudoku.model, cn);
					jt.setModel(model);
					jt.repaint();
					
					System.out.println(filePath);
			
				}
				break;
				
			case "Solve file":
				Integer[] cn = new Integer[9];
				for(int i = 0; i < cn.length; i++) cn[i] = i;
				
				handler(filePath);
				
				DefaultTableModel model = new DefaultTableModel(currentSudoku.model, cn);
				jt.setModel(model);
				jt.repaint();
				
				break;
				
			case "Save file":
				JFileChooser fc1 = new JFileChooser();
				int retrival = fc1.showSaveDialog(null);
				if(retrival == JFileChooser.APPROVE_OPTION){
					try {
						FileWriter fw = new FileWriter(fc1.getSelectedFile()+".txt");
						fw.write(currentSudoku.toString());
						fw.close();
					} catch (IOException e1) {
						e1.printStackTrace();
					}
					
				}
				
				break;
		}
	}
	
	
		
	}
	

	