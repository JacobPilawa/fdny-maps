import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.InstructionIterator;

/** List direct MOV stores whose destination mentions a structure-field offset. */
public class FieldStoreAudit extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] requested = getScriptArgs();
        if (requested.length == 0) {
            println("Usage: FieldStoreAudit.java <field-offset> [immediate-value]");
            return;
        }

        long fieldOffset = Long.decode(requested[0]);
        Long immediate = requested.length > 1 ? Long.decode(requested[1]) : null;
        String fieldHex = Long.toHexString(fieldOffset).toLowerCase();
        InstructionIterator instructions = currentProgram.getListing().getInstructions(true);
        while (instructions.hasNext() && !monitor.isCancelled()) {
            Instruction instruction = instructions.next();
            if (!instruction.getMnemonicString().startsWith("MOV")
                    || instruction.getNumOperands() < 2) {
                continue;
            }

            String destination = instruction.getDefaultOperandRepresentation(0).toLowerCase();
            if (!destination.contains("0x" + fieldHex) || !destination.contains("[")) {
                continue;
            }
            if (immediate != null && !hasImmediate(instruction, 1, immediate.longValue())) {
                continue;
            }

            Function function = getFunctionContaining(instruction.getAddress());
            println(instruction.getAddress() + " function="
                + (function == null ? "<none>" : function.getEntryPoint() + " " + function.getName())
                + " " + instruction);
        }
    }

    private boolean hasImmediate(Instruction instruction, int operand, long expected) {
        for (Object object : instruction.getOpObjects(operand)) {
            if (object instanceof ghidra.program.model.scalar.Scalar
                    && ((ghidra.program.model.scalar.Scalar) object).getValue() == expected) {
                return true;
            }
        }
        return false;
    }
}
