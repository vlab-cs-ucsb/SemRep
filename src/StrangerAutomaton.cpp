// Generated from /testJ2c/src/StrangerAutomaton.java
#include "StrangerAutomaton.hpp"

//#include "RegExp.hpp"
#include "StrangerAutomatonException.hpp"

using namespace std;

struct stringbuilder
{
    std::stringstream ss;
    template<typename T>
    stringbuilder & operator << (const T &data)
    {
        ss << data;
        return *this;
    }
    operator std::string() { return ss.str(); }
};

StrangerAutomaton::StrangerAutomaton(DFA* dfa)
{
	init();
	this->dfa = dfa;
}

StrangerAutomaton::StrangerAutomaton()
{
	init();
}
void StrangerAutomaton::init()
{
    top = false;
    bottom = false;
    this->ID = -1;
    this->autoTraceID = traceID++;
}

StrangerAutomaton::~StrangerAutomaton()
{
	if (this->dfa != NULL ){
		dfaFree(this->dfa);
		this->dfa = NULL;
	}
	else
		abort();
}

void StrangerAutomaton::staticInit() {
    
}

// some static members
int StrangerAutomaton::num_ascii_track = 8;
int* StrangerAutomaton::indices_main = allocateAscIIIndexWithExtraBit(num_ascii_track);
unsigned* StrangerAutomaton::u_indices_main = getUnsignedIndices(num_ascii_track);



int StrangerAutomaton::getID()
{
    return ID;
}

void StrangerAutomaton::setID(int id)
{
    ID = id;
}


char StrangerAutomaton::slash = '/';

bool StrangerAutomaton::coarseWidening = false;

PerfInfo StrangerAutomaton::perfInfo;


DFA* StrangerAutomaton::getDfa()
{
    return this->dfa;
}

StrangerAutomaton* StrangerAutomaton::clone(int id)
{
	debug(stringbuilder() << id << " = clone(" << this->ID << ")");
	if (isBottom())
		return makeBottom(id);
	else if (isTop())
		return makeTop(id);
	else {
		debugToFile(stringbuilder() << "M[" << traceID << "] = dfaCopy(M["  << this->autoTraceID << "]);//" << id << " = clone(" << this->ID << ")");
		StrangerAutomaton* retMe = new StrangerAutomaton(dfaCopy(this->dfa));
		{
			retMe->setID(id);
			retMe->debugAutomaton();
		}
		return retMe;
	}
}

StrangerAutomaton* StrangerAutomaton::clone()
{
    return this->clone(-1);
}



/**
 * Creates a new automaton that holds the bottom value of the String Analysis lattice.
 * (bottom < phi). This artificial bottom is used to make things faster as we do not
 * need to create a native c library DFA which holds language phi - the empty language.
 * If you need to actually make the language phi then refer to {@link makeAnyString}
 * and {@link complement}.
 * This method should be used for:
 * 1- Variables of type string if they are uninitialized and the
 * semantics of the language consider them to hold an unknown or null value.
 * 2- Variables with unknown type in dynamically typed languages. In this case as soon
 * as type is detected then we should use another factory method.
 * @param id
 *            : id of node associated with this auto; used for debugging
 *            purposes only
 */
StrangerAutomaton* StrangerAutomaton::makeBottom(int id)
{
    
	debug(stringbuilder() << id << " = makeBottom()");
	StrangerAutomaton* retMe =  new StrangerAutomaton(NULL);
    
	{
		retMe->setID(id);
	}
	retMe->bottom = true;
	return retMe;
}

/**
 * Creates a new automaton that holds the bottom value of the String Analysis lattice.
 * (bottom < phi). This artificial bottom is used to make things faster as we do not
 * need to create a native c library DFA which holds language phi - the empty language.
 * If you need to actually make the language phi then refer to {@link makeAnyString}
 * and {@link complement}.
 * This method should be used for:
 * 1- Variables of type string if they are uninitialized and the
 * semantics of the language consider them to hold an unknown or null value.
 * 2- Variables with unknown type in dynamically typed languages. In this case as soon
 * as type is detected then we should use another factory method.
 */
StrangerAutomaton* StrangerAutomaton::makeBottom()
{
    
    return makeBottom(traceID);
}

/**
 * Creates a new automaton that holds the top value of the String Analysis lattice
 * which is undefined (undefined > Sigma*).
 * It is used for variables in dynamically typed languages which may change their type from
 * a string type to another nonstring type.
 * If you need to create Sigma* (the actual top) then refer to {@link makeAnyString}.
 * Joining a variable x of type string with itself should yield top
 * only if the other copy of the variable has a different type (hold top
 * value).
 * If a variable is declared but with no value then it should be bottom.
 * @param id
 *            : id of node associated with this auto; used for debugging
 *            purposes only
 */
StrangerAutomaton* StrangerAutomaton::makeTop(int id)
{
    
	debug(stringbuilder() << id << " = makeTop()");
	StrangerAutomaton* retMe =  new StrangerAutomaton(NULL);
	retMe->top = true;
    
	{
		retMe->setID(id);
	}
	return retMe;
}

/**
 * Creates a new automaton that holds the top value of the String Analysis lattice
 * which is undefined (undefined > Sigma*).
 * It is used for variables in dynamically typed languages which may change their type from
 * a string type to another nonstring type.
 * If you need to create Sigma* (the actual top) then refer to {@link makeAnyString}.
 * Joining a variable x of type string with itself should yield top
 * only if the other copy of the variable has a different type (hold top
 * value).
 * If a variable is declared but with no value then it should be bottom.
 */
StrangerAutomaton* StrangerAutomaton::makeTop()
{
    
    return makeTop(traceID);
}

/**
 * Creates an automaton that accepts exactly the given string. It also
 * assigns this string to StrangerAutomaton.str. If parameter s is empty
 * string then it will call StrangerAutomaton.makeEmptyString
 * @param id
 *            : id of node associated with this auto; used for debugging
 *            purposes only
 */
StrangerAutomaton* StrangerAutomaton::makeString(std::string s, int id)
{
    
	debug(stringbuilder() << id << " = makeString(" << s << ")");
    
	StrangerAutomaton* retMe;
	// We need to set the string explicitly because the current way we deal
	// with replace is with a string parameter for replace instead of an
	// automaton. Until we have a replace function with the replace
	// parameter
	// as a string we will keep this->
    
	// if the string is empty then make sure you generate an empty string
	// automaton
	// cause empty string needs special treatment
	if (s.empty()) {
		return StrangerAutomaton::makeEmptyString(id);
	} else {
        
		debugToFile(stringbuilder() << "M[" << traceID << "] = dfa_construct_string(\"" << escapeSpecialChars(s) << "\", NUM_ASCII_TRACKS, indices_main);//" << id << " = makeString(" << escapeSpecialChars(s) << ")");
        
        
        char *str = StrangerAutomaton::strToCharStar(s);
		//std::string encodedString = getEncodedString(s);
		retMe = new StrangerAutomaton(
                                      dfa_construct_string(str, num_ascii_track,
                                                           indices_main));
        free(str);
        
		{
			retMe->setID(id);
			retMe->debugAutomaton();
		}
		return retMe;
	}
}

/**
 * Creates an automaton that accepts exactly the given string. It also
 * assigns this string to StrangerAutomaton.str. If parameter s is empty
 * string then it will call StrangerAutomaton.makeEmptyString
 */
StrangerAutomaton* StrangerAutomaton::makeString(std::string s)
{
    
    return makeString(s, traceID);
}

/**
 * Creates an automaton that accepts exactly the given character. It also
 * assigns this character to StrangerAutomaton.str
 * @param id
 *            : id of node associated with this auto; used for debugging
 *            purposes only
 * */
StrangerAutomaton* StrangerAutomaton::makeChar(char c, int id)
{
	debug(stringbuilder() << id << " = makeChar(" << c << ") -- start");
    StrangerAutomaton* retMe = StrangerAutomaton::makeString((stringbuilder() << c), id);
    debug(stringbuilder() << id << " = makeChar(" << c << ") -- end");
    {
    	retMe->setID(id);
    	retMe->debugAutomaton();
    }
    return retMe;
}
/**
 * Creates an automaton that accepts exactly the given character. It also
 * assigns this character to StrangerAutomaton.str
 * */
StrangerAutomaton* StrangerAutomaton::makeChar(char c)
{
    
    return makeChar(c, traceID);
}

/**
 * returns an automaton that accepts a single character in the range between
 * from and to
 * @param id
 *            : id of node associated with this auto; used for debugging
 *            purposes only
 */
StrangerAutomaton* StrangerAutomaton::makeCharRange(char from, char to, int id) {
    debug(stringbuilder() << id <<  " = makeCharRange(" << from << ", " << to << ")");
    
    debugToFile(stringbuilder() << "M[" << traceID << "] = dfa_construct_range('" << from << "', '" << to << "', NUM_ASCII_TRACKS, indices_main);//" << id << " = makeCharRange(" << from << ", " << to << ")");
    
    StrangerAutomaton* retMe = new StrangerAutomaton(
                                                     dfa_construct_range(from, to,
                                                                         num_ascii_track,
                                                                         indices_main));
    
    {
        retMe->setID(id);
        retMe->debugAutomaton();
    }
    return retMe;
}

/**
 * returns an automaton that accepts a single character in the range between
 * from and to
 */
StrangerAutomaton* StrangerAutomaton::makeCharRange(char from, char to) {
    return makeCharRange(from, to, traceID);
}

/**
 * Creates an automaton that accepts any string including empty string (.*)
 * @param id
 *            : id of node associated with this auto; used for debugging
 *            purposes only
 * */
StrangerAutomaton* StrangerAutomaton::makeAnyString(int id) {
    debug(stringbuilder() << id <<  " = makeAnyString()");
    
    debugToFile(stringbuilder() << "M[" << traceID << "] = dfaAllStringASCIIExceptReserveWords(NUM_ASCII_TRACKS, indices_main);//" << id << " = makeAnyString()");
    
    StrangerAutomaton* retMe = new StrangerAutomaton(
                                                     dfaAllStringASCIIExceptReserveWords(
                                                                                         num_ascii_track,
                                                                                         indices_main));
    {
        retMe->setID(id);
        retMe->debugAutomaton();
    }
    return retMe;
}

/**
 * Creates an automaton that accepts any string including empty string (.*)
 * */
StrangerAutomaton* StrangerAutomaton::makeAnyString() {
    return makeAnyString(traceID);
}

/**
 * Creates an automaton that accepts everything (.*) within the length from
 * l1 to l2 l2 = -1, indicates unbounded upperbound l1 = -1, indicates
 * unbounded lowerbound StrangerAutomaton.str will be assigned null.
 * @param id
 *            : id of node associated with this auto; used for debugging
 *            purposes only
 */
// TODO: check to see if l1 is allowed to be 0
StrangerAutomaton* StrangerAutomaton::makeAnyStringL1ToL2(int l1, int l2, int id) {
    debug(stringbuilder() << "makeAnyStringL1ToL2(" << l1 << "," << l2 << ")");
    debugToFile(stringbuilder() << "M[" << traceID << "] = dfaSigmaC1toC2(" << l1 << "," << l2 << ",NUM_ASCII_TRACKS, indices_main);//" << id << " = dfaSigmaC1toC2()");
    
    StrangerAutomaton* retMe = new StrangerAutomaton(
                                                     dfaSigmaC1toC2(l1, l2,
                                                                    num_ascii_track,
                                                                    indices_main));
    
    {
        retMe->setID(id);
        retMe->debugAutomaton();
    }
    return retMe;
}

/**
 * Creates an automaton that accepts everything (.*) within the length from
 * l1 to l2 l2 = -1, indicates unbounded upperbound l1 = -1, indicates
 * unbounded lowerbound StrangerAutomaton.str will be assigned null.
 */
StrangerAutomaton* StrangerAutomaton::makeAnyStringL1ToL2(int l1, int l2) {
    return makeAnyStringL1ToL2(l1, l2, traceID);
}

/**
 * Creates an automaton that accepts only the empty string "epsilon". It
 * also assigns empty string ("") to StrangerAutomaton.str to be used later
 * with autoToString method.
 * @param id
 *            : id of node associated with this auto; used for debugging
 *            purposes only
 * @return
 */
StrangerAutomaton* StrangerAutomaton::makeEmptyString(int id) {
    debug(stringbuilder() << id <<  " = makeEmptyString()");
    
    debugToFile(stringbuilder() << "M[" << traceID << "] = dfaASCIIOnlyNullString(NUM_ASCII_TRACKS, indices_main);//" << id << " = makeEmptyString()");
    
    StrangerAutomaton* retMe = new StrangerAutomaton(
                                                     dfaASCIIOnlyNullString(
                                                                            num_ascii_track,
                                                                            indices_main));
    
    {
        retMe->setID(id);
        retMe->debugAutomaton();
    }
    return retMe;
}

/**
 * Creates an automaton that accepts only the empty string "epsilon". It
 * also assigns empty string ("") to StrangerAutomaton.str to be used later
 * with autoToString method.
 * @return
 */
StrangerAutomaton* StrangerAutomaton::makeEmptyString() {
    return makeEmptyString(traceID);
}

