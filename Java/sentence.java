
/**
 * TODO How do we explain this shit.
 * 
 * @author Tyler Egan
 */
public class sentence {
	
	private final int MAXPRED = 50;
	
	private String comment;
	private int refutePart;
	private int[] pred;
	private int[] neg;
	private int numPred;

	public sentence(String input, int refute, int preds) {
		comment = input;
		refutePart = refute;
		pred = new int[MAXPRED];
		neg = new int[MAXPRED];
		numPred = preds;
	}
	
}
