package at.ac.tuwien.infosys.www.pixy.automaton.BuildAutomatonTest;

import java.util.ArrayList;

import junit.framework.TestCase;
import at.ac.tuwien.infosys.www.pixy.automaton.BuildAutomaton;
import at.ac.tuwien.infosys.www.pixy.automaton.Transition;
import at.ac.tuwien.infosys.www.pixy.automaton.BuildAutomaton.Visitor;

public class VisitorTestCase extends TestCase {

	Visitor visitor;
	protected void setUp() throws Exception {
		BuildAutomaton autoBuilder = new BuildAutomaton();
		this.visitor = autoBuilder.new Visitor(0);
		super.setUp();
	}

	public void testMONAChatToCharSet() {
		char[] oneChar = new char[8];
		for (int i = 0; i < 7; i++)
			oneChar[i] = '0';
		oneChar[7] = '1';
		char[] result = visitor.MONACharToCharSet(oneChar, 0);
		assertEquals(result.length, 1);
		assertEquals(result[0], (char)1);
		
		oneChar = new char[8];
		for (int i = 0; i < 7; i++)
			oneChar[i] = '0';
		oneChar[7] = 'x';
		result = visitor.MONACharToCharSet(oneChar, 0);
		assertEquals(result.length, 2);
		assertEquals(result[0], 0);
		assertEquals(result[1], (char)1);
		
		oneChar = new char[8];
		for (int i = 1; i < 8; i++)
			oneChar[i] = '0';
		oneChar[0] = 'x';
		result = visitor.MONACharToCharSet(oneChar, 0);
		assertEquals(result.length, 2);
		assertEquals(result[0], (char)0);
		assertEquals(result[1], (char)128);
		
		oneChar = new char[8];
		for (int i = 1; i < 4; i++)
			oneChar[i] = '0';
		for (int i = 5; i < 8; i++)
			oneChar[i] = '0';
		oneChar[0] = 'x';
		oneChar[4] = 'x';
		result = visitor.MONACharToCharSet(oneChar, 0);
		assertEquals(result.length, 4);
		assertEquals(result[0], (char)0);
		assertEquals(result[1], (char)8);
		assertEquals(result[2], (char)128);
		assertEquals(result[3], (char)136);
		
		oneChar = new char[8];
		for (int i = 1; i < 4; i++)
			oneChar[i] = '0';
		for (int i = 5; i < 8; i++)
			oneChar[i] = '1';
		oneChar[0] = 'x';
		oneChar[4] = 'x';
		result = visitor.MONACharToCharSet(oneChar, 0);
		assertEquals(result.length, 4);
		assertEquals(result[0], (char)7);
		assertEquals(result[1], (char)15);
		assertEquals(result[2], (char)135);
		assertEquals(result[3], (char)143);
		
		oneChar = new char[8];
		for (int i = 0; i < 8; i++)
			oneChar[i] = 'x';
		result = visitor.MONACharToCharSet(oneChar, 0);
		assertEquals(result.length, 256);
		assertEquals(result[0], (char)0);
		assertEquals(result[1], (char)1);
		assertEquals(result[128], (char)128);
		assertEquals(result[255], (char)255);
		assertEquals(result[87], 'W');
	}
	public void testparseMonaEdge() {
		String edgeLabel = "\"0 0 0 0 0 0 0 1 1 1 1 1 1\\n0 0 0 0 0 0 1 0 1 1 1 1 1\\n0 1 1 1 1 1 X X 0 1 1 1 1\\nX 0 0 0 0 1 X X X 0 1 1 1\\nX 0 0 0 1 X X X X X 0 1 1\\nX 0 0 1 X X X X X X X 0 1\\nX 0 1 X X X X X X X X X 0\\nX,0,X,X,X,X,X,X,X,X,X,X,X\"";
		ArrayList<Transition> result = visitor.parseMonaEdge(edgeLabel, null);
		//32-32 34-253  (original is 0-32 34-253 but we do not output ASCII < 32 non printable chars)
		assertEquals(result.size(), 2);
		assertEquals(result.get(0).getMin(), ' ');
		assertEquals(result.get(0).getMax(), ' ');
		assertEquals(result.get(1).getMin(), '"');
		assertEquals(result.get(1).getMax(), (char)253);
		
		edgeLabel = "\"1\\n1\\n1\\n1\\n1\\n1\\n1\\nX\"";
		result = visitor.parseMonaEdge(edgeLabel, null);
		//254-255
		assertEquals(result.size(), 1);
		assertEquals(result.get(0).getMin(), (char)254);
		assertEquals(result.get(0).getMax(), (char)255);
		
		edgeLabel = "\"0 1 1 1 1 1 1\\nX 0 1 1 1 1 1\\nX X 0 1 1 1 1\\nX X X 0 1 1 1\\nX X X X 0 1 1\\nX X X X X 0 1\\nX X X X X X 0\\nX,X,X,X,X,X,X\"";
		result = visitor.parseMonaEdge(edgeLabel, null);
		//32-253
		assertEquals(result.size(), 1);
		assertEquals(result.get(0).getMin(), (char)32);
		assertEquals(result.get(0).getMax(), (char)253);
		
		edgeLabel = "\"0\\n0\\n1\\n0\\n0\\n0\\n0\\n1\"";
		result = visitor.parseMonaEdge(edgeLabel, null);
		//33
		assertEquals(result.size(), 1);
		assertEquals(result.get(0).getMin(), '!');
		assertEquals(result.get(0).getMax(), '!');
		
		edgeLabel = "\"0 1\\n0 1\\n1 1\\n0 1\\n0 1\\n0 1\\n0 1\\n1,X\"";
		result = visitor.parseMonaEdge(edgeLabel, null);
		//33 254-255
		assertEquals(result.size(), 2);
		assertEquals(result.get(0).getMin(), (char)33);
		assertEquals(result.get(0).getMax(), (char)33);
		assertEquals(result.get(1).getMin(), (char)254);
		assertEquals(result.get(1).getMax(), (char)255);
		
		edgeLabel = "\"0 1\\n0 1\\n1 1\\n0 1\\n0 1\\n0 1\\n0 1\\n1,0\"";
		result = visitor.parseMonaEdge(edgeLabel, null);
		//33 254
		assertEquals(result.size(), 2);
		assertEquals(result.get(0).getMin(), (char)33);
		assertEquals(result.get(0).getMax(), (char)33);
		assertEquals(result.get(1).getMin(), (char)254);
		assertEquals(result.get(1).getMax(), (char)254);
		
		edgeLabel = "\"0 1\\n0 1\\n1 1\\n0 1\\n0 1\\n0 1\\n0 1\\nX,X\"";
		result = visitor.parseMonaEdge(edgeLabel, null);
		//32-33 254-255
		assertEquals(result.size(), 2);
		assertEquals(result.get(0).getMin(), (char)32);
		assertEquals(result.get(0).getMax(), (char)33);
		assertEquals(result.get(1).getMin(), (char)254);
		assertEquals(result.get(1).getMax(), (char)255);
		
		edgeLabel = "\"0 0 1\\n0 1 1\\n1 1 1\\n0 1 1\\n0 1 1\\n0 1 1\\n0 1 1\\nX,1,X\"";
		result = visitor.parseMonaEdge(edgeLabel, null);
		//32-33 127 254-255
		assertEquals(result.size(), 3);
		assertEquals(result.get(0).getMin(), (char)32);
		assertEquals(result.get(0).getMax(), (char)33);
		assertEquals(result.get(1).getMin(), (char)127);
		assertEquals(result.get(1).getMax(), (char)127);
		assertEquals(result.get(2).getMin(), (char)254);
		assertEquals(result.get(2).getMax(), (char)255);
		
		edgeLabel = "\"0 0 1\\n0 1 1\\n1 1 1\\n0 1 1\\n0 X 1\\n0 1 1\\n0 1 1\\nX,1,X\"";
		result = visitor.parseMonaEdge(edgeLabel, null);
		//32-33 119 127 254-255
		assertEquals(result.size(), 4);
		assertEquals(result.get(0).getMin(), (char)32);
		assertEquals(result.get(0).getMax(), (char)33);
		assertEquals(result.get(1).getMin(), (char)119);
		assertEquals(result.get(1).getMax(), (char)119);
		assertEquals(result.get(2).getMin(), (char)127);
		assertEquals(result.get(2).getMax(), (char)127);
		assertEquals(result.get(3).getMin(), (char)254);
		assertEquals(result.get(3).getMax(), (char)255);
		
	}

}