/**
 * creates an automaton that represents a dot (.) in a regular expressions.
 * Dot means single character of any value in alphabet.
 * @param id
 *            : id of node associated with this auto; used for debugging
 *            purposes only
 * @return
 */
StrangerAutomaton* StrangerAutomaton::makeDot(int id) {
    debug(stringbuilder() << id <<  " = makeDot()");
    
    debugToFile(stringbuilder() << "M[" << traceID << "] = dfaDot(NUM_ASCII_TRACKS, indices_main);//"<< id << " = makeDot()");
    
    StrangerAutomaton* retMe = new StrangerAutomaton(
                                                     dfaDot(
                                                            num_ascii_track,
                                                            indices_main));
    
    {
        retMe->setID(id);
        retMe->debugAutomaton();
    }
    return retMe;
}

/**
 * creates an automaton that represents a dot (.) in a regular expressions.
 * Dot means single character of any value in alphabet.
 * @return
 */
StrangerAutomaton* StrangerAutomaton::makeDot() {
    return makeDot(traceID);
}

/**
 * creates an automaton that accepts nothing, not even empty string
 * @param id
 *            : id of node associated with this auto; used for debugging
 *            purposes only
 * */
StrangerAutomaton* StrangerAutomaton::makePhi(int id) {
    debug(stringbuilder() << id <<  " = makePhi");
    
    debugToFile(stringbuilder() << "M[" << traceID << "] = dfaASCIINonString(NUM_ASCII_TRACKS, indices_main);//"<< id << " = makePhi()");
    
    StrangerAutomaton* retMe = new StrangerAutomaton(dfaASCIINonString(num_ascii_track, indices_main));
    {
        retMe->setID(id);
        retMe->debugAutomaton();
    }
    return retMe;
}

/**
 * creates an automaton that accepts nothing, not even empty string
 * */
StrangerAutomaton* StrangerAutomaton::makePhi() {
    return makePhi(traceID);
}

std::string StrangerAutomaton::generateSatisfyingExample()
{
    return NULL;
}

//***************************************************************************************
//*                                  Unary Operations                                   *
//*									-------------------									*
//* These operations are given one automata and result in a newly created one.			*
//***************************************************************************************

/**
 * Returns a new automaton that accepts (empty_string) union L(this auto)).
 * @param id: id of node associated with this auto; used for debugging
 *            purposes only
 * @return
 */
StrangerAutomaton* StrangerAutomaton::optional(int id) {
    debug(stringbuilder() << id <<  " = makeOptional("  << this->ID <<  ") -- start");
    
    StrangerAutomaton* retMe = this->unionWithEmptyString(id);
    
    debug(stringbuilder() << id <<  " = makeOptional("  << this->ID <<  ") -- end");
    
    {
        retMe->setID(id);
        retMe->debugAutomaton();
    }
    return retMe;
}

/**
 * Returns a new automaton that accepts (empty_string) union L(this auto)).
 * @return
 */
StrangerAutomaton* StrangerAutomaton::optional() {
    return this->optional(traceID);
}



/**
 * Returns a new automaton that accepts (empty_string) union closure(this auto)).
 * @param id
 *            : id of node associated with this auto; used for debugging
 *            purposes only
 * @return
 */
StrangerAutomaton* StrangerAutomaton::kleensStar(int id) {
    debug(stringbuilder() << id <<  " = kleensStar("  << this->ID <<  ") -- start");
    
    StrangerAutomaton* temp = this->closure(this->ID);
    StrangerAutomaton* retMe = temp->unionWithEmptyString(id);
    delete temp;
    debug(stringbuilder() << id <<  " = kleensStar("  << this->ID <<  ") -- end");
    {
        retMe->setID(id);
        retMe->debugAutomaton();
    }
    return retMe;
}

/**
 * Returns a new automaton that accepts (empty_string) union closure(this auto)).
 * @return
 */
StrangerAutomaton* StrangerAutomaton::kleensStar() {
    return this->kleensStar(traceID);
}

/**
 * Returns a new automaton that accepts (empty_string) union closure(auto)).
 * @param auto: input auto to do kleens star for
 * @param id
 *            : id of node associated with this auto; used for debugging
 *            purposes only
 * @return
 */
StrangerAutomaton* StrangerAutomaton::kleensStar(StrangerAutomaton* otherAuto, int id) {
    debug(stringbuilder() << id <<  " = kleensStar(" << otherAuto->ID << ")");
    
    StrangerAutomaton* retMe = otherAuto->kleensStar(id);
    {
        retMe->setID(id);
        retMe->debugAutomaton();
    }
    return retMe;
}

/**
 * Returns a new automaton that accepts (empty_string) union closure(auto)).
 * @param auto: input auto to do kleens star for
 * @return
 */
StrangerAutomaton* StrangerAutomaton::kleensStar(StrangerAutomaton* otherAuto) {
    return kleensStar(otherAuto, traceID);
}


/**
 * returns a new automaton with language L = closure(L(this auto))
 * @param id
 *            : id of node associated with this auto; used for debugging
 *            purposes only
 * @return
 */
StrangerAutomaton* StrangerAutomaton::closure(int id) {
    debug(stringbuilder() << id <<  " = closure("  << this->ID <<  ")");
    
    if (isTop() || isBottom()) return this->clone(id);
    
    debugToFile(stringbuilder() << "M[" << traceID << "] = dfa_closure_extrabit(M["<< this->autoTraceID << "], NUM_ASCII_TRACKS, indices_main);//"<<id << " = closure("  << this->ID <<  ")");
    perfInfo.numOfClosure++;
    long start = PerfInfo::currentTimeMillis();
    
    StrangerAutomaton* retMe = new StrangerAutomaton(
                                                     dfa_closure_extrabit(this->dfa,
                                                                          num_ascii_track,
                                                                          indices_main));
    
    long stop = PerfInfo::currentTimeMillis();
    perfInfo.closureTime += (stop - start);
    
    {
        retMe->setID(id);
        retMe->debugAutomaton();
    }
    return retMe;
}

/**
 * returns a new automaton with language L = closure(L(this auto))
 * @return
 */
StrangerAutomaton* StrangerAutomaton::closure() {
    return this->closure(traceID);
}

/**
 * returns a new automaton with language L = closure(L(auto))
 * @param auto
 * @param id
 *            : id of node associated with this auto; used for debugging
 *            purposes only
 * @return
 */
StrangerAutomaton* StrangerAutomaton::closure(StrangerAutomaton* otherAuto, int id) {
    debug(stringbuilder() << id <<  " = closure(" << otherAuto->ID << ")");
    
    StrangerAutomaton* retMe = otherAuto->closure(id);
    {
        retMe->setID(id);
        retMe->debugAutomaton();
    }
    return retMe;
}

/**
 * returns a new automaton with language L = closure(L(auto))
 * @param auto
 * @return
 */
StrangerAutomaton* StrangerAutomaton::closure(StrangerAutomaton* otherAuto) {
    return closure(otherAuto, traceID);
}

/**
 * Returns new automaton that accepts <code>min</code> or more concatenated
 * repetitions of the language of this automaton.
 *
 * @param min: minimum number of concatenations
 * @param id: id of node associated with this auto; used for debugging
 *            purposes only
 * @return
 */
StrangerAutomaton* StrangerAutomaton::repeat(unsigned min, int id) {
    debug(stringbuilder() << id <<  " = repeate(" << min << ","  << this->ID <<  ") -- start");
    
    StrangerAutomaton* retMe = NULL;
    if (min == 0)
        retMe = this->kleensStar(id);
    else if (min == 1)
        retMe = this->closure(id);
    else {
        StrangerAutomaton* temp = this->closure(id);
        
        StrangerAutomaton* unionAuto = this->clone(id);
        StrangerAutomaton* concatAuto = this->clone(id);
        for (unsigned int i = 2; i < min; i++) {
        	StrangerAutomaton* tempConcat = concatAuto;
			concatAuto = tempConcat->concatenate(this,id);
			delete tempConcat;
            
			StrangerAutomaton* tempUnion = unionAuto;
			unionAuto = tempUnion->union_(concatAuto, id);
			delete tempUnion;
        }
        
        StrangerAutomaton* complement = unionAuto->complement(id);
        retMe = temp->intersect(complement);
        
        delete complement;
        delete concatAuto;
        delete unionAuto;
        delete temp;
    }
    debug(stringbuilder() << id <<  " = repeate(" << min << ","  << this->ID <<  ") -- end");
    {
        retMe->setID(id);
        retMe->debugAutomaton();
    }
    return retMe;
}

/**
 * Returns new automaton that accepts <code>min</code> or more concatenated
 * repetitions of the language of this automaton.
 *
 * @param min: minimum number of concatenations
 * @return
 */
StrangerAutomaton* StrangerAutomaton::repeat(unsigned min) {
    return this->repeat(min, traceID);
}


/**
 * Returns new automaton that accepts between <code>min</code> and
 * <code>max</code> (including both) concatenated repetitions of the
 * language of this automaton.
 *
 * @param min: minimum number of concatenations
 * @param max: maximum number of concatenations
 *
 * @param id: id of node associated with this auto; used for debugging
 *            purposes only
 * @return
 */
StrangerAutomaton* StrangerAutomaton::repeat(unsigned min, unsigned max, int id) {
    debug(stringbuilder() << id <<  " = repeate(" << min << ", " << max << ", " << this->ID << ") -- start");
    
    StrangerAutomaton* retMe = NULL;
    if (min > max) {
        retMe = makePhi(id);
    }
    else {
        StrangerAutomaton* unionAuto = NULL;
        StrangerAutomaton* concatAuto = NULL;
        
        // handle min
        if ( min == 0) { // {min, max} where min is 0
        	unionAuto = makeEmptyString(id);
        	concatAuto = makeEmptyString(id);
        } else { 								   	// {min, max} where min > 0
        	concatAuto = this->clone(id);			// {min, max} where min = 1
            for (unsigned int i = 2; i <= min; i++) { 		// {min, max} where min > 1
            	StrangerAutomaton* tempConcat = concatAuto;
				concatAuto = tempConcat->concatenate(this,id);
				delete tempConcat;
            }
            unionAuto = concatAuto->clone(id);
        }
        
        // handle min + 1, max
    	for (unsigned int i = min + 1; i <= max; i++){
    		StrangerAutomaton* tempConcat = concatAuto;
    		concatAuto = tempConcat->concatenate(this,id);
    		delete tempConcat;
            
    		StrangerAutomaton* tempUnion = unionAuto;
    		unionAuto = tempUnion->union_(concatAuto, id);
    		delete tempUnion;
    	}
        
    	delete concatAuto;
    	retMe = unionAuto;
    }
    
    debug(stringbuilder() << id <<  " = repeate(" <<  min << ", " << max << ", " << this->ID << ") -- end");
    {
        retMe->setID(id);
        retMe->debugAutomaton();
    }
    
    return retMe;
}

/**
 * Returns new automaton that accepts between <code>min</code> and
 * <code>max</code> (including both) concatenated repetitions of the
 * language of this automaton.
 * @param min: minimum number of concatenations
 * @param max: maximum number of concatenations
 * @return
 */
StrangerAutomaton* StrangerAutomaton::repeat1(unsigned min, unsigned max) {
    return this->repeat(min, max, traceID);
}

/**
 * Returns a new automaton auto with L(auto)= the complement of the language of current automaton
 * @param id: id of node associated with this auto; used for debugging
 *            purposes only
 * @return
 */
StrangerAutomaton* StrangerAutomaton::complement(int id) {
    debug(stringbuilder() << id <<  " = complement("  << this->ID <<  ")");
    if (isTop())
        // top is an unknown type so can not be complemented
        return makeTop(id);
    else if (isBottom())
        // bottom is efficient phi so complement is Sigma*
        return makeAnyString(id);
    
    debugToFile(stringbuilder() << "M[" << traceID << "] = dfa_negate(M[" << this->autoTraceID << "], NUM_ASCII_TRACKS, indices_main);//"<<id << " = complement("  << this->ID <<  ")");
    perfInfo.numOfComplement++;
    long start = PerfInfo::currentTimeMillis();
    
    StrangerAutomaton* retMe = new StrangerAutomaton(
                                                     dfa_negate(this->dfa,
                                                                num_ascii_track,
                                                                indices_main));
    
    
    long stop = PerfInfo::currentTimeMillis();
    perfInfo.complementTime += (stop - start);
    
    
    
    {
        retMe->setID(id);
        retMe->debugAutomaton();
    }
    
    return retMe;
}

/**
 * Returns a new automaton auto with L(auto)= the complement of the language of current automaton
 * @return
 */
StrangerAutomaton* StrangerAutomaton::complement() {
    return this->complement(traceID);
}

//***************************************************************************************
//*                                  Binary Operations                                  *
//*									-------------------									*
//* These operations are given two automata and result in a newly created one.			*
//***************************************************************************************


/**
 * Returns a new automaton auto with L(auto)= L(this) union L(auto)
 * @param auto
 * @param id
 *            : id of node associated with this auto; used for debugging
 *            purposes only
 * @return
 */
