/*
 * TestReplace.hpp
 *
 *  Created on: Jan 21, 2014
 *      Author: baki
 */

#ifndef TESTREPLACE_HPP_
#define TESTREPLACE_HPP_
//
//  testNewReplace.h
//  stranger
//
//  Created by Muath Alkhalaf on 12/12/13.
//  Copyright (c) 2013 Muath Alkhalaf. All rights reserved.
//



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
#include <vector>

using namespace std;

#include "PerfInfo.h"
PerfInfo *perfInfo = new PerfInfo();

StrangerAutomaton *addSlashesWithReplace(StrangerAutomaton *a1){
    StrangerAutomaton *sq = StrangerAutomaton::makeString("'");
    StrangerAutomaton *dq = StrangerAutomaton::makeString("\"");
    StrangerAutomaton *bs = StrangerAutomaton::makeString("\\");

    StrangerAutomaton *a2 = StrangerAutomaton::str_replace(sq, "\\'", a1);
    StrangerAutomaton *a3 = StrangerAutomaton::str_replace(dq, "\\\"", a2);
    delete a2;
    StrangerAutomaton *a4 = StrangerAutomaton::str_replace(bs, "\\\\", a3);
    delete a3;

    delete sq;
    delete dq;
    delete bs;

    return a4;
}

StrangerAutomaton *preAddSlashesWithReplace(StrangerAutomaton *a1){
    StrangerAutomaton *sq = StrangerAutomaton::makeString("'");
    StrangerAutomaton *dq = StrangerAutomaton::makeString("\"");
    StrangerAutomaton *bs = StrangerAutomaton::makeString("\\");

    StrangerAutomaton *a2 = a1->preReplace(sq, "\\'");
    StrangerAutomaton *a3 = a2->preReplace(dq, "\\\"");
    delete a2;
    StrangerAutomaton *a4 = a3->preReplace(bs, "\\\\");
    delete a3;

    delete sq;
    delete dq;
    delete bs;

    return a4;
}


StrangerAutomaton *htmlSpecialCharsWithReplace(StrangerAutomaton *a){
    StrangerAutomaton *sq = StrangerAutomaton::makeString("'");
    StrangerAutomaton *dq = StrangerAutomaton::makeString("\"");
    StrangerAutomaton *lt = StrangerAutomaton::makeString("<");
    StrangerAutomaton *gt = StrangerAutomaton::makeString(">");
    StrangerAutomaton *amp = StrangerAutomaton::makeString("&");

    StrangerAutomaton *a1 = StrangerAutomaton::str_replace(amp, "&amp;", a);
    StrangerAutomaton *a2 = StrangerAutomaton::str_replace(sq, "&apos;", a1);
    delete a1;
    StrangerAutomaton *a3 = StrangerAutomaton::str_replace(dq, "&quot;", a2);
    delete a2;
    StrangerAutomaton *a4 = StrangerAutomaton::str_replace(lt, "&lt;", a3);
    delete a3;
    StrangerAutomaton *a5 = StrangerAutomaton::str_replace(gt, "&gt;", a4);
    delete a4;


    delete sq;
    delete dq;
    delete lt;
    delete gt;
    delete amp;

    return a5;
}

StrangerAutomaton *preHtmlSpecialCharsWithReplace(StrangerAutomaton *a){
    StrangerAutomaton *sq = StrangerAutomaton::makeString("'");
    StrangerAutomaton *dq = StrangerAutomaton::makeString("\"");
    StrangerAutomaton *lt = StrangerAutomaton::makeString("<");
    StrangerAutomaton *gt = StrangerAutomaton::makeString(">");
    StrangerAutomaton *amp = StrangerAutomaton::makeString("&");

    StrangerAutomaton *a1 = a->preReplace(amp, "&amp;");
    StrangerAutomaton *a2 = a1->preReplace(sq, "&apos;");
    delete a1;
    StrangerAutomaton *a3 = a2->preReplace(dq, "&quot;");
    delete a2;
    StrangerAutomaton *a4 = a3->preReplace(lt, "&lt;");
    delete a3;
    StrangerAutomaton *a5 = a4->preReplace(gt, "&gt;");
    delete a4;


    delete sq;
    delete dq;
    delete lt;
    delete gt;
    delete amp;

    return a5;
}

