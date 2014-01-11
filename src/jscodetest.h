//
//  jscodetest.h
//  stranger
//
//  Created by Muath Alkhalaf on 10/24/13.
//  Copyright (c) 2013 Muath Alkhalaf. All rights reserved.
//

#ifndef __stranger__jscodetest__
#define __stranger__jscodetest__

//
//  jscodetest.cpp
//  stranger
//
//  Created by Muath Alkhalaf on 10/24/13.
//  Copyright (c) 2013 Muath Alkhalaf. All rights reserved.
//

#include "jscodetest.h"
#include "stranger_lib_internal.h"
#include <iostream>
#include <vector>
#include "stranger.h"
#include "bdd.h"
#include "dfa.h"
#include "bdd_dump.h"
#include "bdd_external.h"
#include "StrangerAutomaton.hpp"
//#include <regex>
#include <fstream>
#include <cstdio>
#include <ctime>

#include <string>
#include <map>
#include <utility>
#include <vector>


using namespace std;

/*
 /Users/muath/Documents/OldDocuments/jsstranger-subjects/validation examples/good/2
 source:
 -------
 http://www.webcheatsheet.com/javascript/form_validation.php

 <td><label for="email">Your email:</label></td>
 <td><input name="email" size="35" maxlength="30" type="text"></td>
 function validateEmail(fld) {
 var error="";
 var tfld = trim(fld.value);                        // value of field with whitespace trimmed off
 var emailFilter = /^[^@]+@[^@.]+\.[^@]*\w\w$/ ;
 var illegalChars= /[\(\)\<\>\,\;\:\\\"\[\]]/ ;

 if (fld.value == "") {
 fld.style.background = 'Yellow';
 error = "You didn't enter an email address.\n";
 } else if (!emailFilter.test(tfld)) {              //test email for illegal characters
 fld.style.background = 'Yellow';
 error = "Please enter a valid email address.\n";
 } else if (fld.value.match(illegalChars)) {
 fld.style.background = 'Yellow';
 error = "The email address contains illegal characters.\n";
 } else {
 fld.style.background = 'White';
 }
 return error;
 }
 */


StrangerAutomaton *email1_fw(StrangerAutomaton *input, vector<StrangerAutomaton*> &v){
    v[0] = StrangerAutomaton::regExToAuto("/.*[\\(\\)\\<\\>\\,\\;\\:\\\\\\\"\\[\\]].*/");
    v[1] = StrangerAutomaton::regExToAuto("/[^@]+@[^@.]+\\.[^@]*[a-zA-Z_][a-zA-Z_]/");
    v[2] = v[0]->complement();
    v[3] = StrangerAutomaton::makeAnyStringL1ToL2(0, 30)->intersect(input);
    v[4] = v[3]->trimSpaces();
    v[5] = StrangerAutomaton::makeEmptyString()->complement();
    v[6] = v[4]->intersect(v[5]);
    v[7] = v[6]->intersect(v[1]);
    v[8] = v[7]->intersect(v[2]);
//    v[8]->toDotAscii(0);
    return v[8];
}

StrangerAutomaton *email1_bw1(){
    vector<StrangerAutomaton*> v(20);
    v[0] = StrangerAutomaton::regExToAuto("/.*[\\(\\)\\<\\>\\,\\;\\:\\\\\\\"\\[\\]].*/");
    v[1] = StrangerAutomaton::regExToAuto("/[^@]+@[^@.]+\\.[^@]*[a-zA-Z_][a-zA-Z_]/");
    v[2] = v[1]->complement();
    v[3] = v[0]->union_(v[2]);
    v[4] = StrangerAutomaton::makeEmptyString();
    v[5] = v[4]->union_(v[3]);
    v[6] = StrangerAutomaton::makeAnyString();
    v[7] = v[6]->trimSpaces();
    v[8] = v[7]->intersect(v[5]);
    v[9] = v[8]->preTrimSpaces();
    v[10] = StrangerAutomaton::makeAnyStringL1ToL2(0, 30)->complement();
    v[11] = v[10]->union_(v[9]);
    return v[11];
}