StrangerAutomaton* StrangerAutomaton::union_(StrangerAutomaton* otherAuto, int id) {
    debug(stringbuilder() << id <<  " = union_("  << this->ID <<  ", " << otherAuto->ID << ")");
    
    // if top or bottom then do not use the c library as dfa == NULL
    if (this->isTop() || otherAuto->isTop())
        return makeTop(id);
    else if (this->isBottom())
        return otherAuto->clone(id);
    else if (otherAuto->isBottom())
        return this->clone(id);
    
    
    debugToFile(stringbuilder() << "M[" << traceID << "] = dfa_union_with_emptycheck(M[" << this->autoTraceID << "], M["<< otherAuto->autoTraceID  << "], NUM_ASCII_TRACKS, indices_main);//"<<id << " = union_("  << this->ID <<  ", " << otherAuto->ID << ")");
    perfInfo.numOfUnion++;
    long start = PerfInfo::currentTimeMillis();
    
    StrangerAutomaton* retMe = new StrangerAutomaton(
                                                     dfa_union_with_emptycheck(this->dfa,
                                                                               otherAuto->dfa,
                                                                               num_ascii_track,
                                                                               indices_main)
                                                     );
    
    
    
    long finish = PerfInfo::currentTimeMillis();
    perfInfo.unionTime += (finish - start);
    
    
    
    {
        retMe->setID(id);
        retMe->debugAutomaton();
    }
    return retMe;
}

/**
 * Returns a new automaton auto with L(auto)= L(this) union L(auto)
 * @param auto
 * @return
 */
StrangerAutomaton* StrangerAutomaton::union_(StrangerAutomaton* otherAuto) {
    return this->union_(otherAuto, traceID);
}

/**
 * Returns a new automaton auto with L(auto)= L(this) union L2 where L2 contains only empty string (epsilon)
 * @param id
 *            : id of node associated with this auto; used for debugging
 *            purposes only
 * @return
 */
StrangerAutomaton* StrangerAutomaton::unionWithEmptyString(int id) {
    debug(stringbuilder() << id <<  " = unionWithEmptyString("  << this->ID <<  ") -- start");
    
    StrangerAutomaton* empty = StrangerAutomaton::makeEmptyString(-100);
    StrangerAutomaton* retMe = this->union_(empty, id);
    delete empty;
    
    debug(stringbuilder() << id <<  " = unionWithEmptyString("  << this->ID <<  ") -- end");
    
    {
        retMe->setID(id);
        retMe->debugAutomaton();
    }
    return retMe;
}

/**
 * Returns a new automaton auto with L(auto)= L(this) union L2 where L2 contains only empty string (epsilon)
 * @return
 */
StrangerAutomaton* StrangerAutomaton::unionWithEmptyString() {
    return this->unionWithEmptyString(traceID);
}


/**
 * Returns a new automaton auto with L(auto)= L(this) intersect L(auto)
 * @param id
 *            : id of node associated with this auto; used for debugging
 *            purposes only
 */
StrangerAutomaton* StrangerAutomaton::intersect(StrangerAutomaton* otherAuto, int id) {
    debug(stringbuilder() << id <<  " = intersect("  << this->ID <<  ", " << otherAuto->ID << ")");
    
    // if top or bottom then do not use the c library as dfa == NULL
    if (this->isBottom() || otherAuto->isBottom())
        return makeBottom(id);
    else if (this->isTop())
        return otherAuto->clone(id);
    else if (otherAuto->isTop())
        return this->clone(id);
    
    debugToFile(stringbuilder() << "M[" << traceID << "] = dfa_intersect(M[" << this->autoTraceID << "], M["<< otherAuto->autoTraceID  << "]);//"<<id << " = intersect("  << this->ID <<  ", " << otherAuto->ID << ")");
    perfInfo.numOfIntersect++;
    long start = PerfInfo::currentTimeMillis();
    
    StrangerAutomaton* retMe = new StrangerAutomaton(
                                                     dfa_intersect(this->dfa, otherAuto->dfa));
    
    
    
    long stop = PerfInfo::currentTimeMillis();
    perfInfo.intersectTime += (stop - start);
    
    {
        retMe->setID(id);
        retMe->debugAutomaton();
    }
    return retMe;
}

/**
 * Returns a new automaton auto with L(auto)= L(this) intersect L(auto)
 */
StrangerAutomaton* StrangerAutomaton::intersect(StrangerAutomaton* otherAuto) {
    return intersect(otherAuto, traceID);
}


//***************************************************************************************
//*                                  Widening operations                                *
//***************************************************************************************

/**
 * This will do widen(this, auto). L(this) should_be_subset_of L(auto)
 * We first apply union, then this widening for a while then the
 * coarse one.
 * @param auto
 * @param id
 *            : id of node associated with this auto; used for debugging
 *            purposes only
 * @return
 */
StrangerAutomaton* StrangerAutomaton::preciseWiden(StrangerAutomaton* otherAuto, int id) {
    debug(stringbuilder() << id <<  " = precise_widen("  << this->ID <<  ", " << otherAuto->ID << ")");
    
    // if top or bottom then do not use the c library as dfa == NULL
    if (this->isTop() || otherAuto->isTop())
        return makeTop(id);
    else if (this->isBottom())
        return otherAuto->clone(id);
    else if (otherAuto->isBottom())
        return this->clone(id);
    
    if (coarseWidening) {
        debugToFile(stringbuilder() << "setPreciseWiden();");
        setPreciseWiden();
        coarseWidening = false;
    }
    
    debugToFile(stringbuilder() << "M[" << traceID << "] = dfaWiden(M[" << this->autoTraceID << "], M["<< otherAuto->autoTraceID  << "]);//"<<id << " = precise_widen("  << this->ID <<  ", " << otherAuto->ID << ")");
    perfInfo.numOfPreciseWiden++;
    long start = PerfInfo::currentTimeMillis();
    
    StrangerAutomaton* retMe = new StrangerAutomaton(
                                                     dfaWiden(this->dfa, otherAuto->dfa));
    
    long stop = PerfInfo::currentTimeMillis();
    perfInfo.preciseWidenTime += (stop - start);
    
    {
        retMe->setID(id);
        retMe->debugAutomaton();
    }
    return retMe;
}

/**
 * This will do widen(this, auto). L(this) should_be_subset_of L(auto)
 * We first apply union, then this widening for a while then the
 * coarse one.
 * @param auto
 * @return
 */
StrangerAutomaton* StrangerAutomaton::preciseWiden(StrangerAutomaton* otherAuto) {
    return preciseWiden(otherAuto, traceID);
}

/**
 * This will do widen(this, auto). L(this) should_be_subset_of L(auto)
 * We first apply union, then precise widening for a while then this
 * coarse one.
 * @param auto
 * @param id
 *            : id of node associated with this auto; used for debugging
 *            purposes only
 * @return
 */
StrangerAutomaton* StrangerAutomaton::coarseWiden(StrangerAutomaton* otherAuto, int id) {
    debug(stringbuilder() << id <<  " = coarse_widen("  << this->ID <<  ", " << otherAuto->ID << ")");
    
    // if top or bottom then do not use the c library as dfa == NULL
    if (this->isTop() || otherAuto->isTop())
        return makeTop(id);
    else if (this->isBottom())
        return otherAuto->clone(id);
    else if (otherAuto->isBottom())
        return this->clone(id);
    
    if (!coarseWidening) {
        debugToFile(stringbuilder() << "setCoarseWiden();");
        setCoarseWiden();
        coarseWidening = true;
    }
    
    debugToFile(stringbuilder() << "M[" << traceID << "] = dfaWiden(M[" << this->autoTraceID << "], M["<< otherAuto->autoTraceID  << "]);//"<<id << " = coarse_widen("  << this->ID <<  ", " << otherAuto->ID << ")");
    perfInfo.numOfCoarseWiden++;
    long start = PerfInfo::currentTimeMillis();
    
    StrangerAutomaton* retMe = new StrangerAutomaton(
                                                     dfaWiden(this->dfa, otherAuto->dfa));
    
    long stop = PerfInfo::currentTimeMillis();
    perfInfo.coarseWidenTime += (stop - start);
    
    {
        retMe->setID(id);
        retMe->debugAutomaton();
    }
    return retMe;
}

/**
 * This will do widen(this, auto). L(this) should_be_subset_of L(auto)
 * We first apply union, then precise widening for a while then this
 * coarse one.
 * @param auto
 * @return
 */
StrangerAutomaton* StrangerAutomaton::coarseWiden(StrangerAutomaton* otherAuto) {
    return coarseWiden(otherAuto, traceID);
}

//***************************************************************************************
//*                                  Forwards Concatenation                             *
//***************************************************************************************

/**
 * Concatenates current automaton with otherAuto-> New automaton will be
 * this+otherAuto-> If both automatons' strings are not null, they will be
 * concatenated too otherwise set null. id : id of node associated with this
 * auto; used for debugging purposes only
 *
 * @param auto
 * @param id
 *            : id of node associated with this auto; used for debugging
 *            purposes only
 * @return
 */
StrangerAutomaton* StrangerAutomaton::concatenate(StrangerAutomaton* otherAuto, int id) {
    debug(stringbuilder() << id <<  " = concatenate("  << this->ID <<  ", " << otherAuto->ID << ")");
    
    // TODO: this is different than javascrit semantics. check http://www.quirksmode.org/js/strings.html
    // if top or bottom then do not use the c library as dfa == NULL
    if (this->isTop() || otherAuto->isTop())
        return makeTop(id);
    else if (this->isBottom() || otherAuto->isBottom())
        return makeBottom(id);
    
    debugToFile(stringbuilder() << "M[" << traceID << "] = dfa_concat(M[" << this->autoTraceID << "], M["<< otherAuto->autoTraceID  << "], NUM_ASCII_TRACKS, indices_main);//"<<id << " = concatenate("  << this->ID <<  ", " << otherAuto->ID << ")");
    perfInfo.numOfConcat++;
    long start = PerfInfo::currentTimeMillis();
    
    // dfa_concat_extrabit returns new dfa structure in memory so no need to
    // worry about the two dfas of this and auto
    StrangerAutomaton* retMe = new StrangerAutomaton(
                                                     dfa_concat(this->dfa,
                                                                otherAuto->dfa, num_ascii_track,
                                                                indices_main));
    
    long finish = PerfInfo::currentTimeMillis();
    perfInfo.concatTime += (finish - start);
    
    {
        retMe->setID(id);
        retMe->debugAutomaton();
    }
    return retMe;
}

/**
 * Concatenates current automaton with otherAuto-> New automaton will be
 * this+otherAuto-> If both automatons' strings are not null, they will be
 * concatenated too otherwise set null. id : id of node associated with this
 * auto; used for debugging purposes only
 *
 * @param auto
 * @param id
 *            : id of node associated with this auto; used for debugging
 *            purposes only
 * @return
 */
StrangerAutomaton* StrangerAutomaton::concatenate(StrangerAutomaton* otherAuto) {
    return concatenate(otherAuto, traceID);
}

//***************************************************************************************
//*                                  Backwards Concatenation                            *
//***************************************************************************************
/**
 * For current automaton concatAuto (this) , method returns an automaton
 * retMe such that L(retME) = all possible strings to be the left side of a
 * concat operation that results in the concatAuto.
 *
 * @param rightSiblingAuto
 * @param concatAuto
 * @param id
 *            : id of node associated with this auto; used for debugging
 *            purposes only
 * @return
 */
StrangerAutomaton* StrangerAutomaton::leftPreConcat(StrangerAutomaton* rightSiblingAuto,
                                                    int id) {
    debug(stringbuilder() << id <<  " = leftPreConcat("  << this->ID <<  ", " << rightSiblingAuto->ID << ")");
    // if top or bottom then do not use the c library as dfa == NULL
    if (this->isBottom() || rightSiblingAuto->isBottom())
        return makeBottom(id);
    else if (this->isTop() || rightSiblingAuto->isTop())
        return makeTop(id);
    
    debugToFile(stringbuilder() << "M[" << traceID << "] = dfa_pre_concat(M[" << this->autoTraceID << "], M[" << rightSiblingAuto->autoTraceID << "], 1, NUM_ASCII_TRACKS, indices_main);//" << id << " = leftPreConcat("  << this->ID <<  ", " << rightSiblingAuto->ID << ")");
    perfInfo.numOfPreConcat++;
    long start = PerfInfo::currentTimeMillis();
    
    StrangerAutomaton* retMe = new StrangerAutomaton(
                                                     dfa_pre_concat(this->dfa,
                                                                    rightSiblingAuto->dfa, 1,
                                                                    num_ascii_track,
                                                                    indices_main));
    
    long stop = PerfInfo::currentTimeMillis();
    perfInfo.preconcatTime += (stop - start);
    
    {
        retMe->setID(id);
        retMe->debugAutomaton();
    }
    return retMe;
}

/**
 * For current automaton concatAuto (this) , method returns an automaton
 * retMe such that L(retME) = all possible strings to be the left side of a
 * concat operation that results in the concatAuto.
 *
 * @param rightSiblingAuto
 * @param concatAuto
 * @return
 */
StrangerAutomaton* StrangerAutomaton::leftPreConcat(StrangerAutomaton* rightSiblingAuto) {
    return leftPreConcat(rightSiblingAuto, traceID);
}


/**
 * For current automaton concatAuto (this) , method returns an automaton
 * retMe such that L(retME) = all possible strings to be the left side of a
 * concat operation that results in the concatAuto.
 *
 * @param rightSiblingAuto
 * @param concatAuto
 * @param id
 *            : id of node associated with this auto; used for debugging
 *            purposes only
 * @return
 */
