import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.InstructionIterator;

/** Find explicit instruction operands that mention a structure-field displacement. */
public class FieldOffsetReferences extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] requested = getScriptArgs();
        if (requested.length != 1) {
            println("Usage: FieldOffsetReferences.java <field-offset>");
            return;
        }

        long fieldOffset = Long.decode(requested[0]);
        String needle = "0x" + Long.toHexString(fieldOffset).toLowerCase();
        InstructionIterator instructions = currentProgram.getListing().getInstructions(true);
        while (instructions.hasNext() && !monitor.isCancelled()) {
            Instruction instruction = instructions.next();
            for (int operand = 0; operand < instruction.getNumOperands(); operand++) {
                String representation = instruction.getDefaultOperandRepresentation(operand)
                    .toLowerCase();
                if (representation.contains("[") && representation.contains(needle)) {
                    Function function = getFunctionContaining(instruction.getAddress());
                    println(instruction.getAddress() + " function="
                        + (function == null ? "<none>"
                            : function.getEntryPoint() + " " + function.getName())
                        + " " + instruction);
                    break;
                }
            }
        }
    }
}
