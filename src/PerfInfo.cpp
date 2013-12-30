/*
 * PerfInfo.cpp
 *
 *  Created on: Aug 4, 2013
 *      Author: muath
 */

#include "PerfInfo.h"

using namespace std;

PerfInfo::PerfInfo() {


    num_of_forward_analysis = 0;
    num_of_backward_analysis = 0;
//	sinkRunningTime = 0;
//	stringLength = 0;
//
//	numOfConcat = 0;
//	numOfUnion = 0;
//	numOfReplace = 0;
//
//	concatTime = 0;
//	unionTime = 0;
//	replaceTime = 0;
//
//	numOfPreConcat = 0;
//	numOfPreReplace = 0;
//
//	preconcatTime = 0;
//	prereplaceTime = 0;
//
//	numOfIntersect = 0;
//	intersectTime = 0;
//	numOfPreciseWiden = 0;
//	preciseWidenTime = 0;
//	numOfCoarseWiden = 0;
//	coarseWidenTime = 0;
//	numOfClosure = 0;
//	closureTime = 0;
//	numOfComplement = 0;
//	complementTime = 0;
//	numOfConstPreConcat = 0;
//	constpreconcatTime = 0;
//	forwardTime = 0;
//	backwardTime = 0;
//	multiTime = 0;
//	generateExampleTime = 0;
//	numOfGenerateExample = 0;

}

PerfInfo::~PerfInfo() {

}



std::string PerfInfo::getInfo() const {
	std::stringstream sb;

//	sb << "Time elapsed in seconds = "
//			<< (((double) sinkRunningTime) / ((double) 1000)) << "\n";
//
//	sb << "Forward analysis time in seconds = "
//			<< (((double) forwardTime) / ((double) 1000)) << "\n";
//
//	sb << "Backward analysis time in seconds = "
//			<< (((double) backwardTime) / ((double) 1000)) << "\n";
//
//	sb << "Forward multi analysis time in seconds = "
//			<< (((double) multiTime) / ((double) 1000)) << "\n";
//
//	sb << "String length = " << stringLength << "\n";
//
//	sb << "Number of concat = " << numOfConcat << "\n";
//	sb << "Time of concat in seconds = "
//			<< (((double) concatTime) / ((double) 1000)) << "\n";
//
//	sb << "Number of union = " << numOfUnion << "\n";
//	sb << "Time of union in seconds = "
//			<< (((double) unionTime) / ((double) 1000)) << "\n";
//
//	sb << "Number of replace = " << numOfReplace << "\n";
//	sb << "Time of replace in seconds = "
//			<< (((double) replaceTime) / ((double) 1000)) << "\n";
//
//	sb << "Number of preconcat = " << numOfPreConcat << "\n";
//	sb << "Time of preconcat in seconds = "
//			<< (((double) preconcatTime) / ((double) 1000)) << "\n";
//
//	sb << "Number of constPreconcat = " << numOfConstPreConcat << "\n";
//	sb << "Time of constPreconcat in seconds = "
//			<< (((double) constpreconcatTime) / ((double) 1000)) << "\n";
//
//	sb << "Total number of preConcat = "
//			<< (numOfPreConcat + numOfConstPreConcat) << "\n";
//	sb << "Total time of preconcat in seconds = "
//			<< ((((double) preconcatTime) + ((double) constpreconcatTime))
//					/ ((double) 1000)) << "\n";
//
//	sb << "Number of prereplace = " << numOfPreReplace << "\n";
//	sb << "Time of prereplace in seconds = "
//			<< (((double) prereplaceTime) / ((double) 1000)) << "\n";
//
//	sb << "-------------     GRAPHS INFO      -----------" << "\n";
//		for (GraphInfo graphInfo: graphsInfo){
//			sb << graphInfo.toString());
//			sb << "------------------------" <<  "\n");
//		}
	return sb.str();
}

void PerfInfo::printUsage() {
//        OperatingSystemMXBean operatingSystemMXBean = ManagementFactory.getOperatingSystemMXBean();
//        for (Method method : operatingSystemMXBean.getClass().getDeclaredMethods()) {
//            method.setAccessible(true);
//            if (method.getName().startsWith("get") && Modifier.isPublic(method.getModifiers())) {
//                Object value;
//                try {
//                    value = method.invoke(operatingSystemMXBean);
//                } catch (Exception e) {
//                    value = e;
//                }
//                System.out.println(method.getName() + " = " + value);
//            }
//        }
}

void PerfInfo::printMemoryUsage() {
//		 long mem = Runtime.getRuntime().totalMemory() -
//	      Runtime.getRuntime().freeMemory();
//		 System.out.println(
//		      "Memory consumption = " + mem + " bytes");
//		 System.out.println("From MONA: total allocated memory = " +StrangerAutomaton.getAllocatedMemory());
}

void PerfInfo::addGraphInfo(string dotName, int numOfNodes, int numOfEdges){
    
}

void PerfInfo::reset() {

    num_of_forward_analysis = 0;
    num_of_backward_analysis = 0;

    sinkRunningTime = 0;
    stringLength = 0;

	numOfConcat = 0;
	numOfUnion = 0;
	numOfReplace = 0;

	concatTime = 0;
	unionTime = 0;
	replaceTime = 0;

	numOfPreConcat = 0;
	numOfPreReplace = 0;

	preconcatTime = 0;
	prereplaceTime = 0;

	numOfIntersect = 0;
	intersectTime = 0;
	numOfPreciseWiden = 0;
	preciseWidenTime = 0;
	numOfCoarseWiden = 0;
	coarseWidenTime = 0;
	numOfClosure = 0;
	closureTime = 0;
	numOfComplement = 0;
	complementTime = 0;
	numOfConstPreConcat = 0;
	constpreconcatTime = 0;
	forwardTime = 0;
	backwardTime = 0;
	multiTime = 0;
//		 graphsInfo = new LinkedList<GraphInfo>();
}

long PerfInfo::currentTimeMillis(){
		return 0;
}

boost::posix_time::ptime PerfInfo::current_time() {
	return boost::posix_time::microsec_clock::local_time();
}

