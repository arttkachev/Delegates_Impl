#include <cassert>
#include <iostream>

namespace delegates {
  template<typename ARG1, typename ARG2>
  class DelegateTwoParams {
    using InstancePtr = void*;
    using FuncPtr = void (*)(InstancePtr, ARG1, ARG2);
    using Stub = std::pair<InstancePtr, FuncPtr>;

    template<void (*Function)(ARG1, ARG2)>
    static void FunctionStub(InstancePtr, ARG1 arg1, ARG2 arg2) {
      return (Function)(arg1, arg2);
    }
    template <class C, void (C::* Function)(ARG1, ARG2)>
    static void MethodStub(InstancePtr instance, ARG1 arg1, ARG2 arg2) {
      return (static_cast<C*>(instance)->*Function)(arg1, arg2);
    }
  public:
    DelegateTwoParams() = default;

    template <void (*Function)(ARG1, ARG2)>
    void Bind() {
      m_stub.first = {};
      m_stub.second = &FunctionStub<Function>;
    }

    template <class C, void (C::* Function)(ARG1, ARG2)>
    void Bind(C* instance) {
      m_stub.first = instance;
      m_stub.second = &MethodStub<C, Function>;
    }

    void Invoke(ARG1 arg1, ARG2 arg2) {
      assert(m_stub.second && "Unbound function detected!");
      return m_stub.second(m_stub.first, arg1, arg2);
    }
  private:
    Stub m_stub{};
  };
  
  template<typename...ARGS>
  class DelegateMultipleParams {
    using InstancePtr = void*;
    using FuncPtr = void (*)(InstancePtr, ARGS...);
    using Stub = std::pair<InstancePtr, FuncPtr>;

    template<void (*Function)(ARGS...)>
    static void FunctionStub(InstancePtr, ARGS... args) {
      return (Function)(args...);
    }
    template <class C, void (C::* Function)(ARGS...)>
    static void MethodStub(InstancePtr instance, ARGS... args) {
      return (static_cast<C*>(instance)->*Function)(args...);
    }
  public:
    DelegateMultipleParams() = default;

    template <void (*Function)(ARGS...)>
    void Bind() {
      m_stub.first = {};
      m_stub.second = &FunctionStub<Function>;
    }

    template <class C, void (C::* Function)(ARGS...)>
    void Bind(C* instance) {
      m_stub.first = instance;
      m_stub.second = &MethodStub<C, Function>;
    }

    
    void Invoke(ARGS... args) {
      assert(m_stub.second && "Unbound function detected!");
      return m_stub.second(m_stub.first, args...);
    }
  private:
    Stub m_stub{};
  };
}

void myFunc(int32_t const value1, int32_t const value2) {
  std::cout << value1 + value2 << std::endl;
}

int main() {
  delegates::DelegateTwoParams<int32_t, int32_t> myDelegate{};
  myDelegate.Bind<&myFunc>();
  myDelegate.Invoke(10, 70);
  size_t exitCode{};
  while (exitCode != 1)
    std::cin >> exitCode;
}
