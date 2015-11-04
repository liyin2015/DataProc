#ifndef FEATURE_SCALLINH_HPP_
#define FEATURE_SCALLINH_HPP_
#include <iostream>
#include "comm.hpp"
#include "matrix.hpp"

/*function [X_norm, mu, sigma] = featureNormalize(X)  
%FEATURENORMALIZE Normalizes the features in X   
%   FEATURENORMALIZE(X) returns a normalized version of X where  
%   the mean value of each feature is 0 and the standard deviation  
%   is 1. This is often a good preprocessing step to do when  
%   working with learning algorithms.  */
/*x=mXn,                      */
  
//You need to set these values correctly 

double** featureNormalize(double** x, int m, int n);
//bool bNorm(Matrix x);
Matrix featureScale(Matrix x,int dimension);
#endif