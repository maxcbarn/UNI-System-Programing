#ifndef ASSEMBLER_HPP
#define ASSEMBLER_HPP

class Assembler
{
private:
    Assembler();
    inline static Assembler * assembler = nullptr;
public:
    ~Assembler();
    static Assembler * GetAssembler();
};
















#endif