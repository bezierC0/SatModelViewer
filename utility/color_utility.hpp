#pragma once

#include <vector>
#include <functional>

#include <rgbcolor.hxx>
#include <api.hxx>

class FACE;

namespace utility
{
    constexpr double default_color_tolerance = 1e-4;

    struct ReadColorInfo
    {
        rgb_color color;
        bool found;
        outcome res;
    };

    struct ReadColorsInfo
    {
        std::vector<rgb_color> colors;
        bool has_no_colored_part;
        bool has_read_error;
    };

    ReadColorInfo read_color(FACE* face);

    // function aliases
    using ColorEqualsFunction = std::function<bool(const rgb_color&, const rgb_color&)>;
    using ColorMatchingFunction = std::function<bool(const rgb_color&)>;

    ColorEqualsFunction create_color_equals_function(double tolerance = default_color_tolerance);
    ColorMatchingFunction create_color_matching_function(const rgb_color& matching_color, const ColorEqualsFunction& color_equals_function);

    bool less_than_color(const rgb_color& lhs, const rgb_color& rhs);
    int convert_into_code(const rgb_color& c);

    void remove_duplication_of_colors(std::vector<rgb_color>& v, const ColorEqualsFunction& equals);

    /**
    * @brief convert acis rgb_color into user defined color struct
    */
    template <typename T>
    T convert_color(const rgb_color& input)
    {
        return T(input.red(), input.green(), input.blue());
    }
}
