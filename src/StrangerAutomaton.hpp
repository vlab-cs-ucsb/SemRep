// Generated from /testJ2c/src/StrangerAutomaton.java

#pragma once

#include "PerfInfo.h"
#include "stranger.h"
#include "StrangerAutomatonException.hpp"
#include "stranger_lib_internal.h"
#include "RegExp.hpp"
#include <string>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <cstring>
//#include <boost/config.hpp>
//#include <boost/graph/edmonds_karp_max_flow.hpp>
//#include <boost/graph/adjacency_list.hpp>
//#include <boost/graph/read_dimacs.hpp>
//#include <boost/graph/graph_utility.hpp>
//#include <boost/graph/graphviz.hpp>

class RegExp;

class StrangerAutomaton
{
public:
    StrangerAutomaton(DFA* dfa);
    virtual ~StrangerAutomaton();
    StrangerAutomaton* clone(int id);
    StrangerAutomaton* clone();
    int getID();
    void setID(int id);
    DFA* getDfa();
    static StrangerAutomaton* makeBottom(int id);
    static StrangerAutomaton* makeBottom();
    static StrangerAutomaton* makeTop(int id);
    static StrangerAutomaton* makeTop();
    static StrangerAutomaton* makeString(std::string s, int id);
    static StrangerAutomaton* makeString(std::string s);
    static StrangerAutomaton* makeChar(char c, int id);
    static StrangerAutomaton* makeChar(char c);
    static StrangerAutomaton* makeCharRange(char from, char to, int id);
    static StrangerAutomaton* makeCharRange(char from, char to);
    static StrangerAutomaton* makeAnyString(int id);
    static StrangerAutomaton* makeAnyString();
    static StrangerAutomaton* makeAnyStringL1ToL2(int l1, int l2, int id);
    static StrangerAutomaton* makeAnyStringL1ToL2(int l1, int l2);
    static StrangerAutomaton* makeEmptyString(int id);
    static StrangerAutomaton* makeEmptyString();
    static StrangerAutomaton* makeDot(int id);
    static StrangerAutomaton* makeDot();
    static StrangerAutomaton* makePhi(int id);
    static StrangerAutomaton* makePhi();
    std::string generateSatisfyingExample();
    StrangerAutomaton* optional(int id);
    StrangerAutomaton* optional();
    StrangerAutomaton* kleensStar(int id);
    StrangerAutomaton* kleensStar();
    static StrangerAutomaton* kleensStar(StrangerAutomaton* auto_, int id);
    static StrangerAutomaton* kleensStar(StrangerAutomaton* auto_);
    StrangerAutomaton* closure(int id);
    StrangerAutomaton* closure();
    static StrangerAutomaton* closure(StrangerAutomaton* auto_, int id);
    static StrangerAutomaton* closure(StrangerAutomaton* auto_);
    StrangerAutomaton* repeat(unsigned min, int id);
    StrangerAutomaton* repeat(unsigned min);
    StrangerAutomaton* repeat(unsigned min, unsigned max, int id);
    StrangerAutomaton* repeat1(unsigned min, unsigned max);
    StrangerAutomaton* complement(int id);
    StrangerAutomaton* complement();
    StrangerAutomaton* union_(StrangerAutomaton* auto_, int id);
    StrangerAutomaton* union_(StrangerAutomaton* auto_);
    StrangerAutomaton* unionWithEmptyString(int id);
    StrangerAutomaton* unionWithEmptyString();
    StrangerAutomaton* intersect(StrangerAutomaton* auto_, int id);
    StrangerAutomaton* intersect(StrangerAutomaton* auto_);
    StrangerAutomaton* preciseWiden(StrangerAutomaton* auto_, int id);
    StrangerAutomaton* preciseWiden(StrangerAutomaton* auto_);
    StrangerAutomaton* coarseWiden(StrangerAutomaton* auto_, int id);
    StrangerAutomaton* coarseWiden(StrangerAutomaton* auto_);
    StrangerAutomaton* concatenate(StrangerAutomaton* auto_, int id);
    StrangerAutomaton* concatenate(StrangerAutomaton* auto_);
    StrangerAutomaton* leftPreConcat(StrangerAutomaton* rightSiblingAuto, int id);
    StrangerAutomaton* leftPreConcat(StrangerAutomaton* rightSiblingAuto);
    StrangerAutomaton* leftPreConcatConst(std::string rightSiblingString, int id);
    StrangerAutomaton* leftPreConcatConst(std::string rightSiblingString);
    StrangerAutomaton* rightPreConcat(StrangerAutomaton* leftSiblingAuto, int id);
    StrangerAutomaton* rightPreConcat(StrangerAutomaton* leftSiblingAuto);
    StrangerAutomaton* rightPreConcatConst(std::string leftSiblingString, int id);
    StrangerAutomaton* rightPreConcatConst(std::string leftSiblingString);
    static StrangerAutomaton* regExToAuto(std::string phpRegexOrig, bool preg, int id);
    static StrangerAutomaton* regExToAuto(std::string phpRegexOrig);
    static StrangerAutomaton* reg_replace(StrangerAutomaton* patternAuto, std::string replaceStr, StrangerAutomaton* subjectAuto, bool preg, int id);
    static StrangerAutomaton* reg_replace(StrangerAutomaton* patternAuto, std::string replaceStr, StrangerAutomaton* subjectAuto);
    static StrangerAutomaton* str_replace(StrangerAutomaton* searchAuto, std::string replaceStr, StrangerAutomaton* subjectAuto, int id);
    static StrangerAutomaton* str_replace(StrangerAutomaton* searchAuto, std::string replaceStr, StrangerAutomaton* subjectAuto);
    StrangerAutomaton* preReplace(StrangerAutomaton* searchAuto, std::string replaceString, int id);
    StrangerAutomaton* preReplace(StrangerAutomaton* searchAuto, std::string replaceString);
    StrangerAutomaton* getUnaryAutomaton(int id);
    StrangerAutomaton* getUnaryAutomaton() { return getUnaryAutomaton(traceID); };
    StrangerAutomaton* restrictLengthByOtherAutomaton(StrangerAutomaton* otherAuto, int id);
    StrangerAutomaton* restrictLengthByOtherAutomaton(StrangerAutomaton* otherAuto)
    {
        return restrictLengthByOtherAutomaton(otherAuto, traceID);
    };
    StrangerAutomaton* restrictLengthByOtherAutomatonFinite(StrangerAutomaton *otherAuto, int id);
    StrangerAutomaton* restrictLengthByOtherAutomatonFinite(StrangerAutomaton *otherAuto)
    {
        return restrictLengthByOtherAutomatonFinite(otherAuto, traceID);
    };
    StrangerAutomaton* restrictLengthByUnaryAutomaton(StrangerAutomaton* uL, int id);
    StrangerAutomaton* restrictLengthByUnaryAutomaton(StrangerAutomaton* uL){
        return restrictLengthByUnaryAutomaton(uL, traceID);
    };
    bool checkIntersection(StrangerAutomaton* auto_, int id1, int id2);
    bool checkIntersection(StrangerAutomaton* auto_);
    bool checkInclusion(StrangerAutomaton* auto_, int id1, int id2);
    bool checkInclusion(StrangerAutomaton* auto_);
    bool checkEquivalence(StrangerAutomaton* auto_, int id1, int id2);
    bool checkEquivalence(StrangerAutomaton* auto_);
    bool isLengthFinite();
    bool equals(StrangerAutomaton* other);
    bool checkEmptiness();
    bool isEmpty();
    bool checkEmptyString();
    bool isSingleton();
    std::string getStr();
    bool isBottom();
    bool isTop();
    StrangerAutomaton* toUpperCase(int id);
    StrangerAutomaton* toUpperCase() { return toUpperCase(traceID);};
    StrangerAutomaton* toLowerCase(int id);
    StrangerAutomaton* toLowerCase() { return toLowerCase(traceID);};
    StrangerAutomaton* preToUpperCase(int id);
    StrangerAutomaton* preToUpperCase() { return preToUpperCase(traceID);};
    StrangerAutomaton* preToLowerCase(int id);
    StrangerAutomaton* preToLowerCase() { return preToLowerCase(traceID);};
    StrangerAutomaton* trimSpaces(int id);
    StrangerAutomaton* trimSpacesLeft(int id);
    StrangerAutomaton* trimSpacesRight(int id);
    StrangerAutomaton* trim(char c, int id);
    StrangerAutomaton* trimLeft(char c, int id);
    StrangerAutomaton* trimRight(char c, int id);
    StrangerAutomaton* trim(char chars[], int id);
    StrangerAutomaton* trimSpaces() { return trimSpaces(traceID); };
    StrangerAutomaton* trimSpacesLeft() { return trimSpacesLeft(traceID); };
    StrangerAutomaton* trimSpacesRight() { return trimSpacesRight(traceID); };
    StrangerAutomaton* trim(char c) { return trim(c, traceID); };
    StrangerAutomaton* trimLeft(char c) { return trimLeft(c, traceID); };
    StrangerAutomaton* trimRight(char c) { return trimRight(c, traceID); };
    StrangerAutomaton* trim(char chars[]) { return trim(chars, traceID); };
//    StrangerAutomaton* trimLeft(char chars[]);
//    StrangerAutomaton* trimRight(char chars[]);
    StrangerAutomaton* preTrimSpaces(int id);
    StrangerAutomaton* preTrimSpaces() { return preTrimSpaces(traceID);};
    static StrangerAutomaton* addslashes(StrangerAutomaton* subjectAuto, int id);
    static StrangerAutomaton* addslashes(StrangerAutomaton* subjectAuto){return addslashes(subjectAuto, traceID);};
    static StrangerAutomaton* pre_addslashes(StrangerAutomaton* subjectAuto, int id);
    static StrangerAutomaton* pre_addslashes(StrangerAutomaton* subjectAuto){return pre_addslashes(subjectAuto, traceID);};
    static StrangerAutomaton* htmlSpecialChars(StrangerAutomaton* subjectAuto, int id);
    static StrangerAutomaton* htmlSpecialChars(StrangerAutomaton* subjectAuto){return htmlSpecialChars(subjectAuto, traceID);};
    static StrangerAutomaton* preHtmlSpecialChars(StrangerAutomaton* subjectAuto, int id);
    static StrangerAutomaton* preHtmlSpecialChars(StrangerAutomaton* subjectAuto){return preHtmlSpecialChars(subjectAuto, traceID);};
    static StrangerAutomaton* stripslashes(StrangerAutomaton* subjectAuto, int id);
    static StrangerAutomaton* stripslashes(StrangerAutomaton* subjectAuto){return stripslashes(subjectAuto, traceID);};
    static StrangerAutomaton* pre_stripslashes(StrangerAutomaton* subjectAuto, int id);
    static StrangerAutomaton* pre_stripslashes(StrangerAutomaton* subjectAuto){return pre_stripslashes(subjectAuto, traceID);};
    static StrangerAutomaton* mysql_real_escape_string(StrangerAutomaton* subjectAuto, int id);
    static StrangerAutomaton* pre_mysql_real_escape_string(StrangerAutomaton* subjectAuto, int id);
    static StrangerAutomaton* nl2br(StrangerAutomaton* subjectAuto, int id);
    static StrangerAutomaton* pre_nl2br(StrangerAutomaton* subjectAuto, int id);
    static StrangerAutomaton* getUndesiredSQLTest();
    static StrangerAutomaton* getUndesiredXSSTest();
    static StrangerAutomaton* getUndesiredMFETest();
//    std::set<char> mincut();
    
