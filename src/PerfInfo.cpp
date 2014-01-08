/*
 * PerfInfo.cpp
 *
 *  Created on: Aug 4, 2013
 *      Author: muath
 */

#include "PerfInfo.h"

using namespace std;

PerfInfo::PerfInfo() {
    reset();
}

PerfInfo::~PerfInfo() {

}

void PerfInfo::reset() {


	num_of_intersect = 0;
	num_of_union = 0;
	num_of_closure = 0;
	num_of_complement = 0;
	num_of_precisewiden = 0;
	num_of_coarsewiden = 0;
	num_of_concat = 0;
	num_of_pre_concat = 0;
	num_of_const_pre_concat = 0;
	num_of_replace = 0;
	num_of_pre_replace = 0;

	number_of_vlab_restrict = 0;
	number_of_pre_vlab_restrict = 0;
	number_of_addslashes = 0;
	number_of_pre_addslashes = 0;
	number_of_htmlspecialchars = 0;
	number_of_pre_htmlspecialchars = 0;
	number_of_stripslashes = 0;
	number_of_pre_stripslashes = 0;
	number_of_mysql_escape_string = 0;
	number_of_pre_mysql_escape_string = 0;
	number_of_to_uppercase = 0;
	number_of_pre_to_uppercase = 0;
	number_of_to_lowercase = 0;
	number_of_pre_to_lowercase = 0;
	number_of_trim_spaces = 0;
	number_of_pre_trim_spaces = 0;
	number_of_trim_spaces_left = 0;
	number_of_pre_trim_spaces_left = 0;
	number_of_trim_spaces_right = 0;
	number_of_pre_trim_spaces_right = 0;
	number_of_trim_set = 0;
	number_of_pre_trim_set = 0;

}


boost::posix_time::ptime PerfInfo::current_time() {
	return boost::posix_time::microsec_clock::local_time();
}

void PerfInfo::calculate_total_validation_extraction_time() {
	validation_patch_extraction_total_time = validation_patchee_backward_time + validation_patcher_backward_time + validation_comparison_time;
}

void PerfInfo::calculate_total_sanitization_extraction_time() {
	sanitization_patch_extraction_total_time = sanitization_patchee_first_forward_time + sanitization_patcher_first_forward_time +
			sanitization_length_issue_check_time + sanitization_length_backward_time + sanitization_length_forward_time +
			sanitization_last_backward_time + sanitization_comparison_time;
}

void PerfInfo::print_validation_extraction_info() {
	cout << endl << "\t Validation Patch Analysis Info" << endl;
	cout << "\t time : patchee backward analysis time : " << validation_patchee_backward_time << " (" << validation_patchee_backward_time.total_milliseconds() << " ms)" << endl;
	cout << "\t time : patcher backward analysis time : " << validation_patcher_backward_time << " (" << validation_patcher_backward_time.total_milliseconds() << " ms)" << endl;
	cout << "\t time : validation comparison time : " << validation_comparison_time << " (" << validation_comparison_time.total_milliseconds() << " ms)" << endl;
	cout << "\t time : validation patch extraction total time : " << validation_patch_extraction_total_time << " (" << validation_patch_extraction_total_time.total_milliseconds() << " ms)" << endl;

}

void PerfInfo::print_sanitization_extraction_info() {
	cout << endl << "\t Sanitization Patch Analysis Info" << endl;
	cout << "\t time : patchee first forward analysis time : " << sanitization_patchee_first_forward_time << " (" << sanitization_patchee_first_forward_time.total_milliseconds() << " ms)" << endl;
	cout << "\t time : patcher first forward analysis time : " << sanitization_patcher_first_forward_time << " (" << sanitization_patcher_first_forward_time.total_milliseconds() << " ms)" << endl;
	cout << "\t time : length issue check time : " << sanitization_length_issue_check_time << " (" << sanitization_length_issue_check_time.total_milliseconds() << " ms)" << endl;
	cout << "\t time : length issue backward time : " << sanitization_length_backward_time << " (" << sanitization_length_backward_time.total_milliseconds() << " ms)" << endl;
	cout << "\t time : length issue forward time : " << sanitization_length_forward_time << " (" << sanitization_length_forward_time.total_milliseconds() << " ms)" << endl;
	cout << "\t time : last backward time : " << sanitization_last_backward_time << " (" << sanitization_last_backward_time.total_milliseconds() << " ms)" << endl;
	cout << "\t time : sanitization comparison time : " << sanitization_comparison_time << " (" << sanitization_comparison_time.total_milliseconds() << " ms)" << endl;
	cout << "\t time : sanitization patch extraction total time : " << sanitization_patch_extraction_total_time << " (" << sanitization_patch_extraction_total_time.total_milliseconds() << " ms)" << endl;

}

