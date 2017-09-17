#ifndef INCLUDED_GB_REGS
#define INCLUDED_GB_REGS

#include "gb_typeutils.h"

class RegBank
{
public:
    RegBank();

    // Getters for each of the individual byte regs
    ByteType A() const;
    ByteType B() const;
    ByteType C() const;
    ByteType D() const;
    ByteType E() const;
    ByteType H() const;
    ByteType L() const;

    // Getters for the word regs
    WordType BC() const;    
    WordType DE() const;    
    WordType HL() const;    
    WordType SP() const;    
    WordType PC() const;    
    
    // Setters for the individual regs
    void A(ByteType val);
    void B(ByteType val);
    void C(ByteType val);
    void D(ByteType val);
    void E(ByteType val);
    void H(ByteType val);
    void L(ByteType val);

    // Setters for the word regs
    void BC(WordType val);    
    void DE(WordType val);    
    void HL(WordType val);    
    void SP(WordType val);    
    void PC(WordType val);    
    WordType IncPC();
	void IncPCBy(WordType val);
    
    // Flag getters
    Flag flagZero() const;
    Flag flagSubtract() const;
    Flag flagHalfCarry() const;
    Flag flagCarry() const;

    // Flag setters
    void flagZero(bool on);
    void flagSubtract(bool on);
    void flagHalfCarry(bool on);
    void flagCarry(bool on);




private:
    typedef union
    {
        WordType word;
        struct {
            ByteType lo;
            ByteType hi;
        };
    } WordRegister;
    typedef ByteType ByteRegister;

    Flag d_fzero;
    Flag d_fsub;
    Flag d_fhc;
    Flag d_fc;
    ByteRegister d_regA;
    WordRegister d_regBC;
    WordRegister d_regDE;
    WordRegister d_regHL;
    WordRegister d_regSP;
    WordRegister d_regPC;

};


#endif