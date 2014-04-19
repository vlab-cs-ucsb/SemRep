package at.ac.tuwien.infosys.www.pixy.automaton;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.FilenameFilter;
import java.io.IOException;
import java.io.Writer;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.Map;
import java.util.Queue;
import java.util.Set;
import java.util.StringTokenizer;
import java.util.regex.Pattern;

import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.CommandLineParser;
import org.apache.commons.cli.HelpFormatter;
import org.apache.commons.cli.Options;
import org.apache.commons.cli.PosixParser;

import Debugger.Debugger;

import dotparser.*;

//import edu.ucsb.cs.www.vlab.stranger.PerfInfo;
//import edu.ucsb.cs.www.vlab.stranger.StrangerAutomaton;

//import edu.ucsb.cs.www.vlab.stranger.StrangerAutomaton.Transition;

public class BuildAutomaton {
	static int id = 0;

	static int debugLevel = 0;

	public static int numOfTracks = 1;
	public static int charLength = 8; // 8 bits for each character

	public class Visitor implements DOTParserVisitor {
		int id;
		Map<String, State> string2State;
		Set<String> acceptStates;
		/**
		 * accept is turned on when we parse the dot grammar non-terminal a_list
		 * in a dot grammar non-terminal node_stmt that has "shape=doublecircle"
		 * and turned off when parsing any other shape value. This allows us to
		 * know that the next states to parse are all accept states.
		 */
		boolean accept = false;

		public Visitor(int myId) {
			this.id = myId;
			string2State = new HashMap<String, State>();
			acceptStates = new HashSet<String>();
		}

		/**
		 * converts a MONA binary (ASCII) character with 0s, 1s, and Xs to a
		 * ascendingly sorted list of actual ASCII characters
		 * 
		 * @param MONAChar
		 *            an array of eight 0's, 1's and/or Xs that represents a set
		 *            of ASCII characters i.e. [0, 1, 0, 1, 0, 1, 0, X] = {U,V}
		 * @param index
		 *            : as this is a recursive call, index tells us which
		 *            position in current input MONAchar to process (first call
		 *            use 0)
		 * @return an array of ASCII characters sorted according to ASCII
		 *         sorting. Example param would give [U, V]
		 */
		public char[] MONAMultiTrackCharToCharSet(char[] MONAChar, int index) {
			char[] result = null;
			// Base case (reading last character in oneChar which is the least
			// significant bit)
			if (index == 7) {
				if (MONAChar[index] == '0' || MONAChar[index] == '1') {
					result = new char[1];
					if (MONAChar[index] == '0')
						result[0] = 0;
					else
						result[0] = 1;
				} else {// oneChar[index] == 'x'
					result = new char[2];
					result[0] = 0;
					result[1] = 1;
				}
			} else {
				// get all possible char values from suffix subarray then add
				// current values to it
				char[] prevResult = MONAMultiTrackCharToCharSet(MONAChar,
						(index + 1));
				if (MONAChar[index] == '0' || MONAChar[index] == '1') {
					result = prevResult;
					for (int i = 0; i < prevResult.length; i++)
						result[i] = (char) (result[i] + (((int) MONAChar[index]) - 48)
								* Math.pow(2, (7 - index)));
				} else {// oneChar[index] == 'x'
					result = new char[prevResult.length * 2];
					// first multiply by 0
					for (int i = 0; i < prevResult.length; i++)
						result[i] = prevResult[i];
					// then multiply by 1
					for (int i = 0; i < prevResult.length; i++)
						result[i + prevResult.length] = (char) (prevResult[i] + Math
								.pow(2, (7 - index)));
				}
			}
			return result;
		}

		public char[] MONAMultiTrackCharToCharSet2(char[] MONAChar, int index) {
			char[] result = null;
			// Base case (reading last character in oneChar which is the least
			// significant bit)
			if (index == 15) {
				if (MONAChar[index] == '0' || MONAChar[index] == '1') {
					result = new char[1];
					if (MONAChar[index] == '0')
						result[0] = 0;
					else
						result[0] = 1;
				} else {// oneChar[index] == 'x'
					result = new char[2];
					result[0] = 0;
					result[1] = 1;
				}
			} else {
				// get all possible char values from suffix subarray then add
				// current values to it
				char[] prevResult = MONAMultiTrackCharToCharSet2(MONAChar,
						(index + 1));
				if (MONAChar[index] == '0' || MONAChar[index] == '1') {
					result = prevResult;
					for (int i = 0; i < prevResult.length; i++)
						result[i] = (char) (result[i] + (((int) MONAChar[index]) - 48)
								* Math.pow(2, (15 - index)));
				} else {// oneChar[index] == 'x'
					result = new char[prevResult.length * 2];
					// first multiply by 0
					for (int i = 0; i < prevResult.length; i++)
						result[i] = prevResult[i];
					// then multiply by 1
					for (int i = 0; i < prevResult.length; i++)
						result[i + prevResult.length] = (char) (prevResult[i] + Math
								.pow(2, (15 - index)));
				}
			}
			return result;
		}

