#pragma once

namespace utility
{
    /**
    \brief ID カウンタ
    */
    class IDCounter
    {
    public:
        IDCounter();
        IDCounter(int start_id);
        ~IDCounter() = default;

        int get();
    private:
        int id;
    };
}