StrangerAutomaton *email1_bw2(StrangerAutomaton *input, vector<StrangerAutomaton*> fw){
    vector<StrangerAutomaton*> v(20);
    v[0] = StrangerAutomaton::regExToAuto("/.*[\\(\\)\\<\\>\\,\\;\\:\\\\\\\"\\[\\]].*/");
    v[1] = StrangerAutomaton::regExToAuto("/[^@]+@[^@.]+\\.[^@]*[a-zA-Z_][a-zA-Z_]/");
    v[2] = v[0]->complement();
    v[3] = input->intersect(v[2]);
    v[4] = v[3]->intersect(v[1]);
    v[5] = StrangerAutomaton::makeEmptyString()->complement();
    v[6] = v[5]->intersect(v[4]);
    v[7] = v[6]->intersect(fw[7]);
    v[9] = v[8]->preTrimSpaces();
    v[10] = StrangerAutomaton::makeAnyStringL1ToL2(0, 30);
    v[11] = v[10]->intersect(v[9]);
    return v[11];
}

/*
 source:
 -------
 http://www.tizag.com/javascriptT/javascriptform.php

 /Users/muath/Documents/OldDocuments/jsstranger-subjects/validation examples/good/4

 function emailValidator(elem, helperMsg){
 var emailExp = /^[\w\-\.\+]+\@[a-zA-Z0-9\.\-]+\.[a-zA-z0-9]{2,4}$/;
 if(elem.value.match(emailExp)){
 return true;
 }else{
 alert(helperMsg);
 elem.focus();
 return false;
 }
 }
 */
StrangerAutomaton *email2_fw(StrangerAutomaton *input, vector<StrangerAutomaton*> &v){
    v[0] = StrangerAutomaton::regExToAuto("/[A-Za-z0-9\\-\\.\\+]+@[a-zA-Z0-9\\.\\-]+\\.[a-zA-z0-9]{2,4}/");
    v[1] = v[0]->intersect(input);
    return v[1];
}

StrangerAutomaton *email2_bw1(){
    vector<StrangerAutomaton*> v(20);
    v[0] = StrangerAutomaton::regExToAuto("/[A-Za-z0-9\\-\\.\\+]+@[a-zA-Z0-9\\.\\-]+\\.[a-zA-z0-9]{2,4}/");
    v[1] = v[0]->complement();
    return v[1];
}

StrangerAutomaton *email2_bw2(StrangerAutomaton *input, vector<StrangerAutomaton*> &fw){
    vector<StrangerAutomaton*> v(20);
    v[0] = StrangerAutomaton::regExToAuto("/[A-Za-z0-9\\-\\.\\+]+@[a-zA-Z0-9\\.\\-]+\\.[a-zA-z0-9]{2,4}/");
    v[1] = v[0]->intersect(input);
    return v[1];
}

/*
 source:
 -------
 http://developer.apple.com/internet/webcontent/validation.html

  /Users/muath/Documents/OldDocuments/jsstranger-subjects/validation examples/good/5

 function checkEmail (strng) {
 var error="";
 if (strng == "") {
 error = "You didn't enter an email address.\n";
 }

 var emailFilter=/^.+@.+\..{2,3}$/;
 if (!(emailFilter.test(strng))) {
 error = "Please enter a valid email address.\n";
 }
 else {
 //test email for illegal characters
 var illegalChars= /[\(\)\<\>\,\;\:\\\"\[\]]/
 if (strng.match(illegalChars)) {
 error = "The email address contains illegal characters.\n";
 }
 }
 return error;
 }

 */
StrangerAutomaton *email3_fw(StrangerAutomaton *input, vector<StrangerAutomaton*> &v){
    v[0] = StrangerAutomaton::regExToAuto("/.*[\\(\\)\\<\\>\\,\\;\\:\\\\\\\"\\[\\]].*/");
    v[1] = StrangerAutomaton::regExToAuto("/.+@.+\\..{2,3}/");
    v[2] = v[0]->complement();
    v[3] = StrangerAutomaton::makeEmptyString()->complement();
    v[4] = input->intersect(v[3]);
    v[5] = v[4]->intersect(v[1]);
    v[6] = v[5]->intersect(v[2]);
    return v[6];
}

StrangerAutomaton *email3_bw1(){
    vector<StrangerAutomaton*> v(20);
    v[0] = StrangerAutomaton::regExToAuto("/.*[\\(\\)\\<\\>\\,\\;\\:\\\\\\\"\\[\\]].*/");
    v[1] = StrangerAutomaton::regExToAuto("/.+@.+\\..{2,3}/");
    v[2] = v[1]->complement();
    v[3] = StrangerAutomaton::makeEmptyString();
    v[4] = v[0]->union_(v[2]);
    v[5] = v[4]->union_(v[3]);
    return v[5];
}

