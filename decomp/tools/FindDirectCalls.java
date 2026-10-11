// Find decoded direct CALL instructions targeting one executable address.
// @category FDNY

import java.io.FileWriter;
import java.io.PrintWriter;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.InstructionIterator;

public class FindDirectCalls extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length != 2) {
            throw new IllegalArgumentException("usage: FindDirectCalls.java OUTPUT_FILE TARGET_ADDRESS");
        }
        Address target = toAddr(args[1].replace("0x", ""));
        int matches = 0;
        try (PrintWriter out = new PrintWriter(new FileWriter(args[0]))) {
            out.println("instruction\tfunction\tdisassembly");
            InstructionIterator instructions = currentProgram.getListing().getInstructions(true);
            while (instructions.hasNext()) {
                Instruction instruction = instructions.next();
                if (!instruction.getMnemonicString().startsWith("CALL")) continue;
                for (Address flow : instruction.getFlows()) {
                    if (target.equals(flow)) {
                        Function function = getFunctionContaining(instruction.getAddress());
                        out.println(instruction.getAddress() + "\t" +
                            (function == null ? "" : function.getEntryPoint()) + "\t" + instruction);
                        matches++;
                        break;
                    }
                }
            }
        }
        println("Found " + matches + " direct calls to " + target);
    }
}
