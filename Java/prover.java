import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;

/**
 * TODO What does this shit do.
 * 
 * @author Tyler Egan, Ben Hetz
 */
public class prover {

	public static void main(String[] args) {
		
		//Argument Handling
		if(args.length > 2) {
			System.out.println("Usage: prover <filename>");
			System.exit(1);
		} else if(args.length == 0) {
			manualTesting();
			System.exit(0);
		}
		try {
			File filename = new File(args[0]);
			Scanner scan = new Scanner(filename);
			
		} catch (FileNotFoundException e){
			System.out.println("File " + args[0] + " could not be found.");
			System.err.print(e.getStackTrace());
			System.exit(1);
		} catch (Exception e){
			System.err.print(e.getStackTrace());
			System.exit(1);
		}
	}

	/**
	 * TODO Bleh.
	 */
	private static void manualTesting() {
		
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

}
