//============================================================================
// Name        : StrangerWrapper.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "stranger_lib_internal.h"
#include <iostream>
#include <vector>
#include "stranger.h"
#include "bdd.h"
#include "dfa.h"
#include "bdd_dump.h"
#include "bdd_external.h"
#include "StrangerAutomaton.hpp"
#include <regex>
#include <fstream>
#include <cstdio>
#include <ctime>

#include <string>
#include <map>
#include <utility>
//#include "DepGraphNormalNode.h"
//#include "DepGraphNode.h"
//#include "DepGraph.h"
//#include "TacPlace.h"
//#include "Variable.h"
//#include "Constant.h"
//#include "Literal.h"
//#include "StringAnalyzer.h"

#include "DepGraph.h"
#include "ForwardImageComputer.h"
#include "StrangerPatcher.h"

using namespace std;

void testReplaceCharWithString(){
    StrangerAutomaton *m = StrangerAutomaton::makeAnyStringL1ToL2(0, 100);
    DFA *a = dfaHtmlSpecialChars(m->dfa, StrangerAutomaton::getVar(), StrangerAutomaton::getIndices());
    DFA *a2 = dfaPreHtmlSpecialChars(a, StrangerAutomaton::getVar(), StrangerAutomaton::getIndices());
//    m->toDotAscii(2);
//    DFA *a = dfa_replace_char_with_string(m->dfa, StrangerAutomaton::getVar(), StrangerAutomaton::getIndices(), '<', "&lt;");
//    cout << "done1 " << a->ns << endl;
//    a = dfa_replace_char_with_string(a, StrangerAutomaton::getVar(), StrangerAutomaton::getIndices(), '>', "&gt;");
////    dfaPrintGraphvizAsciiRange(a, StrangerAutomaton::getVar(), StrangerAutomaton::getIndices(), 0);
//    cout << "done2 " << a->ns << endl;
//    a = dfa_replace_char_with_string(a, StrangerAutomaton::getVar(), StrangerAutomaton::getIndices(), '\'', "&apos;");
//    cout << "done " << a->ns << endl;
//    a = dfa_replace_char_with_string(a, StrangerAutomaton::getVar(), StrangerAutomaton::getIndices(), '"', "&quot;");
//    cout << "done3 " << a->ns << endl;
//        a = dfa_replace_char_with_string(a, StrangerAutomaton::getVar(), StrangerAutomaton::getIndices(), '&', "&amp;");
////    dfaPrintGraphvizAsciiRange(a, StrangerAutomaton::getVar(), StrangerAutomaton::getIndices(), 0);
//        cout << "done4 " << a->ns << endl;
//    a = dfa_pre_replace_char_with_string(a, StrangerAutomaton::getVar(), StrangerAutomaton::getIndices(), '&', "&amp;");
//    cout << "done5 " << a->ns << endl;
//    a = dfa_pre_replace_char_with_string(a, StrangerAutomaton::getVar(), StrangerAutomaton::getIndices(), '"', "&quot;");
//    //    dfaPrintGraphvizAsciiRange(a, StrangerAutomaton::getVar(), StrangerAutomaton::getIndices(), 0);
//    cout << "done6 " << a->ns << endl;
//    a = dfa_pre_replace_char_with_string(a, StrangerAutomaton::getVar(), StrangerAutomaton::getIndices(), '\'', "&apos;");
//    cout << "done7 " << a->ns << endl;
//    a = dfa_pre_replace_char_with_string(a, StrangerAutomaton::getVar(), StrangerAutomaton::getIndices(), '>', "&gt;");
//    cout << "done8 " << a->ns << endl;
//    a = dfa_pre_replace_char_with_string(a, StrangerAutomaton::getVar(), StrangerAutomaton::getIndices(), '<', "&lt;");
//    //    dfaPrintGraphvizAsciiRange(a, StrangerAutomaton::getVar(), StrangerAutomaton::getIndices(), 0);
//    cout << "done9 " << a->ns << endl;
    assert(check_inclusion(StrangerAutomaton::makeString("gj&gtl")->dfa,a, StrangerAutomaton::getVar(), StrangerAutomaton::getIndices()));

}

void testPreReplaceCharWithString(){
    StrangerAutomaton *m = StrangerAutomaton::makeAnyStringL1ToL2(0, 10);
//    //    m->toDotAscii(2);
    DFA *a = dfa_replace_char_with_string(m->dfa, StrangerAutomaton::getVar(), StrangerAutomaton::getIndices(), '<', "&lt;");
    cout << "done" << a->ns << endl;
    assert(!check_inclusion(StrangerAutomaton::makeString("gj<l")->dfa,a, StrangerAutomaton::getVar(), StrangerAutomaton::getIndices()));
//    a = dfa_replace_char_with_string(a, StrangerAutomaton::getVar(), StrangerAutomaton::getIndices(), '&', "&amp;");
//
//    a = dfa_replace_char_with_string(a, StrangerAutomaton::getVar(), StrangerAutomaton::getIndices(), '\'', "&apos;");
//    cout << "done" << a->ns << endl;
    a = dfa_pre_replace_char_with_string(a, StrangerAutomaton::getVar(), StrangerAutomaton::getIndices(), '<', "&lt;");
    assert(check_inclusion(StrangerAutomaton::makeString("gj<l")->dfa,a, StrangerAutomaton::getVar(), StrangerAutomaton::getIndices()));
    dfaPrintGraphvizAsciiRange(a, StrangerAutomaton::getVar(), StrangerAutomaton::getIndices(), 0);
//    a = dfa_replace_char_with_string(a, StrangerAutomaton::getVar(), StrangerAutomaton::getIndices(), '"', "&quot;");
//    cout << "done" << a->ns << endl;
//    a = dfa_replace_char_with_string(a, StrangerAutomaton::getVar(), StrangerAutomaton::getIndices(), '>', "&gt;");
//    //    dfaPrintGraphvizAsciiRange(a, StrangerAutomaton::getVar(), StrangerAutomaton::getIndices(), 0);
//    cout << "done" << a->ns << endl;
//    assert(check_inclusion(StrangerAutomaton::makeString("gj&gtl")->dfa,a, StrangerAutomaton::getVar(), StrangerAutomaton::getIndices()));

}


void testIcse2(){
	int nosink = 0;
    cout << "*******************************************************\n";
    cout << "*         Patching Server against Client\n\n          *\n";
    cout << "*******************************************************\n\n";
    int l = 4;
    bool DEBUG_PRINT = l < 15;
    StrangerAutomaton* tmp = nullptr;

    cout << "*** FORWARD PHASE 1 --> CLIENT ***\n\n";
	//GET CLIENT
    StrangerAutomaton* clientInput1 = StrangerAutomaton::makeAnyString();
    cout << "\n\nclient input 1:\n";
	if (DEBUG_PRINT) clientInput1->toDotAscii(nosink);

    tmp = StrangerAutomaton::makeAnyStringL1ToL2(0, l);
	StrangerAutomaton* clientSink1 = clientInput1->intersect(tmp);
    delete tmp;
//    StrangerAutomaton* clientSink1 = clientInput1->intersect(StrangerAutomaton::makeAnyString());

    cout << "\n\nclient sink 1:\n";
	if (DEBUG_PRINT) clientSink1->toDotAscii(nosink);


    cout << "*** FORWARD PHASE 1 --> SERVER ***\n\n";
	//FORWARD1
	StrangerAutomaton* serverInput1 = StrangerAutomaton::makeAnyString();
	cout << "\n\nserver input 1:\n";
	if (DEBUG_PRINT) serverInput1->toDotAscii(nosink);

	StrangerAutomaton* serverAddslashes1 = StrangerAutomaton::addslashes(serverInput1);
	cout << "\n\nserverAddslashes1:\n";
	if (DEBUG_PRINT) serverAddslashes1->toDotAscii(nosink);
	StrangerAutomaton* serverSink1 = serverAddslashes1->trimSpaces();
	cout << "\n\nserverSink1:\n";
	if (DEBUG_PRINT) serverSink1->toDotAscii(nosink);


    cout << "*** FORWARD PHASE 1 --> RESTRICT SERVER LENGTH ***\n\n";
	// RESTRICT LENGTH BY CLIENT
    assert(clientSink1->isLengthFinite() == true);
	StrangerAutomaton* serverfw1 = serverSink1->restrictLengthByOtherAutomatonFinite(clientSink1);
	cout << "\n\n\nserverfw1-len restricted\n";
	if (DEBUG_PRINT) serverfw1->toDotAscii(nosink);

    cout << "*** BACKWARD PHASE 1 --> SERVER ***\n\n";
	//BACKWARD1
	StrangerAutomaton* pretrim1 = serverfw1->preTrimSpaces();
    tmp = pretrim1;
    pretrim1 = pretrim1->intersect(serverAddslashes1);
    delete tmp;
	cout << "\n\n\npretrim1\n";
	if (DEBUG_PRINT) pretrim1->toDotAscii(nosink);

	StrangerAutomaton* serverbw1 = StrangerAutomaton::pre_addslashes(pretrim1);
	cout << "\n\n\npreAddSlashes1-serverbw1\n";
	if (DEBUG_PRINT) serverbw1->toDotAscii(nosink);

	/******* HERE WE CONSTRAIN LENGTH BY MATCHING AGAINST THIS RESULT ******/

    cout << "*** FORWARD PHASE 2 --> SERVER ***\n\n";
	//FORWARD2
	StrangerAutomaton* addslashes2 = StrangerAutomaton::addslashes(serverbw1);
	cout << "\n\n\naddSlashes2\n";
	if (DEBUG_PRINT) addslashes2->toDotAscii(nosink);

	StrangerAutomaton* serverSink2 = addslashes2->trimSpaces();
	cout << "\n\n\ntrim2- serverSink2\n";
	if (DEBUG_PRINT) serverSink2->toDotAscii(nosink);



    cout << "*** FORWARD PHASE 2 -->  DIFFERENCE (SERVER - CLIENT) ***\n\n";
	//RESTRICT SERVER BY CLIENT
    tmp = clientSink1->complement();
	StrangerAutomaton* serverfw2 = serverSink2->intersect(tmp);
    delete tmp;
	cout << "\n\n\nserverfw2\n";
	if (DEBUG_PRINT) serverfw2->toDotAscii(nosink);



    cout << "*** BACKWARD PHASE 2 --> SERVER ***\n\n";
	//backward2
	StrangerAutomaton* pretrim2 = serverfw2->preTrimSpaces();
	cout << "\n\n\npreTrim2\n";
	if (DEBUG_PRINT) pretrim2->toDotAscii(nosink);

	StrangerAutomaton* serverbw2 = StrangerAutomaton::pre_addslashes(pretrim2);
	cout << "\n\n\npreAddSlashes1-serverbw2\n";
	if (DEBUG_PRINT) serverbw2->toDotAscii(nosink);

    delete clientInput1;
    delete clientSink1;
    delete serverInput1;
    delete serverSink1;
    delete serverfw1;
    delete serverSink2;
    delete serverfw2;
    delete addslashes2;
    delete pretrim1;
    delete pretrim2;
    delete serverbw1;
    delete serverbw2;
    delete serverAddslashes1;

}


