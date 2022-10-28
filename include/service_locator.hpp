#pragma once
#include <unordered_map>
#include <memory>
#include "type_hash.h"

class ServiceLocator
{
private:
    inline static std::unordered_map<type_id_t, std::shared_ptr<void>> instances;

public:
    ~ServiceLocator()
    {
        instances.clear();
    };

    template <typename T>
    static std::shared_ptr<T> Register(T* instance = new T())
    {
        auto existing = Resolve<T>();

        if (existing == nullptr)
        {
            instances.emplace(type_id<T>(), std::shared_ptr<void>(instance));
            return Resolve<T>();
        }

        return existing;
    }

    template <typename T>
    static std::shared_ptr<T> Resolve()
    {
        auto itr1 = instances.find(type_id<T>());
        if (itr1 != instances.end())
            return std::static_pointer_cast<T>(itr1->second);

        return nullptr;
    }
};