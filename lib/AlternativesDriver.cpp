#include <AlternativesDriver.hpp>

#include <cassert>
#include <fstream>
#include <sstream>


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
      regreg(false),
      alternatives(),
      randoms()
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
  } else if (i == "lsh") {
    osBuildAlternatives
        << "BinaryOperator * "
        << r3 << " = BinaryOperator::Create(Instruction::LShr, "
        << r1 << ", " << r2 << ", \"\", BB);" << std::endl;
  } else if (i == "rsh") {
    osBuildAlternatives
        << "BinaryOperator * "
        << r3 << " = BinaryOperator::Create(Instruction::Shr, "
        << r1 << ", " << r2 << ", \"\", BB);" << std::endl;
  } else if (i == "add") {
    osBuildAlternatives
        << "BinaryOperator * "
        << r3 << " = BinaryOperator::Create(Instruction::Add, "
        << r1 << ", " << r2 << ", \"\", BB);" << std::endl;
  } else if (i == "sub") {
    osBuildAlternatives
        << "BinaryOperator * "
        << r3 << " = BinaryOperator::Create(Instruction::Sub, "
        << r1 << ", " << r2 << ", \"\", BB);" << std::endl;
  } else if (i == "mul") {
    osBuildAlternatives
        << "BinaryOperator * "
        << r3 << " = BinaryOperator::Create(Instruction::Mul, "
        << r1 << ", " << r2 << ", \"\", BB);" << std::endl;
  } else if (i == "not") {
    osBuildAlternatives
        << "Constant * allOnes" << r1 << ";\n"
        << "if (isa<VectorType>(" << r1 << "->getType())) {\n"
        << "VectorType * " << r1 << "type = VectorType::getInteger(cast<VectorType>("
        << r1 << "->getType()));\n"
        << "unsigned bitNum = " << r1 << "type->getBitWidth() / "
        << r1 << "type->getNumElements();\n"
        << "IntegerType * " << r1 << "ElemType = IntegerType::get("
        << r1 << "->getContext(), bitNum);\n"
        << "Constant * elem = ConstantInt::get(" << r1 << "ElemType, "
        << r1 << "ElemType->getMask());\n"
        "allOnes" << r1 << " = ConstantVector::getSplat("
        << r1 << "type->getNumElements(), elem);\n"
        << "} else {\n"
        << "IntegerType * "<< r1 << "type = IntegerType::get("
        << r1 << "->getContext(),"
        << r1 << "->getType()->getIntegerBitWidth());\n"
        << "allOnes"<< r1 << " = ConstantInt::get(" << r1 << "type,"
        << r1 << "type->getMask());\n}\n"
        << "BinaryOperator * " << r3
        << " = BinaryOperator::Create(Instruction::Xor, "
        << r1 << ", allOnes" << r1 << ", \"\", BB);"
        << std::endl;
  } else if (i == "neg") {
    osBuildAlternatives
        << "Constant * allOnes" << r1 << ";\n"
        << "if (isa<VectorType>(" << r1 << "->getType())) {\n"
        << "VectorType * " << r1 << "type = VectorType::getInteger(cast<VectorType>("
        << r1 << "->getType()));\n"
        << "unsigned bitNum = " << r1 << "type->getBitWidth() / "
        << r1 << "type->getNumElements();\n"
        << "IntegerType * " << r1 << "ElemType = IntegerType::get("
        << r1 << "->getContext(), bitNum);\n"
        << "Constant * elem = ConstantInt::get(" << r1 << "ElemType, "
        << r1 << "ElemType->getMask());\n"
        "allOnes" << r1 << " = ConstantVector::getSplat("
        << r1 << "type->getNumElements(), elem);\n"
        << "} else {\n"
        << "IntegerType * "<< r1 << "type = IntegerType::get("
        << r1 << "->getContext(),"
        << r1 << "->getType()->getIntegerBitWidth());\n"
        << "allOnes"<< r1 << " = ConstantInt::get(" << r1 << "type,"
        << r1 << "type->getMask());\n}\n"
        << "BinaryOperator * " << r1 << "xor"
        << " = BinaryOperator::Create(Instruction::Xor, "
        << r1 << ", allOnes" << r1 << ", \"\", BB);"
        << "Constant * one;\n"
        << "if (isa<VectorType>(" << r1 << "->getType())) {\n"
        << "VectorType * " << r1 << "type = VectorType::getInteger(cast<VectorType>("
        << r1 << "->getType()));\n"
        << "unsigned bitNum = " << r1 << "type->getBitWidth() / "
        << r1 << "type->getNumElements();\n"
        << "IntegerType * " << r1 << "ElemType = IntegerType::get("
        << r1 << "->getContext(), bitNum);\n"
        << "Constant * elem = ConstantInt::get(" << r1 << "ElemType, 1);\n"
        "one" << r1 << " = ConstantVector::getSplat("
        << r1 << "type->getNumElements(), elem);\n"
        << "} else {\n"
        << "IntegerType * "<< r1 << "type = IntegerType::get("
        << r1 << "->getContext(),"
        << r1 << "->getType()->getIntegerBitWidth());\n"
        << "one"<< r1 << " = ConstantInt::get(" << r1 << "type, 1);\n}\n"
        << std::endl;
  } else if (i == "load") {
    osBuildAlternatives
        << "LoadInst * "
        << r3 << " = new LoadInst("
        << r1 << ", \"\", BB);" << std::endl;
  } else if (i == "store") {
    osBuildAlternatives
        << "StoreInst * "
        << r3 << " = new StoreInst("
        << r1 << ", " << r2 << ", \"\", BB);" << std::endl;
  } else {
    osBuildAlternatives << "Error.MissingInstruction;" << std::endl;
  }
}