		/**
		 * Parses a MONA automaton transition label and returns a list of
		 * automaton transitions corresponding to every range of chars in this
		 * MONA transition label Note that if a transition is labeled with
		 * NON-ASCII chars or ASCII chars that are non-printable then it will
		 * not be added.
		 * 
		 * @param MONA
		 *            transition label that is a matrix or 0s, 1s, and/or Xs
		 * @param the
		 *            automaton state that represents the destination of these
		 *            transitions
		 * @return a list of Transitions (could be empty)
		 */
		public ArrayList<Transition> parseMonaMultiTrackEdge(String edgeLabel,
				State to) {

			int numOfLines = charLength * numOfTracks; // number of lines in a
														// character matrix
			edgeLabel = edgeLabel.substring(1);// remove first " in MONA label
			edgeLabel = edgeLabel.substring(0, edgeLabel.length() - 1);// remove
			// last
			// " in
			// MONA
			// label
			StringTokenizer edgeTokenizer = new StringTokenizer(edgeLabel,
					"\\n");
			// each row in MONALabel is a multi track character (8bits, 16bits,
			// ...etc). Number of rows (height) equals width of label
			char[][] MONALabel = null;
			// for every line in
			// label matrix (num of lines is length of char by num of tracks)
			// fill in the MONAlabel (width and height are for MONALabel)
			for (int width = 0; width < numOfLines; width++) {
				String nextToken = edgeTokenizer.nextToken();
				StringTokenizer lineTokenizer = new StringTokenizer(nextToken,
						" ,");
				if (MONALabel == null)
					MONALabel = new char[lineTokenizer.countTokens()][numOfLines];
				int height = 0;// height of label matrix
				while (lineTokenizer.hasMoreTokens()) {
					String character = lineTokenizer.nextToken();
					MONALabel[height++][width] = character.charAt(0);
				}
			}
			/** debugging **/
			// for (int i = 0; i < MONALabel.length; i++){
			// System.out.print("[" + MONALabel[i][0]);
			// for (int j = 1; j < MONALabel[i].length; j++)
			// System.out.print(", " + MONALabel[i][j]);
			// System.out.println("]\n");
			// }
			// System.out.println("\n");
			/** end debugging **/
			if (edgeTokenizer.hasMoreTokens())
				throw new RuntimeException(
						"There are more than"
								+ numOfLines
								+ " bit in a MONA ASCII char. You need to change character length of numoftracks.");

			// Convert every MONA binary character into a set of actual
			// characters

			// tracks will hold an array for each track "trackSet". this array
			// contains the characters for this track on current edge
			boolean tracks[][] = new boolean[numOfTracks][256];

			for (int t = 0; t < numOfTracks; t++) {
				// every row in MONALabel is a char
				for (int i = 0; i < MONALabel.length; i++) {
					// for each column in mona label matrix (row in MONALabel)
					// break it into multiple tracks (2 or more) then for each
					// character in a track convert into a set of characters
					/** add a track */
					// convert a mona char of one track (8 bits) with 0s, 1s,
					// and/or Xs to a set of characters
					char[] charSet = MONAMultiTrackCharToCharSet(MONALabel[i],
							0);

					/** for debugging **/
					// System.out.print("track"+(t+1)+": ");
					// for (int k = 0; k < charSet.length; k++){
					// System.out.print(Transition.returnCharString(charSet[k])
					// + " ");
					// }
					// System.out.println();
					/** end for debugging **/
					// ignore lambda
					// if (charSet.length == 1 && ((int)charSet[0]) == 255){
					// System.out.print("\n --> lambda ignored");
					// }
					// else
					{
						// ArrayList<Character> charArray = new
						// ArrayList<Character>();
						for (int j = 0; j < charSet.length; j++)
							// only add printable characters
							// if (charSet[j] > 31 && charSet[j] < 127)
							tracks[t][(int) charSet[j]] = true;
						// add the previous set of characters to the whole set
						// of
						// characters
						// trackSet.addAll(charArray);
					}
					// System.out.println();
					// if we read last track no need to shift left
					if ((t + 1) < numOfTracks)
						shiftLeft(MONALabel[i]);

					// /** add second track */
					//
					// // convert a char with 0s, 1s, and/or Xs to a set of
					// characters
					// char[] charSet2 =
					// MONAMultiTrackCharToCharSet(MONALabel[i], 0);
					// System.out.print("track2: ");
					// for (int k = 0; k < charSet2.length; k++){
					// //charSet2[k] = (char)(Math.pow(2, 8) +
					// ((int)charSet2[k]));
					// System.out.print(Transition.returnCharString(charSet2[k])
					// + " ");
					// }
					// // if it is lambda in this track and the corresponding
					// character in the first track is not lambda then ignore
					// lambda
					// // if (charSet2.length == 1 && ((int)charSet2[0]) == 255
					// && !(charSet.length == 1 && ((int)charSet[0]) == 255)){
					// // System.out.print("\n --> lambda ignored");
					// // }
					// // else
					// {
					// ArrayList<Character> charArray = new
					// ArrayList<Character>();
					// for (int j = 0; j < charSet2.length; j++)
					// // only add printable characters
					// //if (charSet[j] > 31 && charSet[j] < 127)
					// charArray.add(charSet2[j]);
					// // add the previous set of characters to the whole set of
					// // characters
					// charSets2.addAll(charArray);
					// }
					// System.out.println("\n");
				}
				/** debugging **/
				// for (int k = 0; k < MONALabel.length; k++){
				// System.out.print("[" + MONALabel[k][0]);
				// for (int j = 1; j < MONALabel[k].length; j++)
				// System.out.print(", " + MONALabel[k][j]);
				// System.out.println("]\n");
				// }
				// System.out.println("\n");
				/** end debugging **/
			}
			/**
			 * The following only works if we have a total ascending order (<
			 * with no equality )according to ASCII on charSets, This is the
			 * case here due to the way we parse the MONA label and construct
			 * the char sets. It will construct transitions and label them with
			 * either range of characters of one character
			 */
			ArrayList<Transition> returnMe = new ArrayList<Transition>();
			int trackNum = 0;
			for (int tr = 0; tr < numOfTracks; tr++) {
				trackNum++;
				boolean trackArray[] = tracks[tr];
				// for (int k = 0;k < trackArray.length; k++)
				// System.out.println(trackArray[k]);
				char first = (char) -1;
				for (int i = 0; i < 256; i++) {
					if (trackArray[i]) {
						if (first == (char) -1) {
							first = (char) i;
						}
					} else if (!(first == (char) -1)) {
						Transition t = new Transition(first, (char) (i - 1),
								trackNum, to);
						// System.out.println("transition with label ["
						// +Transition.returnCharString(first) + "-" +
						// Transition.returnCharString((char)(i-1)) +
						// "] added to track "+trackNum );
						returnMe.add(t);
						// start a new range
						first = (char) -1;
					}
				}
				if (!(first == (char) -1)) {
					// process the last char in the array
					Transition t = new Transition(first, (char) 255, trackNum,
							to);
					// System.out.println("transition with label ["
					// +Transition.returnCharString(first) + "-" +
					// Transition.returnCharString((char)255) +
					// "] added to track "+trackNum );
					returnMe.add(t);
				}
			}

			// if (!charSets2.isEmpty()) { // if all the transitions has only
			// NON-ASCII chars then do not create any transition
			// // first char in an range
			// char first = charSets2.remove(0).charValue();
			// // previously read char (to check if we still have an range)
			// char prev = first;
			// char current = first;
			// for (Character c : charSets2) {
			// current = c.charValue();
			// // this to check that we have the total order
			// assert (current != prev);
			// // if we are still in a range then keep going
			// if (current == (prev + 1)) {
			// prev = current;
			// continue;
			// }
			// // if we finished one range then construct a transition with
			// // this range (may have only one char)
			// else {
			// Transition t = new Transition(first, prev, 2, to);
			// System.out.println("transition with label ["
			// +Transition.returnCharString(first) + "-" +
			// Transition.returnCharString(prev) + "] added to track 2" );
			// returnMe.add(t);
			// // start a new range
			// first = prev = current;
			// }
			// }
			// // process the last char in the array
			// Transition t = new Transition(first, current, 2, to);
			// System.out.println("transition with label ["
			// +Transition.returnCharString(first) + "-" +
			// Transition.returnCharString(current) + "] added to track 2" );
			// returnMe.add(t);
			// }
			// else {
			// System.out.println("second track has no chars");
			// }
			return returnMe;
		}

