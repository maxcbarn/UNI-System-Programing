#ifndef MACRO_PROCESSOR_HPP
#define MACRO_PROCESSOR_HPP

#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

struct MacroDefinition {
    string              name;
    vector<string>      params;    
    vector<string>      body;       
};

class MacroProcessor {
public:
    string Process(const string& inputFilepath);

    vector<string> errors;

private:
    vector<unordered_map<string, MacroDefinition>>
        macroScopes;

    vector<string> ProcessLines(
        const vector<string>& lines,
        int nestLevel = 0
    );

    vector<string> ExpandMacro(
        const MacroDefinition& macro,
        const vector<string>& actualParams,
        int nestLevel
    );

    const MacroDefinition* FindMacro(
        const string& name
    ) const;

    string Trim(const string& s) const;
    string ToUpper(const string& s) const;
    string StripComment(const string& s) const;
    vector<string> SplitParams(const string& s) const;
    bool IsMacroCall(const string& word) const;

    string MakeOutputPath(const string& inputPath) const;
};

#endif 