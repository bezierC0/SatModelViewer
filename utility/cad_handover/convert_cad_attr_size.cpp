#include "convert_cad_attr_size.hpp"

#include <cctype>
#include <vector>

#include <boost/range/algorithm/count.hpp>


namespace
{
    using size_type = std::string::size_type;


    bool starts_with_inch_num_and_split(const std::string& src_str, std::string& dst_head, std::string& dst_trail, bool& dst_is_frac)
    {
        const std::string safe_src_str(src_str);
        bool is_starts_with_inch_num = (!safe_src_str.empty() && std::isdigit(safe_src_str.at(0)));

        if (is_starts_with_inch_num)
        {
            bool is_found_connector = false;
            bool is_found_vinculum = false;
            bool is_fractional = false;
            size_type connector_posn = 0;
            size_type inch_num_len = 0;
            for (const auto& safe_src_chr : safe_src_str)
            {
                if (std::isdigit(safe_src_chr))
                {
                    if (is_found_connector)
                    {
                        is_found_connector = false;
                        ++inch_num_len;
                    }
                    else if (is_found_vinculum)
                    {
                        is_found_vinculum = false;
                        is_fractional = true;
                        ++inch_num_len;
                    }
                    ++inch_num_len;
                }
                else if (safe_src_chr == '-')
                {
                    if (!is_found_connector && !is_found_vinculum && !is_fractional && (connector_posn == 0))
                    {
                        is_found_connector = true;
                        connector_posn = inch_num_len;
                    }
                    else
                    {
                        break;
                    }
                }
                else if (safe_src_chr == '/')
                {
                    if (!is_found_connector && !is_found_vinculum && !is_fractional)
                    {
                        is_found_vinculum = true;
                    }
                    else
                    {
                        break;
                    }
                }
                else
                {
                    break;
                }
            }
            if (!is_fractional && (connector_posn > 0))
            {
                inch_num_len = connector_posn;
            }
            dst_head.assign(safe_src_str, 0, inch_num_len);
            dst_trail.assign(safe_src_str, inch_num_len);
            dst_is_frac = is_fractional;
        }

        return is_starts_with_inch_num;
    }

    bool starts_with_real_num_and_split(const std::string& src_str, std::string& dst_head, std::string& dst_trail, bool& dst_is_frac)
    {
        const std::string safe_src_str(src_str);
        bool is_starts_with_real_num = (!safe_src_str.empty() && std::isdigit(safe_src_str.at(0)));

        if (is_starts_with_real_num)
        {
            bool is_int_part = true;
            bool is_found_point = false;
            bool is_fractional = false;
            size_type real_num_len = 0;
            for (const auto& safe_src_chr : safe_src_str)
            {
                if (std::isdigit(safe_src_chr))
                {
                    if (is_found_point)
                    {
                        is_found_point = false;
                        is_fractional = true;
                        ++real_num_len;
                    }
                    ++real_num_len;
                }
                else if (safe_src_chr == '.')
                {
                    if (is_int_part)
                    {
                        is_int_part = false;
                        is_found_point = true;
                    }
                    else
                    {
                        break;
                    }
                }
                else
                {
                    break;
                }
            }
            dst_head.assign(safe_src_str, 0, real_num_len);
            dst_trail.assign(safe_src_str, real_num_len);
            dst_is_frac = is_fractional;
        }

        return is_starts_with_real_num;
    }

    bool starts_with_int_num_and_split(const std::string& src_str, std::string& dst_head, std::string& dst_trail)
    {
        const std::string safe_src_str(src_str);
        bool is_starts_with_int_num = (!safe_src_str.empty() && std::isdigit(safe_src_str.at(0)));

        if (is_starts_with_int_num)
        {
            size_type int_num_len = 0;
            for (const auto& safe_src_chr : safe_src_str)
            {
                if (std::isdigit(safe_src_chr))
                {
                    ++int_num_len;
                }
                else
                {
                    break;
                }
            }
            dst_head.assign(safe_src_str, 0, int_num_len);
            dst_trail.assign(safe_src_str, int_num_len);
        }

        return is_starts_with_int_num;
    }

