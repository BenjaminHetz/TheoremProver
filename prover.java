import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;
import java.util.ArrayList;

/**
 * TODO What does this shit do.
 * 
 * @author Tyler Egan, Ben Hetz
 */
public class prover
{
	//Global variables
	private static double rTime = 0;
	private static double hTime = 0;
	private static double rSteps = 0;
	private static double hSteps = 0;
	
	private static ArrayList<sentence> knowledgeBase;
	
	
	public static void main(String[] args)
	{
		knowledgeBase = new ArrayList<>();
		//Argument Handling
		if(args.length > 2) {
			System.out.println("Usage: prover <filename>");
			System.exit(1);
		} else if(args.length == 0) {
			manualTesting();
			System.exit(0);
		}

		readKB(args[0]);
		resolve();
	}

	/**
	 * TODO Explain heuristicResolve
	 */
	private static void heuristicResolve()
	{
		// 
		double start = System.currentTimeMillis();
		
		double end = System.currentTimeMillis();
		hTime = start-end;
		System.out.println("Heuristic resolve: #steps = " + hSteps + ", time = " 
							+ hTime);
	}
	
	/**
	 * TODO Bleh.
	 */
	private static void manualTesting()
	{
		
//		done = 0;
//	    while(!done) {
//	        do {
//	            system("clear");
//	            printf("\nMain Menu");
//	            printf("\n---------");
//	            printf("\n(A)dd sentence to database");
//	            printf("\n(C)lear database");
//	            printf("\n(L)oad database");
//	            printf("\n(S)how database");
//	            printf("\n(P)rove query");
//	            printf("\n(Q)uit program");
//	            printf("\n\nChoice: ");
//	            fgets(choice,64,stdin);
//	        }
//	        while(choice[0] != 'a' && choice[0] != 'c' && choice[0] != 'l' &&
//	              choice[0] != 'p' && choice[0] != 's' && choice[0] != 'q');
//	        switch(choice[0]) {
//	            case 'a':
//	                AddKBSentence();
//	                break;
//	            case 'c':
//	                InitializeKB();
//	                break;
//	            case 'l':
//	                InitializeKB();
//	                LoadKB();
//	                break;
//	            case 's':
//	            {
//	                char temp[100];
//	                ShowKB();
//	                printf("Press enter to continue... ");
//	                fgets(temp,100,stdin);
//	                break;
//	            }
//	            case 'p':
//	                ProveQuery();
//	                break;
//	            case 'q':
//	                done = 1;
//	                break;
//	        }
//	    }
//	    return 0;
		System.out.println("Manual testing not yet implemented");
	}
	
	/**
	 * TODO Explain randomResolve
	 */
	private static void randomResolve()
	{
		// 
		double start = System.currentTimeMillis();
		
		double end = System.currentTimeMillis();
		rTime = start-end;
		System.out.println("Random resolve: #steps = " + rSteps + ", time = " 
							+ rTime);
	}
	
	/**
     * TODO Implement
     */
    private static void readKB(String fileName)
    {
        try {
            File kbFile = new File(fileName);
            Scanner scan = new Scanner(kbFile);
            while(scan.hasNext()){

            }
        } catch (FileNotFoundException e){
            System.out.println("File " + fileName + " could not be found.");
            System.err.print(e.getStackTrace());
            System.exit(1);
        } catch (Exception e){
            System.err.print(e.getStackTrace());
            System.exit(1);
        }
    }
	
	/**
	 * TODO Explain resolve.
	 */
	private static void resolve()
	{
		randomResolve();
	    heuristicResolve();
	    System.out.println("Heuristic vs Random ratios:  hSteps/rSteps = " 
	    					+ hSteps/rSteps + ", hTime/rTime = " + hTime/rTime);
	}
	
	/**
     * TODO Implement
     */
    private static void showKB()
    {

    }

}
