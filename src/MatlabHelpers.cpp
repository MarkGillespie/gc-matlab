#include "MatlabHelpers.h"

// https://stackoverflow.com/a/49957392
Eigen::Map<MatlabSparse> matlab_to_eigen_sparse(const mxArray* mat) {
    mxAssert(mxGetClassID(mat) == mxDOUBLE_CLASS,
             "Type of the input matrix isn't double");
    mwSize m  = mxGetM(mat);
    mwSize n  = mxGetN(mat);
    mwSize nz = mxGetNzmax(mat);
    /*Theoretically fails in very very large matrices*/
    mxAssert(nz <= std::numeric_limits<std::make_signed<mwIndex>::type>::max(),
             "Unsupported Data size.");
    double* pr = mxGetPr(mat);
    MatlabSparse::StorageIndex* ir =
        reinterpret_cast<MatlabSparse::StorageIndex*>(mxGetIr(mat));
    MatlabSparse::StorageIndex* jc =
        reinterpret_cast<MatlabSparse::StorageIndex*>(mxGetJc(mat));
    Eigen::Map<MatlabSparse> result(m, n, nz, jc, ir, pr);
    return result;
}

// https://www.mathworks.com/help/matlab/matlab_external/pass-sparse-arrays-to-matlab-1.html
matlab::data::SparseArray<double>
eigen_to_matlab_sparse(const SparseMatrix<double>& M) {
    // Connect to named shared MATLAB session started as:
    // matlab -r "matlab.engine.shareEngine('myMatlabEngine')"
    matlab::engine::String session(u"myMatlabEngine");
    std::unique_ptr<matlab::engine::MATLABEngine> matlabPtr =
        matlab::engine::connectMATLAB(session);

    // Define the data for the sparse array
    std::vector<double> data = {3.5, 12.98, 21.76};
    std::vector<size_t> rows = {0, 0, 1};
    std::vector<size_t> cols = {0, 4, 8};
    size_t nnz               = 3;

    // Create buffers for the data
    // clang-format off
    matlab::data::ArrayFactory factory;
    matlab::data::buffer_ptr_t<double> data_p = factory.createBuffer<double>(nnz);
    matlab::data::buffer_ptr_t<size_t> rows_p = factory.createBuffer<size_t>(nnz);
    matlab::data::buffer_ptr_t<size_t> cols_p = factory.createBuffer<size_t>(nnz);
    // clang-format on

    // Write data into the buffers
    double* dataPtr = data_p.get();
    size_t* rowsPtr = rows_p.get();
    size_t* colsPtr = cols_p.get();
    std::for_each(data.begin(), data.end(),
                  [&](const double& e) { *(dataPtr++) = e; });
    std::for_each(rows.begin(), rows.end(),
                  [&](const size_t& e) { *(rowsPtr++) = e; });
    std::for_each(cols.begin(), cols.end(),
                  [&](const size_t& e) { *(colsPtr++) = e; });

    // Use the buffers to create the sparse array
    matlab::data::SparseArray<double> arr = factory.createSparseArray<double>(
        {2, 9}, nnz, std::move(data_p), std::move(rows_p), std::move(cols_p));

    return arr;
}
