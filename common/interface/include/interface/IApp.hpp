#include <utility>
#include "internal_types/Point.hpp"

class IApp
{
public:
    virtual ~IApp() = default;

    virtual void start(int, char* []) = 0;
    virtual void setContactPoints(
        const std::pair<internal_types::Point, internal_types::Point>&,
        const internal_types::Point&) = 0;
};