StrangerAutomaton* StrangerAutomaton::leftPreConcatConst(std::string rightSiblingString,
                                                         int id) {
    debug(stringbuilder() << id <<  " = rightPreConcatConst("  << this->ID <<  ", " << rightSiblingString << ")");
    
    // if top or bottom then do not use the c library as dfa == NULL
    if (this->isBottom())
        return makeBottom(id);
    else if (this->isTop())
        return makeTop(id);
    
    debugToFile(stringbuilder() << "M[" << traceID << "] = dfa_pre_concat_const(M[" << this->autoTraceID << "], \"" << escapeSpecialChars(rightSiblingString) << "\", 1, NUM_ASCII_TRACKS, indices_main);//" <<id << " = rightPreConcatConst("  << this->ID <<  ", " << escapeSpecialChars(rightSiblingString) << ")");
    perfInfo.numOfConstPreConcat++;
    long start = PerfInfo::currentTimeMillis();
    
    StrangerAutomaton* retMe = new StrangerAutomaton(
                                                     dfa_pre_concat_const(this->dfa,
                                                                          StrangerAutomaton::strToCharStar(rightSiblingString), 1,
                                                                          num_ascii_track,
                                                                          indices_main));
    
    //		StrangerAutomaton* retMe = new StrangerAutomaton(
    //				dfa_pre_concat(this->dfa,
    //						makeString(rightSiblingString, -1).dfa, 1,
    //						num_ascii_track,
    //						indices_main));
    
    long stop = PerfInfo::currentTimeMillis();
    perfInfo.constpreconcatTime += (stop - start);
    
    
    {
        retMe->setID(id);
        retMe->debugAutomaton();
    }
    return retMe;
}

/**
 * For current automaton concatAuto (this) , method returns an automaton
 * retMe such that L(retME) = all possible strings to be the left side of a
 * concat operation that results in the concatAuto.
 *
 * @param rightSiblingAuto
 * @param concatAuto
 * @return
 */
StrangerAutomaton* StrangerAutomaton::leftPreConcatConst(std::string rightSiblingString) {
    return leftPreConcatConst(rightSiblingString, traceID);
}

/**
 * For current automaton concatAuto (this) , method returns an automaton
 * retMe such that L(retME) = all possible strings to be the right side of a
 * concat operation that results in the concatAuto.
 *
 * @param leftSiblingAuto
 * @param concatAuto
 * @param id
 *            : id of node associated with this auto; used for debugging
 *            purposes only
 * @return
 */
StrangerAutomaton* StrangerAutomaton::rightPreConcat(StrangerAutomaton* leftSiblingAuto,
                                                     int id) {
    debug(stringbuilder() << id <<  " = rightPreConcat("  << this->ID <<  ", " << leftSiblingAuto->ID<< ")");
    
    // if top or bottom then do not use the c library as dfa == NULL
    if (this->isBottom() || leftSiblingAuto->isBottom())
        return makeBottom(id);
    else if (this->isTop() || leftSiblingAuto->isTop())
        return makeTop(id);
    
    debugToFile(stringbuilder() << "M[" << (traceID) << "] = dfa_pre_concat(M[" << this->autoTraceID << "], M[" << leftSiblingAuto->autoTraceID << "], 2, NUM_ASCII_TRACKS, indices_main);//"<<id << " = rightPreConcat("  << this->ID <<  ", " << leftSiblingAuto->ID
				<< ")");
    perfInfo.numOfPreConcat++;
    long start = PerfInfo::currentTimeMillis();
    
    StrangerAutomaton* retMe = new StrangerAutomaton(
                                                     dfa_pre_concat(this->dfa,
                                                                    leftSiblingAuto->dfa, 2,
                                                                    num_ascii_track,
                                                                    indices_main));
    
    long stop = PerfInfo::currentTimeMillis();
    perfInfo.preconcatTime += (stop - start);
    
    {
        retMe->setID(id);
        retMe->debugAutomaton();
    }
    return retMe;
}

/**
 * For current automaton concatAuto (this) , method returns an automaton
 * retMe such that L(retME) = all possible strings to be the right side of a
 * concat operation that results in the concatAuto.
 *
 * @param leftSiblingAuto
 * @param concatAuto
 * @return
 */
StrangerAutomaton* StrangerAutomaton::rightPreConcat(StrangerAutomaton* leftSiblingAuto) {
    return rightPreConcat(leftSiblingAuto, traceID);
}

/**
 * For current automaton concatAuto (this) , method returns an automaton
 * retMe such that L(retME) = all possible strings to be the right side of a
 * concat operation that results in the concatAuto.
 *
 * @param leftSiblingString
 * @param concatAuto
 * @param id
 *            : id of node associated with this auto; used for debugging
 *            purposes only
 * @return
 */
StrangerAutomaton* StrangerAutomaton::rightPreConcatConst(std::string leftSiblingString,
                                                          int id) {
    debug(stringbuilder() << id <<  " = rightPreConcatConst("  << this->ID <<  ", " << leftSiblingString << ")");
    
    // if top or bottom then do not use the c library as dfa == NULL
    if (this->isBottom())
        return makeBottom(id);
    else if (this->isTop())
        return makeTop(id);
    
    debugToFile(stringbuilder() << "M[" << traceID << "] = dfa_pre_concat_const(M[" << this->autoTraceID << "], \"" << escapeSpecialChars(leftSiblingString) << "\", 2, NUM_ASCII_TRACKS, indices_main);//" << id << " = rightPreConcatConst("  << this->ID <<  ", "
				<< escapeSpecialChars(leftSiblingString) << ")");
    perfInfo.numOfConstPreConcat++;
    long start = PerfInfo::currentTimeMillis();
    
    StrangerAutomaton* retMe = new StrangerAutomaton(
                                                     dfa_pre_concat_const(this->dfa,
                                                                          StrangerAutomaton::strToCharStar(leftSiblingString), 2, num_ascii_track,
                                                                          indices_main));
    
    //		StrangerAutomaton* retMe = new StrangerAutomaton(
    //				dfa_pre_concat(this->dfa,
    //						makeString(leftSiblingString, -1).dfa, 2, num_ascii_track,
    //						indices_main));
    
    long stop = PerfInfo::currentTimeMillis();
    perfInfo.constpreconcatTime += (stop - start);
    
    
    
    {
        retMe->setID(id);
        retMe->debugAutomaton();
    }
    return retMe;
}

/**
 * For current automaton concatAuto (this) , method returns an automaton
 * retMe such that L(retME) = all possible strings to be the right side of a
 * concat operation that results in the concatAuto.
 *
 * @param leftSiblingString
 * @param concatAuto
 * @return
 */
StrangerAutomaton* StrangerAutomaton::rightPreConcatConst(std::string leftSiblingString) {
    return rightPreConcatConst(leftSiblingString, traceID);
}

//***************************************************************************************
//*                                  Forward Replacement                                *
//***************************************************************************************

/**
 * Parses a PHP regular expression and converts it into stranger automaton.
 * For allowed regular expressions check {@link RegExp}.
 * @param phpRegexOrig: the string literal representing the regular expression
 * @param preg: if following preg or ereg (now only supports preg)
 * @param id
 *            : id of node associated with this auto; used for debugging
 *            purposes only
 * @return
 */
StrangerAutomaton* StrangerAutomaton::regExToAuto(std::string phpRegexOrig,
                                                  bool preg, int id) {
    		debug(stringbuilder() << "============");
    		debug(stringbuilder() << id <<  " = regExToAuto(" << phpRegexOrig << ") -- start");
    
    		StrangerAutomaton* retMe = NULL;
    
    		if (phpRegexOrig.empty()) {
                throw invalid_argument(stringbuilder() << "regular expression is empty");
//    			cerr << "regular expression is empty so using makeEmptyString";
//    			retMe = StrangerAutomaton::makeEmptyString(id);
    		} else {
                std::string phpRegex = phpRegexOrig;
    			if (preg) {
    				// if the preg regex is not delimited...
    				char first = phpRegex[0];
    				if (!first == StrangerAutomaton::slash){
    					throw invalid_argument(stringbuilder() << "Undelimited preg regexp: \"" << phpRegexOrig << "\"");
    				}
                    std::string::size_type last = phpRegex.substr(1).find_last_of(StrangerAutomaton::slash);
                    if (last == std::string::npos)
       					throw invalid_argument(stringbuilder() << "Undelimited preg regexp: \"" << phpRegexOrig << "\"");
    				// peel off delimiter
    				phpRegexOrig = phpRegex.substr(1, last);
    				debug(stringbuilder() << id <<  ": regular expression after removing delimeters = \""
    						<< phpRegexOrig << "\"");
    			}
                RegExp::restID();// for debugging purposes only
    			// following loop is to convert a special character from its representation
    			// to actual character.
    			// Example: convert \x41 to A
//    			while(phpRegexOrig.matches(".*(\\\\x\\w\\w).*")){
//    				Pattern p = Pattern.compile(".*(\\\\x\\w\\w).*");
//    				std::string tempRegExp = phpRegexOrig;
//    				Matcher m = p.matcher(tempRegExp);
//    				m.find();
//    				std::string hexChar = m.group(1);
//    				char c = (char)Integer.valueOf(hexChar.substring(2,4), 16).intValue();
//    				phpRegexOrig = phpRegexOrig.replace(hexChar, "\\" << c);
//    			}
    			RegExp* regExp = new RegExp(phpRegexOrig, RegExp::NONE);
                std::string regExpStringVal;
    			debug(stringbuilder() << id <<  ": regExToString = "
    					<< regExp->toStringBuilder(regExpStringVal));
    			retMe = regExp->toAutomaton();
    		}
    
    		debug(stringbuilder() << id <<  " = regExToAuto(" << phpRegexOrig << ") -- end");
    		debug(stringbuilder() << "============");
    		{
    			retMe->setID(id);
    			retMe->debugAutomaton();
    		}
    		return retMe;
}

/**
 * Parses a PHP regular expression and converts it into stranger automaton.
 * Regualr expression should follow preg syntax.
 * For allowed regular expressions check {@link RegExp}.
 * @param phpRegexOrig: the string literal representing the regular expression
 * @return
 */
StrangerAutomaton* StrangerAutomaton::regExToAuto(std::string phpRegexOrig) {
    return regExToAuto(phpRegexOrig, true, traceID);
}

/**
 * constructs a StrangerAutomaton that accepts the result of replacing every
 * occurrence of a string of patternAuto language in subjectAuto language
 * with replaceStr. var and indices is the depth of the BDD (number of
 * variables in the BDD) and ordering of them
 * If patternAuto or subjectAuto are bottom then it will throw StrangerAutomatonException
 * IF patternAuto is top then it will throw StrangerAutomatonException
 * If subjectAuto is top then it will return top as replacing something in top which is not
 * guaranteed to be a string variable may cause errors
 *
 * @param patternAuto
 *            : search auto (of type StrangerAutomaton) , replace substrings
 *            which match elements in L(searchAuto)
 * @param replaceStr
 *            : the replace string (of type String) , replace with this
 *            string
 * @param subjectAuto
 *            : target auto (of type StrangerAutomaton) , replace substrings
 *            in L(subjectAuto)
 * @param id
 *            : id of node associated with this auto; used for debugging
 *            purposes only
 */
//TODO: merge this with str_replace as we no longer need preg
StrangerAutomaton* StrangerAutomaton::reg_replace(StrangerAutomaton* patternAuto,
                                                  std::string replaceStr, StrangerAutomaton* subjectAuto, bool preg,
                                                  int id) {
    
    debug(stringbuilder() << id <<  " = reg_replace(" << patternAuto->ID << ", " << replaceStr << ", " << subjectAuto->ID << ")");
    // Note: the replaceAuto parameter should be of type
    // Automaton not String. We changed it
    // to use the replace function from StrangerLibrary.
    // TODO: Otherwise we need a method to accept all three parameters as
    // automaton in Stranger Library
    debug(stringbuilder() << "calling reg_replace with the following order (" << subjectAuto->ID << ", " << patternAuto->ID << ", " << replaceStr << ")");
    if (patternAuto->isBottom() || subjectAuto->isBottom())
        throw new StrangerAutomatonException(
                                             "SNH: In StrangerAutoatmon.reg_replace: either patternAuto or subjectAuto is bottom element (phi) which can not be used in replace.");
    else if (patternAuto->isTop())
        throw new StrangerAutomatonException(
                                             "SNH: In StrangerAutoatmon.reg_replace: patternAuto is top (indicating that the variable may no longer be of type string) and can not be used in replacement");
    else if (subjectAuto->isTop())
        throw new StrangerAutomatonException(
                                             "SNH: In StrangerAutoatmon.reg_replace: subjectAuto is top (indicating that the variable may no longer be of type string) and can not be used in replacement");
    
    debugToFile(stringbuilder() << "M[" << (traceID) << "] = dfa_replace_extrabit(M["<< subjectAuto->autoTraceID  << "], M[" << patternAuto->autoTraceID << "], \"" << replaceStr << "\" , NUM_ASCII_TRACKS, indices_main);//"<<id << " = reg_replace(" << patternAuto->ID << ", " << replaceStr
				<< ", " << subjectAuto->ID << ")");
    perfInfo.numOfReplace++;
    long start = PerfInfo::currentTimeMillis();
    
    StrangerAutomaton* retMe = new StrangerAutomaton(
                                                     dfa_replace_extrabit(subjectAuto->dfa,
                                                                          patternAuto->dfa, StrangerAutomaton::strToCharStar(replaceStr),
                                                                          num_ascii_track,
                                                                          indices_main));
    
    long stop = PerfInfo::currentTimeMillis();
    perfInfo.replaceTime += (stop - start);
    
    {
        retMe->ID = id;
        retMe->debugAutomaton();
    }
    return retMe;
}

