#include "include/pca.hpp"






void hints(void){

  MatrixXd M;
  
  // compute the column wize mean of a data matrix
  VectorXd mean =  M.colwise().sum() / (double) M.rows(); 
  
  // some hints to center some data (with the exterior product)
  std::cout << VectorXd::Ones(M.rows())*mean.transpose() << std::endl;

  // compute some eigen vectors
  SelfAdjointEigenSolver<MatrixXd> eigensolver(M);
  std::cout << "\neigenvectors of M \n" << eigensolver.eigenvectors().rowwise().reverse() << std::endl;
  std::cout << "\neigenvalues of M : " << eigensolver.eigenvalues().colwise().reverse().transpose() << std::endl;
  
  // extract some rows or columns from a matrix
  std::cout << M.leftCols(3) << std::endl; 
  std::cout << M.topRows(3) << std::endl; 
	
}

MatrixXd centerData(MatrixXd moyM, MatrixXd M){
  MatrixXd center = M - moyM;
  return center;
}

VectorXd meanData(MatrixXd M){
  VectorXd mean =  M.colwise().sum() / (double) M.rows(); 
  return mean;
}

MatrixXd normalize(MatrixXd M){
  MatrixXd normM;
  int nbInd = M.rows();
  VectorXd sigma = M.cwiseAbs().colwise().sum() / nbInd;
  std::cout << "sigma\n" << sigma << std::endl; 
  
  //std::cout << "sigma\n" << sigma << std::endl; 
  MatrixXd sigmaM = VectorXd::Ones(M.rows()) * sigma.transpose();
  normM = M.cwiseQuotient(sigmaM);
  return normM;
}

MatrixXd covariance(MatrixXd M1, MatrixXd M2){
  MatrixXd covM;
     covM = (M2.transpose() * M2) / (double(M1.rows() -1));
  return covM;
}

void pca(std::string database)
{
  if(database == " "){
    std::cerr << "usage : put correct data in database.mat" << std::endl;
    exit(0);
  }
  
  // load the data
  MatrixXd A;
  loadMatrix(A,database);
  std::cout << "A\n" << A << std::endl;

  // mean of the data
  VectorXd vmoyA;
  vmoyA = meanData(A);
  std::cout << "vmoyA\n" << vmoyA << std::endl; 

  // center the data
  MatrixXd centerA;
  MatrixXd meanA;
  meanA = VectorXd::Ones(A.rows())*vmoyA.transpose();
  centerA = centerData(meanA,A);  
  std::cout << "centerA\n" << centerA << std::endl; 

  // normalize the data
  MatrixXd normA = normalize(centerA);
  std::cout << "normA\n" << normA << std::endl; 

  // build the covariance matrix 
  MatrixXd covA;
  covA = covariance(centerA, normA);
  std::cout << "covA\n" << covA << std::endl;

  // compute the eigen vectors
  SelfAdjointEigenSolver<MatrixXd> eigensolver(covA);
  MatrixXd T = eigensolver.eigenvectors().rowwise().reverse();

  std::cout << "\neigenvectors of covA \n" << T << std::endl;
  std::cout << "\neigenvalues of covA : " << T.transpose() << std::endl;


  // keep only n dimensions
  std::cout << "\n 3 best eigenvectors rows of covA \n" << T.transpose().topRows(3) << std::endl; 
  
  // project the data
  MatrixXd At = T.transpose().topRows(3) * normA.transpose();
  std::cout << "\n At \n" << At.transpose() << std::endl; 
  
  
  // project a new vector (remind to center and scale this vector)
  
}
