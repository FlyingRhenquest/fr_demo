/**
 * Make sure the Socket Server works
 *
 * Copyright 2011 Bruce Ide
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "socket_server.h"
#include <iostream>
#include <sstream>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <ext/stdio_filebuf.h>
#include <cppunit/extensions/HelperMacros.h>

class SocketServerTest : public CppUnit::TestFixture {

  CPPUNIT_TEST_SUITE(SocketServerTest);
  CPPUNIT_TEST(testEcho);
  CPPUNIT_TEST_SUITE_END();

  class EchoServer {
    int fdes;
    SocketServer<EchoServer> *owner;
  public:
    EchoServer(SocketServer<EchoServer> *owner, int fdes) : owner(owner), fdes(fdes)
    {
    }

    EchoServer(EchoServer &cp) : owner(cp.owner), fdes(cp.fdes) 
    {      
    }

    void operator()()
    {
      // Create some streams from the file descriptor (This probably
      // won't work very well if you're not on g++)
      __gnu_cxx::stdio_filebuf<char> buf_in(fdes, std::ios_base::in, 1);
      __gnu_cxx::stdio_filebuf<char> buf_out(fdes, std::ios_base::out, 1);
      std::istream stream_in(&buf_in);
      std::ostream stream_out(&buf_out);
      std::string buffer;
      while(stream_in.good() && stream_out.good()) {
        getline(stream_in, buffer);
        stream_out << buffer << std::endl;
      }
      close(fdes);
    }
  };

public:

  void testEcho()
  {
    SocketServer<EchoServer> server(12345);
    int retval;
    std::string data;
    boost::thread *serverThread = server.start();
    int sock_conn = socket(AF_INET, SOCK_STREAM, 0);
    CPPUNIT_ASSERT(0 < sock_conn);
    while (!server.ready()) {
      boost::this_thread::sleep(boost::posix_time::seconds(1));
    }

    sockaddr_in addr;
    inet_pton(AF_INET, "127.0.0.1", &addr);
    addr.sin_port = htons(12345);
    addr.sin_family = AF_INET;

    retval = connect(sock_conn, (sockaddr *) &addr, sizeof(sockaddr_in));
    
    CPPUNIT_ASSERT(0 == retval);
    __gnu_cxx::stdio_filebuf<char> buf_in(sock_conn, std::ios_base::in, 1);
    __gnu_cxx::stdio_filebuf<char> buf_out(sock_conn, std::ios_base::out, 1);
    std::istream stream_in(&buf_in);
    std::ostream stream_out(&buf_out);    

    std::string foo("Foo!");
    stream_out << foo << std::endl;
    getline(stream_in, data);
    CPPUNIT_ASSERT(data == foo);
    server.shutdown();
    close(sock_conn);
    serverThread->join();
  }

};

CPPUNIT_TEST_SUITE_REGISTRATION(SocketServerTest);
