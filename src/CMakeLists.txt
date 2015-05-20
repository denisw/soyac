set (SOYAC_SOURCES
  analysis/BasicAnalyzer.cpp
  analysis/ControlFlowAnalyzer.cpp
  analysis/FunctionGroup.cpp
  analysis/NavigatingVisitor.cpp
  analysis/SymbolTable.cpp
  ast/ArrayCreationExpression.cpp
  ast/ArrayType.cpp
  ast/AssignmentExpression.cpp
  ast/Block.cpp
  ast/BooleanLiteral.cpp
  ast/BooleanType.cpp
  ast/BuiltInType.cpp
  ast/CallExpression.cpp
  ast/CastExpression.cpp
  ast/CharacterLiteral.cpp
  ast/CharacterType.cpp
  ast/ClassType.cpp
  ast/CombinedRelationalExpression.cpp
  ast/CompoundAssignmentExpression.cpp
  ast/Constructor.cpp
  ast/ConstructorInitializer.cpp
  ast/DeclarationBlock.cpp
  ast/DeclarationStatement.cpp
  ast/DeclaredEntity.cpp
  ast/DoStatement.cpp
  ast/Expression.cpp
  ast/EntityImport.cpp
  ast/EnumConstant.cpp
  ast/EnumType.cpp
  ast/EnumValueExpression.cpp
  ast/ExpressionStatement.cpp
  ast/FloatingPointLiteral.cpp
  ast/FloatingPointType.cpp
  ast/FloatingPointValue.cpp
  ast/ForStatement.cpp
  ast/Function.cpp
  ast/FunctionExpression.cpp
  ast/FunctionGroupImport.cpp
  ast/FunctionParameterExpression.cpp
  ast/FunctionParameter.cpp
  ast/FunctionType.cpp
  ast/IfStatement.cpp
  ast/Import.cpp
  ast/InstanceFunctionExpression.cpp
  ast/InstanceVariableExpression.cpp
  ast/IntegerLiteral.cpp
  ast/IntegerType.cpp
  ast/IntegerValue.cpp
  ast/Location.cpp
  ast/LogicalExpression.cpp
  ast/LogicalNotExpression.cpp
  ast/Module.cpp
  ast/ModuleExpression.cpp
  ast/ModuleImport.cpp
  ast/Name.cpp
  ast/NamedEntity.cpp
  ast/Node.cpp
  ast/ObjectCreationExpression.cpp
  ast/Property.cpp
  ast/PropertyGetAccessor.cpp
  ast/PropertySetAccessor.cpp
  ast/ReturnStatement.cpp
  ast/Statement.cpp
  ast/StatementWithBody.cpp
  ast/StructType.cpp
  ast/Type.cpp
  ast/ThisExpression.cpp
  ast/TypeExpression.cpp
  ast/UnknownArrayType.cpp
  ast/UnknownFunctionType.cpp
  ast/UnknownType.cpp
  ast/UnresolvedBaseConstructorInitializer.cpp
  ast/UnresolvedBinaryExpression.cpp
  ast/UnresolvedConstructorInitializer.cpp
  ast/UnresolvedDefaultConstructorInitializer.cpp
  ast/UnresolvedElementAccessExpression.cpp
  ast/UnresolvedEntityImport.cpp
  ast/UnresolvedFunctionExpression.cpp
  ast/UnresolvedMemberAccessExpression.cpp
  ast/UnresolvedModuleImport.cpp
  ast/UnresolvedObjectCreationExpression.cpp
  ast/UnresolvedInstanceFunctionExpression.cpp
  ast/UnresolvedInstancePropertyExpression.cpp
  ast/UnresolvedSimpleNameExpression.cpp
  ast/UnresolvedThisExpression.cpp
  ast/UserDefinedType.cpp
  ast/Variable.cpp
  ast/VariableExpression.cpp 
  ast/VoidType.cpp
  ast/WhileStatement.cpp 
  codegen/CodeGenerator.cpp
  codegen/LLValueExpression.cpp
  common/PassResult.cpp
  common/PassResultBuilder.cpp
  common/Problem.cpp
  driver/config.cpp
  driver/FileProcessor.cpp
  driver/main.cpp
  driver/link.cpp
  driver/ProblemReport.cpp
  parser/Lexer.cpp 
  parser/ParserDriver.cpp 
)


# Add a target for running bison 

add_custom_target (soyaparser echo "Generating parser.cpp from soya.y")

add_custom_command (
  SOURCE   parser/soya.y
  COMMAND  bison 
  ARGS     -y ${soyac_SOURCE_DIR}/src/parser/soya.y
           -o ${soyac_BINARY_DIR}/src/parser/parser.cpp
  TARGET   soyaparser
  OUTPUTS  ${soyac_BINARY_DIR}/src/parser/parser.cpp)

set (SOYAC_SOURCES ${soyac_BINARY_DIR}/src/parser/parser.cpp ${SOYAC_SOURCES})
set_source_files_properties (${soyac_BINARY_DIR}/src/parser/parser.cpp GENERATED)
include_directories (${soyac_BINARY_DIR})


add_executable (soyac ${SOYAC_SOURCES})
target_link_libraries (soyac 
  boost_regex
  popt
  sigc-2.0
  LLVMCore
  LLVMSupport
  LLVMSystem
  pthread
  dl
  m
  elf
)

install (TARGETS soyac DESTINATION bin)