    bool starts_with_keys_and_split(const std::string& src_str, const std::vector<std::string>& keys, std::string& dst_head, std::string& dst_trail)
    {
        const std::string safe_src_str(src_str);
        bool is_starts_with = false;

        for (const auto& key_str : keys)
        {
            if (safe_src_str.find(key_str) == 0)
            {
                is_starts_with = true;
                dst_head.assign(safe_src_str, 0, key_str.size());
                dst_trail.assign(safe_src_str, key_str.size());
                break;
            }
        }

        return is_starts_with;
    }

    bool find_keys_and_split(const std::string& src_str, const std::vector<std::string>& keys, std::string& dst_head, std::string& dst_trail)
    {
        const std::string safe_src_str(src_str);
        bool is_find_key = false;

        for (const auto& key_str : keys)
        {
            const auto key_posn = safe_src_str.find(key_str);
            if (key_posn != std::string::npos)
            {
                is_find_key = true;
                dst_head.assign(safe_src_str, 0, (key_posn + key_str.size()));
                dst_trail.assign(safe_src_str, (key_posn + key_str.size()));
                break;
            }
        }

        return is_find_key;
    }

    boost::optional<std::string> round_diameter_string(const std::string& src, const unsigned int dst_flac_digit)
    {
        boost::optional<std::string> dst;

        // src（diameter）は std::to_string(double val) で出力され、小数点以下の末尾の「0」が削除されている前提
        const auto src_point_count = boost::count(src, '.');
        std::string dst_rvs;
        if (src_point_count == 0)
        {
            if (!src.empty())
            {
                if (dst_flac_digit > 0)
                {
                    dst_rvs.assign(std::string(dst_flac_digit, '0') + ".");
                }
                const auto src_rbegin = src.crbegin();
                const auto src_rend = src.crend();
                const auto src_it_last = src_rend - 1;
                for (auto src_it = src_rbegin; src_it != src_rend; ++src_it)
                {
                    if ((std::isdigit(*src_it)) ||
                        ((*src_it == '-') && (src_it != src_rbegin) && (src_it == src_it_last)))
                    {
                        dst_rvs.push_back(*src_it);
                    }
                    else
                    {
                        dst_rvs.clear();
                        break;
                    }
                }
            }
        }
        else if (src_point_count == 1)
        {
            const auto src_flac_digit = src.size() - (src.find('.') + 1);
            if (dst_flac_digit >= src_flac_digit)
            {
                if (src_flac_digit > 0)
                {
                    dst_rvs.assign(std::string(dst_flac_digit - src_flac_digit, '0'));
                    const auto src_rbegin = src.crbegin();
                    const auto src_rend = src.crend();
                    const auto src_it_last = src_rend - 1;
                    for (auto src_it = src_rbegin; src_it != src_rend; ++src_it)
                    {
                        if ((std::isdigit(*src_it)) ||
                            ((*src_it == '.') && (src_it != src_it_last) && (std::isdigit(*(src_it + 1)))) ||
                            ((*src_it == '-') && (src_it == src_it_last)))
                        {
                            dst_rvs.push_back(*src_it);
                        }
                        else
                        {
                            dst_rvs.clear();
                            break;
                        }
                    }
                }
            }
            else
            {
                const auto src_rbegin = src.crbegin() + (src_flac_digit - (dst_flac_digit + 1));
                const auto src_rend = src.crend();
                const auto src_it_last = src_rend - 1;
                bool is_carry = false;
                for (auto src_it = src_rbegin; src_it != src_rend; ++src_it)
                {
                    if (std::isdigit(*src_it))
                    {
                        unsigned int src_it_dec = *src_it - '0';
                        if (src_it == src_rbegin)
                        {
                            is_carry = (src_it_dec >= 5);
                        }
                        else
                        {
                            if (is_carry)
                            {
                                is_carry = (++src_it_dec == 10);
                            }
                            dst_rvs.push_back((src_it_dec % 10) + '0');

                            if ((src_it == src_it_last) && (is_carry))
                            {
                                dst_rvs.push_back('1');
                            }
                        }
                    }
                    else if ((*src_it == '.') && (src_it != src_it_last) && (std::isdigit(*(src_it + 1))))
                    {
                        if (dst_flac_digit > 0)
                        {
                            dst_rvs.push_back(*src_it);
                        }
                    }
                    else if ((*src_it == '-') && (src_it == src_it_last))
                    {
                        if (is_carry)
                        {
                            dst_rvs.push_back('1');
                        }
                        dst_rvs.push_back(*src_it);
                    }
                    else
                    {
                        dst_rvs.clear();
                        break;
                    }
                }
            }
        }

        if (!dst_rvs.empty())
        {
            dst.emplace(std::string(dst_rvs.crbegin(), dst_rvs.crend()));
        }

        return dst;
    }
}