    void printAutomaton();
    void printAutomatonVitals();
    void toDot();
    void toDotAscii(int printSink);
    static void openCtraceFile(std::string name);
    static void appendCtraceFile(std::string name);
    static void closeCtraceFile();
    void debugAutomaton();
    static void debugToFile(std::string str);
    static void debug(std::string s);
    static int getVar(){ return num_ascii_track;};
    static int* getIndices(){ return indices_main;};
    static int* getIndices(int length) { return allocateAscIIIndexWithExtraBit(length);};
    static unsigned *getUnsignedIndices(int length);
    static char* strToCharStar(const std::string s);
//    static void setPerfInfo(PerfInfo& pInfo) { perfInfo = pInfo; };
//    static PerfInfo& getPerfInfo() { return perfInfo; };
    static void staticInit();
    static PerfInfo perfInfo;
    
    StrangerAutomaton* restrict(StrangerAutomaton* otherAuto, int id){
        StrangerAutomaton* retMe = this->intersect(otherAuto);
        retMe->ID = id;
        return retMe;
    };
    
    StrangerAutomaton* restrict(StrangerAutomaton* otherAuto){
        return this->restrict(otherAuto, traceID);
    };
    
    StrangerAutomaton* restrict(std::string regExp, int id){
        StrangerAutomaton* regExpAuto = regExToAuto(regExp);
        StrangerAutomaton* retMe = this->intersect(regExpAuto);
        delete regExpAuto;
        retMe->ID = id;
        return retMe;
    };
    
