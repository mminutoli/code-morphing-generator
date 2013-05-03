#include <AlternativesDriver.hpp>

#include <fstream>

int main(int argc, char *argv[])
{
  if (argc != 2) return -1;

  std::ofstream BuildAlternatives("BuildAlternatives.def");
  std::ofstream InstructionDecl("InstructionDecl.def");
  std::ofstream InstructionTable("InstructionTable.def");
  std::ofstream HandleInstruction("HandleInstruction.def");
  std::ofstream ReverseMapping("ReverseMapping.def");

  IA::AlternativesDriver driver(BuildAlternatives, InstructionDecl,
                                InstructionTable, HandleInstruction,
                                ReverseMapping);
  driver.parse(argv[1]);
  driver.buildTMPTable();
  return 0;
}


















