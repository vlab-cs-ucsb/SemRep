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

class PerfInfo {
public:
	PerfInfo();
	virtual ~PerfInfo();
	std::string getInfo() const;
	static void printUsage();
	static void printMemoryUsage();
	void reset();
	static long currentTimeMillis();
    static void addGraphInfo(std::string dotName, int numOfNodes, int numOfEdges);
	long sinkRunningTime;
	int stringLength;

	int numOfConcat;
	int numOfUnion;
	int numOfReplace;

	long concatTime;
	long unionTime;
	long replaceTime;

	int numOfPreConcat;
	int numOfPreReplace;

	long preconcatTime;
	long prereplaceTime;

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