/**
 * constructs a StrangerAutomaton that accepts the result of replacing every
 * occurrence of a string of patternAuto language in subjectAuto language
 * with replaceStr. var and indices is the depth of the BDD (number of
 * variables in the BDD) and ordering of them
 *
 * @param patternAuto
 *            : search auto (of type StrangerAutomaton) , replace substrings
 *            which match elements in L(searchAuto)
 * @param replaceStr
 *            : the replace string (of type String) , replace with this
 *            string
 * @param subjectAuto
 *            : target auto (of type StrangerAutomaton) , replace substrings
 *            in L(subjectAuto)
 */
//TODO: merge this with str_replace as we no longer need preg
StrangerAutomaton* StrangerAutomaton::reg_replace(StrangerAutomaton* patternAuto,
                                                  std::string replaceStr, StrangerAutomaton* subjectAuto) {
    return reg_replace(patternAuto, replaceStr, subjectAuto, true, traceID);
}

/**
 * constructs a StrangerAutomaton that accepts the result of replacing every
 * occurrence of a string of searchAuto language in subjectAuto language
 * with replaceStr. var and indices is the depth of the BDD (number of
 * variables in the BDD) and ordering of them
 *
 * @param searchAuto
 *            : search auto (of type StrangerAutomaton) , replace substrings
 *            which match elements in L(searchAuto)
 * @param replaceStr
 *            : the replace string (of type String) , replace with this
 *            string
 * @param subjectAuto
 *            : target auto (of type StrangerAutomaton) , replace substrings
 *            in L(subjectAuto)
 * @param id
 *            : id of node associated with this auto; used for debugging
 *            purposes only
 */
StrangerAutomaton* StrangerAutomaton::str_replace(StrangerAutomaton* searchAuto,
                                                  std::string replaceStr, StrangerAutomaton* subjectAuto, int id) {
    
    debug(stringbuilder() << id <<  " = str_replace(" << searchAuto->ID << ", " << replaceStr << ", " << subjectAuto->ID << ")");
    // Note: the original replaceAuto parameter in FSAAutomaton is of type
    // Automaton not String. We changed it
    // to use the replace function from StrangerLibrary which only accepts a
    // string literal.
    debug(stringbuilder() << "calling str_replace with the following order (" << subjectAuto->ID << ", " << searchAuto->ID << ", " << replaceStr << ")");
    
    if (searchAuto->isBottom() || subjectAuto->isBottom())
        throw new StrangerAutomatonException(
                                             "SNH: In StrangerAutoatmon.str_replace: either searchAuto or subjectAuto is bottom element (phi) which can not be used in replace.");
    else if (searchAuto->isTop())
        throw new StrangerAutomatonException(
                                             "SNH: In StrangerAutoatmon.str_replace: searchAuto is top (indicating that the variable may no longer be of type string) and can not be used in replacement");
    else if (subjectAuto->isTop())
        throw new StrangerAutomatonException(
                                             "SNH: In StrangerAutoatmon.str_replace: subjectAuto is top (indicating that the variable may no longer be of type string) and can not be used in replacement");
    
    
    
    debugToFile(stringbuilder() << "M[" << (traceID) << "] = dfa_replace_extrabit(M["<< subjectAuto->autoTraceID  << "], M[" << searchAuto->autoTraceID << "], \"" << replaceStr << "\" , NUM_ASCII_TRACKS, indices_main);//"<<id << " = str_replace(" << searchAuto->ID << ", " << replaceStr << ", "
				<< subjectAuto->ID << ")");
    perfInfo.numOfReplace++;
    long start = PerfInfo::currentTimeMillis();
    
    StrangerAutomaton* retMe = new StrangerAutomaton(
                                                     dfa_replace_extrabit(subjectAuto->dfa,
                                                                          searchAuto->dfa, StrangerAutomaton::strToCharStar(replaceStr),
                                                                          num_ascii_track,
                                                                          indices_main));
    
    long stop = PerfInfo::currentTimeMillis();
    perfInfo.replaceTime += (stop - start);
    
    {
        retMe->ID = id;
        retMe->debugAutomaton();
    }
    return retMe;
}

/**
 * constructs a StrangerAutomaton that accepts the result of replacing every
 * occurrence of a string of searchAuto language in subjectAuto language
 * with replaceStr. var and indices is the depth of the BDD (number of
 * variables in the BDD) and ordering of them
 *
 * @param searchAuto
 *            : search auto (of type StrangerAutomaton) , replace substrings
 *            which match elements in L(searchAuto)
 * @param replaceStr
 *            : the replace string (of type String) , replace with this
 *            string
 * @param subjectAuto
 *            : target auto (of type StrangerAutomaton) , replace substrings
 *            in L(subjectAuto)
 */
StrangerAutomaton* StrangerAutomaton::str_replace(StrangerAutomaton* searchAuto,
                                                  std::string replaceStr, StrangerAutomaton* subjectAuto) {
    return str_replace(searchAuto, replaceStr, subjectAuto, traceID);
}

//***************************************************************************************
//*                                  Backward Replacement                               *
//***************************************************************************************

/**
 * This is for backward analysis to compute the preimage of replace
 * @param searchAuto
 *            : search auto (of type StrangerAutomaton) , replace substrings
 *            which match elements in L(searchAuto)
 * @param replaceStr
 *            : the replace string (of type String) , replace with this
 *            string
 * @param id
 *            : id of node associated with this auto; used for debugging
 *            purposes only
 * @return
 */
StrangerAutomaton* StrangerAutomaton::preReplace(StrangerAutomaton* searchAuto,
                                                 std::string replaceString, int id) {
    debug(stringbuilder() << id <<  " = preReplace("  << this->ID <<  ", " << searchAuto->ID << ")");
    if (searchAuto->isBottom() || this->isBottom())
        throw new StrangerAutomatonException(
                                             "SNH: In StrangerAutoatmon.preReplace: either searchAuto or subjectAuto is bottom element (phi) which can not be used in replace.");
    else if (searchAuto->isTop())
        throw new StrangerAutomatonException(
                                             "SNH: In StrangerAutoatmon.preReplace: searchAuto is top (indicating that the variable may no longer be of type string) and can not be used in replacement");
    else if (this->isTop())
        throw new StrangerAutomatonException(
                                             "SNH: In StrangerAutoatmon.preReplace: subjectAuto is top (indicating that the variable may no longer be of type string) and can not be used in replacement");
    
    
    debugToFile(stringbuilder() << "M[" << (traceID) << "] = dfa_pre_replace_str(M[" << this->autoTraceID << "], M[" << searchAuto->autoTraceID << "], \"" << replaceString << "\" , NUM_ASCII_TRACKS, indices_main);//"<<id << " = preReplace("  << this->ID <<  ", " << searchAuto->ID << ")");
    perfInfo.numOfPreReplace++;
    long start = PerfInfo::currentTimeMillis();
    
    StrangerAutomaton* retMe = new StrangerAutomaton(
                                                     dfa_pre_replace_str(this->dfa,
                                                                         searchAuto->dfa, StrangerAutomaton::strToCharStar(replaceString),
                                                                         num_ascii_track,
                                                                         indices_main));
    
    long stop = PerfInfo::currentTimeMillis();
    perfInfo.prereplaceTime += (stop - start);
    
    {
        retMe->setID(id);
        retMe->debugAutomaton();
    }
    return retMe;
}

/**
 * This is for backward analysis to compute the preimage of replace
 * @param searchAuto
 *            : search auto (of type StrangerAutomaton) , replace substrings
 *            which match elements in L(searchAuto)
 * @param replaceStr
 *            : the replace string (of type String) , replace with this
 *            string
 * @return
 */
StrangerAutomaton* StrangerAutomaton::preReplace(StrangerAutomaton* searchAuto,
                                                 std::string replaceString) {
    return this->preReplace(searchAuto, replaceString, traceID);
}


//***************************************************************************************
//*                                  Length Operations                                  *
//*									-------------------									*
//* These operations retrieve or restrict length of current automaton language.			*
//***************************************************************************************

StrangerAutomaton* StrangerAutomaton::getUnaryAutomaton(int id){
    debug(stringbuilder() << id <<  " = dfa_string_to_unaryDFA("  << this->ID << ")");
    StrangerAutomaton* retMe = new StrangerAutomaton(dfa_string_to_unaryDFA(this->dfa, num_ascii_track, indices_main));
    retMe->ID = id;
    retMe->debugAutomaton();
    return retMe;
}


StrangerAutomaton* StrangerAutomaton::restrictLengthByOtherAutomaton(StrangerAutomaton* otherAuto, int id){
    StrangerAutomaton* uL = otherAuto->getUnaryAutomaton();
    StrangerAutomaton* retMe = this->restrictLengthByUnaryAutomaton(uL);
    delete uL;
    retMe->ID = id;
    retMe->debugAutomaton();
    return retMe;
}

StrangerAutomaton* StrangerAutomaton::restrictLengthByOtherAutomatonFinite(StrangerAutomaton *otherAuto, int id){
     P_DFAFiniteLengths pDFAFiniteLengths = dfaGetLengthsFiniteLang(otherAuto->dfa, num_ascii_track, indices_main);
    unsigned *lengths = pDFAFiniteLengths->lengths;
    const unsigned size = pDFAFiniteLengths->size;
    int i;
    for(i = 0; i < size; i++)
        cout << lengths[i] << ", ";
    cout << endl;
    vector<unsigned> vec(lengths, lengths + size);
    debug(stringbuilder() << id <<  " = dfaRestrictByFiniteLengths("  << this->ID << ", " << otherAuto->ID << ")");
//    cout << "lengths are: " << vec << endl;
    StrangerAutomaton* retMe = new StrangerAutomaton(dfaRestrictByFiniteLengths(this->dfa, lengths, size, false, num_ascii_track, indices_main));
    retMe->ID = id;
    retMe->debugAutomaton();
    
    free(pDFAFiniteLengths->lengths);
    free(pDFAFiniteLengths);
    
    return retMe;
}


StrangerAutomaton* StrangerAutomaton::restrictLengthByUnaryAutomaton(StrangerAutomaton* uL, int id){
    debug(stringbuilder() << id <<  " = dfa_restrict_by_unaryDFA("  << this->ID << ", " << uL->ID << ")");
    StrangerAutomaton* retMe = new StrangerAutomaton(dfa_restrict_by_unaryDFA(this->dfa, uL->dfa, num_ascii_track, indices_main));
    retMe->ID = id;
    retMe->debugAutomaton();
    return retMe;
}


//***************************************************************************************
//*                                  Automata checks                                    *
//*									-------------------									*
//* These operations only check current automaton without creating a new one.			*
//***************************************************************************************
/**
 * returns true if L(this auto) intersect L(auto) != phi (empty language)
 * @param auto
 * @param id1
 *            : id of node associated with this auto; used for debugging
 *            purposes only
 * @param id2
 *            : id of node associated with this auto; used for debugging
 *            purposes only
 * @return
 */
bool StrangerAutomaton::checkIntersection(StrangerAutomaton* otherAuto, int id1, int id2) {
    std::string debugStr = stringbuilder() << "checkIntersection("  << this->ID <<  ", " << otherAuto->ID << ") = ";
    
    if (this->isTop() || otherAuto->isTop()){
        debug(stringbuilder() << debugStr << "true");
        return true;
    } else if (this->isBottom() || otherAuto->isBottom()){
        debug(stringbuilder() << debugStr << "false");
        return false;
    }
    
    
    debugToFile(stringbuilder() << "check_intersection(M[" << this->autoTraceID << "],M["<< otherAuto->autoTraceID  << "], NUM_ASCII_TRACKS, indices_main);//check_intersection("  << this->ID <<  ", " << otherAuto->ID << ")");
    int result = check_intersection(this->dfa,
                                    otherAuto->dfa, num_ascii_track,
                                    indices_main);
    
    {
        debug(stringbuilder() << debugStr <<  (result == 0 ? false : true));
    }
    
    if (result == 0)
        return false;
    else if (result == 1)
        return true;
    else
        // TODO: we should have our own exception for StrangerAutomaton
        throw new std::runtime_error(
                                     "Error in checkIntersection result for StrangerAutomaton.");
}

/**
 * returns true if L(this auto) intersect L(auto) != phi (empty language)
 * @param auto
 * @return
 */
bool StrangerAutomaton::checkIntersection(StrangerAutomaton* otherAuto) {
    return this->checkIntersection(otherAuto, -1, -1);
}

/**
 * return true if parameter auto includes this otherAuto-> i.e. returns true if L(this auto)
 * is_subset_of L(parameter auto)
 *
 * @param auto
 * @param id1
 *            : id of node associated with this auto; used for debugging
 *            purposes only
 * @param id2
 *            : id of node associated with this auto; used for debugging
 *            purposes only * @return
 */
