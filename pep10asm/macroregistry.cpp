#include "macroregistry.h"
#include "pep.h"

MacroRegistry::MacroRegistry(QString registryName) : macroList()
{
    QString builtinDir = ":/help-asm/macros/builtin";
    registerCoreMacros(QDir(builtinDir));
}

MacroRegistry::~MacroRegistry()
{

}

bool MacroRegistry::hasMacro(QString macroName) const
{
    auto equalIg = [&macroName](const QSharedPointer<Macro>& other){
        return other->macroName.compare(macroName, Qt::CaseInsensitive) == 0;
    };
    auto item = std::find_if(macroList.cbegin(), macroList.cend(), equalIg);
    return item != macroList.cend();
}

QSharedPointer<const Macro> MacroRegistry::getMacro(QString macroName) const
{
    auto equalIg = [&macroName](const QSharedPointer<Macro>& other){
        return other->macroName.compare(macroName, Qt::CaseInsensitive) == 0;
    };
    auto item = std::find_if(macroList.cbegin(), macroList.cend(), equalIg);
    if(item != macroList.end()) {
        return item.value();
    }
    return nullptr;
}

bool MacroRegistry::registerCoreMacro(QString macroName, QString macroText)
{
    return registerMacro(macroName, macroText, MacroType::CoreMacro);
}

bool MacroRegistry::registerSystemCall(QString macroName, QString macroText)
{
    return registerMacro(macroName, macroText, MacroType::SystemMacro);
}

bool MacroRegistry::registerCustomMacro(QString macroName, QString macroText)
{
    return registerMacro(macroName, macroText, MacroType::UserMacro);
}

std::tuple<bool, QString, quint16> MacroRegistry::macroDefinition(QString macroText)
{
    /*
     * A macro file must begin with with name of the macro, followed by an arbitrary number of spaces
     * followed by an integer in [0,16] specifying the number of arguments the macro takes.
     *
     * Neither comments nor whitespace may occur before this definition line.
     *
     * Below are valid examples:
     * @DECI 2
     * @UNOP 0
     *
     * Below are invalid examples, with comments descrbing why.
     *
     * Whitepace preceding macro definiton:
     *      @DECI 2
     * Space between macro name and macro symbol @.
     * @ deci 2
     *
     * Line ends in a comment
     * @deci 2 ;My comment
     *
     */
    QString firstLine = macroText.mid(0, macroText.indexOf("\n"));
    QStringList lineList = firstLine.split(QRegExp("\\s+"), QString::SkipEmptyParts);
    if(lineList.size() != 2) {
        // Malformed macro declaration.
        return {false, "", 0};
    }

    QString name = lineList[0];
    if(name.indexOf("@") != 0) {
        // Name must begin with @
        return {false, "", 0};
    }
    // Remove the @ from the name.
    name.remove(0, 1);

    bool ok;
    quint8 argCount = lineList[1].toUShort(&ok);
    if(!ok) {
        // Arg count was not an integer.
        return {false, "", 0};
    }
    return {true, name, argCount};
}

bool MacroRegistry::registerCoreMacros(QDir dir)
{
    bool retVal = true;
    for(auto item : dir.entryInfoList()) {
        if(item.isDir()) {
            retVal &= registerCoreMacros(QDir(item.filePath()));
        }
        else {
            // Must use macro assembler to validate contents of macros.
            QString macroName = item.baseName();
            QString macroText = Pep::resToString(item.filePath(), false);
            retVal &= registerCoreMacro(macroName, macroText);
        }
    }
    return retVal;
}

bool MacroRegistry::registerMacro(QString macroName, QString macroText, MacroType type)
{
    if(macroList.contains(macroName)) {
        return false;
    }
    auto [success, name, argCount] = macroDefinition(macroText);
    if(!success) {
        return false;
    }
    if(macroName.compare(name, Qt::CaseInsensitive) != 0) {
        return false;
    }
    QSharedPointer<Macro> newMacro = QSharedPointer<Macro>::create();
    newMacro->macroName = macroName;
    newMacro->macroText = macroText;
    newMacro->argCount = argCount;
    newMacro->type = type;
    macroList.insert(macroName, newMacro);
    return true;
}
