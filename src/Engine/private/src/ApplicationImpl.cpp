module;

#include <ranges>
#include <vector>
#include <memory>

module Engine.Application.ApplicationImpl;

import Log20;
import Engine.TypeRegistry;

import Common.SmartPointer;

using namespace Reef;


ApplicationImpl::~ApplicationImpl()
{
	std::ranges::reverse_view rv{ mLayers };

	for (auto& layer : rv)
	{
		layer->deinitialize();
	}
}


bool
ApplicationImpl::run()
{
	if (mIsRunning)
	{
		return false;
	}

	mIsRunning = true;

	bool result = updateLoop();
	
	std::ranges::reverse_view rv { mLayers };

	for (auto& layer : rv)
	{
		layer->start();
	}

	auto runResult = updateLoop();

	for (auto& layer : rv)
	{
		layer->stop();
	}

	mIsRunning = false;

	return runResult;
}


bool
ApplicationImpl::updateLoop()
{
	std::ranges::reverse_view rv{ mLayers };

	while (!mShouldStop)
	{
		

		/*for (auto& layer : rv)
		{
			layer->beginFrame();
		}

		for (auto& layer : rv)
		{
			layer->update();
		}

		for (auto& layer : rv)
		{
			layer->endFrame();
		}

		for (auto& layer : rv)
		{
			layer->render();
		}*/
	}

	return true;
}


bool
ApplicationImpl::stop()
{
	if (!mIsRunning)
	{
		return false;
	}

	mShouldStop = true;

	return true;
}


bool
ApplicationImpl::shouldStop()
{
	return mShouldStop;
}


Layer*
ApplicationImpl::addLayer(const Type& type)
{
	if (mIsRunning)
	{
		return nullptr;
	}

	if (!type.isSubclassOf(Layer::classType()))
	{
		return nullptr;
	}

	auto ctor = TypeRegistry::constructor(type);

	if (!ctor)
	{
		return nullptr;
	}

	auto object = ctor();

	auto layer = Common::staticPointerCast<Layer>(std::move(object));

	if (!layer)
	{
		return nullptr;
	}

	if (!layer->initialize(*this))
	{
		return nullptr;
	}

	auto& storedLayer = mLayers.emplace_back(std::move(layer));
	return storedLayer.get();
}


void 
ApplicationImpl::removeLayer(Layer* layer)
{
	if (mIsRunning)
	{
		return;
	}

	std::erase_if(mLayers, [=](const auto& l) { return l.get() == layer; });
}


Layer*
ApplicationImpl::getLayer(const Type& type)
{
	for (auto& layer : mLayers)
	{
		if (layer->type().isSubclassOf(type))
		{
			return layer.get();
		}
	}

	return nullptr;
}