StrangerAutomaton *trimWithReplace(StrangerAutomaton *a1){
    StrangerAutomaton *sp = StrangerAutomaton::makeString(" ");

    StrangerAutomaton *a2 = StrangerAutomaton::str_replace(sp, "", a1);

    delete sp;

    return a2;
}

StrangerAutomaton *preTrimWithReplace(StrangerAutomaton *a1){
    StrangerAutomaton *sp = StrangerAutomaton::makeString(" ");

    StrangerAutomaton *a2 = a1->preReplace(sp, "");

    delete sp;

    return a2;
}

void replaceVsAddslashes(){

    string path = "/home/abaki/RA/PLDI/PLDI14/experiments/replacePerformance/";

    StrangerAutomaton *regexp = StrangerAutomaton::regExToAuto("/[A-Za-z0-9\\-\\.\\+]+ @[a-zA-Z0-9\\.\\-]+\\.[a-zA-Z0-9 ]{2,4}/");
    string timeFile = stringbuilder() << path << "addSlashes_time.csv";
    string memFile = stringbuilder() << path << "addSlashes_memory.csv";

    std::ofstream timeS;
    std::ofstream memS;
    boost::posix_time::ptime start_time = perfInfo->current_time();
    try {
        timeS.open(timeFile, std::ofstream::out);
        memS.open(memFile, std::ofstream::out);
        timeS << "length,addSlashes,addSlashesWithReplace" << endl;
        memS << "length,addSlashesSTATES,addSlashesBDDNODES,addSlashesWithReplaceSTATES,addSlashesWithReplaceBDDNODES" << endl;
        for (int l = 35; l <= 35; l += 1){
            cout << "length:  " << l << "\n------------" << endl;
            timeS << l << ",";
            memS << l << ",";
            StrangerAutomaton *a = StrangerAutomaton::makeAnyStringL1ToL2(0, l);
//            StrangerAutomaton *a2 = a->intersect(regexp);
            StrangerAutomaton *a2 = a->intersect(StrangerAutomaton::makeAnyString());
//            if (l == 10) {
//            	cout << "**********************" << endl;
//            	a2->toDotAscii(0);
//            	cout << endl;
//            }
            delete a;

            start_time = perfInfo->current_time();
            StrangerAutomaton *a3 = StrangerAutomaton::addslashes(a2);
            perfInfo->performance_time = perfInfo->current_time() - start_time;
            cout << "addSlashes           :  " << perfInfo->performance_time.total_microseconds() << "us" << endl;
            timeS << perfInfo->performance_time.total_microseconds() << ",";
            memS << a3->get_num_of_states() << "," << a3->get_num_of_bdd_nodes() << ",";
            if (l == 10) {
            	cout << "**********************" << endl;
            	a3->toDotAscii(0);
            	cout << endl;
            }
            delete a3;

            start_time = perfInfo->current_time();
            a3 = addSlashesWithReplace(a2);
            perfInfo->performance_time = perfInfo->current_time() - start_time;
            cout << "addSlashesWithReplace:  " << perfInfo->performance_time.total_microseconds() << "us" << endl;
            timeS << perfInfo->performance_time.total_microseconds() << endl;
            memS << a3->get_num_of_states() << "," << a3->get_num_of_bdd_nodes() << endl;
//            cout << "states" << a3->get_num_of_states() << ", bddnodes: " << a3->get_num_of_bdd_nodes() << endl;
            if (l == 10) {
            	cout << "**********************" << endl;
            	a3->toDotAscii(0);
            	cout << endl;
            }
            delete a3;
            delete a2;

            cout << "\n\n\n" << endl;

        }
        memS.close();
        timeS.close();
    } catch (exception const &e) {
        cerr << "Can not open time or memory perfrmance file " << timeFile << ",  " << memFile << ". Following exception happened:\n" << e.what();
        if (timeS.is_open())
            timeS.close();
        if (memS.is_open())
            memS.close();
        exit(EXIT_FAILURE);
    }
}

