
#pragma once

#include <algorithm>
#include <cassert>
#include <iostream>
#include <utility>
#include <vector>

template <typename T>
class Action {};

template <typename R>
class Action<R()> {
    public:
        template <R (*Function)()>
        void bind() {
            Stub s(nullptr, &functionStub<Function>);
            if(containsStub(s)) {
                std::cout << "Action is already bound." << std::endl;
                return;
            }

            stubs.push_back(s);
        }

        template <R (*Function)()>
        void unbind() {
            Stub s(nullptr, &functionStub<Function>);
            if(containsStub(s)) {
                markStubForDeletion(s);
                return;
            }

            std::cout << "Cannot unbind Action that is not bound." << std::endl;
        }

        template <class C, R (C::*Function)()>
        void bind(C* instance) {
            Stub s(instance, &classMethodStub<C, Function>);
            if(containsStub(s)) {
                std::cout << "Action is already bound." << std::endl;
                return;
            }

            stubs.push_back(s);
        }

        template <class C, R (C::*Function)()>
        void unbind(C* instance) {
            Stub s(instance, &classMethodStub<C, Function>);
            if(containsStub(s)) {
                markStubForDeletion(s);
                return;
            }

            std::cout << "Cannot unbind Action that is not bound." << std::endl;
        }

        void invoke() {
            cleanStubs();

            for(Stub stub : stubs) {
                assert(stub.second != nullptr);
                stub.second(stub.first);
            }
        }

    private:
        typedef R* InstancePtr;
        typedef R (*InternalFunction)(InstancePtr);
        typedef std::pair<InstancePtr, InternalFunction> Stub;

        std::vector<Stub> stubs = {};

        bool containsStub(Stub other) {
            for(Stub stub : stubs) {
                if(stub.first == other.first && stub.second == other.second) {
                    return true;
                }
            }

            return false;
        }

        void markStubForDeletion(Stub s) {
            for(size_t i = 0; i < stubs.size(); i++) {
                if(stubs[i].first == s.first && stubs[i].second == s.second) {
                    stubs[i].first = nullptr;
                    stubs[i].second = nullptr;
                    return;
                }
            }
        }

        void cleanStubs() {
            for(int i = stubs.size()-1; i >= 0; --i) {
                if(stubs[i].first == nullptr && stubs[i].second == nullptr) {
                    stubs.erase(stubs.begin() + i);
                }
            }
        }

        template <R (*Function)()>
        static inline R functionStub(InstancePtr) {
            return (Function)();
        }

        template <class C, R (C::*Function)()>
        static inline void classMethodStub(InstancePtr instance) {
            return (static_cast<C*>(instance)->*Function)();
        }
};

template <typename R, typename PARAM1>
class Action<R(PARAM1)> {
    public:
        template <R (*Function)(PARAM1)>
        void bind() {
            Stub s(nullptr, &functionStub<Function>);
            if(containsStub(s)) {
                std::cout << "Action is already bound." << std::endl;
                return;
            }

            stubs.push_back(s);
        }

        template <R (*Function)(PARAM1)>
        void unbind() {
            Stub s(nullptr, &functionStub<Function>);
            if(containsStub(s)) {
                deleteStub(s);
                return;
            }

            std::cout << "Cannot unbind Action that is not bound." << std::endl;
        }

        template <class C, R (C::*Function)(PARAM1)>
        void bind(C* instance) {
            Stub s(instance, &classMethodStub<C, Function>);
            if(containsStub(s)) {
                std::cout << "Action is already bound." << std::endl;
                return;
            }

            stubs.push_back(s);
        }

        template <class C, R (C::*Function)(PARAM1)>
        void unbind(C* instance) {
            Stub s(instance, &classMethodStub<C, Function>);
            if(containsStub(s)) {
                deleteStub(s);
                return;
            }

            std::cout << "Cannot unbind Action that is not bound." << std::endl;
        }

        void invoke(PARAM1 param1) {
            cleanStubs();

            for(Stub stub : stubs) {
                assert(stub.second != nullptr);
                stub.second(stub.first, param1);
            }
        }

    private:
        typedef R* InstancePtr;
        typedef R (*InternalFunction)(InstancePtr, PARAM1);
        typedef std::pair<InstancePtr, InternalFunction> Stub;