StrangerAutomaton *email3_bw2(StrangerAutomaton *input, vector<StrangerAutomaton*> &fw){
    vector<StrangerAutomaton*> v(20);
    v[0] = StrangerAutomaton::regExToAuto("/.*[\\(\\)\\<\\>\\,\\;\\:\\\\\\\"\\[\\]].*/");
    v[1] = StrangerAutomaton::regExToAuto("/.+@.+\\..{2,3}/");
    v[2] = v[0]->complement();
    v[3] = StrangerAutomaton::makeEmptyString()->complement();
    v[4] = input->intersect(v[2]);
    v[5] = v[4]->intersect(v[1]);
    v[6] = v[5]->intersect(v[3]);
    return v[6];
}

/**
 * source: headfirst book
 *
 /Users/muath/Documents/OldDocuments/jsstranger-subjects/validators/email/validator1.js
function validateEmail(inputField, helpText){
    // First see if the input value contains data
    // See if the input data validates OK
    if (!/.+/.test(inputField.value)) {
        // The data is invalid, so set the help message and return false
        if (helpText != null)
            helpText.innerHTML = "Please enter a value.";
    TEST2: return false;
    }
    else {
        // The data is OK, so clear the help message and return true
        if (helpText != null)
            helpText.innerHTML = "";
        // Then see if the input value is an email address
        // See if the input data validates OK
        // -- original -- if (!/^[\w\.-_\+]+@[\w-]+(\.\w{2,3})+$/.test(inputField.value)) {
        if (!/^[a-zA-Z0-9_\.-_\+]+@[a-zA-Z0-9_\-]+(\.[a-zA-Z0-9_]{2,3})+$/.test(inputField.value)) {
            // The data is invalid, so set the help message and return false
            if (helpText != null)
                helpText.innerHTML = "Please enter an email address (for example, johndoe@acme.com).";
        TEST3: return false;
        }
        else {
            // The data is OK, so clear the help message and return true
            if (helpText != null)
                helpText.innerHTML = "";
        TEST1: return true;
        }
    }
}
*/
StrangerAutomaton *email4_fw(StrangerAutomaton *input, vector<StrangerAutomaton*> &v){
    v[0] = StrangerAutomaton::makeEmptyString();
    v[1] = StrangerAutomaton::regExToAuto("/[a-zA-Z0-9\\.-_\\+]+@[a-zA-Z0-9\\-]+(\\.[a-zA-Z0-9]{2,3})+/");
    v[2] = v[0]->complement();
    v[3] = input->intersect(v[2]);
    v[4] = v[3]->intersect(v[1]);
    return v[4];
}

StrangerAutomaton *email4_bw1(){
    vector<StrangerAutomaton*> v(20);
    v[0] = StrangerAutomaton::makeEmptyString();
    v[1] = StrangerAutomaton::regExToAuto("/[a-zA-Z0-9\\.-_\\+]+@[a-zA-Z0-9\\-]+(\\.[a-zA-Z0-9]{2,3})+/");
    v[2] = v[1]->complement();
    v[4] = v[0]->union_(v[2]);
    return v[4];
}

StrangerAutomaton *email4_bw2(StrangerAutomaton *input, vector<StrangerAutomaton*> &fw){
    vector<StrangerAutomaton*> v(20);
    v[0] = StrangerAutomaton::makeEmptyString();
    v[1] = StrangerAutomaton::regExToAuto("/[a-zA-Z0-9\\.-_\\+]+@[a-zA-Z0-9\\-]+(\\.[a-zA-Z0-9]{2,3})+/");
    v[2] = v[0]->complement();
    v[3] = input->intersect(v[1]);
    v[4] = v[3]->intersect(v[2]);
    return v[4];
}

/*
 * source:
 * -------
 * ./clinicweb.war/js/scriptsform.js
 full-apps/subjects/ClinicWeb-Windows

 /Users/muath/Documents/OldDocuments/jsstranger-subjects/validators/email/validator3.js

//Verifica se o e-mail é válido -- check if email is valid
function validaemail(obj){
    str = obj.value;
    var filter=/^.+@.+\..{2,3}$/
    if (filter.test(str))
        //testresults=true // commented by muath
        return true;// added by muath
    else{
        //testresults=false // commented by muath
        return false;// added by muath
    }

    //Se escreveu algum e-mail e se o teste deu inválido, mensagem
    // If you wrote some email and if the test was invalid, message
    //if(str != "" && !testresults) {
    //	alert("e-mail inválido!");
    //	obj.focus();
    //}
}
 */

