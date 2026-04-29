#include "FunctionalUnit.hpp"
#include "memory/Memory.hpp"
#include "memory/Registers.hpp"
#include <bit> 
#include <cstdint>

FunctionalUnit::FunctionalUnit() {

}

FunctionalUnit::~FunctionalUnit() {

}

void FunctionalUnit::Jump( Adress address ) {
    Registers::GetRegisters()->SetProgramCounter( address );
}

void FunctionalUnit::JumpOffset( Adress offset ) {
    Registers * regs = Registers::GetRegisters();
    Adress newAddress = regs->GetProgramCounter() + offset;
    regs->SetProgramCounter( newAddress );
}

void FunctionalUnit::Call( Adress address ) {
    PushStack( Registers::GetRegisters()->GetProgramCounter() );
    Jump( address );
}

void FunctionalUnit::Return() {
    PopStack( Registers::GetRegisters()->GetProgramCounter() );
    Jump( Registers::GetRegisters()->ReadFrom16bRegister( AF ) );
}


Word FunctionalUnit::Load( Adress address ) {
    return Memory::GetMemory()->ReadMemory( Registers::GetRegisters()->GetStartMemory() + address );
}

void FunctionalUnit::Store( Adress address , Word data ) {
    Memory::GetMemory()->ModifyMemory( Registers::GetRegisters()->GetStartMemory() + address , data );
}


FunctionalUnit * FunctionalUnit::GetFunctionalUnit() {
    if( functionalUnit != nullptr ) {
        return functionalUnit;
    }
    functionalUnit = new FunctionalUnit();
    return functionalUnit;
}

void FunctionalUnit::PushStack( REGISTERS_16b registerEnum ) {
    Memory * memory = Memory::GetMemory();
    Registers * registers = Registers::GetRegisters();
    registers->DecreaseStackPtr();
    memory->ModifyMemory( registers->GetStackPtr() , registers->ReadFrom16bRegisterLow( registerEnum ) );
    registers->DecreaseStackPtr();
    memory->ModifyMemory( registers->GetStackPtr() , registers->ReadFrom16bRegisterHigh( registerEnum ) );
}

void FunctionalUnit::PushStack( DoubleWord data ) {
    Memory * memory = Memory::GetMemory();
    Registers * registers = Registers::GetRegisters();
    registers->DecreaseStackPtr();
    memory->ModifyMemory( registers->GetStackPtr() , ( Word )( data & 0x00FF ) );
    registers->DecreaseStackPtr();
    memory->ModifyMemory( registers->GetStackPtr() , ( Word )( data >> 8 ) );
}

void FunctionalUnit::PopStack( REGISTERS_16b registerEnum ) {
    Memory * memory = Memory::GetMemory();
    Registers * registers = Registers::GetRegisters();
    registers->WriteTo16bRegisterHigh( registerEnum , memory->ReadMemory( registers->GetStackPtr() ) );
    registers->IncreaseStackPtr();
    registers->WriteTo16bRegisterLow( registerEnum , memory->ReadMemory( registers->GetStackPtr() ) );
    registers->IncreaseStackPtr();
}   

void FunctionalUnit::PopStack( DoubleWord data ) {
    Memory * memory = Memory::GetMemory();
    Registers * registers = Registers::GetRegisters();
    registers->WriteTo16bRegisterHigh( AF , ( Word )( data >> 8 ) );
    registers->IncreaseStackPtr();
    registers->WriteTo16bRegisterLow( AF , ( Word )( data & 0x00FF ) );
    registers->IncreaseStackPtr();
}

void FunctionalUnit::Nop() {
    return;
}

