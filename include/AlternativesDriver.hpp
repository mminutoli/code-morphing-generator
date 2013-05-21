#ifndef _ALTERNATIVESDRIVER_H_
#define _ALTERNATIVESDRIVER_H_

#include <AlternativesScanner.hpp>
#include <Parser.tab.hh>

#include <ostream>
#include <map>


namespace IA {

class AlternativesDriver
{
 public:
  AlternativesDriver(
      std::ostream & _osBuildAlternatives,
      std::ostream & _osEnumInstruction,
      std::ostream & _osSetAlternativeTable,
      std::ostream & _osHandleInstructionTable,
      std::ostream & _osReverseMapTable);
  virtual ~AlternativesDriver();

  void parse(char const * const filename);

  void emitInstruction(std::string const & r3,
                       std::string const & i,
                       std::string const & r1,
                       std::string const & r2 = "");

  void openRegRegAlternativeBuilder();
  void openRegConstAlternativeBuilder();
  void closeAlternativeBuilder();

  void setInputs(std::string & i1, std::string & i2);

  void buildAlternativesSpecialization();

  void currentInstruction(std::string const & i);

  bool isRegReg() { return regreg; }
  void regReg() { regreg = true; }
  void regConst() { regreg = false; }

  void addAlternative();

  void buildTMPTable();

  unsigned int handleRandomArgs(std::string const & r);

 private:
  IA::AlternativesScanner * scanner;
  IA::AlternativesParser * parser;

  std::ostream & osBuildAlternatives;
  std::ostream & osEnumInstruction;
  std::ostream & osSetAlternativeTable;
  std::ostream & osHandleInstructionTable;
  std::ostream & osReverseMapTable;

  std::string currentInst;
  std::string in1;
  std::string in2;
  int seqNumber;
  bool regreg;
  std::map<std::string, std::pair<int, int> > alternatives;
  std::map<std::string, unsigned int> randoms;
};

}

#endif /* _ALTERNATIVESDRIVER_H_ */
