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
		StrangerPatcher strangerPatcher(patcher_name, patchee_name, field_name);
		strangerPatcher.extractValidationPatch();
		strangerPatcher.extractSanitizationPatch();

		cout << endl << "\t------ OVERALL RESULT for: " << field_name << " ------" << endl;
		cout << "\t    Patcher: " << patcher_name << endl;
		cout << "\t    Patchee: " << patchee_name << endl;

		strangerPatcher.printResults();
		strangerPatcher.writeAutoforMinCut(patcher_name, patchee_name);

		cout << endl << "\t------ END RESULT for: " << field_name << " ------" << endl;
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
	string field_name = "numperiods";
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

void patch_schoolmate_manageparents_46__student(bool reversed) {
	string field_name = "student";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageparents_46.dot";
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

void patch_schoolmate_managesemesters_11__title(bool reversed) {
	string field_name = "title";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_managesemesters_11.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_managesemesters__title.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_schoolmate_managesemesters_11__term(bool reversed) {
	string field_name = "term";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_managesemesters_11.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_managesemesters__term.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_schoolmate_managesemesters_11__half(bool reversed) {
	string field_name = "half";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_managesemesters_11.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_managesemesters__half.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_schoolmate_managesemesters_23__title(bool reversed) {
	string field_name = "title";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_managesemesters_23.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_managesemesters__title.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_schoolmate_managesemesters_23__half(bool reversed) {
	string field_name = "half";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_managesemesters_23.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_managesemesters__half.dot";
	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_schoolmate_managesemesters_23__semesterid(bool reversed) {
	string field_name = "semesterid";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_managesemesters_23.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_managesemesters_23__semesterid.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_schoolmate_managestudents_31__fname(bool reversed) {
	string field_name = "fname";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_managestudents_31.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_managestudents__fname.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_schoolmate_managestudents_31__lname(bool reversed) {
	string field_name = "lname";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_managestudents_31.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_managestudents__lname.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_schoolmate_managestudents_31__mi(bool reversed) {
	string field_name = "mi";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_managestudents_31.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_managestudents__mi.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_schoolmate_managestudents_31__username(bool reversed) {
	string field_name = "username";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_managestudents_31.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_managestudents__username.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_schoolmate_managestudents_43__fname(bool reversed) {
	string field_name = "fname";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_managestudents_43.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_managestudents__fname.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_schoolmate_managestudents_43__lname(bool reversed) {
	string field_name = "lname";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_managestudents_43.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_managestudents__lname.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_schoolmate_managestudents_43__mi(bool reversed) {
	string field_name = "mi";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_managestudents_43.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_managestudents__mi.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_schoolmate_managestudents_43__username(bool reversed) {
	string field_name = "username";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_managestudents_43.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_managestudents__username.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_schoolmate_managestudents_43__studentid(bool reversed) {
	string field_name = "studentid";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_managestudents_43.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_managestudents_43__studentid.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_schoolmate_manageteachers_29__fname(bool reversed) {
	string field_name = "fname";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageteachers_29.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageteachers__fname.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_schoolmate_manageteachers_29__lname(bool reversed) {
	string field_name = "lname";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageteachers_29.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageteachers__lname.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_schoolmate_manageteachers_29__username(bool reversed) {
	string field_name = "username";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageteachers_29.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageteachers__username.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_schoolmate_manageteachers_39__fname(bool reversed) {
	string field_name = "fname";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageteachers_39.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageteachers__fname.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_schoolmate_manageteachers_39__lname(bool reversed) {
	string field_name = "lname";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageteachers_39.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageteachers__lname.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_schoolmate_manageteachers_39__username(bool reversed) {
	string field_name = "username";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageteachers_39.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageteachers__username.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_schoolmate_manageteachers_39__teacherid(bool reversed) {
	string field_name = "teacherid";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageteachers_39.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageteachers__teacherid.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}



void patch_schoolmate_manageterms_9__title(bool reversed) {
	string field_name = "title";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageterms_9.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageterms__title.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_schoolmate_manageterms_19__title(bool reversed) {
	string field_name = "title";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageterms_19.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageterms__title.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_schoolmate_manageterms_19__termid(bool reversed) {
	string field_name = "termid";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageterms_19.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageterms_19__termid.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_schoolmate_manageusers_28__username(bool reversed) {
	string field_name = "username";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageusers_28.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageusers__username.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_schoolmate_manageusers_28__type(bool reversed) {
	string field_name = "type";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageusers_28.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageusers__type.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_schoolmate_manageusers_28__password(bool reversed) {
	string field_name = "password";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageusers_28.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageusers_28__password.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_schoolmate_manageusers_39__username(bool reversed) {
	string field_name = "username";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageusers_39.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageusers__username.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_schoolmate_manageusers_39__type(bool reversed) {
	string field_name = "type";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageusers_39.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageusers__type.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_schoolmate_manageusers_39__password(bool reversed) {
	string field_name = "password";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageusers_39.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageusers_39__password.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_schoolmate_manageusers_39__userid(bool reversed) {
	string field_name = "userid";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageusers_39.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_manageusers_39__userid.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_schoolmate_validatelogin__username(bool reversed) {
	string field_name = "username";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_validatelogin.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_validatelogin__username.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
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

void patch_usebb_login__user(bool reversed) {
	string field_name = "user";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/usebb/usebb_login.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/usebb/usebb_login_user.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_usebb_register__user(bool reversed) {
	string field_name = "user";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/usebb/usebb_register.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/usebb/usebb_register_user.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

void patch_usebb_register__email(bool reversed) {
	string field_name = "email";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/usebb/usebb_register.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/usebb/usebb_register_email.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

/****************************************************************************************************/
/*********** SERVER-SERVER Comparisons *********************************************/
/****************************************************************************************************/

void patch_(bool reversed) {
	string field_name = "username";
	string server = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_validatelogin.dot";
	string client = "/home/abaki/RA/PLDI/PLDI14/experiments/schoolmate/schoolmate_validatelogin__username.dot";

	if (reversed) {
		call_patcher(server, client, field_name);
	} else {
		call_patcher(client, server, field_name);
	}
}

/****************************************************************************************************/
/*********** CLIENT-CLIENT Comparisons *********************************************/
/****************************************************************************************************/

void patch_email_1__email_2(bool reversed) {
	string field_name = "email";
	string reference = "/home/abaki/RA/PLDI/PLDI14/experiments/js/client_email_1.dot";
	string target = "/home/abaki/RA/PLDI/PLDI14/experiments/js/client_email_2.dot";

	if (reversed) {
		call_patcher(target, reference, field_name);
	} else {
		call_patcher(reference, target, field_name);
	}
}

void patch_email_1__email_3(bool reversed) {
	string field_name = "email";
	string reference = "/home/abaki/RA/PLDI/PLDI14/experiments/js/client_email_1.dot";
	string target = "/home/abaki/RA/PLDI/PLDI14/experiments/js/client_email_3.dot";

	if (reversed) {
		call_patcher(target, reference, field_name);
	} else {
		call_patcher(reference, target, field_name);
	}
}

void patch_email_1__email_4(bool reversed) {
	string field_name = "email";
	string reference = "/home/abaki/RA/PLDI/PLDI14/experiments/js/client_email_1.dot";
	string target = "/home/abaki/RA/PLDI/PLDI14/experiments/js/client_email_4.dot";

	if (reversed) {
		call_patcher(target, reference, field_name);
	} else {
		call_patcher(reference, target, field_name);
	}
}

void patch_email_1__email_5(bool reversed) {
	string field_name = "email";
	string reference = "/home/abaki/RA/PLDI/PLDI14/experiments/js/client_email_1.dot";
	string target = "/home/abaki/RA/PLDI/PLDI14/experiments/js/client_email_5.dot";

	if (reversed) {
		call_patcher(target, reference, field_name);
	} else {
		call_patcher(reference, target, field_name);
	}
}

void patch_email_1__email_6(bool reversed) {
	string field_name = "email";
	string reference = "/home/abaki/RA/PLDI/PLDI14/experiments/js/client_email_1.dot";
	string target = "/home/abaki/RA/PLDI/PLDI14/experiments/js/client_email_6.dot";

	if (reversed) {
		call_patcher(target, reference, field_name);
	} else {
		call_patcher(reference, target, field_name);
	}
}

void patch_email_2__email_3(bool reversed) {
	string field_name = "email";
	string reference = "/home/abaki/RA/PLDI/PLDI14/experiments/js/client_email_2.dot";
	string target = "/home/abaki/RA/PLDI/PLDI14/experiments/js/client_email_3.dot";

	if (reversed) {
		call_patcher(target, reference, field_name);
	} else {
		call_patcher(reference, target, field_name);
	}
}

void patch_email_2__email_4(bool reversed) {
	string field_name = "email";
	string reference = "/home/abaki/RA/PLDI/PLDI14/experiments/js/client_email_2.dot";
	string target = "/home/abaki/RA/PLDI/PLDI14/experiments/js/client_email_4.dot";

	if (reversed) {
		call_patcher(target, reference, field_name);
	} else {
		call_patcher(reference, target, field_name);
	}
}

void patch_email_2__email_5(bool reversed) {
	string field_name = "email";
	string reference = "/home/abaki/RA/PLDI/PLDI14/experiments/js/client_email_2.dot";
	string target = "/home/abaki/RA/PLDI/PLDI14/experiments/js/client_email_5.dot";

	if (reversed) {
		call_patcher(target, reference, field_name);
	} else {
		call_patcher(reference, target, field_name);
	}
}

void patch_email_2__email_6(bool reversed) {
	string field_name = "email";
	string reference = "/home/abaki/RA/PLDI/PLDI14/experiments/js/client_email_2.dot";
	string target = "/home/abaki/RA/PLDI/PLDI14/experiments/js/client_email_6.dot";

	if (reversed) {
		call_patcher(target, reference, field_name);
	} else {
		call_patcher(reference, target, field_name);
	}
}

void patch_email_3__email_4(bool reversed) {
	string field_name = "email";
	string reference = "/home/abaki/RA/PLDI/PLDI14/experiments/js/client_email_3.dot";
	string target = "/home/abaki/RA/PLDI/PLDI14/experiments/js/client_email_4.dot";

	if (reversed) {
		call_patcher(target, reference, field_name);
	} else {
		call_patcher(reference, target, field_name);
	}
}

void patch_email_3__email_5(bool reversed) {
	string field_name = "email";
	string reference = "/home/abaki/RA/PLDI/PLDI14/experiments/js/client_email_3.dot";
	string target = "/home/abaki/RA/PLDI/PLDI14/experiments/js/client_email_5.dot";

	if (reversed) {
		call_patcher(target, reference, field_name);
	} else {
		call_patcher(reference, target, field_name);
	}
}

void patch_email_3__email_6(bool reversed) {
	string field_name = "email";
	string reference = "/home/abaki/RA/PLDI/PLDI14/experiments/js/client_email_3.dot";
	string target = "/home/abaki/RA/PLDI/PLDI14/experiments/js/client_email_6.dot";

	if (reversed) {
		call_patcher(target, reference, field_name);
	} else {
		call_patcher(reference, target, field_name);
	}
}

void patch_email_4__email_5(bool reversed) {
	string field_name = "email";
	string reference = "/home/abaki/RA/PLDI/PLDI14/experiments/js/client_email_4.dot";
	string target = "/home/abaki/RA/PLDI/PLDI14/experiments/js/client_email_5.dot";

	if (reversed) {
		call_patcher(target, reference, field_name);
	} else {
		call_patcher(reference, target, field_name);
	}
}

void patch_email_4__email_6(bool reversed) {
	string field_name = "email";
	string reference = "/home/abaki/RA/PLDI/PLDI14/experiments/js/client_email_4.dot";
	string target = "/home/abaki/RA/PLDI/PLDI14/experiments/js/client_email_6.dot";

	if (reversed) {
		call_patcher(target, reference, field_name);
	} else {
		call_patcher(reference, target, field_name);
	}
}

void patch_email_5__email_6(bool reversed) {
	string field_name = "email";
	string reference = "/home/abaki/RA/PLDI/PLDI14/experiments/js/client_email_5.dot";
	string target = "/home/abaki/RA/PLDI/PLDI14/experiments/js/client_email_6.dot";

	if (reversed) {
		call_patcher(target, reference, field_name);
	} else {
		call_patcher(reference, target, field_name);
	}
}

void patch_date_1__date_2(bool reversed) {
	string field_name = "date";
	string reference = "/home/abaki/RA/PLDI/PLDI14/experiments/js/client_date_1.dot";
	string target = "/home/abaki/RA/PLDI/PLDI14/experiments/js/client_date_2.dot";

	if (reversed) {
		call_patcher(target, reference, field_name);
	} else {
		call_patcher(reference, target, field_name);
	}
}

void patch_phone_1__phone_2(bool reversed) {
	string field_name = "phone";
	string reference = "/home/abaki/RA/PLDI/PLDI14/experiments/js/client_phone_1.dot";
	string target = "/home/abaki/RA/PLDI/PLDI14/experiments/js/client_phone_2.dot";

	if (reversed) {
		call_patcher(target, reference, field_name);
	} else {
		call_patcher(reference, target, field_name);
	}
}

void patch_phone_1__phone_3(bool reversed) {
	string field_name = "phone";
	string reference = "/home/abaki/RA/PLDI/PLDI14/experiments/js/client_phone_1.dot";
	string target = "/home/abaki/RA/PLDI/PLDI14/experiments/js/client_phone_3.dot";

	if (reversed) {
		call_patcher(target, reference, field_name);
	} else {
		call_patcher(reference, target, field_name);
	}
}

void patch_phone_2__phone_3(bool reversed) {
	string field_name = "phone";
	string reference = "/home/abaki/RA/PLDI/PLDI14/experiments/js/client_phone_2.dot";
	string target = "/home/abaki/RA/PLDI/PLDI14/experiments/js/client_phone_3.dot";

	if (reversed) {
		call_patcher(target, reference, field_name);
	} else {
		call_patcher(reference, target, field_name);
	}
}


int main(int argc, char *argv[]) {

/***********	Client - Server Patching	***********/
	bool patchClient = false;
	bool reversed = false;

//	patch_paperexample_email(patchClient);

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

//	patch_schoolmate_announcements_9__title(patchClient);
//	patch_schoolmate_announcements_9__message(patchClient);
//	patch_schoolmate_announcements_18__title(patchClient);
//	patch_schoolmate_announcements_18__message(patchClient);
//	patch_schoolmate_announcements_18__announcementid(patchClient);
//	patch_schoolmate_header__apoint(patchClient);
//	patch_schoolmate_header__bpoint(patchClient);
//	patch_schoolmate_header__cpoint(patchClient);
//	patch_schoolmate_header__dpoint(patchClient);
//	patch_schoolmate_header__fpoint(patchClient);
//	patch_schoolmate_header__numperiods(patchClient);
//	patch_schoolmate_header__numsemesters(patchClient);
//	patch_schoolmate_header__schooladdress(patchClient);
//	patch_schoolmate_header__schoolname(patchClient);
//	patch_schoolmate_header__schoolphone(patchClient);
//	patch_schoolmate_header__sitemessage(patchClient);
//	patch_schoolmate_header__sitetext(patchClient);
//	patch_schoolmate_manageclasses_25__semester(patchClient);
//	patch_schoolmate_manageclasses_28__periodnum(patchClient);
//	patch_schoolmate_manageclasses_28__roomnum(patchClient);
//	patch_schoolmate_manageclasses_28__sectionnum(patchClient);
//	patch_schoolmate_manageclasses_28__semester(patchClient);
//	patch_schoolmate_manageclasses_28__substitute(patchClient);
//	patch_schoolmate_manageclasses_28__teacher(patchClient);
//	patch_schoolmate_manageclasses_28__title(patchClient);
//	patch_schoolmate_manageclasses_83__courseid(patchClient);
//	patch_schoolmate_manageclasses_83__periodnum(patchClient);
//	patch_schoolmate_manageclasses_83__roomnum(patchClient);
//	patch_schoolmate_manageclasses_83__sectionnum(patchClient);
//	patch_schoolmate_manageclasses_83__semester(patchClient);
//	patch_schoolmate_manageclasses_83__substitute(patchClient);
//	patch_schoolmate_manageclasses_83__teacher(patchClient);
//	patch_schoolmate_manageclasses_83__title(patchClient);
//	patch_schoolmate_manageparents_36__fname(patchClient);
//	patch_schoolmate_manageparents_36__lname(patchClient);
//	patch_schoolmate_manageparents_46__student(patchClient);
//	patch_schoolmate_manageparents_36__username(patchClient);
//	patch_schoolmate_manageparents_58__fname(patchClient);
//	patch_schoolmate_manageparents_58__lname(patchClient);
//	patch_schoolmate_manageparents_58__username(patchClient);
//	patch_schoolmate_manageparents_58__parentid(patchClient);
//	patch_schoolmate_managesemesters_11__half(patchClient);
//	patch_schoolmate_managesemesters_11__term(patchClient);
//	patch_schoolmate_managesemesters_11__title(patchClient);
//	patch_schoolmate_managesemesters_23__half(patchClient);
//	patch_schoolmate_managesemesters_23__title(patchClient);
//	patch_schoolmate_managesemesters_23__semesterid(patchClient);
//	patch_schoolmate_managestudents_31__fname(patchClient);
//	patch_schoolmate_managestudents_31__lname(patchClient);
//	patch_schoolmate_managestudents_31__mi(patchClient);
//	patch_schoolmate_managestudents_31__username(patchClient);
//	patch_schoolmate_managestudents_43__fname(patchClient);
//	patch_schoolmate_managestudents_43__lname(patchClient);
//	patch_schoolmate_managestudents_43__mi(patchClient);
//	patch_schoolmate_managestudents_43__username(patchClient);
//	patch_schoolmate_managestudents_43__studentid(patchClient);
//	patch_schoolmate_manageteachers_29__fname(patchClient);
//	patch_schoolmate_manageteachers_29__lname(patchClient);
//	patch_schoolmate_manageteachers_29__username(patchClient);
//	patch_schoolmate_manageteachers_39__fname(patchClient);
//	patch_schoolmate_manageteachers_39__lname(patchClient);
//	patch_schoolmate_manageteachers_39__username(patchClient);
//	patch_schoolmate_manageteachers_39__teacherid(patchClient);
//	patch_schoolmate_manageterms_9__title(patchClient);
//	patch_schoolmate_manageterms_19__title(patchClient);
//	patch_schoolmate_manageterms_19__termid(patchClient);
//	patch_schoolmate_manageusers_28__password(patchClient);
//	patch_schoolmate_manageusers_28__type(patchClient);
//	patch_schoolmate_manageusers_28__username(patchClient);
//	patch_schoolmate_manageusers_39__password(patchClient);
//	patch_schoolmate_manageusers_39__type(patchClient);
//	patch_schoolmate_manageusers_39__username(patchClient);
//	patch_schoolmate_manageusers_39__userid(patchClient);
//	patch_schoolmate_validatelogin__username(patchClient);

	patch_mybloggie_login__username(patchClient);

//	patch_usebb_login_user(patchClient);
//	patch_usebb_register_user(patchClient);
//	patch_usebb_register_email(patchClient);

//	Server-Server Comparisons ////////////////////////////////////////


//	Client-Client Comparisons ////////////////////////////////////////

//	patch_email_1__email_2(reversed);
//	patch_email_1__email_3(reversed);
//	patch_email_1__email_4(reversed);
//	patch_email_1__email_5(reversed);
//	patch_email_1__email_6(reversed);
//	patch_email_2__email_3(reversed);
//	patch_email_2__email_4(reversed);
//	patch_email_2__email_5(reversed);
//	patch_email_2__email_6(reversed);
//	patch_email_3__email_4(reversed);
//	patch_email_3__email_6(reversed);
//	patch_email_3__email_6(reversed);
//	patch_email_4__email_5(reversed);
//	patch_email_4__email_6(reversed);
//	patch_email_5__email_6(reversed);
//
//	patch_date_1__date_2(reversed);

//	patch_phone_1__phone_2(reversed);
//	patch_phone_1__phone_3(reversed);
//	patch_phone_2__phone_3(reversed);


	return 0;
}
