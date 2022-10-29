#include "MatrixVector/MatrixSquare.h"
#include "Utilities/MessageHandler.h"
#include "MatrixVector/IMatrixUtils.h"


MatrixSquare::MatrixSquare(int dim) : m_matrix(dim*dim), m_inverse(dim*dim), m_pivot(dim), m_indexer(dim, dim), m_dim(dim), m_dirty(true)
{
	CheckDimensions(*this);
	std::fill(m_matrix.begin(), m_matrix.end(), 0.0);
}


double MatrixSquare::operator() (int row, int col)  const
{
	CheckRowCol(*this, row, col);
	return m_matrix[row*m_dim + col];
}

double& MatrixSquare::operator() (int row, int col)
{
	CheckRowCol(*this, row, col);
	m_dirty = true;
	return m_matrix[row*m_dim + col];
}


int MatrixSquare::GetDimension() const
{
	return m_dim;
}


// Found on the internet

////////////////////////////////////////////////////////////////////////////////
// File: crout_pivot.c                                                        //
// Routines:                                                                  //
//    Crout_LU_Decomposition_with_Pivoting                                    //
//    Crout_LU_with_Pivoting_Solve                                            //
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  int Crout_LU_Decomposition_with_Pivoting(double *A, int pivot[], int n)   //
//                                                                            //
//  Description:                                                              //
//     This routine uses Crout's method to decompose a row interchanged       //
//     version of the n x n matrix A into a lower triangular matrix L and a   //
//     unit upper triangular matrix U such that A = LU.                       //
//     The matrices L and U replace the matrix A so that the original matrix  //
//     A is destroyed.                                                        //
//     Note!  In Crout's method the diagonal elements of U are 1 and are      //
//            not stored.                                                     //
//     Note!  The determinant of A is the product of the diagonal elements    //
//            of L.  (det A = det L * det U = det L).                         //
//     The LU decomposition is convenient when one needs to solve the linear  //
//     equation Ax = B for the vector x while the matrix A is fixed and the   //
//     vector B is varied.  The routine for solving the linear system Ax = B  //
//     after performing the LU decomposition for A is                         //
//                      Crout_LU_with_Pivoting_Solve.                         //
//     (see below).                                                           //
//                                                                            //
//     The Crout method with partial pivoting is: Determine the pivot row and //
//     interchange the current row with the pivot row, then assuming that     //
//     row k is the current row, k = 0, ..., n - 1 evaluate in order the      //
//     the following pair of expressions                                      //
//       L[i][k] = (A[i][k] - (L[i][0]*U[0][k] + . + L[i][k-1]*U[k-1][k]))    //
//                                 for i = k, ... , n-1,                      //
//       U[k][j] = A[k][j] - (L[k][0]*U[0][j] + ... + L[k][k-1]*U[k-1][j])    //
//                                                                  / L[k][k] //
//                                      for j = k+1, ... , n-1.               //
//       The matrix U forms the upper triangular matrix, and the matrix L     //
//       forms the lower triangular matrix.                                   //
//                                                                            //
//  Arguments:                                                                //
//     double *A       Pointer to the first element of the matrix A[n][n].    //
//     int    pivot[]  The i-th element is the pivot row interchanged with    //
//                     row i.                                                 //
//     int     n       The number of rows or columns of the matrix A.         //
//                                                                            //
//  Return Values:                                                            //
//     0  Success                                                             //
//    -1  Failure - The matrix A is singular.                                 //
//                                                                            //
//  Example:                                                                  //
//     #define N                                                              //
//     double A[N][N];                                                        //
//     int    pivot[N];                                                       //
//                                                                            //
//     (your code to intialize the matrix A)                                  //
//                                                                            //
//     err = Crout_LU_Decomposition_with_Pivoting(&A[0][0], pivot, N);        //
//     if (err < 0) printf(" Matrix A is singular\n");                        //
//     else { printf(" The LU decomposition of A is \n");                     //
//           ...                                                              //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //

static int Crout_LU_Decomposition_with_Pivoting(double *A, int pivot[], int n)
{
	int i, j, k;
	double *p_k, *p_row, *p_col = nullptr;
	double max;

	//         For each row and column, k = 0, ..., n-1,

	for (k = 0, p_k = A; k < n; p_k += n, k++) {

		//            find the pivot row

		pivot[k] = k;
		max = std::abs(*(p_k + k));
		for (j = k + 1, p_row = p_k + n; j < n; j++, p_row += n) {
			if (max < std::abs(*(p_row + k))) {
				max = std::abs(*(p_row + k));
				pivot[k] = j;
				p_col = p_row;
			}
		}

		//     and if the pivot row differs from the current row, then
		//     interchange the two rows.

		if (pivot[k] != k)
			for (j = 0; j < n; j++) {
				max = *(p_k + j);
				*(p_k + j) = *(p_col + j);
				*(p_col + j) = max;
			}

		//                and if the matrix is singular, return error

		if (*(p_k + k) == 0.0) return -1;

		//      otherwise find the upper triangular matrix elements for row k. 

		for (j = k + 1; j < n; j++) {
			*(p_k + j) /= *(p_k + k);
		}

		//            update remaining matrix

		for (i = k + 1, p_row = p_k + n; i < n; p_row += n, i++)
			for (j = k + 1; j < n; j++)
				*(p_row + j) -= *(p_row + k) * *(p_k + j);

	}
	return 0;
}