bool StrangerAutomaton::checkInclusion(StrangerAutomaton* otherAuto, int id1, int id2) {
    std::string debugStr = stringbuilder() << "checkInclusion("  << this->ID <<  ", " << otherAuto->ID << ") = ";
    // phi is always a subset of any other set, top is always superset of anything
    if (this->isBottom() || otherAuto->isTop()){
        debug(stringbuilder() << debugStr << "true");
        return true;
    } else if (otherAuto->isBottom() || this->isTop()){
        debug(stringbuilder() << debugStr << "false");
        return false;
    }
    
    debugToFile(stringbuilder() << "check_inclusion(M[" << this->autoTraceID << "],M["<< otherAuto->autoTraceID  << "], NUM_ASCII_TRACKS, indices_main);//check_inclusion("  << this->ID <<  ", " << otherAuto->ID << ")");
    int result = check_inclusion(this->dfa,
                                 otherAuto->dfa, num_ascii_track,
                                 indices_main);
    
    {
        debug(stringbuilder() << debugStr <<  (result == 0 ? false : true));
    }
    
    if (result == 0)
        return false;
    else if (result == 1){
        return true;
    }
    
    else
        // TODO: we should have our own exception for StrangerAutomaton
        throw new std::runtime_error(
                                     "Error in checkInclusion result for StrangerAutomaton.");
}

/**
 * return true if parameter auto includes this otherAuto-> i.e. returns true if L(this auto)
 * is_subset_of L(parameter auto)
 *
 * @param auto
 * @return
 */
bool StrangerAutomaton::checkInclusion(StrangerAutomaton* otherAuto) {
    return this->checkInclusion(otherAuto, -1, -1);
}

/**
 * returns true if this auto is equivalent to parameter otherAuto-> i.e. returns true if
 * L(parameter auto) == L(this auto)
 *
 * @param auto
 * @param id1
 *            : id of node associated with this auto; used for debugging
 *            purposes only
 * @param id2
 *            : id of node associated with this auto; used for debugging
 *            purposes only * @return
 */
bool StrangerAutomaton::checkEquivalence(StrangerAutomaton* otherAuto, int id1, int id2) {
    std::string debugStr = stringbuilder() << "checkEquivalence("  << this->ID <<  ", " << otherAuto->ID << ") = ";
    
    if ((this->isTop() && otherAuto->isTop()) || (this->isBottom() && otherAuto->isBottom())){
        debug(stringbuilder() << debugStr << "true");
        return true;
    }
    else if (this->isTop() || this->isBottom() || otherAuto->isTop() || otherAuto->isBottom()){
        debug(stringbuilder() << debugStr << "false");
        return false;
    }
    
    debugToFile(stringbuilder() << "check_equivalence(M[" << this->autoTraceID << "],M["<< otherAuto->autoTraceID  << "], NUM_ASCII_TRACKS, indices_main);//check_equivalence("  << this->ID <<  ", " << otherAuto->ID << ")");
    int result = check_equivalence(this->dfa,
                                   otherAuto->dfa, num_ascii_track,
                                   indices_main);
    
    {
        debug(stringbuilder() << debugStr << (result == 0 ? false : true));
    }
    
    if (result == 0)
        return false;
    else if (result == 1)
        // They should both either include or exclude empty string
        //			return (this->empty == otherAuto->empty);
        return true;
    else
        // TODO: we should have our own exception for StrangerAutomaton
        throw new std::runtime_error(
                                     "Error in checkInclusion result for StrangerAutomaton.");
}

/**
 * returns true if this auto is equivalent to parameter otherAuto i.e. returns true if
 * L(parameter auto) == L(this auto)
 *
 * @param auto
 * @return
 */
bool StrangerAutomaton::checkEquivalence(StrangerAutomaton* otherAuto) {
    return this->checkEquivalence(otherAuto, -1, -1);
}

/**
 * returns true (1) if {|w| < n: w elementOf L(M) && n elementOf Integers}
 * In other words length of all strings in the language is bounded by a value n
 */
bool StrangerAutomaton::isLengthFinite(){
    std::string debugString = stringbuilder() << "isLengthFinite("  << this->ID << ") = ";
    int result = ::isLengthFiniteTarjan(this->dfa, num_ascii_track, indices_main);
    debug(stringbuilder() << debugString << ( result == 0 ? false : true ));
    if (result == 0)
        return false;
    else
        return true;
}

/**
 * returns the result of this->checkEquivalence(other)
 */
bool StrangerAutomaton::equals(StrangerAutomaton* otherAuto) {
    return (otherAuto != NULL) &&
    this->checkEquivalence(otherAuto);
}

/**
 * returns true if this auto is empty. i.e. returns true if
 * L(this auto) == phi (empty set)
 *
 * @param id1
 *            : id of node associated with this auto; used for debugging
 *            purposes only
 *
 */
bool StrangerAutomaton::checkEmptiness() {
    std::string debugStr = stringbuilder() << "checkEmptiness("  << this->ID <<  ") = ";
    if (this->isBottom()){
        debug(stringbuilder() << debugStr << "true");
        return true;
    }
    else if (this->isTop()){
        debug(stringbuilder() << debugStr << "false");
        return false;
    }
    
    debugToFile(stringbuilder() << "check_emptiness(M[" << this->autoTraceID << "], NUM_ASCII_TRACKS, indices_main);//check_emptiness("  << this->ID <<  ")");
    int result = check_emptiness(this->dfa, num_ascii_track,
                                 indices_main);
    {
        debug(stringbuilder() << debugStr << (result == 0 ? false : true));
    }
    
    if (result == 0)
        return false;
    else if (result == 1)
        // if it contains empty string then it is not Phi
        return true;
    else
        // TODO: we should have our own exception for StrangerAutomaton
        throw new std::runtime_error(
                                     "Error in checkEmptiness result for StrangerAutomaton.");
}


/**
 * returns true if this L(automaton) == bottom
 * if you need to check if the language is actual phi use {@link checkEmptiness}
 */
bool StrangerAutomaton::isEmpty() {
    return checkEmptiness();
}


/**
 * check if this automaton only accepts empty string i.e. string of length
 * 0.
 *
 * @return
 */
bool StrangerAutomaton::checkEmptyString() {
    if (this->isBottom() || this->isTop())
        return false;
    debugToFile(stringbuilder() << "checkEmptyString(M[" << this->autoTraceID << "]);//checkEmptyString("  << this->ID <<  ")");
    if (::checkEmptyString(this->dfa) == 1)
        return true;
    else
        return false;
}

bool StrangerAutomaton::isSingleton() {
    return (::isSingleton(this->dfa, num_ascii_track, indices_main) != NULL);
}

string StrangerAutomaton::getStr(){
    char* result = ::isSingleton(this->dfa, num_ascii_track, indices_main);
    if (result == NULL){
        throw runtime_error("Trying to get a string for an automaton with a nonSingleton language.");
    }
    string retMe(result);
    return retMe;
}

/**
 * check if this automaton represents the bottom of the lattice.
 * @return
 */
bool StrangerAutomaton::isBottom() {
    //TODO: checkEmptiness causes lots of crashes so be careful here
    return (this->bottom == true);
}

/**
 * check if this automaton represents the top of the lattice.
 * @return
 */
bool StrangerAutomaton::isTop() {
    return (this->top == true);
}


StrangerAutomaton* StrangerAutomaton::toUpperCase(int id){
    debug(stringbuilder() << id <<  " = dfaToUpperCase("  << this->ID << ")");
    StrangerAutomaton* retMe = new StrangerAutomaton(dfaToUpperCase(this->dfa, num_ascii_track, indices_main));
    retMe->setID(id);
    retMe->debugAutomaton();
    return retMe;
}

StrangerAutomaton* StrangerAutomaton::toLowerCase(int id){
    debug(stringbuilder() << id <<  " = dfaToLowerCase("  << this->ID << ")");
    StrangerAutomaton* retMe = new StrangerAutomaton(dfaToLowerCase(this->dfa, num_ascii_track, indices_main));
    retMe->setID(id);
    retMe->debugAutomaton();
    return retMe;
}

StrangerAutomaton* StrangerAutomaton::preToUpperCase(int id){
    debug(stringbuilder() << id <<  " = dfaPreToUpperCase("  << this->ID << ")");
    StrangerAutomaton* retMe = new StrangerAutomaton(dfaPreToUpperCase(this->dfa, num_ascii_track, indices_main));
    retMe->setID(id);
    retMe->debugAutomaton();
    return retMe;
}

StrangerAutomaton* StrangerAutomaton::preToLowerCase(int id){
    debug(stringbuilder() << id <<  " = dfaPreToLowerCase("  << this->ID << ")");
    StrangerAutomaton* retMe = new StrangerAutomaton(dfaPreToLowerCase(this->dfa, num_ascii_track, indices_main));
    retMe->setID(id);
    retMe->debugAutomaton();
    return retMe;
}

StrangerAutomaton* StrangerAutomaton::trimSpaces(int id){
    debug(stringbuilder() << id <<  " = dfaTrim(' ', "  << this->ID << ")");
    StrangerAutomaton* retMe = new StrangerAutomaton(dfaTrim(this->dfa, ' ', num_ascii_track, indices_main));
    retMe->setID(id);
    retMe->debugAutomaton();
    return retMe;
}
StrangerAutomaton* StrangerAutomaton::trimSpacesLeft(int id){
    debug(stringbuilder() << id <<  " = dfaLeftTrim(' ', "  << this->ID << ")");
    StrangerAutomaton* retMe = new StrangerAutomaton(dfaLeftTrim(this->dfa, ' ', num_ascii_track, indices_main));
    retMe->setID(id);
    retMe->debugAutomaton();
    return retMe;
}
StrangerAutomaton* StrangerAutomaton::trimSpacesRight(int id){
    debug(stringbuilder() << id <<  " = dfaRightTrim(' ', "  << this->ID << ")");
    StrangerAutomaton* retMe = new StrangerAutomaton(dfaRightTrim(this->dfa, ' ', num_ascii_track, indices_main));
    retMe->setID(id);
    retMe->debugAutomaton();
    return retMe;
}
StrangerAutomaton* StrangerAutomaton::trim(char c, int id){
    debug(stringbuilder() << id <<  " = dfaTrim(" << this->ID << "," << c << ")");
    StrangerAutomaton* retMe = new StrangerAutomaton(dfaTrim(this->dfa, c, num_ascii_track, indices_main));
    retMe->setID(id);
    retMe->debugAutomaton();
    return retMe;
}
StrangerAutomaton* StrangerAutomaton::trimLeft(char c, int id){
    debug(stringbuilder() << id <<  " = dfaLeftTrim(" << this->ID << "," << c << ")");
    StrangerAutomaton* retMe = new StrangerAutomaton(dfaLeftTrim(this->dfa, c, num_ascii_track, indices_main));
    retMe->setID(id);
    retMe->debugAutomaton();
    return retMe;
}
StrangerAutomaton* StrangerAutomaton::trimRight(char c, int id){
    debug(stringbuilder() << id <<  " = dfaRightTrim(" << this->ID << "," << c << ")");
    StrangerAutomaton* retMe = new StrangerAutomaton(dfaRightTrim(this->dfa, c, num_ascii_track, indices_main));
    retMe->setID(id);
    retMe->debugAutomaton();
    return retMe;
}
StrangerAutomaton* StrangerAutomaton::trim(char chars[], int id){
    debug(stringbuilder() << id <<  " = dfaTrimSet(" << this->ID << ",chars )\n");
    StrangerAutomaton* retMe = new StrangerAutomaton(dfaTrimSet(this->dfa, chars, (int)strlen(chars), num_ascii_track, indices_main));
    retMe->setID(id);
    retMe->debugAutomaton();
    return retMe;
}
//	    StrangerAutomaton* StrangerAutomaton::trimLeft(char chars[]){
//	    	return new StrangerAutomaton(dfaTrimLeftSet(this->dfa, chars, strlen(chars), num_ascii_track, indices_main));
//	    }
//	    StrangerAutomaton* StrangerAutomaton::trimRight(char chars[]){
//	    	return new StrangerAutomaton(dfaTrimSet(this->dfa, chars, strlen(chars), num_ascii_track, indices_main));
//	    }

StrangerAutomaton* StrangerAutomaton::preTrimSpaces(int id){
    debug(stringbuilder() << id <<  " = dfaPreTrim(" << this->ID << ",' ')\n");
    StrangerAutomaton* retMe = new StrangerAutomaton(dfaPreTrim(this->dfa, ' ', num_ascii_track, indices_main));
    retMe->setID(id);
    retMe->debugAutomaton();
    return retMe;
}

StrangerAutomaton* StrangerAutomaton::addslashes(StrangerAutomaton* subjectAuto, int id)
{
    
    debug(stringbuilder() << id << " = addSlashes(" << subjectAuto->ID << ");");
    StrangerAutomaton* retMe = new StrangerAutomaton(dfaAddSlashes(subjectAuto->dfa, num_ascii_track, indices_main));
    {
        retMe->ID = id;
        retMe->debugAutomaton();
    }
    
    return retMe;
}