void replaceVsHtmlSpecialChars(){
    string path = "/home/abaki/RA/PLDI/PLDI14/experiments/replacePerformance/";
    StrangerAutomaton *regexp = StrangerAutomaton::regExToAuto("/[A-Za-z0-9\\-\\.\\+]+ @[a-zA-Z0-9\\.\\-]+\\.[a-zA-Z0-9 ]{2,4}/");
    string timeFile = stringbuilder() << path << "htmlSpecialChars_time.csv";
    string memFile = stringbuilder() << path << "htmlSpecialChars_memory.csv";
    std::ofstream timeS;
    std::ofstream memS;

    boost::posix_time::ptime start_time = perfInfo->current_time();
    try {
        timeS.open(timeFile, std::ofstream::out);
        memS.open(memFile, std::ofstream::out);
        timeS << "length,htmlSpecialChars,htmlSpecialCharsWithReplace" << endl;
        memS << "length,htmlSpecialCharsSTATES,htmlSpecialCharsBDDNODES,htmlSpecialCharsWithReplaceSTATES,htmlSpecialCharsWithReplaceBDDNODES" << endl;
        for (int l = 5; l <= 35; l += 5){
            cout << "length:  " << l << "\n------------" << endl;
            timeS << l << ",";
            memS << l << ",";
            StrangerAutomaton *a = StrangerAutomaton::makeAnyStringL1ToL2(0, l);
//            StrangerAutomaton *a2 = a->intersect(regexp);
            StrangerAutomaton *a2 = a->intersect(StrangerAutomaton::makeAnyString());
            delete a;



            start_time = perfInfo->current_time();
            StrangerAutomaton *a3 = StrangerAutomaton::htmlSpecialChars(a2, "ENT_QUOTES");
            perfInfo->performance_time = perfInfo->current_time() - start_time;
            cout << "htmlSpecialChars           :  " << perfInfo->performance_time.total_microseconds() << "us" << endl;
            timeS << perfInfo->performance_time.total_microseconds() << ",";
            memS << a3->get_num_of_states() << "," << a3->get_num_of_bdd_nodes() << ",";
            if (l == 10) {
            	cout << "**********************" << endl;
            	a3->toDotAscii(0);
            	cout << endl;
            }
            delete a3;

            start_time = perfInfo->current_time();
            a3 = htmlSpecialCharsWithReplace(a2);
            perfInfo->performance_time = perfInfo->current_time() - start_time;
            cout << "htmlSpecialCharsWithReplace:  " << perfInfo->performance_time.total_microseconds() << "us" << endl;
            timeS << perfInfo->performance_time.total_microseconds() << endl;
            memS << a3->get_num_of_states() << "," << a3->get_num_of_bdd_nodes() << endl;
            if (l == 10) {
            	cout << "**********************" << endl;
            	a3->toDotAscii(0);
            	cout << endl;
            }
            delete a3;
            delete a2;

            cout << "\n\n\n" << endl;

        }

        memS.close();
        timeS.close();

    } catch (exception const &e) {
        cerr << "Can not open time or memory perfrmance file " << timeFile << ",  " << memFile << ". Following exception happened:\n" << e.what();
        if (timeS.is_open())
        timeS.close();
        if (memS.is_open())
        memS.close();
        exit(EXIT_FAILURE);
    }
}





#endif /* TESTREPLACE_HPP_ */
