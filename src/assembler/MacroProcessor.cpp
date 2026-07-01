#include "assembler/MacroProcessor.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

string MacroProcessor::Trim(const string& s) const {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}


string MacroProcessor::ToUpper(const string& s) const {
    string result = s;
    transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}


string MacroProcessor::StripComment(const string& s) const {
    size_t pos = s.find(';');
    if (pos == string::npos) return s;
    return s.substr(0, pos);
}


vector<string> MacroProcessor::SplitParams(const string& s) const {
    vector<string> result;
    stringstream ss(s);
    string token;
    while (getline(ss, token, ',')) {
        result.push_back(Trim(token));
    }
    return result;
}


bool MacroProcessor::IsMacroCall(const string& word) const {
    return FindMacro(word) != nullptr;
}


const MacroDefinition*MacroProcessor::FindMacro(const string& name) const {
    string upper = ToUpper(name);

    for (
        auto it = macroScopes.rbegin();
        it != macroScopes.rend();
        ++it
    ) {
        auto found = it->find(upper);

        if (found != it->end())
            return &found->second;
    }

    return nullptr;
}


string MacroProcessor::MakeOutputPath(const string& inputPath) const {
    size_t dotPos = inputPath.rfind('.');
    if (dotPos != string::npos)
        return inputPath.substr(0, dotPos) + "_expanded.asm";
    return inputPath + "_expanded.asm";
}


string MacroProcessor::Process(const string& inputFilepath) {
    errors.clear();

    macroScopes.clear();
    macroScopes.emplace_back();

    ifstream inFile(inputFilepath);
    if (!inFile.is_open()) {
        errors.push_back("The file could not be opened: " + inputFilepath);
        cerr << "[MACRO] " << errors.back() << endl;
        return "";
    }

    vector<string> lines;
    string line;
    while (getline(inFile, line))
        lines.push_back(line);
    inFile.close();

    cout << "\n=== MACRO PRE-PROCESSOR ===" << endl;
    cout << "Input file: " << inputFilepath << endl;

    vector<string> expanded = ProcessLines(lines, 0);

    string outputPath = MakeOutputPath(inputFilepath);
    ofstream outFile(outputPath);
    if (!outFile.is_open()) {
        errors.push_back("The output file could not be created" + outputPath);
        cerr << "[MACRO] " << errors.back() << endl;
        return "";
    }

    for (const auto& l : expanded)
        outFile << l << "\n";
    outFile.close();

    cout << "Output file   : " << outputPath << endl;
    cout << "Defined macros  : " << macroScopes.front().size() << endl;
    if (!errors.empty()) {
        cout << "Errors found  :" << endl;
        for (const auto& e : errors)
            cout << "  - " << e << endl;
    }
    cout << "================================\n" << endl;

    return outputPath;
}


