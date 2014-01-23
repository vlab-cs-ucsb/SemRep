//============================================================================
// Name        : StrangerWrapper.cpp
// Author      : Abdulbaki Aydin
// Version     : 0.1b
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================



#include "StrangerPatcher.h"
#include "TestReplace.hpp"

using namespace std;


void call_patcher(string patcher_name, string patchee_name, string field_name){
	try {
		cout << endl << "\t------ Starting Analysis for: " << field_name << " ------" << endl;
		cout << endl << "\t       Reference: " << patcher_name  << endl;
		cout << endl << "\t       Target: " << patchee_name  << endl;
		StrangerPatcher strangerPatcher(patcher_name, patchee_name, field_name);
		strangerPatcher.calculate_rejected_set = true;
		strangerPatcher.computeValidationPatch();
		strangerPatcher.computeSanitizationPatch();

		cout << endl << "\t------ OVERALL RESULT for: " << field_name << " ------" << endl;
		cout << "\t    Patcher: " << patcher_name << endl;
		cout << "\t    Patchee: " << patchee_name << endl;

		strangerPatcher.printResults();
		strangerPatcher.writeAutosforCodeGeneration(patcher_name, patchee_name);

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
//		int i = 0;
		for (vector<string>::iterator it = targets.begin(); it != targets.end() - 1; it++) {
			for(vector<string>::iterator jit = it+1; jit != targets.end(); jit++) {
				if (reversed) {
					call_patcher(*jit, *it, *name);
				} else {
					call_patcher(*it, *jit, *name);
				}
			}
//			if (++i == 2)
//				break;
		}
	}
}