void testIcsepre1(){
	int nosink = 0;
    cout << "*******************************************************\n";
    cout << "*         Patching Server against Client\n\n          *\n";
    cout << "*******************************************************\n\n";
    int l = 400;
    int l2 = 4;
    bool DEBUG_PRINT = l < 15;
    StrangerAutomaton* tmp = nullptr;

    StrangerAutomaton* clientC1 = StrangerAutomaton::makeAnyStringL1ToL2(0, l)->complement();
    clientC1->toDotAscii(0);
    StrangerAutomaton *trimmed = StrangerAutomaton::makeAnyString()->trimSpaces();
    trimmed->toDotAscii(0);
    clientC1 = clientC1->intersect(trimmed);
    clientC1->toDotAscii(0);
    clientC1 = clientC1->preTrimSpaces();
    clientC1->toDotAscii(0);

}

void testIcsepre2(){
	int nosink = 0;
    cout << "*******************************************************\n";
    cout << "*         Patching Server against Client\n\n          *\n";
    cout << "*******************************************************\n\n";
    int l = 8;
    int l2 = 4;
    bool DEBUG_PRINT = l < 15;
    StrangerAutomaton* tmp = nullptr;

    StrangerAutomaton* clientC1 = StrangerAutomaton::regExToAuto("/.+@.+\\.com/")->complement();
//    clientC1->toDotAscii(0);
    StrangerAutomaton *lower = StrangerAutomaton::makeAnyString()->toLowerCase();
//    lower->toDotAscii(0);
    clientC1 = clientC1->intersect(lower);
//    clientC1->toDotAscii(0);
    clientC1 = clientC1->preToLowerCase();
    clientC1->toDotAscii(0);

    StrangerAutomaton *clientC2 = StrangerAutomaton::makeAnyStringL1ToL2(0, l)->complement();
//    clientC2->toDotAscii(0);
    clientC1 = clientC1->union_(clientC2);
    StrangerAutomaton *trimmed = StrangerAutomaton::makeAnyString()->trimSpaces();
//    trimmed->toDotAscii(0);
    clientC1 = clientC1->intersect(trimmed);
//    clientC1->toDotAscii(0);
    clientC1 = clientC1->preTrimSpaces();
    clientC1->toDotAscii(0);
}


void testIcsepre3(){
	int nosink = 0;
    cout << "*******************************************************\n";
    cout << "*         Patching Server against Client\n\n          *\n";
    cout << "*******************************************************\n\n";
    int l = 4;
    int l2 = 4;
    bool DEBUG_PRINT = l < 15;
    StrangerAutomaton* tmp = nullptr;

    StrangerAutomaton* clientC1 = StrangerAutomaton::regExToAuto("/.+@.+\\.com/")->complement();
    clientC1->toDotAscii(0);
    StrangerAutomaton *trimmed = StrangerAutomaton::makeAnyString()->trimSpaces();
    trimmed->toDotAscii(0);
    clientC1 = clientC1->intersect(trimmed);
    clientC1->toDotAscii(0);
    clientC1 = clientC1->preTrimSpaces();
    clientC1->toDotAscii(0);
}



void testIcse2Reverse(){
	int nosink = 0;
    cout << "*******************************************************\n";
    cout << "*         Patching Server against Client\n\n          *\n";
    cout << "*******************************************************\n\n";

    cout << "*** FORWARD PHASE 1 --> CLIENT ***\n\n";
	//GET CLIENT
    StrangerAutomaton* clientInput1 = StrangerAutomaton::makeAnyString();
    cout << "\n\nclient input 1:\n";
	clientInput1->toDotAscii(nosink);


	StrangerAutomaton* clientSink1 = clientInput1->intersect(StrangerAutomaton::makeAnyStringL1ToL2(0, 10));
    //    StrangerAutomaton* clientSink1 = clientInput1->intersect(StrangerAutomaton::makeAnyString());

    cout << "\n\nclient sink 1:\n";
	clientSink1->toDotAscii(nosink);


    cout << "*** FORWARD PHASE 1 --> SERVER ***\n\n";
	//FORWARD1
	StrangerAutomaton* serverInput1 = StrangerAutomaton::makeAnyString();
	cout << "\n\nserver input 1:\n";
//	serverInput1->toDotAscii(nosink);

	StrangerAutomaton* serverTrim1 = serverInput1->trimSpaces();
	cout << "\n\nserverSink1:\n";
//	serverAddslashes1->toDotAscii(nosink);

    StrangerAutomaton* serverSink1 = StrangerAutomaton::addslashes(serverTrim1);
	cout << "\n\nserverAddslashes1:\n";
//	serverAddslashes1->toDotAscii(nosink);



    cout << "*** FORWARD PHASE 1 --> RESTRICT SERVER LENGTH ***\n\n";
	// RESTRICT LENGTH BY CLIENT
	StrangerAutomaton* serverfw1 = serverSink1->restrictLengthByOtherAutomaton(clientSink1);
	cout << "\n\n\nserverfw1-len restricted\n";
	serverfw1->toDotAscii(nosink);

    cout << "*** BACKWARD PHASE 1 --> SERVER ***\n\n";
	//BACKWARD1
    StrangerAutomaton* preAddSlashes1 = StrangerAutomaton::pre_addslashes(serverfw1);
	cout << "\n\n\npreAddSlashes1-serverbw1\n";
	preAddSlashes1->toDotAscii(nosink);

	StrangerAutomaton* pretrim1 = preAddSlashes1->preTrimSpaces();
	cout << "\n\n\npretrim1\n";
	pretrim1->toDotAscii(nosink);



	/******* HERE WE CONSTRAIN LENGTH BY MATCHING AGAINST THIS RESULT ******/

    cout << "*** FORWARD PHASE 2 --> SERVER ***\n\n";
	//FORWARD2
	StrangerAutomaton* trimspaces = pretrim1->trimSpaces();
	cout << "\n\n\naddSlashes2\n";
	trimspaces->toDotAscii(nosink);

	StrangerAutomaton* serverSink2 = StrangerAutomaton::addslashes(trimspaces);
	cout << "\n\n\ntrim2- serverSink2\n";
//	serverSink2->toDotAscii(nosink);



    cout << "*** FORWARD PHASE 2 -->  DIFFERENCE (SERVER - CLIENT) ***\n\n";
	//RESTRICT SERVER BY CLIENT

	StrangerAutomaton* serverfw2 = serverSink2->intersect(clientSink1->complement());
	cout << "\n\n\nserverfw2\n";
	serverfw2->toDotAscii(nosink);



    cout << "*** BACKWARD PHASE 2 --> SERVER ***\n\n";
	//backward2
	StrangerAutomaton* pretrim2 = serverfw2->preTrimSpaces();
	cout << "\n\n\npreTrim2\n";
	pretrim2->toDotAscii(nosink);

	StrangerAutomaton* serverbw2 = StrangerAutomaton::pre_addslashes(pretrim2);
	cout << "\n\n\npreAddSlashes1-serverbw2\n";
	serverbw2->toDotAscii(nosink);


}




