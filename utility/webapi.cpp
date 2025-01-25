#include "webapi.hpp"
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/range/algorithm_ext.hpp>

namespace utility
{
    WebAPI::WebAPI(std::string host_, int port_, std::string target_)
        : host(host_), port(port_), target(target_)
    {}

    std::string WebAPI::get(std::string body)
    {
        namespace beast = boost::beast;
        namespace http = beast::http;
        namespace net = boost::asio;
        using tcp = net::ip::tcp;

        net::io_context ioc;
        tcp::resolver resolver(ioc);
        tcp::socket socket{ ioc };
        auto const results = resolver.resolve(host, std::to_string(port));
        boost::asio::connect(socket, results.begin(), results.end());
        http::request<http::string_body> req;
        req.method(http::verb::get);
        req.target(target);
        req.set(http::field::host, host + ":" + std::to_string(port));
        req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
        req.set(http::field::content_type, "application/json");
        req.body() = body;
        req.prepare_payload();
        http::write(socket, req);
        boost::beast::flat_buffer buffer;
        http::response<http::string_body> res;
        http::read(socket, buffer, res);
        boost::system::error_code ec;
        socket.shutdown(tcp::socket::shutdown_both, ec);
        if (ec && ec != boost::system::errc::not_connected)
        {
            throw boost::system::system_error{ ec };
        }
        return res.body();
    }

    json11::Json WebAPI::call(json11::Json &input)
    {
        std::string err;
        return json11::Json::parse(get(input.dump()), err);
    }

    json11::Json WebAPI::call(json11::Json &&input)
    {
        std::string err;
        return json11::Json::parse(get(input.dump()), err);
    }
}
