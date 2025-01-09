//
// Created by Matq on 19/12/2024.
//

#pragma once

#include "Strings.h"

template<typename T>
class Singleton {
private:
    static T* instance_;
    static const char* singleton_name_;

public:
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    static void init(T* instance) { instance_ = instance; }
    static void destroy() { delete instance_; }

    // Getting
    static T* Get() {
        if (instance_ == nullptr)
            throw std::runtime_error(Strings::FString("%s is not initialized. Call initialize first.", singleton_name_));

        return instance_;
    }

};