void testIcse3(){
	int nosink = 0;
    cout << "*******************************************************\n";
    cout << "*         Patching Client against server\n\n          *\n";
    cout << "*******************************************************\n\n";

    cout << "*** FORWARD PHASE 1 --> CLIENT ***\n\n";

	//GET CLIENT
    StrangerAutomaton* clientInput1 = StrangerAutomaton::makeAnyString();
    cout << "\n\nclient input 1:\n";
	clientInput1->toDotAscii(nosink);


	StrangerAutomaton* clientSink1 = clientInput1->intersect(StrangerAutomaton::makeAnyStringL1ToL2(0, 3));

    cout << "\n\nclient sink 1:\n";
	clientSink1->toDotAscii(nosink);


    cout << "*** FORWARD PHASE 1 --> SERVER ***\n\n";

	//FORWARD1
	StrangerAutomaton* serverInput1 = StrangerAutomaton::makeAnyString();
	cout << "\n\nserver input 1:\n";
	serverInput1->toDotAscii(nosink);

	StrangerAutomaton* serverAddslashes1 = StrangerAutomaton::addslashes(serverInput1);
	cout << "\n\nserverAddslashes1:\n";
	serverAddslashes1->toDotAscii(nosink);
	StrangerAutomaton* serverSink1 = serverAddslashes1->trimSpaces();
	cout << "\n\nserverSink1:\n";
	serverSink1->toDotAscii(nosink);

    cout << "*** FORWARD PHASE 1 --> DIFFERENCE (CLIENT - SERVER) ***\n\n";

	StrangerAutomaton* clientFw1 = clientSink1->intersect(serverSink1->complement());
    cout << "\n\nclientFw1 (client - server):\n";
	clientFw1->toDotAscii(nosink);

    cout << "*** BACKWARD PHASE 1 --> CLIENT ***\n\n";
    //in client we consider input to be constrained by HTML
    StrangerAutomaton* clientInputBWImage = clientFw1->clone();
    cout << "\n\nclientInputBWImage :\n";
	clientInputBWImage->toDotAscii(nosink);
    clientInputBWImage->toDot();

}





void testEscapeCharsInMinCut(){
	int nosink = 0;
    cout << "*******************************************************\n";
    cout << "*         Patching Client against server\n\n          *\n";
    cout << "*******************************************************\n\n";

    cout << "*** FORWARD PHASE 1 --> CLIENT ***\n\n";

	//GET CLIENT
    StrangerAutomaton* clientInput1 = StrangerAutomaton::makeAnyString();
    cout << "\n\nclient input 1:\n";
	clientInput1->toDotAscii(nosink);


	StrangerAutomaton* clientSink1 = clientInput1->intersect(StrangerAutomaton::makeAnyStringL1ToL2(0, 3));

    cout << "\n\nclient sink 1:\n";
	clientSink1->toDotAscii(nosink);


    cout << "*** FORWARD PHASE 1 --> SERVER ***\n\n";

	//FORWARD1
	StrangerAutomaton* serverInput1 = StrangerAutomaton::makeAnyString();
	cout << "\n\nserver input 1:\n";
	serverInput1->toDotAscii(nosink);

    StrangerAutomaton* serverEscape = StrangerAutomaton::regExToAuto("/.*ab.*/")->complement();
	cout << "\n\nserver escape 1:\n";
	serverEscape->toDotAscii(nosink);
//    DFA* a = dfa_replace_extrabit( serverInput1->getDfa(), dfa_construct_string("abc", StrangerAutomaton::getVar(), StrangerAutomaton::getIndices()), "", StrangerAutomaton::getVar(), StrangerAutomaton::getIndices());
//    dfaPrintVerbose(a);
//    dfaPrintGraphvizAsciiRange(a, StrangerAutomaton::getVar(), StrangerAutomaton::getIndices(), 0);

    StrangerAutomaton* serverTestEscape = serverEscape->trimSpaces();
	cout << "\n\nserverSink1:\n";
	serverTestEscape->toDotAscii(nosink);



	StrangerAutomaton* serverAddslashes1 = StrangerAutomaton::addslashes(serverEscape);
	cout << "\n\nserverAddslashes1:\n";
	serverAddslashes1->toDotAscii(nosink);
	StrangerAutomaton* serverSink1 = serverAddslashes1->trimSpaces();
	cout << "\n\nserverSink1:\n";
	serverSink1->toDotAscii(nosink);

    cout << "*** FORWARD PHASE 1 --> DIFFERENCE (CLIENT - SERVER) ***\n\n";

	StrangerAutomaton* clientFw1 = clientSink1->intersect(serverSink1->complement());
    cout << "\n\nclientFw1 (client - server):\n";
	clientFw1->toDotAscii(nosink);

    cout << "*** BACKWARD PHASE 1 --> CLIENT ***\n\n";
    //in client we consider input to be constrained by HTML
    StrangerAutomaton* clientInputBWImage = clientFw1->clone();
    cout << "\n\nclientInputBWImage :\n";
	clientInputBWImage->toDotAscii(nosink);
    clientInputBWImage->toDot();
}




char *bintostr(unsigned long n, int k) {
	char *str;

	// no extra bit
	str = (char *) malloc(k + 1);
	str[k] = '\0';

	for (k--; k >= 0; k--) {
		if (n & 1)
			str[k] = '1';
		else
			str[k] = '0';
		if (n > 0)
			n >>= 1;
	}
	//printf("String:%s\n", str);
	return str;
}

void testmincut(){
//    int var = StrangerAutomaton::getVar();
//    int* indices_main = StrangerAutomaton::getIndices();
//
//    char* ltrA = bintostr('A', var);
//	char* ltrB = bintostr('B', var);
//	char* ltra = bintostr('a', var);
//	char* ltrb = bintostr('b', var);
//	char* ltrz = bintostr('z', var);
//	char* ltrSpace = bintostr(' ', var);
//
//
//    dfaSetup(15, var, indices_main);
//
//	//s0
//	dfaAllocExceptions(3);
//	dfaStoreException(1, ltrb);
//	dfaStoreException(1, ltrSpace);
//	dfaStoreException(6, ltra);
//	dfaStoreState(14);
//
//	//s1
//	dfaAllocExceptions(3);
//	dfaStoreException(2, ltrb);
//	dfaStoreException(2, ltrSpace);
//	dfaStoreException(3, ltra);
//	dfaStoreState(14);
//
//	//s2
//	dfaAllocExceptions(2);
//	dfaStoreException(4, ltra);
//	dfaStoreException(4, ltrSpace);
//	dfaStoreState(14);
//
//	//s3
//	dfaAllocExceptions(2);
//	dfaStoreException(4, ltra);
//	dfaStoreException(4, ltrSpace);
//	dfaStoreState(14);
//
//	//s4
//	dfaAllocExceptions(2);
//	dfaStoreException(5, ltra);
//	dfaStoreException(5, ltrSpace);
//	dfaStoreState(14);
//
//	//s5 - accept state 1
//	dfaAllocExceptions(2);
//	dfaStoreException(5, ltrSpace);
//	dfaStoreException(2, ltra);
//	dfaStoreState(14);
//
//	//s6
//	dfaAllocExceptions(3);
//	dfaStoreException(11, ltrb);
//	dfaStoreException(7, ltrz);
//	dfaStoreException(9, ltra);
//	dfaStoreState(14);
//
//	//s7
//	dfaAllocExceptions(2);
//	dfaStoreException(8, ltrSpace);
//	dfaStoreException(8, ltra);
//	dfaStoreState(14);
//
//	//s8
//	dfaAllocExceptions(2);
//	dfaStoreException(5, ltrSpace);
//	dfaStoreException(5, ltra);
//	dfaStoreState(14);
//
//	//s9
//	dfaAllocExceptions(2);
//	dfaStoreException(10, ltrSpace);
//	dfaStoreException(10, ltrb);
//	dfaStoreState(14);
//
//	//s10
//	dfaAllocExceptions(2);
//	dfaStoreException(13, ltrSpace);
//	dfaStoreException(9, ltra);
//	dfaStoreState(14);
//
//	//s11
//	dfaAllocExceptions(2);
//	dfaStoreException(9, ltra);
//	dfaStoreException(12, ltrb);
//	dfaStoreState(14);
//
//	//s12
//	dfaAllocExceptions(2);
//	dfaStoreException(10, ltrSpace);
//	dfaStoreException(10, ltra);
//	dfaStoreState(14);
//
//	//s13 - accept satate 2
//	dfaAllocExceptions(1);
//	dfaStoreException(5, ltrSpace);
//	dfaStoreState(14);
//
//	//s14 - sink
//	dfaAllocExceptions(0);
//	dfaStoreState(14);
//
//
//	DFA* complex = dfaBuild("0000000000000+-");
//	printf("dfa_Complex3_BeforeTrim:\n");
//	dfaPrintGraphvizAsciiRange(complex, var, indices_main, 0);
//	DFA* trimmed = dfaRightTrim(complex, ' ', var, indices_main);
//	printf("dfa_Complex3_AfterTrim:\n\n\n\n");
//	dfaPrintGraphvizAsciiRange(trimmed, var, indices_main, 0);
//    StrangerAutomaton* com = new StrangerAutomaton(complex);
////    com->mincut();
////    StrangerAutomaton* tr = new StrangerAutomaton(trimmed);
////    tr->mincut();

}


