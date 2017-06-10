#include <redispp.h>
#include <stdio.h>
#include <string>
#ifdef _WIN32
#include <Windows.h>
#endif

using namespace redispp;
using namespace std;

const char *TEST_PORT = "0";
const char *TEST_HOST = "127.0.0.1";
const char *TEST_UNIX_DOMAIN_SOCKET = "/tmp/redis.sock";

int main(int argc, char *argv[]) {
#ifdef _WIN32
  WSADATA wsaData;
  WORD version;
  version = MAKEWORD(2, 0);
  WSAStartup(version, &wsaData);
#endif

#ifdef UNIX_DOMAIN_SOCKET
  Connection conn(TEST_UNIX_DOMAIN_SOCKET, "");
#else
  Connection conn(TEST_HOST, TEST_PORT, "");
#endif

  conn.set("x", "a");
  conn.set("y", "b");

  Transaction trans(&conn);
  VoidReply one = conn.set("x", "1");
  VoidReply two = conn.set("y", "21");
  StringReply resOne = conn.get("x");
  trans.commit();

  std::string res1 = resOne;
  std::string res2 = conn.get("x");
  if (res2 != res1)
    std::cout << "did not match: " << res1 << " " << res2 << std::endl;
  else
    std::cout << "they match: " << res1 << " " << res2 << std::endl;
  return 0;
}
