#include "assembler/Tables.hpp"
#include "instructions/AdressingTypesFactoryInstructions.hpp"
#include "instructions/AdressingTypesInstructions.hpp"
#include <iostream>

Tables::~Tables() {
    opcodeTable.clear();
    registerTable.clear();
    tables = nullptr;
}


Tables::Tables(  ) {

}

Tables * Tables::GetTables() {
    if ( tables != nullptr ) {
        return tables;
    }
    tables = new Tables();
    return tables;
}

void Tables::CreateTables( INPUTADRESSINGTYPES addresingType ) {
    AdressingTypesInstructions * addresingTypeInstruction = AdressingTypesFactoryInstructions::GetAdressingTypesFactoryInstructions()->GetAdressingTypeInstructions( addresingType );
    opcodeTable.push_back( { NOP,        ( int )NOP,        addresingTypeInstruction->GetInstructionWordQuantity( NOP ),        "NOP"  } );
    opcodeTable.push_back( { HLT,        ( int )HLT,        addresingTypeInstruction->GetInstructionWordQuantity( HLT ),        "HALT" } );
    opcodeTable.push_back( { ADD,        ( int )ADD,        addresingTypeInstruction->GetInstructionWordQuantity( ADD ),        "ADD"  } );
    opcodeTable.push_back( { SUB,        ( int )SUB,        addresingTypeInstruction->GetInstructionWordQuantity( SUB ),        "SUB"  } );
    opcodeTable.push_back( { AND,        ( int )AND,        addresingTypeInstruction->GetInstructionWordQuantity( AND ),        "AND"  } );
    opcodeTable.push_back( { OR,         ( int )OR,         addresingTypeInstruction->GetInstructionWordQuantity( OR ),         "OR"   } );
    opcodeTable.push_back( { XOR,        ( int )XOR,        addresingTypeInstruction->GetInstructionWordQuantity( XOR ),        "XOR"  } );
    opcodeTable.push_back( { CP,         ( int )CP,         addresingTypeInstruction->GetInstructionWordQuantity( CP ),         "CP"   } );
    opcodeTable.push_back( { INC,        ( int )INC,        addresingTypeInstruction->GetInstructionWordQuantity( INC ),        "INC"  } );
    opcodeTable.push_back( { DEC,        ( int )DEC,        addresingTypeInstruction->GetInstructionWordQuantity( DEC ),        "DEC"  } );
    opcodeTable.push_back( { PUSH,       ( int )PUSH,       addresingTypeInstruction->GetInstructionWordQuantity( PUSH ),       "PUSH" } );
    opcodeTable.push_back( { POP,        ( int )POP,        addresingTypeInstruction->GetInstructionWordQuantity( POP ),        "POP"  } );
    opcodeTable.push_back( { JP,         ( int )JP,         addresingTypeInstruction->GetInstructionWordQuantity( JP ),         "JP"   } );
    opcodeTable.push_back( { JPOFFSET,   ( int )JPOFFSET,   addresingTypeInstruction->GetInstructionWordQuantity( JPOFFSET ),   "JR"   } );
    opcodeTable.push_back( { CALL,       ( int )CALL,       addresingTypeInstruction->GetInstructionWordQuantity( CALL ),       "CALL" } );
    opcodeTable.push_back( { RET,        ( int )RET,        addresingTypeInstruction->GetInstructionWordQuantity( RET ),        "RET"  } );
    opcodeTable.push_back( { LDREGTOREG, ( int )LDREGTOREG, addresingTypeInstruction->GetInstructionWordQuantity( LDREGTOREG ), "LD"   } );  
    opcodeTable.push_back( { LDVALTOREG, ( int )LDVALTOREG, addresingTypeInstruction->GetInstructionWordQuantity( LDVALTOREG ), "LD"   } );  
    opcodeTable.push_back( { LDREGTOMEM, ( int )LDREGTOMEM, addresingTypeInstruction->GetInstructionWordQuantity( LDREGTOMEM ), "LD"   } );  
    opcodeTable.push_back( { LDMEMTOREG, ( int )LDMEMTOREG, addresingTypeInstruction->GetInstructionWordQuantity( LDMEMTOREG ), "LD"   } );

    registerTable.push_back( { A,  ( int )A,  "A"  } );
    registerTable.push_back( { B,  ( int )B,  "B"  } );
    registerTable.push_back( { C,  ( int )C,  "C"  } );
    registerTable.push_back( { D,  ( int )D,  "D"  } );
    registerTable.push_back( { E,  ( int )E,  "E"  } );
    registerTable.push_back( { H,  ( int )H,  "H"  } );
    registerTable.push_back( { L,  ( int )L,  "L"  } );
    registerTable.push_back( { AF, ( int )AF, "AF" } );
    registerTable.push_back( { BC, ( int )BC, "BC" } );
    registerTable.push_back( { DE, ( int )DE, "DE" } );
    registerTable.push_back( { HL, ( int )HL, "HL" } );  
    registerTable.push_back( { PC, ( int )PC, "PC" } );
    registerTable.push_back( { SP, ( int )SP, "SP" } );
    registerTable.push_back( { IX, ( int )IX, "IX" } );
    registerTable.push_back( { IY, ( int )IY, "IY" } );
}

void Tables::ShowOpcodeTable() {
    cout << "OPCODE TABLE:" << endl;
    for ( auto & entry : opcodeTable ) {
        cout << entry.Show() << endl;
    }
}

void Tables::ShowRegisterTable() {
    cout << "REGISTER TABLE:" << endl;
    for ( auto & entry : registerTable ) {
        cout << entry.Show() << endl;
    }
}