StrangerAutomaton *email5_fw(StrangerAutomaton *input, vector<StrangerAutomaton*> &v){
    v[0] = StrangerAutomaton::regExToAuto("/.+@.+\\..{2,3}/");
    v[1] = input->intersect(v[0]);
    return v[1];
}

StrangerAutomaton *email5_bw1(){
    vector<StrangerAutomaton*> v(20);
    v[0] = StrangerAutomaton::regExToAuto("/.+@.+\\..{2,3}/");
    v[1] = v[0]->complement();
    return v[1];
}

StrangerAutomaton *email5_bw2(StrangerAutomaton *input, vector<StrangerAutomaton*> &fw){
    vector<StrangerAutomaton*> v(20);
    v[0] = StrangerAutomaton::regExToAuto("/.+@.+\\..{2,3}/");
    v[1] = input->intersect(v[0]);
    return v[1];
}

/*
 *
 * file: jsstranger-subjects/full-apps/working/introductionpackage-4.4.6a/t3lib/jsfunc.validateform.js
 * app name: TYPO3 Content Management Framework
 * version: 4.4.6
 * source: http://typo3.org/download/, http://sourceforge.net/projects/typo3/
 * downloaded file name: introductionpackage-4.4.6a

 /Users/muath/Documents/OldDocuments/jsstranger-subjects/validators/email/validator4.js

//case "_EMAIL": //commented by Muath
function validateEmail(value){
	var theRegEx_notValid = new RegExp("(\@.*\@)|(\.\.)|(\@\.)|(\.\@)|(^\.)", "gi");
	var theRegEx_isValid = new RegExp("^.+\@[a-zA-Z0-9\-\.]+\.([a-zA-Z]{2,4}|[0-9]{1,3})$","");
	if (!theRegEx_isValid.test(value))	{	// This part was supposed to be a part of the condition: " || theRegEx_notValid.test(value)" - but I couldn't make it work (Mozilla Firefox, linux) - Anyone knows why?
		msg+="\n"+theLabel+' ('+(emailMess ? emailMess : 'Email address not valid!')+')';
    TEST2: return false;
	}
    else
        TEST1: return true;
}
*/

StrangerAutomaton *email6_fw(StrangerAutomaton *input, vector<StrangerAutomaton*> &v){
    v[0] = StrangerAutomaton::regExToAuto("/.+@[a-zA-Z0-9\\-\\.]+\\.([a-zA-Z]{2,4}|[0-9]{1,3})/");
    v[1] = input->intersect(v[0]);
    return v[1];
}

StrangerAutomaton *email6_bw1(){
    vector<StrangerAutomaton*> v(20);
    v[0] = StrangerAutomaton::regExToAuto("/.+@[a-zA-Z0-9\\-\\.]+\\.([a-zA-Z]{2,4}|[0-9]{1,3})/");
    v[1] = v[0]->complement();
    return v[1];
}

StrangerAutomaton *email6_bw2(StrangerAutomaton *input, vector<StrangerAutomaton*> &fw){
    vector<StrangerAutomaton*> v(20);
    v[0] = StrangerAutomaton::regExToAuto("/.+@[a-zA-Z0-9\\-\\.]+\\.([a-zA-Z]{2,4}|[0-9]{1,3})/");
    v[1] = input->intersect(v[0]);
    return v[1];
}



/*********************************************************************************************/
/*********************************************************************************************/
/*********************************************************************************************/
/*********************************************************************************************/


vector<function<StrangerAutomaton *(StrangerAutomaton *, vector<StrangerAutomaton*> &)>> getFwFuncs(){
    vector<function<StrangerAutomaton *(StrangerAutomaton *, vector<StrangerAutomaton*> &)>> fw_functors(10);
    fw_functors[1] = email1_fw;
    fw_functors[2] = email2_fw;
    fw_functors[3] = email3_fw;
    fw_functors[4] = email4_fw;
    fw_functors[5] = email5_fw;
    fw_functors[6] = email6_fw;
    return fw_functors;
}

