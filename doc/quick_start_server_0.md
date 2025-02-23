Quick Start Server 0 {#quick_start_server_0}
==

\section simple_server A simple server

\par
This section provides an example of a simple server. The server will run
for 10 seconds and then automatically shut-down.

\par Summary
This is an overview of the code we will need in our example server:
-# Include headers
-# Initialize the dbus-cxx library
-# Create a dispatcher to manage threads, timeouts and I/O watches
-# Create a connection to the D-Bus session bus
-# Request a name for our application on the bus
-# Create an object on the bus
-# Add a method named \c add to our object
-# Wait for incoming calls

\section quick_start_server_0_code Simple server 0 code

\par
Here is the code in full, followed by a section-by-section discussion.

\include quickstart_server_0.cpp

\section quick_start_server_0_discussion Simple server 0 code discussion

\par Include headers
First, we need to include the main dbus-cxx library header:
\code
#include <dbus-cxx.h>
\endcode

\par
Then we will define a function that will be our workhorse on the server side.
This function will be the actual function that will be called when the dbus
\c add method is invoked, so we will name our function similarly. But, note
that the actual name of the function in our program and the name in our
dbus server do not have to match.
\code double add( double param1, double param2 )      { return param1 + param2; } \endcode

\par
And now for the beginning of our \c main() function. We'll also declare a
variable \c ret that can be used to check the return value of functions.
\code{.cpp}
int main()
{
  int ret;
\endcode

\par Create a dispatcher to manage threads, timeouts and I/O watches
Now, we will create a dispatcher to handle incoming and outgoing messages.
Handling incoming and outgoing messages can be messy and the Dispatcher
class handles much of this for us.
\par Since this is just a standalone example, we will use the standalone dispatcher
\code std::shared_ptr<DBus::Dispatcher> dispatcher = DBus::StandaloneDispatcher::create(); \endcode

\par Create a connection to the D-Bus session bus
Now that we have a dispatcher we need to create a connection to the session bus.
\code std::shared_ptr<DBus::Connection> conn = dispatcher->create_connection(DBus::BusType::SESSION); \endcode

\par Request a name for our application on the bus
Next, we need to request a name that will identify our application on the
session bus.
\code
  if( conn->request_name( "dbuscxx.quickstart_0.server", DBUSCXX_NAME_FLAG_REPLACE_EXISTING ) != DBus::RequestNameResponse::PrimaryOwner )
      return 1;
\endcode

\par Create an object on the bus
Now that our application has a name on the bus we need to add an object with
it's path.
\code std::shared_ptr<DBus::Object> object = conn->create_object("/dbuscxx/quickstart_0", DBus::ThreadForCalling::DispatcherThread); \endcode

\par Add a method named \c add to our object
We will now create a method named \c add for our object. The functionality
of the method will be provided by the function we declared above also named
\c add(). We must add this to an interface, and the D-Bus specification
required that interface names must contain at least one \b . (period)
character so we will use the interface name \c "dbuscxx.Quickstart".

\par
Note that our object's method named \c add has nothing to do with C++
class methods. In essence we are creating virtual objects on the D-Bus
and can choose to provide their functionality with either C++ class methods
and/or plain functions.
\code object->create_method<double(double,double)>("dbuscxx.Quickstart", "add", sigc::ptr_fun(add) ); \endcode

\par Wait for incoming calls
Finally, we need to wait for incoming calls. We will sleep for 10 seconds in
our \c main() function and the calls will be handled in the dispatcher's
threads.
\code
  std::this_thread::sleep_for(std::chrono::seconds(10));
  
  return 0;
}
\endcode

<b>Continue On:</b> \ref quick_start_client_0

<b>Go Back:</b> \ref quick_start
