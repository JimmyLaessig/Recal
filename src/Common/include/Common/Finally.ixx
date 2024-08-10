module;

#include <Common/System.h>

export module Finally;

import Common.NonCopyable;

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
		if (mAction)
		{
			mAction();
		}
	}

private:
	Action mAction;
};

} // export namespace Common