vector<function<StrangerAutomaton *()>> getBw1Funcs(){
    vector<function<StrangerAutomaton *()>> bw_functors1(10);
    bw_functors1[1] = email1_bw1;
    bw_functors1[2] = email2_bw1;
    bw_functors1[3] = email3_bw1;
    bw_functors1[4] = email4_bw1;
    bw_functors1[5] = email5_bw1;
    bw_functors1[6] = email6_bw1;
    return bw_functors1;
}

vector<function<StrangerAutomaton *(StrangerAutomaton *, vector<StrangerAutomaton*> &)>> getBw2Funcs(){
    vector<function<StrangerAutomaton *(StrangerAutomaton *, vector<StrangerAutomaton*> &)>> bw_functors2(10);
    bw_functors2[1] = email1_bw2;
    bw_functors2[2] = email2_bw2;
    bw_functors2[3] = email3_bw2;
    bw_functors2[4] = email4_bw2;
    bw_functors2[5] = email5_bw2;
    bw_functors2[6] = email6_bw2;
    return bw_functors2;
}




/*********************************************************************************************/
/*********************************************************************************************/
/*********************************************************************************************/
/*********************************************************************************************/



/*
 * source:
 * -------
 * http://www.the-art-of-web.com/javascript/validate-date/
 *
 /Users/muath/Documents/OldDocuments/jsstranger-subjects/validators/date/validator1.js

// this code is extracted form a form validation function and I added some lines
function validateDate(form){  //added by Muath
    // regular expression to match required date format
    var re = /^[0-9]{1,2}\/[0-9]{1,2}\/[0-9]{4}$/;// added "var" by muath also changed \d to [0-9]

    if(form.startdate.value != '' && !form.startdate.value.match(re)) {
        alert("Invalid date format: " + form.startdate.value);
        form.startdate.focus();
    TEST2: return false;
    }
    else  //added by Muath
    	TEST1: return true; //added by Muath
} //added by Muath

*/
StrangerAutomaton *date1_bw1(){
    vector<StrangerAutomaton*> v(20);
    v[0] = StrangerAutomaton::regExToAuto("/[0-9]{1,2}\/[0-9]{1,2}\/[0-9]{4}/");
    v[1] = v[0]->complement();
    v[2] = StrangerAutomaton::makeEmptyString()->complement();
    v[3] = v[1]->intersect(v[2]);
    return v[3];
}

/**
 * source:
 * Headfirst book
  /Users/muath/Documents/OldDocuments/jsstranger-subjects/validators/date/validator2.js

function validateDate(inputField, helpText) {
	// First see if the input value contains data
    // See if the input data validates OK
    if (!/.+/.test(inputField.value)) {
        // The data is invalid, so set the help message and return false
        if (helpText != null)
            helpText.innerHTML = "Please enter a value.";
    TEST3: return false;
    }
    else {
        // The data is OK, so clear the help message and return true
        if (helpText != null)
            helpText.innerHTML = "";
        // See if the input data validates OK
        if (!/^[0-9]{2}\/[0-9]{2}\/([0-9]{2}|[0-9]{4})$/.test(inputField.value)) {// changed by muath \d to [0-9]
            // The data is invalid, so set the help message and return false
            if (helpText != null)
                helpText.innerHTML = "Please enter a date (for example, 01/14/1975).";
        TEST2: return false;
        }
        else {
            // The data is OK, so clear the help message and return true
            if (helpText != null)
                helpText.innerHTML = "";
        TEST1: return true;
        }
    }
}
*/
StrangerAutomaton *date2_bw1(){
    vector<StrangerAutomaton*> v(20);
    v[0] = StrangerAutomaton::regExToAuto("/[0-9]{2}\/[0-9]{2}\/([0-9]{2}|[0-9]{4})/");
    v[1] = v[0]->complement();
    v[2] = StrangerAutomaton::makeEmptyString();
    v[3] = v[1]->union_(v[2]);
    return v[3];
}

vector<function<StrangerAutomaton *()>> getDateBw1Funcs(){
    vector<function<StrangerAutomaton *()>> bw_functors1(10);
    bw_functors1[1] = date1_bw1;
    bw_functors1[2] = date2_bw1;
    return bw_functors1;
}
#endif /* defined(__stranger__jscodetest__) */
