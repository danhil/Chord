#ifndef _thread_h
#define _thread_h

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <signal.h>
#include <iostream>

#include "Exception.h"
#include "DebugUtil.h"

class Thread {

private:
  // The thread
  pthread_t thread_m;
  // Thread attributes
  pthread_attr_t threadAttr_m;
  // The thread lock
  pthread_mutex_t threadLock_m;
  // The signal to stop the thread
  pthread_cond_t stopSignal_m;
  // The lock to suspend the thread
  pthread_mutex_t sleepLock_m;
  // The sleep signal predicate
  bool threadStopSignaled_m;
  // The thread signal
  pthread_cond_t threadSignal_m;
  // The lock for the signal
  pthread_mutex_t threadSignalLock_m;
  // The signal predicate
  bool threadSignaled_m;
  // Is the thread done?
  bool threadDone_m;
  // The thread process id
  int32_t pid_m;
  // Debug module
  std::shared_ptr<Debug> debug_m;

protected:

  inline static void * Run(void * opaque_i)
  {
    Thread * thread = static_cast<Thread*>(opaque_i);
    // Set this threads process identifyer
    thread->pid_m = getpid();
    //
    pthread_testcancel();
    // Run forrest run!
    thread->Run();
    // Wait for the thread to complete
    //pthread_join(this);
    // The thread is done
    thread->threadDone_m = true;
    // Exit the thread
    pthread_exit(0);
    // Return the thread
    return static_cast<void*>(thread);
  }

public:

  Thread(std::shared_ptr<Debug> debug_i)
    : thread_m(0)
      , threadStopSignaled_m(false)
      , threadSignaled_m(false)
      , threadDone_m(false)
      , debug_m(debug_i)
  {
    int32_t status = 0;
    do
    {
      if( (status = pthread_mutex_init(&(this->threadLock_m), 0)) != 0) break;

      if( (status = pthread_mutex_init(&(this->threadSignalLock_m), 0)) != 0) break;

      if( (status = pthread_cond_init(&(this->threadSignal_m), 0)) != 0) break;

      if( (status = pthread_mutex_init(&(this->sleepLock_m), 0)) != 0) break;

      if( (status = pthread_cond_init(&(this->stopSignal_m), 0)) != 0) break;

      if( (status = pthread_attr_init(&this->threadAttr_m)) != 0) break;
    }while(0);

    // Check that the init went ok
    if(status != 0)
    {
      *debug_m << Debug::E << "Thread initialization failed." << std::endl;
      // Init went wrong
      throw(new Exception("Could not initialize the thread"));
    }
  }

  virtual ~Thread()
  {
    //std::cout << "thread destructor" << std::endl;
    pthread_attr_destroy(&(this->threadAttr_m));
    pthread_mutex_destroy(&(this->threadLock_m));

    pthread_mutex_destroy(&(this->threadSignalLock_m));
    pthread_cond_destroy(&(this->threadSignal_m));

    pthread_mutex_destroy(&(this->sleepLock_m));
    pthread_cond_destroy(&(this->stopSignal_m));
  }

  inline pthread_t GetThreadID(void)
  {
    *debug_m << Debug::V << "The thread id is: " << pthread_self() << std::endl;

    return (pthread_self());
  }

  inline void Lock(void)
  {
    *debug_m << Debug::V << "Locking the thread with id; " << this->GetThreadID() << std::endl;

    if (pthread_mutex_lock(&(this->threadLock_m)) != 0)
    {
      *debug_m << Debug::E << "Could not lock the thread with id; " << this->GetThreadID() << std::endl;

      throw(new Exception("Could not get the threadLock_m"));
    }
  }

  inline void Unlock(void)
  {
    *debug_m << Debug::V << "Unlocking the thread with id; " << this->GetThreadID() << std::endl;

    if (pthread_mutex_unlock(&(this->threadLock_m)) != 0)
    {
      *debug_m << Debug::E << "Could not unlock the thread with id; " << this->GetThreadID() << std::endl;
      throw(new Exception("Could not release the threadLock_m"));
    }
  }

  inline bool IsDone(void)
  {
    *debug_m << Debug::V << "The thread is done :" << (this->threadDone_m ? "Yes" : "No") << std::endl;
    return this->threadDone_m;
  }

  inline void Detach(void)
  {
    *debug_m << Debug::V << "Detaching the thread with id; " << this->GetThreadID() << std::endl;

    if (pthread_attr_setdetachstate(&threadAttr_m, PTHREAD_CREATE_DETACHED) != 0)
    {
      throw(new Exception("Could not set detached state"));
    }
  }

  // Run the thread
  virtual void Run(){}
  // Start the thread
  void Start();
  // Join the thread
  void Join();
  // Suspend the thread form seconds time
  void Sleep(const uint32_t sec = 0);
  // Suspend the thread form seconds time
  void SleepMs(const uint32_t sec = 0);
  // Suspend the thread for microseconds time
  int32_t SleepUs(const uint64_t micros);
  // Wait for the thread signal to be signaled
  void Wait(void);
  // Notify this thread
  void Notify(void);
  // Destroy this thread
  inline void Kill(void)
  {
    *debug_m << Debug::V << "Killing the thread with id; " << this->GetThreadID() << std::endl;

    if (this->thread_m != 0 && pthread_cancel(this->thread_m) != 0)
    {
      throw(new Exception("Unable to cancel the Thread"));
    }
    else // The thread is done
    {
      this->threadDone_m = true;
    }
  }

  // Send a signal to this thread
  inline bool Signal(int32_t signal_i)
  {
    return (pthread_kill(this->thread_m, signal_i) == 0);
  }

  // Cancel the thread
  inline bool CancelSleep(void)
  {
    bool status = true;
    if (pthread_mutex_lock(&(this->sleepLock_m)) == 0)
    {
      threadStopSignaled_m = true;
      // Set the signal
      pthread_cond_signal(&(this->stopSignal_m));
      // Unlock the mutex
      pthread_mutex_unlock(&(this->sleepLock_m));
    }
    else
    {
      *debug_m << Debug::E << "Cancel thread sleep - could not obtain the lock" << std::endl;
      status = false;
    }
    return status;
  }

};

#endif
