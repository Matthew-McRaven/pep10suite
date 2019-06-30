#ifndef MACROISAASSEMBLER_H
#define MACROISAASSEMBLER_H

#include <QString>
#include <QtCore>
#include <QObject>

#include "ngraph.h"
#include "macromodules.h"
#include "macropreprocessor.h"


class MacroTokenizer;
class MacroRegistry;
/*
 * The MacroISAAssembler coordinates the various phases of assembly in the macro assembler.
 * They are as follows
 *
 * Preprocess:
 *  The preprocessing phase of the assembler detects any macro invocations, and validates
 *  that the macro syntax and semantics are correct.
 * Build:
 *  The resulting AssemblyGraph is converted into a list of codelines.
 *  Just as in Pep9, we use a tokenizer to assemble our programs.
 *  The assembler adapts which methods are available based on the type of the module being parsed.
 * Link:
 *  External symbols (e.g. symbols in the operating system that had a .EXPORT) are pulled into
 *  all modules symbol tables.
 *  Checks for multiply defined / indefined symbols in module table.
 *  The address of each code line is calculated.
 * Annotate:
 *  In Pep9, the detection of trace tags was "baked in" to the build step.
 *  By having a separate step where stack trace symbol information is collected,
 *  we can improve the code quality used in this section. This code was
 *  becoming difficult to maintain, and has lead to several open issues.
 * Validate:
 *  After we have finished assembling, make sure that the final program makes sense.
 *  e.g. Is the program length longer than 2^16 bytes? If so, it will fail here.
 *  After validation is complete, we can construct an executable assembly program,
 *  and finally return to the user.
 *
 * All errors generated by nested modules MUST be propogated to the root module, preferably on
 * the macro invocation that started the path to the error.
 */
class MacroISAAssembler
{
public:
    MacroISAAssembler(const MacroRegistry* registry);
    ~MacroISAAssembler();
    void* assembleUserProgram(QString input);
    void* assembleOperatingSystem(QString input);
    bool validateMacro(QString input);

private:
    // All instances of a macro will include the same macros, so
    // it is safe to use a ModulePrototype. This means an expression of the form %DECO$1 is
    // an invalid macro, and must be flagged as such.
    // Detect any macros used in the module.
    PreprocessorResult preprocess();


    // Use module instances, because macros may have different arguments. If two instances
    // share the same macro and arguments, they are the same macro.
    // Replace macro symbols $1 $2 $3 before they reach the normal assembly function
    // Generate the code line objects, detect symbols, but do not assign addresses or sizes.
    void assembleProgram(ModuleInstance& module);
    // Calculate real address of each code line,
    void link(ModuleInstance& module);
    // Annotate functions that modify the stack.
    void annotate(ModuleInstance& module);
    // Perform any last-minute sanity checks before returning AssemblyProgram.
    // For example, that the program object code can fit in main memory.
    void validate(ModuleInstance& module);

    MacroTokenizer* tokenizer;
    const MacroRegistry* registry;
    MacroPreprocessor* processor;
    ModuleAssemblyGraph graph;

};

#endif // MACROISAASSEMBLER_H
