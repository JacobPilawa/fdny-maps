import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.InstructionIterator;

/** Print the original listing for explicitly requested function entries. */
public class ExportFunctionAssembly extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] requested = getScriptArgs();
        if (requested.length == 0) {
            println("Usage: ExportFunctionAssembly.java <entry-address> [entry-address ...]");
            return;
        }

        for (String text : requested) {
            String normalized = text.startsWith("0x") || text.startsWith("0X")
                ? text.substring(2) : text;
            Address address = toAddr(normalized);
            Function function = getFunctionAt(address);
            if (function == null) {
                println("NO FUNCTION at " + address);
                continue;
            }

            println("FUNCTION " + function.getEntryPoint() + " " + function.getName());
            InstructionIterator instructions = currentProgram.getListing()
                .getInstructions(function.getBody(), true);
            while (instructions.hasNext()) {
                Instruction instruction = instructions.next();
                StringBuilder operands = new StringBuilder();
                for (int index = 0; index < instruction.getNumOperands(); index++) {
                    if (index > 0) {
                        operands.append(", ");
                    }
                    operands.append(instruction.getDefaultOperandRepresentation(index));
                }
                println("  " + instruction.getAddress() + "  "
                    + instruction.getMnemonicString() + " " + operands);
            }
        }
    }
}