//
//void testIcse1(){
//	int nosink = 0;
//	StrangerAutomaton* clientfw = StrangerAutomaton::makeAnyStringL1ToL2(0, 3);
//	StrangerAutomaton* input = StrangerAutomaton::makeAnyString();
//	cout << "\n\n\ninput\n";
//	input->toDot();
//	input->toDotAscii(nosink);
////	StrangerAutomaton* addslashes1 = StrangerAutomaton::addslashes(input);
////	addslashes1->toDotAscii(0);
////	StrangerAutomaton* serverfw1 = addslashes1->trimSpaces();
//	StrangerAutomaton* serverfw1 = input->trimSpaces();
//	cout << "\n\n\ntrim1-serverfw1\n";
//	serverfw1->toDotAscii(nosink);
//	StrangerAutomaton* serverfw1restricted = serverfw1->restrictLengthByOtherAutomaton(clientfw);
//	cout << "\n\n\nserverfw1-len restricted\n";
//	serverfw1restricted->toDotAscii(nosink);
////	StrangerAutomaton* pretrim1 = serverfw1restricted->preTrimSpaces();
////	cout << "\n\n\npretrim1\n";
////	pretrim1->toDotAscii(nosink);
//	StrangerAutomaton* serverbw1 = serverfw1restricted->preTrimSpaces();
//	cout << "\n\n\npreTrim1-serverbw1\n";
//	serverbw1->toDotAscii(nosink);
//	/******* GET LENGTH ******/
////	StrangerAutomaton* addslashes2 = StrangerAutomaton::addslashes(serverbw1);
//	StrangerAutomaton* serverfw2 = serverbw1->trimSpaces();
//	cout << "\n\n\ntrim2- serverfw2\n";
//	serverfw2->toDotAscii(nosink);
//	StrangerAutomaton* serverfw2restricted = serverfw2->intersect(clientfw);
//	cout << "\n\n\nserverfw2restricted1\n";
//	serverfw2restricted->toDotAscii(nosink);
//	StrangerAutomaton* serverbw2 = serverfw2restricted->preTrimSpaces();
//	cout << "\n\n\npreTrim2-serverbw2\n";
//	serverbw2->toDotAscii(nosink);
////	StrangerAutomaton* serverbw2 = StrangerAutomaton::pre_addslashes(pretrim2);
//
//}

int testmaps()
{
//    DepGraph depGraph = DepGraph::parseDotFile("/Users/muath/Documents/temp/test_graph.dot");
//    cout << depGraph.toDot() << endl;
//    depGraph.sort(depGraph);
//    cout << depGraph.toDot() << endl;
//    //   // 1) Assignment using array index notation
//    //   Employees[5234] = "Mike C.";
//    //   Employees[3374] = "Charlie M.";
//    //   Employees[1923] = "David D.";
//    //   Employees[7582] = "John A.";
//    //   Employees[5328] = "Peter Q.";
//    //
//    //   cout << "Employees[3374]=" << Employees[3374] << endl << endl;
//    //
//    //   cout << "Map size: " << Employees.size() << endl;
//    //
//    //   for( map<int,string>::iterator ii=Employees.begin(); ii!=Employees.end(); ++ii)
//    //   {
//    //       cout << (*ii).first << ": " << (*ii).second << endl;
//    //   }
}

//test github





void icseExample(){
    int nosink = 0;
    cout << "*******************************************************\n";
    cout << "*         Patching Server against Client\n\n          *\n";
    cout << "*******************************************************\n\n";

    std::string re = "/(([^<>()[\\]\\\\.,;: @\"']+(\\.[^<>()[\\]\\\\.,;: @\"']+)*)|(\\\"[^\"']+\\\"))@((\\[[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\])|(([a-zA-Z0-9\\-]+\\.)+[a-zA-Z]{2,}))/";
    std::string re2 = "/[a-z0-9-]+(\\.[a-z0-9-]+)*(\\.[a-z]{2,4})/";
    int l2 = 70;
    bool DEBUG_PRINT = l2 <= 15;
    StrangerAutomaton* reAuto = StrangerAutomaton::regExToAuto(re);
    StrangerAutomaton* re2Auto = StrangerAutomaton::regExToAuto(re2);

    cout << "*** FORWARD PHASE 1 --> CLIENT ***\n\n";
	//GET CLIENT
    StrangerAutomaton* clientInput1 = StrangerAutomaton::makeAnyString();
    cout << "\n\nclient input 1:\n";
	if (DEBUG_PRINT) clientInput1->toDotAscii(nosink);


	StrangerAutomaton* clientSink1 = clientInput1->intersect(StrangerAutomaton::makeAnyStringL1ToL2(0, l2));

    cout << "\n\nclient sink 1:\n";
	if (DEBUG_PRINT) clientSink1->toDotAscii(nosink);
    cout << "\n\n\n\n\n\n\n" << endl;


    cout << "*** FORWARD PHASE 1 --> SERVER ***\n\n";
	//FORWARD1
	StrangerAutomaton* serverInput1 = StrangerAutomaton::makeAnyString();
	cout << "\n\nserver input 1:\n";
	if (DEBUG_PRINT) serverInput1->toDotAscii(nosink);
    StrangerAutomaton* serverfw1trim = serverInput1->trimSpaces();
    StrangerAutomaton* serverfw1lower = serverfw1trim->toLowerCase();
    StrangerAutomaton* serverfw1noempty = serverfw1lower->restrict(StrangerAutomaton::makeEmptyString()->complement());
    StrangerAutomaton* serverfw1l1tol2 = serverfw1noempty->restrict(StrangerAutomaton::makeAnyStringL1ToL2(0, l2));
    StrangerAutomaton* serverfw1re1 = serverfw1l1tol2->restrict(reAuto);
    StrangerAutomaton* serverfw1re1comp = serverfw1noempty->restrict(serverfw1re1->complement());
    StrangerAutomaton* serverfw1re2 = serverfw1re1comp->restrict(re2Auto);
    StrangerAutomaton* serverfw1union = serverfw1re1->union_(serverfw1re2);
    StrangerAutomaton* serverSink1 = StrangerAutomaton::addslashes(serverfw1union);


    cout << "*** FORWARD PHASE 1 --> RESTRICT SERVER LENGTH ***\n\n";
	// RESTRICT LENGTH BY CLIENT
    assert(clientSink1->isLengthFinite() == true);
	StrangerAutomaton* serverfw1Result = serverSink1->restrictLengthByOtherAutomatonFinite(clientSink1);
	cout << "\n\n\nserverfw1-len restricted\n";
	if (DEBUG_PRINT) serverfw1Result->toDotAscii(nosink);
    cout << "\n\n\n\n\n\n\n" << endl;

    cout << "*** BACKWARD PHASE 1 --> SERVER ***\n\n";
    StrangerAutomaton* serverbw1 = StrangerAutomaton::pre_addslashes(serverfw1Result);
    delete serverfw1Result;
    StrangerAutomaton* serverbw1re2 = serverbw1->intersect(serverfw1re2);
    delete serverfw1re2;
    StrangerAutomaton* serverbw1re1 = serverbw1->intersect(serverfw1re1);
    delete serverfw1re1;
    delete serverbw1;
    StrangerAutomaton* serverbw1re1comp = serverbw1re2->intersect(serverfw1re1comp);
    delete serverfw1re1comp;
    delete serverbw1re2;
    StrangerAutomaton* serverbw1l1tol2 = serverbw1re1->intersect(serverfw1l1tol2);
    delete serverfw1l1tol2;
    delete serverbw1re1;
    StrangerAutomaton* temp = serverbw1l1tol2->union_(serverbw1re1comp);
    delete serverbw1re1comp;
    delete serverbw1l1tol2;
    StrangerAutomaton* serverbw1noempty = temp->intersect(serverfw1noempty);
    delete temp;
    delete serverfw1noempty;
    StrangerAutomaton* serverbw1lower = serverbw1noempty->intersect(serverfw1lower);
    delete serverfw1lower;
    delete serverbw1noempty;
    temp = serverbw1lower->preToLowerCase();
    delete serverbw1lower;
    StrangerAutomaton* serverbw1trim = temp->intersect(serverfw1trim);
    delete serverfw1trim;
    StrangerAutomaton* serverInput2 = serverbw1trim->preTrimSpaces();
    cout << "serverInput restricting length" << endl;
    if (DEBUG_PRINT) serverInput2->toDotAscii(0);
    cout << "\n\n\n\n\n\n\n" << endl;

    //FORWARD2
    cout << "*** FORWARD PHASE 2 --> SERVER ***\n\n";
	cout << "\n\nserver input 2:\n";
	if (DEBUG_PRINT) serverInput2->toDotAscii(nosink);
    StrangerAutomaton* serverfw2trim = serverInput2->trimSpaces();
    StrangerAutomaton* serverfw2lower = serverfw2trim->toLowerCase();
    StrangerAutomaton* serverfw2noempty = serverfw2lower->restrict(StrangerAutomaton::makeEmptyString()->complement());
    StrangerAutomaton* serverfw2l1tol2 = serverfw2noempty->restrict(StrangerAutomaton::makeAnyStringL1ToL2(0, l2));
    StrangerAutomaton* serverfw2re1 = serverfw2l1tol2->restrict(reAuto);
    StrangerAutomaton* serverfw2re1comp = serverfw2noempty->restrict(serverfw2re1->complement());
    StrangerAutomaton* serverfw2re2 = serverfw2re1comp->restrict(re2Auto);
    StrangerAutomaton* serverfw2union = serverfw2re1->union_(serverfw2re2);
    StrangerAutomaton* serverSink2 = StrangerAutomaton::addslashes(serverfw2union);

    cout << "*** FORWARD PHASE 2 --> SERVER - CLIENT ***\n\n";
	// RESTRICT LENGTH BY CLIENT
	StrangerAutomaton* serverfw2Result = serverSink2->intersect(clientSink1->complement());
    delete serverSink2;
	cout << "\n\n\nserverfw2-difference\n";
	if (DEBUG_PRINT) serverfw2Result->toDotAscii(nosink);
    cout << "\n\n\n\n\n\n\n" << endl;

    cout << "*** BACKWARD PHASE 2 --> SERVER ***\n\n";
    StrangerAutomaton* serverbw2 = StrangerAutomaton::pre_addslashes(serverfw2Result);
    delete serverfw2Result;
    StrangerAutomaton* serverbw2re2 = serverbw2->intersect(serverfw2re2);
    delete serverfw2re2;
    StrangerAutomaton* serverbw2re1 = serverbw2->intersect(serverfw2re1);
    delete serverfw2re1;
    delete serverbw2;
    StrangerAutomaton* serverbw2re1comp = serverbw2re2->intersect(serverfw2re1comp);
    delete serverfw2re1comp;
    delete serverbw2re2;
    StrangerAutomaton* serverbw2l1tol2 = serverbw2re1->intersect(serverfw2l1tol2);
    delete serverfw2l1tol2;
    delete serverbw2re1;
    temp = serverbw2l1tol2->union_(serverbw2re1comp);
    delete serverbw2re1comp;
    delete serverbw2l1tol2;
    StrangerAutomaton* serverbw2noempty = temp->intersect(serverfw2noempty);
    delete serverfw2noempty;
    StrangerAutomaton* serverbw2lower = serverbw2noempty->intersect(serverfw2lower);
    delete serverfw2lower;
    delete serverbw2noempty;
    temp = serverbw2lower->preToLowerCase();
    delete serverbw2lower;
    StrangerAutomaton* serverbw2trim = temp->intersect(serverfw2trim);
    delete temp;
    delete serverfw2trim;
    temp = serverbw2trim->preTrimSpaces();
    delete serverbw2trim;
    StrangerAutomaton* serverInputFinal = temp->intersect(serverInput2);
    delete temp;
    cout << "serverInput Final" << endl;
    if (DEBUG_PRINT) serverInputFinal->toDotAscii(0);
    cout << "\n\n\n\n\n\n\n" << endl;

    cout << "*******************************************************\n";
    cout << "*         Patching Client against server\n\n          *\n";
    cout << "*******************************************************\n\n";
    assert(serverSink1->isLengthFinite() == false);
    serverSink1->toDotAscii(0);
    serverSink1->toDot();
    StrangerAutomaton* clientDiffResult = clientSink1->intersect(serverSink1->complement());
	cout << "\n\n\nclient-difference\n";
	if (DEBUG_PRINT) clientDiffResult->toDotAscii(nosink);
    //    clientDiffResult->toDot();

}


