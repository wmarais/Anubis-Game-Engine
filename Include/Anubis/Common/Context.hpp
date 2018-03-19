#ifndef ANUBIS_COMMON_CONTEXT_HPP
#define ANUBIS_COMMON_CONTEXT_HPP

#include "Misc.hpp"

namespace Anubis
{
  namespace Common
  {
    class Context
    {
      /** Indicate if the thread is running or not. */
      std::atomic_bool fIsExecuting;

      /** The exception if one occured during executing of the thread. */
      std::exception_ptr fException;

      /** The thread used for execution. */
      std::thread fThread;

      /** The entrypoint for the thread. */
      void threadEntry();

      /** The rate at which the update() function is invoked. */
      const std::chrono::nanoseconds kUpdateRate;

    protected:

      /*********************************************************************//**
       * The update function that must be implemented by any inheriting
       * class. An attempt is made to invoke the function at the specified
       * update rate.
       *
       * @param dt  The amount of time since update was last called.
       ************************************************************************/
      virtual void update(float dt) = 0;

    public:

      /*********************************************************************//**
       * Create an context update with the specific thread update rate. The
       * constructor also starts the context thread that invokes the update
       * function at roughly the specified update rate. (Ofcourse this is just
       * a best try and if the update rate exceeds this time, then update
       * duration will dt will increase to match it.)
       *
       * @param updateRate  The update rate in nano seconds. I.e. F/1e9.
       ************************************************************************/
      Context(const std::chrono::nanoseconds & updateRate =
          std::chrono::nanoseconds(0));

      /*********************************************************************//**
       * Destroy the context object which stops update thread.
       ************************************************************************/
      virtual ~Context();

      /*********************************************************************//**
       * Return the exception value stored for the context. This will only ever
       * be set anything other than nullptr if an exception is thrown.
       *
       * @return  The pointer to the thrown exception, or nullptr if no
       *          exception has been thrown.
       ************************************************************************/
      std::exception_ptr getException();
    };
  }
}

#endif /* ANUBIS_COMMON_CONTEXT_HPP */