void run_paper_example(bool reversed) {
	string field_name = "x";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/paperexample/reference_depgraph.dot";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/paperexample/target_depgraph.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

/****************************************************************************************************/
/*********** SNIPE GALLERY *********************************************/
/****************************************************************************************************/

void patch_snipe_gallery(bool reversed) {

	string dep_graph_path = "/home/abaki/RA/PLDI/PLDI14/experiments/snipegallery/";

	map<string,string> snipe_gallery_clients_115;
	vector<string> snipe_gallery_servers_115;

	snipe_gallery_servers_115.push_back(dep_graph_path + "snipe_gallery_115.dot"); // new gallery form
	snipe_gallery_clients_115["cat_parent"] = dep_graph_path + "snipe_gallery__cat_parent.dot";
	snipe_gallery_clients_115["form_cat_description"] = dep_graph_path + "snipe_gallery__form_cat_description.dot";
	snipe_gallery_clients_115["form_gallery_name"] = dep_graph_path + "snipe_gallery__form_gallery_name.dot";


	map<string,string> snipe_gallery_clients_41;
	vector<string> snipe_gallery_servers_41;

	snipe_gallery_servers_41.push_back(dep_graph_path + "snipe_gallery_41.dot");

	snipe_gallery_clients_41["cat_parent"] = dep_graph_path + "snipe_gallery__cat_parent.dot";
	snipe_gallery_clients_41["form_cat_description"] = dep_graph_path + "snipe_gallery__form_cat_description.dot";
	snipe_gallery_clients_41["form_gallery_name"] = dep_graph_path + "snipe_gallery__form_gallery_name.dot";
	snipe_gallery_clients_41["form_frame_id"] = dep_graph_path + "snipe_gallery__form_frame_id.dot";
	snipe_gallery_clients_41["form_orderby"] = dep_graph_path + "snipe_gallery__form_orderby.dot";
	snipe_gallery_clients_41["form_order"] = dep_graph_path + "snipe_gallery__form_order.dot";
	snipe_gallery_clients_41["form_watermark_txt"] = dep_graph_path + "snipe_gallery__form_watermark_txt.dot";
	snipe_gallery_clients_41["gallery_id"] = dep_graph_path + "snipe_gallery__gallery_id.dot";
	snipe_gallery_clients_41["thumb_type"] = dep_graph_path + "snipe_gallery__thumb_type.dot";

	map<string,string> snipe_image_clients;
	vector<string> snipe_image_servers;

	snipe_image_servers.push_back(dep_graph_path + "snipe_image_177.dot");
	snipe_image_servers.push_back(dep_graph_path + "snipe_image_539.dot");

	snipe_image_clients["form_author"] = dep_graph_path + "snipe_image__form_author.dot";
	snipe_image_clients["form_details"] = dep_graph_path + "snipe_image__form_details.dot";
	snipe_image_clients["form_keywords"] = dep_graph_path + "snipe_image__form_keywords.dot";
	snipe_image_clients["form_location"] = dep_graph_path + "snipe_image__form_location.dot";
	snipe_image_clients["form_publish"] = dep_graph_path + "snipe_image__form_publish.dot";
	snipe_image_clients["gallery_id"] = dep_graph_path + "snipe_image__gallery_id.dot";

	map<string,string> snipe_local_clients;
	vector<string> snipe_local_servers;

	snipe_local_servers.push_back(dep_graph_path + "snipe_local.dot");

	snipe_local_clients["form_author"] = dep_graph_path + "snipe_local__form_author.dot";
	snipe_local_clients["form_details"] = dep_graph_path + "snipe_local__form_details.dot";
	snipe_local_clients["form_keywords"] = dep_graph_path + "snipe_local__form_keywords.dot";
	snipe_local_clients["form_location"] = dep_graph_path + "snipe_local__form_location.dot";
	snipe_local_clients["form_publish"] = dep_graph_path + "snipe_local__form_publish.dot";
	snipe_local_clients["gallery_id"] = dep_graph_path + "snipe_local__gallery_id.dot";


	map<string,string> snipe_zip_clients;
	vector<string> snipe_zip_servers;

	snipe_zip_servers.push_back(dep_graph_path + "snipe_zip.dot");

	snipe_zip_clients["form_author"] = dep_graph_path + "snipe_zip__form_author.dot";
	snipe_zip_clients["form_details"] = dep_graph_path + "snipe_zip__form_details.dot";
	snipe_zip_clients["form_keywords"] = dep_graph_path + "snipe_zip__form_keywords.dot";
	snipe_zip_clients["form_location"] = dep_graph_path + "snipe_zip__form_location.dot";
	snipe_zip_clients["form_publish"] = dep_graph_path + "snipe_zip__form_publish.dot";
	snipe_zip_clients["gallery_id"] = dep_graph_path + "snipe_zip__gallery_id.dot";

	map<string,string> snipe_frame_clients;
	vector<string> snipe_frame_servers;

	snipe_frame_clients["form_frame_name"] = dep_graph_path + "snipe_frame__form_frame_name.dot";
	snipe_frame_servers.push_back(dep_graph_path + "snipe_frame.dot");

	generate_repairs_for_client_server_pairs(snipe_gallery_clients_115,snipe_gallery_servers_115, reversed);
	generate_repairs_for_client_server_pairs(snipe_gallery_clients_41,snipe_gallery_servers_41, reversed);
	generate_repairs_for_client_server_pairs(snipe_image_clients,snipe_image_servers, reversed);
	generate_repairs_for_client_server_pairs(snipe_local_clients,snipe_local_servers, reversed);
	generate_repairs_for_client_server_pairs(snipe_zip_clients,snipe_zip_servers, reversed);
	generate_repairs_for_client_server_pairs(snipe_frame_clients,snipe_frame_servers, reversed);


}


/****************************************************************************************************/
/*********** PHPNEWS ********************************************************************************/
/****************************************************************************************************/

void patch_phpnews(bool reversed) {

	string dep_graph_path = "/home/abaki/RA/PLDI/PLDI14/experiments/phpnews/";

	vector<string> phpnews_field_names;
	vector<string> phpnews_clients;
	vector<string> phpnews_servers;

	phpnews_field_names.push_back("username");
	phpnews_clients.push_back(dep_graph_path + "phpnews_auth_89__user.dot");
	phpnews_servers.push_back(dep_graph_path + "phpnews_auth_89_user_password.dot");

	phpnews_field_names.push_back("password");
	phpnews_clients.push_back(dep_graph_path + "phpnews_auth_89__password.dot");
	phpnews_servers.push_back(dep_graph_path + "phpnews_auth_89_user_password.dot");

	phpnews_field_names.push_back("subject");
	phpnews_clients.push_back(dep_graph_path + "phpnews_admin_post__subject.dot");
	phpnews_servers.push_back(dep_graph_path + "phpnews_admin_post2_1527_subject.dot");

	phpnews_field_names.push_back("subject");
	phpnews_clients.push_back(dep_graph_path + "phpnews_admin_post__subject.dot");
	phpnews_servers.push_back(dep_graph_path + "phpnews_admin_modifypost3_2382_subject.dot");

	phpnews_field_names.push_back("username");
	phpnews_clients.push_back(dep_graph_path + "phpnews_admin_poster__username.dot");
	phpnews_servers.push_back(dep_graph_path + "phpnews_admin_newsposter2_1780_email_username.dot");

	phpnews_field_names.push_back("email");
	phpnews_clients.push_back(dep_graph_path + "phpnews_admin_poster__email.dot");
	phpnews_servers.push_back(dep_graph_path + "phpnews_admin_newsposter2_1780_email_username.dot");

	phpnews_field_names.push_back("email");
	phpnews_clients.push_back(dep_graph_path + "phpnews_admin_poster__email.dot");
	phpnews_servers.push_back(dep_graph_path + "phpnews_admin_modifynewsposter3_3091_email.dot");

	generate_repairs_for_client_server_pairs(phpnews_clients, phpnews_servers, phpnews_field_names, reversed);
}


/****************************************************************************************************/
/*********** SCHOOLMATE *****************************************************************************/
/****************************************************************************************************/

void patch_schoolmate(bool reversed) {

	string dep_graph_path = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/";

	map<string,string> schoolmate_header_clients;
	vector<string> schoolmate_header_servers;
	schoolmate_header_servers.push_back(dep_graph_path + "schoolmate_header.dot");
	schoolmate_header_clients["schoolname"] = dep_graph_path + "schoolmate_header__schoolname.dot";
	schoolmate_header_clients["schooladdress"] = dep_graph_path + "schoolmate_header__schooladdress.dot";
	schoolmate_header_clients["schoolphone"] = dep_graph_path + "schoolmate_header__schoolphone.dot";
	schoolmate_header_clients["sitemessage"] = dep_graph_path + "schoolmate_header__sitemessage.dot";
	schoolmate_header_clients["sitetext"] = dep_graph_path + "schoolmate_header__sitetext.dot";
	schoolmate_header_clients["numsemesters"] = dep_graph_path + "schoolmate_header__numsemesters.dot";
	schoolmate_header_clients["numperiods"] = dep_graph_path + "schoolmate_header__numperiods.dot";
	schoolmate_header_clients["apoint"] = dep_graph_path + "schoolmate_header__apoint.dot";
	schoolmate_header_clients["bpoint"] = dep_graph_path + "schoolmate_header__bpoint.dot";
	schoolmate_header_clients["cpoint"] = dep_graph_path + "schoolmate_header__cpoint.dot";
	schoolmate_header_clients["dpoint"] = dep_graph_path + "schoolmate_header__dpoint.dot";
	schoolmate_header_clients["fpoint"] = dep_graph_path + "schoolmate_header__fpoint.dot";


	map<string,string> schoolmate_announcements_clients;
	vector<string> schoolmate_announcements_servers;
	schoolmate_announcements_servers.push_back(dep_graph_path + "schoolmate_manageannouncements_9.dot");
	schoolmate_announcements_servers.push_back(dep_graph_path + "schoolmate_manageannouncements_18.dot");
	schoolmate_announcements_clients["title"] = dep_graph_path + "schoolmate_manageannouncements__title.dot";
	schoolmate_announcements_clients["message"] = dep_graph_path + "schoolmate_manageannouncements__message.dot";


	map<string,string> schoolmate_manageclasses_clients;
	vector<string> schoolmate_manageclasses_servers;
	schoolmate_manageclasses_servers.push_back(dep_graph_path + "schoolmate_manageclasses_28.dot");
	schoolmate_manageclasses_servers.push_back(dep_graph_path + "schoolmate_manageclasses_83.dot");
	schoolmate_manageclasses_clients["semester"] = dep_graph_path + "schoolmate_manageclasses__semester.dot";
	schoolmate_manageclasses_clients["periodnum"] = dep_graph_path + "schoolmate_manageclasses__periodnum.dot";
	schoolmate_manageclasses_clients["roomnum"] = dep_graph_path + "schoolmate_manageclasses__roomnum.dot";
	schoolmate_manageclasses_clients["sectionnum"] = dep_graph_path + "schoolmate_manageclasses__sectionnum.dot";
	schoolmate_manageclasses_clients["substitute"] = dep_graph_path + "schoolmate_manageclasses__substitute.dot";
	schoolmate_manageclasses_clients["teacher"] = dep_graph_path + "schoolmate_manageclasses__teacher.dot";
	schoolmate_manageclasses_clients["title"] = dep_graph_path + "schoolmate_manageclasses__title.dot";


	map<string,string> schoolmate_manageparents_clients;
	vector<string> schoolmate_manageparents_servers;
	schoolmate_manageparents_servers.push_back(dep_graph_path + "schoolmate_manageparents_36.dot");
	schoolmate_manageparents_servers.push_back(dep_graph_path + "schoolmate_manageparents_58.dot");
	schoolmate_manageparents_clients["fname"] = dep_graph_path + "schoolmate_manageparents__fname.dot";
	schoolmate_manageparents_clients["lname"] = dep_graph_path + "schoolmate_manageparents__lname.dot";
	schoolmate_manageparents_clients["username"] = dep_graph_path + "schoolmate_manageparents__username.dot";


	map<string,string> schoolmate_managesemesters_clients;
	vector<string> schoolmate_managesemesters_servers;
	schoolmate_managesemesters_servers.push_back(dep_graph_path + "schoolmate_managesemesters_11.dot");
	schoolmate_managesemesters_servers.push_back(dep_graph_path + "schoolmate_managesemesters_23.dot");
	schoolmate_managesemesters_clients["title"] = dep_graph_path + "schoolmate_managesemesters__title.dot";
	schoolmate_managesemesters_clients["half"] = dep_graph_path + "schoolmate_managesemesters__half.dot";


	map<string,string> schoolmate_managestudents_clients;
	vector<string> schoolmate_managestudents_servers;
	schoolmate_managestudents_servers.push_back(dep_graph_path + "schoolmate_managestudents_31.dot");
	schoolmate_managestudents_servers.push_back(dep_graph_path + "schoolmate_managestudents_43.dot");
	schoolmate_managestudents_clients["fname"] = dep_graph_path + "schoolmate_managestudents__fname.dot";
	schoolmate_managestudents_clients["lname"] = dep_graph_path + "schoolmate_managestudents__lname.dot";
	schoolmate_managestudents_clients["mi"] = dep_graph_path + "schoolmate_managestudents__mi.dot";
	schoolmate_managestudents_clients["username"] = dep_graph_path + "schoolmate_managestudents__username.dot";


	map<string,string> schoolmate_manageteachers_clients;
	vector<string> schoolmate_manageteachers_servers;
	schoolmate_manageteachers_servers.push_back(dep_graph_path + "schoolmate_manageteachers_29.dot");
	schoolmate_manageteachers_servers.push_back(dep_graph_path + "schoolmate_manageteachers_39.dot");
	schoolmate_manageteachers_clients["fname"] = dep_graph_path + "schoolmate_manageteachers__fname.dot";
	schoolmate_manageteachers_clients["lname"] = dep_graph_path + "schoolmate_manageteachers__lname.dot";
	schoolmate_manageteachers_clients["username"] = dep_graph_path + "schoolmate_manageteachers__username.dot";


	map<string,string> schoolmate_manageusers_clients;
	vector<string> schoolmate_manageusers_servers;
	schoolmate_manageusers_servers.push_back(dep_graph_path + "schoolmate_manageusers_28.dot");
	schoolmate_manageusers_servers.push_back(dep_graph_path + "schoolmate_manageusers_39.dot");
	schoolmate_manageusers_clients["username"] = dep_graph_path + "schoolmate_manageusers__username.dot";
	schoolmate_manageusers_clients["type"] = dep_graph_path + "schoolmate_manageusers__type.dot";
	schoolmate_manageusers_clients["password"] = dep_graph_path + "schoolmate_manageusers_28__password.dot";

	//	single client-server pairs

	vector<string> schoolmate_field_names;
	vector<string> schoolmate_clients;
	vector<string> schoolmate_servers;

	schoolmate_field_names.push_back("announcementid");
	schoolmate_clients.push_back(dep_graph_path + "schoolmate_manageannouncements__announcementid.dot");
	schoolmate_servers.push_back(dep_graph_path + "schoolmate_manageannouncements_18.dot");

	schoolmate_field_names.push_back("semester");
	schoolmate_clients.push_back(dep_graph_path + "schoolmate_manageclasses__semester.dot");
	schoolmate_servers.push_back(dep_graph_path + "schoolmate_manageclasses_25.dot");

	schoolmate_field_names.push_back("courseid");
	schoolmate_clients.push_back(dep_graph_path + "schoolmate_manageclasses__courseid.dot");
	schoolmate_servers.push_back(dep_graph_path + "schoolmate_manageclasses_83.dot");

	schoolmate_field_names.push_back("student");
	schoolmate_clients.push_back(dep_graph_path + "schoolmate_manageparents__student.dot");
	schoolmate_servers.push_back(dep_graph_path + "schoolmate_manageparents_46.dot");

	schoolmate_field_names.push_back("parentid");
	schoolmate_clients.push_back(dep_graph_path + "schoolmate_manageparents__parentid.dot");
	schoolmate_servers.push_back(dep_graph_path + "schoolmate_manageparents_58.dot");

	schoolmate_field_names.push_back("term");
	schoolmate_clients.push_back(dep_graph_path + "schoolmate_managesemesters__term.dot");
	schoolmate_servers.push_back(dep_graph_path + "schoolmate_managesemesters_11.dot");

	schoolmate_field_names.push_back("semesterid");
	schoolmate_clients.push_back(dep_graph_path + "schoolmate_managesemesters_23__semesterid.dot");
	schoolmate_servers.push_back(dep_graph_path + "schoolmate_managesemesters_23.dot");

	schoolmate_field_names.push_back("studentid");
	schoolmate_clients.push_back(dep_graph_path + "schoolmate_managestudents_43__studentid.dot");
	schoolmate_servers.push_back(dep_graph_path + "schoolmate_managestudents_43.dot");

	schoolmate_field_names.push_back("teacherid");
	schoolmate_clients.push_back(dep_graph_path + "schoolmate_manageteachers__teacherid.dot");
	schoolmate_servers.push_back(dep_graph_path + "schoolmate_manageteachers_39.dot");

	schoolmate_field_names.push_back("title");
	schoolmate_clients.push_back(dep_graph_path + "schoolmate_manageterms__title.dot");
	schoolmate_servers.push_back(dep_graph_path + "schoolmate_manageterms_9.dot");

	schoolmate_field_names.push_back("title");
	schoolmate_clients.push_back(dep_graph_path + "schoolmate_manageterms__title.dot");
	schoolmate_servers.push_back(dep_graph_path + "schoolmate_manageterms_19.dot");

	schoolmate_field_names.push_back("termid");
	schoolmate_clients.push_back(dep_graph_path + "schoolmate_manageterms_19__termid.dot");
	schoolmate_servers.push_back(dep_graph_path + "schoolmate_manageterms_19.dot");

	schoolmate_field_names.push_back("userid");
	schoolmate_clients.push_back(dep_graph_path + "schoolmate_manageusers_39__userid.dot");
	schoolmate_servers.push_back(dep_graph_path + "schoolmate_manageusers_39.dot");

	schoolmate_field_names.push_back("username");
	schoolmate_clients.push_back(dep_graph_path + "schoolmate_validatelogin__username.dot");
	schoolmate_servers.push_back(dep_graph_path + "schoolmate_validatelogin.dot");

	generate_repairs_for_client_server_pairs(schoolmate_header_clients, schoolmate_header_servers, reversed);
	generate_repairs_for_client_server_pairs(schoolmate_announcements_clients, schoolmate_announcements_servers, reversed);
	generate_repairs_for_client_server_pairs(schoolmate_manageclasses_clients, schoolmate_manageclasses_servers, reversed);
	generate_repairs_for_client_server_pairs(schoolmate_manageparents_clients, schoolmate_manageparents_servers, reversed);
	generate_repairs_for_client_server_pairs(schoolmate_managesemesters_clients, schoolmate_managesemesters_servers, reversed);
	generate_repairs_for_client_server_pairs(schoolmate_managestudents_clients, schoolmate_managestudents_servers, reversed);
	generate_repairs_for_client_server_pairs(schoolmate_manageteachers_clients, schoolmate_manageteachers_servers, reversed);
	generate_repairs_for_client_server_pairs(schoolmate_manageusers_clients, schoolmate_manageusers_servers, reversed);
	generate_repairs_for_client_server_pairs(schoolmate_clients, schoolmate_servers, schoolmate_field_names, reversed);
}


/****************************************************************************************************/
/*********** MyBloggie *********************************************/
/****************************************************************************************************/

void patch_mybloggie_login__username(bool reversed) {
	string field_name = "username";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/mybloggie/mybloggie_login_username_password.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/mybloggie/mybloggie_login__username.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

/****************************************************************************************************/
/*********** useBB *********************************************/
/****************************************************************************************************/
void patch_usebb(bool reversed) {

	string dep_graph_path = "/home/abaki/RA/PLDI/PLDI14/experiments/usebb/";

	vector<string> usebb_field_names;
	vector<string> usebb_clients;
	vector<string> usebb_servers;

//	usebb_field_names.push_back("username");
//	usebb_clients.push_back(dep_graph_path + "usebb_login__user.dot");
//	usebb_servers.push_back(dep_graph_path + "usebb_login.dot");

//	usebb_field_names.push_back("username");
//	usebb_clients.push_back(dep_graph_path + "usebb_register__user.dot");
//	usebb_servers.push_back(dep_graph_path + "usebb_register.dot");

	usebb_field_names.push_back("email");
	usebb_clients.push_back(dep_graph_path + "usebb_register__email.dot");
	usebb_servers.push_back(dep_graph_path + "usebb_register.dot");

	generate_repairs_for_client_server_pairs(usebb_clients, usebb_servers, usebb_field_names, reversed);
}


/****************************************************************************************************/
/*********** SERVER-SERVER Comparisons *********************************************/
/****************************************************************************************************/

void patch_server_server(bool reversed) {

	string dep_graph_path = "/home/abaki/RA/PLDI/PLDI14/experiments/";

	vector<string> username_field_name;
	vector<string> username_servers;


	username_field_name.push_back("username");
	username_servers.push_back(dep_graph_path + "mybloggie/mybloggie_login_username_password.dot");
	username_servers.push_back(dep_graph_path + "usebb/usebb_register.dot");
	username_servers.push_back(dep_graph_path + "usebb/usebb_login.dot");
	username_servers.push_back(dep_graph_path + "phpnews/phpnews_admin_newsposter2_1780_email_username.dot");
	username_servers.push_back(dep_graph_path + "phpnews/phpnews_auth_89_user_password.dot");
	username_servers.push_back(dep_graph_path + "schoolmate/schoolmate_manageparents_36.dot");
	username_servers.push_back(dep_graph_path + "schoolmate/schoolmate_manageparents_58.dot");
	username_servers.push_back(dep_graph_path + "schoolmate/schoolmate_managestudents_31.dot");
	username_servers.push_back(dep_graph_path + "schoolmate/schoolmate_managestudents_43.dot");
	username_servers.push_back(dep_graph_path + "schoolmate/schoolmate_manageteachers_29.dot");
	username_servers.push_back(dep_graph_path + "schoolmate/schoolmate_manageteachers_39.dot");
	username_servers.push_back(dep_graph_path + "schoolmate/schoolmate_manageusers_28.dot");
	username_servers.push_back(dep_graph_path + "schoolmate/schoolmate_manageusers_39.dot");
	username_servers.push_back(dep_graph_path + "schoolmate/schoolmate_validatelogin.dot");

	vector<string> email_field_name;
	vector<string> email_servers;
	email_field_name.push_back("email");
	email_servers.push_back(dep_graph_path + "usebb/usebb_register.dot");
	email_servers.push_back(dep_graph_path + "phpnews/phpnews_admin_newsposter2_1780_email_username.dot");
	email_servers.push_back(dep_graph_path + "phpnews/phpnews_admin_modifynewsposter3_3091_email.dot");

	// snipe gallery

	vector<string> snipe1_field_names;
	vector<string> snipe1_servers;
	snipe1_field_names.push_back("form_author");
	snipe1_field_names.push_back("form_details");
	snipe1_field_names.push_back("form_keywords");
	snipe1_field_names.push_back("form_location");
	snipe1_field_names.push_back("form_publish");
	snipe1_servers.push_back(dep_graph_path + "snipegallery/snipe_image_177.dot");
	snipe1_servers.push_back(dep_graph_path + "snipegallery/snipe_image_539.dot");
	snipe1_servers.push_back(dep_graph_path + "snipegallery/snipe_local.dot");
	snipe1_servers.push_back(dep_graph_path + "snipegallery/snipe_zip.dot");

	vector<string> snipe2_field_names;
	vector<string> snipe2_servers;
	snipe2_field_names.push_back("cat_parent");
	snipe2_field_names.push_back("form_cat_description");
	snipe2_field_names.push_back("form_gallery_name");
	snipe2_servers.push_back(dep_graph_path + "snipegallery/snipe_gallery_115.dot");
	snipe2_servers.push_back(dep_graph_path + "snipegallery/snipe_gallery_41.dot");

	vector<string> snipe3_field_names;
	vector<string> snipe3_servers;
	snipe3_field_names.push_back("gallery_id");
	snipe3_servers.push_back(dep_graph_path + "snipegallery/snipe_gallery_41.dot");
	snipe3_servers.push_back(dep_graph_path + "snipegallery/snipe_image_177.dot");
	snipe3_servers.push_back(dep_graph_path + "snipegallery/snipe_image_539.dot");
	snipe3_servers.push_back(dep_graph_path + "snipegallery/snipe_local.dot");
	snipe3_servers.push_back(dep_graph_path + "snipegallery/snipe_zip.dot");

	// phpnews
	vector<string> phpnews_field_names;
	vector<string> phpnews_servers;
	phpnews_field_names.push_back("subject");
	phpnews_servers.push_back(dep_graph_path + "phpnews/phpnews_admin_post2_1527_subject.dot");
	phpnews_servers.push_back(dep_graph_path + "phpnews/phpnews_admin_modifypost3_2382_subject.dot");

	// schoolmate
	vector<string> schoolmate1_field_names;
	vector<string> schoolmate1_servers;
	schoolmate1_field_names.push_back("message");
	schoolmate1_servers.push_back(dep_graph_path + "schoolmate/schoolmate_manageannouncements_9.dot");
	schoolmate1_servers.push_back(dep_graph_path + "schoolmate/schoolmate_manageannouncements_18.dot");

	vector<string> schoolmate2_field_names;
	vector<string> schoolmate2_servers;
	schoolmate2_field_names.push_back("semester");
	schoolmate2_field_names.push_back("periodnum");
	schoolmate2_field_names.push_back("sectionnum");
	schoolmate2_field_names.push_back("substitute");
	schoolmate2_field_names.push_back("teacher");
	schoolmate2_servers.push_back(dep_graph_path + "schoolmate/schoolmate_manageclasses_28.dot");
	schoolmate2_servers.push_back(dep_graph_path + "schoolmate/schoolmate_manageclasses_83.dot");

	vector<string> schoolmate3_field_names;
	vector<string> schoolmate3_servers;
	schoolmate3_field_names.push_back("title");
	schoolmate3_servers.push_back(dep_graph_path + "schoolmate/schoolmate_manageannouncements_9.dot");
	schoolmate3_servers.push_back(dep_graph_path + "schoolmate/schoolmate_manageannouncements_18.dot");
	schoolmate3_servers.push_back(dep_graph_path + "schoolmate/schoolmate_manageclasses_28.dot");
	schoolmate3_servers.push_back(dep_graph_path + "schoolmate/schoolmate_manageclasses_83.dot");
	schoolmate3_servers.push_back(dep_graph_path + "schoolmate/schoolmate_managesemesters_11.dot");
	schoolmate3_servers.push_back(dep_graph_path + "schoolmate/schoolmate_managesemesters_23.dot");
	schoolmate3_servers.push_back(dep_graph_path + "schoolmate/schoolmate_manageterms_9.dot");
	schoolmate3_servers.push_back(dep_graph_path + "schoolmate/schoolmate_manageterms_19.dot");

	vector<string> schoolmate4_field_names;
	vector<string> schoolmate4_servers;
	schoolmate4_field_names.push_back("fname");
	schoolmate4_field_names.push_back("lname");
	schoolmate4_servers.push_back(dep_graph_path + "schoolmate/schoolmate_manageteachers_29.dot");
	schoolmate4_servers.push_back(dep_graph_path + "schoolmate/schoolmate_manageteachers_39.dot");
	schoolmate4_servers.push_back(dep_graph_path + "schoolmate/schoolmate_managestudents_31.dot");
	schoolmate4_servers.push_back(dep_graph_path + "schoolmate/schoolmate_managestudents_43.dot");
	schoolmate4_servers.push_back(dep_graph_path + "schoolmate/schoolmate_manageparents_36.dot");
	schoolmate4_servers.push_back(dep_graph_path + "schoolmate/schoolmate_manageparents_58.dot");

	vector<string> schoolmate5_field_names;
	vector<string> schoolmate5_servers;
	schoolmate5_field_names.push_back("half");
	schoolmate5_servers.push_back(dep_graph_path + "schoolmate/schoolmate_managesemesters_11.dot");
	schoolmate5_servers.push_back(dep_graph_path + "schoolmate/schoolmate_managesemesters_23.dot");

	vector<string> schoolmate6_field_names;
	vector<string> schoolmate6_servers;
	schoolmate6_field_names.push_back("mi");
	schoolmate6_servers.push_back(dep_graph_path + "schoolmate/schoolmate_managestudents_31.dot");
	schoolmate6_servers.push_back(dep_graph_path + "schoolmate/schoolmate_managestudents_43.dot");

	vector<string> schoolmate7_field_names;
	vector<string> schoolmate7_servers;
	schoolmate7_field_names.push_back("type");
	schoolmate7_field_names.push_back("password");
	schoolmate7_servers.push_back(dep_graph_path + "schoolmate/schoolmate_manageusers_28.dot");
	schoolmate7_servers.push_back(dep_graph_path + "schoolmate/schoolmate_manageusers_39.dot");

	generate_repairs_for_ss_cc_pairs(username_servers, username_field_name, reversed);
	generate_repairs_for_ss_cc_pairs(email_servers, email_field_name, reversed);
	generate_repairs_for_ss_cc_pairs(snipe1_servers, snipe1_field_names, reversed);
	generate_repairs_for_ss_cc_pairs(snipe2_servers, snipe2_field_names, reversed);
	generate_repairs_for_ss_cc_pairs(snipe3_servers, snipe3_field_names, reversed);
	generate_repairs_for_ss_cc_pairs(phpnews_servers, phpnews_field_names, reversed);
	generate_repairs_for_ss_cc_pairs(schoolmate1_servers, schoolmate1_field_names, reversed);
	generate_repairs_for_ss_cc_pairs(schoolmate2_servers, schoolmate2_field_names, reversed);
	generate_repairs_for_ss_cc_pairs(schoolmate3_servers, schoolmate3_field_names, reversed);
	generate_repairs_for_ss_cc_pairs(schoolmate4_servers, schoolmate4_field_names, reversed);
	generate_repairs_for_ss_cc_pairs(schoolmate5_servers, schoolmate5_field_names, reversed);
	generate_repairs_for_ss_cc_pairs(schoolmate6_servers, schoolmate6_field_names, reversed);
	generate_repairs_for_ss_cc_pairs(schoolmate7_servers, schoolmate7_field_names, reversed);
}

/****************************************************************************************************/
/*********** CLIENT-CLIENT Comparisons *********************************************/
/****************************************************************************************************/

void patch_client_client(bool reversed) {

	string dep_graph_path = "/home/abaki/RA/PLDI/PLDI14/experiments/js/";

	vector<string> email_field_name;
	vector<string> email_clients;

	email_field_name.push_back("email");
	email_clients.push_back(dep_graph_path + "client_email_1.dot");
	email_clients.push_back(dep_graph_path + "client_email_2.dot");
	email_clients.push_back(dep_graph_path + "client_email_3.dot");
	email_clients.push_back(dep_graph_path + "client_email_4.dot");
	email_clients.push_back(dep_graph_path + "client_email_5.dot");
	email_clients.push_back(dep_graph_path + "client_email_6.dot");

	vector<string> date_field_name;
	vector<string> date_clients;

	date_field_name.push_back("date");
	date_clients.push_back(dep_graph_path + "client_date_1.dot");
	date_clients.push_back(dep_graph_path + "client_date_2.dot");

	vector<string> phone_field_name;
	vector<string> phone_clients;
	phone_field_name.push_back("phone");
	phone_clients.push_back(dep_graph_path + "client_phone_1.dot");
	phone_clients.push_back(dep_graph_path + "client_phone_2.dot");
//	phone_clients.push_back(dep_graph_path + "client_phone_3.dot");


//	generate_repairs_for_ss_cc_pairs(email_clients, email_field_name, reversed);
//	generate_repairs_for_ss_cc_pairs(date_clients, date_field_name, reversed);
	generate_repairs_for_ss_cc_pairs(phone_clients, phone_field_name, reversed);
}



void test_performance() {
	replaceVsAddslashes();
//	replaceVsHtmlSpecialChars();
}



int main(int argc, char *argv[]) {

	bool reversed = false;

//	run_paper_example(reversed);

//	patch_schoolmate(reversed);
//	patch_phpnews(reversed);
//	patch_snipe_gallery(reversed);
//	patch_usebb(reversed);
//	patch_mybloggie_login__username(reversed);


//	patch_server_server(reversed);

//	patch_client_client(reversed);


	PerfInfo *perfInfo = new PerfInfo();
	StrangerAutomaton::perfInfo = perfInfo;
//
//	test_performance();


	StrangerAutomaton* subject = StrangerAutomaton::regExToAuto("/.+/");
	StrangerAutomaton* anystring = StrangerAutomaton::makeAnyString();
	StrangerAutomaton* negsubject = subject->complement();
	StrangerAutomaton* regx = StrangerAutomaton::makeString("<");

	string x = "";
	StrangerAutomaton* forward = StrangerAutomaton::reg_replace(regx,"",anystring);
	forward->toDotAscii(0);

	StrangerAutomaton* intersect = forward->intersect(negsubject);
	intersect->toDotAscii(0);
	StrangerAutomaton* preresult = intersect->preReplace(regx,"");
	preresult->toDotAscii(0);

	return 0;
}
