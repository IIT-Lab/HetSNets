//
// Created by lee on 17-11-10.
//

#ifndef HETSNETS_POLARIZATION_H
#define HETSNETS_POLARIZATION_H

#include <Eigen/Dense>

using namespace Eigen;

void SimplePolarizationChannel(double x,double r,double r1, Matrix2cd& H);
void polarizationChannel(double cpr,double xpr,double RCorrCoefficients, double TCorrCoefficients, double cpc, double apc, Matrix2cd& matrixDepolarization);

#endif //HETSNETS_POLARIZATION_H