void icseSnipeGallarey(){
//    DepGraph depGraph = DepGraph::parseDotFile("/Users/muath/Documents/temp/pixy_output/SnipeGallarey/xsssink_snipeGallarey.php_2_dep.dot");
//    cout << depGraph.toDot() << endl;
//    depGraph.sort(depGraph);
//    cout << depGraph.toDot() << endl;
//    DepGraph depGraph2 = DepGraph::parseDotFile("/Users/muath/Documents/temp/pixy_output/SnipeGallareyClient/xsssink_snipeGallareyClient.php_2_dep.dot");
//    cout << depGraph2.toDot() << endl;
//    depGraph2.sort(depGraph2);
//    cout << depGraph2.toDot() << endl;
//    ForwardImageComputer::staticInit();
//    StringAnalyzer stringAnalyzer("/Users/muath/Documents/temp/pixy_output/SnipeGallarey/xsssink_snipeGallarey.php_2_dep.dot", "/Users/muath/Documents/temp/pixy_output/SnipeGallareyClient/xsssink_snipeGallareyClient.php_2_dep.dot", "Diff Analysis");
//    stringAnalyzer.analyze();

}

void testReplaceHeavy(){
//    char monaCharacter[] = "XXXXXXXX";
//    char *charachters[] = {"01011100", "00101100", "00100010"};
    int numOfChars = 2;
//    int var = StrangerAutomaton::getVar();
//    int maxExceptions = 1 << var;
//    char** result = (char **) mem_alloc(maxExceptions * sizeof(char *));
//    int size;
//    removeTransitionOnChars(monaCharacter, charachters, numOfChars, var, result, &size);
//    int i = 0;
//    for (i = 0; i < size; i++){
//        printf("result[%d] = %s\n", i, result[i]);
//    }

    StrangerAutomaton* a = StrangerAutomaton::makeAnyStringL1ToL2(0, 10);
    std::string re = "/(([^<>()[\\]\\\\.,;: @\"']+(\\.[^<>()[\\]\\\\.,;: @\"']+)*)|(\\\"[^\"']+\\\"))@((\\[[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\])|(([a-zA-Z0-9\\-]+\\.)+[a-zA-Z]{2,}))/";
    StrangerAutomaton* reAuto = StrangerAutomaton::regExToAuto(re);
    a = a->intersect(reAuto);
    assert(a->isLengthFinite());
    cout << "Number of states = " << a->dfa->ns << endl;
    a = a->trimSpaces();
    cout << "trimmed";
    char escapeChars[] = {'\'', '"'};
    a->toDotAscii(0);
    a->dfa = dfa_escape(a->dfa, StrangerAutomaton::getVar(), StrangerAutomaton::getIndices(), '\\', escapeChars, numOfChars);


}


int myPow(int x, int p)
{
    if (p == 0) return 1;
    if (p == 1) return x;

    int tmp = myPow(x, p/2);
    if (p%2 == 0) return tmp * tmp;
    else return x * tmp * tmp;
}

void testProjection(){
    std::clock_t start;
    double duration;

    start = std::clock();

    StrangerAutomaton* a = StrangerAutomaton::makeAnyStringL1ToL2(0, 10000);
    cout << "start testing project" << endl;
    test_project(a->dfa, StrangerAutomaton::getVar(), StrangerAutomaton::getIndices(), 16, 0);
    cout << "end testing project" << endl;


    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;

    std::cout<<"printf: "<< duration <<'\n';
}

