
#pragma once

#include <iostream>
#include <vector>

#define ACTION_DESTRUCTOR                                                                               \
    ~Action() {                                                                                         \
        cleanStubs();                                                                                   \
        if(!stubs.empty()) {                                                                            \
            std::cerr << "An Action is still bound to " << stubs.size() << " functions" << std::endl;   \
        }                                                                                               \
    }

#define ACTION_BIND_TO_FUNC                                                                             \
    void bind() {                                                                                       \
        Stub s(nullptr, &functionStub<Function>);                                                       \
        if(containsStub(s)) {                                                                           \
            std::cout << "Action is already bound." << std::endl;                                       \
            return;                                                                                     \
        }                                                                                               \
        stubs.push_back(s);                                                                             \
    }

#define ACTION_UNBIND_FROM_FUNC                                                                         \
    void unbind() {                                                                                     \
        Stub s(nullptr, &functionStub<Function>);                                                       \
        if(containsStub(s)) {                                                                           \
            markStubForDeletion(s);                                                                     \
        }                                                                                               \
    }

#define ACTION_BIND_TO_CLASS                                                                            \
    void bind(C* instance) {                                                                            \
        Stub s(instance, &classMethodStub<C, Function>);                                                \
        if(containsStub(s)) {                                                                           \
            std::cout << "Action is already bound." << std::endl;                                       \
            return;                                                                                     \
        }                                                                                               \
        stubs.push_back(s);                                                                             \
    }

#define ACTION_UNBIND_FROM_CLASS                                                                        \
    void unbind(C* instance) {                                                                          \
        Stub s(instance, &classMethodStub<C, Function>);                                                \
        if(containsStub(s)) {                                                                           \
            markStubForDeletion(s);                                                                     \
        }                                                                                               \
    }

#define ACTION_EMPTY_CHECK bool isEmpty() { return stubs.empty(); }

#define ACTION_CONTAINS_STUB                                                                            \
    bool containsStub(Stub other) {                                                                     \
        for(Stub stub : stubs) {                                                                        \
            if(stub.first == other.first && stub.second == other.second) {                              \
                return true;                                                                            \
            }                                                                                           \
        }                                                                                               \
        return false;                                                                                   \
    }

#define ACTION_MARK_STUB_FOR_DELETION                                                                   \
    void markStubForDeletion(Stub s) {                                                                  \
        for(size_t i = 0; i < stubs.size(); i++) {                                                      \
            if(stubs[i].first == s.first && stubs[i].second == s.second) {                              \
                stubs.erase(stubs.begin() + i);                                                         \
                return;                                                                                 \
            }                                                                                           \
        }                                                                                               \
    }

#define ACTION_CLEAN_STUBS                                                                              \
    void cleanStubs() {                                                                                 \
        for(int i = stubs.size()-1; i >= 0; --i) {                                                      \
            if(stubs[i].first == nullptr && stubs[i].second == nullptr) {                               \
                stubs.erase(stubs.begin() + i);                                                         \
            }                                                                                           \
        }                                                                                               \
    }

template <typename T>
class Action {};

template <typename R>
class Action<R()> {
    public:
        ACTION_DESTRUCTOR

        template <R (*Function)()>
        ACTION_BIND_TO_FUNC;
        template <R (*Function)()>
        ACTION_UNBIND_FROM_FUNC;

        template <class C, R (C::*Function)()>
        ACTION_BIND_TO_CLASS;
        template <class C, R (C::*Function)()>
        ACTION_UNBIND_FROM_CLASS;

        ACTION_EMPTY_CHECK;

        void invoke() {
            cleanStubs();

            for(int i = stubs.size()-1; i >= 0; --i) {
                if(stubs[i].first == nullptr || stubs[i].second == nullptr) { return; }
                stubs[i].second(stubs[i].first);
            }
        }

    private:
        typedef R* InstancePtr;
        typedef R (*InternalFunction)(InstancePtr);
        typedef std::pair<InstancePtr, InternalFunction> Stub;

        std::vector<Stub> stubs = {};

        ACTION_CONTAINS_STUB;
        ACTION_MARK_STUB_FOR_DELETION;
        ACTION_CLEAN_STUBS;

