#pragma once

#include <vector>
#include <list>
#include <boost/range/algorithm.hpp>

namespace utility
{
    /**
     \brief 集合の要素をグループ分けする
     \param range 集合
     \param is_member 所属判定関数
     \return いくつかの部分集合(グループ)の集合
     \remark is_member の関数形式は bool (const Range& group, const Range::value_type element);
     */
    template<typename Range, typename BinaryFunc>
    std::vector<Range> group_by(const Range& range, BinaryFunc is_member)
    {
        // 集合(グループ)の集合
        std::vector<Range> groups;

        // 未処理リスト
        std::list<typename Range::value_type> rest = { std::begin(range), std::end(range) };

        while(!rest.empty())
        {
            // グループ化されたものが在ったフラグ
            bool grouped = false;

            // 未処理要素を順番に所属判定する
            for (auto it = rest.begin(); it != rest.end(); /**/)
            {
                // 所属判定がtrueになるグループが在るか探す
                const auto group = boost::find_if(groups, [&it, &is_member](const auto& g) { return is_member(g, *it); });
                if (group != groups.end())
                {
                    // 在れば、そのグループに追加する
                    group->emplace_back(*it);
                    // 未処理リストから除去する
                    it = rest.erase(it);
                    // 追加したフラグを立てる
                    grouped = true;
                }
                else
                {
                    ++it;
                }
            }

            // 1つもグループ化されなかったら
            if (!grouped)
            {
                // 未処理リストの先頭要素を取り出して、新しいグループを作る
                const auto top = rest.front();
                groups.emplace_back(Range{ top });
                rest.pop_front();
            }

            // グループの状態が変わったら残要素を再試験
        }
        return groups;
    }
}