void testProjectExpBin(){
    //    StrangerAutomaton* a = StrangerAutomaton::regExToAuto("/.*a/")->concatenate(StrangerAutomaton::makeAnyStringL1ToL2(0, 16));
    //    a->toDotAscii(0);
    DFA *b;
    std::clock_t start;
    double duration;

    int s = 32;
    cout << "Testing project on the following language over the alphabet {0,1} L= (0|1)*1(0|1){n-1} for some integer n (at least n characters where the nth character from the left is 1)" << endl;
    cout << "n = " << s-2 << ", Expected number of states = 2^" << s-2 << " = " << (myPow(2,(s-2))) << "\n\n" << endl;
    //    cout << "************* Starting the test ***************\n\n" << endl;
    cout << "************************************" << endl;
    cout << "*   Starting first construction:   *" << endl;
    cout << "************************************" << endl;
    cout << "0 -> 0 {0X, 10}" << endl;
    cout << "0 -> 1 {11}" << endl;
    cout << "i -> i+1 {XX}   => for n+1 > i > 0" << endl;
    cout << "n+1 -> n+1 {XX}" << "\n" << endl;

    start = std::clock();

    char *statuces = new char[s];
    dfaSetup(s, 2, allocateArbitraryIndex(2));
    dfaAllocExceptions(2);
    dfaStoreException(0, "0X");
    dfaStoreException(0, "10");
    dfaStoreState(1);
    statuces[0] = '-';
    //    dfaAllocExceptions(0);
    //    dfaStoreState(2);
    //    statuces[1] = '-';
    int i;
    for (i = 1; i < s-1; i++ ){
        dfaAllocExceptions(0);
        dfaStoreState(i+1);
        statuces[i] = '-';
    }
    dfaAllocExceptions(0);
    dfaStoreState(s-1);
    statuces[s-1] = '-';
    statuces[s-2] = '+';
    DFA *a = dfaBuild(statuces);

    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    std::cout<<"NFA constructed with a number of states = " << s << ".\nTime to construct NFA: "<< duration <<"\n" << endl;

    start = std::clock();
    dfaPrintGraphviz(a, 2, StrangerAutomaton::getUnsignedIndices(2));

    b = dfaProject(a, 1);
    dfaFree(a);

    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;

    std::cout<<"Project time: "<< duration << "s" << endl;
    cout << "After project: number of states = " << b->ns << "\n" << endl;

    start = std::clock();

    a = dfaMinimize(b);
    dfaPrintGraphviz(a, 1, StrangerAutomaton::getUnsignedIndices(1));


    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    std::cout<<"Minimize time: "<< duration << "s" << endl;
    cout << "After minimize final number of states = " << a->ns << "\n\n\n" << endl;
    dfaFree(b);
    dfaFree(a);

    cout << "************************************" << endl;
    cout << "*   Start second construction:     *" << endl;
    cout << "************************************" << endl;

    //    dfaPrintGraphviz(a, 1, StrangerAutomaton::getUnsignedIndices(1));
    //    dfaPrintVerbose(a);

    cout << "0 -> 0 {00, 10}" << endl;
    cout << "0 -> n+1 {01}" << endl;
    cout << "0 -> 1 {11}" << endl;
    cout << "i -> i+1 {X0}   => for n+1 > i > 0" << endl;
    cout << "i -> n+1 {X1}  => for n+1 > i > 0" << endl;
    cout << "n+1 -> n+1 {XX}" << "\n" << endl;

    start = std::clock();

    statuces = new char[s];
    dfaSetup(s, 2, allocateArbitraryIndex(2));
    dfaAllocExceptions(3);
    dfaStoreException(0, "00");
    dfaStoreException(0, "10");
    dfaStoreException(1, "11");
    dfaStoreState(s-1);
    statuces[0] = '-';
    //    dfaAllocExceptions(0);
    //    dfaStoreState(2);
    //    statuces[1] = '-';
    for (i = 1; i < s-1; i++ ){
        dfaAllocExceptions(1);
        dfaStoreException(i+1, "X0");
        dfaStoreState(s-1);
        statuces[i] = '-';
    }
    dfaAllocExceptions(0);
    dfaStoreState(s-1);
    statuces[s-1] = '-';
    statuces[s-2] = '+';
    a = dfaBuild(statuces);

    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    std::cout<<"NFA constructed with a number of states = " << s << ".\nTime to construct NFA: "<< duration << "s" <<"\n" << endl;

    start = std::clock();

    b = dfaProject(a, 1);
    dfaFree(a);

    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;

    std::cout<<"Project time: "<< duration << "s" << endl;
    cout << "After project: number of states = " << b->ns << "\n\n" << endl;

    start = std::clock();

    a = dfaMinimize(b);
    dfaFree(b);
    dfaFree(a);

    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    std::cout<<"Minimize time: "<< duration << "s" << endl;
    cout << "After minimize final number of states = " << a->ns << "\n\n\n\n\n" << endl;

}


void testProjectExp(){
//    StrangerAutomaton* a = StrangerAutomaton::regExToAuto("/.*a/")->concatenate(StrangerAutomaton::makeAnyStringL1ToL2(0, 16));
//    a->toDotAscii(0);
    DFA *b;
    std::clock_t start;
    double duration;

    int s = 12;
    cout << "Testing project on the following language over the alphabet {0,1} L= (0|1)*1(0|1){n-1} for some integer n (at least n characters where the nth character from the left is 1)" << endl;
    cout << "n = " << s-2 << ", Expected number of states = 2^" << s-2 << " = " << (myPow(2,(s-2))) << "\n\n" << endl;
//    cout << "************* Starting the test ***************\n\n" << endl;
    cout << "************************************" << endl;
    cout << "*   Starting first construction:   *" << endl;
    cout << "************************************" << endl;
    cout << "0 -> 0 {0X, 10}" << endl;
    cout << "0 -> 1 {11}" << endl;
    cout << "i -> i+1 {XX}   => for n+1 > i > 0" << endl;
    cout << "n+1 -> n+1 {XX}" << "\n" << endl;

    start = std::clock();

    char *statuces = new char[s];
    dfaSetup(s, 9, allocateArbitraryIndex(9));
    dfaAllocExceptions(9);
    dfaStoreException(0, "000000000");
    dfaStoreException(0, "1XXXXXX0X");
    dfaStoreException(0, "01XXXXX0X");
    dfaStoreException(0, "001XXXX0X");
    dfaStoreException(0, "0001XXX0X");
    dfaStoreException(0, "00001XX0X");
    dfaStoreException(0, "000001X0X");
    dfaStoreException(0, "00000010X");
    dfaStoreException(0, "XXXXXXX1X");
    dfaStoreState(1);
    statuces[0] = '-';
    int i;
    for (i = 1; i < s-1; i++ ){
        dfaAllocExceptions(0);
        dfaStoreState(i+1);
        statuces[i] = '-';
    }
    dfaAllocExceptions(0);
    dfaStoreState(s-1);
    statuces[s-1] = '-';
    statuces[s-2] = '+';
    DFA *a = dfaBuild(statuces);

    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    std::cout<<"NFA constructed with a number of states = " << s << ".\nTime to construct NFA: "<< duration <<"\n" << endl;

    start = std::clock();

    b = dfaProject(a, 8);
//    dfaFree(a);

    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;

    std::cout<<"Project time: "<< duration << "s" << endl;
    cout << "After project: number of states = " << b->ns << "\n" << endl;

    start = std::clock();

    DFA *c = dfaMinimize(b);
//    dfaPrintGraphvizAsciiRange(c, 8, allocateArbitraryIndex(8), 0);
//    dfaFree(b);
//    dfaFree(a);

    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    std::cout<<"Minimize time: "<< duration << "s" << endl;
    cout << "After minimize final number of states = " << c->ns << "\n\n\n" << endl;

    cout << "************************************" << endl;
    cout << "*   Start second construction:     *" << endl;
    cout << "************************************" << endl;

//    dfaPrintGraphviz(a, 1, StrangerAutomaton::getUnsignedIndices(1));
//    dfaPrintVerbose(a);

    cout << "0 -> 0 {00, 10}" << endl;
    cout << "0 -> n+1 {01}" << endl;
    cout << "0 -> 1 {11}" << endl;
    cout << "i -> i+1 {X0}   => for n+1 > i > 0" << endl;
    cout << "i -> n+1 {X1}  => for n+1 > i > 0" << endl;
    cout << "n+1 -> n+1 {XX}" << "\n" << endl;

    start = std::clock();

    statuces = new char[s];
    dfaSetup(s, 9, allocateArbitraryIndex(9));
    dfaAllocExceptions(10);
    dfaStoreException(0, "XXXXXXX10");
    dfaStoreException(0, "000000000");
    dfaStoreException(1, "000000001");
    dfaStoreException(0, "1XXXXXX00");
    dfaStoreException(0, "01XXXXX00");
    dfaStoreException(0, "001XXXX00");
    dfaStoreException(0, "0001XXX00");
    dfaStoreException(0, "00001XX00");
    dfaStoreException(0, "000001X00");
    dfaStoreException(0, "000000100");
    dfaStoreState(s-1);
    statuces[0] = '-';
    //    dfaAllocExceptions(0);
    //    dfaStoreState(2);
    //    statuces[1] = '-';
    for (i = 1; i < s-1; i++ ){
        dfaAllocExceptions(1);
        dfaStoreException(i+1, "XXXXXXXX0");
        dfaStoreState(s-1);
        statuces[i] = '-';
    }
    dfaAllocExceptions(0);
    dfaStoreState(s-1);
    statuces[s-1] = '-';
    statuces[s-2] = '+';
    DFA *a2 = dfaBuild(statuces);

    DFA *p1 = dfaProduct(a, a2, dfaIMPL);
    DFA *p2 = dfaProduct(a2, a, dfaIMPL);
    DFA *inter = dfaProduct(p1, p2, dfaAND);
    dfaNegation(inter);
    char *result = dfaMakeExample(inter, 1, 9, StrangerAutomaton::getUnsignedIndices(9));
//    assert(result == NULL);

    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    std::cout<<"NFA constructed with a number of states = " << s << ".\nTime to construct NFA: "<< duration << "s" <<"\n" << endl;

    start = std::clock();

    DFA *b2 = dfaProject(a2, 8);
//    dfaFree(a2);

    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;

    std::cout<<"Project time: "<< duration << "s" << endl;
    cout << "After project: number of states = " << b2->ns << "\n\n" << endl;

    start = std::clock();

    DFA *c2 = dfaMinimize(b2);
//    dfaPrintGraphvizAsciiRange(c2, 8, allocateArbitraryIndex(8), 0);
//    dfaFree(b2);
//    dfaFree(a2);

    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    std::cout<<"Minimize time: "<< duration << "s" << endl;
    cout << "After minimize final number of states = " << c2->ns << "\n\n\n\n\n" << endl;

}




