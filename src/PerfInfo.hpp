/*
 * PerfInfo.hpp
 *
 *  Created on: Feb 11, 2014
 *      Author: baki
 */

#ifndef PERFINFO_HPP_
#define PERFINFO_HPP_

#include <string>
#include <boost/date_time/posix_time/posix_time.hpp>

class PerfInfo {
public:
	PerfInfo();
	virtual ~PerfInfo();

	 void reset();

	 boost::posix_time::ptime current_time();

	 void print_validation_extraction_info();
	 void print_sanitization_extraction_info();
	 void print_operations_info();

	 void calculate_total_validation_extraction_time();
	 void calculate_total_sanitization_length_extraction_time();
	 void calculate_total_sanitization_extraction_time();

//	Validation Extraction Time
	 boost::posix_time::time_duration validation_target_backward_time;
	 boost::posix_time::time_duration validation_reference_backward_time;
	 boost::posix_time::time_duration validation_comparison_time;
	 boost::posix_time::time_duration validation_patch_extraction_total_time;

//    Sanitization Extraction Time
	 boost::posix_time::time_duration sanitization_target_first_forward_time;
	 boost::posix_time::time_duration sanitization_reference_first_forward_time;
	 boost::posix_time::time_duration sanitization_length_issue_check_time;
	 boost::posix_time::time_duration sanitization_length_backward_time;
	 boost::posix_time::time_duration sanitization_length_patch_extraction_total_time;
	 boost::posix_time::time_duration sanitization_patch_backward_time;
	 boost::posix_time::time_duration sanitization_comparison_time;
	 boost::posix_time::time_duration sanitization_patch_extraction_total_time;

//    Stranger Automaton Operations

//    Core string operation information
//    concat, union, intersection, closure, replace...

	 boost::posix_time::time_duration intersect_total_time;
	 boost::posix_time::time_duration union_total_time;
	 boost::posix_time::time_duration closure_total_time;
	 boost::posix_time::time_duration complement_total_time;
	 boost::posix_time::time_duration precisewiden_total_time;
	 boost::posix_time::time_duration coarsewiden_total_time;
	 boost::posix_time::time_duration concat_total_time;
	 boost::posix_time::time_duration pre_concat_total_time;
	 boost::posix_time::time_duration const_pre_concat_total_time;
	 boost::posix_time::time_duration replace_total_time;
	 boost::posix_time::time_duration pre_replace_total_time;

	 boost::posix_time::time_duration performance_time;


	 unsigned int num_of_intersect;
	 unsigned int num_of_union;
	 unsigned int num_of_closure;
	 unsigned int num_of_complement;
	 unsigned int num_of_precisewiden;
	 unsigned int num_of_coarsewiden;
	 unsigned int num_of_concat;
	 unsigned int num_of_pre_concat;
	 unsigned int num_of_const_pre_concat;
	 unsigned int num_of_replace;
	 unsigned int num_of_pre_replace;


//    Composed string operations
	 boost::posix_time::time_duration vlab_restrict_total_time;
	 boost::posix_time::time_duration pre_vlab_restrict_total_time;
	 boost::posix_time::time_duration addslashes_total_time;
	 boost::posix_time::time_duration pre_addslashes_total_time;
	 boost::posix_time::time_duration htmlspecialchars_total_time;
	 boost::posix_time::time_duration pre_htmlspecialchars_total_time;
	 boost::posix_time::time_duration stripslashes_total_time;
	 boost::posix_time::time_duration pre_stripslashes_total_time;
	 boost::posix_time::time_duration mysql_escape_string_total_time;
	 boost::posix_time::time_duration pre_mysql_escape_string_total_time;
	 boost::posix_time::time_duration to_uppercase_total_time;
	 boost::posix_time::time_duration pre_to_uppercase_total_time;
	 boost::posix_time::time_duration to_lowercase_total_time;
	 boost::posix_time::time_duration pre_to_lowercase_total_time;
	 boost::posix_time::time_duration trim_spaces_total_time;
	 boost::posix_time::time_duration pre_trim_spaces_total_time;
	 boost::posix_time::time_duration trim_spaces_left_total_time;
	 boost::posix_time::time_duration pre_trim_spaces_left_total_time;
	 boost::posix_time::time_duration trim_spaces_right_total_time;
	 boost::posix_time::time_duration pre_trim_spaces_rigth_total_time;
	 boost::posix_time::time_duration trim_set_total_time;
	 boost::posix_time::time_duration pre_trim_set_total_time;
	 boost::posix_time::time_duration substr_total_time;
	 boost::posix_time::time_duration pre_substr_total_time;

	 unsigned int number_of_vlab_restrict;
	 unsigned int number_of_pre_vlab_restrict;
	 unsigned int number_of_addslashes;
	 unsigned int number_of_pre_addslashes;
	 unsigned int number_of_htmlspecialchars;
	 unsigned int number_of_pre_htmlspecialchars;
	 unsigned int number_of_stripslashes;
	 unsigned int number_of_pre_stripslashes;
	 unsigned int number_of_mysql_escape_string;
	 unsigned int number_of_pre_mysql_escape_string;
	 unsigned int number_of_to_uppercase;
	 unsigned int number_of_pre_to_uppercase;
	 unsigned int number_of_to_lowercase;
	 unsigned int number_of_pre_to_lowercase;
	 unsigned int number_of_trim_spaces;
	 unsigned int number_of_pre_trim_spaces;
	 unsigned int number_of_trim_spaces_left;
	 unsigned int number_of_pre_trim_spaces_left;
	 unsigned int number_of_trim_spaces_rigth;
	 unsigned int number_of_pre_trim_spaces_rigth;
	 unsigned int number_of_trim_set;
	 unsigned int number_of_pre_trim_set;
	 unsigned int number_of_substr;
	 unsigned int number_of_pre_substr;
};


#endif /* PERFINFO_HPP_ */
