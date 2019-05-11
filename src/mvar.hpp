#ifndef MVAR_MVAR_HPP
#define MVAR_MVAR_HPP

#include <condition_variable>
#include <mutex>
#include <optional>

namespace mvar
{

    template <typename T>
    class MVar
    {
    public:
        using Self = MVar<T>;

        MVar() = default;
        explicit MVar(T &&v) : v(v)
        {
        }

        MVar(const Self &other) = delete;
        Self &operator=(const Self &other) = delete;

        void put(T &&v)
        {
            {
                std::unique_lock<std::mutex> l(m);
                putCondition.wait(l, [this] { return !this->v; });
                this->v = v;
            }

            takeCondition.notify_all();
        }
        
         bool tryPut(T &&v) {
            {
                std::unique_lock<std::mutex> l(m);

                if (this->v) {
                    return false;
                }

                this->v = v;
                return true;
            }

            takeCondition.notify_all();
        }

        T take()
        {
            std::optional<T> r;

            {
                std::unique_lock<std::mutex> l(m);
                takeCondition.wait(l, [this] { return this->v; });
                r = std::move(*v);
                v.reset();
            }

            putCondition.notify_all();

            return *r;
        }
        
        std::optional<T> tryTake() {
             std::optional<T> r;
            
             {
                std::unique_lock<std::mutex> l(m);
                r = std::move(*v);
                v.reset();
            }
            
            return r;
        }

        const T &read()
        {
            std::unique_lock<std::mutex> l(m);
            takeCondition.wait(l, [this] { return this->v; });

            return *v;
        }

        bool isEmpty()
        {
            std::unique_lock<std::mutex> l(m);
            return !v;
        }
        
        T swapMVar(T &&v) {
            auto r = take();
            put(std::move(v));
            return r;
        }

    private:
        std::optional<T> v;
        std::mutex m;
        std::condition_variable takeCondition;
        std::condition_variable putCondition;
    };

    template <>
    class MVar<void>
    {
    public:
        using Self = MVar<void>;

        MVar() = default;
        MVar(const Self &other) = delete;
        Self &operator=(const Self &other) = delete;

        void put()
        {
            {
                std::unique_lock<std::mutex> l(m);
                putCondition.wait(l, [this] { return !this->v; });
                this->v = true;
            }

            takeCondition.notify_all();
        }
        
        bool tryPut() {
            {
                std::unique_lock<std::mutex> l(m);
                if (this->v) {
                    return false;
                }

                this->v = true;
                return true;
            }

            takeCondition.notify_all();
        }

        void take()
        {
            {
                std::unique_lock<std::mutex> l(m);
                takeCondition.wait(l, [this] { return this->v; });
                this->v = false;
            }

            putCondition.notify_all();
        }
        
        bool tryTake() {
                std::unique_lock<std::mutex> l(m);
                auto r = v;
                
                if (r) { v = false; }
                
                return r;
            
        }

        void read()
        {
            std::unique_lock<std::mutex> l(m);
            takeCondition.wait(l, [this] { return this->v; });
        }

        bool isEmpty()
        {
            std::unique_lock<std::mutex> l(m);
            return !v;
        }

    private:
        bool v{false};
        std::mutex m;
        std::condition_variable takeCondition;
        std::condition_variable putCondition;
    };
}

#endif // MVAR_MVAR_HPP