void testTransitionRelation(){
//    StrangerAutomaton* a = StrangerAutomaton::makeAnyString();
//        StrangerAutomaton* a = StrangerAutomaton::makeString("abcde");
//    StrangerAutomaton* b = StrangerAutomaton::makeString("abcxy");

    StrangerAutomaton* a = StrangerAutomaton::makeAnyStringL1ToL2(0, 100);
//    cout << "done";
    std::string re = "/(([^<>()[\\]\\\\.,;: @\"']+(\\.[^<>()[\\]\\\\.,;: @\"']+)*)|(\\\"[^\"']+\\\"))@((\\[[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\])|(([a-zA-Z0-9\\-]+\\.)+[a-zA-Z]{2,}))/";
    StrangerAutomaton* reAuto = StrangerAutomaton::regExToAuto(re);
//    a = a->union_(b);
//    StrangerAutomaton* e = StrangerAutomaton::makeString("end");
//    a = a->concatenate(e);
    a = a->intersect(reAuto);
//    a->toDotAscii(0);
//    cout << "\n" << ( isLengthFiniteTarjan(a->dfa) ? "length finite" : "length infinite") << endl;
    if (isLengthFiniteTarjan(a->dfa,  StrangerAutomaton::getVar(), StrangerAutomaton::getIndices())){
        cout << "tarjan done\n";
        int i;
        cout << "\n\n";
        P_DFAFiniteLengths pDFAFiniteLengths = dfaGetLengthsFiniteLang(a->dfa,  StrangerAutomaton::getVar(), StrangerAutomaton::getIndices());
        cout << "\n\nPossible lengths for the language: ";
        for (i = 0; i < pDFAFiniteLengths->size; i++){
            if (i == 0)
                cout << pDFAFiniteLengths->lengths[i];
            else
                cout << ", " << pDFAFiniteLengths->lengths[i];
        }
        cout << endl;

        DFA *b = dfaSigmaLengthsSet(pDFAFiniteLengths->lengths, pDFAFiniteLengths->size, false, StrangerAutomaton::getVar(), StrangerAutomaton::getIndices());
        dfaPrintGraphvizAsciiRange(b, StrangerAutomaton::getVar(), StrangerAutomaton::getIndices(), 2);
    }
//    pTransitionRelation p_transitionRelation = dfaGetTransitionRelation(a->dfa);
//    dfaPrintTransitionRelation(p_transitionRelation);
//    p_transitionRelation = dfaGetReverseTransitionRelation(a->dfa);
//    dfaPrintTransitionRelation(p_transitionRelation);
//    bdd_update_statistics(a->dfa->bddm, 0);
//    bdd_print_statistics(0, "bddm");
//    dfaPrintBDD(a->dfa, 0, NUM_ASCII_TRACKS);

}

void testRegexToAuto() {
//	StrangerAutomaton* t1 = StrangerAutomaton::regExToAuto("/.{0,60}/",true,1);
//
//	cout << endl << endl;
//	t1->toDot();
//	cout << endl << endl;
//
//	StrangerAutomaton* t2 = StrangerAutomaton::regExToAuto("/.*(.+).*/", true, 2);
//
//	cout << endl << endl;
//	t2->toDot();
//	cout << endl << endl;

	StrangerAutomaton* t3 = StrangerAutomaton::regExToAuto("/a{0,3}/", true, 3);

	cout << endl << endl;
	t3->toDot();
	cout << endl << endl;

	StrangerAutomaton* t4 = StrangerAutomaton::regExToAuto("/a{1,3}/", true, 4);

	cout << endl << endl;
	t4->toDot();
	cout << endl << endl;
}

void call_patcher(string patcher_name, string patchee_name, string field_name){
	try {
		StrangerPatcher strangerPatcher(patcher_name, patchee_name, field_name);
		strangerPatcher.extractValidationPatch();
		strangerPatcher.extractSanitizationPatch();

		cout << endl << "\t------ OVERALL RESULT for: " << field_name << " ------" << endl;
		if (strangerPatcher.is_validation_patch_required) {
			cout << "\t    - validation patch is generated" << endl;
			//		cout << endl << endl;
//					strangerPatcher.getValidationPatchAuto()->toDotAscii(0);
		} else {
			cout << "\t    - no validation patch" << endl;
		}

		if (strangerPatcher.is_sanitization_patch_required) {
			cout << "\t    - sanitization patch is generated" << endl;
			//		cout << endl << endl;
					//strangerPatcher.getSanitizationPatchAuto()->toDotAscii(0);
			strangerPatcher.getSanitizationPatchAuto()->toDot();
		} else {
			cout << "\t    - no sanitization patch" << endl;
		}

	} catch (StrangerStringAnalysisException const &e) {
		cerr << e.what();
		exit(EXIT_FAILURE);
	}

}

void patch_paperexample_email(bool reversed) {
	string field_name = "email";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/paperexample/patchee.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/paperexample/patcher.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

/****************************************************************************************************/
/*********** SNIPE GALLERY *********************************************/
/****************************************************************************************************/

void patch_snipe_frame__form_frame_name(bool reversed) {
	string field_name = "form_frame_name";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/snipegallery/snipe_frame.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/snipegallery/snipe_frame__form_frame_name.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}

}