vector<string> MacroProcessor::ProcessLines(const vector<string>& lines, int nestLevel) {
    vector<string> output;
    vector<MacroDefinition> defStack;

    for (size_t i = 0; i < lines.size(); ++i) {
        string raw  = lines[i];
        string noComment = StripComment(raw);
        string trimmed   = Trim(noComment);

        if (trimmed.empty()) {
            if (defStack.empty())
                output.push_back(raw);  
            else
                defStack.back().body.push_back(raw);  
            continue;
        }

        istringstream iss(trimmed);
        vector<string> tokens;
        {
            string tok;
            while (iss >> tok)
                tokens.push_back(tok);
        }
        if (tokens.empty()) {
            if (defStack.empty()) output.push_back(raw);
            else defStack.back().body.push_back(raw);
            continue;
        }

        bool hasLabel   = false;
        string labelStr = "";
        size_t cmdIdx   = 0;   
        if (!tokens[0].empty() && tokens[0].back() == ':') {
            hasLabel = true;
            labelStr = tokens[0];
            cmdIdx   = 1;
        }

        auto getRest = [&](size_t fromIdx) -> string {
            string res;
            for (size_t k = fromIdx; k < tokens.size(); ++k) {
                if (k > fromIdx) res += " ";
                res += tokens[k];
            }
            return res;
        };

        bool isEndm = (cmdIdx < tokens.size() && ToUpper(tokens[cmdIdx]) == "ENDM");

        if (!isEndm) {
            bool isMacroDef = false;
            string macroDefName;
            string macroDefParams;

            if (!hasLabel && tokens.size() >= 2 && ToUpper(tokens[1]) == "MACRO") {
                isMacroDef     = true;
                macroDefName   = ToUpper(tokens[0]);
                macroDefParams = getRest(2);
            } else if (hasLabel && cmdIdx < tokens.size() && ToUpper(tokens[cmdIdx]) == "MACRO") {
                isMacroDef     = true;
                macroDefName   = ToUpper(labelStr.substr(0, labelStr.size() - 1));
                macroDefParams = getRest(cmdIdx + 1);
            }

            if (isMacroDef) {
                macroDefParams = Trim(macroDefParams);
                vector<string> params;
                if (!macroDefParams.empty())
                    params = SplitParams(macroDefParams);

                MacroDefinition newDef;
                newDef.name   = macroDefName;
                newDef.params = params;

                defStack.push_back(newDef);

                cout << "[MACRO] Defining macro \"" << macroDefName
                     << "\" (level " << defStack.size() << ")" << endl;
                continue;
            }
        }

        if (isEndm) {
            if (defStack.empty()) {
                errors.push_back("ENDM without corresponding MACRO in the line "
                                 + to_string(i + 1));
                cerr << "[MACRO] " << errors.back() << endl;
                continue;
            }

            MacroDefinition completed = defStack.back();
            defStack.pop_back();

            if (!defStack.empty()) {
                string headerLine = completed.name + " MACRO";
                if (!completed.params.empty()) {
                    headerLine += " ";
                    for (size_t p = 0; p < completed.params.size(); ++p) {
                        if (p) headerLine += ",";
                        headerLine += completed.params[p];
                    }
                }
                defStack.back().body.push_back(headerLine);
                for (const auto& bl : completed.body)
                    defStack.back().body.push_back(bl);
                defStack.back().body.push_back("ENDM");
            } else {
                macroScopes.back()[completed.name] = completed;
                cout << "[MACRO] Macro \"" << completed.name
                     << "\" registered (" << completed.body.size()
                     << " lines on the body)" << endl;
            }
            continue;
        }

        if (!defStack.empty()) {
            defStack.back().body.push_back(raw);
            continue;
        }

        string callName = (cmdIdx < tokens.size()) ? ToUpper(tokens[cmdIdx]) : "";

        if (!callName.empty() && IsMacroCall(callName)) {
            string argPart = getRest(cmdIdx + 1);
            argPart = Trim(argPart);

            vector<string> actualParams;
            if (!argPart.empty())
                actualParams = SplitParams(argPart);

            const MacroDefinition*macroDef = FindMacro(callName);

            if (!macroDef) {
                errors.push_back(
                    "Macro \"" +
                    callName +
                    "\" not defined"
                );
                continue;
            }

            if (hasLabel)
                output.push_back(labelStr + "  ; label preserved");

            cout << "[MACRO] Expanding call from \"" << callName
                 << "\" (level " << nestLevel + 1 << ")" << endl;

            vector<string> expanded = ExpandMacro(*macroDef, actualParams, nestLevel + 1);
            output.insert(output.end(), expanded.begin(), expanded.end());
            continue;
        }

        if (hasLabel && cmdIdx >= tokens.size()) {
            output.push_back(labelStr);
        } else if (cmdIdx < tokens.size()) {
            output.push_back(raw);
        }
    }

    for (const auto& def : defStack) {
        errors.push_back("Macro \"" + def.name + "\" not closed (ENDM missing)");
        cerr << "[MACRO] " << errors.back() << endl;
    }

    return output;
}


vector<string> MacroProcessor::ExpandMacro(const MacroDefinition& macro, const vector<string>& actualParams, int nestLevel) {
    macroScopes.emplace_back();

    if (actualParams.size() < macro.params.size()) {
        errors.push_back(
            "Macro \"" + macro.name + "\": expected " +
            to_string(macro.params.size()) + " argument(s), got " +
            to_string(actualParams.size())
        );
        cerr << "[MACRO] " << errors.back() << endl;
        macroScopes.pop_back();
        return {};
    }


    vector<string> substituted;
    substituted.reserve(macro.body.size());

    for (const string& bodyLine : macro.body) {
        string result = bodyLine;

        for (size_t p = 0; p < macro.params.size(); ++p) {
            const string& formal = macro.params[p];  
            const string& actual = actualParams[p];

            string out;
            out.reserve(result.size());
            size_t pos = 0;
            while (pos < result.size()) {
                size_t found = result.find(formal, pos);
                if (found == string::npos) {
                    out += result.substr(pos);
                    break;
                }
                out += result.substr(pos, found - pos);
                out += actual;
                pos = found + formal.size();
            }
            result = out;
        }
        substituted.push_back(result);
    }

    vector<string> result =
    ProcessLines(
        substituted,
        nestLevel
    );

    macroScopes.pop_back();

    return result;
}