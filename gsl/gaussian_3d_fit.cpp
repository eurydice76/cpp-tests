#include <functional>
#include <iostream>
#include <random>
#include <stdexcept>

#include <Eigen/Dense>

#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_multifit_nlinear.h>

class Gaussian {

public:

    using residuals_type = std::function<int(Eigen::ArrayXd&)>;

    using jacobian_type = std::function<int(Eigen::ArrayXXd&)>;

    Gaussian(double amplitude, double background, const Eigen::Vector3d& center, const Eigen::Matrix3d& inverse_sigma);

    Eigen::ArrayXd evaluate(const Eigen::ArrayXd& x, const Eigen::ArrayXd& y, const Eigen::ArrayXd& z);

    Eigen::ArrayXXd jacobian(const Eigen::ArrayXd& x, const Eigen::ArrayXd& y, const Eigen::ArrayXd& z);

    double background() const;
    
    double amplitude() const;

    const Eigen::Vector3d& center() const;

    const Eigen::Matrix3d& inverseSigma() const;

    void setResidualsFunc(residuals_type residual_func);

    void setJacobianFunc(jacobian_type jacobian_func);

    void setParameters(const gsl_vector* parameters);

    friend int gsl_residuals_wrapper(const gsl_vector* params, void* data, gsl_vector* residuals);

    friend int gsl_jacobian_wrapper(const gsl_vector* params, void* data, gsl_matrix* residuals);

    void print(std::ostream& os) const;

private:

    double _amplitude;

    double _background;

    Eigen::Vector3d _center;

    Eigen::Matrix3d _inverse_sigma;

    residuals_type _residuals_func;

    jacobian_type _jacobian_func;
};

std::ostream& operator<<(std::ostream& os, const Gaussian& gaussian)
{
    gaussian.print(os);
    return os;
}

int gsl_residuals_wrapper(const gsl_vector* gsl_params, void* gsl_data, gsl_vector* gsl_residuals)
{
    Gaussian* g = reinterpret_cast<Gaussian*>(gsl_data);
    if (g->_residuals_func == nullptr) {
        std::runtime_error("the residual function is not defined");    
    }

    g->setParameters(gsl_params);

    Eigen::ArrayXd residuals;
    int result = g->_residuals_func(residuals);

    for (auto i=0; i<residuals.size();++i) {
        gsl_vector_set(gsl_residuals,i,residuals(i));
    }    

    return result;
}

int gsl_jacobian_wrapper(const gsl_vector* gsl_params, void* gsl_data, gsl_matrix* gsl_jacobian)
{
    Gaussian* g = reinterpret_cast<Gaussian*>(gsl_data);
    if (g->_residuals_func == nullptr) {
        std::runtime_error("the residual function is not defined");    
    }

    g->setParameters(gsl_params);

    Eigen::ArrayXXd jacobian;
    int result = g->_jacobian_func(jacobian);

    for (auto i=0; i<jacobian.rows();++i) {
        for (auto j=0; j<jacobian.cols();++j) {
            gsl_matrix_set(gsl_jacobian,i,j,jacobian(i,j));
        }
    }

    return result;
}


Gaussian::Gaussian(double amplitude, double background, const Eigen::Vector3d& center, const Eigen::Matrix3d& inverse_sigma)
: _amplitude(amplitude),
  _background(background),
  _center(center),
  _inverse_sigma(inverse_sigma),
  _residuals_func(nullptr),
  _jacobian_func(nullptr)
{
}

void Gaussian::print(std::ostream& os) const
{
    os<<"background = "<<_background<<std::endl;
    os<<"amplitude = "<<_amplitude<<std::endl;
    os<<"center = "<<_center.transpose()<<std::endl;
    os<<"inverse sigma = "<<std::endl;
    os<<_inverse_sigma<<std::endl;
}

void Gaussian::setResidualsFunc(residuals_type residuals_func)
{
    _residuals_func = residuals_func;
}

void Gaussian::setJacobianFunc(jacobian_type jacobian_func)
{
    _jacobian_func = jacobian_func;
}