		/**
		 * removes the current track to process the next one
		 * 
		 * @param cs
		 */
		private void shiftLeft(char[] cs) {
			for (int i = 0; i < cs.length - charLength; i++) {
				cs[i] = cs[i + charLength];
			}
			// for debugging
			// System.out.print("after shift: [");
			// for (int i = 0; i < 8; i++){
			//				
			// System.out.print(cs[i]+", ");
			// }
			// System.out.println("]");

		}

		/**
		 * converts a MONA binary (ASCII) character with 0s, 1s, and Xs to a
		 * ascendingly sorted list of actual ASCII characters
		 * 
		 * @param MONAChar
		 *            an array of eight 0's, 1's and/or Xs that represents a set
		 *            of ASCII characters i.e. [0, 1, 0, 1, 0, 1, 0, X] = {U,V}
		 * @param index
		 *            : as this is a recursive call, index tells us which
		 *            position in current input MONAchar to process (first call
		 *            use 0)
		 * @return an array of ASCII characters sorted according to ASCII
		 *         sorting. Example param would give [U, V]
		 */
		public char[] MONACharToCharSet(char[] MONAChar, int index) {
			char[] result = null;
			// Base case (reading last character in oneChar which is the least
			// significant bit)
			if (index == 7) {
				if (MONAChar[index] == '0' || MONAChar[index] == '1') {
					result = new char[1];
					if (MONAChar[index] == '0')
						result[0] = 0;
					else
						result[0] = 1;
				} else {// oneChar[index] == 'x'
					result = new char[2];
					result[0] = 0;
					result[1] = 1;
				}
			} else {
				// get all possible char values from suffix subarray then add
				// current values to it
				char[] prevResult = MONACharToCharSet(MONAChar, (index + 1));
				if (MONAChar[index] == '0' || MONAChar[index] == '1') {
					result = prevResult;
					for (int i = 0; i < prevResult.length; i++)
						result[i] = (char) (result[i] + (((int) MONAChar[index]) - 48)
								* Math.pow(2, (7 - index)));
				} else {// oneChar[index] == 'x'
					result = new char[prevResult.length * 2];
					// first multiply by 0
					for (int i = 0; i < prevResult.length; i++)
						result[i] = prevResult[i];
					// then multiply by 1
					for (int i = 0; i < prevResult.length; i++)
						result[i + prevResult.length] = (char) (prevResult[i] + Math
								.pow(2, (7 - index)));
				}
			}
			return result;
		}

		private void commonProcessing(SimpleNode node, Visitor visitor) {
			if (node.jjtGetParent() instanceof SimpleNode) {
				int parentId = node.jjtGetParent().hashCode();
				int myId = node.hashCode();
				// Debugger.debug(myId + " [shape=circle,label=\"" +
				// visitor.id++
				// + ":" + node.toString() + "\"];");
				// Debugger.debug(parentId + " -> " + myId);
			} else {
				int myId = node.hashCode();
				// Debugger.debug(myId + " [shape=box,label=\"" + visitor.id++
				// + ":" + node.toString() + "no parent" + "\"];");
			}
		}

		@Override
		public Object visit(SimpleNode node, Object data) {
			commonProcessing(node, this);
			node.childrenAccept(this, data);
			return null;
		}

		@Override
		public Object visit(ASTparse node, Object data) {
			commonProcessing(node, this);
			node.childrenAccept(this, data);
			return null;
		}

		@Override
		public Object visit(ASTgraph node, Object data) {
			commonProcessing(node, this);
			node.childrenAccept(this, data);
			return null;
		}

		@Override
		public Object visit(ASTstmt_list node, Object data) {
			commonProcessing(node, this);
			node.childrenAccept(this, data);
			return null;
		}

		@Override
		public Object visit(ASTStatement node, Object data) {
			commonProcessing(node, this);
			node.childrenAccept(this, data);
			return null;
		}

		@Override
		public Object visit(ASTideq_stmt node, Object data) {
			commonProcessing(node, this);
			node.childrenAccept(this, data);
			return null;
		}