namespace utility
{
    ConvCadAttrSizeRslt convert_cad_attr_size_to_size_pitch(const std::string& attr_size)
    {
        ConvCadAttrSizeRslt conv_rslt;

        if (!attr_size.empty())
        {
            std::string head;
            std::string trail;
            bool is_frac = false;

            if (starts_with_keys_and_split(attr_size, {"M"}, head, trail))
            {
                std::string size(head);
                if (starts_with_real_num_and_split(trail, head, trail, is_frac))
                {
                    size.append(head);
                    conv_rslt.size.emplace(size);

                    if (starts_with_keys_and_split(trail, {"x", "X", u8"×", "P"}, head, trail) ||
                        find_keys_and_split(trail, {"p=", "P="}, head, trail))
                    {
                        if (starts_with_real_num_and_split(trail, head, trail, is_frac))
                        {
                            conv_rslt.pitch.emplace(head);
                        }
                    }
                }
            }
            else if (starts_with_keys_and_split(attr_size, {u8"Ø", u8"φ"}, head, trail))
            {
                std::string size(head);
                if (starts_with_real_num_and_split(trail, head, trail, is_frac))
                {
                    size.append(head);
                    conv_rslt.size.emplace(size);
                }
            }
            else if (starts_with_keys_and_split(attr_size, {"#"}, head, trail))
            {
                std::string size(head);
                if (starts_with_int_num_and_split(trail, head, trail))
                {
                    size.append(head);
                    conv_rslt.size.emplace(size);

                    if (starts_with_keys_and_split(trail, {"-"}, head, trail))
                    {
                        if (starts_with_int_num_and_split(trail, head, trail))
                        {
                            conv_rslt.pitch.emplace(head);
                        }
                    }
                }
            }
            else if (starts_with_real_num_and_split(attr_size, head, trail, is_frac) && (is_frac || trail.empty()))
            {
                conv_rslt.size.emplace(head);
            }
            else if (starts_with_inch_num_and_split(attr_size, head, trail, is_frac))
            {
                std::string size(head);
                if (trail.empty())
                {
                    conv_rslt.size.emplace(size);
                }
                else if (starts_with_keys_and_split(trail, {"-"}, head, trail))
                {
                    if (starts_with_int_num_and_split(trail, head, trail))
                    {
                        conv_rslt.size.emplace(size);
                        conv_rslt.pitch.emplace(head);
                    }
                }
                else if (is_frac)
                {
                    conv_rslt.size.emplace(size);
                }
            }
        }

        return conv_rslt;
    }

    boost::optional<std::string> convert_cad_attr_size_to_diameter(const std::string& attr_size, const unsigned int dia_flac_digit)
    {
        boost::optional<std::string> diameter;

        const auto rnd_dia = round_diameter_string(attr_size, dia_flac_digit);
        if (rnd_dia.has_value())
        {
            auto dia_val = u8"Ø" + rnd_dia.value();
            if (dia_flac_digit > 0)
            {
                auto erase_posn = dia_val.find_last_not_of('0');
                if (dia_val.at(erase_posn) != '.')
                {
                    ++erase_posn;
                }
                dia_val.erase(erase_posn);
            }
            diameter.emplace(dia_val);
        }

        return diameter;
    }
}
