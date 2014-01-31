/*
 * Stranger
 * Copyright (C) 2013-2014 University of California Santa Barbara.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the  Free Software
 * Foundation, Inc., 51 Franklin Street, Suite 500, Boston, MA 02110-1335,
 * USA.
 *
 * Authors: Abdulbaki Aydin, Muath Alkhalaf
 */

#include <boost/program_options.hpp>

using namespace boost;
namespace po = boost::program_options;

#include "StrangerPatcher.h"

#include<iostream>
#include <algorithm>
#include <iterator>
using namespace std;


void call_patcher(string patcher_name, string patchee_name, string field_name){
	try {
		cout << endl << "\t------ Starting Analysis for: " << field_name << " ------" << endl;
		cout << endl << "\t       Reference: " << patcher_name  << endl;
		cout << endl << "\t       Target: " << patchee_name  << endl;
		StrangerPatcher strangerPatcher(patcher_name, patchee_name, field_name);
		strangerPatcher.calculate_rejected_set = false;
		strangerPatcher.computeValidationPatch();
		strangerPatcher.computeSanitizationPatch();
        
		cout << endl << "\t------ OVERALL RESULT for: " << field_name << " ------" << endl;
		cout << "\t    Patcher: " << patcher_name << endl;
		cout << "\t    Patchee: " << patchee_name << endl;
        
		strangerPatcher.printResults();
		strangerPatcher.writeAutosforCodeGeneration(field_name, patcher_name, patchee_name);
        
		cout << endl << "\t------ END RESULT for: " << field_name << " ------" << endl;
	} catch (StrangerStringAnalysisException const &e) {
		cerr << e.what();
		exit(EXIT_FAILURE);
	}
    
}

void generate_repairs_for_client_server_pairs(vector<string>& clients, vector<string>& servers, vector<string>& field_names, bool reversed) {
    
	if (reversed) {
		for (unsigned i = 0; i < field_names.size(); i++) {
			call_patcher(servers[i], clients[i], field_names[i]);
		}
	} else {
		for (unsigned i = 0; i < field_names.size(); i++) {
			call_patcher(clients[i], servers[i], field_names[i]);
		}
	}
}

void generate_repairs_for_client_server_pairs(map<string,string>& clients, string server, bool reversed ) {
	if (reversed) {
		for (map<string,string>::iterator it = clients.begin(); it != clients.end(); it++) {
			call_patcher(server, it->second, it->first);
		}
	} else {
		for (map<string,string>::iterator it = clients.begin(); it != clients.end(); it++) {
			call_patcher(it->second, server, it->first);
		}
	}
}


void generate_repairs_for_client_server_pairs(map<string,string>& clients, vector<string>& servers, bool reversed) {
	for (vector<string>::iterator it = servers.begin(); it != servers.end(); it++) {
		generate_repairs_for_client_server_pairs(clients,*it, reversed);
	}
}

void generate_repairs_for_ss_cc_pairs(vector<string>& targets, vector<string>& field_names, bool reversed) {
	for (vector<string>::iterator name = field_names.begin(); name != field_names.end(); name++) {
		for (vector<string>::iterator it = targets.begin(); it != targets.end() - 1; it++) {
			for(vector<string>::iterator jit = it+1; jit != targets.end(); jit++) {
				if (reversed) {
					call_patcher(*jit, *it, *name);
				} else {
					call_patcher(*it, *jit, *name);
				}
			}
            
		}
	}
}

// A helper function to simplify the main part.
template<class T>
ostream& operator<<(ostream& os, const vector<T>& v)
{
    copy(v.begin(), v.end(), ostream_iterator<T>(cout, " "));
    return os;
}

int main(int argc, char *argv[]) {
    try {
    
        po::options_description desc("Allowed options");
        desc.add_options()
            ("help", "produce help message")
            ("verbose", po::value<string>()->implicit_value("0"), "verbosity level")
            ("target,t", po::value<string>()->required(), "Path to dependency graph file for repair target function.")
            ("reference,r", po::value<string>()->required(), "Path to dependency graph file for repair reference function.")
            ("fieldname,f", po::value<string>()->required(), "Name of the input field for which sanitization code needs to be repaired.")
        ;
        po::positional_options_description p;
        p.add("target", 1);
        p.add("reference", 1);
        p.add("fieldname", 1);
        
        
        po::variables_map vm;
        po::store(po::command_line_parser(argc, argv).
                  options(desc).positional(p).run(), vm);
        
        if (vm.count("help"))
        {
            cout << "Usage: SemRep [options] <target> <reference> <fieldname>\n";
            cout << "<target> is Path to dependency graph file for repair target function.\n";
            cout << "<reference> is Path to dependency graph file for repair reference function.\n";
            cout << "<fieldname> is Name of the input field for which sanitization code needs to be repaired.\n";
            cout << desc << "\n";
            return 0;
        }
        
        po::notify(vm);
        
        if (vm.count("target") && vm.count("reference") && vm.count("fieldname"))
        {
            call_patcher(vm["reference"].as<string>(), vm["target"].as<string>(), vm["fieldname"].as<string>());
        }
        else {
            cerr << "Unknown error while parsing cmdline options!" << "\n";
            return false;
        }
    
    } catch(std::exception& e)
    {
           cerr << "Error: " << e.what() << "\n";
           exit(EXIT_FAILURE);
    }
    catch(...)
    {
        cerr << "Unknown error!" << "\n";
        return false;
    }
    
}