void PerfInfo::print_operations_info() {

	cout << endl <<"\t Stranger Automaton Operations Info" << endl;
	cout << "\t intersection : #" << num_of_intersect << " : " << intersect_total_time << " (" << intersect_total_time.total_milliseconds() << " ms)" << endl;
	cout << "\t union : #" << num_of_union << " : " << union_total_time << " (" << union_total_time.total_milliseconds() << " ms)" << endl;
	cout << "\t closure : #" << num_of_closure << " : " << closure_total_time << " (" << closure_total_time.total_milliseconds() << " ms)" << endl;
	cout << "\t complement : #" << num_of_complement << " : " << complement_total_time << " (" << complement_total_time.total_milliseconds() << " ms)" << endl;
	cout << "\t precisewiden : #" << num_of_precisewiden << " : " << precisewiden_total_time << " (" << precisewiden_total_time.total_milliseconds() << " ms)" << endl;
	cout << "\t coarsewiden : #" << num_of_coarsewiden << " : " << coarsewiden_total_time << " (" << coarsewiden_total_time.total_milliseconds() << " ms)" << endl;
	cout << "\t concat : #" << num_of_concat << " : " << concat_total_time << " (" << concat_total_time.total_milliseconds() << " ms)" << endl;
	cout << "\t pre concat : #" << num_of_pre_concat << " : " << pre_concat_total_time << " (" << pre_concat_total_time.total_milliseconds() << " ms)" << endl;
	cout << "\t const pre concat : #" << num_of_const_pre_concat << " : " << const_pre_concat_total_time << " (" << const_pre_concat_total_time.total_milliseconds() << " ms)" << endl;
	cout << "\t replace : #" << num_of_replace << " : " << replace_total_time << " (" << replace_total_time.total_milliseconds() << " ms)" << endl;
	cout << "\t pre replace : #" << num_of_pre_replace << " : " << pre_replace_total_time << " (" << pre_replace_total_time.total_milliseconds() << " ms)" << endl;
	cout << "\t vlab_restrict : #" << number_of_vlab_restrict << " : " << vlab_restrict_total_time << " (" << vlab_restrict_total_time.total_milliseconds() << " ms)" << endl;
	cout << "\t pre vlab_restrict : #" << number_of_pre_vlab_restrict << " : " << pre_vlab_restrict_total_time << " (" << pre_vlab_restrict_total_time.total_milliseconds() << " ms)" << endl;
	cout << "\t addslahses : #" << number_of_addslashes << " : " << addslashes_total_time << " (" << addslashes_total_time.total_milliseconds() << " ms)" << endl;
	cout << "\t pre addslahses : #" << number_of_pre_addslashes << " : " << pre_addslashes_total_time << " (" << pre_addslashes_total_time.total_milliseconds() << " ms)" << endl;
	cout << "\t htmlspecialchars : #" << number_of_htmlspecialchars << " : " << htmlspecialchars_total_time << " (" << htmlspecialchars_total_time.total_milliseconds() << " ms)" << endl;
	cout << "\t pre htmlspecialchars : #" << number_of_pre_htmlspecialchars << " : " << pre_htmlspecialchars_total_time << " (" << pre_htmlspecialchars_total_time.total_milliseconds() << " ms)" << endl;
	cout << "\t stripslashes : #" << number_of_stripslashes << " : " << stripslashes_total_time << " (" << stripslashes_total_time.total_milliseconds() << " ms)" << endl;
	cout << "\t pre stripslashes : #" << number_of_pre_stripslashes << " : " << pre_stripslashes_total_time << " (" << pre_stripslashes_total_time.total_milliseconds() << " ms)" << endl;
	cout << "\t mysql_escape_string : #" << number_of_mysql_escape_string << " : " << mysql_escape_string_total_time << " (" << mysql_escape_string_total_time.total_milliseconds() << " ms)" << endl;
	cout << "\t pre mysql_escape_string : #" << number_of_pre_mysql_escape_string << " : " << pre_mysql_escape_string_total_time << " (" << pre_mysql_escape_string_total_time.total_milliseconds() << " ms)" << endl;
	cout << "\t to_uppercase : #" << number_of_to_uppercase << " : " << to_uppercase_total_time << " (" << to_uppercase_total_time.total_milliseconds() << " ms)" << endl;
	cout << "\t pre to_uppercase : #" << number_of_pre_to_uppercase << " : " << pre_to_uppercase_total_time << " (" << pre_to_uppercase_total_time.total_milliseconds() << " ms)" << endl;
	cout << "\t to_lowercase : #" << number_of_to_lowercase << " : " << to_lowercase_total_time << " (" << to_lowercase_total_time.total_milliseconds() << " ms)" << endl;
	cout << "\t pre to_lowercase : #" << number_of_pre_to_lowercase << " : " << pre_to_lowercase_total_time << " (" << pre_to_lowercase_total_time.total_milliseconds() << " ms)" << endl;
	cout << "\t trim_spaces : #" << number_of_trim_spaces << " : " << trim_spaces_total_time << " (" << trim_spaces_total_time.total_milliseconds() << " ms)" << endl;
	cout << "\t pre trim_spaces : #" << number_of_pre_trim_spaces << " : " << pre_trim_spaces_total_time << " (" << pre_trim_spaces_total_time.total_milliseconds() << " ms)" << endl;
	cout << "\t trim_spaces_left : #" << number_of_trim_spaces_left << " : " << trim_spaces_left_total_time << " (" << trim_spaces_left_total_time.total_milliseconds() << " ms)" << endl;
	cout << "\t pre trim_spaces_left : #" << number_of_pre_trim_spaces_left << " : " << pre_trim_left_spaces_total_time << " (" << pre_trim_left_spaces_total_time.total_milliseconds() << " ms)" << endl;
	cout << "\t trim_spaces_rigth : #" << number_of_trim_spaces_right << " : " << trim_spaces_right_total_time << " (" << trim_spaces_right_total_time.total_milliseconds() << " ms)" << endl;
	cout << "\t pre trim_spaces_rigth : #" << number_of_pre_trim_spaces_right<< " : " << pre_trim_spaces_right_total_time << " (" << pre_trim_spaces_right_total_time.total_milliseconds() << " ms)" << endl;
	cout << "\t trim_set : #" << number_of_trim_set << " : " << trim_set_total_time << " (" << trim_set_total_time.total_milliseconds() << " ms)" << endl;
	cout << "\t pre trim_set : #" << number_of_pre_trim_set << " : " << pre_trim_set_total_time << " (" << pre_trim_set_total_time.total_milliseconds() << " ms)" << endl;
}

