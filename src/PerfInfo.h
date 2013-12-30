/*
 * PerfInfo.h
 *
 *  Created on: Aug 4, 2013
 *      Author: muath
 */

#ifndef PERFINFO_H_
#define PERFINFO_H_

#include <string>
#include <sstream>
#include <cstdlib>
#include <sys/time.h>

#include <boost/date_time/posix_time/posix_time.hpp>

class PerfInfo {
public:
	PerfInfo();
	virtual ~PerfInfo();
	std::string getInfo() const;
	static void printUsage();
	static void printMemoryUsage();
	void reset();
	static long currentTimeMillis();
	static boost::posix_time::ptime current_time();
    static void addGraphInfo(std::string dotName, int numOfNodes, int numOfEdges);

    boost::posix_time::time_duration validation_patch_extraction;
    boost::posix_time::time_duration length_patch_extraction;
    boost::posix_time::time_duration sanitization_patch_extraction;
    boost::posix_time::time_duration forward_analysis_time;
    boost::posix_time::time_duration backward_analysis_time;

    int num_of_forward_analysis;
    int num_of_backward_analysis;

    long sinkRunningTime;
	int stringLength;

	int numOfConcat;
	int numOfUnion;
	int numOfReplace;

	int concatTime;
	int unionTime;
	int replaceTime;

	int numOfPreConcat;
	int numOfPreReplace;

	int preconcatTime;
	int prereplaceTime;

	int numOfIntersect;
	long intersectTime;
	int numOfPreciseWiden;
	long preciseWidenTime;
	int numOfCoarseWiden;
	long coarseWidenTime;
	int numOfClosure;
	long closureTime;
	int numOfComplement;
	long complementTime;
	int numOfConstPreConcat;
	long constpreconcatTime;
	long forwardTime;
	long backwardTime;
	long validation_phase_time;
	long multiTime;
	long generateExampleTime;
	int numOfGenerateExample;
};

#endif /* PERFINFO_H_ */
