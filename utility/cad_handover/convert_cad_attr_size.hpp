#pragma once

#include <string>

#include <boost/optional.hpp>


namespace utility
{
    struct ConvCadAttrSizeRslt
    {
        boost::optional<std::string> size;
        boost::optional<std::string> pitch;
    };


    ConvCadAttrSizeRslt convert_cad_attr_size_to_size_pitch(const std::string& attr_size);
    boost::optional<std::string> convert_cad_attr_size_to_diameter(const std::string& attr_size, const unsigned int dia_flac_digit);
}
