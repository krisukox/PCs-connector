#include <boost/asio.hpp>
#include <memory>
#include <utility>
#include "CursorGuard.hpp"
#include "internal_types/Point.hpp"

namespace commons
{
class IApp
{
public:
    virtual ~IApp() = default;
    IApp(std::shared_ptr<commons::CursorGuard>&&);

    virtual void connect(const boost::asio::ip::address&) = 0;
    void setContactPoints(const std::pair<internal_types::Point, internal_types::Point>&, const internal_types::Point&);

protected:
    std::shared_ptr<CursorGuard> cursorGuard;
};
} // namespace commons