void Gaussian::setParameters(const gsl_vector* parameters)
{
    _background = gsl_vector_get(parameters,0);
    _amplitude = gsl_vector_get(parameters,1);
    _center(0) = gsl_vector_get(parameters,2);
    _center(1) = gsl_vector_get(parameters,3);
    _center(2) = gsl_vector_get(parameters,4);
    _inverse_sigma(0,0) = gsl_vector_get(parameters,5);
    _inverse_sigma(0,1) = _inverse_sigma(1,0) = gsl_vector_get(parameters,6);
    _inverse_sigma(0,2) = _inverse_sigma(2,0) = gsl_vector_get(parameters,7);
    _inverse_sigma(1,1) = gsl_vector_get(parameters,8);
    _inverse_sigma(1,2) = _inverse_sigma(2,1) = gsl_vector_get(parameters,9);
    _inverse_sigma(2,2) = gsl_vector_get(parameters,10);
}

Eigen::ArrayXd Gaussian::evaluate(const Eigen::ArrayXd& x, const Eigen::ArrayXd& y, const Eigen::ArrayXd& z)
{
    const size_t n_points = x.size();
    if (y.size() != n_points || z.size() != n_points) {
        std::runtime_error("Invalid number of x,y,z points.");
    }

    auto dx = x - _center(0);
    auto dy = y - _center(1);
    auto dz = z - _center(2);

    auto arg = -0.5*(    _inverse_sigma(0,0)*dx*dx +
                         _inverse_sigma(1,1)*dy*dy +
                         _inverse_sigma(2,2)*dz*dz +
                     2.0*_inverse_sigma(0,1)*dx*dy +
                     2.0*_inverse_sigma(0,2)*dx*dz +
                     2.0*_inverse_sigma(1,2)*dy*dz);
    Eigen::ArrayXd result(n_points);    
    result = _background + _amplitude*arg.exp();
    
    return result;
}

Eigen::ArrayXXd Gaussian::jacobian(const Eigen::ArrayXd& x, const Eigen::ArrayXd& y, const Eigen::ArrayXd& z)
{    
    const size_t n_points = x.size();
    if (y.size() != n_points || z.size() != n_points) {
        std::runtime_error("Invalid number of x,y,z points.");
    }

    auto dx = x - _center(0);
    auto dy = y - _center(1);
    auto dz = z - _center(2);

    auto arg = -0.5*(    _inverse_sigma(0,0)*dx*dx +
                         _inverse_sigma(1,1)*dy*dy +
                         _inverse_sigma(2,2)*dz*dz +
                     2.0*_inverse_sigma(0,1)*dx*dy +
                     2.0*_inverse_sigma(0,2)*dx*dz +
                     2.0*_inverse_sigma(1,2)*dy*dz);

    auto e = arg.exp();

    Eigen::ArrayXXd jac(n_points,11);

    // The variables of the gaussian 3D are: background (b), amplitude (a), center (c), 6 upper elements of inverse_sigma (Dxx,Dyy,Dzz,Dxy,Dxz,Dyz)    
    // g(x,y,z) = b + a*exp(-0.5*[Dxx*(x-cx)^2 + Dyy*(y-cy)^2 + Dzz*(z-cz)^2 + 2*Dxy*(x-cx)*(y-cy) + 2*Dxz*(x-cx)*(z-cz) + 2*Dyz*(y-cy)*(z-cz)

    // derivative wrt b
    jac.col(0) = 1;

    // derivative wrt a
    jac.col(1) = e;

    // derivative wrt cx
    jac.col(2) = _amplitude*e*(_inverse_sigma(0,0)*dx + _inverse_sigma(0,1)*dy + _inverse_sigma(0,2)*dz);

    // derivative wrt cy
    jac.col(3) = _amplitude*e*(_inverse_sigma(1,0)*dx + _inverse_sigma(1,1)*dy + _inverse_sigma(1,2)*dz);
    
    // derivative wrt cz
    jac.col(4) = _amplitude*e*(_inverse_sigma(2,0)*dx + _inverse_sigma(2,1)*dy + _inverse_sigma(2,2)*dz);
    
    // derivative wrt Dxx
    jac.col(5) = -0.5*_amplitude*e*dx*dx;

    // derivative wrt Dxy
    jac.col(6) = -_amplitude*e*dx*dy;

    // derivative wrt Dxz
    jac.col(7) = -_amplitude*e*dx*dz;

    // derivative wrt Dyy
    jac.col(8) = -0.5*_amplitude*e*dy*dy;

    // derivative wrt Dyz
    jac.col(9) = -_amplitude*e*dy*dz;

    // derivative wrt Dzz
    jac.col(10) = -0.5*_amplitude*e*dz*dz;

    return jac;
}

