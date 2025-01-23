#ifndef COMMON_FINALLY_HPP
#define COMMON_FINALLY_HPP

#include <Common/NonCopyable.hpp>

export namespace Common
{

/// Utility class that invokes the passed action on destruction
template<typename Action>
struct COMMON_API Finally : public Common::NonCopyable
{
public:

	Finally(Action&& action)
		: mAction(action)
	{
	}

	~Finally()
	{
		mAction();
	}

private:
	Action mAction;
};

} // namespace Common

#endif // !COMMON_FINALLY_HPP