		@Override
		public Object visit(ASTattr_stmt node, Object data) {
			commonProcessing(node, this);
			if (node.jjtGetValue().equals("node")) {
				for (int i = 0; i < node.jjtGetNumChildren(); i++) {
					Node child = node.jjtGetChild(i);
					if (child instanceof ASTattr_list) {
						String childNodeShape = (String) child.jjtAccept(this,
								data);
						if (childNodeShape.equals("doublecircle") || childNodeShape.equals("box"))
							this.accept = true;
						else
							this.accept = false;
					} else {
						// not interested in its data (may be in the subtree
						// data so
						// run accept to visit its subtree)
						child.jjtAccept(this, data);
					}
				}
			} else
				// not interested in specific child data (may be in the subtree
				// data
				// so run accept to visit its subtree)
				node.childrenAccept(this, data);

			return null;
		}

		@Override
		public Object visit(ASTnode_stmt node, Object data) {
			commonProcessing(node, this);
			Automaton auto = (Automaton) data;
			for (int i = 0; i < node.jjtGetNumChildren(); i++) {
				Node child = node.jjtGetChild(i);
				if (child instanceof ASTnode_id) {
					String value = (String) child.jjtAccept(this, data);
					State s = string2State.get(value);
					if (s == null) {
						s = new State();
						string2State.put(value, s);
						if (accept) // an accept state (with a doublecircle
							// shape in MONA dot file)
							s.accept = true;
						try {
							// keep 0 for inits
							s.number = s.id = Integer.parseInt(value);
							if (s.id == 0)
								auto.setInitialState(s);
						} catch (NumberFormatException e) {
							// all states from mona dot files are numbered
							// except a dump initial state called "init"
							if (!value.equals("init"))
								throw new RuntimeException(
										"Parsing Error: A state has a noninteger value but not init");
						}
						Debugger.debug("Node " + value + " has been read\n", 2,
								debugLevel);
					} else {
						throw new RuntimeException("Parsing Error: State "
								+ value
								+ " has been parsed before (duplicate state)");
					}

				} else {
					// not interested in its data (may be in the subtree data so
					// run accept to visit its subtree)
					child.jjtAccept(this, data);
				}
			}

			return null;
		}

		/**
		 * node_id : ID An ID is one of the following: # Any string of
		 * alphabetic ([a-zA-Z\200-\377]) characters, underscores ('_') or
		 * digits ([0-9]), not beginning with a digit; # a numeral [-]?(.[0-9]+
		 * | [0-9]+(.[0-9]*)? ); # any double-quoted string ("...") possibly
		 * containing escaped quotes (\")1; # an HTML string (<...>).
		 */
		@Override
		public Object visit(ASTnode_id node, Object data) {
			commonProcessing(node, this);
			return node.jjtGetValue();
		}

		/**
		 * This is a subset of dot grammar
		 * (http://www.graphviz.org/doc/info/lang.html) that only shows what we
		 * deal with in MONA dot files. Example : 0 -> 0
		 * [label="0\n0\n1\n0\n0\n0\n0\n1"] edge_stmt : node_id edgeRHS [
		 * attr_list ] edgeRHS : edgeop node_id attr_list : '[' a_list ']'
		 * a_list : ID '=' ID node_id : ID edgeop : '->' in directed graphs
		 */
		@Override
		public Object visit(ASTedge_stmt node, Object data) {
			commonProcessing(node, this);
			Automaton auto = (Automaton) data;
			int nodesCount = 0;
			State from = null, to = null;
			Transition t = null;
			String edgeLabel = null;
			for (int i = 0; i < node.jjtGetNumChildren(); i++) {
				Node child = node.jjtGetChild(i);

				// read from state
				if (child instanceof ASTnode_id) {
					String value = (String) child.jjtAccept(this, data);
					State s = string2State.get(value);
					if (s != null) {
						from = s;
						Debugger.debug("from: " + s.id + "\n", 2, debugLevel);
					} else {
						// TODO: not pretty sure about this
						throw new RuntimeException(
								"SNH: a node in an edge is not defined before (MONA always defines nodes before using them)");
					}

					// read to state
				} else if (child instanceof ASTedgeRHS) {
					String value = (String) child.jjtAccept(this, data);
					State s = string2State.get(value);
					if (s != null) {
						to = s;
						Debugger.debug("to: " + s.id + "\n", 2, debugLevel);
					} else {
						// TODO: not pretty sure about this
						throw new RuntimeException(
								"SNH: a node in an edge is not defined before (MONA always defines nodes before using them)");
					}

					// read edge label
				} else if (child instanceof ASTattr_list) {
					edgeLabel = (String) child.jjtAccept(this, data);
					Debugger.debug("Edge: " + edgeLabel + "\n", 2, debugLevel);

					// not interesting information from children in AST
				} else {
					// not interested in current childs data but may be in it
					// subtree in AST so
					// run accept to visit its subtree
					child.jjtAccept(this, data);
				}
			}
			assert (from != null);
			assert (to != null);
			// only one edge: (init -> 0) has no label (null). 'init' is a dummy
			// initial state in MONA automaton
			if (edgeLabel != null) {
				ArrayList<Transition> transitions = null;
				if (numOfTracks == 1)
					transitions = this.parseMonaEdge(edgeLabel, to);
				else
					transitions = this.parseMonaMultiTrackEdge(edgeLabel, to);
				for (Transition tr : transitions)
					from.addTransition(tr);
			} else {
				// there is always an edge from init state to some state with no
				// label
				// t = new Transition(' ', to);
				// from.addTransition(t);
			}
			Debugger.debug(from.id + " -> " + to.id + "\n\n\n\n\n\n", 2,
					debugLevel);
			return null;
		}

