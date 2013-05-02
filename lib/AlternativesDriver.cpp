#include <AlternativesDriver.hpp>

#include <cassert>
#include <fstream>


using namespace IA;


AlternativesDriver::AlternativesDriver(
    std::ostream & _osBuildAlternatives,
    std::ostream & _osEnumInstruction,
    std::ostream & _osSetAlternativeTable,
    std::ostream & _osHandleInstructionTable,
    std::ostream & _osReverseMapTable)
    : scanner(NULL), parser(NULL),
      osBuildAlternatives(_osBuildAlternatives),
      osEnumInstruction(_osEnumInstruction),
      osSetAlternativeTable(_osSetAlternativeTable),
      osHandleInstructionTable(_osHandleInstructionTable),
      osReverseMapTable(_osReverseMapTable),
      seqNumber(0), currentInst(""), in1(""), in2(""),
      regRegAlternatives()
{}


AlternativesDriver::~AlternativesDriver()
{
  delete(scanner);
  delete(parser);
}


void
AlternativesDriver::parse(char const * const filename)
{
  assert(filename != NULL);
  std::ifstream iFile(filename);
  if (!iFile.good())
  {
    std::cerr << "Error opening file" << std::endl;
    exit(-1);
  }

  scanner = new AlternativesScanner(&iFile);
  parser = new AlternativesParser(*scanner, *(this));

  if (parser->parse() == -1)
    std::cerr << "Parse failed" << std::endl;
}


void
AlternativesDriver::emitInstruction(std::string const & r3,
                                    std::string const & i,
                                    std::string const & r1,
                                    std::string const & r2)
{
  if (i == "and") {
    osBuildAlternatives
        << "BinaryOperator * "
        << r3 << " = BinaryOperator::Create(Instruction::And, "
        << r1 << ", " << r2 << ", \"\", BB);" << std::endl;
  } else if (i == "or") {
    osBuildAlternatives
        << "BinaryOperator * "
        << r3 << " = BinaryOperator::Create(Instruction::Or, "
        << r1 << ", " << r2 << ", \"\", BB);" << std::endl;
  } else if (i == "xor") {
    osBuildAlternatives
        << "BinaryOperator * "
        << r3 << " = BinaryOperator::Create(Instruction::Xor, "
        << r1 << ", " << r2 << ", \"\", BB);" << std::endl;
  } else if (i == "not") {
    osBuildAlternatives
        << "IntegerType * "<< r1 << "type = IntegerType::get("
        << r1 << "->getContext(),"
        << r1 << "->getType()->getIntegerBitWidth());\n"
        << "Constant * allOnes"<< r1 << " = ConstantInt::get(" << r1 << "type,"
        << r1 << "type->getMask());\n"
        << "BinaryOperator * " << r3
        << " = BinaryOperator::Create(Instruction::Xor, "
        << r1 << ", allOnes" << r1 << ", \"\", BB);"
        << std::endl;
  } else {
    osBuildAlternatives << "Error.MissingInstruction;" << std::endl;
  }
}


void
AlternativesDriver::openAlternativeBuilder()
{
  osBuildAlternatives
      << "BasicBlock * build" << currentInst << "RegReg"
      << ++seqNumber << "(Value * " << in1 << ", Value * "<< in2 << ") {\n"
      << "BasicBlock * BB = BasicBlock::Create(" << in1 << "->getContext());"
      << std::endl;
}


void
AlternativesDriver::closeAlternativeBuilder()
{
  osBuildAlternatives << "return BB;\n}\n\n" << std::endl;
}


void
AlternativesDriver::setInputs(std::string & i1, std::string & i2)
{
  in1 = i1;
  in2 = i2;
}


void
AlternativesDriver::buildAlternativesSpecialization()
{
  osBuildAlternatives
      << "template <>\n"
      << "std::vector<llvm::BasicBlock *> buildAlternatives<"
      << currentInst << ">(llvm::Instruction & I)"
      "{\nstd::vector<BasicBlock *> alternatives;\n\n"
      "Value * firstOperand = I.getOperand(0);\n"
      "Value * secondOperand = I.getOperand(1);\n"
      << "BasicBlock * BB;\n"
      << std::endl;

  for (int i = 1; i <= seqNumber; ++i)
  {
    osBuildAlternatives
        << "BB = build" << currentInst << "RegReg" << i
        << "(firstOperand, secondOperand);\n"
        << "alternatives.push_back(BB);\n"
        << std::endl;
  }

  osBuildAlternatives << "return alternatives;\n}\n" << std::endl;     
}


void
AlternativesDriver::currentInstruction(std::string const & i)
{
  seqNumber = 0;
  if (i == "or")
    currentInst = "Or";
  else if (i == "xor")
    currentInst = "Xor";
  else if (i == "and")
    currentInst = "And";
  else
    currentInst = "<UnKnownInst>";
}


void
AlternativesDriver::addRegRegAlternative()
{
  regRegAlternatives.push_back(std::make_pair(currentInst, seqNumber));
}


void
AlternativesDriver::buildTMPTable()
{
  typedef std::vector< std::pair<std::string, int> >::const_iterator const_iterator;
  int i = 0;
  for (const_iterator itr = regRegAlternatives.begin(),
                      end = regRegAlternatives.end();
       itr != end; ++itr, ++i)
  {
    osEnumInstruction << itr->first << " = " << i << "," << std::endl;

    osSetAlternativeTable
        << "CMP_SET_ALTERNATIVE_NUMBER("
        << itr->first << ", " << itr->second << ");"
        << std::endl;

    osHandleInstructionTable
        << "HANDLE_ISTRUCTION_TYPE("
        << itr->first << ");"
        << std::endl;

    osReverseMapTable
        << "CHECK_INST("
        << itr->first << ");"
        << std::endl;
  }
}