void patch_snipe_local__form_author(bool reversed) {
	string field_name = "form_author";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/snipegallery/snipe_local.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/snipegallery/snipe_local__form_author.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_snipe_local__form_details(bool reversed) {
	string field_name = "form_details";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/snipegallery/snipe_local.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/snipegallery/snipe_local__form_details.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_snipe_local__form_keywords(bool reversed) {
	string field_name = "form_keywords";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/snipegallery/snipe_local.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/snipegallery/snipe_local__form_keywords.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_snipe_local__form_location(bool reversed) {
	string field_name = "form_location";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/snipegallery/snipe_local.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/snipegallery/snipe_local__form_location.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_snipe_local__form_publish(bool reversed) {
	string field_name = "form_publish";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/snipegallery/snipe_local.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/snipegallery/snipe_local__form_publish.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_snipe_local__gallery_id(bool reversed) {
	string field_name = "gallery_id";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/snipegallery/snipe_local.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/snipegallery/snipe_local__gallery_id.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

//gallery_115 corresponds to new gallery form
void patch_snipe_gallery_115__cat_parent(bool reversed) {
	string field_name = "cat_parent";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/snipegallery/snipe_gallery_115.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/snipegallery/snipe_gallery__cat_parent.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_snipe_gallery_115__form_cat_description(bool reversed) {
	string field_name = "form_cat_description";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/snipegallery/snipe_gallery_115.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/snipegallery/snipe_gallery__form_cat_description.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_snipe_gallery_115__form_gallery_name(bool reversed) {
	string field_name = "form_gallery_name";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/snipegallery/snipe_gallery_115.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/snipegallery/snipe_gallery__form_gallery_name.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

// gallery_41 corresponds to edit gallery form
void patch_snipe_gallery_41__cat_parent(bool reversed) {
	string field_name = "cat_parent";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/snipegallery/snipe_gallery_41.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/snipegallery/snipe_gallery__cat_parent.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_snipe_gallery_41__form_cat_description(bool reversed) {
	string field_name = "form_cat_description";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/snipegallery/snipe_gallery_41.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/snipegallery/snipe_gallery__form_cat_description.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_snipe_gallery_41__form_gallery_name(bool reversed) {
	string field_name = "form_gallery_name";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/snipegallery/snipe_gallery_41.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/snipegallery/snipe_gallery__form_gallery_name.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_snipe_gallery_41__form_frame_id(bool reversed) {
	string field_name = "form_frame_id";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/snipegallery/snipe_gallery_41.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/snipegallery/snipe_gallery__form_frame_id.dot";

		if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_snipe_gallery_41__form_orderby(bool reversed) {
	string field_name = "form_orderby";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/snipegallery/snipe_gallery_41.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/snipegallery/snipe_gallery__form_orderby.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_snipe_gallery_41__form_order(bool reversed) {
	string field_name = "form_order";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/snipegallery/snipe_gallery_41.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/snipegallery/snipe_gallery__form_order.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_snipe_gallery_41__form_watermark_txt(bool reversed) {
	string field_name = "form_watermark_txt";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/snipegallery/snipe_gallery_41.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/snipegallery/snipe_gallery__form_watermark_txt.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_snipe_gallery_41__gallery_id(bool reversed) {
	string field_name = "gallery_id";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/snipegallery/snipe_gallery_41.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/snipegallery/snipe_gallery__gallery_id.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

/****************************************************************************************************/
/*********** PHPNEWS ********************************************************************************/
/****************************************************************************************************/

void patch_phpnews_auth_89__user(bool reversed) {
	string field_name = "user";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/phpnews/phpnews_auth_89_user_password.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/phpnews/phpnews_auth_89__user.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_phpnews_auth_89__password(bool reversed) {
	string field_name = "password";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/phpnews/phpnews_auth_89_user_password.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/phpnews/phpnews_auth_89__password.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_phpnews_admin_post2_1527__subject(bool reversed) {
	string field_name = "subject";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/phpnews/phpnews_admin_post2_1527_subject.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/phpnews/phpnews_admin_post__subject.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_phpnews_admin_modifypost3_2382__subject(bool reversed) {
	string field_name = "subject";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/phpnews/phpnews_admin_modifypost3_2382_subject.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/phpnews/phpnews_admin_post__subject.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_phpnews_admin_newsposter2_1780__username(bool reversed) {
	string field_name = "username";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/phpnews/phpnews_admin_newsposter2_1780_email_username.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/phpnews/phpnews_admin_poster__username.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_phpnews_admin_newsposter2_1780__email(bool reversed) {
	string field_name = "email";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/phpnews/phpnews_admin_newsposter2_1780_email_username.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/phpnews/phpnews_admin_poster__email.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_phpnews_admin_modifynewsposter3_3091__email(bool reversed) {
	string field_name = "email";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/phpnews/phpnews_admin_modifynewsposter3_3091_email.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/phpnews/phpnews_admin_poster__email.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

/****************************************************************************************************/
/*********** SCHOOLMATE *****************************************************************************/
/****************************************************************************************************/

void patch_schoolmate_header__schoolname(bool reversed) {
	string field_name = "schoolname";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_header.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_header__schoolname.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_schoolmate_header__schooladdress(bool reversed) {
	string field_name = "schooladdress";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_header.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_header__schooladdress.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_schoolmate_header__schoolphone(bool reversed) {
	string field_name = "schoolphone";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_header.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_header__schoolphone.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_schoolmate_header__sitemessage(bool reversed) {
	string field_name = "sitemessage";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_header.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_header__sitemessage.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_schoolmate_header__sitetext(bool reversed) {
	string field_name = "sitetext";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_header.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_header__sitetext.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_schoolmate_header__numsemesters(bool reversed) {
	string field_name = "numsemesters";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_header.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_header__numsemesters.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_schoolmate_header__numperiods(bool reversed) {
	string field_name = "numsemesters";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_header.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_header__numperiods.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_schoolmate_header__apoint(bool reversed) {
	string field_name = "apoint";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_header.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_header__apoint.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}
void patch_schoolmate_header__bpoint(bool reversed) {
	string field_name = "bpoint";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_header.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_header__bpoint.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}
void patch_schoolmate_header__cpoint(bool reversed) {
	string field_name = "cpoint";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_header.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_header__cpoint.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}
void patch_schoolmate_header__dpoint(bool reversed) {
	string field_name = "dpoint";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_header.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_header__dpoint.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}
void patch_schoolmate_header__fpoint(bool reversed) {
	string field_name = "fpoint";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_header.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_header__fpoint.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_schoolmate_announcements_9__title(bool reversed) {
	string field_name = "title";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageannouncements_9.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageannouncements__title.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_schoolmate_announcements_9__message(bool reversed) {
	string field_name = "message";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageannouncements_9.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageannouncements__message.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_schoolmate_announcements_18__title(bool reversed) {
	string field_name = "title";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageannouncements_18.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageannouncements__title.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}
void patch_schoolmate_announcements_18__message(bool reversed) {
	string field_name = "message";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageannouncements_18.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageannouncements__message.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_schoolmate_announcements_18__announcementid(bool reversed) {
	string field_name = "announcementid";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageannouncements_18.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageannouncements__announcementid.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_schoolmate_manageclasses_25__semester(bool reversed) {
	string field_name = "semester";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageclasses_25.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageclasses__semester.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_schoolmate_manageclasses_28__semester(bool reversed) {
	string field_name = "semester";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageclasses_28.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageclasses__semester.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_schoolmate_manageclasses_28__periodnum(bool reversed) {
	string field_name = "periodnum";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageclasses_28.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageclasses__periodnum.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_schoolmate_manageclasses_28__roomnum(bool reversed) {
	string field_name = "roomnum";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageclasses_28.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageclasses__roomnum.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_schoolmate_manageclasses_28__sectionnum(bool reversed) {
	string field_name = "sectionnum";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageclasses_28.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageclasses__sectionnum.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_schoolmate_manageclasses_28__substitute(bool reversed) {
	string field_name = "substitute";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageclasses_28.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageclasses__substitute.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_schoolmate_manageclasses_28__teacher(bool reversed) {
	string field_name = "teacher";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageclasses_28.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageclasses__teacher.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_schoolmate_manageclasses_28__title(bool reversed) {
	string field_name = "title";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageclasses_28.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageclasses__title.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_schoolmate_manageclasses_83__semester(bool reversed) {
	string field_name = "semester";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageclasses_83.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageclasses__semester.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_schoolmate_manageclasses_83__periodnum(bool reversed) {
	string field_name = "periodnum";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageclasses_83.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageclasses__periodnum.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_schoolmate_manageclasses_83__roomnum(bool reversed) {
	string field_name = "roomnum";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageclasses_83.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageclasses__roomnum.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_schoolmate_manageclasses_83__sectionnum(bool reversed) {
	string field_name = "sectionnum";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageclasses_83.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageclasses__sectionnum.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_schoolmate_manageclasses_83__substitute(bool reversed) {
	string field_name = "substitute";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageclasses_83.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageclasses__substitute.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_schoolmate_manageclasses_83__teacher(bool reversed) {
	string field_name = "teacher";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageclasses_83.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageclasses__teacher.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_schoolmate_manageclasses_83__title(bool reversed) {
	string field_name = "title";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageclasses_83.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageclasses__title.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_schoolmate_manageclasses_83__courseid(bool reversed) {
	string field_name = "courseid";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageclasses_83.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageclasses__courseid.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_schoolmate_manageparents_36__fname(bool reversed) {
	string field_name = "fname";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageparents_36.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageparents__fname.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_schoolmate_manageparents_36__lname(bool reversed) {
	string field_name = "lname";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageparents_36.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageparents__lname.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_schoolmate_manageparents_36__student(bool reversed) {
	string field_name = "student";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageparents_36.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageparents__student.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_schoolmate_manageparents_36__username(bool reversed) {
	string field_name = "username";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageparents_36.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageparents__username.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_schoolmate_manageparents_58__fname(bool reversed) {
	string field_name = "fname";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageparents_58.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageparents__fname.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_schoolmate_manageparents_58__lname(bool reversed) {
	string field_name = "lname";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageparents_58.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageparents__lname.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_schoolmate_manageparents_58__student(bool reversed) {
	string field_name = "student";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageparents_58.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageparents__student.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_schoolmate_manageparents_58__username(bool reversed) {
	string field_name = "username";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageparents_58.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageparents__username.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_schoolmate_manageparents_58__parentid(bool reversed) {
	string field_name = "parentid";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageparents_58.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageparents__parentid.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

int main(int argc, char *argv[]) {

/***********	Client - Server Patching	***********/
	bool patchClient = false;
//	patch_snipe_frame__form_frame_name(patchClient);
//	patch_snipe_local__form_author(patchClient);
//	patch_snipe_local__form_details(patchClient);
//	patch_snipe_local__form_keywords(patchClient);
//	patch_snipe_local__form_location(patchClient);
//	patch_snipe_local__form_publish(patchClient);
//	patch_snipe_local__gallery_id(patchClient);
//	patch_snipe_gallery_115__cat_parent(patchClient);
//	patch_snipe_gallery_115__form_cat_description(patchClient);
//	patch_snipe_gallery_115__form_gallery_name(patchClient);
//	patch_snipe_gallery_41__cat_parent(patchClient);
//	patch_snipe_gallery_41__form_cat_description(patchClient);
//	patch_snipe_gallery_41__form_gallery_name(patchClient);
//	patch_snipe_gallery_41__form_frame_id(patchClient);
//	patch_snipe_gallery_41__form_orderby(patchClient);
//	patch_snipe_gallery_41__form_order(patchClient);
//	patch_snipe_gallery_41__form_watermark_txt(patchClient);
//	patch_snipe_gallery_41__gallery_id(patchClient);

//	patch_phpnews_auth_89__user(patchClient);
//	patch_phpnews_auth_89__password(patchClient);
//	patch_phpnews_admin_post2_1527__subject(patchClient);
//	patch_phpnews_admin_modifypost3_2382__subject(patchClient);
//	patch_phpnews_admin_newsposter2_1780__username(patchClient);
//	patch_phpnews_admin_newsposter2_1780__email(patchClient);
//	patch_phpnews_admin_modifynewsposter3_3091__email(patchClient);

	patch_paperexample_email(false);

	return 0;
}
