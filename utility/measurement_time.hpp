#pragma once

#include <chrono>
#include <boost/log/trivial.hpp>

namespace utility
{
    /**
    \brief 時間測定
    *      基本単位：ミリ秒
    */
    class MeasurementTime
    {
    protected:
        static auto now()
        {
            return std::chrono::steady_clock::now();
        }

        std::chrono::steady_clock::time_point time_start;

    public:
        MeasurementTime()
        : time_start(now())
        {
        }

        virtual ~MeasurementTime() = default;

        [[nodiscard]] auto duration() const
        {
            auto result = now() - time_start;
            return std::chrono::duration_cast<std::chrono::milliseconds>(result); // 精度ミリ秒
        }
    };

    // 寿命とともにログファイルへ所要時間を出力
    class ElapsedLogger: public MeasurementTime
    {
    private:
        const char* const name_;

        using fn_result_t = void (ElapsedLogger::*)(const std::chrono::milliseconds&) const;
        fn_result_t       fn_result_;

        // functions for make-result(on destruct)
        void result_millisec(const std::chrono::milliseconds& ms) const
        {
            BOOST_LOG_TRIVIAL(info) << "Finish " << name_ << " (" << ms.count() << "ms)";
        }

        void result_sec(const std::chrono::milliseconds& ms) const
        {
            auto elapsed = std::chrono::duration<float>(ms); // 秒 (1.258s)
            BOOST_LOG_TRIVIAL(info) << "Finish " << name_ << " (" << elapsed.count() << "s)";
        }

        void result_sw(const std::chrono::milliseconds& ms) const
        {
            if ( ms.count() >= 10000 )
            {
                result_sec(ms);
            }
            else
            {
                result_millisec(ms);
            }
        }

    public:
        enum elapsedunit
        {
            millisec = 0, // ミリ秒表記 "1258ms"
            sec,          // 秒表記     "1.258s"
            autoswitch,   // 10000ms(10秒)以上で秒表記になります
        };
        ElapsedLogger(const char* blockname = "", elapsedunit eu = autoswitch) // 定義に際して __FUNCTION__ 使用可能
        : MeasurementTime()
        , name_(blockname)
        {
            static fn_result_t fn[] = {
                &ElapsedLogger::result_millisec,
                &ElapsedLogger::result_sec,
                &ElapsedLogger::result_sw, 
            };
            fn_result_ = fn[eu];

            BOOST_LOG_TRIVIAL(info) << "Start  " << name_;
        }

        ~ElapsedLogger() override
        {
            (this->*fn_result_)(duration());
        }

        // ElapsedLoggerは使い捨てを想定しているため、コピーや代入を禁止する
        ElapsedLogger(const ElapsedLogger&) = delete;
        ElapsedLogger(ElapsedLogger&&) = delete;
        ElapsedLogger& operator=(const ElapsedLogger&) = delete; 
        ElapsedLogger& operator=(ElapsedLogger&&) = delete;
    };
}
