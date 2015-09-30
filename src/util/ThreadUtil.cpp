#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

#include <time.h>

#ifdef OSX
#include "TimeHelper.h"
#endif

#include "ThreadUtil.h"

// Start the thread
void Thread::Start()
{
  *debug_m << Debug::V << "Starting the thread with id : " << this->GetThreadID() << std::endl;

  srand((long) pthread_self());
  int32_t status = pthread_create(&this->thread_m, &this->threadAttr_m, &Run, this);
  if (status != 0)
  {
    *debug_m << Debug::E << "Unable to create thread, error = " << std::strerror(status) << std::endl;
    throw(new Exception("Unable to create Thread"));
  }
}

// Join the thread
void Thread::Join()
{
  if (pthread_join(this->thread_m, NULL) != 0)
  {
    throw(new Exception("Unable to join Thread"));
  }
}

void Thread::Sleep(const uint32_t sec)
{
  if (sec > 0)
  {
    usleep(static_cast<uint64_t>(sec * 1000000L));
  }
  else
  {
    usleep(static_cast<uint64_t>(-1));
  }
}


void Thread::SleepMs(const uint32_t msec)
{
  if (msec > 0)
  {
    usleep(static_cast<uint64_t>(msec * 1000L));
  }
  else
  {
    usleep(static_cast<uint64_t>(-1));
  }
}


// Suspend the thread for microsecond time interval
int32_t Thread::SleepUs(uint64_t micros) {

  // init retval to 0
  int32_t retval = 0;
  // Lock the sleeplock and do a timedwait
  if (pthread_mutex_lock(&(this->sleepLock_m)) == 0)
  {
    struct timespec tm;
    // Fill in the current time
    clock_gettime(CLOCK_REALTIME, &tm);
    tm.tv_nsec = tm.tv_nsec + (micros * 1000.0);
    // Wait for the specified time or until signaled
    while(!threadStopSignaled_m || retval == 0)
    {
      retval = pthread_cond_timedwait(&(this->stopSignal_m),&(this->sleepLock_m), &tm);
    }
    // Reset the signal predicate
    threadStopSignaled_m = false;
    // Unlock the lock
    pthread_mutex_unlock(&(this->sleepLock_m));
  }
  else
  {
    *debug_m << Debug::E << "Thread::usleep could not obtain the threadlock" << std::endl;
  }
  return retval;
}


// Wait for a signal on this thread
void Thread::Wait()
{
  if (pthread_mutex_lock(&(this->threadSignalLock_m)) == 0)
  {
    *debug_m << Debug::V << "Thread : " << this->GetThreadID() << " waiting for signal." << std::endl;
    // Wait for the signal
    while(!threadSignaled_m) pthread_cond_wait(&(this->threadSignal_m), &(this->threadSignalLock_m));
    // Reset the thread signal predicate
    threadSignaled_m = false;
    // Unlock the mutex
    pthread_mutex_unlock(&(this->threadSignalLock_m));
  }
  else
  {
    *debug_m << Debug::E << "Thread::wait could not obtain the thread signal lock" << std::endl;
  }
}

void Thread::Notify()
{
  *debug_m << Debug::V << "Thread : " << this->GetThreadID() << " notifying." << std::endl;

  if (pthread_mutex_lock(&(this->threadSignalLock_m)) == 0)
  {
    // Set the predicate.
    threadSignaled_m = true;
    // Signal the thread
    pthread_cond_signal(&(this->threadSignal_m));
    // Unlock the signal lock
    pthread_mutex_unlock(&(this->threadSignalLock_m));
  }
  else
  {
    *debug_m << Debug::E << "Thread::notify could not obtain the thread signal lock" << std::endl;
  }

}