		/**
		 * Parses a MONA automaton transition label and returns a list of
		 * automaton transitions corresponding to every range of chars in this
		 * MONA transition label Note that if a transition is labeled with
		 * NON-ASCII chars or ASCII chars that are non-printable then it will
		 * not be added.
		 * 
		 * @param MONA
		 *            transition label that is a matrix or 0s, 1s, and/or Xs
		 * @param the
		 *            automaton state that represents the destination of these
		 *            transitions
		 * @return a list of Transitions (could be empty)
		 */
		public ArrayList<Transition> parseMonaEdge(String edgeLabel, State to) {
			edgeLabel = edgeLabel.substring(1);// remove first " in MONA label
			edgeLabel = edgeLabel.substring(0, edgeLabel.length() - 1);// remove
			// last
			// " in
			// MONA
			// label
			StringTokenizer edgeTokenizer = new StringTokenizer(edgeLabel,
					"\\n");
			char[][] MONALabel = null;
			for (int width = 0; width < 8; width++) {// for every column in
				// label matrix (actual
				// ASCII char)
				String nextToken = edgeTokenizer.nextToken();
				StringTokenizer lineTokenizer = new StringTokenizer(nextToken,
						" ,");
				if (MONALabel == null)
					MONALabel = new char[lineTokenizer.countTokens()][8];
				int height = 0;// height of label matrix
				while (lineTokenizer.hasMoreTokens()) {
					String character = lineTokenizer.nextToken();
					MONALabel[height++][width] = character.charAt(0);
				}
			}
			/** debugging **/
			// for (int i = 0; i < label.length; i++){
			// System.out.print("[" + label[i][0]);
			// for (int j = 1; j < label[i].length; j++)
			// System.out.print(", " + label[i][j]);
			// System.out.println("]");
			// }
			/** end debugging **/
			if (edgeTokenizer.hasMoreTokens())
				throw new RuntimeException(
						"There are more than 8 bit in a MONA ASCII char");

			// Convert every MONA binary character into a set of actual
			// characters
			ArrayList<Character> charSets = new ArrayList<Character>();
			for (int i = 0; i < MONALabel.length; i++) {
				// convert a char with 0s, 1s, and/or Xs to a set of characters
				char[] charSet = MONACharToCharSet(MONALabel[i], 0);
				ArrayList<Character> charArray = new ArrayList<Character>();
				for (int j = 0; j < charSet.length; j++)
					// only add printable characters
					// if (charSet[j] > 31 && charSet[j] < 127)
					charArray.add(charSet[j]);
				// add the previous set of characters to the whole set of
				// characters
				charSets.addAll(charArray);
			}

			/**
			 * The following only works if we have a total ascending order (<
			 * with no equality )according to ASCII on charSets, This is the
			 * case here due to the way we parse the MONA label and construct
			 * the char sets. It will construct transitions and label them with
			 * either range of characters of one character
			 */
			ArrayList<Transition> returnMe = new ArrayList<Transition>();
			if (!charSets.isEmpty()) { // if all the transitions has only
										// NON-ASCII chars then do not create
										// any transition
				// first char in an range
				char first = charSets.remove(0).charValue();
				// previously read char (to check if we still have an range)
				char prev = first;
				char current = first;
				for (Character c : charSets) {
					current = c.charValue();
					// this to check that we have the total order
					assert (current != prev);
					// if we are still in a range then keep going
					if (current == (prev + 1)) {
						prev = current;
						continue;
					}
					// if we finished one range then construct a transition with
					// this range (may have only one char)
					else {
						Transition t = new Transition(first, prev, to);
						returnMe.add(t);
						// start a new range
						first = prev = current;
					}
				}
				// process the last char in the array
				Transition t = new Transition(first, current, to);
				returnMe.add(t);
			}
			return returnMe;
		}

		@Override
		public Object visit(ASTsubgraph node, Object data) {
			commonProcessing(node, this);
			node.childrenAccept(this, data);
			return null;
		}

		@Override
		public Object visit(ASTedgeRHS node, Object data) {
			commonProcessing(node, this);
			String value = null;
			for (int i = 0; i < node.jjtGetNumChildren(); i++) {
				Node child = node.jjtGetChild(i);
				if (child instanceof ASTnode_id) {
					value = (String) child.jjtAccept(this, data);

				} else {
					child.jjtAccept(this, data);
				}
			}

			return value;
		}

		@Override
		public Object visit(ASTedgeop node, Object data) {
			commonProcessing(node, this);
			node.childrenAccept(this, data);
			return null;
		}

		@Override
		public Object visit(ASTattr_list node, Object data) {
			commonProcessing(node, this);
			String attrValue = null;
			for (int i = 0; i < node.jjtGetNumChildren(); i++) {
				Node child = node.jjtGetChild(i);
				if (child instanceof ASTa_list) {
					attrValue = (String) child.jjtAccept(this, data);
				} else {
					// not interested in its data (may be in the subtree data so
					// run accept to visit its subtree)
					child.jjtAccept(this, data);
				}
			}
			return attrValue;
		}

		@Override
		public Object visit(ASTa_list node, Object data) {
			commonProcessing(node, this);
			node.childrenAccept(this, data);
			// StringTokenizer tokenizer = new
			// StringTokenizer((String)node.jjtGetValue(), " ");
			// String[] attr = new String[2];
			// attr[0] = tokenizer.nextToken();// attribute name
			// attr[1] = tokenizer.nextToken();// attribute value

			return node.jjtGetValue();
		}

	}

