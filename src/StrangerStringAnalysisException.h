//
//  StrangerStringAnalysisException.h
//  stranger
//
//  Created by Muath Alkhalaf on 8/26/13.
//  Copyright (c) 2013 Muath Alkhalaf. All rights reserved.
//

#ifndef stranger_StrangerStringAnalysisException_h
#define stranger_StrangerStringAnalysisException_h


#include <string>

class StrangerStringAnalysisException {
public:
    StrangerStringAnalysisException ( const string err){ error = err;};
    const std::string what() const { return error;};

private:
    std::string error;
    
};

#endif
