#pragma once

#include "MatlabDataArray.hpp"
#include "MatlabEngine.hpp"
#include "mex.h"

#include "geometrycentral/numerical/linear_algebra_types.h"

using namespace geometrycentral;

typedef Eigen::SparseMatrix<double, Eigen::ColMajor,
                            std::make_signed<mwIndex>::type>
    MatlabSparse;