	public static Automaton testAuto3tracks() {
		Automaton auto = new Automaton();
		State s = new State();
		s.id = s.number = 0;
		auto.setInitialState(s);

		State s1 = new State();
		s1.id = s1.number = 1;
		Transition tr = new Transition('<', '<', 1, s1);
		s.addTransition(tr);
		tr = new Transition((char) 255, (char) 255, 2, s1);
		s.addTransition(tr);
		tr = new Transition((char) 255, (char) 255, 3, s1);
		s.addTransition(tr);

		State s2 = new State();
		s2.id = s2.number = 2;
		tr = new Transition((char) 0, (char) 255, 2, s2);
		s.addTransition(tr);
		tr = new Transition((char) 0, (char) 255, 1, s2);
		s.addTransition(tr);
		tr = new Transition((char) 0, (char) 255, 3, s2);
		s.addTransition(tr);

		State s3 = new State();
		s3.id = s3.number = 3; // s3.accept = true;
		tr = new Transition((char) 0, (char) 255, 1, s3);
		s1.addTransition(tr);
		tr = new Transition((char) 0, (char) 255, 2, s3);
		s1.addTransition(tr);
		tr = new Transition((char) 0, (char) 255, 3, s3);
		s1.addTransition(tr);
		tr = new Transition((char) 0, (char) 255, 2, s3);
		s2.addTransition(tr);
		tr = new Transition((char) 0, (char) 255, 3, s3);
		s2.addTransition(tr);
		tr = new Transition((char) 0, (char) 255, 1, s3);
		s2.addTransition(tr);

		State s4 = new State();
		s4.id = s4.number = 4;
		s4.accept = true;
		tr = new Transition((char) 255, (char) 255, 1, s4);
		// s3.addTransition(tr);
		tr = new Transition((char) 255, (char) 255, 2, s4);
		// s3.addTransition(tr);
		tr = new Transition((char) 255, (char) 255, 3, s4);
		// s3.addTransition(tr);

		// State s5 = new State();
		// s5.id = s5.number = 3; //s3.accept = true;
		// tr = new Transition((char)255, (char)255, 1, s5);
		// s1.addTransition(tr);
		// tr = new Transition('<', '<', 2, s5);
		// s1.addTransition(tr);

		return auto;

	}

	public static Automaton testAuto() {
		Automaton auto = new Automaton();
		State s = new State();
		s.id = s.number = 0;
		auto.setInitialState(s);

		State s1 = new State();
		s1.id = s1.number = 1;
		Transition tr = new Transition((char) 255, (char) 255, 1, s1);
		s.addTransition(tr);
		tr = new Transition('S', 'S', 2, s1);
		s.addTransition(tr);

		State s2 = new State();
		s2.id = s2.number = 2;
		tr = new Transition((char) 255, (char) 255, 2, s2);
		s.addTransition(tr);
		tr = new Transition('<', '<', 1, s2);
		s.addTransition(tr);

		State s3 = new State();
		s3.id = s3.number = 3; // s3.accept = true;
		tr = new Transition((char) 255, (char) 255, 1, s3);
		s1.addTransition(tr);
		tr = new Transition('<', '<', 2, s3);
		s1.addTransition(tr);
		tr = new Transition((char) 255, (char) 255, 2, s3);
		s2.addTransition(tr);
		tr = new Transition('<', '<', 1, s3);
		s2.addTransition(tr);

		State s4 = new State();
		s4.id = s4.number = 4;
		s4.accept = true;
		tr = new Transition((char) 255, (char) 255, 1, s4);
		s3.addTransition(tr);
		tr = new Transition((char) 255, (char) 255, 2, s4);
		s3.addTransition(tr);

		// State s5 = new State();
		// s5.id = s5.number = 3; //s3.accept = true;
		// tr = new Transition((char)255, (char)255, 1, s5);
		// s1.addTransition(tr);
		// tr = new Transition('<', '<', 2, s5);
		// s1.addTransition(tr);

		return auto;

	}

//	public static void setUpStrangerLibrary() throws Exception {
//		StrangerAutomaton.initialize(true);
//		PerfInfo perfInfo = new PerfInfo();
//		StrangerAutomaton.perfInfo = perfInfo;
//		StrangerAutomaton
//				.appendCtraceFile("/tmp/stranger_automaton_exec_trace.c");
//	}
//
//	public static void tearDownStrangerLibrary() throws Exception {
//		StrangerAutomaton.closeCtraceFile();
//	}
	public static void runSimulationAlgorithm(String patchAutoPath, String type) throws FileNotFoundException, ParseException {
		DOTParser parser;
		ConsoleMessage.body2("parsing patch auto : " + patchAutoPath);
		parser = new DOTParser(new FileReader(patchAutoPath));
		SimpleNode rootPatchNode = parser.parse();
		Visitor graphVisitor = new BuildAutomaton().new Visitor(0);
		Automaton patchAuto = new Automaton();
		rootPatchNode.childrenAccept(graphVisitor, patchAuto);
		
		ConsoleMessage.body2("generating simulator for '" + type + "'language...");
		Performance.startTimer("simgen_time");
		if (type.equalsIgnoreCase("c")) {
			patchAuto.toCMatchCode();
		}
		else if (type.equalsIgnoreCase("js")) {
			patchAuto.toJSMatchCode();
		}
		else if (type.equalsIgnoreCase("php")){
			patchAuto.toMatchCode();
		}
	
		Performance.stopTimer("simgen_time");
		Performance.printTimers();
		ConsoleMessage.body1("it ends here end");
	}
	