        std::vector<Stub> stubs = {};

        bool containsStub(Stub other) {
            for(Stub stub : stubs) {
                if(stub.first == other.first && stub.second == other.second) {
                    return true;
                }
            }

            return false;
        }

        void markStubForDeletion(Stub s) {
            for(size_t i = 0; i < stubs.size(); i++) {
                if(stubs[i].first == s.first && stubs[i].second == s.second) {
                    stubs[i].first = nullptr;
                    stubs[i].second = nullptr;
                    return;
                }
            }
        }

        void cleanStubs() {
            for(int i = stubs.size()-1; i >= 0; --i) {
                if(stubs[i].first == nullptr && stubs[i].second == nullptr) {
                    stubs.erase(stubs.begin() + i);
                }
            }
        }

        template <R (*Function)(PARAM1)>
        static inline R functionStub(InstancePtr, PARAM1 param1) {
            return (Function)(param1);
        }

        template <class C, R (C::*Function)(PARAM1)>
        static inline void classMethodStub(InstancePtr instance, PARAM1 param1) {
            return (static_cast<C*>(instance)->*Function)(param1);
        }
};

template <typename R, typename PARAM1, typename PARAM2>
class Action<R(PARAM1, PARAM2)> {
    public:
        template <R (*Function)(PARAM1, PARAM2)>
        void bind() {
            Stub s(nullptr, &functionStub<Function>);
            if(containsStub(s)) {
                std::cout << "Action is already bound." << std::endl;
                return;
            }

            stubs.push_back(s);
        }

        template <R (*Function)(PARAM1, PARAM2)>
        void unbind() {
            Stub s(nullptr, &functionStub<Function>);
            if(containsStub(s)) {
                deleteStub(s);
                return;
            }

            std::cout << "Cannot unbind Action that is not bound." << std::endl;
        }

        template <class C, R (C::*Function)(PARAM1, PARAM2)>
        void bind(C* instance) {
            Stub s(instance, &classMethodStub<C, Function>);
            if(containsStub(s)) {
                std::cout << "Action is already bound." << std::endl;
                return;
            }

            stubs.push_back(s);
        }

        template <class C, R (C::*Function)(PARAM1, PARAM2)>
        void unbind(C* instance) {
            Stub s(instance, &classMethodStub<C, Function>);
            if(containsStub(s)) {
                deleteStub(s);
                return;
            }

            std::cout << "Cannot unbind Action that is not bound." << std::endl;
        }

        void invoke(PARAM1 param1, PARAM2 param2) {
            cleanStubs();

            for(Stub stub : stubs) {
                assert(stub.second != nullptr);
                stub.second(stub.first, param1, param2);
            }
        }

    private:
        typedef R* InstancePtr;
        typedef R (*InternalFunction)(InstancePtr, PARAM1, PARAM2);
        typedef std::pair<InstancePtr, InternalFunction> Stub;

        std::vector<Stub> stubs = {};

        bool containsStub(Stub other) {
            for(Stub stub : stubs) {
                if(stub.first == other.first && stub.second == other.second) {
                    return true;
                }
            }

            return false;
        }

        void markStubForDeletion(Stub s) {
            for(size_t i = 0; i < stubs.size(); i++) {
                if(stubs[i].first == s.first && stubs[i].second == s.second) {
                    stubs[i].first = nullptr;
                    stubs[i].second = nullptr;
                    return;
                }
            }
        }

        void cleanStubs() {
            for(int i = stubs.size()-1; i >= 0; --i) {
                if(stubs[i].first == nullptr && stubs[i].second == nullptr) {
                    stubs.erase(stubs.begin() + i);
                }
            }
        }

        template <R (*Function)(PARAM1, PARAM2)>
        static inline R functionStub(InstancePtr, PARAM1 param1, PARAM2 param2) {
            return (Function)(param1, param2);
        }

        template <class C, R (C::*Function)(PARAM1, PARAM2)>
        static inline void classMethodStub(InstancePtr instance, PARAM1 param1, PARAM2 param2) {
            return (static_cast<C*>(instance)->*Function)(param1, param2);
        }
};