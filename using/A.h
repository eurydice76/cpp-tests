#include <vector>

class A {

public:

    using vecint = std::vector<int>;

    const vecint& vint(){return _vint;}

    // const vecdouble& vdouble(){return _vdouble;} --> Will not compile because vecdouble is defined in private area


private:

    using vecdouble = std::vector<double>;

    vecint _vint;
    vecdouble _vdouble;
};