StrangerAutomaton* StrangerAutomaton::pre_addslashes(StrangerAutomaton* subjectAuto, int id)
{
    
	debug(stringbuilder() << id << " = pre_addSlashes(" << subjectAuto->ID << ");");
	StrangerAutomaton* retMe = new StrangerAutomaton(dfaPreAddSlashes(subjectAuto->dfa, num_ascii_track, indices_main));
	{
		retMe->ID = id;
        retMe->debugAutomaton();
	}
    
	return retMe;
}

StrangerAutomaton* StrangerAutomaton::htmlSpecialChars(StrangerAutomaton* subjectAuto, int id)
{
    
    debug(stringbuilder() << id << " = htmlSpecialChars(" << subjectAuto->ID << ");");
    StrangerAutomaton* retMe = new StrangerAutomaton(dfaHtmlSpecialChars(subjectAuto->dfa, num_ascii_track, indices_main));
    {
        retMe->ID = id;
        retMe->debugAutomaton();
    }
    
    return retMe;
}



StrangerAutomaton* StrangerAutomaton::preHtmlSpecialChars(StrangerAutomaton* subjectAuto, int id)
{
    
	debug(stringbuilder() << id << " = preHtmlSpecialChars(" << subjectAuto->ID << ");");
	StrangerAutomaton* retMe = new StrangerAutomaton(dfaPreHtmlSpecialChars(subjectAuto->dfa, num_ascii_track, indices_main));
	{
		retMe->ID = id;
        retMe->debugAutomaton();
	}
    
	return retMe;
}


StrangerAutomaton* StrangerAutomaton::stripslashes(StrangerAutomaton* subjectAuto, int id)
{
    StrangerAutomaton *sigmaStar = StrangerAutomaton::regExToAuto("/(.*('|\\\"|\\\\).*)+/", true, id);
    sigmaStar->toDotAscii(0);
    StrangerAutomaton *sigmaStarSlashed = addslashes(sigmaStar, id);
    sigmaStarSlashed->toDotAscii(0);
    delete sigmaStar;
    StrangerAutomaton *sigmaStarNotSlashed = sigmaStarSlashed->complement(id);
    sigmaStarNotSlashed->toDotAscii(0);
    StrangerAutomaton *slashed = subjectAuto->intersect(sigmaStarSlashed, id);
    slashed->toDotAscii(0);
    delete sigmaStarSlashed;
    StrangerAutomaton *notSlashed = subjectAuto->intersect(sigmaStarNotSlashed, id);
    notSlashed->toDotAscii(0);
    delete sigmaStarNotSlashed;
    StrangerAutomaton *slashedPre = pre_addslashes(slashed, id);
    slashedPre->toDotAscii(0);
    delete slashed;
    StrangerAutomaton *retMe = notSlashed->union_(slashedPre, id);
    delete slashedPre;
    {
		retMe->ID = id;
        retMe->debugAutomaton();
	}

    return retMe;
}

StrangerAutomaton* StrangerAutomaton::pre_stripslashes(StrangerAutomaton* subjectAuto, int id)
{
    StrangerAutomaton *slashed = addslashes(subjectAuto, id);
    StrangerAutomaton *result = slashed->union_(subjectAuto, id);
    delete slashed;
    return result;
}

StrangerAutomaton* StrangerAutomaton::mysql_real_escape_string(StrangerAutomaton* subjectAuto, int id)
{
    
    //    debug(::java::lang::StringBuilder().append(id)->append(" = mysql_real_escape_string(")
    //        ->append(npc(subjectAuto)->ID)
    //        ->append(") -- start")->toString());
    //    StrangerAutomaton* searchAuto = StrangerAutomaton::makeString("\\", int32_t(1));
    //    std::string replaceStr = "\\\\";
    //    StrangerAutomaton* retMe = str_replace(searchAuto, replaceStr, subjectAuto, int32_t(2));
    //    searchAuto = StrangerAutomaton::makeString("'", int32_t(3));
    //    replaceStr = "\\'";
    //    retMe = str_replace(searchAuto, replaceStr, retMe, int32_t(4));
    //    searchAuto = StrangerAutomaton::makeString("\"", int32_t(5));
    //    replaceStr = "\\\"";
    //    retMe = str_replace(searchAuto, replaceStr, retMe, int32_t(6));
    //    debug(::java::lang::StringBuilder().append(id)->append(" = mysql_real_escape_string(")
    //        ->append(npc(subjectAuto)->ID)
    //        ->append(") -- end")->toString());
    //{
    //        npc(subjectAuto)->ID = id;
    //        npc(subjectAuto)->debugAutomaton();
    //    }
    //
    //    return retMe;
	throw new std::runtime_error("not implemented");
    
}

StrangerAutomaton* StrangerAutomaton::pre_mysql_real_escape_string(StrangerAutomaton* subjectAuto, int id)
{
    
    //    debug(::java::lang::StringBuilder().append(id)->append(" = pre_mysql_real_escape_string(")
    //        ->append(npc(subjectAuto)->ID)
    //        ->append(") -- start")->toString());
    //    StrangerAutomaton* searchAuto = StrangerAutomaton::makeString("\\", int32_t(1));
    //    std::string replaceStr = "\\\\";
    //    StrangerAutomaton* retMe = npc(subjectAuto)->preReplace(searchAuto, replaceStr, int32_t(2));
    //    searchAuto = StrangerAutomaton::makeString("'", int32_t(5));
    //    replaceStr = "\\'";
    //    retMe = retMe->preReplace(searchAuto, replaceStr, int32_t(2));
    //    searchAuto = StrangerAutomaton::makeString("\"", int32_t(5));
    //    replaceStr = "\\\"";
    //    retMe = retMe->preReplace(searchAuto, replaceStr, int32_t(2));
    //    debug(::java::lang::StringBuilder().append(id)->append(" = pre_mysql_real_escape_string(")
    //        ->append(npc(subjectAuto)->ID)
    //        ->append(") -- end")->toString());
    //{
    //        npc(subjectAuto)->ID = id;
    //        npc(subjectAuto)->debugAutomaton();
    //    }
    //
    //    return retMe;
	throw new std::runtime_error("not implemented");
    
}

StrangerAutomaton* StrangerAutomaton::nl2br(StrangerAutomaton* subjectAuto, int id)
{
    
    //    debug(::java::lang::StringBuilder().append(id)->append(" = nl2br(")
    //        ->append(npc(subjectAuto)->ID)
    //        ->append(") -- start")->toString());
    //    StrangerAutomaton* searchAuto = StrangerAutomaton::makeString("\\n", int32_t(1));
    //    std::string replaceStr = "<br/>";
    //    StrangerAutomaton* retMe = StrangerAutomaton::str_replace(searchAuto, replaceStr, subjectAuto, int32_t(2));
    //    debug(::java::lang::StringBuilder().append(id)->append(" = nl2br(")
    //        ->append(npc(subjectAuto)->ID)
    //        ->append(") -- end")->toString());
    //{
    //        npc(subjectAuto)->ID = id;
    //        npc(subjectAuto)->debugAutomaton();
    //    }
    //
    //    return retMe;
	throw new std::runtime_error("not implemented");
    
}

StrangerAutomaton* StrangerAutomaton::pre_nl2br(StrangerAutomaton* subjectAuto, int id)
{
    
    //    debug(::java::lang::StringBuilder().append(id)->append(" = pre_nl2br(")
    //        ->append(npc(subjectAuto)->ID)
    //        ->append(") -- start")->toString());
    //    StrangerAutomaton* searchAuto = StrangerAutomaton::makeString("\\n", int32_t(1));
    //    std::string replaceStr = "<br/>";
    //    StrangerAutomaton* retMe = npc(subjectAuto)->preReplace(searchAuto, replaceStr, int32_t(2));
    //    debug(::java::lang::StringBuilder().append(id)->append(" = pre_nl2br(")
    //        ->append(npc(subjectAuto)->ID)
    //        ->append(") -- end")->toString());
    //{
    //        npc(subjectAuto)->ID = id;
    //        npc(subjectAuto)->debugAutomaton();
    //    }
    //
    //    return retMe;
	throw new std::runtime_error("not implemented");
    
}

StrangerAutomaton* StrangerAutomaton::getUndesiredSQLTest()
{
    
    //    debug("Building automaton for SQL vulnerabilities");
    //    debug("----------------------------------------");
    //    StrangerAutomaton* autoAttackPattern = regExToAuto("/.*'or 1=1'.*/", true, int32_t(0));
    //    debug("----------------------------------------");
    //    return autoAttackPattern;
	throw new std::runtime_error("not implemented");
    
}

StrangerAutomaton* StrangerAutomaton::getUndesiredXSSTest()
{
    //
    //    StrangerAutomaton* autoKleensStar = StrangerAutomaton::makeAnyString(int32_t(1));
    //    StrangerAutomaton* retMe = regExToAuto("/.*\\<SCRIPT .*\\>.*/", true, int32_t(0));
    //    return retMe;
	throw new std::runtime_error("not implemented");
    
}

StrangerAutomaton* StrangerAutomaton::getUndesiredMFETest()
{
    
    //    StrangerAutomaton* retMe = regExToAuto("//evil/", true, int32_t(0));
    //    return retMe;
	throw new std::runtime_error("not implemented");
    
}


