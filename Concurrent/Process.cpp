//
// Created by up duan on 24/7/15.
//

#include "Process.h"

std::unique_ptr<Process> Process::Create(std::wstring const& command) {
    Process* result = new Process(command);
    return std::unique_ptr<Process>(result);
    //return std::make_unique(result);
}

Process::Process(std::wstring const& command) : executableFile_(command), processInfo_(nullptr) {}

void Process::Start() {
    //WinExec("上海市中小学数字教材\\上海中小学数字教材.exe", SW_SHOWNORMAL);
//    STARTUPINFO StartupInfo; //This is an [in] parameter
//    memset(&StartupInfo, 0, sizeof(StartupInfo));
//    StartupInfo.cb = sizeof StartupInfo; //Only compulsory field
//    return TRUE == CreateProcess(const_cast<wchar_t*>(executableFile.c_str()), NULL, NULL, NULL, FALSE, 0, NULL, NULL, &StartupInfo, &ProcessInfo);
}

void Process::Stop() {
//    TerminateProcess(ProcessInfo.hProcess, 0);
//    CloseHandle(ProcessInfo.hProcess);
}

//#include <windows.h>
//#include <tchar.h>
//#include <stdio.h>

////
//// Thread pool wait callback function template
////
//VOID CALLBACK MyWaitCallback(PTP_CALLBACK_INSTANCE Instance, PVOID Parameter, PTP_WAIT Wait, TP_WAIT_RESULT WaitResult) {
//    // Instance, Parameter, Wait, and WaitResult not used in this example.
//    UNREFERENCED_PARAMETER(Instance);
//    UNREFERENCED_PARAMETER(Parameter);
//    UNREFERENCED_PARAMETER(Wait);
//    UNREFERENCED_PARAMETER(WaitResult);
//
//    //
//    // Do something when the wait is over.
//    //
//    _tprintf(_T("MyWaitCallback: wait is over.\n"));
//}


////
//// Thread pool timer callback function template
////
//VOID CALLBACK MyTimerCallback(PTP_CALLBACK_INSTANCE Instance, PVOID Parameter, PTP_TIMER Timer) {
//    // Instance, Parameter, and Timer not used in this example.
//    UNREFERENCED_PARAMETER(Instance);
//    UNREFERENCED_PARAMETER(Parameter);
//    UNREFERENCED_PARAMETER(Timer);
//
//    //
//    // Do something when the timer fires.
//    //
//    _tprintf(_T("MyTimerCallback: timer has fired.\n"));
//}


////
//// This is the thread pool work callback function.
////
//VOID CALLBACK MyWorkCallback(PTP_CALLBACK_INSTANCE Instance, PVOID Parameter, PTP_WORK Work) {
//    // Instance, Parameter, and Work not used in this example.
//    UNREFERENCED_PARAMETER(Instance);
//    UNREFERENCED_PARAMETER(Parameter);
//    UNREFERENCED_PARAMETER(Work);
//
//    BOOL bRet = FALSE;
//
//    //
//    // Do something when the work callback is invoked.
//    //
//    {
//        _tprintf(_T("MyWorkCallback: Task performed.\n"));
//    }
//
//    return;
//}


