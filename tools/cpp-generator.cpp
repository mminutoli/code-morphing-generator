#include <AlternativesDriver.hpp>

#include <fstream>

int main(int argc, char *argv[])
{
  if (argc != 2) return -1;

  std::ofstream a("a.def");
  std::ofstream b("b.def");
  std::ofstream c("c.def");
  std::ofstream d("d.def");
  std::ofstream e("e.def");

  IA::AlternativesDriver driver(a, b, c, d, e);
  driver.parse(argv[1]);
  driver.buildTMPTable();
  return 0;
}


















