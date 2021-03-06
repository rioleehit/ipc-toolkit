#pragma once

#include <Eigen/Core>
#include <Eigen/Sparse>

#include <ipc/collision_constraint.hpp>
#include <ipc/friction/friction_constraint.hpp>

namespace ipc {

// C1 clamping
template <typename T>
inline T f0_SF(const T& x_squared, const double& epsv_times_h)
{
    double epsv_times_h_squared = epsv_times_h * epsv_times_h;
    if (x_squared >= epsv_times_h_squared) {
        return sqrt(x_squared);
    }
    return x_squared * (-sqrt(x_squared) / 3.0 + epsv_times_h)
        / (epsv_times_h_squared)
        + epsv_times_h / 3.0;
}

/// Derivative of f0_SF divided by the relative norm
template <typename T>
inline T f1_SF_div_relative_displacement_norm(
    const T& x_squared, const double& epsv_times_h)
{
    double epsv_times_h_squared = epsv_times_h * epsv_times_h;
    if (x_squared >= epsv_times_h_squared) {
        return 1 / sqrt(x_squared);
    }
    return (-sqrt(x_squared) + 2.0 * epsv_times_h) / epsv_times_h_squared;
}

template <typename T> inline T f2_SF(const T&, const double& epsv_times_h)
{
    // same for x_squared >= epsv_times_h * epsv_times_h for C1 clamped friction
    return T(-1 / (epsv_times_h * epsv_times_h));
}

void construct_friction_constraint_set(
    const Eigen::MatrixXd& V,
    const Eigen::MatrixXi& E,
    const Eigen::MatrixXi& F,
    const Constraints& contact_constraint_set,
    double dhat,
    double barrier_stiffness,
    double mu,
    FrictionConstraints& friction_constraint_set);

/// @brief Compute the friction potential between to positions.
///
/// @param V0 Vertex positions at start of time-step (or newton-solve) (rowwise)
/// @param V1 Current vertex positions (rowwise)
/// @param E  Edge vertex indicies
/// @param F  Face vertex indicies (empty in 2D)
/// @param friction_constraint_set
/// @param closest_points
/// @param tangent_bases
/// @param normal_force_magnitudes
/// @param epsv_times_h
/// @param mu Coefficient of friction for all contacts
template <typename T>
T compute_friction_potential(
    const Eigen::MatrixXd& V0,
    const Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>& V1,
    const Eigen::MatrixXi& E,
    const Eigen::MatrixXi& F,
    const FrictionConstraints& friction_constraint_set,
    double epsv_times_h);

Eigen::VectorXd compute_friction_potential_gradient(
    const Eigen::MatrixXd& V0,
    const Eigen::MatrixXd& V1,
    const Eigen::MatrixXi& E,
    const Eigen::MatrixXi& F,
    const FrictionConstraints& friction_constraint_set,
    double epsv_times_h);

Eigen::SparseMatrix<double> compute_friction_potential_hessian(
    const Eigen::MatrixXd& V0,
    const Eigen::MatrixXd& V1,
    const Eigen::MatrixXi& E,
    const Eigen::MatrixXi& F,
    const FrictionConstraints& friction_constraint_set,
    double epsv_times_h,
    bool project_to_psd = true);

} // namespace ipc

#include "friction.tpp"