//VOID DemoCleanupPersistentWorkTimer() {
//    BOOL bRet = FALSE;
//    PTP_WORK work = NULL;
//    PTP_TIMER timer = NULL;
//    PTP_POOL pool = NULL;
//    PTP_WORK_CALLBACK workcallback = MyWorkCallback;
//    PTP_TIMER_CALLBACK timercallback = MyTimerCallback;
//    TP_CALLBACK_ENVIRON CallBackEnviron;
//    PTP_CLEANUP_GROUP cleanupgroup = NULL;
//    FILETIME FileDueTime;
//    ULARGE_INTEGER ulDueTime;
//    UINT rollback = 0;
//
//    InitializeThreadpoolEnvironment(&CallBackEnviron);
//
//    //
//    // Create a custom, dedicated thread pool.
//    //
//    pool = CreateThreadpool(NULL);
//
//    if (NULL == pool) {
//        _tprintf(_T("CreateThreadpool failed. LastError: %u\n"), GetLastError());
//        goto main_cleanup;
//    }
//
//    rollback = 1; // pool creation succeeded
//
//    //
//    // The thread pool is made persistent simply by setting
//    // both the minimum and maximum threads to 1.
//    //
//    SetThreadpoolThreadMaximum(pool, 1);
//
//    bRet = SetThreadpoolThreadMinimum(pool, 1);
//
//    if (FALSE == bRet) {
//        _tprintf(_T("SetThreadpoolThreadMinimum failed. LastError: %u\n"), GetLastError());
//        goto main_cleanup;
//    }
//
//    //
//    // Create a cleanup group for this thread pool.
//    //
//    cleanupgroup = CreateThreadpoolCleanupGroup();
//
//    if (NULL == cleanupgroup) {
//        _tprintf(_T("CreateThreadpoolCleanupGroup failed. LastError: %u\n"), GetLastError());
//        goto main_cleanup;
//    }
//
//    rollback = 2;  // Cleanup group creation succeeded
//
//    //
//    // Associate the callback environment with our thread pool.
//    //
//    SetThreadpoolCallbackPool(&CallBackEnviron, pool);
//
//    //
//    // Associate the cleanup group with our thread pool.
//    // Objects created with the same callback environment
//    // as the cleanup group become members of the cleanup group.
//    //
//    SetThreadpoolCallbackCleanupGroup(&CallBackEnviron, cleanupgroup, NULL);
//
//    //
//    // Create work with the callback environment.
//    //
//    work = CreateThreadpoolWork(workcallback, NULL, &CallBackEnviron);
//
//    if (NULL == work) {
//        _tprintf(_T("CreateThreadpoolWork failed. LastError: %u\n"), GetLastError());
//        goto main_cleanup;
//    }
//
//    rollback = 3;  // Creation of work succeeded
//
//    //
//    // Submit the work to the pool. Because this was a pre-allocated
//    // work item (using CreateThreadpoolWork), it is guaranteed to execute.
//    //
//    SubmitThreadpoolWork(work);
//
//
//    //
//    // Create a timer with the same callback environment.
//    //
//    timer = CreateThreadpoolTimer(timercallback, NULL, &CallBackEnviron);
//
//
//    if (NULL == timer) {
//        _tprintf(_T("CreateThreadpoolTimer failed. LastError: %u\n"), GetLastError());
//        goto main_cleanup;
//    }
//
//    rollback = 4;  // Timer creation succeeded
//
//    //
//    // Set the timer to fire in one second.
//    //
//    ulDueTime.QuadPart = (ULONGLONG) - (1 * 10 * 1000 * 1000);
//    FileDueTime.dwHighDateTime = ulDueTime.HighPart;
//    FileDueTime.dwLowDateTime = ulDueTime.LowPart;
//
//    SetThreadpoolTimer(timer, &FileDueTime, 0, 0);
//
//    //
//    // Delay for the timer to be fired
//    //
//    Sleep(1500);
//
//    //
//    // Wait for all callbacks to finish.
//    // CloseThreadpoolCleanupGroupMembers also releases objects
//    // that are members of the cleanup group, so it is not necessary
//    // to call close functions on individual objects
//    // after calling CloseThreadpoolCleanupGroupMembers.
//    //
//    CloseThreadpoolCleanupGroupMembers(cleanupgroup, FALSE, NULL);
//
//    //
//    // Already cleaned up the work item with the
//    // CloseThreadpoolCleanupGroupMembers, so set rollback to 2.
//    //
//    rollback = 2;
//    goto main_cleanup;
//
//    main_cleanup:
//    //
//    // Clean up any individual pieces manually
//    // Notice the fall-through structure of the switch.
//    // Clean up in reverse order.
//    //
//
//    switch (rollback) {
//        case 4:
//        case 3:
//            // Clean up the cleanup group members.
//            CloseThreadpoolCleanupGroupMembers(cleanupgroup, FALSE, NULL);
//        case 2:
//            // Clean up the cleanup group.
//            CloseThreadpoolCleanupGroup(cleanupgroup);
//        case 1:
//            // Clean up the pool.
//            CloseThreadpool(pool);
//        default:
//            break;
//    }
//
//    return;
//}