        template <R (*Function)()>
        static R functionStub(InstancePtr) {
            return (Function)();
        }

        template <class C, R (C::*Function)()>
        static void classMethodStub(InstancePtr instance) {
            return (static_cast<C*>(instance)->*Function)();
        }
};

template <typename R, typename PARAM1>
class Action<R(PARAM1)> {
    public:
        ACTION_DESTRUCTOR

        template <R (*Function)(PARAM1)>
        ACTION_BIND_TO_FUNC;
        template <R (*Function)(PARAM1)>
        ACTION_UNBIND_FROM_FUNC;

        template <class C, R (C::*Function)(PARAM1)>
        ACTION_BIND_TO_CLASS;
        template <class C, R (C::*Function)(PARAM1)>
        ACTION_UNBIND_FROM_CLASS;

        ACTION_EMPTY_CHECK;

        void invoke(PARAM1 param1) {
            cleanStubs();

            for(Stub stub : stubs) {
                if(stub.first == nullptr || stub.second == nullptr) { return; }

                stub.second(stub.first, param1);
            }
        }

    private:
        typedef R* InstancePtr;
        typedef R (*InternalFunction)(InstancePtr, PARAM1);
        typedef std::pair<InstancePtr, InternalFunction> Stub;

        std::vector<Stub> stubs = {};

        ACTION_CONTAINS_STUB;
        ACTION_MARK_STUB_FOR_DELETION;
        ACTION_CLEAN_STUBS;

        template <R (*Function)(PARAM1)>
        static R functionStub(InstancePtr, PARAM1 param1) {
            return (Function)(param1);
        }

        template <class C, R (C::*Function)(PARAM1)>
        static void classMethodStub(InstancePtr instance, PARAM1 param1) {
            return (static_cast<C*>(instance)->*Function)(param1);
        }
};

template <typename R, typename PARAM1, typename PARAM2>
class Action<R(PARAM1, PARAM2)> {
    public:
        ACTION_DESTRUCTOR

        template <R (*Function)(PARAM1, PARAM2)>
        ACTION_BIND_TO_FUNC;
        template <R (*Function)(PARAM1, PARAM2)>
        ACTION_UNBIND_FROM_FUNC;

        template <class C, R (C::*Function)(PARAM1, PARAM2)>
        ACTION_BIND_TO_CLASS;
        template <class C, R (C::*Function)(PARAM1, PARAM2)>
        ACTION_UNBIND_FROM_CLASS;

        ACTION_EMPTY_CHECK;

        void invoke(PARAM1 param1, PARAM2 param2) {
            cleanStubs();

            for(Stub stub : stubs) {
                if(stub.first == nullptr || stub.second == nullptr) { return; }

                stub.second(stub.first, param1, param2);
            }
        }

    private:
        typedef R* InstancePtr;
        typedef R (*InternalFunction)(InstancePtr, PARAM1, PARAM2);
        typedef std::pair<InstancePtr, InternalFunction> Stub;

        std::vector<Stub> stubs = {};

        ACTION_CONTAINS_STUB;
        ACTION_MARK_STUB_FOR_DELETION;
        ACTION_CLEAN_STUBS;

        template <R (*Function)(PARAM1, PARAM2)>
        static R functionStub(InstancePtr, PARAM1 param1, PARAM2 param2) {
            return (Function)(param1, param2);
        }

        template <class C, R (C::*Function)(PARAM1, PARAM2)>
        static void classMethodStub(InstancePtr instance, PARAM1 param1, PARAM2 param2) {
            return (static_cast<C*>(instance)->*Function)(param1, param2);
        }
};

#undef ACTION_DESTRUCTOR

#undef ACTION_BIND_TO_FUNC
#undef ACTION_UNBIND_FROM_FUNC

#undef ACTION_BIND_TO_CLASS
#undef ACTION_UNBIND_FROM_CLASS

#undef ACTION_EMPTY_CHECK

#undef ACTION_CONTAINS_STUB
#undef ACTION_MARK_STUB_FOR_DELETION
#undef ACTION_CLEAN_STUBS