	public static void runMinCutAlgorithm(String patchAutoPath, String referenceAutoPath, String type) throws FileNotFoundException, ParseException {
		DOTParser parser;
		ConsoleMessage.body2("parsing patch auto : " + patchAutoPath);
		parser = new DOTParser(new FileReader(patchAutoPath));
		SimpleNode rootPatchNode = parser.parse();
		Visitor graphVisitor = new BuildAutomaton().new Visitor(0);
		Automaton patchAuto = new Automaton();
		rootPatchNode.childrenAccept(graphVisitor, patchAuto);
		
		ConsoleMessage.body2("parsing reference auto : " + referenceAutoPath);
		parser = new DOTParser(new FileReader(referenceAutoPath));
		SimpleNode rootReferenceNode = parser.parse();
		graphVisitor = new BuildAutomaton().new Visitor(0);
		Automaton referenceAuto = new Automaton();
		rootReferenceNode.childrenAccept(graphVisitor, referenceAuto);
		
		ConsoleMessage.body2("calculating mincut charset");
		Performance.startTimer("mincut_time");
		LinkedList<Character> minCharCut = patchAuto.minCharCut();  //This will get the characters in the mincut
		ConsoleMessage.body3("result : size : " + minCharCut.size() + " : chars : " + minCharCut.toString() + " ");
		
	
//		for (Character c : minCharCut ) {
//			String hex = String.format("%04x", (int) c.charValue());
//			System.out.println("----------    '"+hex+"'");
//		}
		// only space char is considered during analysis
		
		StringBuilder code_builder = new StringBuilder();
		if (type.equalsIgnoreCase("js")) {
			code_builder.append("<!DOCTYPE html>\n<html>\n<head>\n\n<meta charset=\"UTF-8\">\n<title>vlab@ucsb : www.cs.ucsb.edu/~vlab</title>\n<script>\n");
			code_builder.append("function sanitize_input(str) {\n");
	
		} 
		else {
			code_builder.append("<?php\nfunction sanitize_input($str) {\n");
		}
		
		if ( minCharCut.contains(' ') ) {
			Character[] chars = {' '};
			for (Character c : chars) {
				int isTrimmed = referenceAuto.isTrimmed(c);
				if (isTrimmed == 2){
					ConsoleMessage.body3("result : delete : preg_replace('/ /', '', $str); ");
					if (type.equalsIgnoreCase("js")) {
						code_builder.append("\tstr = str.replace(/ /g,'');\n");
					} else {
						code_builder.append("\t\t$str = preg_replace('/ /', '', $str);\n");
					}
				}
				else if (isTrimmed == 1) {
					ConsoleMessage.body3("result : trim : trim(); ");
					if (type.equalsIgnoreCase("js")) {
						code_builder.append("\tstr = str.trim();\n");
					} else {
						code_builder.append("\t\t$str = trim($str);\n");
					}
				}
				else {
					ConsoleMessage.body3("result : delete not precise : preg_replace('/ /', '', $str); ");
					if (type.equalsIgnoreCase("js")) {
						code_builder.append("\tstr = str.replace(/ /g,'');\n");
					} else {
						code_builder.append("\t\t$str = preg_replace('/ /', '', $str);\n");
					}
				}
			}
			
			minCharCut.remove(new Character(' '));
		}
		
		if (minCharCut.size() != 0) {
			boolean escape = true;
			char resultChar = (char) 255;
			for (Character c: minCharCut) {
				resultChar = referenceAuto.isEscaped(c.charValue());
				if (resultChar == (char) 255) {
					escape = false;
					break;
				}
				else {
					ConsoleMessage.body3("escapee : '" + c + "' escaper : '" + resultChar + "'");
				}
					
			}
						
			if (escape) {
				ConsoleMessage.body3("result : escape : escape");
				
				if (type.equalsIgnoreCase("js")) {
					code_builder.append("\tvar char_buff = str;\n");
					code_builder.append("\tstr = \"\";\n");
					code_builder.append("\tvar search = \"\" ");
					for (Character c : minCharCut ) {
						code_builder.append(" + String.fromCharCode(" + (int)c + ")");
					}
					code_builder.append(";\n");
					code_builder.append("\tfor(i=0; i<char_buff.length; i++) {\n");
					code_builder.append("\t\tif( search.indexOf(char_buff[i]) != -1) {\n");
					code_builder.append("\t\t\tstr += String.fromCharCode(" + (int)resultChar + ") + iter_str[i];\n");
					code_builder.append("\t\t} else {\n");
					code_builder.append("\t\t\tstr += char_buff[i];\n");
					code_builder.append("\t\t}\n");
					code_builder.append("\t}\n");
				} else {
					code_builder.append("\t$char_buff = preg_split('//', $str, -1);\n");
					code_builder.append("\t$str = \"\";\n");
					code_builder.append("\t$search = \"\"");
					for (Character c : minCharCut ) {
						code_builder.append(".chr(" + (int)c + ")");
					}
					code_builder.append(";\n");
					code_builder.append("\tfor($i=0; $i < sizeof($char_buff); $i++) {\n");
					code_builder.append("\t\tif( strpos($search,$char_buff[$i]) !== false) {\n");
					code_builder.append("\t\t\t$str .= chr(" + (int)resultChar + ") . $char_buff[$i];\n");
					code_builder.append("\t\t} else {\n");
					code_builder.append("\t\t\t$str .= $char_buff[$i];\n");
					code_builder.append("\t\t}\n");
					code_builder.append("\t}\n");
				}	

			}	
			else {
				ConsoleMessage.body3("result : delete : delete");
				if (type.equalsIgnoreCase("js")) {
					code_builder.append("\tvar char_buff = str;\n");
					code_builder.append("\tstr = \"\";\n");
					code_builder.append("\tvar search = \"\" ");
					for (Character c : minCharCut ) {
						code_builder.append(" + String.fromCharCode(" + (int)c + ")");
					}
					code_builder.append(";\n");
					code_builder.append("\tfor(i=0; i<char_buff.length; i++) {\n");
					code_builder.append("\t\tif( search.indexOf(char_buff[i]) != -1) {\n");
					code_builder.append("\t\t\tcontinue;\n");
					code_builder.append("\t\t} else {\n");
					code_builder.append("\t\t\tstr += char_buff[i];\n");
					code_builder.append("\t\t}\n");
					code_builder.append("\t}\n");
				} else {
					code_builder.append("\t$char_buff = preg_split('//', $str, -1);\n");
					code_builder.append("\t$str = \"\";\n");
					code_builder.append("\t$search = \"\"");
					for (Character c : minCharCut ) {
						code_builder.append(".chr(" + (int)c + ")");
					}
					code_builder.append(";\n");
					code_builder.append("\tfor($i=0; $i < sizeof($char_buff); $i++) {\n");
					code_builder.append("\t\tif( strpos($search,$char_buff[$i]) !== false) {\n");
					code_builder.append("\t\t\tcontinue;\n");
					code_builder.append("\t\t} else {\n");
					code_builder.append("\t\t\t$str .= $char_buff[$i];\n");
					code_builder.append("\t\t}\n");
					code_builder.append("\t}\n");
				}
				
			}
		}
		
		if (type.equalsIgnoreCase("js")) {
			code_builder.append("\treturn str;\n\t}\n");
			code_builder.append("function myFunction() {\n");
			code_builder.append("\tvar x=document.getElementById(\"fname\");\n");
			code_builder.append("\tvar r=document.getElementById(\"result\");\n");
			code_builder.append("\tr.innerHTML=\"&#10004; \" + sanitize_input(x.value);\n");
			code_builder.append("}\n");
			code_builder.append("window.onload = myFunction;\n");
			code_builder.append("</script>\n</head>\n<body>\n");
			code_builder.append("Enter your string: <input type=\"text\" id=\"fname\" oninput=\"myFunction()\">&nbsp; <span id=\"result\" style=\"color:rgb(0,255,0)\"></span>\n");
			code_builder.append("<p>As you type the characters, a function is triggered which transforms the input string.</p>\n");
			code_builder.append("</body>\n</html>");
			
		} else {
			code_builder.append("\treturn $str;\n}\n?>");
		}
		
		System.out.println("code:\n" + code_builder.toString());
		Performance.stopTimer("mincut_time");
		Performance.printTimers();
		ConsoleMessage.body1("it ends here end");
	}

