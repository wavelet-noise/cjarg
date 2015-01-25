#ifndef AGENT_H
#define AGENT_H
#include <string>
#include <memory>

class Caster
{
private:
        static int next_magic_number()
        {
            static int magic(0);
            return magic++;
        }

        template <typename T_>
        static int magic_number_for()
        {
            static int result(next_magic_number());
            return result;
        }

        struct CasterValueBase
        {
            int magic_number;

            CasterValueBase(const int m) :
                magic_number(m)
            {
            }

            virtual ~CasterValueBase()
            {
            }
        };

        template <typename T_>
        struct CasterValue :
            CasterValueBase
        {
            T_ value;

            CasterValue(const T_ & v) :
                CasterValueBase(magic_number_for<T_>()),
                value(v)
            {
            }
        };

        std::shared_ptr<CasterValueBase> _value;

    public:
        template <typename T_>
        Caster(const T_ &t) :
            _value(new CasterValue<T_>(t))
        {
        }

        template <typename T_>
        T_ &as()
        {
            if (magic_number_for<T_>() != _value->magic_number)
                throw;
            return std::static_pointer_cast<CasterValue<T_>>(_value)->value;
        }

        template <typename T_>
        bool is()
        {
            if (magic_number_for<T_>() != _value->magic_number)
                return false;
            return true;
        }
};

class Agent
{
public:
    Agent();
    ~Agent();

    //virtual const std::string type = "";
};

#endif // AGENT_H
