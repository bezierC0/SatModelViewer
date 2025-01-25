#include "color_utility.hpp"

#include <boost/range/algorithm.hpp>
#include <boost/range/algorithm_ext/erase.hpp>

#include <api.hxx>
#include <face.hxx>
#include <rnd_api.hxx>

namespace utility
{
    ReadColorInfo read_color(FACE* face)
    {
        rgb_color color;
        logical found;
        outcome res;

        API_BEGIN
            res = api_rh_get_entity_rgb(face, color, TRUE, found);
        API_END
;
        return ReadColorInfo{ color, (found != FALSE), res };
    }

    ColorMatchingFunction create_color_matching_function(const rgb_color& matching_color, const ColorEqualsFunction& color_equals_function)
    {
        return [matching_color, color_equals_function](const rgb_color& c)->bool
        {
            return color_equals_function(matching_color, c);
        };
    }

    ColorEqualsFunction create_color_equals_function(double tolerance)
    {
        return [tolerance](const rgb_color& lhs, const rgb_color& rhs)->bool
        {
            // original operator method
            if (lhs == rhs)
            {
                return true;
            }

            // user specified tolerance
            const auto has_same_red = std::fabs(lhs.red() - rhs.red()) < tolerance;
            const auto has_same_green = std::fabs(lhs.green() - rhs.green()) < tolerance;
            const auto has_same_blue = std::fabs(lhs.blue() - rhs.blue()) < tolerance;

            return has_same_red && has_same_green && has_same_blue;
        };
    }

    // color comparator (substitution of operator<)
    bool less_than_color(const rgb_color& lhs, const rgb_color& rhs)
    {
        return convert_into_code(lhs) < convert_into_code(rhs);
    }

    int convert_into_code(const rgb_color& c)
    {
        const auto r = static_cast<unsigned int>(c.red() * 255.0) & 0xFF;
        const auto g = static_cast<unsigned int>(c.green() * 255.0) & 0xFF;
        const auto b = static_cast<unsigned int>(c.blue() * 255.0) & 0xFF;

        return static_cast<int>((r << 16) | (g << 8) | b);
    }

    void remove_duplication_of_colors(std::vector<rgb_color>& v, const ::utility::ColorEqualsFunction& equals)
    {
        boost::sort(v, ::utility::less_than_color);
        boost::erase(v, boost::unique<boost::return_found_end>(v, equals));
    };
}
