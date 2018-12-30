#pragma once

#include "gb_typeutils.h"
#include "ReverseOpcodeMap.h"

class OpcodeResultContext
{

public:
	enum PCAction
	{
		FREEZE,
		EXPLICIT_SET,
		DEFAULT_INC,
		EXPLICIT_INC
	};

	struct Properties
	{
		bool isCB;
		OpcodeMetadata metadata;
		ByteType opcode;
		CycleAction action;
		ByteType cycles;
		ByteType pcIncrement;
		WordType pcSet;

		enum PCAction pc_action_taken;
	};

	Properties properties;
	OpcodeResultContext(const Properties &props) : properties(props)
	{}
	
	class Builder
	{
	public:
		Builder(ByteType opcode, bool isCB = false)
		{
			props.opcode = opcode;
			if ( (props.isCB = isCB) ) {
				props.metadata = INSTR_CB_META[opcode];
			}
			else {
				props.metadata = INSTR_META[opcode];
			}
		}

		Builder& LongCycle()
		{
			props.action = CYCLE_TAKEN;
			props.cycles = props.metadata.cyclesTaken;
			if (props.isCB) props.cycles += 4;
			return *this;
		}

		Builder& ShortCycle()
		{
			props.action = CYCLE_UNTAKEN;
			props.cycles = props.metadata.cyclesUntaken;
			if (props.isCB) props.cycles += 4;
			return *this;
		}

		Builder& IncrementPCDefault()
		{
			props.pcIncrement = 0;
			props.pc_action_taken = PCAction::DEFAULT_INC;
			return *this;
		}

		Builder& IncrementPCBy(ByteType pcInc)
		{
			props.pcIncrement = pcInc;
			props.pc_action_taken = PCAction::EXPLICIT_INC;
			return *this;
		}

		Builder& FreezePC()
		{
			// When called, will not change the value of the PC
			props.pcIncrement = 0;
			props.pc_action_taken = PCAction::FREEZE;
			return *this;
		}

		Builder& SetPCTo(WordType pc)
		{
			props.pcSet = pc;
			props.pc_action_taken = PCAction::EXPLICIT_SET;
			return *this;
		}

		OpcodeResultContext Build()
		{
			return OpcodeResultContext(props);
		}


	private:
		Properties props;
	};

};