	/**
	 * @param args
	 *            : args[1]: the dot file for MONA automaton (better to be in
	 *            test folder in the project), args[2]: the file to write result
	 *            of mincut to args[3]: number of tracks Note that the file to
	 *            dump automaton to for debugging will automatically be inferred
	 * @throws ParseException
	 * @throws IOException
	 */
	public static void main(String[] args) throws ParseException, IOException {
		try {
			
			Options comOptions = new Options();
			
			comOptions.addOption("p", "patch-auto", true, "sanitization patch automaton/automata (file or folder)");
			comOptions.addOption("r", "reference-auto", true, "reference automaton/automata (file or folder), only used for mincut");
			comOptions.addOption("s", "simulate", false, "generate automaton simulator for patch automaton, no mincut");
			comOptions.addOption("l", "language", true, "output language (C, JS, PHP)");
			comOptions.addOption("c", "boost-chars", true, "characters needs to be boosted by mincut algorithm, input as java regex");
			comOptions.addOption("b", "boost-value", true, "boost value for the characters defined in boost-chars option");
			comOptions.addOption("h", "help", false, "list available options");
			
			CommandLineParser comParser = new PosixParser();
			CommandLine com = null;
			
			try {
				com = comParser.parse(comOptions, args);
			} catch (Exception e) {
				 HelpFormatter helpFormatter = new HelpFormatter();
			     helpFormatter.printHelp( "check available options", comOptions);
			     System.out.println(e.getMessage());
			}
			if (com.hasOption('h')) {
				HelpFormatter helpFormatter = new HelpFormatter();
				helpFormatter.printHelp( "check available options", comOptions);
				System.exit(0);
			}
			
			if (com.hasOption('s')) {
				ComOptions.RUN_MINCUT = false;
			}
			
			if (com.hasOption('l')) {
				ComOptions.SIMULATION_LANGUAGE = com.getOptionValue('l');
			}
			
			if (com.hasOption('r')) {
				ComOptions.REFERENCE_AUTO_PATH = com.getOptionValue('r');
			}
			
			if (com.hasOption('c')) {
				ComOptions.BOOST_CHARSET = com.getOptionValue('c');
			}
			
			if (com.hasOption('b')) {
				ComOptions.BOOST_VALUE = com.getOptionValue('b');
			}
			
			// read patch automaton/automata
			if ( com.hasOption('p')) {
				ComOptions.PATCH_AUTO_PATH = com.getOptionValue('p');
				
				Queue<String> fileNames = new LinkedList<String>();
				
				ConsoleMessage.header("Wellcome to mincut calculator");
				
				File inputs = new File(ComOptions.PATCH_AUTO_PATH);
				if ( inputs.isDirectory() )
				{
					if (com.hasOption('r') && !ComOptions.REFERENCE_AUTO_PATH.endsWith("/")) {
						ComOptions.REFERENCE_AUTO_PATH += "/";
					}
					
					FilenameFilter filter = new FilenameFilter() {						
						@Override
						public boolean accept(File dir, String name) {
							if ( name.endsWith(".dot") ) return true;
							return false;
						}
					};
					for ( File f : inputs.listFiles(filter) ) {
						fileNames.add(f.getAbsolutePath());
					}
					
					// iterate over filtered files
					while ( ! fileNames.isEmpty() ) {
						String fileName = fileNames.poll();
						String name = fileName.substring(fileName.lastIndexOf('/')+1);
						ConsoleMessage.body1("mincut for : " + name);
						if (ComOptions.RUN_MINCUT) {
							runMinCutAlgorithm(fileName, ComOptions.REFERENCE_AUTO_PATH + name, ComOptions.SIMULATION_LANGUAGE);
						} else {
							runSimulationAlgorithm(fileName,ComOptions.SIMULATION_LANGUAGE);
						}
					}					
				}
				else {
					fileNames.add(inputs.getAbsolutePath());
					ConsoleMessage.body1("mincut for : " + inputs.getName());
					if (ComOptions.RUN_MINCUT) {
						runMinCutAlgorithm(inputs.getAbsolutePath(), ComOptions.REFERENCE_AUTO_PATH, ComOptions.SIMULATION_LANGUAGE);
					}
					else {
						runSimulationAlgorithm(inputs.getAbsolutePath(),ComOptions.SIMULATION_LANGUAGE);
					}
				}
			} else {
				 HelpFormatter helpFormatter = new HelpFormatter();
			     helpFormatter.printHelp( "check available options", comOptions);
			}
			
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	@SuppressWarnings("unused")
	private static void dumpAuto(String fileName, Automaton auto)
			throws IOException {
		Writer output;
		File autoFile = new File(fileName);
		output = new BufferedWriter(new FileWriter(autoFile));
		output.write(auto.toDot());
		output.close();
	}

}
