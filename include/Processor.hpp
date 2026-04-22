#ifndef PROCESSOR_HPP
#define PROCESSOR_HPP

class Processor {
    private:
        Processor();
        bool halt = false;
        inline static Processor * processor = nullptr;
    public:
        ~Processor();
        void Halt();
        void Reset();
};






#endif