Word FunctionalUnit::Add( Word data1 , Word data2 ) {
    Registers * regs = Registers::GetRegisters();
    DoubleWord sum = ( DoubleWord )data1 + ( DoubleWord )data2;
    Word result = ( Word )sum;

    if( sum > 0xFF ) {
        regs->AddFlag( FLAGS::CARRY );
    } else {
        regs->ClearFlag( FLAGS::CARRY );
    }
    if( result == 0 ) {
        regs->AddFlag( FLAGS::ZERO );
    } else {
        regs->ClearFlag( FLAGS::ZERO );
    }
    if( result & 0x80 ) {
        regs->AddFlag( FLAGS::SIGN );
    } else {
        regs->ClearFlag( FLAGS::SIGN );
    }
    if( ( ( data1 & 0x0F ) + ( data2 & 0x0F ) ) > 0x0F ) {
        regs->AddFlag( FLAGS::HALF_CARRY );
    } else {
        regs->ClearFlag( FLAGS::HALF_CARRY );
    }
    if( ( ~( data1 ^ data2 ) & ( data1 ^ result ) & 0x80 ) != 0 ) {
        regs->AddFlag( FLAGS::PARITY_OVERFLOW );
    } else {
        regs->ClearFlag( FLAGS::PARITY_OVERFLOW );
    }
    regs->ClearFlag( FLAGS::ADD_SUBTRACT );
    return result;
}

Word FunctionalUnit::Sub( Word data1 , Word data2 ) {
    Registers * regs = Registers::GetRegisters();

    int sub = (int)data1 - (int)data2;
    Word result = (Word)sub;
    
    regs->AddFlag(FLAGS::ADD_SUBTRACT);

    if (result == 0) {
        regs->AddFlag(FLAGS::ZERO);
    } else {
        regs->ClearFlag(FLAGS::ZERO);
    }
    
    if (result & 0x80) {
        regs->AddFlag(FLAGS::SIGN);
    } else {
        regs->ClearFlag(FLAGS::SIGN);
    }
    
    if (data1 < data2) {
        regs->AddFlag(FLAGS::CARRY);
    } else {
        regs->ClearFlag(FLAGS::CARRY);
    }

    if ((data1 & 0x0F) < (data2 & 0x0F))
        regs->AddFlag(FLAGS::HALF_CARRY);
    else {
        regs->ClearFlag(FLAGS::HALF_CARRY);
    }
    if (((data1 ^ data2) & (data1 ^ result) & 0x80) != 0) {
        regs->AddFlag(FLAGS::PARITY_OVERFLOW);
    } else {
        regs->ClearFlag(FLAGS::PARITY_OVERFLOW);
    }

    return result;
}

Word FunctionalUnit::Inc( Word data1 ) {
    Registers * regs = Registers::GetRegisters();
    Word result = data1 + 1;

    if( result == 0 ) {
        regs->AddFlag( FLAGS::ZERO );
    } else {
        regs->ClearFlag( FLAGS::ZERO );
    }
    if( result & 0x80 ) {
        regs->AddFlag( FLAGS::SIGN );
    } else {
        regs->ClearFlag( FLAGS::SIGN );
    }
    if( ( ( data1 & 0x0F ) + 1 ) > 0x0F ) {
        regs->AddFlag( FLAGS::HALF_CARRY );
    } else {
        regs->ClearFlag( FLAGS::HALF_CARRY );
    }
    if( data1 == 0x7F ) {
        regs->AddFlag( FLAGS::PARITY_OVERFLOW );
    } else {
        regs->ClearFlag( FLAGS::PARITY_OVERFLOW );
    }
    regs->ClearFlag( FLAGS::CARRY );
    regs->ClearFlag( FLAGS::ADD_SUBTRACT );
    return result;
}

Word FunctionalUnit::Dec( Word data1 ) {
    Registers * regs = Registers::GetRegisters();
    Word result = data1 - 1;

    if( result == 0 ) {
        regs->AddFlag( FLAGS::ZERO );
    } else {
        regs->ClearFlag( FLAGS::ZERO );
    }
    if( result & 0x80 ) {
        regs->AddFlag( FLAGS::SIGN );
    } else {
        regs->ClearFlag( FLAGS::SIGN );
    }
    if( ( data1 & 0x0F ) == 0x00 ) {
        regs->AddFlag( FLAGS::HALF_CARRY );
    } else {
        regs->ClearFlag( FLAGS::HALF_CARRY );
    }
    if( data1 == 0x80 ) {
        regs->AddFlag( FLAGS::PARITY_OVERFLOW );
    } else {
        regs->ClearFlag( FLAGS::PARITY_OVERFLOW );
    }
    regs->AddFlag( FLAGS::CARRY );
    regs->AddFlag( FLAGS::ADD_SUBTRACT );
    return result;
}