void
AlternativesDriver::openRegRegAlternativeBuilder()
{
  osBuildAlternatives
      << "BasicBlock * build" << currentInst << "RegReg"
      << ++seqNumber << "(Value * " << in1 << ", Value * "<< in2 << ") {\n"
      << "BasicBlock * BB = BasicBlock::Create(" << in1 << "->getContext());"
      << std::endl;
}


void
AlternativesDriver::openRegConstAlternativeBuilder()
{
  osBuildAlternatives
      << "BasicBlock * build" << currentInst << "RegConst"
      << ++seqNumber << "(Value * " << in1 << ", Value * "<< in2 << ") {\n"
      << "BasicBlock * BB = BasicBlock::Create(" << in1 << "->getContext());"
      << std::endl;
}


void
AlternativesDriver::closeAlternativeBuilder()
{
  osBuildAlternatives << "return BB;\n}\n\n" << std::endl;
  randoms.clear();
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
  typedef std::map<std::string, std::pair<int, int> >::const_iterator const_iterator;
  for (const_iterator itr = alternatives.begin(),
                      end = alternatives.end();
       itr != end; ++itr)
  {
    osBuildAlternatives
        << "template <>\n"
        << "std::vector<llvm::BasicBlock *> buildAlternatives<"
        << itr->first << ">(llvm::Instruction & I)"
        "{\nstd::vector<BasicBlock *> alternatives;\n\n"
        "Value * firstOperand = I.getOperand(0);\n"
        "Value * secondOperand = I.getOperand(1);\n"
        << "BasicBlock * BB;\n"
        << std::endl;

    osBuildAlternatives
        << "if (isa<Constant>(firstOperand)) {\n";
    for (int i = 1; i <= itr->second.second; ++i)
    {
      osBuildAlternatives
          << "BB = build" << itr->first << "RegConst" << i
          << "(secondOperand, firstOperand);\n"
          << "alternatives.push_back(BB);\n";
    }
    osBuildAlternatives
        << "} else if (isa<Constant>(secondOperand)) {\n";
    for (int i = 1; i <= itr->second.second; ++i)
    {
      osBuildAlternatives
          << "BB = build" << itr->first << "RegConst" << i
          << "(secondOperand, firstOperand);\n"
          << "alternatives.push_back(BB);\n";
    }
    osBuildAlternatives
        << "} else {\n";
    for (int i = 1; i <= itr->second.first; ++i)
    {
      osBuildAlternatives
          << "BB = build" << itr->first << "RegReg" << i
          << "(firstOperand, secondOperand);\n"
          << "alternatives.push_back(BB);\n";
    }
    osBuildAlternatives << "}" << std::endl;
    osBuildAlternatives << "return alternatives;\n}\n" << std::endl;
  }
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
AlternativesDriver::addAlternative()
{
  if (regreg)
    alternatives[currentInst].first = seqNumber;
  else
    alternatives[currentInst].second = seqNumber;
}


void
AlternativesDriver::buildTMPTable()
{
  typedef std::map<std::string, std::pair<int, int> >::const_iterator const_iterator;
  int i = 0;
  for (const_iterator itr = alternatives.begin(),
                      end = alternatives.end();
       itr != end; ++itr, ++i)
  {
    osEnumInstruction << itr->first << " = " << i << "," << std::endl;

    osSetAlternativeTable
        << "CMP_SET_ALTERNATIVE_NUMBER("
        << itr->first << ", " << itr->second.first << ", RegReg);"
        << std::endl;
    osSetAlternativeTable
        << "CMP_SET_ALTERNATIVE_NUMBER("
        << itr->first << ", " << itr->second.second << ", RegConst);"
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


unsigned int
AlternativesDriver::handleRandomArgs(std::string const & r)
{
  if (randoms.find(r) == randoms.end())
  {
    unsigned int result = std::rand();
    randoms[r] = result;
    osBuildAlternatives
        << "IntegerType * "<< r << "type = IntegerType::get("
        << "BB->getContext(), 32);\n"
        << "ConstantInt * " << r << " = ConstantInt::get(" << r << "type, "
        << result << ");\n"
        << std::endl;
    return result;
  } else {
    return randoms[r];
  }
}

std::string *
AlternativesDriver::handleGenTimeConstant(unsigned int c)
{
  static unsigned int idx = 0;

  osBuildAlternatives
      << "IntegerType * constE" << idx << "type = IntegerType::get("
      << "BB->getContext(), 32);\n"
      << "ConstantInt * constE" << idx << " = ConstantInt::get(constE" << idx
      << "type, " << c << ");\n"
      << std::endl;

  std::ostringstream name;
  name << "constE" << idx++;
  return new std::string(name.str());
}