    StrangerAutomaton* restrict(std::string regExp){
        return this->restrict(regExp, traceID);
    }
    
    StrangerAutomaton* preRestrict(std::string regExp, int id){
        StrangerAutomaton* regExpAuto = regExToAuto(regExp);
        StrangerAutomaton* regExpAutoComplement = regExpAuto->complement();
        delete regExpAuto;
        StrangerAutomaton* retMe = this->union_(regExpAutoComplement);
        delete regExpAutoComplement;
        retMe->ID = id;
        return retMe;
    };
    
    StrangerAutomaton* preRestrict(std::string regExp){
        return this->preRestrict(regExp, traceID);
    };
    
    StrangerAutomaton* preRestrict(StrangerAutomaton* otherAuto, int id){
        StrangerAutomaton* otherAutoComplement = otherAuto->complement();
        StrangerAutomaton* retMe = this->union_(otherAutoComplement);
        delete otherAutoComplement;
        retMe->ID = id;
        return retMe;
    };
    
    StrangerAutomaton* preRestrict(StrangerAutomaton* otherAuto){
        return this->preRestrict(otherAuto, traceID);
    };
    DFA* dfa;
private:
    
    int ID;
    int autoTraceID;
    bool top;
    bool bottom;
    static int num_ascii_track;
    static int* indices_main;
    static unsigned* u_indices_main;
    static int traceID;
    static int baseTraceID;
    static int tempTraceID;
    static int baseTempTraceID;
    static int debugLevel;
    static bool coarseWidening;
    static char slash;
	StrangerAutomaton();
	void init();
    static bool& initialized();
    static void resetTraceID();
    static std::string escapeSpecialChars(std::string s);
};
