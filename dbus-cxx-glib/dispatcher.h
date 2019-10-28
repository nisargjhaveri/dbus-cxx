/***************************************************************************
 *   Copyright (C) 2007,2008,2009,2010 by Rick L. Vinyard, Jr.             *
 *   rvinyard@cs.nmsu.edu                                                  *
 *                                                                         *
 *   This file is part of the dbus-cxx library.                            *
 *                                                                         *
 *   The dbus-cxx library is free software; you can redistribute it and/or *
 *   modify it under the terms of the GNU General Public License           *
 *   version 3 as published by the Free Software Foundation.               *
 *                                                                         *
 *   The dbus-cxx library is distributed in the hope that it will be       *
 *   useful, but WITHOUT ANY WARRANTY; without even the implied warranty   *
 *   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU   *
 *   General Public License for more details.                              *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this software. If not see <http://www.gnu.org/licenses/>.  *
 ***************************************************************************/
#include <dbus-cxx/dispatcher.h>

#ifndef DBUSCXX_GLIB_DISPATCHER_H
#define DBUSCXX_GLIB_DISPATCHER_H

namespace DBus
{
  namespace Glib {

    /**
     * Reimplements DBus::Dispatcher to conduct all dispatching in
     * the provided Glib MainContext if one is provided or in the
     * default context otherwise.
     *
     * This class still creates multiple threads for handling I/O file
     * descriptors and for controlling dispatching. However, the
     * actual dispatching will occur in the provided context' main loop.
     *
     * @ingroup core
     *
     * @author Rick L Vinyard Jr <rvinyard@cs.nmsu.edu>
     */
    class Dispatcher : public ::DBus::Dispatcher
    {
      public:
        static std::shared_ptr<Dispatcher> create( bool is_running=true );

        static std::shared_ptr<Dispatcher> create( bool is_running, const ::Glib::RefPtr< ::Glib::MainContext >& context );

      private: 
        Dispatcher(bool is_running=true);
        
        Dispatcher(bool is_running, const ::Glib::RefPtr< ::Glib::MainContext >& context );

        virtual ~Dispatcher();

      protected:

        ::Glib::Dispatcher m_glibmm_dispatcher;

        virtual void dispatch_thread_main();

        void on_glibmm_dispatch();
    };

  }
}

#endif
