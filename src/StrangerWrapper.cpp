//============================================================================
// Name        : StrangerWrapper.cpp
// Author      : Abdulbaki Aydin
// Version     : 0.1b
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================



#include "StrangerPatcher.h"

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

int main(int argc, char *argv[]) {


	return 0;
}