//VOID DemoNewRegisterWait() {
//    PTP_WAIT Wait = NULL;
//    PTP_WAIT_CALLBACK waitcallback = MyWaitCallback;
//    HANDLE hEvent = NULL;
//    UINT i = 0;
//    UINT rollback = 0;
//
//    //
//    // Create an auto-reset event.
//    //
//    hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
//
//    if (NULL == hEvent) {
//        // Error Handling
//        return;
//    }
//
//    rollback = 1; // CreateEvent succeeded
//
//    Wait = CreateThreadpoolWait(waitcallback, NULL, NULL);
//
//    if (NULL == Wait) {
//        _tprintf(_T("CreateThreadpoolWait failed. LastError: %u\n"), GetLastError());
//        goto new_wait_cleanup;
//    }
//
//    rollback = 2; // CreateThreadpoolWait succeeded
//
//    //
//    // Need to re-register the event with the wait object
//    // each time before signaling the event to trigger the wait callback.
//    //
//    for (i = 0; i < 5; i++) {
//        SetThreadpoolWait(Wait, hEvent, NULL);
//
//        SetEvent(hEvent);
//
//        //
//        // Delay for the waiter thread to act if necessary.
//        //
//        Sleep(500);
//
//        //
//        // Block here until the callback function is done executing.
//        //
//
//        WaitForThreadpoolWaitCallbacks(Wait, FALSE);
//    }
//
//    new_wait_cleanup:
//    switch (rollback) {
//        case 2:
//            // Unregister the wait by setting the event to NULL.
//            SetThreadpoolWait(Wait, NULL, NULL);
//
//            // Close the wait.
//            CloseThreadpoolWait(Wait);
//
//        case 1:
//            // Close the event.
//            CloseHandle(hEvent);
//
//        default:
//            break;
//    }
//    return;
//}


//int main(void) {
//    DemoNewRegisterWait();
//    DemoCleanupPersistentWorkTimer();
//    return 0;
//}


//#include <windows.h>
//#include <functional>
//#include <memory>
//#include <type_traits>