//std::set<char> StrangerAutomaton::mincut(){
//    using namespace boost;
//    
//    int printSink = 1;
//    
//    std::set<char> result;
//    
//    DFA* a = this->dfa;
//    
//    /**********    SETUP GRAPH     **********/
//    
//    typedef adjacency_list_traits < vecS, vecS, directedS > Traits;
//    typedef adjacency_list < listS, vecS, directedS,
//    property < vertex_name_t, std::string >,
//    property < edge_capacity_t, long,
//    property < edge_residual_capacity_t, long,
//    property < edge_reverse_t, Traits::edge_descriptor > > > > Graph;
//    
//    Graph g;
//    
//    property_map < Graph, edge_capacity_t >::type
//    capacity = get(edge_capacity, g);
//    property_map < Graph, edge_reverse_t >::type reverse_edge = get(edge_reverse, g);
//    property_map < Graph, edge_residual_capacity_t >::type
//    residual_capacity = get(edge_residual_capacity, g);
//    
//    Traits::vertex_descriptor s, t;
//    
//    typedef typename graph_traits<Graph>::vertices_size_type vertices_size_type;
//    typedef typename graph_traits<Graph>::vertex_descriptor vertex_descriptor;
//    typedef typename graph_traits<Graph>::edge_descriptor edge_descriptor;
//    
//    cout << "digraph G {" << endl;
//
//    
//    /************   ADD VERTICES    *************/
//    
//    std::vector<vertex_descriptor> verts;
//    //number of nodes
//    long numOfNodes;
//    if (printSink == 0)
//        numOfNodes = a->ns - 1;
//    else
//        numOfNodes = a->ns;
//    for (long vi = 0; vi < numOfNodes; ++vi){
//        cout << vi << ";" << endl;
//        verts.push_back(add_vertex(g));
//    }
//    
//    
//    /**********   ADD EDGES FROM AUTOMATON   *********/
//    
//    paths state_paths, pp;
//	trace_descr tp;
//	int i, j, k, l, size, maxexp, sink, numAccept = 0, acceptState = -1;
//	pCharPair *buffer;//array of charpairs references
//	char *character;
//	pCharPair **toTrans;//array for all states, each entry is an array of charpair references
//	int *toTransIndecies;
//    int no_free_vars = StrangerAutomaton::getVar();
//    int *offsets = StrangerAutomaton::getIndices();
//    
//	sink = find_sink(a);
//	assert( sink > -1);//with reserved chars, sink is always reject even when negated
//    
//    
//	maxexp = 1<<no_free_vars;
//	buffer = (pCharPair*) malloc(sizeof(pCharPair) * maxexp);//max no of chars from Si to Sj = 2^no_free_vars
//	character = (char*) malloc((no_free_vars+1) * sizeof(char));
//	toTrans = (pCharPair**) malloc(sizeof(pCharPair*) * a->ns);//need this to gather all edges out to state Sj from Si
//	for (i = 0; i < a->ns; i++){
//		toTrans[i] = (pCharPair*) malloc(maxexp * sizeof(pCharPair));
//	}
//	toTransIndecies = (int*) malloc(a->ns * sizeof(int));//for a state Si, how many edges out to each state Sj
//    
//    
//	for (i = 0; i < a->ns; i++) {
//        
//        //check if accept state except for start state
//        if(i > 0 && a->f[i] == 1)
//            numAccept++;
//        
//		//get transitions out from state i
//		state_paths = pp = make_paths(a->bddm, a->q[i]);
//        
//		//init buffer
//		for (j = 0; j < a->ns; j++) {
//			toTransIndecies[j] = 0;
//		}
//		for (j = 0; j < maxexp; j++){
//			for (k = 0; k < a->ns; k++)
//				toTrans[k][j] = 0;
//			buffer[j] = 0;
//		}
//        
//		//gather transitions out from state i
//		//for each transition pp out from state i
//		while (pp) {
//			if (pp->to == sink && printSink == 0){
//				pp = pp->next;
//				continue;
//			}
//			//get mona character on transition pp
//			for (j = 0; j < no_free_vars; j++) {
//				for (tp = pp->trace; tp && (tp->index != offsets[j]);
//                     tp = tp->next)
//					;
//                
//				if (tp) {
//					if (tp->value)
//						character[j] = '1';
//					else
//						character[j] = '0';
//				} else
//					character[j] = 'X';
//			}
//			character[j] = '\0';
//			if (no_free_vars == 8){
//				//break mona character into ranges of ascii chars (example: "0XXX000X" -> [\s-!], [0-1], [@-A], [P-Q])
//				size = 0;
//				getTransitionChars(character, no_free_vars, buffer, &size);
//				//get current index
//				k = toTransIndecies[pp->to];
//				//print ranges
//				for (l = 0; l < size; l++) {
//					toTrans[pp->to][k++] = buffer[l];
//					buffer[l] = 0;//do not free just detach
//				}
//				toTransIndecies[pp->to] = k;
//			} else {
//                //				k = toTransIndecies[pp->to];
//                //				toTrans[pp->to][k] = (char*) malloc(sizeof(char) * (strlen(character) + 1));
//                //				strcpy(toTrans[pp->to][k], character);
//                //				toTransIndecies[pp->to] = k + 1;
//			}
//			pp = pp->next;
//		}
//        
//		//print transitions out of state i
//		for (j = 0; j < a->ns; j++) {
//			size = toTransIndecies[j];
//			if (size == 0 || (sink == j && printSink == 0))
//				continue;
//            long  cap = 0;
//			//for each trans k on char/range from i to j
//            //get capacity from a range
//			for (k = 0; k < size; k++) {
//                cap += toTrans[j][k]->last - toTrans[j][k]->first;
//			}
//            // build an edge from tail to head (since directed graph, tail is the tail of arrow and head is head of arrow)
//            int from, to;
//            if (printSink == 0 && i > sink)
//                from = i - 1;
//            else
//                from = i;
//            if (printSink == 0 && i > sink)
//                to = j - 1;
//            else
//                to = j;
//            
//            if (from == to)
//                continue;
//            
//            cout << from << " -> " << to << ";" << endl;
//            edge_descriptor e1, e2;
//            bool in1, in2;
//            boost::tie(e1, in1) = add_edge(verts[from], verts[to], g);
//            boost::tie(e2, in2) = add_edge(verts[to], verts[from], g);
//            if (!in1 || !in2) {
//                throw runtime_error(stringbuilder() << "while doing min cut, unable to add edge (" << from << "," << to << ")\n"
//                );
//            }
//            capacity[e1] = cap;
//            capacity[e2] = 0;
//            reverse_edge[e1] = e2;
//            reverse_edge[e2] = e1;
//		}
//		//for each state free charRange
//		//merge with loop above for better performance
//		for (j = 0; j < a->ns; j++){
//			if (j == sink && printSink == 0)
//				continue;
//			size = toTransIndecies[j];
//			for (k = 0; k < size; k++)
//				free(toTrans[j][k]);
//		}
//        
//        
//		kill_paths(state_paths);
//	}//end for each state
//    
//	free(character);
//	free(buffer);
//	for (i = 0; i < a->ns; i++){
//		free(toTrans[i]);
//	}
//	free(toTrans);
//	free(toTransIndecies);
//    
//    
//    /**********   SET SOURCE AND SINK   ************/
//    
//    // source == start state
//    s = verts[0];
//    // sink
//    if (numAccept == 0){
//        // no accept state or only one accept state which is the start state (empty string) --> no mincut
//        return result;
//    }
//    else if (numAccept == 1){
//        // only one accept state which is not the start state
//        for (i = 1; i < a->ns; i++){
//            //check if accept state
//            if(a->f[i] == 1){
//                acceptState = i;
//                break;
//            }
//        }
//        if (printSink == 0 && acceptState > sink)
//            acceptState--;
//        t = verts[acceptState];
//    }
//    else {
//        // more than one accept state not counting the start state
//        //1- add a new sink state
//        verts.push_back(add_vertex(g));
//        cout << numOfNodes << ";" << endl;
//        t = verts[numOfNodes];
//        //2- link accept states to new snik state with inifinite capacity
//        for (i = 1; i < a->ns; i++){
//            //check if accept state
//            if(a->f[i] == 1){
//                int from;
//                if (printSink == 0 && i > sink)
//                    from = i - 1;
//                else
//                    from = i;
//                cout << from << " -> " << numOfNodes << ";" << endl;
//                edge_descriptor e1, e2;
//                bool in1, in2;
//                boost::tie(e1, in1) = add_edge(verts[from], verts[numOfNodes], g);
//                boost::tie(e2, in2) = add_edge(verts[numOfNodes], verts[from], g);
//                if (!in1 || !in2) {
//                    throw runtime_error(stringbuilder() << "while doing min cut, unable to add edge (" << from << "," << numOfNodes << ")\n"
//                                        );
//                }
//                capacity[e1] = 1000;
//                capacity[e2] = 0;
//                reverse_edge[e1] = e2;
//                reverse_edge[e2] = e1;
//            }
//            
//        }
//    }
//    cout << "}" << endl;
//    //    read_dimacs_max_flow(g, capacity, reverse_edge, s, t, ifs);
//    
//#if defined(BOOST_MSVC) && BOOST_MSVC <= 1300
//    std::vector<default_color_type> color(num_vertices(g));
//    std::vector<Traits::edge_descriptor> pred(num_vertices(g));
//    long flow = edmonds_karp_max_flow
//    (g, s, t, capacity, residual_capacity, rev, &color[0], &pred[0]);
//#else
//    long flow = edmonds_karp_max_flow(g, s, t);
//#endif
//    
//    std::cout << "c  The total flow:" << std::endl;
//    std::cout << "s " << flow << std::endl << std::endl;
//    
//    std::cout << "c flow values:" << std::endl;
//    graph_traits < Graph >::vertex_iterator u_iter, u_end;
//    graph_traits < Graph >::out_edge_iterator ei, e_end;
//    for (tie(u_iter, u_end) = vertices(g); u_iter != u_end; ++u_iter)
//        for (tie(ei, e_end) = out_edges(*u_iter, g); ei != e_end; ++ei)
//            if (capacity[*ei] > 0)
//                std::cout << "f " << *u_iter << " " << target(*ei, g) << " "
//                << capacity[*ei] << " " << residual_capacity[*ei] << " "
//                << (capacity[*ei] - residual_capacity[*ei]) << std::endl;
//
////    write_graphviz(std::cout, g);
//    
//    return result;
//}



void StrangerAutomaton::printAutomaton()
{
	std::cout.flush();
    debugToFile(stringbuilder() << "dfaPrintVerbose(M[" << this->autoTraceID << "]);");
    dfaPrintVerbose(this->dfa);
    debugToFile(stringbuilder() << "flush_output();");
    std::cout.flush();
}

void StrangerAutomaton::printAutomatonVitals()
{
	std::cout.flush();
    debugToFile(stringbuilder() << "dfaPrintVitals(M[" << this->autoTraceID << "]);");
    dfaPrintVitals(this->dfa);
    debugToFile(stringbuilder() << "flush_output();");
    std::cout.flush();
}

/**
 * specifically done to be passed to dfaPrintGraphviz
 */
unsigned *StrangerAutomaton::getUnsignedIndices(int length){
	unsigned i;
	unsigned* indices;
	indices = new unsigned[length + 1];
	for (i = 0; i <= (unsigned)length; i++)
		indices[i] = i;
	return indices;
    
}


/**
 * Prints the current automaton to the out stream in a dot format (see Graphviz).
 * Unfortunately until now there is no interface to provide a file to the C library
 * to print the output into yet :-(
 */
void StrangerAutomaton::toDot()
{
	unsigned* indices_main_unsigned = getUnsignedIndices(num_ascii_track);
	std::cout.flush();
    debugToFile(stringbuilder() << "dfaPrintGraphviz(M[" << this->autoTraceID << "], NUM_ASCII_TRACKS, indices_main);//dfaPrintGraphviz( this->ID)");
    dfaPrintGraphviz(this->dfa, num_ascii_track, indices_main_unsigned);
    delete indices_main_unsigned;
    debugToFile(stringbuilder() << "flush_output();");
    std::cout.flush();
}

/**
 * Prints the current automaton to the out stream in a dot format (see Graphviz).
 * Unfortunately until now there is no interface to provide a file to the C library
 * to print the output into yet :-(
 */
void StrangerAutomaton::toDotAscii(int printSink)
{
	unsigned* indices_main_unsigned = getUnsignedIndices(num_ascii_track);
	std::cout.flush();
    debugToFile(stringbuilder() << "dfaPrintGraphviz(M[" << this->autoTraceID << "], NUM_ASCII_TRACKS, indices_main);//dfaPrintGraphviz( this->ID)");
    //if the automaton is the empty language then we must enable printing the sink
    // if there is one state and it is a rejecting state
    if (this->dfa->ns == 1 && this->dfa->f[0] == -1)
        printSink = 2;
    dfaPrintGraphvizAsciiRange(this->dfa, num_ascii_track, indices_main, printSink);
    delete indices_main_unsigned;
    debugToFile(stringbuilder() << "flush_output();");
    std::cout.flush();
}


int StrangerAutomaton::debugLevel = 1;

void StrangerAutomaton::debug(std::string s)
{
    
    if(debugLevel >= 1)
        std::cout << s << endl;
    
}

void StrangerAutomaton::debugAutomaton()
{
    if(debugLevel >= 3) {
        this->toDotAscii(0);
    }
}


int StrangerAutomaton::traceID = 0;


int StrangerAutomaton::baseTraceID = 0;

int StrangerAutomaton::tempTraceID = 0;

int StrangerAutomaton::baseTempTraceID = 0;



void StrangerAutomaton::resetTraceID()
{
    traceID = baseTraceID;
    tempTraceID = baseTempTraceID;
}

void StrangerAutomaton::openCtraceFile(std::string name)
{
    //
    //    try {
    //        resetTraceID();
    //        fstream_ = (new ::java::io::FileWriter(name));
    //        out_ = (new ::java::io::BufferedWriter(fstream_));
    //        npc(out_)->write("int* indices_main = (int *) allocateAscIIIndexWithExtraBit(NUM_ASCII_TRACKS);\nint i;\nDFA* M[1000];\nfor (i = 0; i < 1000; i++)\n\t M[i] = 0;\n");
    //    } catch (::java::lang::Exception* e) {
    //        throw (new StrangerAutomatonException("exec_trace.c file can not be opened."));
    //    }
}

void StrangerAutomaton::appendCtraceFile(std::string name)
{
    //
    //    try {
    //        resetTraceID();
    //        fstream_ = (new ::java::io::FileWriter(name, true));
    //        out_ = (new ::java::io::BufferedWriter(fstream_));
    //    } catch (::java::lang::Exception* e) {
    //        throw (new StrangerAutomatonException(::java::lang::StringBuilder().append("exec_trace.c file can not be opened.")->append(npc(e)->getMessage())->toString()));
    //    }
}

void StrangerAutomaton::closeCtraceFile()
{
    //
    //    if(fstream_ != NULL)
    //        try {
    //            npc(out_)->write("for (i = 0; i < 1000; i++)\n\tif (M[i] != 0){\n\t\tdfaFree(M[i]);\n\t\tM[i] = 0;\n}\nprintf(\"Finished execution.\");\n");
    //            npc(out_)->flush();
    //            npc(fstream_)->close();
    //        } catch (::java::io::IOException* e) {
    //            throw (new StrangerAutomatonException("Can not close file exec_trace.c"));
    //        }
    
}

void StrangerAutomaton::debugToFile(std::string str)
{
    //
    //    if(fstream_ == NULL) {
    //        std::string property = "java.io.tmpdir";
    //        std::string tempDir = ::java::lang::System::getProperty(property);
    //        openCtraceFile(::java::lang::StringBuilder().append(tempDir)->append("/stranger_automaton_exec_trace.c")->toString());
    //    }
    //    try {
    //        npc(out_)->write(::java::lang::StringBuilder().append(str)->append("\n")->toString());
    //        npc(out_)->flush();
    //    } catch (::java::io::IOException* e) {
    //        throw (new StrangerAutomatonException("Can not write to exec_trace.c file"));
    //    }
}

std::string StrangerAutomaton::escapeSpecialChars(std::string s)
{
    //	std::string b;
    //    bool skip = false;
    //    for (int i = int32_t(0); i < npc(s)->length(); i++) {
    //        char c = npc(s)->charAt(i);
    //        if(c == u'\u000a') {
    //            if(!skip)
    //                npc(b)->append("\\n");
    //            else
    //                skip = false;
    //        } else if(c == u'\u000d') {
    //            npc(b)->append("\\n");
    //            skip = true;
    //        } else if(c == u'"')
    //            npc(b)->append("\\\"");
    //        else if(c == u'\\')
    //            npc(b)->append("\\\\");
    //        else
    //            npc(b)->append(c);
    //    }
    //    return npc(b)->toString();
	return s;
}

/**
 * Helps to convert std::string into char*
 * to pass to StrangerLibrary functions
 */
char* StrangerAutomaton::strToCharStar(const std::string s){
	char *a = new char[s.size()+1];
	a[s.size()] = '\0';
	memcpy(a,s.c_str(), s.size());
	return a;
}

