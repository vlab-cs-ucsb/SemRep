/*
 * PerfInfo.cpp
 *
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

#include "PerfInfo.hpp"

using namespace std;

PerfInfo::PerfInfo() {
    reset();
}

PerfInfo::~PerfInfo() {

}

void PerfInfo::reset() {

	//	Validation Extraction Time
	 validation_target_backward_time = boost::posix_time::microseconds(0);
	 validation_reference_backward_time = boost::posix_time::microseconds(0);
	 validation_comparison_time = boost::posix_time::microseconds(0);
	 validation_patch_extraction_total_time = boost::posix_time::microseconds(0);

//    Sanitization Extraction Time
	 sanitization_target_first_forward_time = boost::posix_time::microseconds(0);
	 sanitization_reference_first_forward_time = boost::posix_time::microseconds(0);
	 sanitization_length_issue_check_time = boost::posix_time::microseconds(0);
	 sanitization_length_backward_time = boost::posix_time::microseconds(0);
	 sanitization_length_patch_extraction_total_time = boost::posix_time::microseconds(0);
	 sanitization_patch_backward_time = boost::posix_time::microseconds(0);
	 sanitization_comparison_time = boost::posix_time::microseconds(0);
	 sanitization_patch_extraction_total_time = boost::posix_time::microseconds(0);

//    Stranger Automaton Operations

//    Core string operation information
//    concat, union, intersection, closure, replace...

	 intersect_total_time = boost::posix_time::microseconds(0);
	 union_total_time = boost::posix_time::microseconds(0);
	 closure_total_time = boost::posix_time::microseconds(0);
	 complement_total_time = boost::posix_time::microseconds(0);
	 precisewiden_total_time = boost::posix_time::microseconds(0);
	 coarsewiden_total_time = boost::posix_time::microseconds(0);
	 concat_total_time = boost::posix_time::microseconds(0);
	 pre_concat_total_time = boost::posix_time::microseconds(0);
	 const_pre_concat_total_time = boost::posix_time::microseconds(0);
	 replace_total_time = boost::posix_time::microseconds(0);
	 pre_replace_total_time = boost::posix_time::microseconds(0);

	 performance_time = boost::posix_time::microseconds(0);

//    Composed string operations
	 vlab_restrict_total_time = boost::posix_time::microseconds(0);
	 pre_vlab_restrict_total_time = boost::posix_time::microseconds(0);
	 addslashes_total_time = boost::posix_time::microseconds(0);
	 pre_addslashes_total_time = boost::posix_time::microseconds(0);
	 htmlspecialchars_total_time = boost::posix_time::microseconds(0);
	 pre_htmlspecialchars_total_time = boost::posix_time::microseconds(0);
	 stripslashes_total_time = boost::posix_time::microseconds(0);
	 pre_stripslashes_total_time = boost::posix_time::microseconds(0);
	 mysql_escape_string_total_time = boost::posix_time::microseconds(0);
	 pre_mysql_escape_string_total_time = boost::posix_time::microseconds(0);
	 to_uppercase_total_time = boost::posix_time::microseconds(0);
	 pre_to_uppercase_total_time = boost::posix_time::microseconds(0);
	 to_lowercase_total_time = boost::posix_time::microseconds(0);
	 pre_to_lowercase_total_time = boost::posix_time::microseconds(0);
	 trim_spaces_total_time = boost::posix_time::microseconds(0);
	 pre_trim_spaces_total_time = boost::posix_time::microseconds(0);
	 trim_spaces_left_total_time = boost::posix_time::microseconds(0);
	 pre_trim_spaces_left_total_time = boost::posix_time::microseconds(0);
	 trim_spaces_right_total_time = boost::posix_time::microseconds(0);
	 pre_trim_spaces_rigth_total_time = boost::posix_time::microseconds(0);
	 trim_set_total_time = boost::posix_time::microseconds(0);
	 pre_trim_set_total_time = boost::posix_time::microseconds(0);
	 substr_total_time = boost::posix_time::microseconds(0);
	 pre_substr_total_time = boost::posix_time::microseconds(0);


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
	number_of_trim_spaces_rigth = 0;
	number_of_pre_trim_spaces_rigth = 0;
	number_of_trim_set = 0;
	number_of_pre_trim_set = 0;
	number_of_substr = 0;
	number_of_pre_substr = 0;

}


boost::posix_time::ptime PerfInfo::current_time() {
	return boost::posix_time::microsec_clock::local_time();
}

void PerfInfo::calculate_total_validation_extraction_time() {
	validation_patch_extraction_total_time = validation_target_backward_time + validation_reference_backward_time + validation_comparison_time;
}

void PerfInfo::calculate_total_sanitization_length_extraction_time() {
	sanitization_length_patch_extraction_total_time = sanitization_length_issue_check_time + sanitization_length_backward_time;
}

void PerfInfo::calculate_total_sanitization_extraction_time() {
	sanitization_patch_extraction_total_time = sanitization_target_first_forward_time + sanitization_reference_first_forward_time +
			sanitization_patch_backward_time + sanitization_comparison_time;
}

void PerfInfo::print_validation_extraction_info() {
	cout << endl << "\t Validation Patch Analysis Info" << endl;
	cout << "\t time : patchee backward analysis time : " << validation_target_backward_time.total_microseconds() << endl;
	cout << "\t time : patcher backward analysis time : " << validation_reference_backward_time.total_microseconds() << endl;
	cout << "\t time : validation comparison time : " << validation_comparison_time.total_microseconds() << endl;
	cout << "\t time : validation patch extraction total time : " << validation_patch_extraction_total_time.total_microseconds() << endl;

}

void PerfInfo::print_sanitization_extraction_info() {
	cout << endl << "\t Sanitization Patch Analysis Info" << endl;
	cout << "\t time : patchee first forward analysis time : " << sanitization_target_first_forward_time.total_microseconds() << endl;
	cout << "\t time : patcher first forward analysis time : " << sanitization_reference_first_forward_time.total_microseconds() << endl;
	cout << "\t time : length issue check time : " << sanitization_length_issue_check_time.total_microseconds() << endl;
	cout << "\t time : length issue backward time : " << sanitization_length_backward_time.total_microseconds() << endl;
	cout << "\t time : length issue patch extraction total time : " << sanitization_length_patch_extraction_total_time.total_microseconds() << endl;
	cout << "\t time : sanitization backward time : " << sanitization_patch_backward_time.total_microseconds() << endl;
	cout << "\t time : sanitization comparison time : " << sanitization_comparison_time.total_microseconds() << endl;
	cout << "\t time : sanitization patch extraction total time : " << sanitization_patch_extraction_total_time.total_microseconds() << endl;

}

void PerfInfo::print_operations_info() {

	cout << endl <<"\t Stranger Automaton Operations Info" << endl;
	cout << "\t intersection : #" << num_of_intersect << " : " << intersect_total_time.total_microseconds() << endl;
	cout << "\t union : #" << num_of_union << " : " << union_total_time.total_microseconds() << endl;
	cout << "\t closure : #" << num_of_closure << " : " << closure_total_time.total_microseconds() << endl;
	cout << "\t complement : #" << num_of_complement << " : " << complement_total_time.total_microseconds() << endl;
	cout << "\t precisewiden : #" << num_of_precisewiden << " : " << precisewiden_total_time.total_microseconds() << endl;
	cout << "\t coarsewiden : #" << num_of_coarsewiden << " : " << coarsewiden_total_time.total_microseconds() << endl;
	cout << "\t concat : #" << num_of_concat << " : " << concat_total_time.total_microseconds() << endl;
	cout << "\t pre_concat : #" << num_of_pre_concat << " : " << pre_concat_total_time.total_microseconds() << endl;
	cout << "\t const_pre_concat : #" << num_of_const_pre_concat << " : " << const_pre_concat_total_time.total_microseconds() << endl;
	cout << "\t replace : #" << num_of_replace << " : " << replace_total_time.total_microseconds() << endl;
	cout << "\t pre_replace : #" << num_of_pre_replace << " : " << pre_replace_total_time.total_microseconds() << endl;
	cout << "\t vlab_restrict : #" << number_of_vlab_restrict << " : " << vlab_restrict_total_time.total_microseconds() << endl;
	cout << "\t pre_vlab_restrict : #" << number_of_pre_vlab_restrict << " : " << pre_vlab_restrict_total_time.total_microseconds() << endl;
	cout << "\t addslahses : #" << number_of_addslashes << " : " << addslashes_total_time.total_microseconds() << endl;
	cout << "\t pre_addslahses : #" << number_of_pre_addslashes << " : " << pre_addslashes_total_time.total_microseconds() << endl;
	cout << "\t htmlspecialchars : #" << number_of_htmlspecialchars << " : " << htmlspecialchars_total_time.total_microseconds() << endl;
	cout << "\t pre_htmlspecialchars : #" << number_of_pre_htmlspecialchars << " : " << pre_htmlspecialchars_total_time.total_microseconds() << endl;
	cout << "\t stripslashes : #" << number_of_stripslashes << " : " << stripslashes_total_time.total_microseconds() << endl;
	cout << "\t pre_stripslashes : #" << number_of_pre_stripslashes << " : " << pre_stripslashes_total_time.total_microseconds()<< endl;
	cout << "\t mysql_escape_string : #" << number_of_mysql_escape_string << " : " << mysql_escape_string_total_time.total_microseconds() << endl;
	cout << "\t pre_mysql_escape_string : #" << number_of_pre_mysql_escape_string << " : " << pre_mysql_escape_string_total_time .total_microseconds() << endl;
	cout << "\t to_uppercase : #" << number_of_to_uppercase << " : " << to_uppercase_total_time.total_microseconds() << endl;
	cout << "\t pre_to_uppercase : #" << number_of_pre_to_uppercase << " : " << pre_to_uppercase_total_time.total_microseconds() << endl;
	cout << "\t to_lowercase : #" << number_of_to_lowercase << " : " << to_lowercase_total_time.total_microseconds() << endl;
	cout << "\t pre_to_lowercase : #" << number_of_pre_to_lowercase << " : " << pre_to_lowercase_total_time.total_microseconds() << endl;
	cout << "\t trim_spaces : #" << number_of_trim_spaces << " : " << trim_spaces_total_time.total_microseconds() << endl;
	cout << "\t pre_trim_spaces : #" << number_of_pre_trim_spaces << " : " << pre_trim_spaces_total_time.total_microseconds() << endl;
	cout << "\t trim_spaces_left : #" << number_of_trim_spaces_left << " : " << trim_spaces_left_total_time.total_microseconds() << endl;
	cout << "\t pre_trim_spaces_left : #" << number_of_pre_trim_spaces_left << " : " << pre_trim_spaces_left_total_time.total_microseconds() << endl;
	cout << "\t trim_spaces_rigth : #" << number_of_trim_spaces_rigth << " : " << trim_spaces_right_total_time.total_microseconds() << endl;
	cout << "\t pre_trim_spaces_rigth : #" << number_of_pre_trim_spaces_rigth<< " : " << pre_trim_spaces_rigth_total_time.total_microseconds() << endl;
	cout << "\t trim_set : #" << number_of_trim_set << " : " << trim_set_total_time.total_microseconds() << endl;
	cout << "\t pre_trim_set : #" << number_of_pre_trim_set << " : " << pre_trim_set_total_time.total_microseconds() << endl;
	cout << "\t substr : #" << number_of_substr << " : " << substr_total_time.total_microseconds() << endl;
	cout << "\t pre_substr : #" << number_of_pre_substr << " : " << pre_substr_total_time.total_microseconds() << endl;
}