//// -------------------------
//namespace tpoollib {
//    // Handles template
//
//    // Destruction Policy
//    template<typename T>
//    class destruction_policy {
//    public:
//        static void destruct(T h) {
//            static_assert(false, "Must define destructor");
//        }
//    };
//
//    // Policies Specialization
//    template<>
//    class destruction_policy<PTP_POOL> {
//    public:
//        static void destruct(PTP_POOL h) { CloseThreadpool(h); }
//    };
//
//    template<>
//    class destruction_policy<PTP_WORK> {
//    public:
//        static void destruct(PTP_WORK h) { CloseThreadpoolWork(h); }
//    };
//
//    template<>
//    class destruction_policy<PTP_WAIT> {
//    public:
//        static void destruct(PTP_WAIT h) { CloseThreadpoolWait(h); }
//    };
//
//    template<>
//    class destruction_policy<PTP_TIMER> {
//    public:
//        static void destruct(PTP_TIMER h) { CloseThreadpoolTimer(h); }
//    };
//
//    template<>
//    class destruction_policy<PTP_IO> {
//    public:
//        static void destruct(PTP_IO h) { CloseThreadpoolIo(h); }
//    };
//
//    template<>
//    class destruction_policy<PTP_CLEANUP_GROUP> {
//    public:
//        static void destruct(PTP_CLEANUP_GROUP h) { CloseThreadpoolCleanupGroup(h); }
//    };
//
//
//    // Template for Handles
//    template<typename T, typename Destruction = destruction_policy<T>>
//    class handle {
//    private:
//        T hX = 0;
//        bool NoDestruct = true;
//        std::shared_ptr<size_t> ptr = std::make_shared<size_t>();
//
//    public:
//
//        // Closing items
//        void Close() {
//            if (!ptr || !ptr.unique()) {
//                ptr.reset();
//                return;
//            }
//            ptr.reset();
//            if (hX != 0 && !NoDestruct)
//                Destruction::destruct(hX);
//            hX = 0;
//        }
//
//        handle() {
//            hX = 0;
//        }
//
//        ~handle() {
//            Close();
//        }
//
//        handle(const handle &h) {
//            Dup(h);
//        }
//
//        handle(handle &&h) {
//            Move(std::forward<handle>(h));
//        }
//
//        handle(T hY, bool NoDestructOnClose) {
//            hX = hY;
//            NoDestruct = NoDestructOnClose;
//        }
//
//        handle &operator=(const handle &h) {
//            Dup(h);
//            return *this;
//        }
//
//        handle &operator=(handle &&h) {
//            Move(std::forward<handle>(h));
//            return *this;
//        }
//
//        void Dup(const handle &h) {
//            Close();
//            NoDestruct = h.NoDestruct;
//            hX = h.hX;
//            ptr = h.ptr;
//        }
//
//        void Move(handle &&h) {
//            Close();
//            hX = h.hX;
//            ptr = h.ptr;
//            NoDestruct = h.NoDestruct;
//            h.ptr.reset();
//            h.hX = 0;
//            h.NoDestruct = false;
//        }
//
//        operator T() const {
//            return hX;
//        }
//    };
//
//    template<bool AutoDestruct = true>
//    class tpool {
//    private:
//        handle<PTP_POOL> p;
//        handle<PTP_CLEANUP_GROUP> pcg;
//        TP_CALLBACK_ENVIRON env;
//
//        tpool(const tpool &) = delete;
//
//        tpool(tpool &&) = delete;
//
//        void operator=(const tpool &) = delete;
//
//        void operator=(tpool &&) = delete;
//
//    public:
//
//        tpool() {
//        }
//
//        ~tpool() {
//            End();
//        }
//
//        void End() {
//            Join();
//            DestroyThreadpoolEnvironment(&env);
//            p.Close();
//        }
//
//        // Creates the interfaces
//        bool Create(unsigned long nmin = 1, unsigned long nmax = 1) {
//            bool jauto = AutoDestruct;
//
//            // Env
//            InitializeThreadpoolEnvironment(&env);
//
//            // Pool and Min/Max
//            handle<PTP_POOL> cx(CreateThreadpool(0), false);
//            p = cx;
//            if (!p) {
//                End();
//                return false;
//            }
//            if (!SetThreadpoolThreadMinimum(p, nmin)) {
//                End();
//                return false;
//            }
//            SetThreadpoolThreadMaximum(p, nmax);
//
//            // Cleanup Group
//            if (jauto) {
//                handle<PTP_CLEANUP_GROUP> cx(CreateThreadpoolCleanupGroup(), false);
//                pcg = cx;
//                if (!pcg) {
//                    End();
//                    return false;
//                }
//            }
//
//            // Sets
//            SetThreadpoolCallbackPool(&env, p);
//            SetThreadpoolCallbackCleanupGroup(&env, pcg, 0);
//
//            return true;
//        }
//
//        // Templates for each of the items, to be specialized later
//        template<typename J>
//        void Wait(handle<J> h, bool Cancel = false) {
//            static_assert(false, "No Wait function is defined");
//        }
//
//        template<typename J, typename CB_J>
//        handle<J> CreateItem(CB_J cb, PVOID opt = 0, HANDLE hX = 0) {
//            static_assert(false, "No Create function is defined");
//        }
//
//        template<typename J, typename ...A>
//        void RunItem(handle<J> h, std::tuple<A...> = std::make_tuple<>()) {
//            static_assert(false, "No Run function is defined");
//        }
//
//        // Work Stuff
//        template<>
//        handle<PTP_WORK> CreateItem<PTP_WORK, PTP_WORK_CALLBACK>(PTP_WORK_CALLBACK cb, PVOID opt, HANDLE) {
//            handle<PTP_WORK> a(CreateThreadpoolWork(cb, opt, &env), AutoDestruct);
//            return a;
//        }
//
//        template<>
//        void RunItem<PTP_WORK>(handle<PTP_WORK> h, std::tuple<>) {
//            SubmitThreadpoolWork(h);
//        }
//
//        template<>
//        void Wait<PTP_WORK>(handle<PTP_WORK> h, bool Cancel) {
//            WaitForThreadpoolWorkCallbacks(h, Cancel);
//        }
//
//        // Wait  stuff
//        template<>
//        handle<PTP_WAIT> CreateItem<PTP_WAIT, PTP_WAIT_CALLBACK>(PTP_WAIT_CALLBACK cb, PVOID opt, HANDLE) {
//            handle<PTP_WAIT> a(CreateThreadpoolWait(cb, opt, &env), AutoDestruct);
//            return a;
//        }
//
//        template<>
//        void Wait<PTP_WAIT>(handle<PTP_WAIT> h, bool Cancel) {
//            WaitForThreadpoolWaitCallbacks(h, Cancel);
//        }
//
//        // Timer stuff
//        template<>
//        handle<PTP_TIMER> CreateItem<PTP_TIMER, PTP_TIMER_CALLBACK>(PTP_TIMER_CALLBACK cb, PVOID opt, HANDLE) {
//            handle<PTP_TIMER> a(CreateThreadpoolTimer(cb, opt, &env), AutoDestruct);
//            return a;
//        }
//
//        template<>
//        void RunItem<PTP_TIMER>(handle<PTP_TIMER> h, std::tuple<FILETIME *, DWORD, DWORD> t) {
//            SetThreadpoolTimer(h, std::get<0>(t), std::get<1>(t), std::get<2>(t));
//        }
//
//        template<>
//        void Wait<PTP_TIMER>(handle<PTP_TIMER> h, bool Cancel) {
//            WaitForThreadpoolTimerCallbacks(h, Cancel);
//        }
//
//        // IO Stuff
//        template<>
//        handle<PTP_IO> CreateItem<PTP_IO, PTP_WIN32_IO_CALLBACK>(PTP_WIN32_IO_CALLBACK cb, PVOID opt, HANDLE hY) {
//            handle<PTP_IO> a(CreateThreadpoolIo(hY, cb, opt, &env), AutoDestruct);
//            return a;
//        }
//
//        template<>
//        void RunItem<PTP_IO>(handle<PTP_IO> h, std::tuple<bool> t) {
//            bool Cancel = std::get<0>(t);
//            if (Cancel)
//                CancelThreadpoolIo(h);
//            else
//                StartThreadpoolIo(h);
//        }
//
//        template<>
//        void Wait<PTP_IO>(handle<PTP_IO> h, bool Cancel) {
//            WaitForThreadpoolIoCallbacks(h, Cancel);
//        }
//
//        // Join functions, one for each option (AutoDestruct or not)
//        template<bool Q = AutoDestruct>
//        typename std::enable_if<Q, void>::type
//        Join(bool Cancel = false) {
//            if (pcg) {
//                CloseThreadpoolCleanupGroupMembers(pcg, Cancel, 0);
//                pcg.Close();
//            }
//        }
//
//        template<bool Q = AutoDestruct>
//        typename std::enable_if<!Q, void>::type
//        Join(bool Cancel = false,
//             std::initializer_list<handle<PTP_WORK>> h1 = std::initializer_list<handle<PTP_WORK>>({}),
//             std::initializer_list<handle<PTP_TIMER>> h2 = std::initializer_list<handle<PTP_TIMER>>({}),
//             std::initializer_list<handle<PTP_WAIT>> h3 = std::initializer_list<handle<PTP_WAIT>>({}),
//             std::initializer_list<handle<PTP_IO>> h4 = std::initializer_list<handle<PTP_IO>>({})
//        ) {
//            for (auto a : h1)
//                Wait<PTP_WORK>(a, Cancel);
//            for (auto a : h2)
//                Wait<PTP_TIMER>(a, Cancel);
//            for (auto a : h3)
//                Wait<PTP_WAIT>(a, Cancel);
//            for (auto a : h4)
//                Wait<PTP_IO>(a, Cancel);
//        }
//
//    };
//
//}


//using namespace tpoollib;
//int __stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
//    CoInitializeEx(0, COINIT_APARTMENTTHREADED);
//
//    // Auto-Destruction of items
//    {
//        tpool<true> t;
//        t.Create();
//
//        auto workit = t.CreateItem<PTP_WORK, PTP_WORK_CALLBACK>([](PTP_CALLBACK_INSTANCE, PVOID, PTP_WORK) {
//            Sleep(((rand() % 5) + 1) * 1000);
//            return;
//        }, 0);
//        for (int i = 0; i < 3; i++)
//            t.RunItem(workit);
//        t.Join();
//    }
//
//    // Manual Destruction of items
//    {
//        tpool<false> t;
//        t.Create();
//
//        auto workit = t.CreateItem<PTP_WORK, PTP_WORK_CALLBACK>([](PTP_CALLBACK_INSTANCE, PVOID, PTP_WORK) {
//            Sleep(((rand() % 5) + 1) * 1000);
//            return;
//        }, 0);
//        for (int i = 0; i < 3; i++)
//            t.RunItem(workit);
//        t.Join(true,{workit});
//    }
//
//    return 0;
//}