double Gaussian::amplitude() const
{
    return _amplitude;
}

double Gaussian::background() const
{
    return _background;
}

const Eigen::Vector3d& Gaussian::center() const
{
    return _center;
}

const Eigen::Matrix3d& Gaussian::inverseSigma() const
{
    return _inverse_sigma;
}

int main() {

    const double amplitude = 1000.0;
    const double background = 600.0;

    const Eigen::Vector3d center(50,60,70);

    Eigen::Matrix3d B;
    B << 2, 0, -1,
         1, 2,  0,
         0,-1,  3;

    const Eigen::Matrix3d sigma = B*B.transpose();

    Gaussian gaussian(amplitude,background,center,sigma.inverse());   
    
    const int n_points(15);
    const int n_points3(n_points*n_points*n_points);

    // Generate the grid of points onto which the gaussian will be evaluated

    Eigen::ArrayXd x(n_points3), y(n_points3), z(n_points3); 

    size_t idx = 0;
    for (auto i = 0; i < n_points; ++i) {
        for (auto j = 0; j < n_points; ++j) {
            for (auto k = 0; k < n_points; ++k) {
                x(idx) = center(0) + 2*(i-n_points/2);
                std::cout<<" -- "<<idx<<" -- "<<center.transpose()<<" -- "<<2*(i-n_points/2)<<" -- "<<x(idx)<<std::endl;
                y(idx) = center(1) + 2*(j-n_points/2);
                z(idx) = center(2) + 2*(k-n_points/2);
                ++idx;
            }
        }
    }

    // Generate the intensity (background + gaussian + noise)

    std::default_random_engine generator;
    std::normal_distribution<double> gaussian_noise(0.0,60.0);

    Eigen::ArrayXd exp_intensity = gaussian.evaluate(x,y,z);
    for (auto i = 0; i < n_points3; ++i) {
        exp_intensity(i) += gaussian_noise(generator);
    }
    
    auto f = [&](Eigen::ArrayXd& residuals) -> int {
        residuals = gaussian.evaluate(x,y,z);
        residuals -= exp_intensity;
        return 0;
    };
    gaussian.setResidualsFunc(f);
    
    auto df = [&](Eigen::ArrayXXd& jacobian) -> int {
        jacobian = gaussian.jacobian(x,y,z);
        return 0;
    };
    gaussian.setJacobianFunc(df);

    // Setup the gsl minimizer

    gsl_multifit_nlinear_workspace* gsl_workspace;
    gsl_multifit_nlinear_fdf gsl_fdf;
    gsl_multifit_nlinear_parameters gsl_fdf_params;
    gsl_matrix* gsl_jacobian;
    gsl_matrix* gsl_covariance;
    int gsl_status;
    int gsl_info;
    gsl_vector* gsl_x;
    gsl_vector* gsl_weight;

    gsl_fdf_params = gsl_multifit_nlinear_default_parameters();

    // allocate initial parameters values and weights
    gsl_x = gsl_vector_alloc(11);
    gsl_covariance = gsl_matrix_alloc(11,11);
    gsl_weight = gsl_vector_alloc(n_points3);
    // Set the weights to 1.0
    for (auto i = 0; i<n_points3; ++i) {
        gsl_vector_set(gsl_weight,i,1.0);
    }

    gsl_workspace = gsl_multifit_nlinear_alloc(gsl_multifit_nlinear_trust, &gsl_fdf_params, n_points3, 11);

    gsl_fdf.f = &gsl_residuals_wrapper;
    gsl_fdf.df = &gsl_jacobian_wrapper; // If nullptr is set instead of a valid function, numeric derivative will be used instead.
    gsl_fdf.p = 11;
    gsl_fdf.n = n_points3;
    gsl_fdf.params = &gaussian;
    gsl_fdf.fvv = nullptr;

    gaussian.setParameters(gsl_x);

    std::cout<<gaussian<<std::endl;

    gsl_multifit_nlinear_winit(gsl_x, gsl_weight, &gsl_fdf, gsl_workspace);
    gsl_status = gsl_multifit_nlinear_driver(100, 1.0e-7, 1.0e-7, 1.0e-7, NULL, NULL, &gsl_info, gsl_workspace);

    std::cout<<gaussian<<std::endl;

    return 0;

}