Word FunctionalUnit::And( Word data1 , Word data2 ) {
    Registers * regs = Registers::GetRegisters();
    Word result = data1 & data2;
    if( result & 0b10000000 ) {
        regs->AddFlag(FLAGS::SIGN);
    } else {
        regs->ClearFlag(FLAGS::SIGN);
    }
    if (result == 0) {
        regs->AddFlag(FLAGS::ZERO);
    } else {
        regs->ClearFlag(FLAGS::ZERO);
    }
    regs->AddFlag(FLAGS::HALF_CARRY);
    regs->ClearFlag(FLAGS::ADD_SUBTRACT);
    regs->ClearFlag(FLAGS::CARRY);  
    if( EvenParity( result ) ) {
        regs->AddFlag(FLAGS::PARITY_OVERFLOW);
    } else {
        regs->ClearFlag(FLAGS::PARITY_OVERFLOW);
    }
    return result;
}

Word FunctionalUnit::Or( Word data1 , Word data2 ) {
    Registers * regs = Registers::GetRegisters();
    Word result = data1 | data2;
    if( result & 0b10000000 ) {
        regs->AddFlag(FLAGS::SIGN);
    } else {
        regs->ClearFlag(FLAGS::SIGN);
    }
    if (result == 0) {
        regs->AddFlag(FLAGS::ZERO);
    } else {
        regs->ClearFlag(FLAGS::ZERO);
    }
    regs->ClearFlag(FLAGS::HALF_CARRY);
    regs->ClearFlag(FLAGS::ADD_SUBTRACT);
    regs->ClearFlag(FLAGS::CARRY);  
    if( EvenParity( result ) ) {
        regs->AddFlag(FLAGS::PARITY_OVERFLOW);
    } else {
        regs->ClearFlag(FLAGS::PARITY_OVERFLOW);
    }
    return result;
}

Word FunctionalUnit::Xor( Word data1 , Word data2 ) {
    Registers * regs = Registers::GetRegisters();
    Word result = data1 ^ data2;
    if( result & 0b10000000 ) {
        regs->AddFlag(FLAGS::SIGN);
    } else {
        regs->ClearFlag(FLAGS::SIGN);
    }
    if (result == 0) {
        regs->AddFlag(FLAGS::ZERO);
    } else {
        regs->ClearFlag(FLAGS::ZERO);
    }
    regs->ClearFlag(FLAGS::HALF_CARRY);
    regs->ClearFlag(FLAGS::ADD_SUBTRACT);
    regs->ClearFlag(FLAGS::CARRY);  
    if( EvenParity( result ) ) {
        regs->AddFlag(FLAGS::PARITY_OVERFLOW);
    } else {
        regs->ClearFlag(FLAGS::PARITY_OVERFLOW);
    }
    return result;
}

void FunctionalUnit::Cp( Word data1 , Word data2 ) {
    Registers * regs = Registers::GetRegisters();
    Word result = data1 - data2;
    //if( result & 0x10000000 )
     if( result & 0x80 ) {
        regs->AddFlag(FLAGS::SIGN);
    } else {
        regs->ClearFlag(FLAGS::SIGN);
    }
    if (result == 0) {
        regs->AddFlag(FLAGS::ZERO);
    } else {
        regs->ClearFlag(FLAGS::ZERO);
    }
    regs->AddFlag(FLAGS::ADD_SUBTRACT);
    if ((data1 & 0x0F) < (data2 & 0x0F)) {
        regs->AddFlag(FLAGS::HALF_CARRY);
    } else {
        regs->ClearFlag(FLAGS::HALF_CARRY);
    }
    if (((data1 ^ data2) & (data1 ^ result) & 0x80) != 0) {
        regs->AddFlag(FLAGS::PARITY_OVERFLOW);
    } else {
        regs->ClearFlag(FLAGS::PARITY_OVERFLOW);
    }
    if (data1 < data2) {
        regs->AddFlag(FLAGS::CARRY);
    } else {
        regs->ClearFlag(FLAGS::CARRY);
    }
    return;
}
