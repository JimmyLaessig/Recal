export module Coral.Factory;

namespace Coral
{

namespace Factory
{

export
template<typename T>
void destroy(T* t)
{
    delete t;
}

export
template<typename T, typename ...Args>
T* create(Args... args)
{
    T* t = new T;

    if (!t->init(args...))
    {
        destroy(t);
        return nullptr;
    }

    return t;
}

} // namespace Factory

} // namespace Coral