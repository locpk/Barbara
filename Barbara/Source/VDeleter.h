#pragma once
#include <functional>
template <typename T>
class VDeleter
{
public:
	VDeleter() : VDeleter([](T, VkAllocationCallbacks*) {}) {}



	VDeleter(std::function<void(T, VkAllocationCallbacks*)> deletef)
	{
		this->deleter = [=](T obj) { deletef(obj, nullptr); };
	}

	VDeleter(const VDeleter<VkInstance>& instance, std::function<void(VkInstance, T, VkAllocationCallbacks*)> deletef)
	{
		this->deleter = [&instance, deletef](T obj) { deletef(instance, obj, nullptr); };
	}



	VDeleter(const VDeleter<VkDevice>& device, std::function<void(VkDevice, T, VkAllocationCallbacks*)> deletef)
	{
		this->deleter = [&device, deletef](T obj) { deletef(device, obj, nullptr); };
	}


	/*template <typename F>
	VDeleter(VDeleter<F>&& other) : object(VK_NULL_HANDLE), deleter(nullptr)
	{
		*this = std::move(other);
	}

	template <typename F>
	VDeleter<F>& operator=(VDeleter<F>&& other)
	{
		if (this != &other)
		{
			object = other.object;
			deleter = other.deleter;
			other.object = VK_NULL_HANDLE;
		}
		return *this;

	}*/


	~VDeleter() 
	{
		cleanup();
	}

	const T* operator &() const 
	{
		return &object;
	}

	T* replace() 
	{
		cleanup();
		return &object;
	}

	void reset()
	{
		object = VK_NULL_HANDLE;
	}

	operator T() const 
	{
		return object;
	}

	void operator=(const T rhs) 
	{
		if (rhs != object) 
		{
			cleanup();
			object = rhs;
		}
	}

	template<typename V>
	bool operator==(V rhs) 
	{
		return object == T(rhs);
	}

private:
	T object{ VK_NULL_HANDLE };
	std::function<void(T)> deleter;

	void cleanup() 
	{
		if (object != VK_NULL_HANDLE)
		{
			deleter(object);
		}
		object = VK_NULL_HANDLE;
	}
};