////////////////////////////////////////////////////////////////////////////////
//  int Crout_LU_with_Pivoting_Solve(double *LU, double B[], int pivot[],     //
//                                                        double x[], int n)  //
//                                                                            //
//  Description:                                                              //
//     This routine uses Crout's method to solve the linear equation Ax = B.  //
//     This routine is called after the matrix A has been decomposed into a   //
//     product of a lower triangular matrix L and a unit upper triangular     //
//     matrix U without pivoting.  The argument LU is a pointer to the matrix //
//     the superdiagonal part of which is U and the subdiagonal together with //
//     the diagonal part is L. (The diagonal part of U is 1 and is not        //
//     stored.)   The matrix A = LU.                                          //
//     The solution proceeds by solving the linear equation Ly = B for y and  //
//     subsequently solving the linear equation Ux = y for x.                 //
//                                                                            //
//  Arguments:                                                                //
//     double *LU      Pointer to the first element of the matrix whose       //
//                     elements form the lower and upper triangular matrix    //
//                     factors of A.                                          //
//     double *B       Pointer to the column vector, (n x 1) matrix, B.       //
//     int    pivot[]  The i-th element is the pivot row interchanged with    //
//                     row i.                                                 //
//     double *x       Solution to the equation Ax = B.                       //
//     int     n       The number of rows or columns of the matrix LU.        //
//                                                                            //
//  Return Values:                                                            //
//     0  Success                                                             //
//    -1  Failure - The matrix A is singular.                                 //
//                                                                            //
//  Example:                                                                  //
//     #define N                                                              //
//     double A[N][N], B[N], x[N];                                            //
//     int    pivot[N];                                                       //
//                                                                            //
//     (your code to create matrix A and column vector B)                     //
//     err = Crout_LU_Decomposition_with_Pivoting(&A[0][0], pivot, N);        //
//     if (err < 0) printf(" Matrix A is singular\n");                        //
//     else {                                                                 //
//        err = Crout_LU_with_Pivoting_Solve(&A[0][0], B, pivot, x, n);       //
//        if (err < 0) printf(" Matrix A is singular\n");                     //
//        else printf(" The solution is \n");                                 //
//           ...                                                              //
//     }                                                                      //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
static int Crout_LU_with_Pivoting_Solve(double *LU, double B[], const int pivot[],
	double x[], int n)
{
	int i, k;
	double *p_k;
	double dum;

	//         Solve the linear equation Lx = B for x, where L is a lower
	//         triangular matrix.                                      

	for (k = 0, p_k = LU; k < n; p_k += n, k++) {
		if (pivot[k] != k) { dum = B[k]; B[k] = B[pivot[k]]; B[pivot[k]] = dum; }
		x[k] = B[k];
		for (i = 0; i < k; i++) x[k] -= x[i] * *(p_k + i);
		x[k] /= *(p_k + k);
	}

	//         Solve the linear equation Ux = y, where y is the solution
	//         obtained above of Lx = B and U is an upper triangular matrix.
	//         The diagonal part of the upper triangular part of the matrix is
	//         assumed to be 1.0.

	for (k = n - 1, p_k = LU + n * (n - 1); k >= 0; k--, p_k -= n) {
		if (pivot[k] != k) { dum = B[k]; B[k] = B[pivot[k]]; B[pivot[k]] = dum; }
		for (i = k + 1; i < n; i++) x[k] -= x[i] * *(p_k + i);
		if (*(p_k + k) == 0.0) return -1;
	}

	return 0;
}

bool MatrixSquare::Solve(std::span<const double> rhs, std::span< double> sol)
{
	MessageHandler::Assert(rhs.size() == m_dim);
	MessageHandler::Assert(sol.size() == m_dim);
	int status = 0;
	if (m_dirty)
	{
		std::copy(m_matrix.begin(), m_matrix.end(), m_inverse.begin());
		status = Crout_LU_Decomposition_with_Pivoting(m_inverse.data(), m_pivot.data(), m_dim);
		if (status != 0)
		{
			return false;
		}
		m_dirty = false;
	}
	// Make copy of rhs, it will get reordered
	std::vector<double> b(rhs.begin(), rhs.end());
	status = Crout_LU_with_Pivoting_Solve(m_inverse.data(), b.data(), m_pivot.data(), sol.data(), m_dim);
	if (status != 0)
	{
		return false;
	}
	return true;
}


const IIndexer<int>& MatrixSquare::GetIndexer()
{
	return m_indexer;
}


